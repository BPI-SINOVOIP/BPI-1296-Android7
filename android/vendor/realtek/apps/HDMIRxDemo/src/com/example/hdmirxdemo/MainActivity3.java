package com.example.hdmirxdemo;

import android.content.Context;
import android.content.res.Resources;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.res.ColorStateList;
import android.app.Activity;
import android.app.PendingIntent;
import android.app.AlarmManager;
import android.app.Dialog;
import android.app.AlertDialog;
import android.os.Handler;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.StatFs;
import android.os.Environment;
import android.os.SystemClock;
import android.os.SystemProperties;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.Window;
import android.view.LayoutInflater;
import android.widget.ListView;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.Spinner;
import android.widget.ArrayAdapter;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.Toast;
import android.widget.DatePicker;
import android.widget.TimePicker;
import android.widget.NumberPicker;
import android.view.KeyEvent;
import android.view.WindowManager;
import android.util.DisplayMetrics;
import android.graphics.drawable.Drawable;

import android.net.ConnectivityManager;
import android.net.NetworkInfo;

import java.util.GregorianCalendar;
import java.util.Calendar;
import java.util.List;
import java.util.ArrayList;
import java.util.Date;
import java.util.StringTokenizer;

import com.realtek.hardware.RtkHDMIRxManager;
import com.realtek.hardware.RtkHDMIRxManager.Size;
import com.realtek.hardware.InvalidOpException;
import com.realtek.hardware.HDMIRxStatus;
import com.realtek.hardware.HDMIRxParameters;

