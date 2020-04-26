///////////////////////////////////////////////////////////////////////////////
/// Log tools for all of platforms.<BR>
/// It supports:<BR>
/// <OL>
///     <LI>Windows</LI>
///     <LI>Linux</LI>
///     <LI>iOS</LI>
///     <LI>Android</LI>
/// </OL>
/// <BR><BR>
/// Please using macro <B>zDebug</B> and <B>zPrint</B> to dump your log.
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_LOG_TOOLS__
#define __INCLUDED_LOG_TOOLS__

namespace rtk   {
namespace tools {

/// Enable dumping log.
///
/// @param bEnable true --> Enable dumping log.
/// @param bDumpToFile true --> Dumping log to a file.
///
void EnableDumpLog(bool bEnable, bool bDumpToFile = false);

/// Flush log file
///
void FlushLogFile();

/// Dump buffer into a file. The file name will be current time stamp.
///
/// @param pBuffer The buffer wants to be dumped
/// @param iDataSize The data size in the buffer.
///
void DumpBuffer(char* pBuffer, int iDataSize);

/// Collect all of log files and compress it.<BR>
/// If there existed a core dump, it will compress it, too.
///
/// @param iPid The PID of process.
///
void CollectLogFiles(int iPid);

/// Check if dumping log is enabled
///
/// @return true --> Enabled.
///
bool IsDumpLogEnabled();

/// Dump log. <BR>
/// If EnableDumpLog(false), the log will be hidden.<BR>
/// If EnableDumpLog(true), the log will be appeared.<BR>
///
/// @param lpszFmt The format string.
/// @param ... Format value
///
void DumpDbgMsg(const char* lpszFmt, ...);

/// Force dumping log even EnableDumpLog(false).
///
/// @param lpszFmt The format string.
/// @param ... Format value
///
void ForceDumpDbgMsg(const char* lpszFmt, ...);

}; // namespace tools
}; // namespace rtk


#define DUMP_LOG_MESSAGE // We always enable log, and let EnableDumpLog() to 
                         // control if log should be appeared or hidden.

//#define DUMP_LOG_AT_TVSERVER

// Quick macro for dumping log.
#ifdef DUMP_LOG_MESSAGE
	#ifdef DUMP_LOG_AT_TVSERVER
	#define zDebug(...)  
	#define zPrint(...)  
	#define zWarning rtk::tools::DumpDbgMsg
	#define zInfo rtk::tools::DumpDbgMsg
	#else	//tvServer usage
	#define zDebug rtk::tools::DumpDbgMsg
	#define zPrint rtk::tools::ForceDumpDbgMsg
	#define zWarning rtk::tools::ForceDumpDbgMsg
	#define zInfo rtk::tools::ForceDumpDbgMsg
	#endif
#else
    #define zDebug(...)
    #define zPrint(...)
    #define zWarning(...)
    #define zInfo(...)
#endif



#endif
