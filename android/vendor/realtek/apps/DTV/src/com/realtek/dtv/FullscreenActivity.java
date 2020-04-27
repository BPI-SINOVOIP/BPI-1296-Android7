package com.realtek.dtv;

import java.util.ArrayList;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.SharedPreferences;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.MediaPlayer.OnVideoSizeChangedListener;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.realtek.dtv.util.SystemUiHider;
import com.realtek.app.TvManager;
import com.realtek.app.tv.*;

// keep below 2 import statements. If Ctrl+Shist+O automatically removes them, I can copy&paste them.
//import com.realtek.app.TvManager;
//import com.realtek.app.tv.*;

public class FullscreenActivity extends Activity implements ManualScanSetting.ManualSettingListener,
        RegionSettingDialog.RegionSettingDialogListener {
    /**
     * Whether or not the system UI should be auto-hidden after
     * {@link #AUTO_HIDE_DELAY_MILLIS} milliseconds.
     */
    private static final boolean AUTO_HIDE = true;

    /**
     * If {@link #AUTO_HIDE} is set, the number of milliseconds to wait after
     * user interaction before hiding the system UI.
     */
    private static final int AUTO_HIDE_DELAY_MILLIS = 4000;

    /**
     * If set, will toggle the system UI visibility upon interaction. Otherwise,
     * will show the system UI visibility upon interaction.
     */
    private static final boolean TOGGLE_ON_CLICK = true;

    /**
     * The flags to pass to {@link SystemUiHider#getInstance}.
     */
    private static final int HIDER_FLAGS = SystemUiHider.FLAG_HIDE_NAVIGATION;

    /**
     * The instance of the {@link SystemUiHider} for this activity.
     */
    private SystemUiHider mSystemUiHider;

    // added for DTV -- start
    final String TAG                 = "DTV";
    final String PREFS_NAME          = "DTV_PREFS";
    final String PREFS_REGION        = "PREFS_REGION";
    final String PREFS_SOURCE        = "PREFS_SOURCE";
    final String PREFS_TV_CHANNEL    = "PREFS_TV_CHANNEL";
    final String PREFS_RADIO_CHANNEL = "PREFS_RADIO_CHANNEL";
    final String FORBIDDEN_STRING    = "XXX";
    final String DTV_PATH            = "stb://0";
    
    // message id
    static final int STARTING_AUTO_SCAN   = 0;
    static final int STARTING_MANUAL_SCAN = 1;
    static final int AUTO_SCAN_STARTED    = 2;
    static final int MANUAL_SCAN_STARTED  = 3;
    static final int SCAN_COMPLETE        = 4;
    static final int STOP_SCAN            = 5;
    static final int UPDATE_SCAN_INFO     = 6;
    static final int CHECK_INPUT          = 7;
    
    static final int SCAN_INTERVAL        = 1000;
    static final int CHECK_INPUT_TIMER    = 1500;
    
    // scan type
    static final int AUTO_SCAN   = 0;
    static final int MANUAL_SCAN = 1;
    
    static final int MAX_CHANNEL_DIGIT = 3;
    
    static final int DEFAULT_FILTER = TvManager.CHANNEL_FLAGS_IS_DELETED|TvManager.CHANNEL_FLAGS_IS_HIDDEN;

    TvManager m_tvManager;
    private boolean isServerConnected = false;
    private boolean isManualScanning  = false;
    private boolean isAutoScanning    = false;
    private int tuner0Status = TvManager.DTV_TUNER_STATUS_IDLE;
    private int tuner1Status = TvManager.DTV_TUNER_STATUS_IDLE;
    private int tunerStatus[] = new int[2];
    Dialog manualScanDlg;
    ScanProgressDialog scanProgressDialog;
    ListView regionListView;
    Spinner bandWidthSpinner;
    Button cancelButton;
    Button manualScanBtn;
    Button cancelManualScanBtn;
    EditText frequencyText;
    ProgressBar scanProgress;
    int region = -1;
    int tunerId = 0, frequency = 0, bandwidth = 0;
    public int scanPercentage = 0;
    private int scanningTunerId = 0;
    
    UIHandler uiHandler;
    TextView radioBackground;
    TextView channelNumberTV; // textview for displaying channel number
    View contentView;
    SurfaceHolder contentViewHolder;
    MediaPlayer mp = null;
    Toast channelToast;
    int inputDigit = 0;
    int channelNumber = 0;
    String channelNumberString = "";
    int channelCount = 0, channelQuality = 0, channelStrength = 0;
    int tvChannel = -1, radioChannel = -1, prevChannel = -1, currentChannel = -1;
    int serviceType = 0;
    int filterOutFlags = DEFAULT_FILTER;
    CheckChannelRunnable checkChannelRunnable = new CheckChannelRunnable();
    SharedPreferences settings;
    ArrayList<String> tvChList = new ArrayList<String>();
    ArrayList<String> radioChList = new ArrayList<String>();
    ArrayAdapter<String> tvChAdapter;
    ArrayAdapter<String> radioChAdapter;
    View channelNumberLayout;
    
    int videoWidth, videoHeight;
    boolean isVideoReady, isVideoSizeKnown;
    // added for DTV -- end

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_fullscreen);

        final View controlsView = findViewById(R.id.fullscreen_content_controls);
        contentView = findViewById(R.id.surfaceview);
        if (contentViewHolder == null) {
            contentViewHolder = ((SurfaceView)contentView).getHolder();
            contentViewHolder.addCallback(new SurfaceCallback());
        } 
        // Set up an instance of SystemUiHider to control the system UI for
        // this activity.
        mSystemUiHider = SystemUiHider.getInstance(this, contentView,
                HIDER_FLAGS);
        mSystemUiHider.setup();
        mSystemUiHider
                .setOnVisibilityChangeListener(new SystemUiHider.OnVisibilityChangeListener() {
                    // Cached values.
                    int mControlsHeight;
                    int mShortAnimTime;

                    @Override
                    @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
                    public void onVisibilityChange(boolean visible) {
                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB_MR2) {
                            // If the ViewPropertyAnimator API is available
                            // (Honeycomb MR2 and later), use it to animate the
                            // in-layout UI controls at the bottom of the
                            // screen.
                            if (mControlsHeight == 0) {
                                mControlsHeight = controlsView.getHeight();
                            }
                            if (mShortAnimTime == 0) {
                                mShortAnimTime = getResources().getInteger(
                                        android.R.integer.config_shortAnimTime);
                            }
                            controlsView
                                    .animate()
                                    .translationY(visible ? 0 : mControlsHeight)
                                    .setDuration(mShortAnimTime);
                        } else {
                            // If the ViewPropertyAnimator APIs aren't
                            // available, simply show or hide the in-layout UI
                            // controls.
                            controlsView.setVisibility(visible ? View.VISIBLE
                                    : View.GONE);
                        }

                        if (visible && AUTO_HIDE) {
                            // Schedule a hide().
                            delayedHide(AUTO_HIDE_DELAY_MILLIS);
                        }
                    }
                });

        // Set up the user interaction to manually show or hide the system UI.
        contentView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.d(TAG, "contentView.onClick");
                if (TOGGLE_ON_CLICK) {
                    mSystemUiHider.toggle();
                } else {
                    mSystemUiHider.show();
                }
            }
        });

        // Upon interacting with UI controls, delay any scheduled hide()
        // operations to prevent the jarring behavior of controls going away
        // while interacting with the UI.
        findViewById(R.id.dummy_button).setOnTouchListener(
                mDelayHideTouchListener);

        radioBackground = (TextView)findViewById(R.id.radio_background);
        channelNumberTV = (TextView)findViewById(R.id.channel_number);
        channelNumberLayout = (View)findViewById(R.id.channel_number_layout);
        
        //initializeDTV();
    }

    @Override
    protected void onResume() {
        super.onResume();
        //initializeDTV();
    }

    private void displayChannelNumber(String str) {
        if (str == null) {
            str = "" + currentChannel;
        }
        channelNumberTV.setText(str);
        
        if (channelNumberLayout.getVisibility() == View.INVISIBLE) {
            channelNumberLayout.setVisibility(View.VISIBLE);
        }
        
        if (false) { // always display
        } else { // start timer
            int delayMillis = AUTO_HIDE_DELAY_MILLIS;
            if (str.equals(FORBIDDEN_STRING) ) {
                delayMillis = delayMillis / 2;
            }
            delayedHideChannelNumber(delayMillis);
        }
    }

    class SurfaceCallback implements SurfaceHolder.Callback {
        @Override
        public void surfaceCreated(SurfaceHolder holder) {
            initMediaPlayer();
        }
        
        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            Log.d(TAG, "surfaceChanged w:" + width + " h:" + height);
        }
        
        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {
            releaseMediaPlayer();
            doCleanUp();
        }
    }
    private void doCleanUp() {
        videoWidth = 0;
        videoHeight = 0;
        isVideoReady = false;
        isVideoSizeKnown = false;
    }
    
    private void playVideo() {
        Log.d(TAG, "playVideo videoWidth:" + videoWidth + " videoHeight:" + videoHeight);
        contentViewHolder.setFixedSize(videoWidth, videoHeight);
        mp.start();
    }
    
    private void releaseMediaPlayer() {
        if (mp != null) {
            mp.stop();
            mp.release();
            mp = null;
        }
    }
    private void initMediaPlayer() {
        doCleanUp();
        
        if (mp == null) {
            mp = new MediaPlayer();
        }
        try {
            mp = new MediaPlayer();
            mp.setDataSource(DTV_PATH);
            mp.setDisplay(contentViewHolder);
            //mp.setAudioStreamType(AudioManager.STREAM_MUSIC);
            mp.prepare();
            
            mp.setOnPreparedListener(new OnPreparedListener() {
                @Override
                public void onPrepared(MediaPlayer mp) {
                    isVideoReady = true;
                    if (isVideoSizeKnown) {
                        playVideo();
                        initializeDTV();
                    }
                }
            });
            mp.setOnVideoSizeChangedListener(new OnVideoSizeChangedListener() {
                @Override
                public void onVideoSizeChanged(MediaPlayer mp, int width, int height) {
                    if (width == 0 || height == 0) {
                        Log.d(TAG, "onVideoSizeChanged() Invalid width:" + width + " height:" + height);
                        return;
                    } else {
                        Log.d(TAG, "onVideoSizeChanged() width:" + width + " height:" + height);
                    }
                    
                    isVideoSizeKnown = true;
                    videoWidth = width;
                    videoHeight = height;
                    if (isVideoReady) {
                        playVideo();
                    }
                }
            });
        } catch (Exception e) {
            Log.e(TAG, e.toString());
        }
    }

    private void initializeDTV() {
        boolean result = false;

        if (m_tvManager == null) {
            m_tvManager = new TvManager();
        }

        isServerConnected = m_tvManager.ConnectToServer();
        if (isServerConnected) {
            Log.d(TAG, "ConnectToServer() OK");
            //Toast.makeText(getApplicationContext(), "ConnectToServer() OK", Toast.LENGTH_SHORT).show();
        }
        else {
            Log.d(TAG, "ConnectToServer() NG");
            Toast.makeText(getApplicationContext(), "ConnectToServer() NG", Toast.LENGTH_SHORT).show();
            return;
        }

        //m_tvManager.SetBroadcastIntentAction("com.example.testtvmanager.action.DtvEvent");
        getPreference();
        tunerStatus[0] = tunerStatus[1] = TvManager.DTV_TUNER_STATUS_IDLE;
        tunerId = acquireTunerForPreview();
        Log.d(TAG, "tunerId:" + tunerId);

        m_tvManager.SetTvRegion(region);

        if (serviceType == DvbChannelInfo.SERVICE_TYPE_DTV) {
            filterOutFlags = filterOutFlags & ~TvManager.CHANNEL_FLAG_IS_DTV | TvManager.CHANNEL_FLAG_IS_RADIO;
        } else if (serviceType == DvbChannelInfo.SERVICE_TYPE_RADIO) {
            filterOutFlags = filterOutFlags & ~TvManager.CHANNEL_FLAG_IS_RADIO | TvManager.CHANNEL_FLAG_IS_DTV;
        }
        //channelCount = 0; // for debugging
        channelCount = m_tvManager.GetChannelCount(filterOutFlags); // need to check different service??
        
        Log.d(TAG, "initializeDTV() channelCount:" + channelCount);
        if (channelCount > 0) { // scan has been performed
            currentChannel = prevChannel; // return -1 if there is no previous selected channel
            
            if (prevChannel == -1) { // no prev channel, play the first one
                DvbChannelInfo dvbChannelInfo = m_tvManager.GetFirstChannel(filterOutFlags);
                if (null == dvbChannelInfo) {
                    Log.e(TAG, "GetFirstChannel() NG.");
                    return;
                }
                if (m_tvManager.PlayChannel(tunerId, dvbChannelInfo.logicalChNum, true, true) == false) {
                    //Toast.makeText(getApplicationContext(), "Preview DVB NG", Toast.LENGTH_SHORT).show();
                    Log.e(TAG, "TvManualScanStart(0) NG.");
                    return;
                } else {
                    currentChannel = dvbChannelInfo.logicalChNum;
                }
            } else {
                if (m_tvManager.PlayChannel(tunerId, prevChannel, true, true) == false) {
                    return;
                } else {
                    currentChannel = prevChannel;
                }
            }
            displayChannelNumber(null);
        }

        uiHandler = new UIHandler(Looper.getMainLooper());
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        boolean isConsumed = false;
        int num = 0;
        Log.d(TAG, "keycode:" + keyCode + " -- event:" + event);

        switch (keyCode) {
            case KeyEvent.KEYCODE_0:
            case KeyEvent.KEYCODE_1:
            case KeyEvent.KEYCODE_2:
            case KeyEvent.KEYCODE_3:
            case KeyEvent.KEYCODE_4:
            case KeyEvent.KEYCODE_5:
            case KeyEvent.KEYCODE_6:
            case KeyEvent.KEYCODE_7:
            case KeyEvent.KEYCODE_8:
            case KeyEvent.KEYCODE_9:
                num = keyCode - KeyEvent.KEYCODE_0;
                handleInputNumber(num);
                isConsumed = true;
                break;
            case KeyEvent.KEYCODE_NUMPAD_0:
            case KeyEvent.KEYCODE_NUMPAD_1:
            case KeyEvent.KEYCODE_NUMPAD_2:
            case KeyEvent.KEYCODE_NUMPAD_3:
            case KeyEvent.KEYCODE_NUMPAD_4:
            case KeyEvent.KEYCODE_NUMPAD_5:
            case KeyEvent.KEYCODE_NUMPAD_6:
            case KeyEvent.KEYCODE_NUMPAD_7:
            case KeyEvent.KEYCODE_NUMPAD_8:
            case KeyEvent.KEYCODE_NUMPAD_9:
                num = keyCode - KeyEvent.KEYCODE_NUMPAD_0;
                handleInputNumber(num);
                isConsumed = true;
                break;
            case KeyEvent.KEYCODE_DPAD_UP: // up arrow
                /**
                    before play channel, consider calling GetNextChannel() and check if it gets null value.
                    but if it is not null, which api should be used? PlayNextChannel or PlayChannel?
                    PlayChannel needs chIdx (LCN or ??), which requires extra step to get chIdx. 
                */
                
                if (m_tvManager.PlayNextChannel(tunerId, true, true, filterOutFlags) == true) {
                    DvbChannelInfo channelInfo = m_tvManager.GetCurChannel(tunerId);

                    if (channelInfo != null) { // save current channel
                        prevChannel = currentChannel;
                        currentChannel = channelInfo.logicalChNum;
                        updateChannelByService();
                        displayChannelNumber(null);
                    } else {
                        Log.e(TAG, "GetCurChannel() returns null");
                    }
                } else {
                    Log.e(TAG, "PlayNextChannel() returns null");
                }
                isConsumed = true;
                break;
            case KeyEvent.KEYCODE_DPAD_DOWN: // down arrow
                if (m_tvManager.PlayPrevChannel(tunerId, true, true, filterOutFlags) == true) {
                    DvbChannelInfo channelInfo = m_tvManager.GetCurChannel(tunerId);
    
                    if (channelInfo != null) { // save current channel
                        prevChannel = currentChannel;
                        currentChannel = channelInfo.logicalChNum;
                        updateChannelByService();
                        displayChannelNumber(null);
                    } else {
                        Log.e(TAG, "GetCurChannel() returns null");
                    }
                } else {
                    Log.e(TAG, "PlayPrevChannel() returns null");
                }
                isConsumed = true;
                break;
            case KeyEvent.KEYCODE_LEFT_BRACKET:
                switchService();
                isConsumed = true;
                break;
            case KeyEvent.KEYCODE_MENU:
                Log.d(TAG, "KEYCODE_MENU");
                if (TOGGLE_ON_CLICK) {
                    mSystemUiHider.toggle();
                } else {
                    mSystemUiHider.show();
                }
                break;
            default:
                if (event.getScanCode() == 377) { // source switch key on remote control
                    switchService();
                    isConsumed = true;
                }
                break;
        }

        if (!isConsumed) {
            return super.onKeyDown(keyCode, event);
        }
        
        return true;
    }

    @Override
    public void onPause() {
        super.onPause();
        setPreference();
        for (int i=0;i<2;i++) {
            if (tunerStatus[i] == TvManager.DTV_TUNER_STATUS_PREVIEW) {
                if (m_tvManager.UnInit(i)) {
                    Log.d(TAG, "UnInit() Ok.");
                } else {
                    Log.e(TAG, "UnInit() NG.");
                }
                tunerStatus[i] = TvManager.DTV_TUNER_STATUS_IDLE;
            }
        }
        m_tvManager.SetBroadcastIntentAction(null);
        m_tvManager = null;
        isServerConnected = false;
        //Toast.makeText(getApplicationContext(), "UnInit DTV & Delete TvManager OK", Toast.LENGTH_SHORT).show();
    }