public class MainActivity3 extends Activity implements View.OnClickListener,
                                                View.OnFocusChangeListener,
                                                ViewTreeObserver.OnGlobalLayoutListener,
                                                View.OnKeyListener,
                                                CompoundButton.OnCheckedChangeListener,
                                                SchRecListViewListener,
                                                DbChangeUIListener,
                                                BroadcastListener,
                                                WindowAliveReplyer {

    public static final boolean DEBUG_VERBOSE = Keys.MAIN_ACT_DEBUG_VERBOSE;

    public static final String TAG="MainActivity3";
    public static final String TAG_VERBOSE = TAG+"-VERBOSE";

    private static final String WRN_NO_HDMI_IN_SIGNAL = "No HDMI in Signal";

    private static final String REC_BTN_TEXT_RECORD = "Record";
    private static final String REC_BTN_TEXT_STOP_RECORD = "Stop record";

    private static final String BC_BTN_TEXT_EN="Broadcast";
    private static final String BC_BTN_TEXT_UN="Stop broadcast";

    // SharedPreferences key
    private static final String SP_KEY_IP="ip";
    private static final String SP_KEY_PORT="port";
    private static final String SP_KEY_RESOLUTION="resolution";
    private static final String SP_KEY_FORMAT="format";
    private static final String SP_KEY_MUTE="mute";
    private static final String SP_KEY_PATH="path";

    /*
    public static final String SP_KEY_X = "x";
    public static final String SP_KEY_Y = "y";
    public static final String SP_KEY_W = "w";
    public static final String SP_KEY_H = "h";
    */

    public static int PIP_X = -1;
    public static int PIP_Y = -1;
    public static int PIP_W = -1;
    public static int PIP_H = -1;

    private static final int STATE_RESUMED = 0;
    private static final int STATE_PAUSED = 1;

    private static final int FILE_PICK = 123;
    private static final int TIMEPICKER_ADJUST_MIN = 1;

    private static final String RIGHT_CLICK_BC = "com.rtk.ControlPanel.ControlPanel_Action";

    private Context mContext;
    private HDMIDbHelper mDbHelper;
    private List<SchRecItem2> mSchRecItem2List;

    private boolean bPreviewInFullScreen;

    // title members
    private TextView mTitleTextView;

    // preview members
    private Button mPIPButton;
    private Button mBroadcastButton;
    private TextView mPreviewSubTitleTextView;
    private View mRxBackground;
    private TextView mNoSignalTextView;
    private ImageButton mPreviewSettingButton;
    private CheckBox mMuteCheckBox; 
    private Dimension mRxBackgroundDimension;
    private boolean mStartPIPOnExit;

    // record setting members
    private TextView mRecordTitleTextView;
    private Button mRecordButton;
    private SpinnerWidget mResolutionSpinner;
    private SpinnerWidget mFormatSpinner;
    private TextView mPathTitleTextView;
    private TextView mPathTextView;
    private Button mPathButton;
    private String mRecPath;

    // schedule record members
    private SchRecListView mRecListView;
    private Button mAddRecButton;
    private TextView mAddRecTextView;

    private HDMIInfo mHDMIInfo;
    private boolean bFinishOnPause;

    private Dimension mFloatingDimension = new Dimension(1920-(192*3)-30,1080-(108*3)-70,192*3,108*3);
    private Dimension mInvisibleDimension = new Dimension(5000,5000,1,1);

    private Dialog mDateDialog = null;
    private Dialog mBroadcastDialog = null;
    private Dialog mWarningDialog = null;
    private Dialog mRecordDialog = null;

    //private boolean mAudioStatus = true;
    private SharedPreferences mSharedPrefs = null;
    private boolean mWindowFocusState = false;

    // NOTE: these receivers are not working, but keep them for now..
    private RxBroadcastReceiver mRxReceiver = null;
    private HDCPBroadcastReceiver mHDCPReceiver = null;

    private PlatformRightClickBroadcastReceiver mRightClickReceiver = null;

    private int mReceiverPriority = -200;

    private boolean bForwardFocus = false;
    private boolean bCanAcceptOKKey = false;

    private int mAppState;
    private boolean mDpadCenterConsumed;
    /*
    static {
        System.loadLibrary("hdmidemoutils");
    }

    // check signature info, if check is invalid, then it would crash in the native function
    public native void check();
    */

    /**
     * low priority broadcast receiver to handle activity task...
     */
    private class RxBroadcastReceiver extends BroadcastReceiver {
        public void onReceive(Context context, Intent intent) {

            String action = intent.getAction();
            if(action.equals(MyReceiver.RX_STATUS_STRING)){
                if (!(intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_PLUGGED_STATE, false))){
                    Log.d(TAG, "Useless receiver - not working");
                }else{
                    Log.d(TAG, "Useless receiver - not working");
                }
            }
        }
    }

    /**
     * low priority broadcast receiver to handle activity task...
     */
    private class HDCPBroadcastReceiver extends BroadcastReceiver {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if(action.equals(MyReceiver.RX_HDCP_STRING)){
                if (intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_HDCP_STATUS, false)){
                    Log.d(TAG, "HDCP on - do nothing..");
                }else{
                    Log.d(TAG, "HDCP off - do nothing");
                }
            }
        }
    }

    private class PlatformRightClickBroadcastReceiver extends BroadcastReceiver {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if(action.equals(RIGHT_CLICK_BC)){
                /*
                if(!bPreviewInFullScreen){
                    finish();
                }else{
                    pipDisplayRxPreview();
                }
                */
            }
        }
    }

    class StopServiceTask extends AsyncTask<Void, Void, Boolean> {
        protected Boolean doInBackground(Void... voids){
            if(FloatingWindowService2.bServiceIsAlive){
                // stop floating window service
                Intent serviceIntent = new Intent(
                    mContext, com.example.hdmirxdemo.FloatingWindowService2.class);
                mContext.stopService(serviceIntent);
            }
            return true;
        }

        protected void onPostExecute(Boolean result){
            Log.d(TAG, "StopServiceTask onPostExecute");
        }
    }

    class Dimension {
        int width;
        int height;
        int x;
        int y;

        public Dimension(int x, int y, int w, int h) {
            width = w;
            height = h;
            this.x = x;
            this.y = y;
        }

        public int[] toIntArray() {
            int[] rst = new int[4];
            rst[0] = x;
            rst[1] = y;
            rst[2] = width;
            rst[3] = height;
            return rst;
        }
    }

    class HDMIInfo {

        private static final String INFO_TAG="MainActivity3-HDMIInfo";
        int width = 0;
        int height = 0;
        int fps = 0;
        List<Size> allSizeList;
        List<Size> filteredSizeList = new ArrayList<Size>();

        private void dumpInfo(){
            Log.d(INFO_TAG, "width="+width+" height="+height+" fps="+fps);
            if(allSizeList != null && allSizeList.size() > 0){
                for(int i=0;i<allSizeList.size();i++){
                    Log.d(INFO_TAG, "size "+allSizeList.get(i).width+","+allSizeList.get(i).height);
                }
            }else{
                Log.e(INFO_TAG,"resolution info error");
            }
            if(filteredSizeList.size()>0){
                for(int i=0;i<filteredSizeList.size();i++) {
                    Log.d(INFO_TAG, "filtered size "+filteredSizeList.get(i).width+","+filteredSizeList.get(i).height);
                }
            }else{
                Log.e(INFO_TAG, "filteredlist empty");
            }
        }

        private void buildFilteredList() {
            if(allSizeList!=null && allSizeList.size() > 0) {
                for(int i=0;i<allSizeList.size();i++){
                    Size s = allSizeList.get(i);
                    if(s.width<=width && s.height<=height){
                        if(s.width==1920 || (s.width==1280 && s.height==720) || s.width==640){
                            filteredSizeList.add(s);
                        }
                    }
                }
            }
        }

        private CharSequence[] getResolutionArray(){
            if(filteredSizeList.size()==0)
                return null;
            int size = filteredSizeList.size();
            CharSequence[] rst = new CharSequence[size];
            for(int i=0;i<size;i++){
                Size s = filteredSizeList.get(size - 1 - i);
                rst[i] = (CharSequence)(""+s.width+"x"+s.height);
            }
            return rst;
        }
    }

    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        //Utils.getInstance().init();
        Log.d(TAG,"onCreate");
        mDpadCenterConsumed = false;
        registerBroadcastReceivers();
        mContext = this;
        bCanAcceptOKKey = false;
        mDbHelper = HDMIDbHelper.getInstance(this); //new HDMIDbHelper(this);
        // for the case has no boot complete.
        GeneralService2.getInstance(this);
        mRxBackgroundDimension = null;
        setContentView(R.layout.activity_main_3);

        mTitleTextView = (TextView) findViewById(R.id.title_textview);
        mTitleTextView.setText("HDMI IN");
    }

    public void onDestroy (){
        super.onDestroy();
        unregisterBroadcastReceivers();
    }

    protected void onResume(){
        super.onResume();
        Log.d(TAG, "onResume");

        FloatingWindowService2.mWindowAliveReplyer = this;

        mStartPIPOnExit = false;
        bPreviewInFullScreen = false;
        bFinishOnPause = true;
        mDateDialog = null;
        mDbHelper.deleteOutdatedItems();
        getHDMIRxStatus();
        initPreviewGroup();
        initRecGroup();
        initSchRecGroup();

        loadAppStatus();

        //boolean aud

        startOrResumeFloatingWindowService();

        updateRecButtonAppearance();

        // refresh mute checkbox again
        //if(FloatingWindowService2.bServiceIsAlive){
        //    mMuteCheckBox.setChecked(!isAudioOn());
        //}

        mDbHelper.setDbChangeUIListener(this);
        // register listener after 
        MyReceiver.sListener = this;
        FloatingWindowService2.sListener = this;
        mAppState = STATE_RESUMED;
    }

    public void onPause(){
        super.onPause();

        Log.d(TAG, "onPause");

        FloatingWindowService2.mWindowAliveReplyer = null;
        mAppState = STATE_PAUSED;

        saveAppStatus();

        if(bFinishOnPause){
            mDbHelper.setDbChangeUIListener(null);
            MyReceiver.sListener = null;
            FloatingWindowService2.sListener = null;
        }
        //mAudioStatus = !mMuteCheckBox.isChecked();

        if(mDateDialog!=null){
            mDateDialog.dismiss();
            mDateDialog = null;
        }

        if(mBroadcastDialog!=null){
            mBroadcastDialog.dismiss();
            mBroadcastDialog = null;
        }

        if(mRecordDialog!=null){
            mRecordDialog.dismiss();
            mRecordDialog = null;
        }

        dismissWarningDialog();

        boolean makeServiceInBg = false;
        Dimension position = mInvisibleDimension;
        boolean vOn = false;
        boolean aOn = false;

        if(mStartPIPOnExit){
            makeServiceInBg = true;
            position = mFloatingDimension;
            vOn = true;
            aOn = true && isAudioOn();
        }else if(serviceIsRecording()) {
            makeServiceInBg = true;
            position = mInvisibleDimension;
            vOn = false;
            aOn = false;
        }

        if(makeServiceInBg){
            setPlayback(vOn,aOn);
            setFloatingWindowSize(position.toIntArray(),true);
            if(FloatingWindowService2.bServiceIsAlive){
                //FloatingWindowService2.showFocusOverlay(false);
            }
        }else{
            stopFloatingWindowService();
        }

        if(bFinishOnPause){
            finish();
        }
    }

    private void setControlButtonsEnable(boolean enabled){
        if(mPIPButton != null) {
            mPIPButton.setEnabled(enabled);
        }
        mRecordButton.setEnabled(enabled);
        mBroadcastButton.setEnabled(enabled);
    }

    private void registerBroadcastReceivers(){
        /*
        if(mRxReceiver == null){
            Log.d(TAG, "register mRxReceiver");
            mRxReceiver = new RxBroadcastReceiver();
            IntentFilter filter = new IntentFilter(MyReceiver.RX_STATUS_STRING);
            filter.setPriority(mReceiverPriority);
            registerReceiver(mRxReceiver,filter);
        }

        if(mHDCPReceiver == null){
            Log.d(TAG, "register mHDCPReceiver");
            mHDCPReceiver = new HDCPBroadcastReceiver();
            IntentFilter filter_hdcp = new IntentFilter(MyReceiver.RX_HDCP_STRING);    
            filter_hdcp.setPriority(mReceiverPriority);
            registerReceiver(mHDCPReceiver,filter_hdcp);
        }
        */

        if(mRightClickReceiver == null) {
            Log.i(TAG_VERBOSE, "register mRightClickReceiver");
            mRightClickReceiver = new PlatformRightClickBroadcastReceiver();
            IntentFilter filter = new IntentFilter(RIGHT_CLICK_BC);
            registerReceiver(mRightClickReceiver,filter);
        }
    }

    private void unregisterBroadcastReceivers(){
        /*
        if(mRxReceiver != null) {
            Log.d(TAG, "unregister mRxReceiver");
            unregisterReceiver(mRxReceiver);
            mRxReceiver = null;
        }

        if(mHDCPReceiver != null) {
            Log.d(TAG, "unregister mHDCPReceiver");
            unregisterReceiver(mHDCPReceiver);
            mHDCPReceiver = null;
        }
        */

        if(mRightClickReceiver != null) {
            Log.i(TAG_VERBOSE, "unregister mRightClickReceiver");
            unregisterReceiver(mRightClickReceiver);
            mRightClickReceiver = null;
        }
    }

    private boolean isAudioOn() {
        if(FloatingWindowService2.bServiceIsAlive){
            return FloatingWindowService2.isAudioOn();
        }
        return true;
    }

    private void stopFloatingWindowService(){
        Log.d(TAG, "call stopFloatingWindowService");
        /*
        if(FloatingWindowService2.bServiceIsAlive){
            Log.d(TAG,"start execute StopServiceTask");
            new StopServiceTask().execute(null,null,null);
        }
        */
        FloatingWindowService2.stopServiceItSelf();
    }

    private void setPlayback(boolean vOn, boolean aOn) {
        if(FloatingWindowService2.bServiceIsAlive) {
            FloatingWindowService2.setPlayback(vOn,aOn);
        }
    }

    private boolean serviceIsRecording() {
        if(FloatingWindowService2.bServiceIsAlive && FloatingWindowService2.isRecording()) {
            return true;
        }else{
            return false;
        }
    }

    private boolean isHDMIAvailable(){
        if(mHDMIInfo==null) return false;
        if(mHDMIInfo.width==0 || mHDMIInfo.height==0 || mHDMIInfo.fps==0)
            return false;
        else
            return true;
    }

    private void getHDMIRxStatus() {
        Log.d(TAG_VERBOSE, "getHDMIRxStatus - start");
        mHDMIInfo = null;
        if(FloatingWindowService2.sHDMIRxStatus != null &&
           FloatingWindowService2.sHDMIRxParameters != null &&
           FloatingWindowService2.bServiceIsAlive){

            Log.d(TAG,"getHDMIInfo from FloatingWindowService2");

            HDMIRxStatus rxStatus = FloatingWindowService2.sHDMIRxStatus;
            HDMIRxParameters hdmirxGetParam = FloatingWindowService2.sHDMIRxParameters;

            mHDMIInfo = new HDMIInfo();
            mHDMIInfo.width = rxStatus.width;
            mHDMIInfo.height = rxStatus.height;
            getSupportedPreviewSize(hdmirxGetParam, rxStatus.width, rxStatus.height);
            mHDMIInfo.fps = getSupportedPreviewFrameRate(hdmirxGetParam);           
        }else{
            Log.d(TAG_VERBOSE, "getHDMIRxStatus - no HDMI info - service status \""+FloatingWindowService2.bServiceIsAlive+"\"");
            if(!FloatingWindowService2.bServiceIsAlive){
                getHDMIStatusByMySelf();
            }
        }

        if(mHDMIInfo != null) {
            mHDMIInfo.buildFilteredList();
            mHDMIInfo.dumpInfo();
        }
    }

    // This is called when no service is working
    private void getHDMIStatusByMySelf() {

        if(DEBUG_VERBOSE){
            Log.d(TAG_VERBOSE, "getHDMIStatusByMySelf - start");
        }

        mHDMIInfo = new HDMIInfo();
        HDMIRxStatus rxStatus = HDMIRxManagerController.getHDMIRxStatus();

        int retryLoop = 5;
        while(rxStatus == null || rxStatus.status != HDMIRxStatus.STATUS_READY) {
            rxStatus = HDMIRxManagerController.getHDMIRxStatus();
            if(DEBUG_VERBOSE){
                Log.d(TAG_VERBOSE, "getHDMIStatusByMySelf - retry polling RxStatus to ready");
            }
            retryLoop=retryLoop-1;

            if(rxStatus == null || rxStatus.status != HDMIRxStatus.STATUS_READY){
                SystemClock.sleep(10);
            }

            if(retryLoop<=0){
                break;
            }
        }

        if(DEBUG_VERBOSE){
            Log.d(TAG_VERBOSE, "getHDMIStatusByMySelf rxStatus:"+rxStatus);
            if(rxStatus != null){
                Log.d(TAG_VERBOSE, "getHDMIStatusByMySelf rxStatus.status:"+rxStatus.status);
            }
        }

        if(rxStatus != null && rxStatus.status == HDMIRxStatus.STATUS_READY) {

            boolean openHDMIRxByMySelf = false;

            if(!HDMIRxManagerController.isOpened()){
                openHDMIRxByMySelf = true;
            }

            if(openHDMIRxByMySelf){
                Log.d(TAG, "getHDMIStatusByMySelf open HDMIRx by MySelf");
                // open HDMIRx
                if(HDMIRxManagerController.open()!=0){
                    HDMIRxManagerController.release();
                    Log.e(TAG, "getHDMIStatusByMySelf open HDMIRx failed");
                    return;
                }
            }

            Log.d(TAG_VERBOSE, "getHDMIStatusByMySelf - collecting HDMIRx Info");
            HDMIRxParameters hdmirxGetParam = HDMIRxManagerController.getParameters();
            mHDMIInfo.width = rxStatus.width;
            mHDMIInfo.height = rxStatus.height;
            getSupportedPreviewSize(hdmirxGetParam, rxStatus.width, rxStatus.height);
            mHDMIInfo.fps = getSupportedPreviewFrameRate(hdmirxGetParam);
            //int scanMode = rxStatus.scanMode;

            if(openHDMIRxByMySelf){
                Log.d(TAG,"getHDMIStatusByMySelf close HDMIRx cause it is opened by me");
                //HDMIRxManagerController.stop();
                HDMIRxManagerController.release();
            }
        }else{
            Log.e(TAG,"getHDMIStatusByMySelf - HDMI status error, clear mHDMIInfo obj");
            mHDMIInfo = null;
        }

        if(DEBUG_VERBOSE){
            Log.d(TAG_VERBOSE, "getHDMIStatusByMySelf - done");
        }
    }

    public int getSupportedPreviewFrameRate(HDMIRxParameters hdmirxGetParam) {
        List<Integer> previewFrameRates = hdmirxGetParam.getSupportedPreviewFrameRates();
        int fps=0;
        if(previewFrameRates!=null && previewFrameRates.size()>0)
            fps = previewFrameRates.get(previewFrameRates.size()-1);
        else
            fps = 30;

        Log.d(TAG, "getSupportedPreviewFrameRate fps:"+fps);
        return fps;
    }

    public void getSupportedPreviewSize(HDMIRxParameters hdmirxGetParam, int rxWidth, int rxHeight) {
        List<Size> previewSizes = hdmirxGetParam.getSupportedPreviewSizes();
        int retWidth=0, retHeight=0;

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "getSupportedPreviewSize - size :"+previewSizes.size());
            for(int i=0;i<previewSizes.size();i++) {
                Log.i(TAG_VERBOSE, "\t size:"+previewSizes.get(i).width+"x"+previewSizes.get(i).height);
            }
        }

        mHDMIInfo.allSizeList = previewSizes;
        if(previewSizes==null || previewSizes.size()<=0)
            return;

        for(int i=0;i<previewSizes.size();i++) {
            if(previewSizes.get(i)!=null && rxWidth==previewSizes.get(i).width) {
                retWidth=previewSizes.get(i).width;
                retHeight=previewSizes.get(i).height;
                if(rxHeight==previewSizes.get(i).height)
                    break;
            }
        }
        if(retWidth==0 && retHeight==0) {
            if(previewSizes.get(previewSizes.size()-1)!=null) {
                retWidth=previewSizes.get(previewSizes.size()-1).width;
                retHeight=previewSizes.get(previewSizes.size()-1).height;
            }
        }
        Log.d(TAG, "getSupportedPreviewSize width:"+retWidth+" height:"+retHeight);
        //mWidth = retWidth;
        //mHeight = retHeight;
    }

    private void initRecGroup() {
        mRecordTitleTextView = (TextView) findViewById(R.id.rec_setting_label);
        mRecordButton = (Button) findViewById(R.id.rec_setting_button);
        mResolutionSpinner = (SpinnerWidget) findViewById(R.id.resolution_spinner);
        mFormatSpinner = null; //(SpinnerWidget) findViewById(R.id.format_spinner);
        mPathTitleTextView = (TextView) findViewById(R.id.path_title_textview);
        mPathTextView = (TextView) findViewById(R.id.path_textview);
        mPathButton = (Button) findViewById(R.id.path_button);

        mRecordTitleTextView.setText("Record setting");

        Bundle arg = new Bundle();
        arg.putCharSequence(SpinnerWidget.KEY_TITLE,"Record format");
        CharSequence[] formats = {"TS", "MP4"};
        arg.putCharSequenceArray(SpinnerWidget.KEY_ITEMS,formats);
        if(mFormatSpinner!=null){
            mFormatSpinner.setParameters(arg);
        }

        arg = new Bundle();
        arg.putCharSequence(SpinnerWidget.KEY_TITLE,"Resolutioan");

        // get from mHDMIInfo, it could be null
        CharSequence[] resolutions;
        if(!isHDMIAvailable()){
            resolutions = new CharSequence[1];
            resolutions[0] = "N/A";
        }else{
            resolutions = mHDMIInfo.getResolutionArray();
            if(resolutions==null){
                Log.e(TAG, "mHDMIInfo report null resolution array");
                resolutions = new CharSequence[1];
                resolutions[0] = "N/A";
            }
        }
        arg.putCharSequenceArray(SpinnerWidget.KEY_ITEMS,resolutions);
        mResolutionSpinner.setParameters(arg);

        mPathTitleTextView.setText("Output path");
        // init default path
        String defaultRecPath = CameraHelper.getOutputMediaFolderPath();
        if(mRecPath==null){
            mPathTextView.setText(defaultRecPath);
        }else{
            mPathTextView.setText(mRecPath);
        }

        mPathButton.setText("..");
        mPathButton.setOnClickListener(this);
        mRecordButton.setOnClickListener(this);
        if(mFormatSpinner==null){
            mRecordButton.setOnKeyListener(this);
            mPathButton.setOnKeyListener(this);
        }
    }

    private void initPreviewGroup() {
        mPIPButton = null;
        //mPIPButton = (Button) findViewById(R.id.pip_button);
        mBroadcastButton = (Button) findViewById(R.id.broadcast_button);
        //mPreviewSubTitleTextView = (TextView) findViewById(R.id.preview_sub_title_textview);
        mNoSignalTextView = (TextView) findViewById(R.id.no_signal_textview);
        mRxBackground = findViewById(R.id.rx_background);
        //mPreviewSettingButton = (ImageButton) findViewById(R.id.preview_setting_button);
        mMuteCheckBox = (CheckBox) findViewById(R.id.mute_checkbox);

        if(mPIPButton != null) {
            mPIPButton.setText("Start PIP");
        }
        mBroadcastButton.setText(BC_BTN_TEXT_EN);
        //mPreviewSubTitleTextView.setText("Preview setting");
        mMuteCheckBox.setText("Mute");
        if(!isHDMIAvailable()){
            mNoSignalTextView.setText("Waiting for HDMI Signal");
        }

        mRxBackground.getViewTreeObserver().addOnGlobalLayoutListener(this);
        mRxBackground.setOnKeyListener(this);
        mRxBackground.setOnClickListener(this);
        mRxBackground.setOnFocusChangeListener(this);

        if(mPIPButton != null){
            mPIPButton.setOnClickListener(this);
        }

        mMuteCheckBox.setOnCheckedChangeListener(this);

        boolean isAudioOn = isAudioOn();
        mMuteCheckBox.setChecked(!isAudioOn);
        //mPreviewSettingButton.setVisibility(View.INVISIBLE);
        mBroadcastButton.setOnClickListener(this);
    }

    private void refreshSchRecList(SchRecItem2 focusItem){

        Log.i(TAG, "call refreshSchRecList");

        mRecListView = (SchRecListView) findViewById(R.id.list_view);
        mRecListView.setListener(this);
        mSchRecItem2List = mDbHelper.getAllItemsByOrder();
        SchRecAdapter adapter = new SchRecAdapter(mContext, mSchRecItem2List);
        mRecListView.setAdapter(adapter);
        mRecListView.setOnItemSelectedListener(mRecListView);
        mRecListView.setOnItemClickListener(mRecListView);
        mRecListView.setOnKeyListener(mRecListView);

        if(focusItem != null){
            mRecListView.requestFocus();
            int position = -1;
            for(int i=0;i<mSchRecItem2List.size();i++) {
                if(mSchRecItem2List.get(i).equals(focusItem)){
                    position = i;
                    break;
                }
            }

            if(position >= 0) {
                mRecListView.smoothScrollToPosition(position);
                mRecListView.setSelection(position);
            }
        }
    }

    private void initSchRecGroup() {

        mAddRecButton = (Button) findViewById(R.id.add_rec_button);
        mAddRecButton.setText("Reservation");
        mAddRecTextView = (TextView) findViewById(R.id.add_rec_label);
        mAddRecTextView.setText("Schedule record");
        refreshSchRecList(null);
        mAddRecButton.setOnClickListener(this);
    }

    private void showToast(String message) {
        Toast.makeText(this,message,Toast.LENGTH_SHORT).show();
    }

    private void validateTimeInfoAndSave2Db(int year, int month, int day,
                int sh, int sm,
                int eh, int em) {

        Log.d(TAG, "validateTimeInfoAndSave"+" year:"+year+
                " month:"+month+" day:"+day+
                " sh:"+sh+" sm:"+sm+
                " eh:"+eh+" em:"+em);

        // construct SchRecItem2

        Calendar start = new GregorianCalendar(year,month,day,sh,sm);
        //start.set(year, month, day, sh, sm);

        //Log.d(TAG,"validateTimeInfoAndSave2Db start sec:"+start.get(Calendar.SECOND));

        Calendar end = new GregorianCalendar(year,month,day,eh,em);
        //end.set(year, month, day, eh, em);

        String path = (String) mPathTextView.getText();
        int format = 0;
        if(mFormatSpinner!=null){
            format = mFormatSpinner.getSelectedPosition();
        }
        Log.d(TAG,"validateTimeInfoAndSave2Db format index:"+format);

        String resolutionString = mResolutionSpinner.getSelectedString();
        Dimension dimen = getResolutionDimension(resolutionString);

        SchRecItem2 item = new SchRecItem2(start,end,
                                path,
                                dimen.width,dimen.height,
                                format);
        // start time check
        if(!item.isBiggerThenCurrentTime()) {
            showToast("ERROR: Schedule start time invalid");
            return;
        }
        // duration check
        if(item.getDurationMin()<=0){
            showToast("ERROR: Record duration error ("+item.getDurationMin()+")");
            return;
        }
        // overlapping check
        SchRecItem2 overlapItem = mDbHelper.getOverlappingItem(item);
        if(overlapItem!=null){
            showToast("ERROR: Schedule time overlapping with \""+overlapItem.getDisplayStringForToast()+"\"");
            return;
        }
        // add to db
        mDbHelper.insert(item);
        // refresh List
        refreshSchRecList(item);


        if(!debugModeEnabled()){
            // show Toast
            showToast(item.getDisplayStringForToast()+" scheduled");
        }
    }

    private int[] str2ipArray(String ipStr){
        StringTokenizer st = new StringTokenizer(ipStr,".");
        String[] tokens = new String[10];
        int size = 0;
        while (st.hasMoreTokens()) {
            tokens[size] = st.nextToken();
            size++;
        }
        for(int i=0;i<size;i++){
            Log.d(TAG, "getIP "+tokens[i]);
        }
        int[] ip = new int[10];

        for(int i=0;i<size;i++){
            ip[i] = Integer.parseInt(tokens[i]);
        }

        return ip;
    }

    private String getRecordInfoString(){

        String format = "TS";
        if(mFormatSpinner!=null){
            format = mFormatSpinner.getSelectedString();
        }

        String rst = "Record settings:\n\n"
                    +"Resolution: "+mResolutionSpinner.getSelectedString()+"\n"
                    +"Format: "+format+"\n"
                    +"Path: "+mPathTextView.getText();
        return rst;
    }

    private void showRecordDialog(){
        mRecordDialog = new Dialog(this);
        mRecordDialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        mRecordDialog.setContentView(R.layout.layout_record_dialog);

        Window w = mRecordDialog.getWindow();
        w.getAttributes().type = WindowManager.LayoutParams.TYPE_SYSTEM_ALERT;
        TextView mainTextView = (TextView) mRecordDialog.findViewById(R.id.main_textview);
        TextView confirmTextView = (TextView) mRecordDialog.findViewById(R.id.ok_textview);

        mainTextView.setText(getRecordInfoString());
        confirmTextView.setText("Press \"OK\" button to start record");

        // support on click event
        View okArea = mRecordDialog.findViewById(R.id.ok_area);
        okArea.setOnClickListener(new View.OnClickListener(){
            public void onClick (View v){
                mRecordDialog.dismiss();
                mRecordDialog = null;
                doStartRecord();
            }
        });

        mRecordDialog.setOnKeyListener( new DialogInterface.OnKeyListener() {
            public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event){
                if(keyCode == KeyEvent.KEYCODE_DPAD_CENTER || keyCode == KeyEvent.KEYCODE_ENTER){
                    if(event.getAction() == KeyEvent.ACTION_UP){
                        mRecordDialog.dismiss();
                        mRecordDialog = null;
                        doStartRecord();
                    }
                    return true;
                }

                if(keyCode == KeyEvent.KEYCODE_BACK) {
                    if(event.getAction() == KeyEvent.ACTION_UP){
                        mRecordDialog.dismiss();
                        mRecordDialog = null;
                    }
                }

                return false;
            }
        });

        mRecordDialog.show();
    }

    private void showBroadcastDialog(){
        mBroadcastDialog = new Dialog(this);
        mBroadcastDialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        mBroadcastDialog.setContentView(R.layout.layout_broadcast_dialog);

        Window w = mBroadcastDialog.getWindow();
        w.getAttributes().type = WindowManager.LayoutParams.TYPE_TOAST;

        TextView ipTextView = (TextView) mBroadcastDialog.findViewById(R.id.ip_textview);
        TextView portTextView = (TextView) mBroadcastDialog.findViewById(R.id.port_textview);
        TextView confirmTextView = (TextView) mBroadcastDialog.findViewById(R.id.ok_textview);

        ipTextView.setText("IP:");
        portTextView.setText("Port:");
        confirmTextView.setText("Press \"OK\" to start broadcast");

        int ip_min_1 = 0;
        int ip_max_1 = 254;

        NumberPicker[] ipPickers = new NumberPicker[4];
        ipPickers[0] = (NumberPicker) mBroadcastDialog.findViewById(R.id.ip_numberpicker1);
        ipPickers[1] = (NumberPicker) mBroadcastDialog.findViewById(R.id.ip_numberpicker2);
        ipPickers[2] = (NumberPicker) mBroadcastDialog.findViewById(R.id.ip_numberpicker3);
        ipPickers[3] = (NumberPicker) mBroadcastDialog.findViewById(R.id.ip_numberpicker4);

        ipPickers[0].setMaxValue(239);
        ipPickers[0].setMinValue(224);

        ipPickers[1].setMaxValue(ip_max_1);
        ipPickers[1].setMinValue(ip_min_1);

        ipPickers[2].setMaxValue(ip_max_1);
        ipPickers[2].setMinValue(ip_min_1);

        ipPickers[3].setMaxValue(254);
        ipPickers[3].setMinValue(0);

        NumberPicker portNumberPicker = (NumberPicker) mBroadcastDialog.findViewById(R.id.port_numberpicker);

        portNumberPicker.setMaxValue(7899);
        portNumberPicker.setMinValue(7890);

        // load previous setting
        String ip = readSetting(SP_KEY_IP);
        String portStr = readSetting(SP_KEY_PORT);

        if(ip==null){
            ip="224.0.0.0";
        }
        if(portStr==null){
            portStr="7890";
        }

        int[] ipArray = str2ipArray(ip);
        int port = Integer.parseInt(portStr);

        for(int i=0;i<4;i++){
            ipPickers[i].setValue(ipArray[i]);
        }
        portNumberPicker.setValue(port);

        // support on click event
        View okArea = mBroadcastDialog.findViewById(R.id.ok_area);
        okArea.setOnClickListener(new View.OnClickListener(){
            public void onClick (View v){
                NumberPicker[] ipPickers = new NumberPicker[4];
                ipPickers[0] = (NumberPicker) mBroadcastDialog.findViewById(R.id.ip_numberpicker1);
                ipPickers[1] = (NumberPicker) mBroadcastDialog.findViewById(R.id.ip_numberpicker2);
                ipPickers[2] = (NumberPicker) mBroadcastDialog.findViewById(R.id.ip_numberpicker3);
                ipPickers[3] = (NumberPicker) mBroadcastDialog.findViewById(R.id.ip_numberpicker4);

                NumberPicker portNumberPicker = (NumberPicker) mBroadcastDialog.findViewById(R.id.port_numberpicker);

                String ip = ""+ipPickers[0].getValue()+"."+ipPickers[1].getValue()+"."+ipPickers[2].getValue()+"."+ipPickers[3].getValue();
                int port = portNumberPicker.getValue();

                saveSetting(SP_KEY_IP,ip);
                saveSetting(SP_KEY_PORT,""+port);

                mBroadcastDialog.dismiss();
                mBroadcastDialog = null;

                //showToast("Start Broadcast IP: "+ip+" Port: "+port);
                processBroadcast(ip,port);
            }
        });

        mBroadcastDialog.setOnKeyListener( new DialogInterface.OnKeyListener() {
            public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event){
                if(keyCode == KeyEvent.KEYCODE_DPAD_CENTER || keyCode == KeyEvent.KEYCODE_ENTER){
                    //Log.d(TAG, "KeyCode:"+keyCode);
                    if(event.getAction() == KeyEvent.ACTION_UP) {

                        NumberPicker[] ipPickers = new NumberPicker[4];
                        ipPickers[0] = (NumberPicker) mBroadcastDialog.findViewById(R.id.ip_numberpicker1);
                        ipPickers[1] = (NumberPicker) mBroadcastDialog.findViewById(R.id.ip_numberpicker2);
                        ipPickers[2] = (NumberPicker) mBroadcastDialog.findViewById(R.id.ip_numberpicker3);
                        ipPickers[3] = (NumberPicker) mBroadcastDialog.findViewById(R.id.ip_numberpicker4);

                        NumberPicker portNumberPicker = (NumberPicker) mBroadcastDialog.findViewById(R.id.port_numberpicker);

                        String ip = ""+ipPickers[0].getValue()+"."+ipPickers[1].getValue()+"."+ipPickers[2].getValue()+"."+ipPickers[3].getValue();
                        int port = portNumberPicker.getValue();

                        saveSetting(SP_KEY_IP,ip);
                        saveSetting(SP_KEY_PORT,""+port);

                        mBroadcastDialog.dismiss();
                        mBroadcastDialog = null;

                        //showToast("Start Broadcast IP: "+ip+" Port: "+port);
                        processBroadcast(ip,port);
                    }
                    return true;
                }

                if(keyCode == KeyEvent.KEYCODE_BACK) {
                    mBroadcastDialog.dismiss();
                    mBroadcastDialog = null;
                }

                return false;
            }
        });

        mBroadcastDialog.show();
    }

    private void updateTimePicker(TimePicker picker, int hour, int minute) {
        int myHour = picker.getHour();
        int myMin = picker.getMinute();

        int myTimeMin = (myHour*60)+myMin;
        int timeMin = (hour*60)+minute;

        if(myTimeMin>timeMin){
            return;
        }else{
            myTimeMin = timeMin+TIMEPICKER_ADJUST_MIN;
            int newHour = myTimeMin/60;
            int newMin = myTimeMin%60;
            if(newHour<24){
                // not overflow..
                picker.setHour(newHour);
                picker.setMinute(newMin);
            }
        }
    }

    private void showDatePickerDialog() {
        mDateDialog = new Dialog(this);
        final Dialog d = mDateDialog; //new Dialog(this);
        d.requestWindowFeature(Window.FEATURE_NO_TITLE);
        d.setContentView(R.layout.layout_sch_rec_dialog);

        // overwrite my dialog attribute (hacking code, but quite cool).
        Window w = d.getWindow();
        w.getAttributes().type = WindowManager.LayoutParams.TYPE_TOAST;

        TimePicker startTimePicker = (TimePicker) d.findViewById(R.id.start_time_picker);
        final TimePicker endTimePicker = (TimePicker) d.findViewById(R.id.end_time_picker);
        DatePicker datePicker = (DatePicker) d.findViewById(R.id.date_picker);

        // support on click event
        View okArea = d.findViewById(R.id.ok_area);
        okArea.setOnClickListener(new View.OnClickListener(){
            public void onClick (View v){
                TimePicker startTimePicker = (TimePicker) d.findViewById(R.id.start_time_picker);
                TimePicker endTimePicker = (TimePicker) d.findViewById(R.id.end_time_picker);
                DatePicker datePicker = (DatePicker) d.findViewById(R.id.date_picker);

                // resolve date info
                int year = datePicker.getYear();
                int month = datePicker.getMonth();
                int day = datePicker.getDayOfMonth();

                int sh = startTimePicker.getHour();
                int sm = startTimePicker.getMinute();
                int eh = endTimePicker.getHour();
                int em = endTimePicker.getMinute();

                d.dismiss();
                mDateDialog = null;
                validateTimeInfoAndSave2Db(year, month, day,
                            sh,sm,
                            eh,em);
            }
        });

        startTimePicker.setOnTimeChangedListener(new TimePicker.OnTimeChangedListener(){
            public void onTimeChanged(TimePicker view, int hourOfDay, int minute){
                Log.d(TAG, "time changed "+hourOfDay+" "+minute);
                updateTimePicker(endTimePicker,hourOfDay,minute);
            }
        });

        d.setOnKeyListener( new DialogInterface.OnKeyListener() {
            public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event){
                if(keyCode == KeyEvent.KEYCODE_DPAD_CENTER || keyCode == KeyEvent.KEYCODE_ENTER){
                    //Log.d(TAG, "KeyCode:"+keyCode);
                    if(event.getAction() == KeyEvent.ACTION_UP) {

                        TimePicker startTimePicker = (TimePicker) d.findViewById(R.id.start_time_picker);
                        TimePicker endTimePicker = (TimePicker) d.findViewById(R.id.end_time_picker);
                        DatePicker datePicker = (DatePicker) d.findViewById(R.id.date_picker);

                        // resolve date info
                        int year = datePicker.getYear();
                        int month = datePicker.getMonth();
                        int day = datePicker.getDayOfMonth();

                        int sh = startTimePicker.getHour();
                        int sm = startTimePicker.getMinute();
                        int eh = endTimePicker.getHour();
                        int em = endTimePicker.getMinute();

                        d.dismiss();
                        mDateDialog = null;
                        validateTimeInfoAndSave2Db(year, month, day,
                                    sh,sm,
                                    eh,em);

                    }
                    return true;
                }

                if(keyCode == KeyEvent.KEYCODE_BACK) {
                    d.dismiss();
                    mDateDialog = null;
                }

                return false;
            }
        });

        startTimePicker.setIs24HourView(true);
        endTimePicker.setIs24HourView(true);

        TextView datePickerTextView = (TextView) d.findViewById(R.id.date_textview);
        datePickerTextView.setText("Date");
        TextView startTimePickerTextView = (TextView) d.findViewById(R.id.start_time_textview);
        startTimePickerTextView.setText("Start time");
        TextView endTimePickerTextView = (TextView) d.findViewById(R.id.end_time_textview);
        endTimePickerTextView.setText("End time");

        TextView infoTextView = (TextView) d.findViewById(R.id.info_textview);
        infoTextView.setText("Press \"OK\" button to confirm");

        d.show();
    }

    @Override
    public void onClick(View v) {
        if(v.equals(mAddRecButton)) {
            showDatePickerDialog();
        }else if(v.equals(mPathButton)) {
            bFinishOnPause = false;
            lunchFileBrowser();
        }else if(v.equals(mPIPButton)) {
            exitAppAndStartPIP();
        }else if(v.equals(mRecordButton)) {
            processRecord();
        }else if(v.equals(mBroadcastButton)){
            processBroadcastButton();
        }else if(v.equals(mRxBackground)){
            //showToast("Click...");
            if(!bPreviewInFullScreen) {
                fullScreenRxPreview();
            }
        }
    }

    private void processBroadcastButton(){

        if(!isHDMIAvailable()){
            showWarningDialog(/*"No HDMI in Signal"*/ WRN_NO_HDMI_IN_SIGNAL);
            return;
        }

        if(!hasNetwork()){
            showWarningDialog("No network available");
            return;
        }

        if(FloatingWindowService2.bServiceIsAlive){
            if(FloatingWindowService2.isRecording()){
                FloatingWindowService2.stopRecord();
            }else{
                showBroadcastDialog();
            }
        }
        updateRecButtonAppearance();
    }

    public void removeSchRecItem2(int position) {
        SchRecItem2 item = mSchRecItem2List.get(position);
        Log.d(TAG, "removeSchRecItem2 - goint to remove item: \""+item.getDisplayString()+"\"");
        // remove item in db
        mDbHelper.deleteItem(item, true);
        refreshSchRecList(null);
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == FILE_PICK) {
            if(resultCode == RESULT_OK){
                String selectPath = data.getStringExtra("SelectPath");
                Log.d(TAG, "result path:"+selectPath);
                mRecPath = selectPath;

                // remove heading text "file://" if exists
                if(mRecPath.startsWith("file://")){
                    mRecPath = mRecPath.substring(7);
                }
                mPathTextView.setText(mRecPath);
                saveSetting(SP_KEY_PATH,mRecPath);
                //getStorageFreeSpace(mRecPath);
            }
        }
    }

    // simple test function, not using it for now
    private void getStorageFreeSpace(String path) {
        StatFs stat = new StatFs(path);
        long freeSizeBytes = stat.getAvailableBytes();
        Log.d(TAG, "free space of "+path+" "+freeSizeBytes/(1024*1024)+" MB");
    }

    private void lunchFileBrowser(){
        Intent intent = new Intent();
        intent.setType("*/*");
        intent.setAction(Intent.ACTION_GET_CONTENT);
        //startActivityForResult(Intent.createChooser(intent, "Title"), FILE_PICK);
        intent.setComponent(new ComponentName("com.rtk.mediabrowser","com.rtk.mediabrowser.MediaBrowser"));
        startActivityForResult(intent, FILE_PICK);
    }

    @Override
    public void onGlobalLayout() {
        Log.d(TAG, "onGlobalLayout");
        mRxBackground.getViewTreeObserver().removeOnGlobalLayoutListener(this);

        DisplayMetrics dm = new DisplayMetrics();
        this.getWindowManager().getDefaultDisplay().getMetrics(dm);
        View globalView = findViewById(android.R.id.content);
        int topOffset = dm.heightPixels - globalView.getMeasuredHeight();

        Log.d(TAG, "onGlobalLayout- topOffset "+topOffset);
        int w = mRxBackground.getWidth();
        int h = mRxBackground.getHeight();
        int[] pos = new int[2];
        mRxBackground.getLocationOnScreen(pos);

        pos[1] = pos[1] - topOffset;
        //mRxBackground.getLocationInWindow(pos);
        Log.d(TAG,"view position, size info "+pos[0]+","+pos[1]+","+w+","+h);

        int offset = 8;

        pos[0] = pos[0]+offset;
        pos[1] = pos[1]+offset;
        w = w - (offset*2);
        h = h - (offset*2);

        if(mRxBackgroundDimension == null) {
            mRxBackgroundDimension = new Dimension(pos[0],pos[1],w,h);
        }

        PIP_X = pos[0];
        PIP_Y = pos[1];
        PIP_W = w;
        PIP_H = h;

        // then try to start or resume floating window service
        startOrResumeFloatingWindowService();
    }

    private void startOrResumeFloatingWindowService() {

        Log.i(TAG, "Call startOrResumeFloatingWindowService service status :"+FloatingWindowService2.bServiceIsAlive);

        if(FloatingWindowService2.bServiceIsAlive) {
            // restore view visibility, display area, become mouse untouchable, enable audio or not
            int[] pipPosition = FloatingWindowService2.getInAppLocation();

            if(pipPosition == null){
                Log.i(TAG, "pip position is null");
                if(mRxBackgroundDimension!=null){
                    FloatingWindowService2.setInAppLocation(mRxBackgroundDimension.toIntArray());
                }
            }else{
                Log.i(TAG, "pipPosition: "+pipPosition[0]+","+pipPosition[1]+","+pipPosition[2]+","+pipPosition[3]);
            }

            // get pip position again
            pipPosition = FloatingWindowService2.getInAppLocation();

            if(pipPosition != null){
                Log.i(TAG, "FloatingWindowService2 inAppLocation: "+pipPosition[0]+","+pipPosition[1]+","+pipPosition[2]+","+pipPosition[3]);
                setFloatingWindowSize(pipPosition,false);
                boolean audioEnable = !mMuteCheckBox.isChecked();
                setPlayback(true,audioEnable);
                // bad workaround... fix me later
                //if(mMuteCheckBox!=null){
                //    mMuteCheckBox.setChecked(!isAudioOn());
                //}
            }
        }else{
            if(mAppState == STATE_RESUMED){
                Log.i(TAG,"[start]OrResumeFloatingWindowServicee - app is resumed. do tasks.");
                startServiceAsAppPreview();
            }else{
                Log.i(TAG, "startOrResumeFloatingWindowService - app is paused. do nothing");
            }
        }
    }

    private void startServiceAsAppPreview(){

        Log.i(TAG, "Call startServiceAsAppPreview");

        if(mRxBackgroundDimension == null) {
            Log.e(TAG, "startServiceAsAppPreview NG preview area not defined");
            return;
        }

        Bundle b = new Bundle();
        b.putBoolean(Keys.KEY_TOUCHABLE,false);
        int[] pos = mRxBackgroundDimension.toIntArray();
        b.putIntArray(Keys.KEY_DIMENSION,pos);
        boolean audioEnable = !mMuteCheckBox.isChecked();
        b.putBoolean(Keys.KEY_AUDIO_ON,audioEnable);
        Intent serviceIntent = new Intent(
                this, com.example.hdmirxdemo.FloatingWindowService2.class);
        serviceIntent.putExtra("rx",b);
        mContext.startService(serviceIntent);
    }

    private Bundle createServiceBundle() {
        Bundle bundle = new Bundle();
        bundle.putIntArray(Keys.KEY_DIMENSION,mRxBackgroundDimension.toIntArray());
        bundle.putBoolean(Keys.KEY_TOUCHABLE,false);
        return bundle;
    }

    private boolean setFloatingWindowSize(int[] position, boolean touchable) {
        Log.i(TAG, "setFloatingWindowSize");
        if(FloatingWindowService2.bServiceIsAlive) {
            FloatingWindowService2.updateWindowLocation(position,touchable);
            return true;
        }
        return false;
    }

    private void exitAppAndStartPIP() {

        if(isHDMIAvailable()){
            mStartPIPOnExit = true;
            finish();
        }else{
            showWarningDialog(/*"No HDMI in Signal"*/ WRN_NO_HDMI_IN_SIGNAL );
        }
    }

    private void fullScreenRxPreview(){
        Dimension dim = new Dimension(0,0,1920,1080);
        bPreviewInFullScreen = setFloatingWindowSize(dim.toIntArray(),false);
        if(FloatingWindowService2.bServiceIsAlive){
            mWindowFocusState = FloatingWindowService2.isFocusShowing();
            //FloatingWindowService2.showFocusOverlay(false);
        }
    }

    public boolean onKey(View v, int keyCode, KeyEvent event) {
        if(v.equals(mRxBackground)) {
            //Log.d(TAG, "onKey :"+keyCode);

            /*
            if(keyCode == KeyEvent.KEYCODE_DPAD_DOWN || keyCode == KeyEvent.KEYCODE_DPAD_UP ||
                keyCode == KeyEvent.KEYCODE_DPAD_RIGHT || keyCode == KeyEvent.KEYCODE_DPAD_LEFT){
                Log.d(TAG, "navigate event happened");
                bCanAcceptOKKey = true;
            }

            if(!bCanAcceptOKKey){
                if(keyCode == KeyEvent.KEYCODE_DPAD_CENTER || keyCode == KeyEvent.KEYCODE_ENTER) {
                    if(event.getAction() == KeyEvent.ACTION_UP){
                        bCanAcceptOKKey = true;
                    }
                    return true;
                }
            }
            */
            bCanAcceptOKKey = true;

            if(keyCode == KeyEvent.KEYCODE_DPAD_CENTER || keyCode == KeyEvent.KEYCODE_ENTER) {
                if(event.getAction() == KeyEvent.ACTION_UP && bCanAcceptOKKey) {

                    fullScreenRxPreview();

                    /*
                    Dimension dim = new Dimension(0,0,1920,1080);
                    bPreviewInFullScreen = setFloatingWindowSize(dim.toIntArray(),false);
                    if(FloatingWindowService2.bServiceIsAlive){
                        mWindowFocusState = FloatingWindowService2.isFocusShowing();
                        //FloatingWindowService2.showFocusOverlay(false);
                    }
                    */

                }
            }
        }

        if(v.equals(mRecordButton)){
            if(keyCode == KeyEvent.KEYCODE_DPAD_DOWN) {
                //Log.d(TAG,"mRecordButton "+keyCode);
                if(event.getAction() == KeyEvent.ACTION_UP){
                    if(mFormatSpinner==null){
                        //Log.d(TAG," mResolutionSpinner requestFocus");
                        mResolutionSpinner.requestFocus();
                    }
                }
                return true;
            }
        }

        if(v.equals(mPathButton)){
            if(keyCode == KeyEvent.KEYCODE_DPAD_UP) {
                //Log.d(TAG,"mPathButton "+keyCode);

                if(event.getAction() == KeyEvent.ACTION_DOWN){
                    bForwardFocus = true;
                }

                if(event.getAction() == KeyEvent.ACTION_UP){
                    if(mFormatSpinner==null && bForwardFocus){
                        //Log.d(TAG," mResolutionSpinner requestFocus");
                        mResolutionSpinner.requestFocus();
                        bForwardFocus = false;
                    }
                }
                return true;
            }
        }

        return false;
    }

    private void pipDisplayRxPreview(){
        setFloatingWindowSize(mRxBackgroundDimension.toIntArray(),false);
        if(FloatingWindowService2.bServiceIsAlive){
            //FloatingWindowService2.showFocusOverlay(mWindowFocusState);
        }
        bPreviewInFullScreen = false;
    }

    private boolean debugModeEnabled(){
        String mode = SystemProperties.get("hdmirx.op.mode","normal");
        if(mode.contains("sqa")){
            return true;
        }else{
            return false;
        }
    }

    private void createDBInBatchMode(){
        // setup startTime
        Calendar calendar = new GregorianCalendar();
        Date trialtime = new Date();
        calendar.setTime(trialtime);

        // we get year, month, day
        int year = calendar.get(Calendar.YEAR);
        int month = calendar.get(Calendar.MONTH);
        int day = calendar.get(Calendar.DATE);
        int hour = 20;
        int minute = 0;

        Calendar base = new GregorianCalendar(year,month,day,hour,0);
        //Calendar end = new GregorianCalendar(year,month,day,hour,1);
        for(int i=0;i<100;i++){
            Calendar start = (Calendar) base.clone();
            int addSize = 10*i;
            start.add(Calendar.MINUTE,addSize);
            Calendar end = (Calendar) start.clone();
            end.add(Calendar.MINUTE,1);
            validateTimeInfoAndSave2Db(
                start.get(Calendar.YEAR),
                start.get(Calendar.MONTH),
                start.get(Calendar.DATE),
                start.get(Calendar.HOUR_OF_DAY),
                start.get(Calendar.MINUTE),
                end.get(Calendar.HOUR_OF_DAY),
                end.get(Calendar.MINUTE));
        }
    }

    public boolean dispatchKeyEvent (KeyEvent event){
        int keycode = event.getKeyCode();
        //Log.d(TAG, "keycode:"+keycode);

        if(keycode == KeyEvent.KEYCODE_DPAD_CENTER && mDpadCenterConsumed == false) {
            mDpadCenterConsumed = true;
            Log.d(TAG, "consume first DPAD_CENTER event");
            return true;
        }

        if(keycode == KeyEvent.KEYCODE_BACK) {

            boolean consume = false;
            if(FloatingWindowService2.bServiceIsAlive && bPreviewInFullScreen){
                consume = true;
            }

            int action = event.getAction();
            if(action==KeyEvent.ACTION_UP && consume) {
                //Log.d(TAG,"dispatchKeyEvent keycode:"+keycode);
                /*
                setFloatingWindowSize(mRxBackgroundDimension.toIntArray(),false);
                if(FloatingWindowService2.bServiceIsAlive){
                    //FloatingWindowService2.showFocusOverlay(mWindowFocusState);
                }
                bPreviewInFullScreen = false;
                */
                pipDisplayRxPreview();
            }
            if(consume)
                return consume;
        }

        if(Keys.ENABLE_BATCH_DB_CREATE && debugModeEnabled()){
            if(keycode == KeyEvent.KEYCODE_MENU) {
                int action = event.getAction();
                if(action == KeyEvent.ACTION_UP){
                    createDBInBatchMode();
                    showToast("Create DB complete, exit app, restart it by your self");
                    finish();
                }
            }
        }

        return super.dispatchKeyEvent(event);
    }

    @Override
    public void onCheckedChanged(CompoundButton buttonView,boolean isChecked) {
        if(buttonView.equals(mMuteCheckBox)){
            setPlayback(true,!isChecked);
        }
    }

    private void dismissAllDialogs() {
        if(mDateDialog!=null){
            mDateDialog.dismiss();
            mDateDialog = null;
        }

        if(mBroadcastDialog!=null){
            mBroadcastDialog.dismiss();
            mBroadcastDialog = null;
        }

        if(mRecordDialog!=null){
            mRecordDialog.dismiss();
            mRecordDialog = null;
        }

        dismissWarningDialog();
    }

    public void handleBroadcast(Intent intent){
        String action = intent.getAction();
        Log.d(TAG, "handleBroadcast "+action);
        if(action.equals(MyReceiver.RX_STATUS_STRING)){
            if ((intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_PLUGGED_STATE, false))){
                Log.d(TAG, "hdmirx plug in again");

                getHDMIRxStatus();
                refreshResolutionSpinner();
                dismissWarningDialog();
                mNoSignalTextView.setText("");
                dismissAllDialogs();

                if(mAppState == STATE_PAUSED) {
                    Log.i(TAG,"app is in paused state. break");
                    return;
                }else{
                    Log.i(TAG,"app is resumed, continue.");
                }

                startOrResumeFloatingWindowService();
            }else{
                // clear HDMIInfo
                Log.d(TAG,"hdmirx plug out");
                mHDMIInfo = null;
                refreshResolutionSpinner();
                mNoSignalTextView.setText("Waiting for HDMI Signal");
                stopFloatingWindowService();
                dismissAllDialogs();

                if(mAppState == STATE_RESUMED){
                    showWarningDialog("HDMI is plugged out");
                }
            }
        }
    }

    private void refreshResolutionSpinner() {
        if(mResolutionSpinner != null) {
            Bundle arg = new Bundle();
            arg.putCharSequence(SpinnerWidget.KEY_TITLE,"Resolutioan");

            // get from mHDMIInfo, it could be null
            CharSequence[] resolutions;
            if(mHDMIInfo == null){
                resolutions = new CharSequence[1];
                resolutions[0] = "N/A";
            }else{
                resolutions = mHDMIInfo.getResolutionArray();
            }
            arg.putCharSequenceArray(SpinnerWidget.KEY_ITEMS,resolutions);
            mResolutionSpinner.setParameters(arg);
        }
    }

    private Dimension getResolutionDimension(String resolutionString){

        if(resolutionString.equals("N/A")){
            return new Dimension(0,0,0,0);
        }

        StringTokenizer st = new StringTokenizer(resolutionString,"x");
        String[] tokens = new String[10];
        int size = 0;
        while (st.hasMoreTokens()) {
            tokens[size] = st.nextToken();
            size++;
        }
        for(int i=0;i<size;i++){
            Log.d(TAG, "getResolutionDimension "+tokens[i]);
        }

        int width = Integer.parseInt(tokens[0]);
        int height = Integer.parseInt(tokens[1]);

        return new Dimension(0,0,width,height);
    }

    private void updateRecButtonAppearance(/*boolean canRecord*/){
        Resources resource=(Resources)getResources();
        ColorStateList cslGreen = (ColorStateList)resource.getColorStateList(R.drawable.btn_text_color_selector);
        ColorStateList cslRed = (ColorStateList)resource.getColorStateList(R.drawable.btn_text_color_selector_red);

        boolean isRecording = false;
        int mode = Keys.OUTPUT_OFF;
        if(FloatingWindowService2.bServiceIsAlive){
            isRecording = FloatingWindowService2.isRecording();
        }
        if(isRecording){
            mode = FloatingWindowService2.getOutputMode();
        }

        if(!isRecording){
            mRecordButton.setEnabled(true);
            mRecordButton.setFocusable(true);

            mBroadcastButton.setEnabled(true);
            mBroadcastButton.setFocusable(true);

            mRecordButton.setText(REC_BTN_TEXT_RECORD);
            mRecordButton.setTextColor(cslGreen);
            //mRecordButton.setCompoundDrawables(recDrawableNormal,null,null,null);
            mRecordButton.setCompoundDrawablesRelativeWithIntrinsicBounds(R.drawable.btn_record,0,0,0);

            mBroadcastButton.setText(BC_BTN_TEXT_EN);
            mBroadcastButton.setTextColor(cslGreen);
            mBroadcastButton.setCompoundDrawablesRelativeWithIntrinsicBounds(R.drawable.btn_broadcast,0,0,0);
        }else{
            if(mode==Keys.OUTPUT_UDP){
                mBroadcastButton.setText(BC_BTN_TEXT_UN);
                mBroadcastButton.setTextColor(cslRed);
                mBroadcastButton.setEnabled(true);
                mBroadcastButton.setFocusable(true);
                mBroadcastButton.setCompoundDrawablesRelativeWithIntrinsicBounds(R.drawable.btn_broadcast_red,0,0,0);

                mRecordButton.setText(REC_BTN_TEXT_RECORD);
                mRecordButton.setTextColor(cslGreen);
                //mRecordButton.setCompoundDrawables(recDrawableNormal,null,null,null);
                mRecordButton.setCompoundDrawablesRelativeWithIntrinsicBounds(R.drawable.btn_record,0,0,0);

                mRecordButton.setEnabled(false);
                mRecordButton.setFocusable(false);

            }else if(mode == Keys.OUTPUT_STORAGE){
                mRecordButton.setEnabled(true);
                mRecordButton.setFocusable(true);
                mRecordButton.setText(REC_BTN_TEXT_STOP_RECORD);
                //mRecordButton.setCompoundDrawables(recDrawableRed,null,null,null);
                mRecordButton.setTextColor(cslRed);
                mRecordButton.setCompoundDrawablesRelativeWithIntrinsicBounds(R.drawable.btn_record_red,0,0,0);

                mBroadcastButton.setText(BC_BTN_TEXT_EN);
                mBroadcastButton.setTextColor(cslGreen);
                mBroadcastButton.setEnabled(false);
                mBroadcastButton.setFocusable(false);
                mBroadcastButton.setCompoundDrawablesRelativeWithIntrinsicBounds(R.drawable.btn_broadcast,0,0,0);
            }else{
                Log.e(TAG,"updateRecButtonAppearance recording but output mode is null, should not reach here");
            }
        }
    }

    private void processBroadcast(String ip, int port){
        if(FloatingWindowService2.bServiceIsAlive){
            // check if is recording
            if(FloatingWindowService2.isRecording()){
                Log.e(TAG, "processBroadcast - record running, should not reach here");
                //showToast("ERROR: found running record instance");
                FloatingWindowService2.stopRecord();
                return;
            }else{
                Bundle arg = new Bundle();
                arg.putInt(Keys.KEY_OUTPUT_MODE,Keys.OUTPUT_UDP);
                arg.putString(Keys.KEY_OUTPUT_PATH,ip);
                arg.putInt(Keys.KEY_OUTPUT_FORMAT,Keys.FORMAT_TS);
                arg.putInt(Keys.KEY_PORT,port);
                FloatingWindowService2.startRecord(arg);
            }
        }
        updateRecButtonAppearance();
    }

    private void doStartRecord(){
        Bundle arg = new Bundle();
        arg.putInt(Keys.KEY_OUTPUT_MODE,Keys.OUTPUT_STORAGE);
        String path = (String) mPathTextView.getText();
        arg.putString(Keys.KEY_OUTPUT_PATH,path);
        if(mFormatSpinner!=null){
            arg.putInt(Keys.KEY_OUTPUT_FORMAT,mFormatSpinner.getSelectedPosition());
        }

        String resolutionString = mResolutionSpinner.getSelectedString();
        Dimension dimen = getResolutionDimension(resolutionString);
        arg.putInt(Keys.KEY_RECORD_WIDTH,dimen.width);
        arg.putInt(Keys.KEY_RECORD_HEIGHT,dimen.height);
        FloatingWindowService2.startRecord(arg);
        //showToast("start record");
        updateRecButtonAppearance();
    }

    private void processRecord(){
        Log.d(TAG,"processRecord");

        if(!isHDMIAvailable()){
            showWarningDialog(/*"No HDMI in Signal"*/ WRN_NO_HDMI_IN_SIGNAL );
            return;
        }

        if(FloatingWindowService2.bServiceIsAlive) {
            // check if is recording
            if(FloatingWindowService2.isRecording()){
                //showToast("ERROR: found running record instance");
                FloatingWindowService2.stopRecord();
            }else{
                showRecordDialog();
            }
        }
        updateRecButtonAppearance();
    }

    public void refreshRecordwidgets(){
        updateRecButtonAppearance();
    }

    private boolean hasNetwork(){
        ConnectivityManager cm =
                (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo activeNetwork = cm.getActiveNetworkInfo();
        boolean isConnected = activeNetwork != null &&
                activeNetwork.isConnectedOrConnecting();

        return isConnected;
    }

    public void onFocusChange (View v, boolean hasFocus){
        if(v.equals(mRxBackground)){
            //Log.d(TAG,"focus changed "+hasFocus);
            if(FloatingWindowService2.bServiceIsAlive){
                //FloatingWindowService2.showFocusOverlay(hasFocus);
            }
        }
    }

    private void dismissWarningDialog(){
        if(mWarningDialog!=null){
            mWarningDialog.dismiss();
            mWarningDialog = null;
        }
    }

    private void showWarningDialog(String message){
        dismissWarningDialog();
        mWarningDialog = new Dialog(this);
        mWarningDialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        mWarningDialog.setContentView(R.layout.layout_warning_dialog);
        Window w = mWarningDialog.getWindow();
        w.getAttributes().type = WindowManager.LayoutParams.TYPE_TOAST;

        TextView warningTextView = (TextView) mWarningDialog.findViewById(R.id.warning_textview);
        TextView confirmTextView = (TextView) mWarningDialog.findViewById(R.id.ok_textview);

        confirmTextView.setText("Press \"OK\" button to close dialog");
        warningTextView.setText(message);

        // support on click event
        View okArea = mWarningDialog.findViewById(R.id.ok_area);
        okArea.setOnClickListener(new View.OnClickListener(){
            public void onClick (View v){
                dismissWarningDialog();
            }
        });

        mWarningDialog.setOnKeyListener( new DialogInterface.OnKeyListener() {
            public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event){
                if(keyCode == KeyEvent.KEYCODE_DPAD_CENTER || keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_BACK){
                    if(event.getAction() == KeyEvent.ACTION_UP){
                        mWarningDialog.dismiss();
                        mWarningDialog = null;
                    }
                }
                return true;
            }
        });

        mWarningDialog.show();
    }

    private void loadAppStatus(){
        String mute = readSetting(SP_KEY_MUTE);
        if(mute==null) mute = "false";
        if(mute.equals("true"))
            mMuteCheckBox.setChecked(true);
        else
            mMuteCheckBox.setChecked(false);

        String resolution = readSetting(SP_KEY_RESOLUTION);
        if(resolution != null){
            mResolutionSpinner.setSelection(resolution);
        }

        String format = readSetting(SP_KEY_FORMAT);
        
        if(format != null && mFormatSpinner != null){
            mFormatSpinner.setSelection(format);
        }

        String path = readSetting(SP_KEY_PATH);
        if(path != null){
            mPathTextView.setText(path);
        }
    }

    private void saveAppStatus(){
        // mute:
        String mute = mMuteCheckBox.isChecked()?"true":"false";
        saveSetting(SP_KEY_MUTE,mute);

        Log.d(TAG,"save mute:"+mute);

        // resolution
        String resolution = mResolutionSpinner.getSelectedString();
        saveSetting(SP_KEY_RESOLUTION,resolution);

        // format
        String format = "TS";
        if(mFormatSpinner != null){
            format = mFormatSpinner.getSelectedString();
        }
        saveSetting(SP_KEY_FORMAT,format);

        // path
        String path = (String) mPathTextView.getText();
        Log.d(TAG,"save path:"+path);
        saveSetting(SP_KEY_PATH, path);
    }

    private SharedPreferences getSharedPrefs(){
        if(mSharedPrefs==null){
            mSharedPrefs = getSharedPreferences("setting",Context.MODE_PRIVATE);
        }
        return mSharedPrefs;
    }

    private void saveSetting(String key, String value){
        getSharedPrefs().edit().putString(key,value).commit();
    }

    private String readSetting(String key){
        return getSharedPrefs().getString(key,null);
    }

    public void onDbUIUpdate(){
        Log.d(TAG, "onDbUIUpdate");
        refreshSchRecList(null);
    }

    public boolean needAlive(){
        return true;
    }

}
