/*
 * Copyright (c) 2013 Realtek Semi. co. All Rights Reserved.
 * Author:  KC HSU
 * Version: V0.1
 * Date:  2014-03-06
 * Comment: This class lets you access the VoutUtil function.
 */

package com.realtek.hardware;

import java.util.StringTokenizer;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;
import android.graphics.Rect;

import com.realtek.hardware.IRtkVoutUtilService;
import android.os.SystemProperties;

public class RtkVoutUtilManager {
	private static final String TAG = "RtkVoutUtilManager";

	// The value used in the function setFormat3d()
	public static final int FORMAT_3D_NULL = -1;
	public static final int FORMAT_3D_AUTO_DETECT = 0;
	public static final int FORMAT_3D_RESERVED = 1;
	public static final int FORMAT_3D_SIDE_BY_SIDE = 2;
	public static final int FORMAT_3D_TOP_AND_BOTTOM = 3;
    /* For Android Kodi APK, VO output normal frame and TV display 3D */
    public static final int FORMAT_3D_NULL_FOR_TV = 4;
    public static final int FORMAT_3D_SIDE_BY_SIDE_FOR_TV = 5;
    public static final int FORMAT_3D_TOP_AND_BOTTOM_FOR_TV = 6;

    // The value used in the function set3dSub()
    public static final int FORMAT_3D_SUB_EXTERNAL = 0;
    public static final int FORMAT_3D_SUB_INTERNAL = 1;

	private IRtkVoutUtilService mRtkVoutUtilService;

    private static final String PROPERTY_VIDEO_BRIGHTNESS = "persist.rtk.video.brightness";
    private static final String PROPERTY_VIDEO_CONTRAST = "persist.rtk.video.contrast";
    private static final String PROPERTY_VIDEO_HUE = "persist.rtk.video.hue";
    private static final String PROPERTY_VIDEO_SATURATION = "persist.rtk.video.saturation";

	public RtkVoutUtilManager() {
		mRtkVoutUtilService = IRtkVoutUtilService.Stub.asInterface(ServiceManager.getService("RtkVoutUtilService"));
		if(mRtkVoutUtilService != null) {
			Log.i(TAG, mRtkVoutUtilService.toString());
		}
		else {
			Log.e(TAG, "error! CANNOT get RtkVoutUtilService!");
		}
	}

	public boolean init() {
		boolean result = false;

		if(mRtkVoutUtilService != null) {
			try {
				result = mRtkVoutUtilService.init();
				Log.d(TAG, "RtkVoutUtilManager::init() [1]");
			} catch (Exception e) {
				Log.e(TAG, "RtkVoutUtilManager::init() [1] failed", e);
			}
		} else {
			Log.d(TAG, "RtkVoutUtilManager::init() [2]");
			result = false;
		}
		return result;
	}

/*
	public boolean setAudioSpdifOutput(int mode) {
		boolean result = false;

		if(mRtkAoutUtilService != null) {
			try {
				result = mRtkAoutUtilService.setAudioSpdifOutput(mode);
				Log.d(TAG, "RtkAoutUtilManager::setAudioSpdifOutput() [1], mode = " + mode);
			} catch (Exception e) {
				Log.e(TAG, "RtkAoutUtilManager::setAudioSpdifOutput() [1] failed", e);
			}
		} else {
			Log.d(TAG, "RtkAoutUtilManager::setAudioSpdifOutput() [2]");
			result = false;
		}
		return result;
	}

	public boolean setAudioHdmiOutput(int mode) {
		boolean result = false;

		if(mRtkAoutUtilService != null) {
			try {
				result = mRtkAoutUtilService.setAudioHdmiOutput(mode);
				Log.d(TAG, "RtkAoutUtilManager::setAudioHdmiOutput() [1], mode = " + mode);
			} catch (Exception e) {
				Log.e(TAG, "RtkAoutUtilManager::setAudioHdmiOutput() [1] failed", e);
			}
		} else {
			Log.d(TAG, "RtkAoutUtilManager::setAudioHdmiOutput() [2]");
			result = false;
		}
		return result;
	}

	public boolean setAudioAGC(int mode) {
		boolean result = false;

		if(mRtkAoutUtilService != null) {
			try {
				result = mRtkAoutUtilService.setAudioAGC(mode);
				Log.d(TAG, "RtkAoutUtilManager::setAudioAGC() [1], mode = " + mode);
			} catch (Exception e) {
				Log.e(TAG, "RtkAoutUtilManager::setAudioAGC() [1] failed", e);
			}
		} else {
			Log.d(TAG, "RtkAoutUtilManager::setAudioAGC() [2]");
			result = false;
		}
		return result;
	}
*/

