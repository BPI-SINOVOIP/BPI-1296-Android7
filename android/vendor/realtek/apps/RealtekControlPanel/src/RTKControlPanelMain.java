package com.rtk.ControlPanel;
 
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class RTKControlPanelMain extends BroadcastReceiver {

	@Override
	public void onReceive(Context arg0, Intent arg1) {
		// TODO Auto-generated method stub
		Log.d("ControlPanelService", "Start RTK ControlPanel Service");
		Intent intent = new Intent(arg0, com.rtk.ControlPanel.RTKControlPanelService.class);
		arg0.startService(intent);
	}
}

