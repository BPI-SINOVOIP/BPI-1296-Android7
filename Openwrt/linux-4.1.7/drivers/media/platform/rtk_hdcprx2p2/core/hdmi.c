/**
 * Functions to handle policing the HDCP 1.x
 *
 * INTEL CONFIDENTIAL
 * Copyright 2010-2011 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to
 * the source code ("Material") are owned by Intel Corporation or its
 * suppliers or licensors.  Title to the Material remains with Intel
 * Corporation or its suppliers and licensors.  The Material contains trade
 * secrets and proprietary and confidential information of Intel or its
 * suppliers and licensors.  The Material is protected by worldwide copyright
 * and trade secret laws and treaty provisions. No part of the Material may
 * be used, copied, reproduced, modified, published, uploaded, posted,
 * transmitted, distributed, or disclosed in any way without Intel's prior
 * express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or
 * delivery of the Materials,  either expressly, by implication, inducement,
 * estoppel or otherwise.  Any license under such intellectual property
 * rights must be express and approved by Intel in writing.
 */

/**
 * This code must be compiled with the same commands.h as the user-space
 * application or communication errors can occur.
 */

/**
 * Includes
 */
#include <hdcp2_hal.h>
#include <hdmi.h>
#include <crypto.h>
#include <hdcp2_interface.h>

/*** Globals ***/
static volatile H2uint32 gHdcp1CheckCount;
static volatile H2bool gbPoliceEnabled = FALSE;

/*** Prototypes ***/

/**
 * Call that checks to see if HDCP 1.x is on. If it is on, the function
 * will atomically increment gHdcp1CheckCount. The police task will
 * periodically check this count to see if we're doing what we're suppose
 * to do.
 *
 * @param [out] bEnabled
 *              Return TRUE if HDCP-1 is on or FALSE if HDCP-1 is off
 *
 * @return Returns H2_OK if successful
 */

/**
 * Turn policing on/off
 *
 */
void hdmi_policeEnable(H2bool bEnable)
{
	gbPoliceEnabled = bEnable;

	return;
}

/**
 * Initialize HDMI policing
 *
 */
H2status hdmi_init()
{

	return H2_OK;
}

/**
 * HDMI policing task
 *
 * Main should call this ~5 seconds
 *
 */
void hdmi_task()
{
	static H2uint32 count;
	unsigned char zks[16];

	if (gbPoliceEnabled && (count == gHdcp1CheckCount)) {

		/* Clear DEMUX KEY */
		memset(zks, 0, sizeof(zks));
		hdcp2_Rx_SetKsXorLc128(zks);
	}

	count = gHdcp1CheckCount;

	return;
}