	public boolean setRescaleMode(int mode) {
		boolean result = false;

		if(mRtkVoutUtilService != null) {
			try {
				result = mRtkVoutUtilService.setRescaleMode(mode);
				Log.d(TAG, "RtkVoutUtilManager::setRescaleMode() [1], mode = " + mode);
			} catch (Exception e) {
				Log.e(TAG, "RtkVoutUtilManager::setRescaleMode() [1] failed", e);
			}
		} else {
			Log.d(TAG, "RtkVoutUtilManager::setRescaleMode() [2]");
			result = false;
		}
		return result;
	}

    public boolean setOSDWindow(Rect osdWin) {
        boolean result = false;

        if (mRtkVoutUtilService != null) {
            try {
                Log.d(TAG, "RtkVoutUtilManager::setOSDWindow(Rect, Rect)");
                result = mRtkVoutUtilService.setOSDWindow(osdWin);
            } catch (Exception e) {
                Log.e(TAG, "RtkVoutUtilManager::setOSDWindow(Rect, Rect): failed", e);
            }
        } else {
            Log.d(TAG, "RtkVoutUtilManager::setOSDWindow(Rect, Rect) [2]");
        }
        return result;
    }

    public boolean nextZoom() {
        boolean result = false;
        if (mRtkVoutUtilService != null) {
            try {
                Log.v(TAG, "RtkVoutUtilManager::nextZoom()");
                result = mRtkVoutUtilService.nextZoom();
            } catch (Exception e) {
                Log.e(TAG, "RtkVoutUtilManager::nextZoom(): failed", e);
            }
        } else {
            Log.w(TAG, "nextZoom(), RtkVoutUtilService is null!");
        }
        return result;
    }

    public boolean prevZoom() {
        boolean result = false;
        if (mRtkVoutUtilService != null) {
            try {
                Log.v(TAG, "RtkVoutUtilManager::prevZoom()");
                result = mRtkVoutUtilService.prevZoom();
            } catch (Exception e) {
                Log.e(TAG, "RtkVoutUtilManager::prevZoom(): failed", e);
            }
        } else {
            Log.w(TAG, "prevZoom(), RtkVoutUtilService is null!");
        }
        return result;
    }

    public boolean isZooming() {
        boolean result = false;
        if (mRtkVoutUtilService != null) {
            try {
                Log.v(TAG, "RtkVoutUtilManager::isZooming()");
                result = mRtkVoutUtilService.isZooming();
            } catch (Exception e) {
                Log.e(TAG, "RtkVoutUtilManager::isZooming(): failed", e);
            }
        } else {
            Log.w(TAG, "isZooming(), RtkVoutUtilService is null!");
        }
        return result;
    }

    public boolean resetZoom() {
        boolean result = false;
        if (mRtkVoutUtilService != null) {
            try {
                Log.v(TAG, "RtkVoutUtilManager::resetZoom()");
                result = mRtkVoutUtilService.resetZoom();
            } catch (Exception e) {
                Log.e(TAG, "RtkVoutUtilManager::resetZoom(): failed", e);
            }
        } else {
            Log.w(TAG, "resetZoom(), RtkVoutUtilService is null!");
        }
        return result;
    }

    public boolean moveZoom(int keycode) {
        boolean result = false;
        if (mRtkVoutUtilService != null) {
            try {
                Log.v(TAG, "RtkVoutUtilManager::moveZoom(), keycode:" + keycode);
                result = mRtkVoutUtilService.moveZoom(keycode);
            } catch (Exception e) {
                Log.e(TAG, "RtkVoutUtilManager::moveZoom(): failed", e);
            }
        } else {
            Log.w(TAG, "moveZoom(), RtkVoutUtilService is null!");
        }
        return result;
    }

	public boolean setFormat3d(int format3d) {
		return setFormat3d(format3d, 0);
	}

	public boolean setFormat3d(int format3d, float fps) {
		boolean result = false;

		if(mRtkVoutUtilService != null) {
			try {
				result = mRtkVoutUtilService.setFormat3d(format3d, fps);
				Log.d(TAG, "RtkVoutUtilManager::setFormat3d(), format3d = " + format3d + " fps = " + fps);
			} catch (Exception e) {
				Log.e(TAG, "RtkVoutUtilManager::setFormat3d() [1] failed", e);
			}
		} else {
			Log.d(TAG, "RtkVoutUtilManager::setFormat3d() [2]");
			result = false;
		}
		return result;
	}

