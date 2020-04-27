package com.realtek.DataProvider;

public class FileFilterType 
{
	public final static int DEVICE_FILE_AUDIO =  0x00000001;
	public final static int DEVICE_FILE_PHOTO =  0x00000002;
	public final static int DEVICE_FILE_VIDEO =  0x00000004;
	public final static int DEVICE_FILE_DIR   =  0x00000008;
	public final static int DEVICE_FILE_DOMAIN = 0x00000010;
	public final static int DEVICE_FILE_DEVICE = 0x00000020;
    public final static int DEVICE_FILE_DLNA_DEVICE = 0x00000040;
    public final static int DEVICE_FILE_SMB_DEVICE = 0x0000080;
    public final static int DEVICE_FILE_VIDEO_FILE_ONLY  = 0x00000104;
    public final static int DEVICE_FILE_VIDEO_ISO_ONLY  =  0x00000204;
    public final static int DEVICE_FILE_PHOTO_DATE = 0x00000402;
	public final static int DEVICE_FILE_APK   =  0x00000800;
	public final static int DEVICE_FILE_PLAYLIST   =  0x00001000;
    public final static int DEVICE_FILE_NONE  =  0x00002000;
	public static final int DEVICE_FILE_BD_FOLDER = 0x00004000;
}

