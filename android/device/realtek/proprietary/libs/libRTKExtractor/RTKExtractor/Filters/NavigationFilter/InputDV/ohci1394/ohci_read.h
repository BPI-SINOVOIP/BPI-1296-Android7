#ifndef __OHCI_READ_H__
#define __OHCI_READ_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <assert.h>
#include <time.h>
#include "raw1394/raw1394.h"
#include "raw1394/raw1394_private.h"
#include "avc1394/avc1394.h"
#include "avc1394/avc1394_internal.h"
#include "avc1394/avc1394_vcr.h"
#include "rom1394/rom1394.h"

/////////////////////////////////////////////////////////////////////////////////////
//some avc constant
// for VCR
#define T_CTLVCR0           AVC1394_CTYPE_CONTROL | \
                                                        AVC1394_SUBUNIT_TYPE_TAPE_RECORDER | \
                                                        AVC1394_SUBUNIT_ID_0
#define T_STATVCR0 		AVC1394_CTYPE_STATUS | AVC1394_SUBUNIT_TYPE_TAPE_RECORDER | AVC1394_SUBUNIT_ID_0
                                                                                                     
#if 0
#define T_AVC1394_CTYPE_CONTROL                         0x00000000
#define T_AVC1394_SUBUNIT_TYPE_TAPE_RECORDER            (4 <<19)

#define T_AVC1394_SUBUNIT_ID_0                          (0 << 16)
#define T_AVC1394_SUBUNIT_ID_1 (1 << 16)
#define T_AVC1394_SUBUNIT_ID_2 (2 << 16)
#define T_AVC1394_SUBUNIT_ID_3 (3 << 16)
#define T_AVC1394_SUBUNIT_ID_4 (4 << 16)

                                                                                                     
#define T_AVC1394_VCR_COMMAND_PLAY                      0x00000C300
                                                                                                     
#define T_AVC1394_VCR_OPERAND_PLAY_FORWARD              0x75
#endif 
///////////////////////////////////////////////////////////////////////////////
const char string_not_compatible[] = "\
This libraw1394 does not work with your version of Linux. You need a different\n\
version that matches your kernel (see kernel help text for the raw1394 option to\n\
find out which is the correct version).\n";
                                                                                                     
const char string_not_loaded[] = "\
This probably means that you don't have raw1394 support in the kernel or that\n\
you haven't loaded the raw1394 module.\n";

//struct raw1394_handle *   raw1394_handler= NULL;
//int                         bRunning= 0;

////////////////////////////////////////////////////////////////////////////////////
// forward declaration
raw1394handle_t Open1394Device(int*                pnNodes,int                 nSelectedCardIn);
int DumpNodeUnitInfo( raw1394handle_t             r1394handle, const int                   n1394Nodes);
int AVHDD_SearchNodeId( raw1394handle_t             r1394handle);
int IterationThread(raw1394handle_t        handle, bool &data_ok);
int SetIsoReceiver(raw1394handle_t   handle);
int IsoReceiverEntry(raw1394handle_t  handle,  int     channel,size_t   length, quadlet_t*     data);
int IsoReceiver(int         channle,   size_t      length,  quadlet_t*  data);
int IsoStart( raw1394handle_t        handle);
int IsoStop(raw1394handle_t        handle);
static enum raw1394_iso_disposition
iso_handle1(raw1394handle_t handle, unsigned char *data,
unsigned int length, unsigned char channel,
unsigned char tag, unsigned char sy, unsigned int cycle,
unsigned int dropped);
void milliSleep( long                mSec);

#endif
