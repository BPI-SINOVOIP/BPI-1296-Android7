/** @file DataObject.h
 * DataObject.h : the header of DataObject.cpp
 */

#ifndef __DATAOBJECTFLASH_CLASS_H__
#define __DATAOBJECTFLASH_CLASS_H__
#ifdef WIN32
	#include "DataObject.h"
#else
	#include <Application/AppClass/DataObject.h>
#endif

class DataObjectFlash : public DataObject 
{
public:
    DataObjectFlash();
    virtual ~DataObjectFlash();
    static void SetInstance(DataObjectFlash *pInstance)
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

    static DataObjectFlash *GetInstance(void)
    {
        return m_ptr;
    }
    virtual int setDB_PATH(char *pPath);
    virtual int Activate();
    virtual int DoSync();
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
    static DataObjectFlash *m_ptr;
    static bool m_bSingle;
    char *m_pDbPath_journal;
    char *m_pSyncDB;
    char *m_pSyncDB_journal;
    char *m_ptmpDbPath;
    char *m_ptmpDbPath_journal;
    char *m_backup_db_filename;
    char *m_sync_file;
};


#endif
