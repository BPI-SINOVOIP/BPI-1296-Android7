////////////////////////////////////////////////////////////////////////////////
/// Just use Visual Studio 2008 build-in debugger to trace memory leak.<BR>
///
/// Notice:<BR> 
////1. <B>This header file should be placed after all of your header files 
/// included in your *.cpp</B><BR>
/// 2. This is valid only for Win32 debug version.
///
/// @see http://msdn.microsoft.com/en-us/library/x98tx3cf(v=vs.90).aspx
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_MEMORY_TRACER__
#define __INCLUDED_MEMORY_TRACER__

#ifdef WIN32
#ifdef _DEBUG
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
    #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #define new DEBUG_NEW
#endif
#endif

#endif