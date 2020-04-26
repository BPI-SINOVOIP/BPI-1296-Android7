/** @file DataObject.h
 * DataObject.h : the header of DataObject.cpp
 */

#ifndef __DATAOBJECT_CLASS_H__
#define __DATAOBJECT_CLASS_H__

//#include <Filters/NavigationFilter/CNavigationFilter.h>
//#include <Filters/NavigationFilter/ts_guide.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h> // modify path by hamer @ 20130723
#include <OSAL/Threads.h>
#include <UTIL/ulQueue.h>

// added by hamer @ 20130723
#define S_IREAD        S_IRUSR
#define S_IWRITE       S_IWUSR
#define S_IEXEC        S_IXUSR

/*! @class DataObjectException
 *  @brief for user to throw DataObjectException
 *  @author Jason Lee
 *  @author jasonlee@realtek.com.tw
 *  @version 0.01
 *  @date    2006/08/23
 *  @bug None.
 *  @warning None.
 */
class DataObjectException {
public:
    /*! default constructor */
    DataObjectException() {m_cDesc=NULL;};
    /*! set the error string constructor */
    DataObjectException(char *cTemp)
    {
        m_cDesc = cTemp;
    };
    DataObjectException(char *cTemp,int iret,int line)
    {
        m_cDesc = cTemp;
        m_ret=iret;
        m_line=line;
    };

    char *GetDesc()
    {
        return m_cDesc;
    };

    int GetRet()
    {
        return m_ret;
    };
    int GetLine()
    {
        return m_line;
    };
private:
    /*! char pointer for store the error description */
    char *m_cDesc;
    int m_ret;
    int m_line;
};
class async_sql_queue
{
public:
    async_sql_queue(char *sql);
    ~async_sql_queue();
    int getSql(char **sql);
private:
    char *m_sql;
};
typedef class ulCircularQueue<async_sql_queue *> async_sql_queue_t;


/*! @class DataObject
 *  @brief for user to get default database handle and data manipulate utility
 *  @author Jason Lee
 *  @author jasonlee@realtek.com.tw
 *  @version 0.01
 *  @date    2006/07/26
 *  @bug None.
 *  @warning None.
 */
class DataObject{
protected:
  bool bActive;
//  virtual char * GetDB_PATH()=0;
  virtual char * GetDB_FILENAME()=0;
  virtual char * GetBACKUP_DB_FILENAME()=0;
  virtual char * GetREALTEK_SQLITE_DB()=0;
  virtual int GetDB_VERSION()=0;
  virtual int GetDB_DEVELOP_VERSION()=0;
  virtual int GetDB_CONNECTION_MAX()=0;
  virtual int GetDB_LOCK_RETRY()=0;
  virtual int GetDB_LOCK_RETRY_TIME()=0;
  int doCopyFile(char *Target,char *Source);
  bool CreateConnectionPool(void);
  bool DestroyConnectionPool(void);
  virtual int beforeExecSQL(sqlite3 *db){return 0;}
  virtual int afterExecSQL(sqlite3 *db){return 0;}
public:
  virtual int setDB_PATH(char *pPath);
/*! @fn DataObject::DataObject()
 *  @brief DataObject Constructor.
 *  @brief to initial static member m_pDB to open sqlite database
 *  @param None
 *  @exception if can't open database may cause system break
 *  @return None
 */
  DataObject();
/*! @fn DataObject::~DataObject()
 *  @brief DataObject deconstructor.
 *  @param None
 *  @exception None
 *  @return None
 */
  virtual ~DataObject();
  virtual int initDB_PRAGMA(sqlite3 *db);
  virtual int Activate();
  virtual int Deactivate();
  bool Active(void);
  virtual int DoSync(){return 0;} // for Flash do Sync
  virtual int DoBackup(){return 0;} // for Flash do Sync
  virtual int fixDBCorrupt();
  virtual bool CompactDB(void);
  int RestoreOrgDB();
/*! @fn sqlite3* DataObject::GetDBID()
 *  @brief To get database ID
 *  @param void not need input any parameter
 *  @exception None
 *  @return a Database pointer
 */
  sqlite3* GetDBID(void);
  bool ReleaseDBID(sqlite3 *&pSqliteDBID,bool reopen=false);
  int GetPrivateDBID(sqlite3 **db);
  int ReleasePrivateDBID(sqlite3 *db=NULL);
  int getRtkDB(sqlite3 **db);
  int releaseRtkDB(sqlite3 **db);
/*! @fn bool convertUTF16BEtoUTF8(unsigned char *pDstStr,int *Dstlen,unsigned char *pStr,int Srclen)
 *  @brief covert UTF16BE string to UTF8 string  (the source UTF8 string must even char)
 *  @param pDstStr char pointer to store result UT8 string
 *  @param Dstlen int pointer to get the destination UTF8 length
 *  @param pStr char pointer to store source UTF-16BE string (compatable to 10646-1)
 *  @param Srclen int to set the source string length
 *  @exception None there are return error when the source UTF-16BE string are odd
 *  @return true successful to convert
 *  @return false fail to convert or the source UTF-16BE string are odd
 */
  bool convertUTF16BEtoUTF8(unsigned char *pDstStr,int *Dstlen,unsigned char *pStr,int Srclen); // in : UTF16-BE , out : UTF8 value (multi bytes char)
  bool GetValue(char* cParameter,void *cValue,int iValueLen);
  bool SetValue(char* cParameter,void *cValue,int iValueLen);
  bool GetValue(char* cParameter,int *iValue);
  bool SetValue(char* cParameter,int iValue);
  bool GetValue(char* cParameter,char *cValue);
  bool SetValue(char* cParameter,char *cValue);
  bool removeKey(char* cParameter);

