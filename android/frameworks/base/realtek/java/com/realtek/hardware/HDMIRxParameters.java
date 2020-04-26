package com.realtek.hardware;

import android.util.Log;
import android.text.TextUtils;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import com.realtek.hardware.RtkHDMIRxManager.Size;

public class HDMIRxParameters {
	// Parameter keys to communicate with the camera driver.
	private static final String KEY_PREVIEW_SIZE = "preview-size";
	private static final String KEY_PREVIEW_FORMAT = "preview-format";
	private static final String KEY_PREVIEW_FRAME_RATE = "preview-frame-rate";
	private static final String KEY_PREVIEW_FPS_RANGE = "preview-fps-range";
	private static final String KEY_PICTURE_SIZE = "picture-size";
	private static final String KEY_PICTURE_FORMAT = "picture-format";
	private static final String KEY_JPEG_THUMBNAIL_SIZE = "jpeg-thumbnail-size";
	private static final String KEY_JPEG_THUMBNAIL_WIDTH = "jpeg-thumbnail-width";
	private static final String KEY_JPEG_THUMBNAIL_HEIGHT = "jpeg-thumbnail-height";
	private static final String KEY_JPEG_THUMBNAIL_QUALITY = "jpeg-thumbnail-quality";
	private static final String KEY_JPEG_QUALITY = "jpeg-quality";
	private static final String KEY_ROTATION = "rotation";
	private static final String KEY_GPS_LATITUDE = "gps-latitude";
	private static final String KEY_GPS_LONGITUDE = "gps-longitude";
	private static final String KEY_GPS_ALTITUDE = "gps-altitude";
	private static final String KEY_GPS_TIMESTAMP = "gps-timestamp";
	private static final String KEY_GPS_PROCESSING_METHOD = "gps-processing-method";
	private static final String KEY_WHITE_BALANCE = "whitebalance";
	private static final String KEY_EFFECT = "effect";
	private static final String KEY_ANTIBANDING = "antibanding";
	private static final String KEY_SCENE_MODE = "scene-mode";
	private static final String KEY_FLASH_MODE = "flash-mode";
	private static final String KEY_FOCUS_MODE = "focus-mode";
	private static final String KEY_FOCUS_AREAS = "focus-areas";
	private static final String KEY_MAX_NUM_FOCUS_AREAS = "max-num-focus-areas";
	private static final String KEY_FOCAL_LENGTH = "focal-length";
	private static final String KEY_HORIZONTAL_VIEW_ANGLE = "horizontal-view-angle";
	private static final String KEY_VERTICAL_VIEW_ANGLE = "vertical-view-angle";
	private static final String KEY_EXPOSURE_COMPENSATION = "exposure-compensation";
	private static final String KEY_MAX_EXPOSURE_COMPENSATION = "max-exposure-compensation";
	private static final String KEY_MIN_EXPOSURE_COMPENSATION = "min-exposure-compensation";
	private static final String KEY_EXPOSURE_COMPENSATION_STEP = "exposure-compensation-step";
	private static final String KEY_AUTO_EXPOSURE_LOCK = "auto-exposure-lock";
	private static final String KEY_AUTO_EXPOSURE_LOCK_SUPPORTED = "auto-exposure-lock-supported";
	private static final String KEY_AUTO_WHITEBALANCE_LOCK = "auto-whitebalance-lock";
	private static final String KEY_AUTO_WHITEBALANCE_LOCK_SUPPORTED = "auto-whitebalance-lock-supported";
	private static final String KEY_METERING_AREAS = "metering-areas";
	private static final String KEY_MAX_NUM_METERING_AREAS = "max-num-metering-areas";
	private static final String KEY_ZOOM = "zoom";
	private static final String KEY_MAX_ZOOM = "max-zoom";
	private static final String KEY_ZOOM_RATIOS = "zoom-ratios";
	private static final String KEY_ZOOM_SUPPORTED = "zoom-supported";
	private static final String KEY_SMOOTH_ZOOM_SUPPORTED = "smooth-zoom-supported";
	private static final String KEY_FOCUS_DISTANCES = "focus-distances";
	private static final String KEY_VIDEO_SIZE = "video-size";
	private static final String KEY_PREFERRED_PREVIEW_SIZE_FOR_VIDEO =
										"preferred-preview-size-for-video";
	private static final String KEY_MAX_NUM_DETECTED_FACES_HW = "max-num-detected-faces-hw";
	private static final String KEY_MAX_NUM_DETECTED_FACES_SW = "max-num-detected-faces-sw";
	private static final String KEY_RECORDING_HINT = "recording-hint";
	private static final String KEY_VIDEO_SNAPSHOT_SUPPORTED = "video-snapshot-supported";
	private static final String KEY_VIDEO_STABILIZATION = "video-stabilization";
	private static final String KEY_VIDEO_STABILIZATION_SUPPORTED = "video-stabilization-supported";
	
	// Parameter key suffix for supported values.
	private static final String SUPPORTED_VALUES_SUFFIX = "-values";

	// LOG TAG
	private static final String TAG = "HDMIRxParameters";

	// KEY - VALUE  pairs
	private HashMap<String, String> mMap;

	public HDMIRxParameters() {
        mMap = new HashMap<String, String>(64);
	}

	//------------------------------------------------------------------
	
