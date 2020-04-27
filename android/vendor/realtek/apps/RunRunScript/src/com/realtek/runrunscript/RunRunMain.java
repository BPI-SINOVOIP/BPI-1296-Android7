package com.realtek.runrunscript;

import android.app.Activity;
import android.app.PackageInstallObserver;
import android.app.ProgressDialog;
import android.content.ComponentName;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;
import android.util.Log;
import android.widget.CheckBox;
import android.widget.TextView;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class RunRunMain extends Activity {
    private static final String TAG = "RunRunScript";
    private static final String TestReport = "RunRunTestReport.txt";
    private Button mBtnStart;
    private Button mBtnScan;
    private TextView mTextInfo;
    private TextView mTextVer;
    private CheckBox mCheckErr;

    private ProgressDialog progDialog;
    private List<File> scriptList = new ArrayList<>();
    private List<StorageUtils.StorageInfo> extVolList;
    private static List<MediaCase> testCases = new ArrayList<>();

    private static int mCurrent = 0;
    private static boolean isRunning = false;
    private static boolean islogging = false;
    private static boolean StopWhenErr = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mTextInfo = (TextView)findViewById(R.id.textInfo);
        mTextVer = (TextView)findViewById(R.id.textVersion);
        mBtnStart = (Button) findViewById(R.id.btnStart);
        mBtnStart = (Button) findViewById(R.id.btnStart);
        mCheckErr = (CheckBox) findViewById(R.id.checkBox);
        mCheckErr.setChecked(false);

        if(mBtnStart !=null ){
            mBtnStart.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    StopWhenErr = mCheckErr.isChecked();
                    Log.d(TAG, "Stop on Error: " + StopWhenErr);
                    doNextScript();
                }
            });
        }

        mBtnScan= (Button) findViewById(R.id.btnScan);
        if(mBtnScan !=null ){
            mBtnScan.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    Log.d(TAG, "Start test cases");
                    doScan();
                }
            });
        }
        mBtnScan.requestFocus();
        mBtnStart.setEnabled(false);
        mTextInfo.setText("");

        //version info
        try {
            PackageInfo pkgInfo = getPackageManager().getPackageInfo(getPackageName(),0);
            mTextVer.setText("Version: " + pkgInfo.versionName);
        } catch (PackageManager.NameNotFoundException e) {
            Log.d(TAG, "pm error: " + e.toString());
        }
    }

    private void initStatus(){
        isRunning = false;
        testCases.clear();
        mCurrent = 0;
        mBtnScan.requestFocus();
        mBtnStart.setEnabled(false);
        mTextInfo.setText("");
    }

    public boolean onKeyDown(int keyCode, KeyEvent e){
        switch(keyCode){
            case KeyEvent.KEYCODE_BACK:
            case KeyEvent.KEYCODE_HOME:
                initStatus();
                break;
        }
        return super.onKeyDown(keyCode, e);
    }
    @Override
    protected void onStart() {
        super.onStart();
        Log.d(TAG, "onStart");
    }
    @Override
    protected void onStop() {
        super.onStop();
        Log.d(TAG, "onStop");
    }

    @Override
    public void onResume(){
        super.onResume();
        Log.d(TAG, "onResume");
        if(isRunning){
            int ret;
            MediaCase mc = testCases.get(mCurrent);

            stopLogger();
            ret = logAnalysis(mc);
            if(StopWhenErr){
                if(ret == MediaErrorCode.OK) {
                    mCurrent++;
                    doNextScript();
                }else{
                    Log.w(TAG,"Got an error in log file: " + mc.getLogFile());
                    isRunning = false;
                    String info = "Running (" + (mCurrent+1) + "/" + testCases.size()+ ")\n";
                    if(ret == MediaErrorCode.ErrFPS){
                        info = info + "AvgFPS("+ String.format("%.2f", mc.getAvgFPS())+") is not matched TargetFPS("
                                + mc.getFPS() +") in" + mc.getMediaPath();
                        //mTextInfo.setText("AvgFPS("+ String.format("%.2f", mc.getAvgFPS())+") is not matched TargetFPS("
                        //        + mc.getFPS() +") in" + mc.getMediaPath() );
                    }else if(ret == MediaErrorCode.ErrLogFileNotFound){
                        String errStr = MediaErrorCode.toString(ret);
                        info = info +  errStr + ", file:" + mc.getLogFile();
                        //mTextInfo.setText(errStr + ", file:" + mc.getLogFile());
                    }else{
                        String errStr = MediaErrorCode.toString(ret);
                        info = info + errStr + " , file: " + mc.getMediaPath();
                        //mTextInfo.setText(errStr + " , file: " + mc.getMediaPath());
                    }
                    mTextInfo.setText(info);
                }

            }else{
                if(ret != MediaErrorCode.OK) {
                    String ErrStr = "";
                    if(ret == MediaErrorCode.ErrFPS){
                        ErrStr = "AvgFPS("+ String.format("%.2f", mc.getAvgFPS())+") is not matched TargetFPS("
                                + mc.getFPS() +") in" + mc.getMediaPath();
                    }else{
                        ErrStr = MediaErrorCode.toString(ret) + ", file:" + mc.getLogFile();
                    }
                    //write to file
                    String ErrorFilePath = mc.getMountPoint() + "/" + TestReport;
                    writeToFile(ErrorFilePath, ErrStr);
                }
                mCurrent++;
                doNextScript();
            }
        }
    }
    private void doScan(){
        Log.d(TAG, "doScan");
        final boolean hasExtVolume = StorageUtils.hasExtStorage(this);
        mBtnStart.setEnabled(false);
        if(hasExtVolume){
            extVolList = StorageUtils.getStorageList(this);
            progDialog = new ProgressDialog(RunRunMain.this);
            progDialog.setMessage("loading....");
            progDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            progDialog.setProgress(0);
            progDialog.setMax(1);
            progDialog.setOnDismissListener(new DialogInterface.OnDismissListener(){
                @Override
                public void onDismiss(DialogInterface dialog){
                    Log.d(TAG, "onSearch done found: " + scriptList.size());
                    //script searching is done.
                    if(scriptList.size() > 0){
                        for(File spell: scriptList){
                            Log.d(TAG,"Running: " + spell.getAbsolutePath() + ", dirpath:" + spell.getParent());
                            String strJSON = Read2Str(spell);
                            //Log.d(TAG,"script:\n" + strJSON);
                            try{
                                JSONObject jsonRoot = new JSONObject(strJSON);
                                String testType = jsonRoot.getString("type");
                                if(testType.equals("media")){
                                    MediaPaser(jsonRoot, spell.getParent());
                                }
                            }catch(JSONException e){
                                Log.e(TAG,"JSONException: \n" + e.toString());
                            }
                        }
                        if(testCases.size() > 0) {
                            mTextInfo.setText("Found " + testCases.size() + " test cases.");
                            mBtnStart.setEnabled(true);
                            mCurrent = 0;
                        }else {
                            mTextInfo.setText("No test case had found!");
                        }
                    }

                }
            });
            progDialog.show();
            new Thread(new Runnable(){
                @Override
                public void run(){
                    Log.d(TAG, "onSearch");
                    scriptList.clear();
                    testCases.clear();
                    mCurrent = 0;
                    //--for test Dialog---
                    /*try {
                        Thread.sleep(2000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }*/
                    //---------------------
                    for(StorageUtils.StorageInfo info: extVolList){
                        Log.d(TAG, "Volume dir:" + info.path + ", RO:" + info.readonly + " , isSD:" + info.isSdCard);
                        if(!info.readonly && !info.isSdCard && !info.path.startsWith("/storage/emulated/0")){
                            Log.d(TAG, "Scan Volume dir:" + info.path);
                            //search script files in external storage
                            scriptSearcher(new File(info.path));
                        }
                    }
                    progDialog.dismiss();
                }
            }).start();
        }else{
            Log.d(TAG, "No External Storage found");
            mTextInfo.setText("No external storage!");
        }


    }

    public void scriptSearcher(File dir){
        File listFile[] = dir.listFiles();
        for(File p: listFile){
            //Log.d(TAG, "file path:" + p.getAbsolutePath());
            if(p.isDirectory()){
                //scriptList.add(p);
                scriptSearcher(p);
            }else{
                if(p.getName().equals("rtkRunScript")){
                    Log.d(TAG, "found path:" + p.getAbsolutePath());
                    scriptList.add(p);
                }
            }
        }
    }

    private void doNextScript(){

        Log.d(TAG, "Start test cases (" + mCurrent +"/"+ testCases.size() +")");
        if(testCases.size() < 0 || mCurrent == testCases.size()){
            Log.d(TAG, "Out of range!!");
            return;
        }

        if(islogging){
            stopLogger();
            if(mCurrent < testCases.size()){
                Log.d(TAG,"LogcatService is restarted");
                startLogger(testCases.get(mCurrent));
            }else{
                //out of test cases
                Log.d(TAG,"out of test cases");
                return;
            }

        }else{
            startLogger(testCases.get(mCurrent));
        }

        startPlayer(testCases.get(mCurrent));
        isRunning = true;
    }

    private String Read2Str(File f){
        String ret = "";
        try {
            InputStreamReader inputStreamReader = new InputStreamReader(new FileInputStream(f));
            BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
            String receiveString = "";
            StringBuilder stringBuilder = new StringBuilder();

            while ( (receiveString = bufferedReader.readLine()) != null ) {
                stringBuilder.append(receiveString);
            }
            inputStreamReader.close();
            ret = stringBuilder.toString();
        }
        catch (FileNotFoundException e) {
            Log.e(TAG, "File not found: " + e.toString());
        } catch (IOException e) {
            Log.e(TAG, "Can not read file: " + e.toString());
        }
        return ret;
    }

    private void startPlayer(MediaCase mc){
        //play video from PIPDemo
        Intent intent = new Intent();
        ComponentName compName = new ComponentName("com.mediacodec","com.mediacodec.DecodeActivity");
        intent.setComponent(compName);
        intent.putExtra("DATA_INT",1);
        intent.putExtra("LIMIT_INT",1);
        intent.putExtra("LIMIT_AUD_INT",0);
        intent.putExtra("REPEAT",0);
        intent.putExtra("AUDIOCLICK",0);
        intent.putExtra("FREERUN",1);
        intent.putExtra("TUNNELMODE",mc.isTunnel());
        ArrayList<String> playList = new ArrayList<>();
        playList.add(mc.getMediaPath());
        intent.putStringArrayListExtra("LIST", playList);
        intent.setAction(Intent.ACTION_VIEW);
        mTextInfo.setText("Running (" + (mCurrent+1) + "/" + testCases.size()+ ")");
        Log.d(TAG,"Current run: " + mCurrent);
        startActivity(intent);

        //play video from gallery3d
        /*
        final Intent intent = new Intent();
        ComponentName compName = new ComponentName("com.android.gallery3d","com.android.gallery3d.app.MovieActivity");
        intent.setComponent(compName);
        intent.putExtra("SourceFrom","Local");
        intent.setAction(Intent.ACTION_VIEW);
        intent.setData(testCases.get(mCurrent).getMediaURI());
        mTextInfo.setText("Running (" + (mCurrent+1) + "/" + testCases.size()+ ")");
        Log.d(TAG,"Current run: " + mCurrent);
        startActivity(intent);
        */
    }

    private void MediaPaser(JSONObject rootObj, String scriptDir) throws JSONException {
        JSONArray items = rootObj.getJSONArray("cases");
        for(int i =0; i < items.length(); i++){
            Log.d(TAG, "item: " + items.getString(i));
            JSONObject obj = items.getJSONObject(i);
            //testCases.add(new MediaCase(scriptDir, obj.getString("file")));
            testCases.add(new MediaCase(scriptDir, obj));
        }
    }

    private void startLogger(MediaCase mc){
        if(islogging)
            return;

        islogging = true;
        File log = new File(mc.getLogFile());
        if(log.exists()){
            boolean del = log.delete();
            Log.d(TAG, "Delete exists log file: " + log.toString() + ", del: " + del);
            try {
                log.createNewFile();
            } catch (IOException e) {
                Log.d(TAG, "Create file error: " + log.toString());
            }
        }
        Intent logIntent = new Intent();
        ComponentName logName = new ComponentName("com.rtk.debug", "com.rtk.debug.LogcatService");
        logIntent.setComponent(logName);
        logIntent.putExtra("EXTRA_TASK", "DumpLog");
        logIntent.putExtra("EXTRA_SAVE_PATH", log.getAbsolutePath());
        logIntent.putExtra("EXTRA_CLEAR", true);
        startService(logIntent);
        Log.d(TAG,"LogcatService is started , logging: " + islogging);
    }
    private void stopLogger(){
        if(!islogging)
            return;

        islogging = false;
        Intent logIntent = new Intent();
        ComponentName logName = new ComponentName("com.rtk.debug", "com.rtk.debug.LogcatService");
        logIntent.setComponent(logName);
        logIntent.putExtra("EXTRA_TASK", "StopDumpLog");
        startService(logIntent);
        Log.d(TAG,"LogcatService is stopped, logging: " + islogging);
    }

    private int logAnalysis(MediaCase mc){
        MediaLogAnalysis logAnz = new MediaLogAnalysis(mc);
        return logAnz.analysis();
    }

    private void writeToFile(String file, String str) {
        Log.d(TAG,"Write " + str + " to " + file);
        String outStr;
        SimpleDateFormat timeFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        try{
            BufferedWriter bw = new BufferedWriter(new FileWriter(file, true));
            Date date = new Date();
            String append = timeFormat.format(date);
            outStr=append +": "+str+"\n";
            bw.write(outStr, 0, outStr.length());
            bw.close();
        } catch (Exception e) {
            Log.d(TAG, "Err:"+e.getMessage());
        }
    }
}
