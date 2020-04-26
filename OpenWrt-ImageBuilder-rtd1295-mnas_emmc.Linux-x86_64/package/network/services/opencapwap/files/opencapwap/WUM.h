/*
 * WUM.h - WTP Update Messages
 *
 *
 * 4/11/2009 - Donato Capitella (d.capitella@gmail.com)
 */

#ifndef WUM_H
#define WUM_H

#include "CWCommon.h"

#define WTP_VERSION_MAJOR	0
#define WTP_VERSION_MINOR	93
#define WTP_VERSION_REVISION	2


#define WTP_LOCK_FILE           "wtp.lock"

void WTPUpdateAgent(char *CupPath);

#endif /* WUM_H */
