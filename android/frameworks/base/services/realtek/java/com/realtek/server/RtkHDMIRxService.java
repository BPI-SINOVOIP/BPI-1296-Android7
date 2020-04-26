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


import com.realtek.hardware.IRtkHDMIRxService;
import com.realtek.hardware.HDMIRxStatus;

public class RtkHDMIRxService extends IRtkHDMIRxService.Stub {
	static {
		System.loadLibrary("realtek_runtime");
	}

	private static final String TAG = "RtkHDMIRxService";

	/**
	 * Binder context for this service
	 */
	private Context mContext;
	private IntentFilter hdmiRxFilter = null;
	private IntentFilter hdmiRxAudioFilter = null;
	private IntentFilter bootCompleteFilter = null;
	private IntentFilter hdmiRxHDCPFilter = null;
	private boolean hdmiRxReceiverRegistered = false;
	private boolean mHdmiRxPlugged = false;
	private boolean mHdmiRxRenderAudio = false;
	private boolean mHdmiRxAudioReady = false;
	private HDMIRxStatus mHDMIRxStatus = new HDMIRxStatus();
	private AudioManager audioManager = null;
	private boolean mHdmiRxHDCPoldState =false;
    private final String HdmiRxSwitch      = "/sys/devices/virtual/switch/rx_video/state";
    private final String HdmiRxAudioSwitch = "/sys/devices/virtual/switch/rx_audio/state";
    private final String HdmiRxUeventKeyword      = "DEVPATH=/devices/virtual/switch/rx_video";
    private final String HdmiRxAudioUeventKeyword = "DEVPATH=/devices/virtual/switch/rx_audio";
    private final String HdmiRxVideoInfo = "/sys/class/video4linux/video250/hdmirx_video_info";
    private final String HdmiRxAudioInfo = "/sys/class/video4linux/video250/hdmirx_audio_info";
	private final String HdmiRxHDCPSwitch = "/sys/devices/virtual/switch/rx_hdcp/state";
    private final String HdmiRxHDCPUeventKeyWord = "DEVPATH=/devices/virtual/switch/rx_hdcp";
	/**
	 * Constructs a new RtkHDMIRxService instance.
	 *
	 */
	public RtkHDMIRxService(Context context) {
		mContext = context;
		Slog.d(TAG, "RtkHDMIRxService is constructed!");
		
		// register the hdmiRxFilter
		hdmiRxFilter = new IntentFilter(HDMIRxStatus.ACTION_HDMIRX_PLUGGED);
		hdmiRxAudioFilter = new IntentFilter(HDMIRxStatus.ACTTION_HDMIRX_AUDIO_CHANGED);
		bootCompleteFilter = new IntentFilter(Intent.ACTION_BOOT_COMPLETED);
		hdmiRxHDCPFilter = new IntentFilter(HDMIRxStatus.ACTTION_HDMIRX_HDCP_CHANGED);
		hookHDMIRxPlugStatus(true);

		// Monitor HDMI Rx device
		initializeHdmiRxState();
		//nativeSetHdmiRxHdcpKey(); //delay until boot completed
	}

