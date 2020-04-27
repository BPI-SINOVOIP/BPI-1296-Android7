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

#ifndef __SYS_CORE_ION_H
#define __SYS_CORE_ION_H

#include <linux/ion.h>
#include <linux/rtk_phoenix_ion.h>

__BEGIN_DECLS

int ion_open();
int ion_close(int fd);
int ion_alloc(int fd, size_t len, size_t align, unsigned int heap_mask,
	      unsigned int flags, ion_user_handle_t *handle);
int ion_alloc_fd(int fd, size_t len, size_t align, unsigned int heap_mask,
		 unsigned int flags, int *handle_fd);
int ion_sync_fd(int fd, int handle_fd);
int ion_free(int fd, ion_user_handle_t handle);
int ion_map(int fd, ion_user_handle_t handle, size_t length, int prot,
            int flags, off64_t offset, unsigned char **ptr, int *map_fd);
int ion_share(int fd, ion_user_handle_t handle, int *share_fd);
int ion_import(int fd, int share_fd, ion_user_handle_t *handle);
#if 1
int ion_alloc_tiler(int fd, size_t w, size_t h, int fmt, unsigned int size, unsigned int flags,
            ion_user_handle_t *handle, size_t *stride);
int ion_phys(int fd, ion_user_handle_t handle, unsigned long *addr, unsigned int *size);
int ion_last_alloc_addr(int fd, unsigned long *addr, unsigned int *size);
int ion_get_phys_from_shared_fd(int share_fd, unsigned long *addr, unsigned int *size);
int ion_flush_fd(int fd, int handle_fd);
int ion_invalidate_fd(int fd, int handle_fd);
int ion_flush_range(int fd, int handle_fd, unsigned long addr, unsigned int size);
int ion_invalidate_range(int fd, int handle_fd, unsigned long addr, unsigned int size);

int ion_send_share_fd(int socket, int sharefd);
int ion_receive_share_fd(int socketfd);
int ion_creat_client_socket(int *socketfd);
int ion_close_client_soket(int socket);

void *IonAllocMemoryFromShareFd(int *IonFd, int *sharefd, int socket , int *size ,int *IonMapFd,void **noncacheable, unsigned long *PhyAddr,ion_user_handle_t *IonHandle);
void *IonGetShareMemory(int *IonFd, int sharefd, int *size ,int *IonMapFd,void **noncacheable, unsigned long *PhyAddr,ion_user_handle_t *IonHandle);
int IonFreeShareMemory(void *VirAddr,int sharefd,int size,int IonFd, int IonMapFd,ion_user_handle_t IonHandle);
int IonPutShareMemory(void *VirAddr,int size,int IonFd, int IonMapFd,ion_user_handle_t IonHandle);

#endif
__END_DECLS

#endif /* __SYS_CORE_ION_H */
