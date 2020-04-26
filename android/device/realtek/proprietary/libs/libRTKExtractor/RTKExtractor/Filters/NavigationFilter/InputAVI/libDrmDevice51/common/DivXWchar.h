/*!
    @file
@verbatim
$Id$

Copyright (c) 2006 DivX, Inc. All rights reserved.

This software is the confidential and proprietary information of
DivX, Inc. and may be used only in accordance with the terms of
your license from DivX, Inc.
@endverbatim

    This header provides a platform-independent method for including
    wchar support, as well as providing some compiler independent
    function translations.

    This header actually doesn't perform any typedefs, it simply
    includes <b>\<wchar.h\></b> (assumed to be includable from the
    project's path).

    It provides function translations for the following functions which
    have implementation differences in Microsoft and GCC libraries.
    - swprintf    @htmlonly &hArr; @endhtmlonly   _snwprintf
    - snprintf    @htmlonly &hArr; @endhtmlonly   _snprintf
    - strcasecmp  @htmlonly &hArr; @endhtmlonly   stricmp
    - strncasecmp @htmlonly &hArr; @endhtmlonly   strnicmp
    - _stricmp    @htmlonly &rArr; @endhtmlonly   strcasecmp
    - _strnicmp   @htmlonly &rArr; @endhtmlonly   strncasecmp

    The following translations are additionally performed in Microsoft environments:
    - _snwprintf  @htmlonly &rArr; @endhtmlonly   swprintf
    - wcstok( wcs, delim, state )   @htmlonly &rArr; @endhtmlonly
        wcstok( wcs, delim )


    The following translations are performed in addition to the base set
    (and not the Microsoft set above) only in GCC environments:
    - wsprintf        @htmlonly &rArr; @endhtmlonly   sprintf


    <br>The set of function mappings is switched by utilizing the foloowing
    macros that are expected to be set by the compiler:
    - <b>_MSC_VER</b>
    - <b>__GNUC__</b>

    <br><b>NOTES:</b>
    - The <b>strtok()</b> functions are <b>not thread safe</b>, 
    <b>modify the search string</b> and are also a <b>security
    vulnerability</b>.
    - Also related to the <b>strtok()</b> functions is the fact that
    their translation only works from GCC style calls to Microsoft
    compatible calls.
    - Consequently, these functions should not be used. Please see
    Architecture for alternatives.
**/

/*  START PROTECTION  */
#ifndef _DIVXWCHAR_H_
#define _DIVXWCHAR_H_

/*!
    Only <wchar.h> should be included by this file, no others.
**/


#ifdef _MSC_VER /* prevent next line creating "symbol undefined" warning */
#if ( _MSC_VER >= 600 )

/*
    _MSC_VER - assuming microsoft compiler, post VS6
                this will accommodate VS6, VS2003 and VS2005 SDKs for WIN32 platforms
*/

#include <wchar.h>


#define snprintf        _snprintf
#define swprintf        _snwprintf
#define snwprintf       _snwprintf

#define strcasecmp      stricmp
#define strncasecmp     strnicmp

#define wcstok( wcs, delim, state )     wcstok( wcs, delim )


#define _DIVXWCHAR_DEFINED


#endif  /*_MSC_VER >= 600*/
#endif  /*_MSC_VER*/



#ifdef __GNUC__ /* prevent next line creating "symbol undefined" warning */
#if     ( __GNUC__ > 2 ) || ( ( __GNUC__ == 2 ) && ( __GNUC_MINOR__ >= 95 ) )

/*
    __GNUC__ - assuming GCC, post 2.95
               accommodates Linux, MacOS and many CE platforms
*/

#ifdef  _DIVXWCHAR_DEFINED
#error MORE THAN ONE COMPILER FLAG
#endif  /*_DIVXWCHAR_DEFINED*/


#ifndef _DIVX_DOXYGEN_PREPROCESSOR

#include <wchar.h>


#define wsprintf        sprintf
#define _snprintf       snprintf
#define _snwprintf      swprintf

#define stricmp         strcasecmp
#define strnicmp        strncasecmp

#define _stricmp        strcasecmp
#define _strnicmp       strncasecmp

// #define wcstok( wcs, delim )     wcstok( wcs, delim, state ) -- CAN'T DO THIS!!


#define _DIVXWCHAR_DEFINED


#endif  /*_DIVX_DOXYGEN_PREPROCESSOR*/

#endif  /*__GNUC__ > 2.95*/
#endif  /*__GNUC__*/



/*
    ALL
*/

#ifndef _DIVXWCHAR_DEFINED
#error  THE COMPILER YOU ARE USING IS NOT RECOGNIZED BY THIS HEADER
#else   /*_DIVXWCHAR_DEFINED*/
#undef  _DIVXWCHAR_DEFINED  /* remove this label, it's purpose fulfilled */
#endif  /*_DIVXWCHAR_DEFINED*/



#endif /* _DIVXWCHAR_H_ */

/****** END OF FILE *********************************************************/
