package com.realtek.hardware;

interface IRtkAoutUtilService
{
    boolean init();
    boolean setAudioSpdifOutput(int mode);
    boolean setAudioHdmiOutput(int mode);
    boolean setAudioAGC(int mode);
    boolean setAudioForceChannelCtrl(int mode);
    boolean setAudioHdmiFreqMode();
    void setAudioSurroundSoundMode(int mode);
    void setAudioNativeSR(int mode);
    void setAudioDelay(int delay);
    void setAudioHdmiMute(boolean bMute);
}
