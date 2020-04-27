package com.rtk.debug.util;

import android.content.Context;
import android.util.Log;


public class LogLineAdapterUtil {
	
	public static final int LOG_WTF = 100; // arbitrary int to signify 'wtf' log level
	
	private static final int NUM_COLORS = 17;

	public static boolean logLevelIsAcceptableGivenLogLevelLimit(int logLevel, int logLevelLimit) {
			
		int minVal = 0;
		switch (logLevel) {
			
			case Log.VERBOSE:
				minVal = 0;
				break;
			case Log.DEBUG:
				minVal = 1;
				break;
			case Log.INFO:
				minVal = 2;
				break;
			case Log.WARN:
				minVal = 3;
				break;
			case Log.ERROR:
				minVal = 4;
				break;
			case LOG_WTF:
				minVal = 5;
				break;
			default: // e.g. the starting line that says "output of log such-and-such"
				return true;
		}
		
		return minVal >= logLevelLimit;

	}
}
