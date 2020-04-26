////////////////////////////////////////////////////////////////////////////////
/// \class rtk::net::android::IHttpClient
/// JNI Context For Android<BR>
/// <BR>
/// JNIContext provides a way to get the pointer of current Java VM and query
/// Java class.
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_JNI_CONTEXT__
#define __INCLUDED_JNI_CONTEXT__

#include <jni.h>
#include <vector>
#include <string>

namespace rtk      {
namespace os       {
namespace android  {
    class JniContext
    {
        public:
            /// This class is a singleton.<BR>
            /// Please use GetInstance() to access instance.
            ///
            /// @return The singleton instance of JniContext.
            /// 
            static JniContext& GetInstance();
            
            /// Initialize JNI context.<BR>
            /// <B>Please call this function in JNI_OnLoad().</B>
            ///
            /// @param pJVM Current Java VM environment.
            /// @param javaClassList The list of Java classes need to be loaded.
            /// 
            void Initialize(JavaVM* pJVM, std::vector<std::string>& javaClassList);
            
            /// Finalize JNI context.<BR>
            /// <B>Please call this function in JNI_OnUnload().</B>
            ///
            /// @param pJVM Current Java VM environment.
            /// 
            void Finalize(JavaVM* pJVM);
            
            /// Get the poiter of current Java VM instance.
            ///
            /// @return Poiter of current Java VM instance.
            /// 
            JavaVM* GetJVM() const;

            /// Get the instance of Java class by class name.
            ///
            /// @param pStrClassName The name of Java class
            ///
            /// @return The class object of specified class name.<BR>
            ///         0 means cannot find the class.
            ///
            jclass GetJavaClass(const char* pStrClassName);
            
            /// Attach current thread to Java VM.<BR>
            /// You need to call this function to attach your own thread to Java VM.<BR>
            /// Without this step, your JIN will be failed in your thread.
            ///
            /// @return The Java environment for your thread.
            /// 
            JNIEnv* AttachThread();
            
            /// You need to detach your thread after your thread will not use JNI.
            ///
            void DetachThread();
            
        private:
            struct PrivateImpl;
            PrivateImpl* m_pImpl;
            
        private:
            JniContext();
            ~JniContext();
            JniContext(const JniContext&);
            JniContext& operator=(const JniContext&);
    };
}; // namespace android
}; // namespace os
}; // namespace rtk

#endif