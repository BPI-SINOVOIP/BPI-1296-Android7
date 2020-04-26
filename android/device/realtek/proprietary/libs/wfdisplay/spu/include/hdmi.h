#ifndef HDCP2_HDMI_H
#define HDCP2_HDMI_H
/**
 * Functions to handle policing the HDCP 1.x
 *
 * This code must be compiled with the same commands.h as the user-space
 * application or communication errors can occur.
 *
 * @author Brad Bitterman <bitter@vtilt.com>
 */
#include "hdcp2_hal.h"

H2status hdmi_init( void );
int hdmi_CheckHdcp1x( H2bool* bEnabled );
void hdmi_policeEnable( H2bool bEnable );
void hdmi_task( void );
#endif //HDCP2_HDMI_H

