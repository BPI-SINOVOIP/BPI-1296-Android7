/** @file DataObject.h
 * DataObject.h : the header of DataObject.cpp
 */

#ifndef __DATAOBJECTHDD_CLASS_H__
#define __DATAOBJECTHDD_CLASS_H__

#ifdef WIN32
	#include "DataObject.h"
#else
	#include <Application/AppClass/DataObject.h>
#endif

//#define DoBackupRecovery

class DataObjectHdd : public DataObject 
{
public:
    DataObjectHdd();
    virtual ~DataObjectHdd();
    static void SetInstance(DataObjectHdd *pInstance)
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

    static DataObjectHdd *GetInstance(void)
    {
        return m_ptr;
    }
    virtual int setDB_PATH(char *pPath);
    
#ifdef DoBackupRecovery    
    virtual int DoBackup();
    virtual int fixDBCorrupt();
#endif    
protected:
    virtual char * GetDB_FILENAME();
    virtual char * GetBACKUP_DB_FILENAME();
    virtual char * GetREALTEK_SQLITE_DB();
    virtual int GetDB_VERSION();
    virtual int GetDB_DEVELOP_VERSION();
    virtual int GetDB_CONNECTION_MAX();
    virtual int GetDB_LOCK_RETRY();
    virtual int GetDB_LOCK_RETRY_TIME();
private:
    static DataObjectHdd *m_ptr;
    static bool m_bSingle;
    uint32_t m_hdd_state;
    char *m_backup_db_filename;
    char *m_bk1_db_filename;
    char *m_bk2_db_filename;
    char *m_tmp_db_filename;
};


#endif
