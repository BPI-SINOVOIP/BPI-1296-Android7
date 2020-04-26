package com.realtek.hardware;

import com.realtek.hardware.HDMIRxStatus;

interface IRtkHDMIRxService
{
	/* Get HDMIRx status */
	boolean isHDMIRxPlugged();
	
	/* Get HDMIRx status */
	HDMIRxStatus getHDMIRxStatus();

        /*To info AudioPolicy to set Audio*/
	boolean setHDMIRxAudio();

        /*To info AudioPolicy to mute Audio*/
	boolean muteHDMIRxAudio();
}
