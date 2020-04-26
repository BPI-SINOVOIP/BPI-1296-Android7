////////////////////////////////////////////////////////////////////////////////
/// \class rtk::ipc::IpcClientFactory
/// Inter-Process Communication Client Factory
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_CLIENT_FACTORY
#define INCLUDED_IPC_CLIENT_FACTORY

namespace rtk {
namespace ipc {
    class IpcClient;
    class IpcClientFactory
    {
        public:
            static IpcClientFactory& GetInstance();
            
            /// Create a IpcClient based on IPC Implementation mode.
            ///
            /// @param iIpcImplMode IpcClientFactory will choose an implementation 
            ///                     of IPC depended on the mode you selected.<BR>
            ///                     The modes are define in IpcMode.h
            ///
            /// @return Return the created IPC client. NULL means failed.
            ///
            IpcClient* Create(int iIpcImplMode);
            
            /// Destroy the IPC client.
            ///
            /// @param pClient The client want to be destroyed.
            ///
            void Destroy(IpcClient* pClient);
    
        private:
            IpcClientFactory();
            IpcClientFactory(const IpcClientFactory&);
            IpcClientFactory& operator=(const IpcClientFactory&);
            ~IpcClientFactory();
            
        private:
            struct PrivateImpl;
            PrivateImpl* m_pImpl;
    };
}; // namespace ipc
}; // namespace rtk

#endif
