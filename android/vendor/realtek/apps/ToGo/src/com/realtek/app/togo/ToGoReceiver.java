package com.realtek.app.togo;

import android.content.Intent;
import android.util.Log;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.app.NotificationManager;
//import android.support.v4.app.NotificationCompat;
import android.app.Notification;
import android.app.Notification.InboxStyle;
import com.realtek.app.togo.ToGoClientStatus;
import android.os.Bundle;

public class ToGoReceiver extends BroadcastReceiver {
	private final static String TAG = "ToGoReceiver";
	private final static String ACT = "com.realtek.app.togo.action.TOGO_NOTIFY";
	private final int mId = 1000;
	private String[] mClientType;
	private String[] mClientState;
	
	@Override
	public void onReceive(Context context, Intent intent) {
		if (Intent.ACTION_BOOT_COMPLETED.equals(intent.getAction())) {
			Intent service = new Intent(context, ToGoAppService.class);
			context.startService(service);
			Log.v(TAG, "got ACTION_BOOT_COMPLETED.");
		}
		else if (ACT.equals(intent.getAction())) {
			Log.v(TAG, "got TOGO_NOTIFY.");

			mClientType = new String [2];
			mClientState = new String [2];

			Bundle bundle = intent.getExtras();
			ToGoClientStatus mClientStatus = bundle.getParcelable("TOGO_CLIENT_STATUS");
			Log.v(TAG, "mClientStatus.count=" + mClientStatus.mCount);
			for (int i=0;i<mClientStatus.mCount;i++) {
				Log.v(TAG, "mClientStatus.i="+i+".id="+mClientStatus.mClientId[i]);
				Log.v(TAG, "mClientStatus.i="+i+".type="+mClientStatus.mSourceType[i]);
				Log.v(TAG, "mClientStatus.i="+i+".state="+mClientStatus.mState[i]);
				switch (mClientStatus.mSourceType[i]) {
					case ToGoSourceStatus.ID_OF_CAMERA:
						mClientType[i] = ToGoSourceStatus.SOURCE_CAMERA;
						break;
					case ToGoSourceStatus.ID_OF_HDMI_RX:
						mClientType[i] = ToGoSourceStatus.SOURCE_HDMI_RX;
						break;
					case ToGoSourceStatus.ID_OF_FILE:
						mClientType[i] = ToGoSourceStatus.SOURCE_FILE;
						break;
					case ToGoSourceStatus.ID_OF_DTV:
						mClientType[i] = ToGoSourceStatus.SOURCE_DTV;
						break;
				}
				switch (mClientStatus.mState[i]) {
					case ToGoClientStatus.ID_OF_STATE_IDLE:
						mClientState[i] = ToGoClientStatus.TOGO_STATE_IDLE;
						break;
					case ToGoClientStatus.ID_OF_STATE_INITED:
						mClientState[i] = ToGoClientStatus.TOGO_STATE_INITED;
						break;
					case ToGoClientStatus.ID_OF_STATE_RUN:
						mClientState[i] = ToGoClientStatus.TOGO_STATE_RUN;
						break;
					case ToGoClientStatus.ID_OF_STATE_STOP:
						mClientState[i] = ToGoClientStatus.TOGO_STATE_STOP;
						break;
				}
			}

			NotificationManager mNotificationManager = (NotificationManager)(context.getSystemService(Context.NOTIFICATION_SERVICE));
			if (mClientStatus.mCount > 0) {
				//NotificationCompat.Builder mBuilder = new NotificationCompat.Builder(context);
				Notification.Builder mBuilder = new Notification.Builder(context);
				mBuilder.setSmallIcon(R.drawable.ic_stat_notify_robot);
				mBuilder.setContentTitle("ToGo Notification");
				//mBuilder.setContentText("This is ToGo Notification.");

				Notification.InboxStyle inboxStyle = new Notification.InboxStyle();
				for (int i=0;i<mClientStatus.mCount;i++) {
					inboxStyle.addLine("ToGo "+mClientType[i]+"."+mClientState[i]+".client id="+mClientStatus.mClientId[i]);
				}
				mBuilder.setStyle(inboxStyle);

				mNotificationManager.notify(mId, mBuilder.build());
				Log.v(TAG, "notify done.");
			}
			else {
				mNotificationManager.cancelAll();
			}
		}
		else {
			Log.v(TAG, "got other broadcast.");
		}		
	}
}