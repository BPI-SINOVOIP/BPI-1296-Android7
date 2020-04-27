#define LOG_NDEBUG 0
#define LOG_TAG "rtksmblib"

#include <cutils/log.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#include "rtkini/rtk_ini.h"
#include "rtksmblib.h"


#define RAMFS_PATH "/tmp/ramfs"
#define FILE_PATH "/tmp/ramfs/smb"
#define MOUNT_ROOT "/tmp/ramfs/mnt"

#define SMB_EXE "/system/bin/rtksmb"
#define FILE_SIZE 64
#define CMD_SIZE 512

#define FREE(p) {if(p){free((void *)p); p=NULL;}}
#define DELETE(p) {if(p){delete p; p=NULL;}}

#define DEBUG
#define SMB_LOG(fmt, ...)               __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, fmt, ##__VA_ARGS__)
static int id=0;

static pthread_mutex_t id_mutex;
class Init
{
public:
    Init(){
        pthread_mutex_init(&id_mutex, NULL);
        SMB_LOG("RTKSMB Init");
        if(access(RAMFS_PATH, R_OK | W_OK | F_OK ))
    	{
    		SMB_LOG("Create Folder /tmp/ramfs");
            int ret = mkdir(RAMFS_PATH, S_IRWXU | S_IRWXG | (S_IROTH|S_IXOTH)); // create FILE_PATH
            if (ret ==-1)
            {
			    SMB_LOG("mkdir err = %s", strerror(errno));
            }
            ret = chmod(RAMFS_PATH, S_IRWXU | S_IRWXG | (S_IROTH|S_IXOTH));
            if (ret ==-1)
            {
			    SMB_LOG("chmod err = %s", strerror(errno));
            }
                
        }
        if(access(FILE_PATH, R_OK | W_OK | F_OK ))
    	{
    		SMB_LOG("Create Folder /tmp/ramfs/smb");
            int ret = mkdir(FILE_PATH, S_IRWXU | S_IRWXG | (S_IROTH|S_IXOTH)); // create FILE_PATH
            if (ret ==-1)
            {
			    SMB_LOG("mkdir err = %s", strerror(errno));
            }
        }
		if(access(MOUNT_ROOT, R_OK | W_OK | F_OK ))
    	{
    		SMB_LOG("Create Folder /tmp/ramfs/mnt");
            int ret = mkdir(MOUNT_ROOT, S_IRWXU | S_IRWXG | (S_IROTH|S_IXOTH)); // create MOUNT_ROOT
            if (ret ==-1)
            {
			    SMB_LOG("mkdir err = %s", strerror(errno));
            }
            ret = chmod(MOUNT_ROOT, S_IRWXU | S_IRWXG | (S_IROTH|S_IXOTH) );
            if (ret ==-1)
            {
			    SMB_LOG("chmod err = %s", strerror(errno));
            }
        }
	}
    ~Init(){pthread_mutex_destroy(&id_mutex);}
};

static Init a;

int getUniqueId()
{
    pthread_mutex_lock(&id_mutex);
    id++;
    pthread_mutex_unlock(&id_mutex);
    return id;
}