  bool GetValue(sqlite3 *db,char* cParameter,void *cValue,int iValueLen);
  bool GetValue(sqlite3 *db,char* cParameter,void *cValue,int iValueLen,int *realsize);
  bool SetValue(sqlite3 *db,char* cParameter,void *cValue,int iValueLen);
  bool GetValue(sqlite3 *db,char* cParameter,int *iValue);
  bool SetValue(sqlite3 *db,char* cParameter,int iValue);
  bool GetValue(sqlite3 *db,char* cParameter,char *cValue);
  bool SetValue(sqlite3 *db,char* cParameter,char *cValue);
  bool removeKey(sqlite3 *db,char* cParameter);

  int sqlite3_get_table(
      sqlite3*,               /* An open database */
      const char *sql,       /* SQL to be executed */
      char ***resultp,       /* Result written to a char *[]  that this points to */
      int *nrow,             /* Number of result rows written here */
      int *ncolumn,          /* Number of result columns written here */
      char **errmsg          /* Error msg written here */
    );

    int sqlite3_prepare(
      sqlite3 *db,            /* Database handle */
      const char *zSql,       /* SQL statement, UTF-8 encoded */
      int nBytes,             /* Length of zSql in bytes. */
      sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
      const char **pzTail     /* OUT: Pointer to unused portion of zSql */
    );

    int sqlite3_step(sqlite3_stmt *stmt);

    int sqlite3_exec(
      sqlite3 *db,                     /* An open database */
      const char *sql,              /* SQL to be executed */
      sqlite3_callback callfunc,             /* Callback function */
      void *iParam,                       /* 1st argument to callback function */
      char **errmsg                 /* Error msg written here */
    );

    int sqlite3_finalize(sqlite3_stmt *pStmt);

    int ExecSqlString(char *cSQL);
    int ExecInsertSqlString(char *cSQL,int *result_id);
    int ExecSelectSqlString(char *cSQL,char ***cResultSet,int *Row,int *Col);
    int ExecCountSelectSqlString(char *cSQL,int *TotalNum);

    int ExecSqlString(sqlite3 *db,char *cSQL);
    int ExecInsertSqlString(sqlite3 *db,char *cSQL,int *result_id);
    int ExecSelectSqlString(sqlite3 *db,char *cSQL,char ***cResultSet,int *Row,int *Col);
    int ExecCountSelectSqlString(sqlite3 *db,char *cSQL,int *TotalNum);

    int beginTransaction();
    int execTransSqlString(char *cSQL);
    int execTransInsertSqlString(char *cSQL,int *result_id);
    int execTransSelectSqlString(char *cSQL,char ***cResultSet,int *Row,int *Col);
    int execTransCountSelectSqlString(char *cSQL,int *TotalNum);
    int endTransaction(bool releaseDB=false);
    sqlite3 *getTransDB(){return m_pTransDB;}

    int manualCopyData(sqlite3 *dest_db,char *dest_prepare_sql,sqlite3 *src_db,char *src_select_sql,int (*stepFunc)(void)=NULL);

    virtual bool IsDBActive(){return bActive;}
    int addAsyncSql(char *sql);
    int getAsyncSql(void **handle,char **sql);
    int releaseAsyncHandle(void *handle);
    int removeAllAsyncSql(void);
    int executeAsyncSql(void);
    int interruptAllUsedDB();
protected:
    char *m_pDbPath;
    int getNewFileName_NeedFree(char **ppNewFileName,char *OrgFileName,char *FileAddExt);
    virtual int UpdateDBSchema();
    virtual int CreateNewDB(void);

private:


  sqlite3 **m_pDB;
  /*!
  0: available
  1: Occupy
  */
  int *m_iConnFlag;
  osal_mutex_t m_do_mutex;
  uint32_t m_state;
  osal_mutex_t m_state_mutex;

  sqlite3 *m_pTransDB;
  int m_db_change_count;

//
// add to remove db release problem
//
  osal_mutex_t m_use_db_mutex;
  int m_connect_num;
  int m_db_manage_flag_cnt;
  void doBeforeManageDB();
  void doAfterManageDB();
  int doBeforeUseDB();
  void doAfterUseDB();
  bool checkDBFileSytemIsReadOnly();
//
// only one thread use trans db
//
  osal_mutex_t m_trans_db_mutex;
  async_sql_queue_t m_async_sql_queue;
};


#endif
