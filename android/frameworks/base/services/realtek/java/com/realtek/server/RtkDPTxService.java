package com.realtek.server;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.BufferedReader;

import android.util.Slog;
import android.content.Context;
import android.os.UEventObserver;
import android.os.Handler;
import android.os.Message;
import android.os.UserHandle;
import android.os.IBinder;
import android.os.PowerManager;
import android.os.RemoteException;
import android.os.SystemProperties;
import android.os.UEventObserver;
import android.content.ContentResolver;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.AudioManager;


import com.realtek.hardware.IRtkDPTxService;

public class RtkDPTxService extends IRtkDPTxService.Stub {
	static {
		System.loadLibrary("realtek_runtime");
	}

	private static final String TAG = "RtkDPTxService";

	/**
	 * Binder context for this service
	 */
	private Context mContext;
	private IntentFilter DPTxFilter = null;
	private boolean DPTReceiverRegistered = false;
	private boolean mDPTxPlugged = false;

    private final String DPTxSwitch = "/sys/devices/virtual/switch/dptx/state";
    private final String DPTxUeventKeyword = "DEVPATH=/devices/virtual/switch/dptx";

	public final static String ACTION_DPTX_PLUGGED = "android.intent.action.DPTX_PLUGGED";
	public final static String EXTRA_DPTX_PLUGGED_STATE = "state";

	/**
	 * Constructs a new RtkDPTxService instance.
	 *
	 */
	public RtkDPTxService(Context context) {
		mContext = context;
		Slog.w(TAG, "RtkDPTxService is constructed!");

		// register the TPTxFilter
		DPTxFilter = new IntentFilter(ACTION_DPTX_PLUGGED);
		hookDPTxPlugStatus(true);


		initMediaLinkController();
		// Monitor HDMI Rx device
		initializeDPTxState();		
	}

	private BroadcastReceiver dpTxHotPlugReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			boolean DPTxPlugged = intent.getBooleanExtra(EXTRA_DPTX_PLUGGED_STATE, false);
			int state = 0;
			if(DPTxPlugged) {
				state = 1;
				Slog.w(TAG, "DPTxPlugged is plugged in\n");
			} else {
				Slog.w(TAG, "DPTxPlugged is pulled out\n");
			}
			handleHotPlug(state);
		}
	};

	private void hookDPTxPlugStatus(boolean bEnable) {
		if(bEnable) {
			if(!DPTReceiverRegistered) {
				mContext.registerReceiver(dpTxHotPlugReceiver, DPTxFilter);
				DPTReceiverRegistered = true;
				Slog.w(TAG, "hookDPTxPlugStatus register");
			}
		}
		else {
			if(DPTReceiverRegistered) {
				mContext.unregisterReceiver(dpTxHotPlugReceiver);
				DPTReceiverRegistered = false;
				Slog.w(TAG, "hookDPTxPlugStatus unregister");
			}
		}
	}	
	
	private UEventObserver mDPTxObserver = new UEventObserver() {
		@Override
		public void onUEvent(UEventObserver.UEvent event) {
			setDPTxPlugged("1".equals(event.get("SWITCH_STATE")));
		}
	};
	
	
	public boolean checkifDPTxPlugged() {
		synchronized (this) {
			return _checkifDPTxPlugged();
		}
	}

	public boolean checkIfDPTxEDIDReady() {
		synchronized(this) {
			return _checkIfDPTxEDIDReady();
		}
	}

	private void setDPTxPlugged(boolean state) {
		if (mDPTxPlugged != state) {
			mDPTxPlugged = state;
			
			Intent intent = new Intent(ACTION_DPTX_PLUGGED);
			intent.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
			intent.putExtra(EXTRA_DPTX_PLUGGED_STATE, state);
			mContext.sendStickyBroadcastAsUser(intent, UserHandle.ALL);
		}
	}

    private boolean getSwitchState(String path) {
        boolean state = false;
        FileReader reader = null;
        try {
            reader = new FileReader(path);
            char[] buf = new char[15];
            int n = reader.read(buf);
            if (n > 0) {
                state = 0 != Integer.parseInt(new String(buf, 0, 1));
            }
        } catch (IOException ex) {
            Slog.w(TAG, "Couldn't read DP Tx state from " + path + ": " + ex);
        } catch (NumberFormatException ex) {
            Slog.w(TAG, "Couldn't read DP Tx state from " + path + ": " + ex);
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException ex) {
                }
            }
        }

        Slog.d(TAG, "getSwitchState state:" + state + " path:" + path);
        return state;
    }

	private void initializeDPTxState() {
        boolean state = false;

		// watch for HDMI Rx plug messages if HDMI Rx switch exists
		if(new File(DPTxSwitch).exists()) {
			Slog.w(TAG, "Initialize DP Tx State");
			mDPTxObserver.startObserving(DPTxUeventKeyword);

            state = getSwitchState(DPTxSwitch);

            // This dance forces the code in setHdmiPlugged to run.
            // Always do this so the sticky intent is stuck (to false) if there is no hdmi.
            setDPTxPlugged(state);
        } else {
            Slog.w(TAG, "failed to initialize DP Tx");
        }
	}	

	public int getTVSystem() {

		String tvSystem = SystemProperties.get("persist.rtk.hdmitx.dp_user", "0");
		
		return Integer.parseInt(tvSystem);
	}

	public int setTVSystem(int inputTvSystem) {

            Slog.d(TAG, "[DPTX2] setTVSystem: "+inputTvSystem);

			final int tvSystem = inputTvSystem;
			Thread thread = new Thread(){
				@Override
				public void run()  {
					String stringValue = Integer.toString(tvSystem);
					SystemProperties.set("persist.rtk.hdmitx.dp_user", stringValue);
					
					changeTVSystem(tvSystem);
				}
			};

			thread.start();

			return 0;
	}

	private int changeTVSystem(int tvSystem) {
		synchronized(this) {
			Slog.w(TAG, Thread.currentThread().getName() + ": change TV System: " + tvSystem + "!!\n");
			// Get the current TV system

			int intSetTVsystem = -1; //Auto
			
            /*
            // convert setting value to video system
			if (tvSystem == RtkTVSystem.TV_SYS_DP_2160P_30HZ)
				intSetTVsystem = RtkTVSystem.VIDEO_DP_2160P_30HZ;
			else if (tvSystem == RtkTVSystem.TV_SYS_DP_1080P_60HZ)
				intSetTVsystem = RtkTVSystem.VIDEO_DP_1080P_60HZ;
			else if (tvSystem == RtkTVSystem.TV_SYS_DP_720P_60HZ)
				intSetTVsystem = RtkTVSystem.VIDEO_DP_720P_60HZ;
			*/

			nativeSetTVSystem(tvSystem);

			// reset zoom
			//if(mgr.isZooming() == true) {
				// reset zoom
				//mgr.resetZoom();
			//}
			Slog.w(TAG, Thread.currentThread().getName() + ": change TV System: " + tvSystem + " done!!\n");
		}
		return 0;
	}

	private native boolean _checkifDPTxPlugged();
	private native boolean _checkIfDPTxEDIDReady();
	//private native RtkTVSystem[] _getVideoFormat();
	private native void nativeSetTVSystem(int select);
	private native void handleHotPlug(int dptxPlugged);
	private native int initMediaLinkController();
}
