/*
 * Copyright (c) 2013 Realtek Semi. co. All Rights Reserved.
 * Author Kevin Su
 * Version V0.1
 * Date:  2013-08-09
 * Comment:This class lets you access the HDMI function.
 */

package com.realtek.hardware;

import java.util.StringTokenizer;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;
import android.os.SystemProperties;

import com.realtek.hardware.RtkTVSystem;
import com.realtek.hardware.IRtkHDMIService;
import com.realtek.hardware.IRtkHDMICallback;

public class RtkHDMIManager {
	private static final String TAG = "RtkHDMIManager";

	private IRtkHDMIService mRtkHDMIService;

    public static final int EVENT_HDMI_PLUGIN   = 1;
    public static final int EVENT_HDMI_PLUGOUT  = 2;
    public static final int EVENT_HDCP_CHANGED  = 3;

    public static final int HDCP_VERSION_NA     = -1;
    public static final int HDCP_VERSION_NONE   =  0;
    public static final int HDCP_VERSION_14     = 14;
    public static final int HDCP_VERSION_22     = 22;

    private static final String PROPERTY_VIDEO_COLOR_MODE = "persist.rtk.video.colormode";
    private static final String PROPERTY_VIDEO_COLOR_DEEP = "persist.rtk.deepcolor";

    public interface IRtkHDMIManagerEventListener {
        public void onHandleEvent(int event);
    }

    private IRtkHDMIManagerEventListener mEventListener = null;
    private boolean mListenerRegistered = false;

	public RtkHDMIManager() {
		mRtkHDMIService = IRtkHDMIService.Stub.asInterface(ServiceManager.getService("RtkHDMIService"));
		if(mRtkHDMIService != null) {
			Log.i(TAG, mRtkHDMIService.toString());
		}
		else {
			Log.e(TAG, "error! CANNOT get RtkHDMIService!");
		}
	}

    private IRtkHDMICallback mCallback = new IRtkHDMICallback.Stub() {
        public void onHandleEvent(int event) {
            Log.d(TAG, "onHandleEvent event:"+event);

            if(mEventListener != null) {
                mEventListener.onHandleEvent(event);
            }
        }
    };

    public void setEventListener(IRtkHDMIManagerEventListener listener) {
        if(listener != null) {
            mEventListener = listener;
            if(!mListenerRegistered) {
                try{
                    mRtkHDMIService.registerCallback(mCallback);
                    mListenerRegistered = true;
                } catch(RemoteException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public void unsetEventListener() {
        mEventListener = null;
        if(mListenerRegistered) {
            try{
                mRtkHDMIService.unregisterCallback(mCallback);
                mListenerRegistered = false;
            } catch(RemoteException e) {
                e.printStackTrace();
            }
        }
    }

    public boolean setHDCPVersion(int hdcpVersion) {
        boolean rst = false;
        if(mRtkHDMIService == null) {
            return rst;
        }

        try {
            rst = mRtkHDMIService.setHDCPVersion(hdcpVersion);
        } catch (Exception e) {
            Log.e(TAG, "RtkHDMIService setHDCPVersion failed", e);
        }
        return rst;
    }

    public int getHDCPVersion() {
        int rst = HDCP_VERSION_NA;
        if(mRtkHDMIService == null) {
            return rst;
        }

        try {
            rst = mRtkHDMIService.getHDCPVersion();
        } catch (Exception e) {
            Log.e(TAG, "RtkHDMIService getHDCPVersion failed", e);
        }
        return rst;
    }

	// check if HDMI cable is connected.
	public boolean checkIfHDMIPlugged() {
		boolean result = false;

		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return false;
		}

		try {
			result = mRtkHDMIService.checkIfHDMIPlugged();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIservice checkIfHDMIPlugged failed!", e);
		}

		return result;
	}

	// check if HDMI is ok (Finish switching TV system)
	public boolean checkIfHDMIReady() {
		boolean result = false;

		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return false;
		}

		try {
			result = mRtkHDMIService.checkIfHDMIReady();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService checkIfHDMIReady failed!", e);
		}

		return result;
	}

	// If EDID is ready, it means that EDID is valid.
	public boolean getHDMIEDIDReady() {
		boolean result = false;

		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return false;
		}

		try {
			result = mRtkHDMIService.checkIfHDMIEDIDReady();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService checkIfHDMIEDIDReady failed!", e);
		}

		return result;
	}

	public boolean sendAudioMute(int select) {
		boolean result = false;

		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return false;
		}

		try {
			result = mRtkHDMIService.sendAudioMute(select);
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService SendAudioMute failed!", e);
		}

		return result;
	}

