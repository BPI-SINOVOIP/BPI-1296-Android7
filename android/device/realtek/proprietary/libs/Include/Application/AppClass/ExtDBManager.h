#ifndef __EXT_DBMANAGER_H__
#define __EXT_DBMANAGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h> // modify path by hamer @ 20130723
#include <OSAL/Threads.h>
#include <Application/AppClass/DataObjectExtHdd.h>
#define MAX_EXESQL_DB_NUM 10
/*  Class structure....
ExtDBManager
        |
    LLExtHdd   -  LLExtHdd  -  LLExtHdd  -  LLExtHdd  
          |                  |                 |                 |
    LLPartition     LLPartition    LLPartition    LLPartition
                              |                 |
                        LLPartition    LLPartition    
                              |                              
                        LLPartition                       
                              |                                    
                        LLPartition                       
    
*/
typedef void * HANDLE;

enum ENUM_DB_CALLBACK_ACTION
{
    DBCB_GET_TEMPDB=1  ,
    DBCB_RELEASE_TEMPDB,
    DBCB_ADD_ASYNC_SQL,
    DBCB_DB_CORRUPT,
    DBCB_SAVE_CONTENT2FILE,
};

typedef struct _dbcb_get_tempdb
{
    char *o_temp_db_path_needfree;
    bool i_build;
    char *i_partition_path;
    char *i_relative_path;
    char *i_db_filename;
    int i_size_MB;
} dbcb_get_tempdb;

typedef struct _dbcb_release_tempdb
{
    char *i_temp_db_path;
    char *i_partition_path;
} dbcb_release_tempdb;

typedef struct _dbcb_add_async_sql
{
    DataObjectExtHdd *db;
} dbcb_add_async_sql;

typedef struct _dbcb_save_content2file
{
    char *i_partition_path;
    char *i_filepath;
    void *i_content;
    int i_content_size;
} dbcb_save_content2file;

typedef int (* DBCallBackFunction )(ENUM_DB_CALLBACK_ACTION,void *info,void *pThis);

class ExtDBManager{
public:
    enum ENUM_DISK_TYPE
    {
        TYPE_EXTERNAL_HDD  = 1,
        TYPE_INTERNAL_HDD  = 2,
    };
    ExtDBManager();
    ~ExtDBManager();
/* Init */
    int setDBPathName(char *dbRelativePath,char *dbName,char *crossdbRelativePath=NULL,char *crossdbName=NULL);
/* for GUI AP Use */
    int execSelectSqlString(char *cSQL,char ***cResultSet,int *Row,int *Col,HANDLE hdd=NULL);
    int execCountSelectSqlString(char *cSQL,int *TotalNum,HANDLE hdd=NULL);
    int execUpdateDeleteSqlString(char *cSQL,unsigned int partition_id=0);
    int execAsyncUpdateDeleteSqlString(char *cSQL,unsigned int partition_id=0);
    int execInsertSqlString(int *result_id,char *cSQL,unsigned int partition_id=0);
    int doAllSync();
/*Utility - for Scan disk*/
    /* 
        input : pattition path , ENUM_DISK_TYPE 
        output : exthdd_id , partition_id
    */
    int addHddPartition(HANDLE *hdd,char *HddPartition_path,char *partition_label,char *partition_volume,ENUM_DISK_TYPE disk_type);
    int manualAddNewHdd(HANDLE *hdd,ENUM_DISK_TYPE disk_type);
    int manualAddPartition(HANDLE *resulthdd,HANDLE hdd,char *HddPartition_path,char *partition_label,char *partition_volume);
    int testCreateHddDatabase(HANDLE hdd,uint64_t *needDBFreeSize);
    int syncHddPartitionId(HANDLE hdd);
    
