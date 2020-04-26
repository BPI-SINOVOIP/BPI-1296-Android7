////////////////////////////////////////////////////////////////////////////////
/// Inter-Process Communication - Pipe Implementation<BR>
/// <BR>
/// Define the path of named pipe
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_PIPE_PATH
#define INCLUDED_IPC_PIPE_PATH

namespace rtk {
namespace ipc {

    /// Set pipe root path. Default is /tmp.
    ///
    /// @param pStrPipeRootPath Pipe root path.
    ///
    void SetPipeRootPath(const char* pStrPipeRootPath);

    /// Get the root path of named pipe(server side)
    ///
    /// @param pServerName Server name
    /// @param pBuffer The buffer which hold the result string.
    /// @param iBufLen The length of pBuffer
    ///
    /// @return true --> Success, false --> Failed, maybe the buffer is too small.
    ///
    bool GetPipeServerRootPath(const char* pServerName, char* pBuffer, int iBufLen);
    
    /// Get the path of named pipe(server side)
    ///
    /// @param pServerName Server name. We combine the server name with pipe path.
    /// @param iPipeId Pipe ID
    /// @param pBuffer The buffer which hold the result string.
    /// @param iBufLen The length of pBuffer
    ///
    ///  @return true --> Success, false --> Failed, maybe the buffer is too small.
    ///
    bool GetPipeServerPath(const char* pServerName, int iPipeId, char* pBuffer, int iBufLen);
    
    /// Get the path of named pipe(client side)
    ///
    /// @param pServerName Server name. We combine the server name with pipe path.
    /// @param iPipeId Pipe ID
    /// @param pBuffer The buffer which hold the result string.
    /// @param iBufLen The length of pBuffer
    ///
    /// @return true --> Success, false --> Failed, maybe the buffer is too small.
    ///
    bool GetPipeClientPath(const char* pServerName, int iPipeId, char* pBuffer, int iBufLen);

}; // namespace ipc
}; // namespace rtk

#endif
