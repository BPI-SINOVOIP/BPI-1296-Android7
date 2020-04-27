/*
 *  ion.c
 *
 * Memory Allocator functions for ion
 *
 *   Copyright 2011 Google, Inc
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
//#define LOG_NDEBUG 0
#if defined(__LINARO_SDK__)
#include <generic_misc.h>
#else
#define LOG_TAG "ion"
#include <utils/Log.h>
#endif

#include <utils/backtrace.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <ion/ion.h>

#include <sys/socket.h>
#include <strings.h>
#include <sys/un.h> 

#define UNIX_DOMAIN  "/tmp/UNIX.domain"  

static int g_ion_fd = -1;

int ion_open()
{
    int fd = open("/dev/ion", O_RDWR);
    if (fd < 0)
        ALOGE("open /dev/ion failed!\n");
    return fd;
}

int ion_close(int fd)
{
    int ret = close(fd);
    if (ret < 0)
        return -errno;
    return ret;
}

static int ion_ioctl(int fd, int req, void *arg)
{
    int ret = ioctl(fd, req, arg);
    if (ret < 0) {
        ALOGE("ioctl %x failed with code %d: %s\n", req, ret, strerror(errno));
        backtrace(LOG_TAG);
        return -errno;
    }
    return ret;
}

int ion_alloc(int fd, size_t len, size_t align, unsigned int heap_mask,
              unsigned int flags, ion_user_handle_t *handle)
{
        int ret;
        struct ion_allocation_data data = {
                .len = len,
                .align = align,
                .heap_id_mask = heap_mask,
                .flags = flags,
        };

        if (handle == NULL)
            return -EINVAL;

        ret = ion_ioctl(fd, ION_IOC_ALLOC, &data);
        if (ret < 0){
                ALOGE("ION_IOC_ALLOC failed: fd:%d len:%zu align:%zu heap:%x flags:%x", fd, len, align, heap_mask, flags);
                return ret;
        }
        *handle = data.handle;
//        ALOGV("%s handle:%p\n", __func__, *handle);
        return ret;
}

int ion_alloc_tiler(int fd, size_t w, size_t h, int fmt, unsigned int size, unsigned int flags,
            ion_user_handle_t *handle, size_t *stride)
{
        int ret;
        struct rtk_phoenix_ion_tiler_alloc_data alloc_data = {
                .w = w,
                .h = h,
                .fmt = fmt,
                .flags = flags,
                .size = size,
        };

        struct ion_custom_data custom_data = {
                .cmd = RTK_PHOENIX_ION_TILER_ALLOC,
                .arg = (unsigned long)(&alloc_data),
        };

        ret = ion_ioctl(fd, ION_IOC_CUSTOM, &custom_data);
        if (ret < 0)
                return ret;
        *stride = alloc_data.stride;
        *handle = alloc_data.handle;

        return ret;
}

int ion_free(int fd, ion_user_handle_t handle)
{
    struct ion_handle_data data = {
        .handle = handle,
    };
    int ret = ion_ioctl(fd, ION_IOC_FREE, &data);
    if(ret < 0)
        ALOGE("ION_IOC_FREE failed: fd:%d handle:%x", fd, handle);

    return ret;
}

int ion_map(int fd, ion_user_handle_t handle, size_t length, int prot,
            int flags, off64_t offset, unsigned char **ptr, int *map_fd)
{
    int ret;
    unsigned char *tmp_ptr;
    struct ion_fd_data data = {
        .handle = handle,
    };

    if (map_fd == NULL)
        return -EINVAL;
    if (ptr == NULL)
        return -EINVAL;

    ret = ion_ioctl(fd, ION_IOC_MAP, &data);
    if (ret < 0){
        ALOGE("ION_IOC_MAP failed: fd:%d handle:%d length:%zu prot:%x flags:%x offset:%" PRIx64, fd, handle, length, prot, flags, offset);
        return ret;
    }
    if (data.fd < 0) {
        ALOGE("map ioctl returned negative fd\n");
        return -EINVAL;
    }
    tmp_ptr = mmap64(NULL, length, prot, flags, data.fd, offset);
    if (tmp_ptr == MAP_FAILED) {
        ALOGE("mmap failed: %s\n", strerror(errno));
        return -errno;
    }
    *map_fd = data.fd;
    *ptr = tmp_ptr;
    return ret;
}

int ion_share(int fd, ion_user_handle_t handle, int *share_fd)
{
    int ret;
    struct ion_fd_data data = {
        .handle = handle,
    };

    if (share_fd == NULL)
        return -EINVAL;

    ret = ion_ioctl(fd, ION_IOC_SHARE, &data);
    if (ret < 0){
        ALOGE("ION_IOC_SHARE failed: fd:%d handle:%d", fd, handle);
        return ret;
    }
    if (data.fd < 0) {
        ALOGE("share ioctl returned negative fd\n");
        return -EINVAL;
    }
    *share_fd = data.fd;
    return ret;
}

int ion_alloc_fd(int fd, size_t len, size_t align, unsigned int heap_mask,
                 unsigned int flags, int *handle_fd) {
    ion_user_handle_t handle;
    int ret;

    ret = ion_alloc(fd, len, align, heap_mask, flags, &handle);
    if (ret < 0)
        return ret;
    ret = ion_share(fd, handle, handle_fd);
    ion_free(fd, handle);
    return ret;
}

int ion_import(int fd, int share_fd, ion_user_handle_t *handle)
{
    int ret;
    struct ion_fd_data data = {
        .fd = share_fd,
    };

    if (handle == NULL)
        return -EINVAL;

    ret = ion_ioctl(fd, ION_IOC_IMPORT, &data);
    if (ret < 0)
        ALOGE("ION_IOC_IMPORT failed: fd:%d share_fd:%d", fd, share_fd);
    else
        *handle = data.handle;

    return ret;
}

int ion_sync_fd(int fd, int handle_fd)
{
    struct ion_fd_data data = {
        .fd = handle_fd,
    };

    int ret = ion_ioctl(fd, ION_IOC_SYNC, &data);
    if(ret < 0)
        ALOGE("ION_IOC_SYNC failed: fd:%d handle_fd:%d", fd, handle_fd);

    return ret;
}

#if 1 //20130208 charleslin
int ion_phys(int fd, ion_user_handle_t handle, unsigned long *addr, unsigned int *size)
{
    int ret;
    struct ion_phys_data data = {
        .handle = handle,
        .addr = 0,
        .len = 0,
    };
    ret = ion_ioctl(fd, ION_IOC_PHYS, &data);
    if (ret < 0){
        ALOGE("ION_IOC_PHYS failed: fd:%d handle:%d", fd, handle);
        return ret;
    }
    *addr = data.addr;
    *size = data.len;
//    ALOGV("%s handle:%p addr:%lx size:%x\n", __func__, handle, data.addr, data.len);
    return ret;
}

int ion_last_alloc_addr(int fd, unsigned long *addr, unsigned int *_size)
{
    int ret;
    unsigned int buf[2];//alloc_addr;
    struct ion_custom_data custom_data = {
                .cmd = RTK_PHOENIX_ION_GET_LAST_ALLOC_ADDR,
//                .arg = (unsigned long)(&alloc_addr),
                .arg = (unsigned long)(&buf[0]),
        };

        ret = ion_ioctl(fd, ION_IOC_CUSTOM, &custom_data);
        if (ret < 0)
                return ret;
//        *addr = alloc_addr;
        ALOGV("in ion_last_alloc_addr  buf[0]=0x%x buf[1]=0x%x\n", buf[0], buf[1]);
        *addr = buf[0];
        *_size = buf[1];

        return ret;
}

//used to get physical address of ion memory by shared fd
int ion_get_phys_from_shared_fd(int share_fd, unsigned long *addr, unsigned int *size)
{
    int ret;
    ion_user_handle_t handle;

    if(g_ion_fd == -1)
        g_ion_fd = ion_open();
    if(g_ion_fd < 0)
        return g_ion_fd;
    ALOGV("g_ion_fd:%d heapID:%d", g_ion_fd, share_fd);

    ret = ion_import(g_ion_fd, share_fd, &handle);
    if(ret != 0)
        return ret;
//    ALOGV("handle:%p", handle);

    ret = ion_phys(g_ion_fd, handle, addr, size);
    if(ret != 0)
        return ret;
    ALOGV("get ion memory addr:%lx size:%d", *addr, *size);

    return 0;
}

int ion_flush_fd(int fd, int handle_fd)
{
    struct ion_custom_data custom_data = {
        .cmd = RTK_ION_IOC_FLUSH,
        .arg = (unsigned long) handle_fd,
    };

    int ret = ion_ioctl(fd, ION_IOC_CUSTOM, &custom_data);

    if(ret < 0)
        ALOGE("RTK_ION_IOC_FLUSH failed: fd:%d handle_fd:%d", fd, handle_fd);

    return ret;
}

int ion_invalidate_fd(int fd, int handle_fd)
{
    struct ion_custom_data custom_data = {
        .cmd = RTK_ION_IOC_INVALIDATE,
        .arg = (unsigned long) handle_fd,
    };

    int ret = ion_ioctl(fd, ION_IOC_CUSTOM, &custom_data);

    if(ret < 0)
        ALOGE("RTK_ION_IOC_INVALIDATE failed: fd:%d handle_fd:%d", fd, handle_fd);

    return ret;
}

int ion_flush_range(int fd, int handle_fd, unsigned long addr, unsigned int size)
{
    struct RTK_ION_IOC_SYNC_RANE sync_range = {
        .handle = handle_fd,
        .phyAddr = addr,
        .len = size,
    };
    struct ion_custom_data custom_data = {
        .cmd = RTK_ION_IOC_FLUSH_RANGE,
        .arg = (unsigned long)&sync_range,
    };
    int ret = ion_ioctl(fd, ION_IOC_CUSTOM, &custom_data);

    if(ret < 0) {
        ALOGE("RTK_ION_IOC_FLUSH_RANGE failed: fd:%d handle_fd:%d", fd, handle_fd);
        ion_flush_fd(fd, handle_fd);
    }

    return ret;
}

int ion_invalidate_range(int fd, int handle_fd, unsigned long addr, unsigned int size)
{
    struct RTK_ION_IOC_SYNC_RANE sync_range = {
        .handle = handle_fd,
        .phyAddr = addr,
        .len = size,
    };
    struct ion_custom_data custom_data = {
        .cmd = RTK_ION_IOC_INVALIDATE_RANGE,
        .arg = (unsigned long)&sync_range,
    };
    int ret = ion_ioctl(fd, ION_IOC_CUSTOM, &custom_data);

    if(ret < 0) {
        ALOGE("RTK_ION_IOC_INVALIDATE_RANGE failed: fd:%d handle_fd:%d", fd, handle_fd);
        ion_invalidate_fd(fd, handle_fd);
    }

    return ret;
}

int ion_send_share_fd(int socket, int sharefd) // send fd by socket
{
	struct msghdr msg;
	memset(&msg, 0x00, sizeof(struct msghdr));
	char buf[CMSG_SPACE(sizeof(sharefd))];
	memset(buf, '\0', sizeof(buf));
	char my_data[]="ABC";
	struct iovec io = { .iov_base = my_data, .iov_len = 3 };
	int ret;

	msg.msg_iov = &io;
	msg.msg_iovlen = 1;
	msg.msg_control = buf;
	msg.msg_controllen = sizeof(buf);
	struct cmsghdr * cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof(sharefd));
	*((int *) CMSG_DATA(cmsg)) = sharefd;
	msg.msg_controllen = cmsg->cmsg_len;
	ret=sendmsg(socket, &msg, 0);
	if (ret < 0){
		ALOGE("Failed to send message\n");
		return ret;
	}
	return 0;
}

int ion_receive_share_fd(int socketfd)	// receive fd from socket
{
	struct msghdr msg;
	memset(&msg, 0x00, sizeof(struct msghdr));

	char recv_buffer[256];
	struct iovec io = { .iov_base = recv_buffer, .iov_len = sizeof(recv_buffer) };
	msg.msg_iov = &io;
	msg.msg_iovlen = 1;

	char extra_buffer[256];
	msg.msg_control = extra_buffer;
	msg.msg_controllen = sizeof(extra_buffer);

	if (recvmsg(socketfd, &msg, 0) < 0)
		ALOGE("Failed to receive message\n");

	struct cmsghdr * cmsg = CMSG_FIRSTHDR(&msg);
	unsigned char * data = (unsigned char *)CMSG_DATA(cmsg);
//	printf("About to extract fd\n");
	int fd = *((int*) data);
	ALOGE("Extracted fd %d\n", fd);

	return fd;
}


int ion_creat_client_socket(int *socketfd)
{
		int connect_fd;  
		int ret;  
		char snd_buf[100];	
		
		static struct sockaddr_un srv_addr;  
	//creat unix socket  
		connect_fd = socket(PF_UNIX,SOCK_STREAM,0);  
		ALOGE("\nion soket=%d\n",connect_fd);
		if(connect_fd < 0)	
		{  
			ALOGE("can't create communication socket!\n");	
			return connect_fd;	
		}	  
		srv_addr.sun_family=AF_UNIX; 
		strcpy(srv_addr.sun_path,UNIX_DOMAIN);	
	//connect server  
		ret=connect(connect_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));  
		if(ret < 0)  
		{  
			ALOGE("[kjj]can't connect to the server!\n");  
			close(connect_fd);	
			return ret; 
		}  
		memset(snd_buf,0,100);	
		strcpy(snd_buf,"message from client!\n");	
	//send info server	
		write(connect_fd,snd_buf,sizeof(snd_buf));	 
		*socketfd= connect_fd;
		return 0;
}
int ion_close_client_soket(int socket)
{
	int ret;
	ret=close(socket);
	if(ret < 0)
		ALOGE("Close Client Soket fail!\n");
	ret=unlink(UNIX_DOMAIN);
	if(ret < 0)
		ALOGE("Close Client unlink Soket fail!\n");
	return 0;
}

	
void *IonGetShareMemory(int *IonFd, int sharefd, int *size ,int *IonMapFd,void **noncacheable, unsigned long *PhyAddr,ion_user_handle_t *IonHandle)
{

	int 				  ret;
	int 				  mIonFd;
	int 				  mIonMmapFd;
	int 				  import_fd;
	ion_user_handle_t	  mIonHandle;
	void *				  mViraddr;
	unsigned long		  mPhyaddr;
	unsigned int		  mSize;

	mIonFd = ion_open();
	if(mIonFd < 0)
	{
		ALOGE("open ion fail!\n");
		return NULL;
	}
	ret=ion_import(mIonFd, sharefd, &mIonHandle);
	if(ret < 0)
	{
		ALOGE("android ion_import fail!\n");
		return NULL;
	}	   
	ret=ion_phys(mIonFd, mIonHandle,&mPhyaddr, &mSize);
	if(ret < 0)
	   ALOGE("android ion_phys fail!\n");

	ret = ion_map(mIonFd, mIonHandle, mSize , PROT_READ | PROT_WRITE,
			MAP_SHARED, 0, (unsigned char **)noncacheable, &mIonMmapFd);
	if(ret < 0)
	   ALOGE("android ion_map fail!\n");

	mViraddr   = *noncacheable;
	*IonFd	   = mIonFd;
	*IonMapFd  = mIonMmapFd;
	*size	   = mSize;
	*PhyAddr   = mPhyaddr;
	*IonHandle = mIonHandle;
	return mViraddr;
}

	
void *IonAllocMemoryFromShareFd(int *IonFd, int *sharefd, int socket , int *size ,int *IonMapFd,void **noncacheable, unsigned long *PhyAddr,ion_user_handle_t *IonHandle)
{

	int 				  ret;
	int 				  mIonFd;
	int 				  mIonMmapFd;
	int 				  import_fd;
	ion_user_handle_t	  mIonHandle;
	void *				  mViraddr;
	unsigned long		  mPhyaddr;
	unsigned int		  mSize;

	mIonFd = ion_open();
		
	import_fd = ion_receive_share_fd(socket);
	
	ALOGE("Read import_fd=%d,mIonFD=%d!\n", import_fd,mIonFd);
	
	ret=ion_import(mIonFd, import_fd, &mIonHandle);
	if(ret < 0)
	{
		ALOGE("android ion_import fail!\n");
		return NULL;
	}	   
	ret=ion_phys(mIonFd, mIonHandle,&mPhyaddr, &mSize);
	if(ret < 0)
	   ALOGE("android ion_phys fail!\n");

	ret = ion_map(mIonFd, mIonHandle, mSize , PROT_READ | PROT_WRITE,
			MAP_SHARED, 0, (unsigned char **)noncacheable, &mIonMmapFd);
	if(ret < 0)
	   ALOGE("android ion_map fail!\n");
	mViraddr   = *noncacheable;
	*IonFd	   = mIonFd;
	*sharefd   = import_fd;
	*IonMapFd  = mIonMmapFd;
	*size	   = mSize;
	*PhyAddr   = mPhyaddr;
	*IonHandle = mIonHandle;
	return mViraddr;
}

int IonFreeShareMemory(void *VirAddr,int sharefd,int size,int IonFd, int IonMapFd,ion_user_handle_t IonHandle)
{
//	printf("@@@@@@mIonMmapFd=%d,sharefd=%d,VirAddr=0x%x,mSize=%d, *mViraddr=%d ionfd=%d,IonHandle=%d@@@\n",IonMapFd,sharefd,VirAddr,size,*(unsigned long *)VirAddr,IonFd,IonHandle);
	close(sharefd);
	close(IonMapFd);
	munmap(VirAddr, size);
	ion_free(IonFd, IonHandle);
	close(IonFd);
	return 0;
}

int IonPutShareMemory(void *VirAddr,int size,int IonFd, int IonMapFd,ion_user_handle_t IonHandle)
{
	close(IonMapFd);
	munmap(VirAddr, size);
	ion_free(IonFd, IonHandle);
	close(IonFd);
	return 0;
}

#endif