    public boolean set3dto2d(int srcformat3d) {
        boolean result = false;

        if(mRtkVoutUtilService != null) {
        try {
                result = mRtkVoutUtilService.set3dto2d(srcformat3d);
                Log.d(TAG, "RtkVoutUtilManager::set3dto2d(), srcformat3d = "+ srcformat3d );
            } catch (Exception e) {
                Log.e(TAG, "RtkVoutUtilManager::set3dto2d() [1] failed", e);
            }
        } else {
            Log.d(TAG, "RtkVoutUtilManager::set3dto2d() [2]");
            result = false;
        }
        return result;
    }

    public boolean set3dSub(int sub3d) {
        boolean result = false;

        if(mRtkVoutUtilService != null) {
        try {
                result = mRtkVoutUtilService.set3dSub(sub3d);
                Log.d(TAG, "RtkVoutUtilManager::set3dSub(), sub3d = "+ sub3d );
            } catch (Exception e) {
                Log.e(TAG, "RtkVoutUtilManager::set3dSub() [1] failed", e);
            }
        } else {
            Log.d(TAG, "RtkVoutUtilManager::set3dSub() [2]");
            result = false;
        }
        return result;
    }

	public boolean applyVoutDisplayWindowSetup() {
		boolean result = false;

		if(mRtkVoutUtilService != null) {
			try {
				result = mRtkVoutUtilService.applyVoutDisplayWindowSetup();
				Log.d(TAG, "RtkVoutUtilManager::applyVoutDisplayWindowSetup()");
			} catch (Exception e) {
				Log.e(TAG, "RtkVoutUtilManager::applyVoutDisplayWindowSetup() [1] failed", e);
			}
		} else {
			Log.d(TAG, "RtkVoutUtilManager::applyVoutDisplayWindowSetup() [2]");
			result = false;
		}
		return result;
	}

	public boolean setDisplayRatio(int ratio) {
		boolean result = false;

		if(mRtkVoutUtilService != null) {
			try {
				result = mRtkVoutUtilService.setDisplayRatio(ratio);
				Log.d(TAG, "RtkVoutUtilManager::setDisplayRatio(), ratio = " + ratio);
			} catch (Exception e) {
				Log.e(TAG, "RtkVoutUtilManager::setDisplayRatio() failed.", e);
			}
		} else {
			Log.d(TAG, "setDisplayRatio(), mRtkVoutUtilService is null!!");
			result = false;
		}
		return result;
	}

	public boolean setDisplayPosition(int x, int y, int width, int height) {
		boolean result = false;

		if(mRtkVoutUtilService != null) {
			try {
				result = mRtkVoutUtilService.setDisplayPosition(x, y, width, height);
				Log.d(TAG, "RtkVoutUtilManager::setDisplayPosition(), x=" + x + "  y=" + y + " width=" + width + " height=" + height);
			} catch (Exception e) {
				Log.e(TAG, "RtkVoutUtilManager::setDisplayPosition() failed.", e);
			}
		} else {
			Log.d(TAG, "setDisplayPosition(), mRtkVoutUtilService is null!!");
			result = false;
		}
		return result;
	}

    /* flag options:
     **  AUTOSDR = 0
     **  FORCESDR_ON = 1
     **  FORCESDR_OFF = 2
     */
	public boolean setEnhancedSDR(int flag) {
		Log.d(TAG, "entering setEnhancedSDR(),flag=" + flag);
		boolean result = false;

		if(mRtkVoutUtilService != null) {
			try {
                mRtkVoutUtilService.setEnhancedSDR(flag);
				result = true;
			} catch (Exception e) {
				Log.e(TAG, "Exception: ", e);
			}
		} else {
			Log.d(TAG, "mRtkVoutUtilService is null!!");
		}

		return result;
	}

	public boolean isHDRtv() {
		Log.d(TAG, "entering isHDRtv()...");
		boolean result = false;

		if(mRtkVoutUtilService != null) {
			try {
				result = mRtkVoutUtilService.isHDRtv();
			} catch (Exception e) {
				Log.e(TAG, "Exception: ", e);
			}
		} else {
			Log.d(TAG, "mRtkVoutUtilService is null!!");
		}

		return result;
	}

