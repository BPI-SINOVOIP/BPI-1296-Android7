/*
 * Copyright (C) 2013 Realtek Semiconductor Corp.
 * Author: stanely
 * History:
 *      2013.12.20 : First draft.
 */
 
package com.realtek.realtekinitialsettings;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.util.Log;

/**
 * Implement this class for our purpose
 */
public class Utils {
	public static String TAG = "Utils";

	public Utils() {
	}

	/* Helper function to let us getting the proper image id for signal
	 * strength display.
	*/
	public static int getSignalStrengthImageId(int level, int securityType) {
		int imgId = 0;
		if(AccessPoint.SECURITY_NONE == securityType) {  // no security
			switch(level) {
				case 3:  // full
					imgId = R.drawable.wifi_3;
					break;
				case 2:
					imgId = R.drawable.wifi_2;
					break;
				case 1:
					imgId = R.drawable.wifi_1;
					break;
				case 0:  // 0
				default:
					imgId = R.drawable.wifi_0;
					break;
			}
		} else {  // has security (WEP, WPA)
			switch(level) {
				case 3:  // full
					imgId = R.drawable.wifi_lock_3;
					break;
				case 2:
					imgId = R.drawable.wifi_lock_2;
					break;
				case 1:
					imgId = R.drawable.wifi_lock_1;
					break;
				case 0:  // 0
				default:
					imgId = R.drawable.wifi_lock_0;
					break;
			}
		}

		return imgId;
	}	

}


