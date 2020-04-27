/*
 * Copyright (C) 2014 Realtek Semiconductor Corp.
 * Author: stanely
 * History:
 *      2014.01.16 : First draft, to register the broadcast receiver (mainly for BOOT_COMPLETED event) so that launch this app at the beginning.
 */

package com.realtek.realtekinitialsettings;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.os.SystemProperties;

public class BootCompletedReceiver extends BroadcastReceiver {

	@Override
	public void onReceive(Context context, Intent intent) {
		// got BOOT_COMPLETED
		Log.d("stanely", "I got BOOT_COMPLETED, let's start RealtekInitialSettings App!!!");

		String propRes = SystemProperties.get("persist.sys.RunRTKIniSettings");
		Log.d("stanely", "onReceiver(), persist.sys.RunRTKIniSettings = " + propRes);
		//if(propRes == null || (propRes != null && propRes.equals("false"))) {
		if(propRes != null && propRes.equals("false")) {
			Log.d("stanely", "I got property, let's start RealtekInitialSettings App!!!");
			Intent newIntent = new Intent(context, InitialSettingsActivity.class);
			newIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			context.startActivity(newIntent);
		}
	}

}
