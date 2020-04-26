package com.realtek.server;

import java.util.ArrayList;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import android.util.Slog;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.os.IBinder;
import android.os.PowerManager;
import android.os.RemoteException;
import android.os.SystemProperties;
import com.realtek.hardware.IRtkVoutUtilService;
//import com.realtek.server.RtkTVSystem;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
import android.content.ContentResolver;
import android.graphics.Rect;
import android.view.KeyEvent;
/*
* The implementation of the RtkVoutUtilService.
*/
public class RtkVoutUtilService extends IRtkVoutUtilService.Stub {
	static {
                System.loadLibrary("realtek_runtime");
	}

	private static final String TAG = "RtkVoutUtilService";

	/**
	 * Binder context for this service
	 */
	private Context mContext;

    /**
    * Zoom state for this service
    */
    private int mZoomState = ZOOM_STATE_NONE;

    public static final int ZOOM_STATE_NONE = 0;
    public static final int ZOOM_STATE_2X = 1;
    public static final int ZOOM_STATE_3X = 2;
    public static final int ZOOM_STATE_4X = 3;
    public static final int ZOOM_STATE_8X = 4;

    private Rect mOriginV1Rect = new Rect(0, 0, 1920, 1080); //hardcode to keep original v1 rect, FIXME: need to get from VO
    private Rect mCurrV1Rect = new Rect(mOriginV1Rect);
	/**
	 * Constructs a new RtkVoutUtilService instance.
	 *
	 */
	public RtkVoutUtilService(Context context) {
		mContext = context;
		Slog.w(TAG, "RtkVoutUtilService is constructed!");
		SystemProperties.set("ctl.start", "regService");

        // BUG_FIX: PHOENIX-330 read property setting
        int zoom = SystemProperties.getInt("persist.rtk.vout.zoom",0);
        if(zoom == 1){
            int zoom_ratio = SystemProperties.getInt("persist.rtk.vout.zoom.ratio",95);
            Slog.d(TAG, "set display ratio to " + zoom_ratio + "%");
            _setDisplayRatio(zoom_ratio);
        }
	}

	public boolean init() {
			return _init();
	}
/*
	public boolean setAudioSpdifOutput(int mode) {
			return _setAudioSpdifOutput(mode);
	}

	public boolean setAudioHdmiOutput(int mode) {
			return _setAudioHdmiOutput(mode);
	}

	public boolean setAudioAGC(int mode) {
			return _setAudioAGC(mode);
	}
*/
	public boolean setRescaleMode(int mode) {
			return _setRescaleMode(mode);
	}

    public boolean setOSDWindow(Rect osdWin) {
        return _setOSDWindow(osdWin);
    }

    public boolean nextZoom() {
        // get next zoom state
        int newState = getNextZoomState();
        // get new state's rect
        Rect rect = getZoomRect(newState);
        boolean res = _setZoomRect(rect);
        if (true == res) {
            mZoomState = newState;
            mCurrV1Rect.set(rect);
        } else {
            Slog.w(TAG, "_setZoomRect() return false!");
        }
        return res;
    }

    public boolean prevZoom() {
        // get prev zoom state
        int newState = getPrevZoomState();
        // get new state's rect
        Rect rect = getZoomRect(newState);
        boolean res = _setZoomRect(rect);
        if (true == res) {
            mZoomState = newState;
            mCurrV1Rect.set(rect);
        } else {
            Slog.w(TAG, "_setZoomRect() return false!");
        }
        return res;
    }

    public boolean isZooming() {
        return (mZoomState != ZOOM_STATE_NONE);
    }

    public boolean resetZoom() {
	  mOriginV1Rect.set(getHWCV1Rect());
	  Slog.v(TAG, "resetZoom: mOriginV1Rect:" + mOriginV1Rect.toString());

        boolean res = _setZoomRect(mOriginV1Rect);
        if (true == res) {
            mZoomState = ZOOM_STATE_NONE;
            mCurrV1Rect.set(mOriginV1Rect);
        } else {
            Slog.w(TAG, "_setZoomRect() return false!");
        }
        return res;
    }