    int getHddCount();
    int getRootHddHandle(HANDLE *root_hdd);
    int getNextHddHandle(HANDLE *hdd,HANDLE root_hdd);
    DataObjectExtHdd *getHddDatabase(HANDLE hdd,uint64_t *needDBFreeSize=NULL);
    int getHddDatabasePath(char **ppDBPath,HANDLE hdd);
    char *getHddMountGroupHead(HANDLE hdd);
    int doSync(HANDLE hdd);
    int getRootPartitionFromHddHandle(HANDLE *root_partition,HANDLE hdd);
    int getPartitionFromHddPartitionIdHandle(HANDLE *partition,HANDLE hdd,unsigned int partition_id);
    int getNextPartitionHandle(HANDLE *partition,HANDLE root_partition);
    int getPartitionInfoFromPartitonHandle(char **partition_path,unsigned int *partition_id,bool *scanned,bool *merged,HANDLE partition);
    int getPartitionInfoFromPartitonId(char **partition_path,unsigned int partition_id);
    int getPartitionLabelFromPartitonId(char **partition_label,unsigned int partition_id);
    int getPartitionVolumeFromPartitonHandle(char **partition_volume,HANDLE partition);
    int getPartitionVolumeFromPartitonId(char **partition_volume,unsigned int partition_id);
    int setPartitionScanned(HANDLE partition,bool scanned);
    bool getPartitionScanned(HANDLE partition);
    int setPartitionMerged(HANDLE partition,bool merged);
    bool getPartitionMerged(HANDLE partition);
    int getHddHandleFromPartitionId(HANDLE *hdd,HANDLE *partiton,unsigned int partition_id);
    int getScanedHddCount();
    DataObjectExtHdd *getCrossDataBase();
    DataObjectExtHdd *genCrossDataBase(uint64_t *needDBFreeSize=NULL);
    DataObjectExtHdd *getQueryDataObject();
    DataObjectExtHdd *getPartitionDataObject(unsigned int partition_id);
    bool isHddExistDataObject(HANDLE hdd);
    int setHddRemoved(HANDLE hdd);
    int rebuildHddDatabase(HANDLE hdd,bool removeDBPath=false);
    char *getDBPath(HANDLE hdd);
/*
ret = 0 : no hdd is removed
ret = 1 : have hdd is removed
*/
    int checkHddIsRemoved(char *partition_path=NULL);
    int getRootRemovedHddHandle(HANDLE *root_hdd);
    int getNextRemovedHddHandle(HANDLE *hdd,HANDLE root_hdd);
    int cleanRemovedHdd();
    int stopExecuteHddSql(HANDLE hdd);

    DataObjectExtErrorHandle *getHddDBErrorHandle(HANDLE hdd);
    DataObjectExtErrorHandle *getCrossDBErrorHandle();
    void setDoMerging(bool isMerging){m_CrossDB_merging=isMerging;}
    int registerDBCallBack(DBCallBackFunction function,void *pThis);

    DBCallBackFunction m_DBCallBackfunction;
// get instance 
    static ExtDBManager *GetInstance(void)
    {
        if(p.m_ptr) return p.m_ptr;
        else 
        {
            p.m_ptr = new ExtDBManager();
            return(p.m_ptr);
        }   
    };
    static void DeleteInstance(void) {
        if(p.m_ptr) 
        {
            delete p.m_ptr;
            p.m_ptr=0;
        }
    };    
    friend class AutoPtr;
private:
    class AutoPtr
    {
        public:
            ExtDBManager *m_ptr;
            AutoPtr(){m_ptr=0;}
            ~AutoPtr(){if(m_ptr) delete m_ptr;}
    };
    static AutoPtr p;    

public:
    class LLPartition
    {
    public:
        LLPartition(char *partition_path,char *partition_label,char *partition_volume);
        ~LLPartition();
        int setPartitionId(unsigned int partition_id);
        unsigned int getPartitionId();
        int setSyncWithDB(bool status);
        bool getSyncWithDB();
        int getSequenceId();
        char *getPartitionPath();
        char *getPartitionLabel();
        char *getPartitionVolume();
        uint64_t getFreeSize();
        uint64_t getTotalSize();
        bool isScanned();
        bool isMerged();
        void setScanned(bool scanned){m_scanned=scanned;}
        void setMerged(bool merged){m_merged=merged;}
        LLPartition *getNextPartition();
        int addPartition(char *partition_path,char *partition_label,char *partition_volume);
        bool isInPartitionList(unsigned int partition_id);
    private:
        unsigned int m_partition_id;
        int m_sequence_id;  // example : sda1 -> 1 , sda2 -> 2
        bool m_syncWithDB;
        char *m_partition_path;
        char *m_partition_label;
        char *m_partition_volume;
        uint64_t m_FreeSize;
        uint64_t m_TotalSize;
        LLPartition *m_NextPartition;
        bool m_scanned;
        bool m_merged;
        int setDiskInfo();
    };
    class LLExtHdd
    {
    public:
        LLExtHdd(LLExtHdd *Prev_ExtHdd,char *exthdd_path,char *db_realtive_path,char *db_name,char *partition_label,char *partition_volume,ExtDBManager *pDBMgr=NULL,void *callbackThis=NULL,ENUM_DISK_TYPE disk_type=TYPE_EXTERNAL_HDD);
        LLExtHdd(LLExtHdd *Prev_ExtHdd,char *db_realtive_path,char *db_name,ExtDBManager *pDBMgr=NULL,void *callbackThis=NULL,ENUM_DISK_TYPE disk_type=TYPE_EXTERNAL_HDD);
        ~LLExtHdd();
        LLPartition *getHeadPartition();
        /*
        intput : partition_path,ENUM_DISK_TYPE
        output : hdd_id
        */
        int addPartition(HANDLE *hdd,char *partition_path,char *partition_label,char *partition_volume,ENUM_DISK_TYPE disk_type=TYPE_EXTERNAL_HDD);
        int addPartition(char *partition_path,char *partition_label,char *partition_volume);

//        bool isInternal(){return (TYPE_INTERNAL_HDD == m_disk_type);}
        bool getDiskType(){return m_disk_type;}
        LLExtHdd *getNextExtHdd();
        int setNextExtHdd(LLExtHdd *Next_ExtHdd);
        LLExtHdd *getPrevExtHdd();
        int setPrevExtHdd(LLExtHdd *Prev_ExtHdd);
        LLExtHdd *getHeadExtHdd();

