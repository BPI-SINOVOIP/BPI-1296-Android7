package com.realtek.Utils;
import android.util.Log;

import java.io.*;
import java.util.ArrayList;


public class BDParser {

    private static final String LOG_TAG = "BDParser";
    private static final String DVD_VIDEO_FOLDER = "VIDEO_TS";
    private static final String DVD_VIDEO_INFO = "VIDEO_TS.IFO";
    //BD Level 1
    private static final String BD_FOLDER_BDMV = "BDMV";
    //BD Level 2
    private static final String BD_FOLDER_CLIPINFO = "CLIPINF";
    private static final String BD_FOLDER_PLAYLIST = "PLAYLIST";
    private static final String BD_FOLDER_STREAM = "STREAM";
    private static final String BD_STREAM = "m2ts";
    
    private String mPath;
    private String mPlayEntry;
    private boolean mIsDir;
    private int mFolderType;
    public static final int DVD_TYPE1=101;//have VIDEO_TS
    public static final int DVD_TYPE2=102;//don't have VIDEO_TS
    public static final int BD_TYPE1=201;// have BDMV
    public static final int BD_TYPE2=202;// don't have BDMV
    public static final int NOT_BD_DVD=0;

    public BDParser(String path)
    {
        mPath = path;
        mFolderType = NOT_BD_DVD;
        CheckBD_DVD();
    }
    public int getType()
    {
        Log.d(LOG_TAG,"getType()="+mFolderType); 
        return mFolderType;
    }
    public String getPlayEntry()
    {
        Log.d(LOG_TAG,"getPlayEntry()="+mPlayEntry);
        return mPlayEntry;
    }

    private void log(String s){
        Log.d(LOG_TAG, s);
    }

    private boolean CheckBD_DVD()
    {
        boolean retval= false;
        File file = new File(mPath);
        int MatchedItems=0;//0x1:CLIPINFO 0x2:PLAYLIST 0x4:STREAM
        if (!file.isDirectory() || !file.canRead())
        {
            //Log.d(LOG_TAG,"It's not a folder or can not read");
            return false;
        }
        File[] files = file.listFiles(new FileFilter() {
            @Override
            public boolean accept(File file) {
                if(file.isDirectory()){
                    switch (file.getName()){
                        case BD_FOLDER_BDMV:
                        case BD_FOLDER_CLIPINFO:
                        case BD_FOLDER_PLAYLIST:
                        case BD_FOLDER_STREAM:
                        case DVD_VIDEO_FOLDER:
                            return true;
                    }
                } else if(file.getName().equals(DVD_VIDEO_INFO))
                    return true;
                return false;
            }
        });
        if (files == null)
        {
            //Log.d(LOG_TAG,"files = null");
            return false;
        }
        if(files.length <=0)
        {
            //Log.d(LOG_TAG,"files length ="+files.length);
            return false;
        }
        for (File file1 : files) {
            boolean isDirectory = file1.isDirectory();
            //Log.d(LOG_TAG,"CheckBD_DVD Folder ="+FileName+"is Dir="+isDirectory);
            if (isDirectory) {
                String FileName = file1.getName();
                if (FileName.equals(BD_FOLDER_BDMV))//It's a BDMV folder
                {
                    int s_MatchedItems = 0;//0x1:CLIPINFO 0x2:PLAYLIST 0x4:STREAM
                    File s_file = new File(mPath + "/" + FileName);
                    File[] s_files = s_file.listFiles();
                    for (File s_file1 : s_files) {
                        String s_FileName = s_file1.getName();
                        boolean s_isDirectory = s_file1.isDirectory();
                        //Log.d(LOG_TAG,"CheckBD_DVD Folder ="+s_FileName+"is Dir="+s_isDirectory);
                        if (s_isDirectory) {
                            if (s_FileName.equals(BD_FOLDER_CLIPINFO)) {
                                s_MatchedItems = s_MatchedItems | 0x1;
                            } else if (s_FileName.equals(BD_FOLDER_PLAYLIST)) {
                                s_MatchedItems = s_MatchedItems | 0x2;
                            } else if (s_FileName.equals(BD_FOLDER_STREAM)) {
                                String BigM2TS = FindBigM2TS(mPath + "/" + FileName + "/" + s_FileName);
                                if (BigM2TS != null) {
                                    //mPlayEntry = mPath+"/"+BD_FOLDER_BDMV+"/"+BD_FOLDER_STREAM+"/"+BigM2TS;
                                    mPlayEntry = mPath;
                                    s_MatchedItems = s_MatchedItems | 0x4;
                                }
                            }
                            if (s_MatchedItems == 0x7) {
                                mFolderType = BD_TYPE1;
                                retval = true;
                                break;
                            }
                        }
                    }
                    if (mFolderType == BD_TYPE1) {
                        break;
                    }
                } else if (FileName.equals(BD_FOLDER_CLIPINFO)) {
                    MatchedItems = MatchedItems | 0x1;
                } else if (FileName.equals(BD_FOLDER_PLAYLIST)) {
                    MatchedItems = MatchedItems | 0x2;
                } else if (FileName.equals(BD_FOLDER_STREAM)) {
                    String BigM2TS = FindBigM2TS(mPath + "/" + FileName);
                    if (BigM2TS != null) {
                        //mPlayEntry = mPath+"/"+BD_FOLDER_STREAM+"/"+BigM2TS;
                        mPlayEntry = mPath;
                        MatchedItems = MatchedItems | 0x4;
                    }
                }
                if (MatchedItems == 0x7) {
                    mFolderType = BD_TYPE2;
                    retval = true;
                    break;
                }

            }
        }
        if (mFolderType==NOT_BD_DVD)
        {
            CheckDVD(files);
        }
        return retval;
    }

