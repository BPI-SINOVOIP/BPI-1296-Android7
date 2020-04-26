/** @file DataObject.h
 * DataObject.h : the header of DataObject.cpp
 */

#ifndef __DATAOBJECTHDDEXT_CLASS_H__
#define __DATAOBJECTHDDEXT_CLASS_H__

#include <Application/AppClass/DataObject.h>

//#define DoBackupRecovery

#define MAX_SQL_PLAN 10

class DataObjectExtErrorHandle{ 
public:
    enum ENUM_ERROR_TYPE
    {
        ERROR_TYPE_INIT  = 0,
        ERROR_TYPE_DB_READY,
        ERROR_TYPE_FEW_FREESIZE, 
        ERROR_TYPE_CANNOT_REMOVE_BROKEN_DB, 
        ERROR_TYPE_CANNOT_CREATE_DB, 
        ERROR_TYPE_DB_CORRUPT, 
        ERROR_TYPE_DB_CLOSE,
    };
    DataObjectExtErrorHandle();
    ~DataObjectExtErrorHandle();
    void setErrorType(ENUM_ERROR_TYPE eType){m_error_type=eType;}
    ENUM_ERROR_TYPE getErrorType(){return m_error_type;}
    
    void setAvailableSize(uint64_t size){m_1_available_size=size;}
    uint64_t getAvailableSize(){return m_1_available_size;}

    void setDBNeedSize(uint64_t size){m_1_db_need_size=size;}
    uint64_t getDBNeedSize(){return m_1_db_need_size;}
    
private:
    ENUM_ERROR_TYPE m_error_type;
    uint64_t m_1_available_size;
    uint64_t m_1_db_need_size;
};


class DataObjectExtHdd : public DataObject 
{
public:
    DataObjectExtHdd(bool isCrossDB);
    DataObjectExtHdd();
    virtual ~DataObjectExtHdd();
    virtual int initDB_PRAGMA(sqlite3 *db);
    
/*    
    static void SetInstance(DataObjectExtHdd *pInstance)
    {
        m_ptr = pInstance;
    }

    static void DeleteInstance(void)
    {
        if (m_ptr)
        {
            delete m_ptr;
            m_ptr = NULL;
        }
    }

    static DataObjectExtHdd *GetInstance(void)
    {
        return m_ptr;
    }
*/    
    virtual int Activate();
    virtual int Deactivate();
    virtual int DoSync();
    virtual bool CompactDB(void){return true;}
    virtual int setDB_PATH(char *pPath);
    virtual char * GetDB_FILENAME();
    virtual bool IsDBActive();
    static int removeDBFile(char *filename);
    
    void setErrorHandle(DataObjectExtErrorHandle *error){m_dbErrorHandle=error;}
    void setExtDBManager(void *pPt){m_ExtDBManager=pPt;}
    DataObjectExtErrorHandle *getErrorHandle(){return m_dbErrorHandle;}
    int setSqlPlan(int index,int plan);
    int getSqlPlan(int index,int *plan);
    int getRtkDBConnection(sqlite3 **pRtkDb);
    int freeRtkDBConnection(sqlite3 *pRtkDb);
#ifdef DoBackupRecovery    
//    virtual int DoBackup();
//    virtual int fixDBCorrupt();
#endif    
    virtual int fixDBCorrupt();
    int rebuildNewDB(bool removeDBPath=false);
protected:
    virtual char * GetBACKUP_DB_FILENAME();
    virtual char * GetREALTEK_SQLITE_DB();
    virtual int GetDB_VERSION();
    virtual int GetDB_DEVELOP_VERSION();
    virtual int GetDB_CONNECTION_MAX();
    virtual int GetDB_LOCK_RETRY();
    virtual int GetDB_LOCK_RETRY_TIME();
    virtual int UpdateDBSchema();
    virtual int CreateNewDB(void);
    virtual int beforeExecSQL(sqlite3 *db);
    virtual int afterExecSQL(sqlite3 *db);
private:
    int InitPrivateVariable(bool isCrossDB);
    
/*    
    static DataObjectExtHdd *m_ptr;
    static bool m_bSingle;
*/    
    uint32_t m_hdd_state;
    char *m_backup_db_filename;
    char *m_md5_filename;
/*
    char *m_bk1_db_filename;
    char *m_bk2_db_filename;
    char *m_tmp_db_filename;
*/    
    bool m_isCrossDB;
    DataObjectExtErrorHandle *m_dbErrorHandle;

    int checkMD5file(char *DBFileName,char *MD5FileName);
    int createMD5file(char *DBFileName,char *MD5FileName);

    unsigned int  crc_table[256];    
    void make_crc_table();
    unsigned int crc32( unsigned int crc, unsigned char *buf, unsigned int len);
    int checkSchema(char *HddDBFileName,char *RtkDBFileName);
    
    int m_sql_plan[MAX_SQL_PLAN];
    void *m_ExtDBManager;
};


#endif
