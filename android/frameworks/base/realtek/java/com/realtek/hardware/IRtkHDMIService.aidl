package com.realtek.hardware;

import com.realtek.hardware.RtkTVSystem;
import com.realtek.hardware.IRtkHDMICallback;

interface IRtkHDMIService
{
	/* Get HDMI status getHDMIPlugged() and getHDMIReady() in Casablanca */
	boolean checkIfHDMIPlugged();
	boolean checkIfHDMIReady();

	/* Check if the box gets an valid EDID from the Sink (getHDMIEDIDReady() in Casablanca) */
	boolean checkIfHDMIEDIDReady();

	/* Mute/Un-mute the audio (Send_AudioMute) */
	boolean sendAudioMute(int select);

	/* set deep color mode */
	void setHDMIDeepColorMode(int mode);

	/* set 3d format */
	void setHDMIFormat3D(int format3d, float fps);

	/* Turn on/off HDCP state */
	int setHDCPState(int state);

	/* Get all the available TV system */
	int[] getVideoFormat();

	/* Get Next available TV system */
	int getNextNVideoFormat(int nextN);

	/* Get the sink's 3d capability */
	int getTV3DCapability();
	int[] getTVSupport3D();
	int[] getTVSupport3DResolution();
	boolean isTVSupport3D();

	/* Change the TV system */
	int setTVSystem(int inputTvSystem);

	/* Get the TV system */
	int getTVSystem();

	/* Get the TV system for restored */
	int getTVSystemForRestored();

	/* Get EDID Raw Data */
	byte[] getEDIDRawData();

	/* Check TV system */
	int checkTVSystem(int inputTvSystem, int colorspace_deepcolor);

	/* set color space and deep color mode */
	void setHDMIVideoColorMode(int mode);
	
	/* get Max Luminace */
	int getTVMaxLuminace();

    /* set HDR Eotf mode */
    int setHDREotfMode(int mode);

    /* get HDR Eotf mode */
    int getHDREotfValue();

    int getColorSpaceValue();

    int getDeepColorValue();

    boolean getHDCP1xEnabled();

    boolean getHDCP22Enabled();

    int getHDCPVersion();

    boolean setHDCPVersion(int hdcpVersion);

    void registerCallback(IRtkHDMICallback cb);

    void unregisterCallback(IRtkHDMICallback cb);

    void setHDMIEnable(int enable);

    int setTVSystem2(int inputTvSystem, boolean deepColorFirst);
}