/*
    @Override
    protected void onDestroy() {
        super.onDestroy();
        setPreference();
        for (int i=0;i<2;i++) {
            if (tunerStatus[i] == TvManager.DTV_TUNER_STATUS_PREVIEW) {
                if (m_tvManager.UnInit(i) == false) {
                    Log.e(TAG, "UnInit() NG.");
                }
                tunerStatus[i] = TvManager.DTV_TUNER_STATUS_IDLE;
            }
        }
        m_tvManager.SetBroadcastIntentAction(null);
        m_tvManager = null;
        isServerConnected = false;
        Toast.makeText(getApplicationContext(), "UnInit DTV & Delete TvManager OK", Toast.LENGTH_SHORT).show();
    }
  */  

    private int acquireTunerForPreview() {
        int i = 0;
        for (i=0; i<2; i++) {
            if (tunerStatus[i] == TvManager.DTV_TUNER_STATUS_PREVIEW) {
                return i;
            }
        }
        // if goes here, it means no tuner has been initialized before. So run Init() here
        for (i=0; i<2; i++) {
            if (tunerStatus[i] == TvManager.DTV_TUNER_STATUS_IDLE) {
                Log.d(TAG, "acquireTunerForPreview()");
                if (m_tvManager.Init(i) == true) {
                    tunerStatus[i] = TvManager.DTV_TUNER_STATUS_PREVIEW;
                    return i;
                }
            }
        }
        return (-1);
    }

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);

        // Trigger the initial hide() shortly after the activity has been
        // created, to briefly hint to the user that UI controls
        // are available.
        delayedHide(100);
    }

    /**
     * Touch listener to use for in-layout UI controls to delay hiding the
     * system UI. This is to prevent the jarring behavior of controls going away
     * while interacting with activity UI.
     */
    View.OnTouchListener mDelayHideTouchListener = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View view, MotionEvent motionEvent) {
            if (AUTO_HIDE) {
                delayedHide(AUTO_HIDE_DELAY_MILLIS);
            }
            return false;
        }
    };

    Handler mHideHandler = new Handler();
    Runnable mHideRunnable = new Runnable() {
        @Override
        public void run() {
            mSystemUiHider.hide();
        }
    };

    /**
     * Schedules a call to hide() in [delay] milliseconds, canceling any
     * previously scheduled calls.
     */
    private void delayedHide(int delayMillis) {
        mHideHandler.removeCallbacks(mHideRunnable);
        mHideHandler.postDelayed(mHideRunnable, delayMillis);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    Runnable mHideChannelNumber = new Runnable() {
        @Override
        public void run() {
            channelNumberLayout.setVisibility(View.INVISIBLE);
        }
    };
    
    private void delayedHideChannelNumber(int delayMillis) {
        mHideHandler.removeCallbacks(mHideChannelNumber);
        mHideHandler.postDelayed(mHideChannelNumber, delayMillis);
    }
    
    public void handleInputNumber(int num) {
        if (inputDigit < MAX_CHANNEL_DIGIT) {
            channelNumberString = channelNumberString + num; // for keeping leading 0
            channelNumber = channelNumber * 10 + num;
            inputDigit++;
            Log.d(TAG, "channelNumber:" + channelNumber + " inputDigit:" + inputDigit + " str:" + channelNumberString);

            displayChannelNumber(channelNumberString);
            delayedCheck(CHECK_INPUT_TIMER);
        }
    }

    private void showScanProgressDlg(int title) {
        scanProgressDialog = ScanProgressDialog.newInstance(title);
        scanProgressDialog.setHandler(uiHandler);
        scanProgressDialog.show(getFragmentManager(), "scanProgressDialog");
    }
    
    private void startAutoScan() {
        boolean result = false;
        
        if (!isServerConnected) {
            Log.e(TAG, "server not connected!!");
            return;
        }

        if (isAutoScanning || isManualScanning) {
            Log.e(TAG, "Scanning now.");
            return;
        }
        
        StartScanRunnable scanRunnable = new StartScanRunnable(AUTO_SCAN);
        Thread startScanThread = new Thread(scanRunnable);
        startScanThread.start();
                
        showScanProgressDlg(R.string.auto_scan_title); // should disable "Cancel" first, enable it after SCAN_STARTED
    }

    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case R.id.region:
            Log.d(TAG, "select RegionSetting");
            RegionSettingDialog regionSettingDlg = new RegionSettingDialog();
            regionSettingDlg.setDefaultRegionID(region);
            regionSettingDlg.show(getFragmentManager(), "regionSettingDlg");
            break;
        case R.id.auto_scan:
            Log.d(TAG, "select Auto Scan");
            if (isServerConnected == false) {
                Log.e(TAG, "Not connect to server yet.");
                break;
            }
            if (isAutoScanning || isManualScanning) {
                    Log.e(TAG, "Scanning now.");
                    break;
            }

            startAutoScan();
            break;
        case R.id.manual_scan:
            Log.d(TAG, "select Manual Scan");
            if (isServerConnected == false) {
                Log.e(TAG, "Not connect to server yet.");
                break;
            }
            if (isAutoScanning || isManualScanning) {
                    Log.e(TAG, "Scanning now.");
                    break;
            }
            ManualScanSetting manualScanSetting = new ManualScanSetting();
            manualScanSetting.show(getFragmentManager(), "manualSetting");
            break;
        }
        return super.onOptionsItemSelected(item);
    }
    
    public void onManualScanClick(DialogFragment dialog) {
        frequency = ((ManualScanSetting)dialog).getFrequency();
        bandwidth = ((ManualScanSetting)dialog).getBandwidth();
        Log.d(TAG, "freq:" + frequency + " bandwidth:" + bandwidth);
        
        startManualScan();
    }
    
    private void startManualScan() {
        boolean result = false;
        
        if (!isServerConnected) {
            Log.e(TAG, "server not connected!!");
                return;
            }

        if (isAutoScanning || isManualScanning) {
            Log.e(TAG, "Scanning now.");
            return;
        }
        
        StartScanRunnable scanRunnable = new StartScanRunnable(MANUAL_SCAN);
        Thread startScanThread = new Thread(scanRunnable);
        startScanThread.start();
        
        isManualScanning = true;
        showScanProgressDlg(R.string.manual_scan_title); // should disable "Cancel" first, enable it after SCAN_STARTED
        Log.d(TAG, "AFT showScanProgressDlg()");
    }

    public void onRegionSet(DialogFragment dialog) {
        region = ((RegionSettingDialog)dialog).getRegionID();
    }

    public void setRegionID(int id) {
        region = id;
    }
    
    public int getDefaultRegionID() {
        // should get ID from DB or user data....
        return 26; // just for engineering testing
    }
    
    class UIHandler extends Handler {
        public UIHandler() {
            super();
        }
        public UIHandler(Looper looper) {
            super(looper);
        }
        
        @Override
        public void handleMessage(Message inputMessage) {
            switch (inputMessage.what) {
            case AUTO_SCAN_STARTED:
            case MANUAL_SCAN_STARTED:
                Log.d(TAG, "handleMessage(XXXX_SCAN_STARTED)");
                //showScanProgressDlg(R.string.manual_scan_title);
                // should start thread to read info
                ReadInfoRunnable readInfoRunnable = new ReadInfoRunnable();
                UIHandler.this.postDelayed(readInfoRunnable, SCAN_INTERVAL);
                break;
            case UPDATE_SCAN_INFO:
                updateScanProgress();
                break;
            case SCAN_COMPLETE:
                Log.d(TAG, "handleMessage(SCAN_COMPLETE)");
                stopScan();
                
                if (m_tvManager.Renew() == false) {
                    Log.e(TAG, "Renew() NG.");
                } else {
                    // Renew will do DvbApp::DeInit(), that's destroy playback flow.
                    // Destory playback flow should do the corresponding rvsd->stop().
                    // So we need to do releaseMediaPlayer and initMediaPlayer to renew MediaPlayer.
                    Log.v(TAG, "begin do releaseMediaPlayer/initMediaPlayer.");
                    releaseMediaPlayer();
                    initMediaPlayer();
                    Log.d(TAG, "end do releaseMediaPlayer/initMediaPlayer.");
                    
                    tunerStatus[0] = tunerStatus[1] = TvManager.DTV_TUNER_STATUS_IDLE;
                }
                
                filterOutFlags = DEFAULT_FILTER | TvManager.CHANNEL_FLAG_IS_RADIO; // select tv source for the first playing
                DvbChannelInfo dvbChannelInfo = m_tvManager.GetFirstChannel(filterOutFlags);
                if (null == dvbChannelInfo) {
                    Log.e(TAG, "GetFirstChannel() NG.");
                    return;
                }
                if (m_tvManager.PlayChannel(tunerId, dvbChannelInfo.logicalChNum, true, true) == false) {
                    Toast.makeText(getApplicationContext(), "Preview DVB NG", Toast.LENGTH_SHORT).show();
                    Log.e(TAG, "TvManualScanStart(0) NG.");
                    return;
                } else {
                    // below settings are also for the first playing
                    serviceType = DvbChannelInfo.SERVICE_TYPE_DTV;
                    currentChannel = dvbChannelInfo.logicalChNum;
                    prevChannel = -1;
                    displayChannelNumber(null);
                }
                boolean isfocused = contentView.requestFocus();
                Log.d(TAG, "contentView isFocusable:" + contentView.isFocusable() + " isfocused:" + isfocused);
                break;
            case STOP_SCAN:
                Log.d(TAG, "handleMessage(STOP_SCAN)");
                stopScan();
                break;

            case CHECK_INPUT:
                Log.d(TAG, "handleMessage(CHECK_INPUT) " + channelNumber + " count:" + channelCount);
                playChannel();
                UIHandler.this.removeCallbacks(checkChannelRunnable);
                break;

            default:
                super.handleMessage(inputMessage);
            }
        }
    }
    
    private void playChannel() {
        DvbChannelInfo dvbChannelInfo = m_tvManager.GetChannelByChannelNumber(channelNumber);
        if (dvbChannelInfo != null && dvbChannelInfo.type == serviceType) {
            if (m_tvManager.PlayChannel(tunerId, channelNumber, true, true) == false) {
                Toast.makeText(getApplicationContext(), "Preview DVB NG", Toast.LENGTH_SHORT).show();
                Log.e(TAG, "TvManualScanStart(0) NG.");
                displayChannelNumber(FORBIDDEN_STRING);
            } else {
                prevChannel = currentChannel;
                currentChannel = channelNumber;

                updateChannelByService();
                displayChannelNumber(null);
            }
        } else {
            displayChannelNumber(FORBIDDEN_STRING);
        }
        
        inputDigit = 0;
        channelNumber = 0;
        channelNumberString = "";
    }

    private void updateChannelByService() {
        if (serviceType == DvbChannelInfo.SERVICE_TYPE_DTV) {
            tvChannel = currentChannel;
        } else if (serviceType == DvbChannelInfo.SERVICE_TYPE_RADIO) {
            radioChannel = currentChannel;
        }
    }
    
    private void stopScan() {
        if (isManualScanning) {
            m_tvManager.TvManualScanStop(tunerId);
            isManualScanning = false;
        } else if (isAutoScanning) {
            m_tvManager.TvAutoScanStop(tunerId);
            isAutoScanning = false;
        }
    }

    class StartScanRunnable implements Runnable {
        int scanType;
        public StartScanRunnable(int t) {
            scanType = t;
        }
        @Override
        public void run() {
            m_tvManager.SetTvRegion(region);
            if ((scanningTunerId = acquireTunerForPreview()) < 0) {
                Log.e(TAG, "acquireTunerForPreview() NG.");
                return;
            }
            
            if (scanType == AUTO_SCAN) {
                if (m_tvManager.TvAutoScanStart(scanningTunerId) == false) {
                    //Toast.makeText(getApplicationContext(), "Auto Scan Start NG", Toast.LENGTH_SHORT).show();
                    Log.e(TAG, "TvAutoScanStart(0) NG.");
                    return;
                }
                else {
                    //Toast.makeText(getApplicationContext(), "Auto Scan Start OK", Toast.LENGTH_SHORT).show();
                    Log.v(TAG, "TvAutoScanStart(0) OK.");
                }
                isAutoScanning = true;
                Message completeMessage = FullscreenActivity.this.uiHandler.obtainMessage(AUTO_SCAN_STARTED);
                FullscreenActivity.this.uiHandler.sendMessage(completeMessage);
            } else if (scanType == MANUAL_SCAN) {
                Log.d(TAG, "BEF scan frq:" + frequency + " Bandwidth:" + bandwidth);
                m_tvManager.TvManualScanStart(scanningTunerId, frequency, bandwidth);
            
                Message completeMessage = FullscreenActivity.this.uiHandler.obtainMessage(MANUAL_SCAN_STARTED);
                FullscreenActivity.this.uiHandler.sendMessage(completeMessage);
            }
        }
    }

    class ReadInfoRunnable implements Runnable {
        @Override
        public void run() {
            if (scanPercentage < 100) {

                scanPercentage = m_tvManager.TvScanInfo(tunerId, TvManager.SCAN_INFO_PROGRESS);
                channelCount = m_tvManager.TvScanInfo(tunerId, TvManager.SCAN_INFO_CHANNEL_COUNT);
                channelStrength = m_tvManager.TvScanInfo(tunerId, TvManager.SCAN_INFO_CHANNEL_STRENGTH);
                channelQuality = m_tvManager.TvScanInfo(tunerId, TvManager.SCAN_INFO_CHANNEL_QUALITY);
                DvbChannelInfo channelInfo;
                Log.d(TAG, "Task percentage:" + scanPercentage + " count:" + channelCount + " strenth:" + channelStrength);
                
                // get TV Channel
                int tvCount = m_tvManager.GetChannelList(DEFAULT_FILTER | TvManager.CHANNEL_FLAG_IS_RADIO);
                tvChList.clear();
                for (int i = 0; i < tvCount; i++) {
                    channelInfo = m_tvManager.GetChannelByChannelIndex(i);
                    if (channelInfo == null) {
                        Log.d(TAG, "D i:" + i + " is null");
                        continue;
                    }
                    tvChList.add(String.format("%02d ", channelInfo.logicalChNum) + channelInfo.channelName);
                    Log.d(TAG, "D i:" + i + " LCN:" + channelInfo.logicalChNum + " Type:" + channelInfo.type + " Name:" + channelInfo.channelName);// + channelInfo.channelName + " LCN:" + channelInfo.logicalChNum);
                }
                m_tvManager.FreeChannelList();

                // get Radio Channel
                int radioCount = m_tvManager.GetChannelList(DEFAULT_FILTER | TvManager.CHANNEL_FLAG_IS_DTV);
                radioChList.clear();
                for (int i = 0; i < radioCount; i++) {
                    channelInfo = m_tvManager.GetChannelByChannelIndex(i);
                    if (channelInfo == null) {
                        Log.d(TAG, "R i:" + i + " is null");
                        continue;
                    }
                    radioChList.add(String.format("%02d ", channelInfo.logicalChNum) + channelInfo.channelName);
                    Log.d(TAG, "R i:" + i + " LCN:" + channelInfo.logicalChNum + " Type:" + channelInfo.type + " Name:" + channelInfo.channelName);// + channelInfo.channelName + " LCN:" + channelInfo.logicalChNum);
                }
                m_tvManager.FreeChannelList();

                uiHandler.postDelayed(ReadInfoRunnable.this, SCAN_INTERVAL);
                
                Message completeMessage = FullscreenActivity.this.uiHandler.obtainMessage(UPDATE_SCAN_INFO);
                FullscreenActivity.this.uiHandler.sendMessage(completeMessage);
            } else { // scanPercentage == 100
                //Toast.makeText(getApplicationContext(), "scan:100%", Toast.LENGTH_LONG).show();
                uiHandler.removeCallbacks(ReadInfoRunnable.this);
                stopScan();
            }
        }
    }

    private void updateScanProgress() {
        scanProgressDialog.setStatus(scanPercentage, channelStrength, channelQuality, tvChList, radioChList);
    }

    private void delayedCheck(int delayMillis) {
        if (checkChannelRunnable != null) {
            uiHandler.removeCallbacks(checkChannelRunnable);
            uiHandler.postDelayed(checkChannelRunnable, delayMillis);
        } else {
            Log.d(TAG, "checkChannelRunnable==NULL!!!!!!!!!!!!!!!!!!");
        }
    }

    class CheckChannelRunnable implements Runnable {
        @Override
        public void run() {
            Message completeMessage = FullscreenActivity.this.uiHandler.obtainMessage(CHECK_INPUT);
            FullscreenActivity.this.uiHandler.sendMessage(completeMessage);
        }
    }

    private void switchService() {
        int targetChannel;
        int newFlags;
        int newServiceType;
        DvbChannelInfo dvbChannelInfo;
        String pref_string;
        int visibility;
        
        if (serviceType == DvbChannelInfo.SERVICE_TYPE_DTV) {
            newFlags = filterOutFlags & ~TvManager.CHANNEL_FLAG_IS_RADIO | TvManager.CHANNEL_FLAG_IS_DTV;
            pref_string = PREFS_RADIO_CHANNEL;
            newServiceType = DvbChannelInfo.SERVICE_TYPE_RADIO;
            visibility = View.VISIBLE;
            targetChannel = radioChannel;
        } else if (serviceType == DvbChannelInfo.SERVICE_TYPE_RADIO) {
            newFlags = filterOutFlags & ~TvManager.CHANNEL_FLAG_IS_DTV | TvManager.CHANNEL_FLAG_IS_RADIO;
            pref_string = PREFS_TV_CHANNEL;
            newServiceType = DvbChannelInfo.SERVICE_TYPE_DTV;
            visibility = View.INVISIBLE;
            targetChannel = tvChannel;
        } else {
            Log.e(TAG, "Incorrect serviceType:" + serviceType);
            return;
        }

        if (targetChannel == -1) { // no saved channel
            // find the first channel
            dvbChannelInfo = m_tvManager.GetFirstChannel(newFlags);
            if (null == dvbChannelInfo) { // no such service
                displayChannelNumber(FORBIDDEN_STRING);
                Log.e(TAG, "GetFirstChannel(" + String.format("0x%X", newFlags) + ") NG.");
                return;
            }
            targetChannel = dvbChannelInfo.logicalChNum;
        }
        
        if (m_tvManager.PlayChannel(tunerId, targetChannel, true, true) == false) {
            Log.e(TAG, "PlayChannel(" + targetChannel + ") NG.");
            return;
        } else {
            // below settings are also for the first playing
            serviceType = newServiceType;
            filterOutFlags = newFlags;
            prevChannel = currentChannel;
            currentChannel = targetChannel;
            radioBackground.setVisibility(visibility);
            updateChannelByService();
            displayChannelNumber(null);
        }
    }
    
    private void getPreference() {
        settings     = getSharedPreferences(PREFS_NAME, Context.MODE_MULTI_PROCESS);
        region       = settings.getInt(PREFS_REGION, -1);
        serviceType  = settings.getInt(PREFS_SOURCE, DvbChannelInfo.SERVICE_TYPE_DTV);
        if (serviceType == DvbChannelInfo.SERVICE_TYPE_DTV) {
            prevChannel = tvChannel = settings.getInt(PREFS_TV_CHANNEL, -1);
        } else if (serviceType == DvbChannelInfo.SERVICE_TYPE_RADIO) {
            prevChannel = radioChannel = settings.getInt(PREFS_RADIO_CHANNEL, -1);
        }
        
        Log.d(TAG, "getPreference region:" + region + " serviceType:" + serviceType + " prevChannel:" + prevChannel);
    }
    
    private void setPreference() {
        if (settings != null) {
            settings.edit()
                    .putInt(PREFS_REGION, region)
                    .putInt(PREFS_SOURCE, serviceType)
                    .putInt(PREFS_TV_CHANNEL, tvChannel)
                    .putInt(PREFS_RADIO_CHANNEL, radioChannel)
                    //.putInt(PREFS_TV_CHANNEL, arg1) // PREFS_TV_CHANNEL should be saved when channel is successfully selected.
                    .commit();
        }
    }
}
