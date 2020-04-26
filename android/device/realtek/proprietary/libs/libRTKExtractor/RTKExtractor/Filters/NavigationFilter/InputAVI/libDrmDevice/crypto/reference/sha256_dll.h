// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SHA256_DLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SHA256_DLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SHA256_DLL_EXPORTS
#define SHA256_DLL_API __declspec(dllexport)
#else
#define SHA256_DLL_API __declspec(dllimport)
#endif

// This class is exported from the sha256_dll.dll
class SHA256_DLL_API Csha256_dll {
public:
	Csha256_dll(void);
	// TODO: add your methods here.
};

extern SHA256_DLL_API int nsha256_dll;

SHA256_DLL_API int fnsha256_dll(void);