	public boolean moveZoom(int keycode) {
		int offset = 0;
		boolean ret = false;
		Rect newRect = new Rect();

		if(mZoomState == ZOOM_STATE_NONE) {
			mOriginV1Rect.set(getHWCV1Rect());
			Slog.v(TAG, "moveZoom: mOriginV1Rect:" + mOriginV1Rect.toString());
			mCurrV1Rect.set(mOriginV1Rect);
			Slog.v(TAG, "moveZoom: mCurrV1Rect:" + mCurrV1Rect.toString());
		}

		offset = mCurrV1Rect.height() / 8;
		newRect.set(mCurrV1Rect);

		switch (keycode) {
		case KeyEvent.KEYCODE_DPAD_UP:
			if ((newRect.top - offset) > mOriginV1Rect.top) {
				newRect.top -= offset;
				newRect.bottom -= offset;
			} else {
				newRect.bottom -= (newRect.top - mOriginV1Rect.top);
				newRect.top = mOriginV1Rect.top;
			}
			break;
		case KeyEvent.KEYCODE_DPAD_DOWN:
			if ((newRect.bottom + offset) < mOriginV1Rect.bottom) {
				newRect.top += offset;
				newRect.bottom += offset;
			} else {
				newRect.top += (mOriginV1Rect.bottom - newRect.bottom);
				newRect.bottom = mOriginV1Rect.bottom;
			}
			break;
		case KeyEvent.KEYCODE_DPAD_LEFT:
			if ((newRect.left - offset) > mOriginV1Rect.left) {
				newRect.left -= offset;
				newRect.right -= offset;
			} else {
				newRect.right -= (newRect.left - mOriginV1Rect.left);
				newRect.left = mOriginV1Rect.left;
			}
			break;
		case KeyEvent.KEYCODE_DPAD_RIGHT:
			if ((newRect.right + offset) < mOriginV1Rect.right) {
				newRect.left += offset;
				newRect.right += offset;
			} else {
				newRect.left += (mOriginV1Rect.right - newRect.right);
				newRect.right = mOriginV1Rect.right;
			}
			break;
		default:
			break;
		}

		Slog.v(TAG, "current V1 rect:" + mCurrV1Rect.toString());
		Slog.v(TAG, "new V1 rect:" + newRect.toString());

		if (newRect != mCurrV1Rect) {
			//apply change
			ret = _setZoomRect(newRect);
			if (true == ret) {
				mCurrV1Rect.set(newRect);
			} else {
				Slog.w(TAG, "_setZoomRect() return false!");
			}
		} else {
			ret = true;
		}

		return ret;
	}

	public boolean setFormat3d(int format3d, float fps) {
		return _setFormat3d(format3d, fps);
	}

	public boolean setShiftOffset3d(boolean exchange_eyeview, boolean shift_direction, int delta_offset) {
		return _setShiftOffset3d(exchange_eyeview, shift_direction, delta_offset);
	}

	public boolean set3dto2d(int srcformat3d) {
		return _set3dto2d(srcformat3d);
	}

	public boolean set3dSub(int sub) {
		return _set3dSub(sub);
	}

	public boolean applyVoutDisplayWindowSetup() {
		return _applyVoutDisplayWindowSetup();
	}

	public boolean setDisplayRatio(int ratio) {
		return _setDisplayRatio(ratio);
	}

	public boolean setDisplayPosition(int x, int y, int width, int height) {
		return _setDisplayPosition(x, y, width, height);
	}

    public void setEnhancedSDR(int flag) {
        _setEnhancedSDR(flag);
    }

    public boolean isHDRtv() {
        return _isHDRtv();
    }

    public boolean setShiftOffset3dByPlane(boolean exchange_eyeview, boolean shift_direction, int delta_offset, int targetPlane) {
        return _setShiftOffset3dByPlane(exchange_eyeview, shift_direction, delta_offset, targetPlane);
    }

