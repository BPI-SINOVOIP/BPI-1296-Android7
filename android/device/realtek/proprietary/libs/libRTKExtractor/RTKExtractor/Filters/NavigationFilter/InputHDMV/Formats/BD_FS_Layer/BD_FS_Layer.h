#ifndef _BD_FS_Layer_H_
#define _BD_FS_Layer_H_

#include "UnicodeUtilities.h"
#include "FilePath.h"
#include "BD_FS_rt_stream.h"
#include "SonicCriticalSection.h"

typedef	UInt32	BD_FS_DeviceHandle;

class BD_FS_Layer
{
public : 
	BD_FS_Layer(/*ref_ptr<BD_Settings> pBDSettings = 0, */bool bUseExclusiveAccess = false, bool bLockDriveTray = false);
	~BD_FS_Layer() ;

	// SetDeviceRoot: Will store device root (path the dir above BDMV. stored path will end with a '/')
	//                Path passed in can be path to BDMV dir or its parent and ending it in a '/' is optional.
	SonicError SetDeviceRoot(BD_FS_DeviceHandle deviceHandle, const FilePath fpDriveRoot);
	
	SonicError CreateStream(BD_FS_DeviceHandle deviceHandle, ref_ptr<FilePath> path, BD_FS_rt_stream **ppStream);
	SonicError CloseStream(BD_FS_rt_stream * stream);

	SonicError DoesFileExist(BD_FS_DeviceHandle deviceHandle, ref_ptr<FilePath> path);

	bool DoesFileExist(char* fileName, bool bFullPath = false);

private:
	SonicError TryToCreateStream(char* path, BD_FS_rt_stream **ppStream, bool bTryDirectIOFirst = false);

private:
	SonicCriticalSection 				m_csLock;
	unicode::string8					m_dirRoot;
};

#endif
