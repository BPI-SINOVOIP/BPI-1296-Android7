////////////////////////////////////////////////////////////////////////////////
/// String Utility
///
/// @author: zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_STR_LIB__
#define __INCLUDED_STR_LIB__

namespace rtk   {
namespace tools {

#ifdef WIN32
    #define FILE_PATH_SLASH_CHAR    '\\'
    #define FILE_PATH_SLASH         "\\"
#else
    #define FILE_PATH_SLASH_CHAR    '/'
    #define FILE_PATH_SLASH         "/"
#endif

#ifndef WIN32 // Windows API already support strlwr() and strupr().
    char* strlwr(char* pStrBuf); // Convert string to lower case
    char* strupr(char* pStrBuf); // Convert string to upper case
#endif

/// Extracting path from a full path.<BR>
/// For example: Input = C:\ABC\TEST\1.mp3, Output = C:\ABC\TEST
///
/// @param pFullPath The full path wants to parse.
///
/// @return A new allocated memory contained the result string.<BR>
///         The caller needs to call delete[] to free memory.
///
char* ExtractFilePath(const char* pFullPath);

/// Extract file extension from a path.<BR>
/// For example: Input = C:\ABC\TEST\1.mp3, Output = .mp3
///
/// @param pFilePath The file path wants to parse.
///
/// @return A new allocated memory contained the result string.<BR>
///         The caller needs to call delete[] to free memory.<BR>   
///         The result string contains '.'
///
char* ExtractFileExtension(const char* pFilePath);

/// Is the pFilePath is a file with file extension pFileExt.<BR
/// For example: IsFileExt("c:/123.m3u", ".m3u") return true<BR>
///              IsFileExt("c:/123.m3u", ".txt") return false<BR>
/// Note: The string comparison is not case sensitive. .M3U is equal .m3u 
/// 
/// @param pFilePath The file path wants to check.
/// @param pFileExt The file extension want to verify.<BR>
///                 This file extension must contain '.'
///
/// @return true --> pFilePath is a file with extension pFileExt. 
///
bool IsFileExt(const char* pFilePath, const char* pFileExt);

/// Extract file name from a path.
/// For example: Input = C:\ABC\TEST\1.mp3, Output = 1.mp3
///
/// @param pFilePath The file path wants to parse.
///
/// @return A new allocated memory contained the result string.<BR>
///         The caller needs to call delete[] to free memory.
///
char* ExtractFileName(const char* pFilePath);

/// Compare if str1 and str2 are the same string ignoring case sensitivity.
///
/// @param str1 String which to be compared
/// @param str2 String which to be compared
///
/// @return 0 --> str1 is equal to str2
///
int StrCmpIgnoreCase(const char* str1, const char* str2);

}; // namespace tools
}; // namespace rtk

#endif
