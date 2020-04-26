 
#ifndef _C2MS_H_
#define _C2MS_H_
 
#ifdef __cplusplus
extern "C" {
#endif

 

void * c2ms_mgmt_init  (uint8 * confile, uint64 uid, uint8 * hid, void * pcache, int cachesize);

int    c2ms_mgmt_clean (void * vmgmt);
int    c2ms_mgmt_test(void * vmgmt, int test);
void * c2ms_mem_alloc    (void * vmgmt, int size);
int    c2ms_mem_free     (void * vmgmt, void * pbuf); 
int    c2ms_mem_restsize (void * vmgmt); 
int    c2ms_mgmt_exception(void * vmgmt, void * vsess, int errcode, uint16 errmsglen, uint8 * errmsg);

uint64  c2ms_uid    (void * vmgmt);
uint8 * c2ms_hid    (void * vmgmt);
uint8 * c2ms_localip(void * vmgmt); 
uint8 * c2ms_natip  (void * vmgmt);
uint32  c2ms_oemid     (void * vmgmt);
uint32  c2ms_version   (void * vmgmt);
time_t  c2ms_buildtime (void * vmgmt);


//外部回调函数的函数指针，作为参数传给sess_open1，可以传空值。可以作为看门狗或退出调用。
//   在sess_open1阻塞期间每秒执行一次，其返回值为负值会造成sess_open1退出。
typedef int (*func_call)();  
void * c2ms_sess_open1   (void * vmgmt, uint8 * url, int * exitcode, func_call func);

void * c2ms_sess_open  (void * vmgmt, uint8 * url, int * exitcode);
int    c2ms_sess_close  (void * vsess);
 
int c2ms_sess_attr      (void * vsess, uint8 * fid, uint8 * mname, uint64 * msize, uint8 * mmime);
int c2ms_sess_attr_bitrate  (void * vsess, uint32 * mduration, uint32 * mbitrate);
int c2ms_sess_idxfile   (void * vsess, uint8 * idxfid, uint32 * idxlen);
     
int c2ms_sess_cache_init(void * vsess, int packsize);

int c2ms_sess_range     (void * vsess, uint64 offset, uint64 length);
int c2ms_sess_set_media (void * vsess, int idxflag, uint64 offset, uint64 length);
int c2ms_sess_seek      (void * vsess, uint64 seekpos);
int c2ms_sess_read      (void * vsess, void * pbuf, int length, int nbmode);
int c2ms_sess_recv      (void * vsess, void * pbuf, int length, int waitms);
 
int c2ms_sess_idx_open  (void * vsess, uint32 * idxsize);
int c2ms_sess_idx_open_asyn (void * vsess, uint32 * idxsize);
int c2ms_sess_idx_read  (void * vsess, uint8 * pbuf, int length, uint32 offset);
int c2ms_sess_idx_close (void * vsess);
int c2ms_sess_idx_offset(void * vsess, uint64 * poffset);

int c2ms_sess_buffering (void * vsess, int bufsize);
int c2ms_sess_bufratio  (void * vsess, int * ratio);

double c2ms_sess_realtime_recvspeed (void * vsess);
double c2ms_sess_average_recvspeed (void * vsess);

//判断是否媒体播放结束，而不是介质的尾部
int c2ms_sess_is_media_end(void *vsess, int size);

char * c2ms_sess_error  (int errcode);

int  c2ms_sess_indexserver_num (void * vsess);
int  c2ms_sess_indexserver_info(void * vsess, uint8 srvindex, uint8 * srvname, int * port, 
								uint8 * type);
int  c2ms_sess_downserver_num (void * vsess);
int  c2ms_sess_downserver_info (void * vsess, uint8 srvindex, uint8 * srvname, int * port, uint8 * type,
								uint8 * parentsrvname, int * opentimes, int * shutcountscur, int * shutcountstotal);
int  c2ms_sess_node_num (void * vsess);
int  c2ms_sess_node_info(void * vsess, uint8 nodeindex, uint32 * nodeid, uint8 * srvname, int * port, 
						 int * state, int64 * rttcur, int64 * rttavg, double * speedreal,
						 double * speedavg, int * load);
						 
#ifdef __cplusplus
}
#endif
 
#endif
 
