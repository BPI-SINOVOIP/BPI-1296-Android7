package com.realtek.runrunscript;

import android.util.Log;

import java.io.*;
import java.util.Arrays;


/**
 * Created by sam.chen on 7/10/17.
 */
public class MediaLogAnalysis {
    private static final String TAG = "MediaLogAnalysis";
    private MediaCase mCase;
    MediaLogAnalysis(MediaCase mc){
        mCase = mc;
    }

    public int analysis(){
        int ret = 0;
        File logFile = new File(mCase.getLogFile());
        Log.d(TAG, "analysis: " + logFile.toString());
        if(logFile.exists() && logFile.isFile()){
            try {
                InputStreamReader inputStreamReader = new InputStreamReader(new FileInputStream(logFile));
                BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
                String strLine = "";
                while ( (strLine = bufferedReader.readLine()) != null ) {
                    if(strLine.contains("[RTKRRS]")){
                        int pos = strLine.lastIndexOf(":");
                        ret = parsingMediaLine(strLine.substring(pos,strLine.length()));
                        if(ret != 0){
                            break;
                        }
                    }
                }
                inputStreamReader.close();
            }
            catch (FileNotFoundException e) {
                Log.e(TAG, "File not found: " + e.toString());
            } catch (IOException e) {
                Log.e(TAG, "Can not read file: " + e.toString());
            }
        }else{
            Log.e(TAG, "Log file not found: " + logFile.toString());
            return MediaErrorCode.ErrLogFileNotFound;
        }
        return ret;
    }

    private int parsingMediaLine(String info){
        Log.d(TAG, "parsing: " + info);
        String[] infolist = info.split("\\s+");
        int ErrPos = Arrays.asList(infolist).indexOf("ErrCode");
        if(ErrPos == -1)
            return MediaErrorCode.ErrParseLogcat;

        int ret = Integer.parseInt(infolist[ErrPos + 1]);
        if(ret == 0 && mCase.getFPS() > 0){
            int FpsPos= Arrays.asList(infolist).indexOf("FPS");
            if(FpsPos == -1)
                return MediaErrorCode.ErrParseLogcat;

            double fps = Double.parseDouble(infolist[FpsPos + 1]);
            mCase.setAvgFPS(fps); //for show on UI
            if(fps < mCase.getFPS()){
                ret = MediaErrorCode.ErrFPS;
            }
        }

        if(mCase.checkIgnoreErr(ret)){
            Log.d(TAG, "Ignore ErrCode: " + ret);
            return MediaErrorCode.OK;
        }
        return ret;
    }
}
