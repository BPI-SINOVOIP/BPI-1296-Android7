// IRealtekTranscodeService.aidl
package com.realtek.transcode;

interface IRealtekTranscodeService {
    // getTranscodeInstanceIdx : get the index of new transcode instance
    // return : 0 ~ 3, -1 (error)
    int getTranscodeInstanceIdx();

    // setDataSource : the initialization for basic setting
    // idx : the index of transcode instance
    // source : the source file path
    // target : the output file path
    // track : 2 (audio + video), 0 (video only), 1(audio only)
    // repeat : for burn-in test only
	void setDataSource(int idx, String source, String target, String output, int track, int repeat);

    // setVideoParameter : the specific setting for video
    // idx : the index of transcode instance
    // width : target width
    // height : target height
    // frameRate : "Original", or any number ex. "30", "60", ...
    // iFrame : I frame interval ex. 1, 0, 3, 5, 10 ...
    // enableDeinterlace : 0 (RTK_AUTODEINT), 1 (RTK_FORCEDEINT), 2 (RTK_FORCEPROGRESSIVE)
    // bitrateMode : 0 : Disable, 1 : Variable, 2 : Constant, 3: VariableSkipFrames, 4 : ConstantSkipFrames
    // profile : 1 (AVCProfileBaseline), 8 (AVCProfileHigh), 2 (AVCProfileMain)
    // level : From 1 (AVCLevel1) to 8192 (AVCLevel42)
    // Note : System supports down scale only for resolution and frameRate
	void setVideoParameter(int idx, int width, int height, String frameRate, int bitRate, int iFrame, 
	int enableDeinterlace, int bitrateMode, int profile, int level);

    // setAudioParameter : the specific setting for audio
    // idx : the index of transcode instance
    // audioMode : "AAC", "ByPass"
    // sampleRate : "48000", "96000", "88200", "64000", "44100", "32000", "24000", "22050", "16000", "12000", "11025", "8000"
    // channel : "2", "3", "4", "5", "6", "1"
	void setAudioParameter(int idx, String audioMode, int sampleRate, int channel);

	// setStreamingParameter : the specific setting for UDP
	// idx : the index of transcode instance
	// ip : the UDP IP
	// port : the UDP port
	void setStreamingParameter(int idx, String ip, int port);

    // startTranscode : start transcode instance
    // idx : the index of transcode instance
    // return : 0, -1(error)
	int startTranscode(int idx);

    // stopTranscode : stop transcode instance
    // idx : the index of transcode instance
    // return : 0, -1(error)
	int stopTranscode(int idx);

    // isVideoTranscodeAlive : check video thread status
    // idx : the index of transcode instance
    // return : true, false
    boolean isVideoTranscodeAlive(int idx);

    // isAudioTranscodeAlive : check audio thread status
    // idx : the index of transcode instance
    // return : true, false
    boolean isAudioTranscodeAlive(int idx);

    // getTranscodeState : check current transcode state
    // idx : the index of transcode instance
    // return : -1(crash), 0(idle), 1(done)
    int getTranscodeState(int idx);

    // getCurrentPTS : get current PTS (transcode progress)
    // idx : the index of transcode instance
    // return : second
    // Note : return video PTS if video thread alive, otherwise return audio PTS
	long getCurrentPTS(int idx);

    // getDecodeAverageFPS : get decode average FPS (queue inputbuffer)
    // idx : the index of transcode instance
    // return : FPS
    // Note : return video FPS if video thread alive, otherwise return audio FPS
	double getDecodeAverageFPS(int idx);

    // getEncodeAverageFPS : get encode average FPS (muxer write)
    // idx : the index of transcode instance
    // return : FPS
	double getEncodeAverageFPS(int idx);

    // getVideoDecodeCurrentFPS : get video current decode FPS (queue inputbuffer)
    // idx : the index of transcode instance
    // return : FPS
    double getVideoDecodeCurrentFPS(int idx);

    // getVideoEncodeCurrentFPS : get video current decode FPS (muxer write) and clear both video decode and encode current FPS status
    // idx : the index of transcode instance
    // return : FPS 
    double getVideoEncodeCurrentFPS(int idx);
}