    private boolean CheckDVD(File[] files)
    {
        boolean retval = false;
        if (files == null)
        {
            return false;
        }
        if(files.length <=0)
        {
            return false;
        }
        for (File file1 : files) {
            String FileName = file1.getName();
            boolean isDirectory = file1.isDirectory();
            //Log.d(LOG_TAG,"CheckDVD Folder ="+FileName+"is Dir="+isDirectory);
            //Check DVD
            if (isDirectory) {
                if (FileName.equals(DVD_VIDEO_FOLDER)) {
                    File s_file = new File(mPath + "/" + FileName);
                    File[] s_files = s_file.listFiles();
                    for (File s_file1 : s_files) {
                        String s_FileName = s_file1.getName();
                        //Log.d(LOG_TAG,"CheckDVD Folder ="+s_FileName);
                        if (s_FileName.equals(DVD_VIDEO_INFO)) {
                            mFolderType = DVD_TYPE1;
                            //mPlayEntry = mPath+"/"+DVD_VIDEO_FOLDER+"/"+DVD_VIDEO_INFO;
                            mPlayEntry = mPath;
                            retval = true;
                            break;
                        }
                    }
                    if (mFolderType == DVD_TYPE1) {
                        break;
                    }
                }
            } else {
                if (FileName.equals(DVD_VIDEO_INFO)) {
                    mFolderType = DVD_TYPE2;
                    //mPlayEntry = mPath+"/"+DVD_VIDEO_INFO;
                    mPlayEntry = mPath;
                    retval = true;
                    break;
                }
            }
        }
        return retval;
    }
    private String FindBigM2TS(String Path)
    {
        File file = new File(Path);
        File[] files = file.listFiles();
        int BigIdx=0;
        long BigSize=0;
        if (files.length ==0)
        {
            return null;
        }
        for (int i=0; i<files.length; i++)
        {
            if (files[i].length()>BigSize)
            {
                BigIdx = i;
                BigSize = files[i].length();
            }
        }
        return files[BigIdx].getName();
    }
}