	private BroadcastReceiver hdmiRxHotPlugReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			boolean hdmiRxPlugged = intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_PLUGGED_STATE, false);
			if(hdmiRxPlugged) {
				Slog.w(TAG, "hdmiRxPlugged is plugged in\n");
			} else {
				Slog.w(TAG, "hdmiRxPlugged is pulled out\n");
			}
            parseRxInfo();
		}
	};
	private BroadcastReceiver hdmiRxAudioStatusReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			boolean hdmiRxAudioReady = intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_AUDIO_STATE, false);
			if(hdmiRxAudioReady) {
				Slog.w(TAG, "HDMI Rx Audio is ready\n");
			} else {
				Slog.w(TAG, "HDMI Rx Audio is not ready\n");
			}
            parseRxInfo();
		}
	};

    private BroadcastReceiver bootCompleteReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            Slog.w(TAG, "Boot completed. Setting up HDMI Rx HDCP key.");
            nativeSetHdmiRxHdcpKey();
        }
    };

	private BroadcastReceiver hdcpStatusReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			boolean hdcpStatus = intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_HDCP_STATUS, false);
			if(hdcpStatus) {
				Slog.w(TAG, "hdcp is on\n");
			} else {
				Slog.w(TAG, "hdcp is off\n");
			}
		}
	};
	
	private void hookHDMIRxPlugStatus(boolean bEnable) {
		if(bEnable) {
			if(!hdmiRxReceiverRegistered) {
				mContext.registerReceiver(hdmiRxHotPlugReceiver, hdmiRxFilter);
				mContext.registerReceiver(hdmiRxAudioStatusReceiver, hdmiRxAudioFilter);
				mContext.registerReceiver(bootCompleteReceiver, bootCompleteFilter);
				mContext.registerReceiver(hdcpStatusReceiver, hdmiRxHDCPFilter);
				hdmiRxReceiverRegistered = true;
				Slog.w(TAG, "hookHDMIRxPlugStatus register");
				if (null == audioManager)
				    audioManager = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
			}
		}
		else {
			if(hdmiRxReceiverRegistered) {
				mContext.unregisterReceiver(hdmiRxHotPlugReceiver);
				mContext.unregisterReceiver(hdmiRxAudioStatusReceiver);
				mContext.unregisterReceiver(bootCompleteReceiver);
				mContext.unregisterReceiver(hdcpStatusReceiver);
				hdmiRxReceiverRegistered = false;
				Slog.w(TAG, "hookHDMIRxPlugStatus unregister");
			}
		}
	}	
	
	private UEventObserver mHDMIRxObserver = new UEventObserver() {
		@Override
		public void onUEvent(UEventObserver.UEvent event) {
            Slog.w(TAG, "mHDMIRxObserver:onUEvent");
			setHdmiRxPlugged("1".equals(event.get("SWITCH_STATE")));
		}
	};
	
	private UEventObserver mHDMIRxAudioObserver = new UEventObserver() {
		@Override
		public void onUEvent(UEventObserver.UEvent event) {
            boolean state = "1".equals(event.get("SWITCH_STATE"));
            Slog.w(TAG, "[RXA] mHDMIRxAudioObserver:onUEvent "+state);
			setHdmiRxAudioStatus("1".equals(event.get("SWITCH_STATE")));
		}
	};
	
	private UEventObserver mHDMIRxHDCPObserver = new UEventObserver() {
		@Override
		public void onUEvent(UEventObserver.UEvent event) {		
			//Slog.w(TAG, "UEventObserver info isï¼?+event.toString());
			setHdmiRxHDCP("1".equals(event.get("SWITCH_STATE")));		
		}
	};
	
	public boolean isHDMIRxPlugged() {
		return mHdmiRxPlugged;
	}

	public boolean setHDMIRxAudio() {
	    mHdmiRxRenderAudio = true;
	    Slog.w(TAG, "setHDMIRxAudio going to render");
	    if (mHdmiRxAudioReady) {
                audioManager.setHDMIRXstate(true);
            }
	    return mHdmiRxRenderAudio;
	}

	public boolean muteHDMIRxAudio() {
	    mHdmiRxRenderAudio = false;
	    Slog.w(TAG, "muteHDMIRxAudio to mute render");
            audioManager.setHDMIRXstate(false);
	    return mHdmiRxRenderAudio;
	}

	public HDMIRxStatus getHDMIRxStatus() {
		if(mHDMIRxStatus != null) {
			Slog.w(TAG, "***HDMIRxStatus: Type:" + mHDMIRxStatus.type + " Status:" + mHDMIRxStatus.status + " " + mHDMIRxStatus.width + "x" + mHDMIRxStatus.height + " ScanMode:" + mHDMIRxStatus.scanMode + " Color:" + mHDMIRxStatus.color);
			Slog.w(TAG, "***HDMIRxStatus: Audio Status:" + mHDMIRxStatus.audioStatus + " Freq:" + mHDMIRxStatus.freq + " RAW:" + mHDMIRxStatus.spdif);
		} else {
			Slog.w(TAG, "*** mHDMIRxStatus is null!!!\n");
		}
		return mHDMIRxStatus;
	}

	private void setHdmiRxPlugged(boolean state) {
        Slog.w(TAG, "setHdmiRxPlugged: state:" + state);
		//if (mHdmiRxPlugged != state) {
			mHdmiRxPlugged = state;
			
		Intent intent = new Intent(HDMIRxStatus.ACTION_HDMIRX_PLUGGED);
			intent.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
			intent.putExtra(HDMIRxStatus.EXTRA_HDMIRX_PLUGGED_STATE, state);
			mContext.sendStickyBroadcastAsUser(intent, UserHandle.ALL);
            //Slog.w(TAG, "setHdmiRxPlugged: sending intent ACTION_HDMIRX_PLUGGED");
		        if (state == false)
                            audioManager.setHDMIRXstate(false); // to mute AO, avoid "pop noise"
		//}
	}

	private void setHdmiRxAudioStatus(boolean state) {
	    //Slog.w(TAG, "setHdmiRxAudioStatus: state:" + state);
	    mHdmiRxAudioReady = state;
	    Intent intent = new Intent(HDMIRxStatus.ACTTION_HDMIRX_AUDIO_CHANGED);
	    intent.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
	    intent.putExtra(HDMIRxStatus.EXTRA_HDMIRX_AUDIO_STATE, state);
	    mContext.sendStickyBroadcastAsUser(intent, UserHandle.ALL);
            //Slog.w(TAG, "setHdmiRxAudioStatus: sending intent ACTTION_HDMIRX_AUDIO_CHANGED");
            if (mHdmiRxRenderAudio) {
                audioManager.setHDMIRXstate(state);
            } else {
                audioManager.setHDMIRXstate(false);
            }
	}
	
	private void setHdmiRxHDCP(boolean state) {
		Slog.w(TAG, "setHdmiRxHDCP: state:" + state);	
		Intent intent = new Intent(HDMIRxStatus.ACTTION_HDMIRX_HDCP_CHANGED);
		intent.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
		intent.putExtra(HDMIRxStatus.EXTRA_HDMIRX_HDCP_STATUS, state);
		mContext.sendStickyBroadcastAsUser(intent, UserHandle.ALL);
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
            Slog.w(TAG, "Couldn't read Hdmi Rx state from " + path + ": " + ex);
        } catch (NumberFormatException ex) {
            Slog.w(TAG, "Couldn't read Hdmi Rx state from " + path + ": " + ex);
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException ex) {
                }
            }
        }

        Slog.w(TAG, "getSwitchState state:" + state + " path:" + path);
        return state;
    }

	private void initializeHdmiRxState() {
        boolean state = false;

		// watch for HDMI Rx plug messages if HDMI Rx switch exists
		if(new File(HdmiRxSwitch).exists()) {
			Slog.w(TAG, "Initialize HDMI Rx State");
			mHDMIRxObserver.startObserving(HdmiRxUeventKeyword);

            state = getSwitchState(HdmiRxSwitch);

            // This dance forces the code in setHdmiPlugged to run.
            // Always do this so the sticky intent is stuck (to false) if there is no hdmi.
            setHdmiRxPlugged(state);
        } else {
            Slog.w(TAG, "failed to initialize HDMIRx");
        }

		// watch for HDMI Rx Audio messages if HDMI Rx switch exists
		if(new File(HdmiRxAudioSwitch).exists()) {
			Slog.w(TAG, "Initialize HDMI Rx Audio State");
			mHDMIRxAudioObserver.startObserving(HdmiRxAudioUeventKeyword);

            state = getSwitchState(HdmiRxAudioSwitch);

            // This dance forces the code in setHdmiPlugged to run.
            // Always do this so the sticky intent is stuck (to false) if there is no hdmi.
            setHdmiRxAudioStatus(state);
        } else {
            Slog.w(TAG, "failed to initialize HDMIRx Audio");
        }
		if(new File(HdmiRxHDCPSwitch).exists()) {
			Slog.w(TAG, "startObserving  HDMI Rx hdcp State");
			mHDMIRxHDCPObserver.startObserving(HdmiRxHDCPUeventKeyWord);
        } else {
            Slog.w(TAG, "failed to startObserving HDCPUevent");
		}
		
	}	

	private void parseRxInfo() {
		Slog.w(TAG, "*** In parseRxInfo\n");
		
		try {
			BufferedReader in;
			String line;
			String tmp;
			int pos = 0;
			int i = 0;

			// parse HDMI Rx Video Info
			in = new BufferedReader(new FileReader(HdmiRxVideoInfo));

			//Type:HDMIRx
			if((line = in.readLine()) != null) {
				pos = line.indexOf(':');
				line = line.substring(pos+1);
				Slog.w(TAG, "*** Video Type: " + line + "\n");
				if(line.compareToIgnoreCase("HDMIRx") == 0) {
					mHDMIRxStatus.type = HDMIRxStatus.TYPE_HDMI_RX;
				} else {
					mHDMIRxStatus.type = HDMIRxStatus.TYPE_MIPI;	
				}
			}

			//Status:NotReady
			if((line = in.readLine()) != null) {
				pos = line.indexOf(':');
				line = line.substring(pos+1);
				Slog.w(TAG, "*** Video Status: " + line + "\n");
				if(line.compareToIgnoreCase("Ready") == 0) {
					mHDMIRxStatus.status= HDMIRxStatus.STATUS_READY;
				} else {
					mHDMIRxStatus.status = HDMIRxStatus.STATUS_NOT_READY;
				}
			}

			//Width:0
			if((line = in.readLine()) != null) {
				pos = line.indexOf(':');
				line = line.substring(pos+1);
				i = Integer.parseInt(line);
				Slog.w(TAG, "*** Video Width: " + i + " \n");
				mHDMIRxStatus.width = i;
			}
			
			//Height:0
			if((line = in.readLine()) != null) {
				pos = line.indexOf(':');
				line = line.substring(pos+1);
				i = Integer.parseInt(line);
				Slog.w(TAG, "*** Video Height: " + i + " \n");
				mHDMIRxStatus.height= i;
			}
			
			//ScanMode:Progressive
			if((line = in.readLine()) != null) {
				pos = line.indexOf(':');
				line = line.substring(pos+1);
				Slog.w(TAG, "*** Video ScanMode: " + line + "\n");
				if(line.compareToIgnoreCase("Progressive") == 0) {
					mHDMIRxStatus.scanMode = HDMIRxStatus.SCANMODE_PROGRESSIVE;
				} else {
					mHDMIRxStatus.scanMode = HDMIRxStatus.SCANMODE_INTERLACED;
				}
			}
			
			//Color:RGB
			if((line = in.readLine()) != null) {
				pos = line.indexOf(':');
				line = line.substring(pos+1);
				Slog.w(TAG, "*** Video Color: " + line + "\n");
				if(line.compareToIgnoreCase("RGB") == 0) {
					mHDMIRxStatus.color= HDMIRxStatus.COLOR_RGB;
				} else if (line.compareToIgnoreCase("YUV422") == 0) {
					mHDMIRxStatus.color = HDMIRxStatus.COLOR_YUV422;
				} else if (line.compareToIgnoreCase("YUV444") == 0) {
					mHDMIRxStatus.color = HDMIRxStatus.COLOR_YUV444;
				} else {
					mHDMIRxStatus.color = HDMIRxStatus.COLOR_YUV411;
				}
			}
			
			//Fps:0
			if((line = in.readLine()) != null) {
				pos = line.indexOf(':');
				line = line.substring(pos+1);
				i = Integer.parseInt(line);
				Slog.w(TAG, "*** Video Framerate: " + i + " \n");
				mHDMIRxStatus.freq = i;
			}

			in.close(); // Always close a stream when you are done with it.

			// parse HDMI Rx Audio Info
			in = new BufferedReader(new FileReader(HdmiRxAudioInfo));

			//Ready:0
			if((line = in.readLine()) != null) {
				pos = line.indexOf(':');
				line = line.substring(pos+1);

                i = Integer.parseInt(line);
				Slog.w(TAG, "*** Audio Status: " + line + "\n");
                if(i == 1)
                    mHDMIRxStatus.audioStatus = HDMIRxStatus.STATUS_READY;
                else
                    mHDMIRxStatus.audioStatus = HDMIRxStatus.STATUS_NOT_READY;
			}

            //Freq:0
			if((line = in.readLine()) != null) {
				pos = line.indexOf(':');
				line = line.substring(pos+1);
				
				i = Integer.parseInt(line);
				Slog.w(TAG, "*** Audio Samplerate: " + i + " \n");
				mHDMIRxStatus.freq = i;
			}

			//Status:NotReady
			if((line = in.readLine()) != null) {
				pos = line.indexOf(':');
				line = line.substring(pos+1);
				Slog.w(TAG, "*** SPDIF Type: " + line + "\n");
				if(line.compareToIgnoreCase("Non-LPCM") == 0) {
					mHDMIRxStatus.spdif = HDMIRxStatus.SPDIF_RAW;
				} else {
					mHDMIRxStatus.spdif = HDMIRxStatus.SPDIF_LPCM;
				}
			}

			in.close();
			
		} catch (IOException e) {
			// Handle FileNotFoundException, etc. here
			Slog.w(TAG, "*** IOException in parseRxInfo\n");
		}
	}

	private native void nativeSetHdmiRxHdcpKey();
}
