package com.rtk.debug.reader;


public abstract class AbsLogcatReader implements LogcatReader {

	protected boolean recordingMode;
	
	public AbsLogcatReader(boolean recordingMode) {
		this.recordingMode = recordingMode;
	}

	public boolean isRecordingMode() {
		return recordingMode;
	}
}