// get IP from NetBios Name
int Samba_NetBiosName2Ip(char *NetBiosName, struct in_addr *ip)
{
    int ret=-1;
    int id;
    char inFile[FILE_SIZE],outFile[FILE_SIZE],executeCmd[CMD_SIZE];
    Rtk_Ini *inIniObj=new Rtk_Ini();
    Rtk_Ini *outIniObj=new Rtk_Ini();
    id=getUniqueId();
    sprintf(inFile,"%s/%d.in",FILE_PATH,id);
    sprintf(outFile,"%s/%d.out",FILE_PATH,id);
    sprintf(executeCmd,"%s %s %s",SMB_EXE,inFile,outFile);
// prepare infile    
    inIniObj->setIntValue(MAIN, COMMAND,1);
    inIniObj->setStrValue(MAIN, P1,NetBiosName);
    inIniObj->saveFile(inFile);
    delete inIniObj;
// execute
    system(executeCmd);
// get ip
    if(!outIniObj->loadFile(outFile))
    {
        int rec_num;
        u_int64_t ip_value=0;
        if(!outIniObj->getIntValue(MAIN,NUMBER,&rec_num))
        {
            if(rec_num>0)
            {
                if(!outIniObj->getU64IntValue(MAIN, "1",&ip_value))
                {
                    ip->s_addr=(unsigned int)ip_value;
                    ret=0;
                }
            }
        }
    }
    delete outIniObj;
#ifndef DEBUG
    unlink(inFile);
    unlink(outFile);
#endif    
    return ret;
}
// Query Domain 
int Samba_QueryDomainList(void **Handle,int *num)
{
    int ret=-1;
    int id;
    char inFile[FILE_SIZE],outFile[FILE_SIZE],executeCmd[CMD_SIZE];
    Rtk_Ini *inIniObj=new Rtk_Ini();
    Rtk_Ini *outIniObj=new Rtk_Ini();
    id=getUniqueId();
    sprintf(inFile,"%s/%d.in",FILE_PATH,id);
    sprintf(outFile,"%s/%d.out",FILE_PATH,id);
    sprintf(executeCmd,"%s %s %s",SMB_EXE,inFile,outFile);
// prepare infile    
    inIniObj->setIntValue(MAIN, COMMAND,2);
    inIniObj->saveFile(inFile);
    delete inIniObj;
// execute
    system(executeCmd);
// get ip
    *Handle = NULL;
    if(!outIniObj->loadFile(outFile))
    {
        int rec_num;
        if(!outIniObj->getIntValue(MAIN,NUMBER,&rec_num))
        {
            if(rec_num<0)
                delete outIniObj;
            else
            {
                *num=rec_num;
                *Handle=(void *)outIniObj;
                outIniObj->removeInsideValueStrMark();
                ret=0;
            }
        }
        else
            delete outIniObj;
    }
    else
        delete outIniObj;
#ifndef DEBUG
    unlink(inFile);
    unlink(outFile);
#endif    
    return ret;
}
int Samba_GetDomainList(void *Handle,int num,char **Domain)
{
    int ret=-1;
    if(Handle)
    {
        Rtk_Ini *outIniObj;
        char *cOutput=NULL;
        char StrNum[32];
        sprintf(StrNum,"%d",num);
        outIniObj=(Rtk_Ini *)Handle;
        if((cOutput=outIniObj->getStrValue(MAIN,StrNum))!=NULL)
        {
            *Domain=cOutput;
            ret=0;
        }
    }
    return ret;
}
// Query Server
int Samba_QueryServerList(void **Handle,int *num,char *domain)
{
    int ret=-1;
    if(domain)
    {
        int id;
        char inFile[FILE_SIZE],outFile[FILE_SIZE],executeCmd[CMD_SIZE];
        Rtk_Ini *inIniObj=new Rtk_Ini();
        Rtk_Ini *outIniObj=new Rtk_Ini();
        id=getUniqueId();
        sprintf(inFile,"%s/%d.in",FILE_PATH,id);
        sprintf(outFile,"%s/%d.out",FILE_PATH,id);
        sprintf(executeCmd,"%s %s %s",SMB_EXE,inFile,outFile);
    // prepare infile    
        char *domain_withmark;
        inIniObj->setIntValue(MAIN, COMMAND,3);
        if(!inIniObj->addStrMark_needfree(domain, &domain_withmark))
        {
            inIniObj->setStrValue(MAIN, P1,domain_withmark);
            FREE(domain_withmark);
        }
        else
            inIniObj->setStrValue(MAIN, P1,domain);
        inIniObj->saveFile(inFile);
        delete inIniObj;
    // execute
        system(executeCmd);
    // get ip
        *Handle=NULL;
        if(!outIniObj->loadFile(outFile))
        {
            int rec_num;
            if(!outIniObj->getIntValue(MAIN,NUMBER,&rec_num))
            {
                if(rec_num<0)
                    delete outIniObj;
                else
                {
                    *num=rec_num;
                    *Handle=(void *)outIniObj;
                    outIniObj->removeInsideValueStrMark();
                    ret=0;
                }
            }
            else
                delete outIniObj;
        }
        else
            delete outIniObj;
#ifndef DEBUG
        unlink(inFile);
        unlink(outFile);
#endif    
    }
    return ret;
}
int Samba_GetServerList(void *Handle,int num,char **Server)
{
    return Samba_GetDomainList(Handle,num,Server);
}
// free Domain or Server's Handle
int Samba_freeHandle(void *Handle)
{
    int ret=-1;
    if(Handle)
    {
        Rtk_Ini *outIniObj;
        outIniObj=(Rtk_Ini *)Handle;
		SMB_LOG("delete handle = %p", Handle);
        delete outIniObj;
        ret=0;
    }
    return ret;
}
// Query Server Share List
int Samba_QueryServerShareList(void **Handle,int *num,char *ip,char *p_username,char *p_password,char *p_domain)
{
    int ret=-1;
    if(ip && p_username && p_password)
    {
        int id;
        char inFile[FILE_SIZE],outFile[FILE_SIZE],executeCmd[CMD_SIZE];
        Rtk_Ini *inIniObj=new Rtk_Ini();
        Rtk_Ini *outIniObj=new Rtk_Ini();
        id=getUniqueId();
        sprintf(inFile,"%s/%d.in",FILE_PATH,id);
        sprintf(outFile,"%s/%d.out",FILE_PATH,id);
        sprintf(executeCmd,"%s %s %s",SMB_EXE,inFile,outFile);
    // prepare infile    
        char *temp;
    
        inIniObj->setIntValue(MAIN, COMMAND,4);
    //P1
        if(!inIniObj->addStrMark_needfree(ip, &temp))
        {
            inIniObj->setStrValue(MAIN, P1,temp);
            FREE(temp);
        }
        else
            inIniObj->setStrValue(MAIN, P1,ip);
    //P2
        if(!inIniObj->addStrMark_needfree(p_username, &temp))
        {
            inIniObj->setStrValue(MAIN, P2,temp);
            FREE(temp);
        }
        else
            inIniObj->setStrValue(MAIN, P2,p_username);
    //P3
        if(!inIniObj->addStrMark_needfree(p_password, &temp))
        {
            inIniObj->setStrValue(MAIN, P3,temp);
            FREE(temp);
        }
        else
            inIniObj->setStrValue(MAIN, P3,p_password);
    //P4
        if(p_domain)
            if(!inIniObj->addStrMark_needfree(p_domain, &temp))
            {
                inIniObj->setStrValue(MAIN, P4,temp);
                FREE(temp);
            }
            else
                inIniObj->setStrValue(MAIN, P4,p_domain);
        
        inIniObj->saveFile(inFile);
        delete inIniObj;
    // execute
        system(executeCmd);
    // get ip
        *Handle=NULL;
        if(!outIniObj->loadFile(outFile))
        {
            int rec_num;
            if(!outIniObj->getIntValue(MAIN,NUMBER,&rec_num))
            {
                if(rec_num<0)
                {
                    ret=rec_num;
                    delete outIniObj;
                }
                else
                {
                    *num=rec_num;
                    *Handle=(void *)outIniObj;
                    outIniObj->removeInsideValueStrMark();
                    ret=0;
                }
            }
            else
                delete outIniObj;
        }
        else
            delete outIniObj;
#ifndef DEBUG
        unlink(inFile);
        unlink(outFile);
#endif    
    }
    return ret;

}
int Samba_GetServerShareList(void *Handle,int num,char **Share,int *type)
{
    int ret=-1;
    if(Handle)
    {
        Rtk_Ini *outIniObj;
        char *cOutput=NULL;
        char StrNum[32];
        int iType;
        sprintf(StrNum,"%d",num);
        outIniObj=(Rtk_Ini *)Handle;
        if((cOutput=outIniObj->getStrValue(StrNum,P1))!=NULL)
        {
            *Share=cOutput;
            ret=0;
        }
        if(!ret && !outIniObj->getIntValue(StrNum,P2,&iType))
        {
            *type=iType;
        }
        else
            *type=-1;
    }
    return ret;
}
// free Server Share Handle , must input total number from Samba_QueryServerShareList
int Samba_freeServerShareHandle(void *Handle,int Total)
{
    return Samba_freeHandle(Handle);
}