    public boolean isCVBSOn() {
        return _isCVBSOn();
    }

    public void setCVBSOff(int off) {
        _setCVBSOff(off);
    }

    public void setPeakLuminance(int level) {
        _setPeakLuminance(level);
    }

    public void setHdrSaturation(int level) {
        _setHdrSaturation(level);
    }

    public void setHdmiRange(int rangeMode) {
        _setHdmiRange(rangeMode);
    }

    public void setCVBSDisplayRatio(int ratio) {
        _setCVBSDisplayRatio(ratio);
    }

    public void setEmbedSubDisplayFixed(int fixed) {
        _setEmbedSubDisplayFixed(fixed);
    }

    public void setBrightness(int value) {
        _setBrightness(value);
    }

    public void setContrast(int value) {
        _setContrast(value);
    }

    public void setHue(int value) {
        _setHue(value);
    }

    public void setSaturation(int value) {
        _setSaturation(value);
    }

    public void setSuperResolutionOff(int off) {
        _setSuperResolutionOff(off);
    }

    public void setHdrtoSDRgma(int mode) {
        _setHdrtoSDRgma(mode);
    }

    public boolean setDisplayWindowDispZoomWin(int plane,Rect videoWin,Rect dispZoomWin) {
        return _setDisplayWindowDispZoomWin(plane,videoWin,dispZoomWin);
    }

    public void setMaxCLLMaxFALLDisable(int disable) {
        _setMaxCLLMaxFALLDisable(disable);
    }
    /**
     * Below declaration are implemented by c native code
     * that implemented by device/realtek/frameworks/services/jni/com_realtek_server_RtkVoutUtilService.cpp
     */

	private native boolean _init();
	//private native boolean _setAudioSpdifOutput(int mode);
	//private native boolean _setAudioHdmiOutput(int mode);
	//private native boolean _setAudioAGC(int mode);
	private native boolean _setRescaleMode(int mode);
	private native boolean _setOSDWindow(Rect osdWin);
	private native boolean _setZoomRect(Rect zoomRect);
	private native boolean _setFormat3d(int format3d, float fps);
	private native boolean _setShiftOffset3d(boolean exchange_eyeview, boolean shift_direction, int delta_offset);
	private native boolean _set3dto2d(int srcformat3d);
	private native boolean _set3dSub(int sub);
	private native boolean _applyVoutDisplayWindowSetup();
	private native boolean _setDisplayRatio(int ratio);
	private native boolean _setDisplayPosition(int x, int y, int width, int height);
	private native Rect _getHWCV1Rect();
    private native void _setEnhancedSDR(int flag);
    private native boolean _isHDRtv();
    private native boolean _setShiftOffset3dByPlane(boolean exchange_eyeview, boolean shift_direction, int delta_offset, int targetPlane);
    private native boolean _isCVBSOn();
    private native void _setCVBSOff(int off);
    private native void _setPeakLuminance(int level);
    private native void _setHdrSaturation(int level);
    private native void _setHdmiRange(int rangeMode);
    private native void _setCVBSDisplayRatio(int ratio);
    private native void _setEmbedSubDisplayFixed(int fixed);
    private native void _setBrightness(int value);
    private native void _setContrast(int value);
    private native void _setHue(int value);
    private native void _setSaturation(int value);
    private native void _setSuperResolutionOff(int off);
    private native void _setHdrtoSDRgma(int mode);
    private native boolean _setDisplayWindowDispZoomWin(int plane,Rect videoWin,Rect dispZoomWin);
    private native void _setMaxCLLMaxFALLDisable(int disable);