        int setRemove();
        bool isRemove(){return m_isRemoved;}
        int removeThisExtHdd();
        
        /*
        needDBFreeSize : reference only return NULL
        */
        DataObjectExtHdd *getDataObject(uint64_t *needDBFreeSize=NULL); 
        DataObjectExtHdd *getPartitionDataObject(unsigned int partition_id,uint64_t *needDBFreeSize=NULL); 
        char *getDBPath();
        int syncPartitionId();
        unsigned int getUniquePartitionId();
        int getMaxFreeSizePartition(uint64_t *freeSize,char **partition_path);
        DataObjectExtErrorHandle *getDBErrorHandle(){return m_db_error_handle;}
        char *getHddMountGroupHead(){return m_exthdd_path;}
        char *getDBMountPath(){return m_db_mount_file;}
        bool isExistDataObject(void);
    private:
        ENUM_DISK_TYPE m_disk_type;
        bool m_isRemoved;
        char *m_exthdd_path;
        char *m_db_realtive_path;
        char *m_db_name;
        char *m_db_mount_file;
        char *m_db_partition_path;
        osal_mutex_t m_db_operate_mutex;
        DataObjectExtHdd *m_pDataObject;
        DataObjectExtErrorHandle *m_db_error_handle;
        LLExtHdd *m_Prev_ExtHdd;
        LLExtHdd *m_Next_ExtHdd;
        LLPartition *m_HeadPartition;
        static int m_unique_id;
        ExtDBManager *m_ExtDBManager;
        void *m_callbackThis;
        int getExtHddPath_needfree(char **exthdd_path,char *fullPath);
        uint64_t getHddTotalSize();
        int passDBCreateCriteria(uint64_t HddSize,uint64_t HddFreeSize,uint64_t *needDBFreeSize);
    };
private:
// ExtDBManager internal member    
    char *m_dbRelativePath;
    char *m_dbName;
    char *m_crossdbRelativePath;
    char *m_crossdbName;
    char *m_crossdb_mount_file;
    char *m_crossdb_partition_path;
    bool m_is_crossdb_use_same_partition;
    DataObjectExtHdd *m_CrossDB;
    bool m_CrossDB_merging;
    DataObjectExtErrorHandle *m_CrossDB_error_handle;
    LLExtHdd *m_HeadExtHdd;
    void *m_callbackThis;
    unsigned int m_state;
// buffer
    DataObjectExtHdd *m_bufferPartitionDB;
    unsigned int m_bufferPartitionId;
// member function    
    int crossDBCreateCriteria(uint64_t HddFreeSize,uint64_t *needDBFreeSize);
// thread to stop long query
public:
    int beforeExecSQL(sqlite3 *db);
    int afterExecSQL(sqlite3 *db);
private:
    osal_mutex_t m_exeSqlDB_mutex;
    sqlite3 *m_exeSqlDB[MAX_EXESQL_DB_NUM];

    bool m_bEndStopSqlThread;
    osal_thread_t m_stopSqlThread;
    osal_sem_t m_testkey_sem;   
    static void stopSqlThread(void *param);
};

#endif
