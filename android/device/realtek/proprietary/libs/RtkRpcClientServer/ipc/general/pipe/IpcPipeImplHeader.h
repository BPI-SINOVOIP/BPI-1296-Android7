////////////////////////////////////////////////////////////////////////////////
/// Inter-Process Communication - Pipe Implementation Stream Header
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_PIPE_IMPL_HEADER
#define INCLUDED_IPC_PIPE_IMPL_HEADER

namespace rtk {
namespace ipc {
    /// Constants
    enum
    {
        IPC_PIPE_MAGIC_NUMBER = (('I' << 24) | ('P' << 16) | ('C' << 8) | ' ') ///< Header magic number
    };

    /// Pipe communication header. <BR>
    /// <BR>
    /// Extra data can be cascaded after this header.<BR>
    /// The length of the cascaded data is IpcPipeImplHeader.iDataLen.<BR>
    /// IpcPipeImplHeader.iDataLen maybe zero.<BR>
    /// <BR>
    /// NOTE: The length of one transmission should not exceed PIPE_BUF.<BR>
    ///       PIPE_BUF is defined in limits.h<BR>
    ///       Within this length, Linux ensure the transmission dose not
    ///       interrupted by other thread.<BR>
    ///       Generally, PIPE_BUF is 4096 bytes.
    ///
    struct IpcPipeImplHeader
    {
        unsigned int iMagicNum; ///< Magic number. This should be IPC_PIPE_MAGIC_NUMBER
        unsigned int iID;       ///< ID for this communication.
        unsigned int iCmd;      ///< Command.
        unsigned int iDataLen;  ///< The length(in bytes) of the cascaded data after this header.
    };
}; // namespace ipc
}; // namespace rtk

#endif
