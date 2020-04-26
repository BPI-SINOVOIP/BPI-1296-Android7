/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef WMDMRND_RESULTS_H
#define WMDRMND_RESULTS_H

#include <drmtypes.h>

//typedef DRM_WORD WMDRMND_RESULT;
#define WMDRMND_RESULT DRM_WORD

#define WMDRMND_SUCCESS                          ((WMDRMND_RESULT)   0)
#define WMDRMND_E_INVALIDCERTIFICATE             ((WMDRMND_RESULT) 100)
#define WMDRMND_E_CERTIFICATEREVOKED             ((WMDRMND_RESULT) 101)
#define WMDRMND_E_MUSTAPPROVE                    ((WMDRMND_RESULT) 102)
#define WMDRMND_E_LICENSEUNAVAILABLE             ((WMDRMND_RESULT) 103)
#define WMDRMND_E_TRANSMITTERFAILURE             ((WMDRMND_RESULT) 104)
#define WMDRMND_E_DEVICELIMITREACHED             ((WMDRMND_RESULT) 105)
#define WMDRMND_E_UNABLETOVERIFYPROXIMITY        ((WMDRMND_RESULT) 106)
#define WMDRMND_E_MUSTREGISTER                   ((WMDRMND_RESULT) 107)
#define WMDRMND_E_MUSTREVALIDATE                 ((WMDRMND_RESULT) 108)
#define WMDRMND_E_INVALIDPROXIMITYRESPONSE       ((WMDRMND_RESULT) 109)
#define WMDRMND_E_INVALIDSESSION                 ((WMDRMND_RESULT) 110)
#define WMDRMND_E_UNABLETOOPENFILE               ((WMDRMND_RESULT) 111)
#define WMDRMND_E_UNSUPPORTEDPROTOCOLVERSION     ((WMDRMND_RESULT) 112)
#define WMDRMND_E_BADREQUEST                     ((WMDRMND_RESULT) 113)

#endif /* WMDRMND_RESULTS */
