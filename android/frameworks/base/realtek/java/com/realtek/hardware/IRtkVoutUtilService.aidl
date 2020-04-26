package com.realtek.hardware;

import android.graphics.Rect;

interface IRtkVoutUtilService
{
    boolean init();
    boolean setRescaleMode(int mode);
    boolean setOSDWindow(in Rect osdWin);
    boolean nextZoom();
    boolean prevZoom();
    boolean isZooming();
    boolean resetZoom();
    boolean moveZoom(int keycode);
    boolean setFormat3d(int format3d, float fps);
    boolean set3dto2d(int srcformat3d);
    boolean set3dSub(int sub3d);
    boolean applyVoutDisplayWindowSetup();
    boolean setDisplayRatio(int ratio);
    boolean setDisplayPosition(int x, int y, int width, int height);
    void setEnhancedSDR(int flag);
    boolean isHDRtv();
    boolean setShiftOffset3dByPlane(boolean exchange_eyeview, boolean shift_direction, int delta_offset, int targetPlane);
    boolean isCVBSOn();
    void setCVBSOff(int off);
    void setPeakLuminance(int level);
    void setHdrSaturation(int level);
    void setHdmiRange(int rangeMode);
    void setCVBSDisplayRatio(int ratio);
    void setEmbedSubDisplayFixed(int fixed);
    void setBrightness(int value);
    void setContrast(int value);
    void setHue(int value);
    void setSaturation(int value);
    void setSuperResolutionOff(int off);
    void setHdrtoSDRgma(int mode);
    boolean setDisplayWindowDispZoomWin(int plane,in Rect videoWin,in Rect dispZoomWin);
    void setMaxCLLMaxFALLDisable(int disable);
}
