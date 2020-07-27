package com.example.hdmirxdemo;

public class Keys {
    public static final String KEY_DIMENSION="dimension";
    public static final String KEY_TOUCHABLE="touchable";
    public static final String KEY_OUTPUT_MODE="outputMode";
    public static final String KEY_OUTPUT_PATH="outputPath";
    public static final String KEY_PORT="port";
    public static final String KEY_OUTPUT_FORMAT="outputFormat";
    public static final String KEY_RECORD_WIDTH="rec_width";
    public static final String KEY_RECORD_HEIGHT="rec_height";
    //public static final String KEY_DURATION_MIN="record_duration_min";
    public static final String KEY_RECORD_END_TIME_MILLIS="rec_end_time_millis";
    public static final String KEY_BOOKED_START_TIME_MILLIS="rec_start_time_millis";
    public static final String KEY_VIDEO_ON="videoOn";
    public static final String KEY_AUDIO_ON="audioOn";

    public static final int TYPE_TEXTUREVIEW    = 0;
    public static final int TYPE_SURFACEVIEW    = 1;
    public static final int TYPE_NOPREVIEW      = 2;

    public static final int EXIT_TO_PIP     = 0;
    public static final int EXIT_TO_HIDE    = 1;
    public static final int EXIT_TO_DELETE  = 2;

    public static final int OUTPUT_STORAGE  = 0;
    public static final int OUTPUT_UDP      = 1;
    public static final int OUTPUT_OFF      = 2;

    public static final int FORMAT_TS       = 0;
    public static final int FORMAT_MP4      = 1;

    private static final boolean G_DEBUG_FLAG = false;

    public static final boolean DB_LISTENER_DEBUG_VERBOSE = G_DEBUG_FLAG;
    public static final boolean DB_HELPER_DEBUG_VERBOSE = G_DEBUG_FLAG;
    public static final boolean FLT_WIN_VG_DEBUG_VERBOSE = G_DEBUG_FLAG;
    public static final boolean GENERAL_SERVICE_DEBUG_VERBOSE = G_DEBUG_FLAG;
    public static final boolean MAIN_ACT_DEBUG_VERBOSE = G_DEBUG_FLAG;
    public static final boolean HDMIRX_MANAGER_DEBUG_VERBOSE = G_DEBUG_FLAG;

    // following are the debug function control variables, do not enable them in production
    public static final boolean ENABLE_BATCH_DB_CREATE = false;

}
