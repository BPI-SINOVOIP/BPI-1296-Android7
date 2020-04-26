/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMERR_H__
#define __DRMERR_H__

#define ChkDR(expr) {               \
            dr = (expr);            \
            if ( DRM_FAILED(dr) )   \
            {                       \
                goto ErrorExit;     \
            }                       \
        }

#define ChkMem(expr) {               \
            if ( NULL == (expr) )    \
            {                        \
                TRACE( ("Allocation failure at %s : %d.\n%s\n", __FILE__, __LINE__, #expr) ); \
                dr = DRM_E_OUTOFMEMORY; \
                goto ErrorExit;     \
            }                       \
        }

#define ChkArg(expr) {               \
            if ( !(expr) )       \
            {                       \
                TRACE( ("Invalid argument at %s : %d.\n%s\n", __FILE__, __LINE__, #expr) ); \
                dr = DRM_E_INVALIDARG; \
                goto ErrorExit;     \
            }                       \
        }

#define ChkDRMString(s) {    \
            if ( !(s) || (s)->pwszString==NULL || (s)->cchString==0 )   \
            {                       \
                TRACE( ("Invalid argument at %s : %d.\n%s\n", __FILE__, __LINE__, #s) ); \
                dr = DRM_E_INVALIDARG; \
                goto ErrorExit;     \
            }                       \
        }

#define ChkWR(expr) {               \
            if (!(expr))            \
            {\
                dr = GetLastError();\
                dr = HRESULT_FROM_WIN32(dr);\
                goto ErrorExit;     \
            }\
        }

#define ChkBOOL(fExpr,err){  \
            if (!(fExpr))            \
            {\
                dr = (err);\
                goto ErrorExit;     \
            }\
        }

#define ChkFAIL(expr) ChkBOOL(expr,DRM_E_FAIL)

#define ChkDRContinue(exp) \
{                          \
    dr=(exp);              \
    if (DRM_FAILED (dr))   \
    {                      \
        continue;          \
    }                      \
}    

#define ChkOverflow(a,b) do {                       \
            if (a < b)                              \
            {                                       \
                ChkDR(DRM_E_ARITHMETIC_OVERFLOW);   \
            }                                       \
        } while(FALSE)

#define SAFE_CHKDR_GETLASTERROR                         \
    {                                                   \
        dr = GetLastError( ) \
        ChkDR( HRESULT_FROM_WIN32( dr ) ); \
        ChkDR( E_UNEXPECTED );                          \
    }

#endif /* __DRMERR_H__ */