    private Rect getZoomRect(int state) {
        Rect rect = new Rect();

        Slog.v(TAG, "getZoomRect: mOriginV1Rect:" + mOriginV1Rect.toString());
        Slog.v(TAG, "getZoomRect: mCurrV1Rect:" + mCurrV1Rect.toString());

        switch (state) {
        case ZOOM_STATE_2X:
            rect.left = (mCurrV1Rect.left + mCurrV1Rect.width() / 2) - mOriginV1Rect.width() * 7 / 20;
            rect.top = (mCurrV1Rect.top + mCurrV1Rect.height() / 2) - mOriginV1Rect.height() * 7 / 20;
            rect.right = rect.left + mOriginV1Rect.width() * 7 / 10;
            rect.bottom = rect.top + mOriginV1Rect.height() * 7 / 10;
            break;
        case ZOOM_STATE_3X:
            rect.left = (mCurrV1Rect.left + mCurrV1Rect.width() / 2) - mOriginV1Rect.width() * 6 / 20;
            rect.top = (mCurrV1Rect.top + mCurrV1Rect.height() / 2) - mOriginV1Rect.height() * 6 / 20;
            rect.right = rect.left + mOriginV1Rect.width() * 6 / 10;
            rect.bottom = rect.top + mOriginV1Rect.height() * 6 / 10;
            break;
        case ZOOM_STATE_4X:
            rect.left = (mCurrV1Rect.left + mCurrV1Rect.width() / 2) - mOriginV1Rect.width() * 5 / 20;
            rect.top = (mCurrV1Rect.top + mCurrV1Rect.height() / 2) - mOriginV1Rect.height() * 5 / 20;
            rect.right = rect.left + mOriginV1Rect.width() * 5 / 10;
            rect.bottom = rect.top + mOriginV1Rect.height() * 5 / 10;
            break;
        case ZOOM_STATE_8X:
            rect.left = (mCurrV1Rect.left + mCurrV1Rect.width() / 2) - mOriginV1Rect.width() * 4 / 20;
            rect.top = (mCurrV1Rect.top + mCurrV1Rect.height() / 2) - mOriginV1Rect.height() * 4 / 20;
            rect.right = rect.left + mOriginV1Rect.width() * 4 / 10;
            rect.bottom = rect.top + mOriginV1Rect.height() * 4 / 10;
            break;
        case ZOOM_STATE_NONE:
        default:
            rect.set(mOriginV1Rect);
        }

	  Slog.v(TAG, "getZoomRect: new V1 rect:" + rect.toString());
        return rect;
    }

	private int getNextZoomState() {
		int new_state = mZoomState + 1;

		if(mZoomState == ZOOM_STATE_NONE) {
			mOriginV1Rect.set(getHWCV1Rect());
	        	Slog.v(TAG, "getNextZoomState: mOriginV1Rect:" + mOriginV1Rect.toString());
			mCurrV1Rect.set(mOriginV1Rect);
			Slog.v(TAG, "getNextZoomState: mCurrV1Rect:" + mCurrV1Rect.toString());
		}

		if (new_state > ZOOM_STATE_8X) {
	            new_state = ZOOM_STATE_NONE;
		}

		return new_state;
	}

	private int getPrevZoomState() {
		int new_state = mZoomState - 1;

		if(mZoomState == ZOOM_STATE_NONE) {
			mOriginV1Rect.set(getHWCV1Rect());
			Slog.v(TAG, "getNextZoomState: mOriginV1Rect:" + mOriginV1Rect.toString());
			mCurrV1Rect.set(mOriginV1Rect);
			Slog.v(TAG, "getNextZoomState: mCurrV1Rect:" + mCurrV1Rect.toString());
		}

		if (new_state < ZOOM_STATE_NONE) {
			new_state = ZOOM_STATE_8X;
		}

		return new_state;
	}

	private Rect getHWCV1Rect() {
            Rect rect = _getHWCV1Rect();
            Slog.v(TAG, "getHWCV1Rect: " + rect.toString());
            if(rect.right-rect.left > 0 && rect.bottom-rect.top >0) {
                return rect;
            }
            return new Rect(0, 0, 1920, 1080);
	}
}
