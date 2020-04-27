package com.rtk.debug.helper;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.preference.PreferenceManager;
import com.rtk.debug.R;
import com.rtk.debug.util.StringUtil;
import com.rtk.debug.util.UtilLogger;

public class PreferenceHelper {
	
	public static Map<Integer, Integer> ellipsisLengthsCache = 
		Collections.synchronizedMap(new HashMap<Integer,Integer>());
	
	private static float textSize = -1;
	private static Character defaultLogLevel = null;



	public static int getLogLinePeriodPreference(Context context) {
		SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(context);
		
		String defaultValue = context.getText(R.string.pref_log_line_period_default).toString();
		
		String intAsString = sharedPrefs.getString(context.getText(R.string.pref_log_line_period).toString(), defaultValue);
		
		try { 
			return Integer.parseInt(intAsString);
		} catch (NumberFormatException e) {
			return Integer.parseInt(defaultValue);
		}
	}

	public static char getDefaultLogLevelPreference(Context context) {
		
		if (defaultLogLevel == null) {
			SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(context);
			
			String logLevelPref = sharedPrefs.getString(
					context.getText(R.string.pref_default_log_level).toString(), 
					context.getText(R.string.log_level_value_verbose).toString());		
			
			defaultLogLevel = logLevelPref.charAt(0);
		}
		
		return defaultLogLevel;
		

	}

	public static final String DELIMITER = ",";
	public static List<String> getBuffers(Context context) {
		SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(context);
		
		String defaultValue = context.getString(R.string.pref_buffer_choice_main_value);
		String key = context.getString(R.string.pref_buffer);
		
		String value = sharedPrefs.getString(key, defaultValue);
		
		return Arrays.asList(StringUtil.split(value, DELIMITER));
	}

}