	public void setHDMIDeepColorMode(int mode) {
		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
		}

		try {
			mRtkHDMIService.setHDMIDeepColorMode(mode);
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService setHDMIDeepColorMode failed!", e);
		}
	}


	public void setHDMIFormat3D(int format3d, float fps) {
		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
		}

		try {
			mRtkHDMIService.setHDMIFormat3D(format3d, fps);
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService setHDMIFormat3D failed!", e);
		}
	}

	public int setHDCPState(int state) {
		int result = 0;

		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return -1;
		}

		try {
			result = mRtkHDMIService.setHDCPState(state);
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService setHDCPState failed!", e);
		}

		return result;
	}

	public int[] getVideoFormat() {
		int[] arrTVSystem = null;

		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return null;
		}

		try {
			arrTVSystem = mRtkHDMIService.getVideoFormat();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService getVideoFormat failed!", e);
		}

		return arrTVSystem;
	}

	public int getNextNVideoFormat(int nextN) {
		 int vid = 0;

		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return 0;
		}

		try {
			vid = mRtkHDMIService.getNextNVideoFormat(nextN);
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService getNextNVideoFormat failed!", e);
		}

		return vid;
	}

	public int getTV3DCapability() {
		int result = 0;

		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return -1;
		}

		try {
			result = mRtkHDMIService.getTV3DCapability();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService getTV3DCapability failed!", e);
		}

		return result;
	}

	public int[] getTVSupport3D() {
		int[] arrResult = {0};
		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return null;
		}

		try {
			arrResult = mRtkHDMIService.getTVSupport3D();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService getTVSupport3D!", e);
		}

		return arrResult;
	}

	public int[] getTVSupport3DResolution() {
		int[] arrResult = {0};
		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return null;
		}

		try {
			arrResult = mRtkHDMIService.getTVSupport3DResolution();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService getTVSupport3DResolution!", e);
		}

		return arrResult;
	}

	public int setTVSystem2(int tvSystem, boolean deepColorFirst) {
		int result = 0;

		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return -1;
		}

		try {
			result = mRtkHDMIService.setTVSystem2(tvSystem,deepColorFirst);
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService setTVSystem2 failed!", e);
		}

		return result;
	}

	public int setTVSystem(int tvSystem) {
		int result = 0;

		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return -1;
		}

		try {
			result = mRtkHDMIService.setTVSystem(tvSystem);
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService setTVSystem failed!", e);
		}

		return result;
	}

	public boolean isTVSupport3D() {
		boolean result = false;

		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return false;
		}

		try {
			result = mRtkHDMIService.isTVSupport3D();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService isTVSupport3D failed!", e);
		}

		return result;
	}

	public byte[] getEDIDRawData() {
		byte[] arrResult = {0};
		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return null;
		}

		try {
			arrResult = mRtkHDMIService.getEDIDRawData();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService getEDIDRawData!", e);
		}

		return arrResult;
	}

	public int getTVSystem() {
		int tvSystem = 0;
		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return -1;
		}

		try {
			tvSystem = mRtkHDMIService.getTVSystem();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService getTVSystem!", e);
		}

		return tvSystem;
	}

	public int getTVSystemForRestored() {
		int tvSystem = 0;
		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return -1;
		}

		try {
			tvSystem = mRtkHDMIService.getTVSystemForRestored();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService getTVSystemForRestored!", e);
		}

		return tvSystem;
	}

	public int checkTVSystem(int inputTvSystem, int colorspace_deepcolor){
		int ret = 0;
		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return -1;
		}

		try {
			ret = mRtkHDMIService.checkTVSystem(inputTvSystem, colorspace_deepcolor);
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService getTVSystemForRestored!", e);
		}

		return ret;
	}

	public void setHDMIVideoColorMode(int mode) {
		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
		}

		try {
			mRtkHDMIService.setHDMIVideoColorMode(mode);
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService setHDMIVideoColorMode failed!", e);
		}
	}

	public int getTVMaxLuminace() {
		int result = 0;

		if(mRtkHDMIService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIService!");
			return -1;
		}

		try {
			result = mRtkHDMIService.getTVMaxLuminace();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIService getTVMaxLuminace failed!", e);
		}

		return result;
	}	

    public int getHDREotfValue() {
        int result = 0;
        if(mRtkHDMIService == null) {
            Log.e(TAG, "failed to connect to RtkHDMIService!");
            return -1;
        }
        try {
            result = mRtkHDMIService.getHDREotfValue();
        } catch (Exception e) {
            Log.e(TAG, "RtkHDMIService getHDREotfValue failed!", e);
        }
        return result;
    }

    public int setHDREotfMode(int mode) {
        int result = 0;
        if(mRtkHDMIService == null) {
            Log.e(TAG, "failed to connect to RtkHDMIService!");
            return -1;
        }
        try {
            result = mRtkHDMIService.setHDREotfMode(mode);
        } catch (Exception e) {
            Log.e(TAG, "RtkHDMIService setHDREotfMode failed!", e);
        }
        return result;
    }

    public int getDeepColorValue() {
        int result = 0;
        if(mRtkHDMIService == null) {
            Log.e(TAG, "failed to connect to RtkHDMIService!");
            return -1;
        }
        try {
            result = mRtkHDMIService.getDeepColorValue();
        } catch (Exception e) {
            Log.e(TAG, "RtkHDMIService getDeepColorValue failed!", e);
        }
        return result;
    }

    public int getColorSpaceValue() {
        int result = 0;
        if(mRtkHDMIService == null) {
            Log.e(TAG, "failed to connect to RtkHDMIService!");
            return -1;
        }
        try {
            result = mRtkHDMIService.getColorSpaceValue();
        } catch (Exception e) {
            Log.e(TAG, "RtkHDMIService getColorSpaceValue failed!", e);
        }
        return result;
    }

    public boolean getHDCP1xEnabled() {
        boolean result = false;
        if(mRtkHDMIService == null) {
            Log.e(TAG, "failed to connect to RtkHDMIService!");
            return false;
        }
        try {
            result = mRtkHDMIService.getHDCP1xEnabled();
        } catch (Exception e) {
            Log.e(TAG, "RtkHDMIService getHDCP1xEnabled failed!", e);
        }
        return result;
    }

    public boolean getHDCP22Enabled() {
        boolean result = false;
        if(mRtkHDMIService == null) {
            Log.e(TAG, "failed to connect to RtkHDMIService!");
            return false;
        }
        try {
            result = mRtkHDMIService.getHDCP22Enabled();
        } catch (Exception e) {
            Log.e(TAG, "RtkHDMIService getHDCP22Enabled failed!", e);
        }
        return result;
    }

    public int getHDRTVSupportedEotfMode(){
        int hdrEotfvalue = getHDREotfValue();
        int checkbit = 1;
        int supportMode = 0;

        //check bit0 ~ bit3
        for(int i = 0; i < 4; i++)
        {
            checkbit = checkbit << i;
            if((hdrEotfvalue & checkbit)!=0)
            {
                checkbit = i;
                    break;
            }
        }

        switch(checkbit)
        {
            case 0: supportMode = RtkTVSystem.HDR_TV_SUPPOTED_EOTF_SDR; break;
            case 1: supportMode = RtkTVSystem.HDR_TV_SUPPOTED_EOTF_HDR; break;
            case 2: supportMode = RtkTVSystem.HDR_TV_SUPPOTED_EOTF_SMPTEST2084; break;
            case 3: supportMode = RtkTVSystem.HDR_TV_SUPPOTED_EOTF_HLG; break;
            default: break;
        }
        return supportMode;
    }

    public int getHDRTVUHDColorSettingStatus(){
        int result = 0;
        result = checkTVSystem(RtkTVSystem.TV_SYS_2160P_60HZ, RtkTVSystem.TV_SYS_RGB444_8BIT);
        return result;
    }

    public int[] getHDRTVWCGColorSupported(){
        int[] result = {0};
        int colorSpaceValue = getColorSpaceValue();

        result[0] = ((colorSpaceValue & 0xa0) !=0)? 1 : 0; //BT2020cYCC
        result[1] = ((colorSpaceValue & 0xa0) !=0)? 1 : 0; //BT2020YCC
        result[2] = ((colorSpaceValue & 0xc0) !=0)? 1 : 0; //BT2020RGB

        return result;
    }

    public int[] getHDRTVDeepColorSupported(){
        int[] result = {0};
        int deepColor = getDeepColorValue();

        result[0] = ((deepColor & 0x01) !=0)? 1:0; //deepColor30
        result[1] = (((deepColor >> 1) & 0x01) !=0)? 1:0;//deepColor36
        result[2] = (((deepColor >> 2) & 0x01)!=0)?1:0;//deepColor48

        return result;
    }

    public int[] getHDREotfModeSupported(){
        int[] result = {0};
        int eotfValue = getHDREotfValue();

        result[0] = ((eotfValue & 0x01)!=0)? 1 : 0; //SDR
        result[1] = (((eotfValue >> 1) & 0x1)!=0)? 1 : 0;//HDR
        result[2] = (((eotfValue >> 2) & 0x1)!=0)? 1 : 0;//HDR_SMPTESt2084
        result[3] = (((eotfValue >> 3) & 0x1)!=0)? 1 : 0;//Future

        return result;
    }

    public void setHDRColorSpace(int colorSpace){
        String colorSpaceStr = Integer.toString(colorSpace);
        SystemProperties.set(PROPERTY_VIDEO_COLOR_MODE, colorSpaceStr);
        setHDMIVideoColorMode(colorSpace);
    }

    public void setHDRColorDepth(int colorDepth){
        String colorDeepStr;

        if (colorDepth == RtkTVSystem.HDR_COLOR_DEPTH_AUTO)
            colorDeepStr = "auto";
        else if (colorDepth == RtkTVSystem.HDR_COLOR_DEPTH_10BIT)
            colorDeepStr = "10bit";
        else if (colorDepth == RtkTVSystem.HDR_COLOR_DEPTH_12BIT)
            colorDeepStr = "12bit";
        else
            colorDeepStr = "off";

        SystemProperties.set(PROPERTY_VIDEO_COLOR_DEEP, colorDeepStr);
        setHDMIDeepColorMode(colorDepth);
    }

	// switch color mode to space and depth
	private int[] switchColorMode(int colormode) {
		int[] result = {0,0}; // result[0] : store space result[1]:store depth 	

		if(colormode == RtkTVSystem.TV_SYS_RGB444_8BIT){
			result[0] = 1;
			result[1] = 1;
		}
		else if( colormode == RtkTVSystem.TV_SYS_RGB444_10BIT){
			result[0] = 1;
			result[1] = 2;
		}
		else if( colormode == RtkTVSystem.TV_SYS_RGB444_12BIT){
			result[0] = 1;
			result[1] = 3;
		}
		else if( colormode == RtkTVSystem.TV_SYS_YCbCr444_8BIT){
			result[0] = 3;
			result[1] = 1;
		}
		else if( colormode == RtkTVSystem.TV_SYS_YCbCr444_10BIT){
			result[0] = 3;
			result[1] = 2;
		}
		else if( colormode == RtkTVSystem.TV_SYS_YCbCr444_12BIT){
			result[0] = 3;
			result[1] = 3;
		}
		else if( colormode == RtkTVSystem.TV_SYS_YCbCr422){
			result[0] = 2;
			result[1] = 1;
		}
		else if( colormode == RtkTVSystem.TV_SYS_YCbCr420_8BIT){
			result[0] = 4;
			result[1] = 1;
		}
		else if( colormode == RtkTVSystem.TV_SYS_YCbCr420_10BIT){
			result[0] = 4;
			result[1] = 2;
		}
		else if( colormode == RtkTVSystem.TV_SYS_YCbCr420_12BIT){
			result[0] = 4;
			result[1] = 3;
		}
		else{
			result[0] = 0;
			result[1] = 0;
		}
		return result;

	}

    public int[][] getHDRColor(){

        int[] colorSpaceDepth = {0};
        int[][] result = {{0},{0}}; 
        int isSupported;
        int colormode;

        //result[0][0] ~result[0][4] : Auto/Rgb/YCbCr422/YCbCr444/YCbCr420
        //result[1][0] ~result[1][3] : Auto/8bit/10bit/12bit

        for(int i = 0; i < RtkTVSystem.TV_SYS_DEEPCOLOR_AUTO_DETECT; i++)
        {
            isSupported = checkTVSystem(getTVSystem(), i);
            if(isSupported > 0)
            {
                colormode = i;
                colorSpaceDepth =switchColorMode(colormode);
                int iSpace= colorSpaceDepth[0];
                int iDepth = colorSpaceDepth[1];

                result[0][iSpace] = 1;
                result[1][iDepth] = 1;
            }
        }

        return result;
    }

    public void setHDMIEnable(int enable) {
        if(mRtkHDMIService == null) {
            Log.e(TAG, "failed to connect to RtkHDMIService!");
            return;
        }
        try {
            mRtkHDMIService.setHDMIEnable(enable);
        } catch (Exception e) {
            Log.e(TAG, "RtkHDMIService setHDMIEnable failed!", e);
        }
    }


}
