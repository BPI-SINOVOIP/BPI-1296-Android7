/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMERR_H__
#define __DRMERR_H__


#if DBG

extern void (*g_pfDebugAnalyzeDR)(unsigned long);

#define SetDbgAnalyzeFunction(pfn) g_pfDebugAnalyzeDR = pfn;

#define ExamineDRValue(_drval_)                   \
            if(g_pfDebugAnalyzeDR != NULL)        \
            {                                     \
                (*g_pfDebugAnalyzeDR)((_drval_)); \
            }


#else  //DBG


#define SetDbgAnalyzeFunction(pfn)
#define ExamineDRValue(_drval_)


#endif  //DBG


#define ChkDR(expr) {               \
            dr = (expr);            \
            ExamineDRValue(dr);     \
            if ( DRM_FAILED(dr) )   \
            {                       \
                goto ErrorExit;     \
            }                       \
        }

#define ChkMem(expr) {               \
            if ( NULL == (expr) )    \
            {                        \
                TRACE( ((DRM_CHAR *)"Allocation failure at %s : %d.\n%s\n", __FILE__, __LINE__, #expr) ); \
                dr = DRM_E_OUTOFMEMORY; \
                ExamineDRValue(dr);     \
                goto ErrorExit;     \
            }                       \
        }

#define ChkArg(expr) {               \
            if ( !(expr) )       \
            {                       \
                TRACE( ((DRM_CHAR *)"Invalid argument at %s : %d.\n%s\n", __FILE__, __LINE__, #expr) ); \
                dr = DRM_E_INVALIDARG; \
                ExamineDRValue(dr);    \
                goto ErrorExit;     \
            }                       \
        }

#define ChkDRMString(s) {    \
            if ( !(s) || (s)->pwszString==NULL || (s)->cchString==0 )   \
            {                       \
                TRACE( ((DRM_CHAR *)"Invalid argument at %s : %d.\n%s\n", __FILE__, __LINE__, #s) ); \
                dr = DRM_E_INVALIDARG; \
                ExamineDRValue(dr);    \
                goto ErrorExit;     \
            }                       \
        }
        
#define ChkDRMANSIString(s) {    \
            if ( !(s) || (s)->pszString==NULL || (s)->cchString==0 )   \
            {                       \
                TRACE( ((DRM_CHAR *)"Invalid argument at %s : %d.\n%s\n", __FILE__, __LINE__, #s) ); \
                dr = DRM_E_INVALIDARG; \
                ExamineDRValue(dr);    \
                goto ErrorExit;     \
            }                       \
        }        

#define ChkWR(expr) {               \
            if (!(expr))            \
            {\
                dr = GetLastError();\
                dr = HRESULT_FROM_WIN32(dr);\
                ExamineDRValue(dr); \
                goto ErrorExit;     \
            }\
        }

#define ChkBOOL(fExpr,err){  \
            if (!(fExpr))            \
            {\
                dr = (err);\
                ExamineDRValue(dr);  \
                goto ErrorExit;     \
            }\
        }

#define ChkFAIL(expr) ChkBOOL(expr,DRM_E_FAIL)

#define ChkDRContinue(exp) \
{                          \
    dr=(exp);              \
    ExamineDRValue(dr);    \
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

#define ChkOverflowUI64(a,b) do {                   \
            if ( DRM_UI64Les( a, b ) )              \
            {                                       \
                ChkDR(DRM_E_ARITHMETIC_OVERFLOW);   \
            }                                       \
        } while(FALSE)

#define ChkOverflowI64(a,b) do {                    \
            if ( DRM_I64Les( a, b ) )               \
            {                                       \
                ChkDR(DRM_E_ARITHMETIC_OVERFLOW);   \
            }                                       \
        } while(FALSE)

#define ChkBoundsLT( index, arrSize )               \
do                                                  \
{                                                   \
    if ( index >= arrSize )                         \
    {                                               \
        ChkDR( DRM_E_BUFFER_BOUNDS_EXCEEDED );      \
    }                                               \
}                                                   \
while ( FALSE )

#define SAFE_CHKDR_GETLASTERROR                         \
    {                                                   \
        dr = GetLastError( );                           \
        ChkDR( HRESULT_FROM_WIN32( dr ) );              \
        ChkDR( E_UNEXPECTED );                          \
    }
                                                                                                                                         
#define ChkDRMap( expr, drOriginal, drMapped ) do{                                                                        \
            dr = ( expr );                                                                                                \
            ExamineDRValue(dr);                                                                                           \
            if ( dr == ( drOriginal ) )                                                                                   \
            {                                                                                                             \
                TRACE( ((DRM_CHAR *)"Error code 0x%X mapped at %s : %d. to 0x%X \n", dr,  __FILE__, __LINE__, drMapped) );            \
                dr = ( drMapped );                                                                                        \
                ExamineDRValue(dr);                                                                                       \
            }                                                                                                             \
            if ( DRM_FAILED( dr ) )                                                                                       \
            {                                                                                                             \
                goto ErrorExit;                                                                                           \
            }                                                                                                             \
     }while( FALSE )   

#define MapDR( drOriginal, drMapped ) do{                                                                                         \
                TRACE( ((DRM_CHAR *)"Error code 0x%X mapped at %s : %d. to 0x%X \n", drOriginal,  __FILE__, __LINE__, drMapped) );            \
                drOriginal = ( drMapped );                                                                                        \
     }while( FALSE )   

#define AssertChkArg(expr)      \
    do {                        \
        DRM_BOOL _f = (expr);   \
        DRMASSERT( _f );        \
        ChkArg( _f );           \
    } while(FALSE)

#endif /* __DRMERR_H__ */