    /*
     * exchange_eyeview:
     *     0:disable , 1: enable;
     * shift_direction:
     *     0:closer, 1: further from user;
     * delta_offset:
     *     delta shift pixels on both eye view, when delta_offset is set 2 pixels closer on V1 plane, means video will closer 2 pixels to user on both eye view
     * targetPlane:
     *     only VO_VIDEO_PLANE_V1, VO_VIDEO_PLANE_SUB1, VO_VIDEO_PLANE_OSD1, VO_VIDEO_PLANE_OSD2 plane are available
    */
	public boolean setShiftOffset3dByPlane(boolean exchange_eyeview, boolean shift_direction, int delta_offset, int target_plane) {
		Log.d(TAG, "entering setShiftOffset3dByPlane(), exchange_eyeview=" + exchange_eyeview + ", shift_direction=" + shift_direction + ", delta_offset=" + delta_offset + ", target_plane=" + target_plane);
		boolean result = false;

		if(mRtkVoutUtilService != null) {
			try {
				result = mRtkVoutUtilService.setShiftOffset3dByPlane(exchange_eyeview, shift_direction, delta_offset, target_plane);
			} catch (Exception e) {
				Log.e(TAG, "Exception: ", e);
			}
		} else {
			Log.d(TAG, "mRtkVoutUtilService is null!!");
		}

		return result;
	}

    public boolean isCVBSOn() {
		Log.d(TAG, "entering isCVBSOn()...");
		boolean result = false;

		if(mRtkVoutUtilService != null) {
			try {
				result = mRtkVoutUtilService.isCVBSOn();
			} catch (Exception e) {
				Log.e(TAG, "Exception: ", e);
			}
		} else {
			Log.d(TAG, "mRtkVoutUtilService is null!!");
		}

		return result;
	}

	public void setCVBSOff(int off) {
		Log.d(TAG, "entering setCVBSOff()...");

		if(mRtkVoutUtilService != null) {
			try {
				mRtkVoutUtilService.setCVBSOff(off);
			} catch (Exception e) {
				Log.e(TAG, "Exception: ", e);
			}
		} else {
			Log.d(TAG, "mRtkVoutUtilService is null!!");
		}
	}

	public boolean setPeakLuminance(int level) {
		Log.d(TAG, "entering setPeakLuminance(), level=" + level);
		boolean result = false;

		if(mRtkVoutUtilService != null) {
			try {
                mRtkVoutUtilService.setPeakLuminance(level);
				result = true;
			} catch (Exception e) {
				Log.e(TAG, "Exception: ", e);
			}
		} else {
			Log.d(TAG, "mRtkVoutUtilService is null!!");
		}

		return result;
	}

	public boolean setHdrSaturation(int level) {
		Log.d(TAG, "entering setHdrSaturation(), level=" + level);
		boolean result = false;

		if(mRtkVoutUtilService != null) {
			try {
                mRtkVoutUtilService.setHdrSaturation(level);
				result = true;
			} catch (Exception e) {
				Log.e(TAG, "Exception: ", e);
			}
		} else {
			Log.d(TAG, "mRtkVoutUtilService is null!!");
		}

		return result;
	}

    public void setHdmiRange(int rangeMode) {
        Log.d(TAG, "entering setHdmiRange()...");
        if(mRtkVoutUtilService != null) {
            try {
                mRtkVoutUtilService.setHdmiRange(rangeMode);
            } catch (Exception e) {
                Log.e(TAG, "Exception: ", e);
            }
        } else {
            Log.d(TAG, "mRtkVoutUtilService is null!!");
        }
    }

    public void setCVBSDisplayRatio(int ratio) {
        Log.d(TAG, "entering setCVBSDisplayRatio()...");
        if(mRtkVoutUtilService != null) {
            try {
                mRtkVoutUtilService.setCVBSDisplayRatio(ratio);
            } catch (Exception e) {
                Log.e(TAG, "Exception: ", e);
            }
        } else {
            Log.d(TAG, "mRtkVoutUtilService is null!!");
        }
    }

    public void setEmbedSubDisplayFixed(int fixed) {
        Log.d(TAG, "entering setEmbedSubDisplayFixed()...");

        if(mRtkVoutUtilService != null) {
            try {
                mRtkVoutUtilService.setEmbedSubDisplayFixed(fixed);
            } catch (Exception e) {
                Log.e(TAG, "Exception: ", e);
            }
        } else {
            Log.d(TAG, "mRtkVoutUtilService is null!!");
        }
    }

    /* Brightness, Contrast, Hue Saturation
     * value range between 0 and 64
     * default value is 32
     */
    public boolean setGraphicBrightness(int value) {
        if(mRtkVoutUtilService != null) {
            if (value > 100)
                value = 100;
            if (value < 0)
                value = 0;
            SystemProperties.set(PROPERTY_VIDEO_BRIGHTNESS, Integer.toString(value));

            try {
                value = 64*value/100;
                mRtkVoutUtilService.setBrightness(value);
            } catch (Exception e) { Log.e(TAG, "Exception: ", e); return false;}
        } else { Log.e(TAG, "mRtkVoutUtilService is null!!"); return false;}
        return true;
    }

