////////////////////////////////////////////////////////////////////////////////
/// Inter-Process Communication Implementation Mode
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_MODE
#define INCLUDED_IPC_MODE

namespace rtk {
namespace ipc {
    /// The implementation of IPC.
    /// IpcServerManager/ will choose an implementation of IPC 
    /// depended on the mode you selected.
    enum IPC_IMPL_MODE 
    { 
        IPC_IMPL_PIPE,    ///< Pipe implementation
        IPC_IMPL_SOCKET,  ///< Socket implementation. NOT IMPLEMENT!!
        IPC_IMPL_MMAP,    ///< Shared memory implementation. NOT IMPLEMENT!!
    };
}; // namespace ipc
}; // namespace rtk

#endif
