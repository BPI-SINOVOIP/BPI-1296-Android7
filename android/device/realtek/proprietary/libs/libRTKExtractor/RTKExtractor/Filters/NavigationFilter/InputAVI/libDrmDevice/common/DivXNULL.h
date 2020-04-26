/*!
    @file
@verbatim
$Id: DivXNULL.h 1701 2006-05-05 00:45:26Z spriyadarshi $

Copyright (c) 2006 DivX, Inc. All rights reserved.

This software is the confidential and proprietary information of
DivX, Inc. and may be used only in accordance with the terms of
your license from DivX, Inc.
@endverbatim

    This file defines a platform independent NULL value.
**/

/*  START PROTECTION  */
#ifndef _DIVXTYPESNULL_H_
#define _DIVXTYPESNULL_H_

/*!
    No files should be included by this file.
    Nothing but typedefs should be placed in this file.
**/



/*!
    This macro provides a platform independent NULL value, iff it
    has not already been defined. However, the language of the
    implementation file that includes this file will control the actual
    define:
@verbatim
*.C :  #define NULL    ( (void *) 0 )
*.CPP: #define NULL    0
@endverbatim
**/

#ifndef NULL


#ifdef __cplusplus

#define NULL    0

#else   /*__cplusplus*/

#define NULL    ( (void *) 0 )

#endif  /*__cplusplus*/


#endif  /*NULL*/



#endif /* _DIVXTYPESNULL_H_ */

/****** END OF FILE *********************************************************/