    public int getGraphicBrightness() {
        return SystemProperties.getInt(PROPERTY_VIDEO_BRIGHTNESS, 50);
    }

    public boolean setGraphicContrast(int value) {
        if(mRtkVoutUtilService != null) {
            if (value > 100)
                value = 100;
            if (value < 0)
                value = 0;
            SystemProperties.set(PROPERTY_VIDEO_CONTRAST, Integer.toString(value));

            try {
                value = 64*value/100;
                mRtkVoutUtilService.setContrast(value);
            } catch (Exception e) { Log.e(TAG, "Exception: ", e); return false;}
        } else { Log.e(TAG, "mRtkVoutUtilService is null!!"); return false;}
        return true;
    }

    public int getGraphicContrast() {
        return SystemProperties.getInt(PROPERTY_VIDEO_CONTRAST, 50);
    }

    public boolean setGraphicHue(int value) {
        if(mRtkVoutUtilService != null) {
            if (value > 100)
                value = 100;
            if (value < 0)
                value = 0;
            SystemProperties.set(PROPERTY_VIDEO_HUE, Integer.toString(value));

            try {
                value = 64*value/100;
                mRtkVoutUtilService.setHue(value);
            } catch (Exception e) { Log.e(TAG, "Exception: ", e); return false;}
        } else { Log.e(TAG, "mRtkVoutUtilService is null!!"); return false;}
        return true;
    }

    public int getGraphicHue() {
        return SystemProperties.getInt(PROPERTY_VIDEO_HUE, 50);
    }

    public boolean setGraphicSaturation(int value) {
        if(mRtkVoutUtilService != null) {
            if (value > 100)
                value = 100;
            if (value < 0)
                value = 0;
            SystemProperties.set(PROPERTY_VIDEO_SATURATION, Integer.toString(value));

            try {
                value = 64*value/100;
                mRtkVoutUtilService.setSaturation(value);
            } catch (Exception e) { Log.e(TAG, "Exception: ", e); return false;}
        } else { Log.e(TAG, "mRtkVoutUtilService is null!!"); return false;}
        return true;
    }

    public int getGraphicSaturation() {
        return SystemProperties.getInt(PROPERTY_VIDEO_SATURATION, 50);
    }

    public void setSuperResolutionOff(int off) {
        Log.d(TAG, "entering setSuperResolutionOff()...");

        if(mRtkVoutUtilService != null) {
            try {
                mRtkVoutUtilService.setSuperResolutionOff(off);
            } catch (Exception e) {
                Log.e(TAG, "Exception: ", e);
            }
        } else {
            Log.d(TAG, "mRtkVoutUtilService is null!!");
        }
    }

    public boolean setHdrtoSDRgma(int mode) {
        Log.d(TAG, "entering setHdrtoSDRgma(), mode=" + mode);
        boolean result = false;

        if(mRtkVoutUtilService != null) {
            try {
                mRtkVoutUtilService.setHdrtoSDRgma(mode);
                result = true;
            } catch (Exception e) {
                Log.e(TAG, "Exception: ", e);
            }
        } else {
            Log.d(TAG, "mRtkVoutUtilService is null!!");
        }

        return result;
    }

    public boolean setDisplayWindowDispZoomWin(int plane,Rect videoWin,Rect dispZoomWin) {
        boolean result = false;

        if (mRtkVoutUtilService != null) {
            try {
                Log.d(TAG, "RtkVoutUtilManager::setDisplayWindowDispZoomWin(int, Rect, Rect)");
                result = mRtkVoutUtilService.setDisplayWindowDispZoomWin(plane,videoWin,dispZoomWin);
            } catch (Exception e) {
                Log.e(TAG, "RtkVoutUtilManager::setDisplayWindowDispZoomWin(int, Rect, Rect): failed", e);
            }
        } else {
            Log.d(TAG, "RtkVoutUtilManager::setDisplayWindowDispZoomWin(int, Rect, Rect) [3]");
        }
        return result;
    }

    public boolean setMaxCLLMaxFALLDisable(int disable) {
        Log.d(TAG, "entering setMaxCLLMaxFALLDisable(), disable=" + disable);
        boolean result = false;

        if(mRtkVoutUtilService != null) {
            try {
                mRtkVoutUtilService.setMaxCLLMaxFALLDisable(disable);
                result = true;
            } catch (Exception e) {
                Log.e(TAG, "Exception: ", e);
            }
        } else {
            Log.d(TAG, "mRtkVoutUtilService is null!!");
        }

        return result;
    }
}