    /**
     * Creates a single string with all the parameters set in
     * this Parameters object.
     *
     * @return a String with all values from this Parameters object, in
     *		   semi-colon delimited key-value pairs
     */
    public String flatten() {
        StringBuilder flattened = new StringBuilder(128);
        for (String k : mMap.keySet()) {
            flattened.append(k);
            flattened.append("=");
            flattened.append(mMap.get(k));
            flattened.append(";");
        }
        // chop off the extra semicolon at the end
        flattened.deleteCharAt(flattened.length()-1);
        return flattened.toString();
    }

    /**
     * Takes a flattened string of parameters and adds each one to
     * this Parameters object.
     */
    public void unflatten(String flattened) {
        mMap.clear();

        TextUtils.StringSplitter splitter = new TextUtils.SimpleStringSplitter(';');
        splitter.setString(flattened);
        for (String kv : splitter) {
            int pos = kv.indexOf('=');
            if (pos == -1) {
                continue;
            }
            String k = kv.substring(0, pos);
            String v = kv.substring(pos + 1);
            mMap.put(k, v);
        }
    }

	public void remove(String key) {
		mMap.remove(key);
	}

	/**
	 * Sets a String parameter.
	 *
	 * @param key	the key name for the parameter
	 * @param value the String value of the parameter
	 */
	public void set(String key, String value) {
		if (key.indexOf('=') != -1 || key.indexOf(';') != -1 || key.indexOf(0) != -1) {
			Log.e(TAG, "Key \"" + key + "\" contains invalid character (= or ; or \\0)");
			return;
		}
		if (value.indexOf('=') != -1 || value.indexOf(';') != -1 || value.indexOf(0) != -1) {
			Log.e(TAG, "Value \"" + value + "\" contains invalid character (= or ; or \\0)");
			return;
		}
	
		mMap.put(key, value);
	}
	
	/**
	* Sets an integer parameter.
	*
	* @param key	the key name for the parameter
	* @param value the int value of the parameter
	*/
	public void set(String key, int value) {
		mMap.put(key, Integer.toString(value));
	}


	/**
	 * Returns the value of a String parameter.
	 *
	 * @param key the key name for the parameter
	 * @return the String value of the parameter
	 */
	public String get(String key) {
		return mMap.get(key);
	}

	/**
	 * Returns the value of an integer parameter.
	 *
	 * @param key the key name for the parameter
	 * @return the int value of the parameter
	 */
	public int getInt(String key) {
		return Integer.parseInt(mMap.get(key));
	}
	
	//------------------------------------------------------------------

	public void setPreviewSize(int width, int height) {
		String v = Integer.toString(width) + "x" + Integer.toString(height);
		set(KEY_PREVIEW_SIZE, v);
	}

	/**
	 * Returns the dimensions setting for preview
	 *
	 * @return a Size object with the width and height setting
	 *			for the preview picture
	 */
	public Size getPreviewSize() {
		String pair = get(KEY_PREVIEW_SIZE);
		return strToSize(pair);
	}
	
	public List<Size> getSupportedPreviewSizes(){
		String str = get(KEY_PREVIEW_SIZE + SUPPORTED_VALUES_SUFFIX);
		return splitSize(str);
	}
	
	public void setPreviewFrameRate(int fps) {
		set(KEY_PREVIEW_FRAME_RATE, fps);
	}

	public int getPreviewFrameRate() {
		return getInt(KEY_PREVIEW_FRAME_RATE);
	}

	/**
	 * Gets the supported preview frame rates.
	 *
	 * @return a list of supported preview frame rates. null if preview
	 *		   frame rate setting is not supported.
	 */
	public List<Integer> getSupportedPreviewFrameRates() {
		String str = get(KEY_PREVIEW_FRAME_RATE + SUPPORTED_VALUES_SUFFIX);
		return splitInt(str);
	}


	//------------------------------------------------------------------

	// Splits a comma delimited string to an ArrayList of Integer.
	// Return null if the passing string is null or the size is 0.
	private ArrayList<Integer> splitInt(String str) {
		if (str == null) return null;
	
		TextUtils.StringSplitter splitter = new TextUtils.SimpleStringSplitter(',');
		splitter.setString(str);
		ArrayList<Integer> substrings = new ArrayList<Integer>();
		for (String s : splitter) {
			substrings.add(Integer.parseInt(s));
		}
		if (substrings.size() == 0) return null;
		return substrings;
	}
	
	// Splits a comma delimited string to an ArrayList of Size.
	// Return null if the passing string is null or the size is 0.
	private ArrayList<Size> splitSize(String str) {
		if (str == null) return null;
	
		TextUtils.StringSplitter splitter = new TextUtils.SimpleStringSplitter(',');
		splitter.setString(str);
		ArrayList<Size> sizeList = new ArrayList<Size>();
		for (String s : splitter) {
			Size size = strToSize(s);
			if (size != null) sizeList.add(size);
		}
		if (sizeList.size() == 0) return null;
		return sizeList;
	}

	// Parses a string (ex: "480x320") to Size object.
	// Return null if the passing string is null.
	private Size strToSize(String str) {
		if (str == null) return null;

		int pos = str.indexOf('x');
		if (pos != -1) {
			String width = str.substring(0, pos);
			String height = str.substring(pos + 1);
			return new Size(Integer.parseInt(width),
							Integer.parseInt(height));
		}
		Log.e(TAG, "Invalid size parameter string=" + str);
		return null;
	}




}
