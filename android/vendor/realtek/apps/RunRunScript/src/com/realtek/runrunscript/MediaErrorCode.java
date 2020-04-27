package com.realtek.runrunscript;

/**
 * Created by sam.chen on 7/14/17.
 */
public class MediaErrorCode {
    public static final int OK = 0;
    public static final int ErrSourceIO = 1;
    public static final int ErrSelectTrack = 2;
    public static final int ErrCreateDecoderByType = 3;
    public static final int ErrCreateDecoder = 4;
    public static final int ErrCallback = 5;
    public static final int ErrDecodeThreadTerminate = 6;
    public static final int ErrIdel = 7;

    public static final int ErrLogFileNotFound = 100;
    public static final int ErrParseLogcat = 101;
    public static final int ErrFPS= 102;

    public static String toString(int ErrCode){
        String str;
        switch(ErrCode){
            case OK:
                str = "PASS";
                break;
            case ErrSourceIO:
                str = "setDataSource IO error";
                break;
            case ErrSelectTrack:
                str = "Select Track Error";
                break;
            case ErrCreateDecoderByType:
                str = "Create Decoder By Type Error";
                break;
            case ErrCreateDecoder:
                str = "Create Decoder Error";
                break;
            case ErrCallback:
                str = "Callback Error";
                break;
            case ErrDecodeThreadTerminate:
                str = "Decode Thread Terminate Error";
                break;
            case ErrIdel:
                str = "Idle Terminate";
                break;
            case ErrLogFileNotFound:
                str = "Logcat file not found";
                break;
            case ErrParseLogcat:
                str = "Logcat parsing failed!!";
                break;
            case ErrFPS:
                str = "FPS is not matched";
                break;
            default:
                str = "Unknown Error";
                break;
        }
        return str;
    }
}
