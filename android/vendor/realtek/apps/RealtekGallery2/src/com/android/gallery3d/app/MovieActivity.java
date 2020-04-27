/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.gallery3d.app;

import android.annotation.TargetApi;
import android.app.ActionBar;
import android.app.Activity;
import android.content.AsyncQueryHandler;
import android.content.ContentResolver;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.Context;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;
import android.content.res.Configuration;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.Point;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.TrackInfo;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.provider.MediaStore;
import android.provider.OpenableColumns;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
import android.view.Display;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ShareActionProvider;

import com.android.gallery3d.R;
import com.android.gallery3d.common.ApiHelper;
import com.android.gallery3d.common.Utils;
import com.android.gallery3d.util.GalleryUtils;
import com.realtek.hardware.RtkVoutUtilManager;
import android.widget.AdapterView.OnItemClickListener;
import android.view.View.OnKeyListener;
import android.widget.AdapterView;
import android.widget.PopupWindow;
import android.view.Gravity;
import android.widget.TextView;
import android.widget.ImageView;
import java.util.ArrayList;
import android.app.Instrumentation;
import java.io.File;
import java.math.BigDecimal;
import android.os.SystemProperties;

import android.widget.Toast;
import android.app.NotificationManager;

/**
 * This activity plays a video from a specified URI.
 *
 * The client of this activity can pass a logo bitmap in the intent (KEY_LOGO_BITMAP)
 * to set the action bar logo so the playback process looks more seamlessly integrated with
 * the original activity.
 */
public class MovieActivity extends Activity {
    @SuppressWarnings("unused")
    private static final String TAG = "MovieActivity";
    public static final String KEY_LOGO_BITMAP = "logo-bitmap";
    public static final String KEY_TREAT_UP_AS_BACK = "treat-up-as-back";

    public static final int AR_FULL_INDEX = 0;
    public static final int AR_FIT_INDEX = 1;

    private MoviePlayer mPlayer;
    public boolean mFinishOnCompletion;
    private boolean mBroadcastStatus;
    private Uri mUri;
    private String mDir = null;
    private String mFileName = null;
    private int mSubIndex = 0;
    private int mSubOffset = 0;
    private int mSubSize = 20;
    private float mSubSync = 0.0f;
    private int mSubEncode = 0;
    private int mSubColor = 0;
    private int mAudioIndex = 0;

    private BigDecimal mSubStringTime = new BigDecimal("0.0");
    private boolean mTreatUpAsBack;
    private int mUseRTMediaPlayer;

    private static final String USE_RT_MEDIAPLAYER_EXTRA = "MEDIA_BROWSER_USE_RT_MEDIA_PLAYER";
    private static final String ACTIVITYCMD = "com.android.gallery3d.movieactivitycommand";
    public static final String CMDSTOP = "stop";
    public static final String CMDPAUSE = "pause";
    public static final String CMDPLAY = "play";
    public static final String CMDSEEK = "seek";
    public static final String CMDRATE = "rate";
    public static final String CMDREPEAT = "repeat";
    public static final String CMDPREV = "previous";
    public static final String CMDNEXT = "next";
    public static final String CMDRESTART = "restart";
    public static final String CMDUPDATEPLAYLIST = "updatePlaylist";

    public static final String SYSTEMUI_EXPAND_ACTION = "com.android.system.ui.panel.expand";
    public static final String SYSTEMUI_COLLAPSE_ACTION = "com.android.system.ui.panel.collapse";

    public static final String CONTROL_PANEL_SHOW_ACTION = "com.rtk.ControlPanel.show";
    public static final String CONTROL_PANEL_HIDE_ACTION = "com.rtk.ControlPanel.hide";

    public static final String SUBTITLE_PREF = "Subtitle_Pref";
    public static final String KEY_SAVE_SUBTITLE_PREF = "Save_Subtitle_Pref";
    public static final String KEY_SUBTITLE_OFFSET_PREF = "Subtitle_Offset_Pref";
    public static final String KEY_SUBTITLE_TEXTSIZE_PREF = "Subtitle_TextSize_Pref";
    public static final String KEY_SUBTITLE_SYNC_PREF = "Subtitle_Sync_Pref";
    public static final String KEY_SUBTITLE_TEXTENCODE_PREF = "Subtitle_TextEncode_Pref";
    public static final String KEY_SUBTITLE_TEXTCOLOR_PREF = "Subtitle_TextColor_Pref";
    public static final String KEY_REPEAT_MODE_PREF = "Repeat_Mode_Pref";

    private RtkVoutUtilManager mRtkVoutUtilManager;
    private RTKOptionMenu  mOptionMenu = null;
    private int mCurrentSubFocus = 0;
    private int mIsTextSubtitle = MoviePlayer.SUBTITLE_NON_TEXT;
    private int m3DFocus = 0;
    private int m3Dto2DFocus = 0;
    private int m3DSubFocus = 0;
    private int m3DMode = 0;
    public int m3Dto2DMode = 0;
    public int m3DSubMode = 0;
    public int mARFocus = AR_FULL_INDEX;
    private RTKVScrollMenu[] mSubMenuGroup = new RTKVScrollMenu[6];
    private RTKVScrollMenu mSubMainMenu = null;
    private RTKVScrollMenu mSubOffsetMenu = null;
    private RTKVScrollMenu mSubSizeMenu = null;
    private RTKVScrollMenu mSubSyncMenu = null;
    private RTKVScrollMenu mSubEncodeMenu = null;
    private RTKVScrollMenu mSubColorMenu = null;
    private RTKVScrollMenu mPropertyMenu = null;
    private RTKVScrollMenu mBDInfoMenu = null;
    private RTKVScrollMenu mBDChapterInfoMenu = null;
    private ImageView mStringBackImg;
    private ImageView mStringAddImg;
    private PopupWindow mStringUpkey = null;
    private PopupWindow mStringDownkey = null;
    private View mStringUpView = null;
    private View mStringDownView = null;
    private TextView mBackHint = null;
    private TextView mAddHint = null;
    
    private RTKVScrollMenu mAudioMenu = null;
    private View mCantHearView = null;
    private ImageView[] m3DMenuGroup;
    private ImageView[] m3Dto2DMenuGroup;
    private ImageView[] m3DSubMenuGroup;
    private View m3DView;
    private View m3Dto2DView;
    private View m3DSubView;
    private PopupWindow m3DOption = null;
    private PopupWindow m3Dto2DOption = null;
    private PopupWindow m3DSubOption = null;

    private ImageView[] mARMenuGroup= new ImageView[2];
    private View mARView;
    private PopupWindow mAROption = null;
    private View mHoverView=null;
    private boolean mNotificationShown=false;
    private int mSystemUIShown=0;
    private boolean mControlPanelShown=false;
    private final Handler mHandler = new Handler();

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    private void setSystemUiVisibility(View rootView) {
        if (ApiHelper.HAS_VIEW_SYSTEM_UI_FLAG_LAYOUT_STABLE) {
            rootView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_IMMERSIVE
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        }
    }

    private final Runnable mSetSubtitleInfo = new Runnable() {
        @Override
        public void run() {
            setSubtitleInfo();
        }
    };

    private final Runnable mUpdateSubtitleMenu = new Runnable() {
        @Override
        public void run() {
            mSubMenuGroup[0].setSelectedItem(mSubIndex);
            mSubMenuGroup[4].setSelectedItem(mSubEncode);
            mSubMenuGroup[5].setSelectedItem(mSubColor);
            mSubMenuGroup[mCurrentSubFocus].Show(true);
        }
    };

    private BroadcastReceiver mIntentReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            String cmd = intent.getStringExtra("command");
            Log.d(TAG, "mIntentReceiver.onReceive " + action + " / " + cmd);
           
            if (CMDPAUSE.equals(cmd)) {
                if (mPlayer.isPlaying())
                    mPlayer.onPlayPause(true, false);

            } else if (CMDPLAY.equals(cmd)) {
                if (!mPlayer.isPlaying())
                    mPlayer.onPlayPause(true, false);

            } else if (CMDSTOP.equals(cmd)) {
                mPlayer.onStop();
                //finish();
            } else if (CMDSEEK.equals(cmd)) {
                int seekTime = intent.getIntExtra("seektime", -1);
                int duration = mPlayer.getDuration();
                Log.d("Seek Command", "duration = " +  duration);
                Log.d("Seek Command", "seekTime = " +  seekTime);
                if (seekTime >= 0 && seekTime < duration)
                {
                    mPlayer.onSeekMove(seekTime);
                }
            } else if (CMDRATE.equals(cmd)) {
                int rateValue = intent.getIntExtra("rate", 1);
                Log.d("Rate Command", "rateValue = " +  rateValue);
                mPlayer.setVideoSpeed(rateValue);
            } else if (CMDREPEAT.equals(cmd)) {
                mPlayer.onRepeatModeChange();
            } else if (CMDPREV.equals(cmd)) {
                mPlayer.onPrevious();
            } else if (CMDNEXT.equals(cmd)) {
                mPlayer.onNext();
            } else if (CMDRESTART.equals(cmd)) {
                String videoUrls[] = intent.getStringArrayExtra("playlist-url");
                mPlayer.onRestart(videoUrls);
            } else if (CMDUPDATEPLAYLIST.equals(cmd)) {
                String videoUrls[] = intent.getStringArrayExtra("playlist-url");
                mPlayer.onUpdatePlaylist(videoUrls);
            }
        }
    };

    private BroadcastReceiver mUIStatusReceiver = new BroadcastReceiver()
    {
        public void onReceive(Context context, Intent intent)
        {
            String source = intent.getStringExtra("source");
            if (intent.getAction().equals(SYSTEMUI_EXPAND_ACTION)) {
                if(source!=null && source.equals("NotificationPanel")) {
                    mNotificationShown=true;
                    Log.d(TAG, "Notification is showing.");
                }
                else {
                    mSystemUIShown++;
                    Log.d(TAG, "System UI shown count: "+mSystemUIShown);
                }
            }
            else if (intent.getAction().equals(SYSTEMUI_COLLAPSE_ACTION)) {
                if(source!=null && source.equals("NotificationPanel")) {
                    mNotificationShown=false;
                    Log.d(TAG, "Notification is hiding.");
                }
                else {
                    mSystemUIShown--;
                    Log.d(TAG, "System UI shown count: "+mSystemUIShown);
                }
            }
            else if(intent.getAction().equals(CONTROL_PANEL_SHOW_ACTION)) {
                mControlPanelShown=true;
            }
            else if(intent.getAction().equals(CONTROL_PANEL_HIDE_ACTION)) {
                mControlPanelShown=false;
            }
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate");
        super.onCreate(savedInstanceState);
        GalleryUtils.awakenDreams();
        SystemProperties.set("rtk.hwc.osd.hide.ms", Long.toString(2000));
        requestWindowFeature(Window.FEATURE_ACTION_BAR);
        requestWindowFeature(Window.FEATURE_ACTION_BAR_OVERLAY);
        // BUG_FIX: 43809
        NotificationManager mgr = (NotificationManager)getSystemService(Context.NOTIFICATION_SERVICE);
        mgr.cancel(MoviePlayer.VIDEO_NOTIFICATION_ID);

        setContentView(R.layout.movie_view);
        View rootView = findViewById(R.id.movie_view_root);
        mHoverView = findViewById(R.id.hover_view);
        mHoverView.setOnHoverListener(new View.OnHoverListener(){
                public boolean onHover (View v, MotionEvent event){
                    if(event.getActionMasked()==MotionEvent.ACTION_HOVER_ENTER){
                        mPlayer.mController.show(true,true);
                    }
                    return true;
                }
            });

        setSystemUiVisibility(rootView);

        Intent intent = getIntent();
        initializeActionBar(intent);
        mFinishOnCompletion = intent.getBooleanExtra(
                MediaStore.EXTRA_FINISH_ON_COMPLETION, true);
        mBroadcastStatus= intent.getBooleanExtra(
                "android.intent.playback.broadcast.status", false);
        mTreatUpAsBack = intent.getBooleanExtra(KEY_TREAT_UP_AS_BACK, false);

        if(getIntent()!=null && getIntent().getExtras()!=null && getIntent().getExtras().containsKey(USE_RT_MEDIAPLAYER_EXTRA)==true) {
            if(getIntent().getExtras().getBoolean(USE_RT_MEDIAPLAYER_EXTRA)==true) {
                mUseRTMediaPlayer = MediaPlayer.FORCE_RT_MEDIAPLAYER;
            }
            else {
                mUseRTMediaPlayer = MediaPlayer.FORCE_ANDROID_MEDIAPLAYER;
            }
        }
        else {
            mUseRTMediaPlayer = MediaPlayer.FORCE_NONE;
        }
        Log.d(TAG, "MediaPlayerType: "+mUseRTMediaPlayer);

        // case only used to DMR playback
        if(getIntent()!=null && getIntent().getExtras()!=null && getIntent().getExtras().containsKey("repeat")==true) {
            if(getIntent().getExtras().getBoolean("repeat")==false) {
                setRepeatModePref(0);
            }
        }

        Uri videoUri = intent.getData();
        
        if(videoUri.getScheme()!=null && videoUri.getScheme().equals("smb")) {
            videoUri = mountSambaServer(videoUri);
        }   
        Log.d(TAG, "videoUri: "+videoUri.toString());
        mPlayer = new MoviePlayer(rootView, this, videoUri, savedInstanceState, !mFinishOnCompletion, mBroadcastStatus, mUseRTMediaPlayer);
        //{
            //@Override
            //public void onCompletion() {
            //    Log.d("MoviePlayer","\033[0;31;31m Override onCompletion \033[m");
            //    if (mFinishOnCompletion) {
            //        finish();
            //    }
            //}
        //};
        if (intent.hasExtra(MediaStore.EXTRA_SCREEN_ORIENTATION)) {
            int orientation = intent.getIntExtra(
                    MediaStore.EXTRA_SCREEN_ORIENTATION,
                    ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED);
            if (orientation != getRequestedOrientation()) {
                setRequestedOrientation(orientation);
            }
        }
        Window win = getWindow();
        WindowManager.LayoutParams winParams = win.getAttributes();
        winParams.buttonBrightness = WindowManager.LayoutParams.BRIGHTNESS_OVERRIDE_OFF;
        winParams.flags |= WindowManager.LayoutParams.FLAG_FULLSCREEN;
        win.setAttributes(winParams);

        // We set the background in the theme to have the launching animation.
        // But for the performance (and battery), we remove the background here.
        win.setBackgroundDrawable(null);

        mRtkVoutUtilManager = new RtkVoutUtilManager();
        if(mRtkVoutUtilManager == null) {
            Log.d(TAG, "mRtkVoutUtilManager is null!");
        }
        // spec out, Acspect Ratio is removed from setting menu
        //setARRescaleModeBySetting();

        // BUG_FIX: 44026
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {
        if ( (event.getSource() & InputDevice.SOURCE_MOUSE) != 0) {
            if(mPlayer!=null) {
                mPlayer.freshHideSystemUi();
            }
            return true;
        }
        else {
            return false;
        }
    }

    private void setActionBarLogoFromIntent(Intent intent) {
        Bitmap logo = intent.getParcelableExtra(KEY_LOGO_BITMAP);
        if (logo != null) {
            getActionBar().setLogo(
                    new BitmapDrawable(getResources(), logo));
        }
    }

    private void initializeActionBar(Intent intent) {
        mUri = intent.getData();
        if (mUri != null) {
            mDir = mUri.getPath();
            //get the directory path
            int loc = mDir.lastIndexOf("/");
            if (loc > 0) {
                int loc2 = mDir.lastIndexOf(".");
                if (loc2 > (loc + 1)) {
                    mFileName = mDir.substring(loc + 1, loc2);
                    mDir = mDir.substring(0, mDir.lastIndexOf("/") + 1);
                    Log.d("MovieActivity", "mFileName = " +  mFileName);
                    Log.d("MovieActivity", "mDir = " +  mDir);
                }
           }
        }
        final ActionBar actionBar = getActionBar();
        if (actionBar == null) {
            return;
        }
        setActionBarLogoFromIntent(intent);
        actionBar.setDisplayOptions(
                ActionBar.DISPLAY_HOME_AS_UP,
                ActionBar.DISPLAY_HOME_AS_UP);

        String title = intent.getStringExtra(Intent.EXTRA_TITLE);
        if (title != null) {
            actionBar.setTitle(title);
        } else {
            // Displays the filename as title, reading the filename from the
            // interface: {@link android.provider.OpenableColumns#DISPLAY_NAME}.
            AsyncQueryHandler queryHandler =
                    new AsyncQueryHandler(getContentResolver()) {
                @Override
                protected void onQueryComplete(int token, Object cookie,
                        Cursor cursor) {
                    try {
                        if ((cursor != null) && cursor.moveToFirst()) {
                            String displayName = cursor.getString(0);

                            // Just show empty title if other apps don't set
                            // DISPLAY_NAME
                            actionBar.setTitle((displayName == null) ? "" :
                                    displayName);
                        }
                    } finally {
                        Utils.closeSilently(cursor);
                    }
                }
            };
            queryHandler.startQuery(0, null, mUri,
                    new String[] {OpenableColumns.DISPLAY_NAME}, null, null,
                    null);
        }
    }

    public void showOption() {
        if(mOptionMenu == null)
            NewOptionMenu();

        ArrayList<String> list = new ArrayList<String>();
        list.add("Subtitle");
        list.add("Audio");
        if(!mPlayer.get3DEnable() && m3Dto2DMode==0) {
            if(mPlayer.get3DType()!=MoviePlayer.MODE_3D_NOT_3DTV) {
                list.add("3D");
            }
            list.add("3Dto2D");
        }
        else {
            list.add("Video RAW");
        }
        list.add("Aspect Ratio");
        if(mPlayer.bHasNaviMenu())
        {
            list.add("Title Menu");
            list.add("Root Menu");
        }
        if(mPlayer.getBDTitleList().size()>0)
        {
            list.add("BD_Title");
        }
        if(mPlayer.getBDChapterList().size()>0)
        {
            list.add("BD_Chapter");
        }
        list.add("Property");
        if(getSubtitlePref()==true) {
            list.add("Save Subtitle: On");
        }
        else {
            list.add("Save Subtitle: Off");
        }
        mOptionMenu.setSelectionList(list);

        mPlayer.hideController();
        Instrumentation ins = new Instrumentation();
        ins.setInTouchMode(false);
        mOptionMenu.Show();
        mPlayer.show3DHint();
    }

    public void hideOption() {
        if(mOptionMenu!=null && mOptionMenu.isShowing()) {
            mOptionMenu.Hide();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        super.onCreateOptionsMenu(menu);
        getMenuInflater().inflate(R.menu.movie, menu);

        // Document says EXTRA_STREAM should be a content: Uri
        // So, we only share the video if it's "content:".
        MenuItem shareItem = menu.findItem(R.id.action_share);
        if (ContentResolver.SCHEME_CONTENT.equals(mUri.getScheme())) {
            shareItem.setVisible(true);
            ((ShareActionProvider) shareItem.getActionProvider())
                    .setShareIntent(createShareIntent());
        } else {
            shareItem.setVisible(false);
        }
        return true;
    }

    private Intent createShareIntent() {
        Intent intent = new Intent(Intent.ACTION_SEND);
        intent.setType("video/*");
        intent.putExtra(Intent.EXTRA_STREAM, mUri);
        return intent;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == android.R.id.home) {
            if (mTreatUpAsBack) {
                finish();
            } else {
                startActivity(new Intent(this, GalleryActivity.class));
                finish();
            }
            return true;
        } else if (id == R.id.action_share) {
            startActivity(Intent.createChooser(createShareIntent(),
                    getString(R.string.share)));
            return true;
        }
        return false;
    }

    @Override
    public void onStart() {
        Log.d(TAG, "onStart");
        ((AudioManager) getSystemService(AUDIO_SERVICE))
                .requestAudioFocus(null, AudioManager.STREAM_MUSIC,
                AudioManager.AUDIOFOCUS_GAIN_TRANSIENT);
        super.onStart();
    }

    @Override
    protected void onStop() {
        Log.d(TAG, "onStop");
        ((AudioManager) getSystemService(AUDIO_SERVICE))
                .abandonAudioFocus(null);
        super.onStop();
    }

    @Override
    public void onPause() {
        Log.d(TAG, "onPause");
        mSubIndex = mPlayer.setSubtitleInfo(mSubIndex, 0, getSubtitleEncodeMapping(mSubEncode), getSubtitleColorMapping(mSubColor), mSubSize, (int)(mSubSync*1000), -mSubOffset);
        mPlayer.mController.hide();
        mPlayer.onPause();
        super.onPause();
        unregisterReceiver(mIntentReceiver);
        unregisterReceiver(mUIStatusReceiver);
        mSubMainMenu = null;
        mAudioMenu = null;
        unmountSambaServer();
        resetActivity();
    }

    @Override
    public void onResume() {
        Log.d(TAG, "onResume");
        getTextEncodeSetup();
        mPlayer.onResume();
        super.onResume();
        IntentFilter commandFilter = new IntentFilter();
        commandFilter.addAction(ACTIVITYCMD);
        commandFilter.addAction(Intent.ACTION_MEDIA_UNMOUNTED);
        commandFilter.addAction(Intent.ACTION_MEDIA_EJECT);
        commandFilter.addAction(Intent.ACTION_MEDIA_REMOVED);
        commandFilter.addAction(Intent.ACTION_MEDIA_BAD_REMOVAL);
        registerReceiver(mIntentReceiver, commandFilter);

        IntentFilter uiStatusFilter = new IntentFilter();
        uiStatusFilter.addAction(SYSTEMUI_EXPAND_ACTION);
        uiStatusFilter.addAction(SYSTEMUI_COLLAPSE_ACTION);
        uiStatusFilter.addAction(CONTROL_PANEL_SHOW_ACTION);
        uiStatusFilter.addAction(CONTROL_PANEL_HIDE_ACTION);
        registerReceiver(mUIStatusReceiver, uiStatusFilter);
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig)
    {
        super.onConfigurationChanged(newConfig);
        Log.d(TAG,"Configuration Change :"+newConfig);
        Log.d(TAG,"Configuration Change keyboard: "+newConfig.keyboard);
        Log.d(TAG,"Configuration Change keyboardHidden: "+newConfig.keyboardHidden);
    }

    @Override
    public void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        mPlayer.onSaveInstanceState(outState);
    }

    @Override
    public void onDestroy() {
        Log.d(TAG, "onDestroy");
        mPlayer.onDestroy();
        super.onDestroy();
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_M) {
            if(event.getRepeatCount()==0) {
                showOption();
            }
            return true;
        }
        return mPlayer.onKeyDown(keyCode, event)
                || super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        return mPlayer.onKeyUp(keyCode, event)
                || super.onKeyUp(keyCode, event);
    }

    private class OptionMenuItemClickListener implements OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id)
        {
        	Log.d("MovieActivity", "Option menu " + mOptionMenu.getItemof(position)+" is clicked");
            mOptionMenu.setSelectedItem(position);
            optionMenuHandler(mOptionMenu.getItemof(position) , position);
        }
    }

    public void selfDestruct(View view) {
        mPlayer.onPlayPause(false, false);
    }

    public boolean optionMenuHandler(String item, int position) {

        if (item.equals("Subtitle")){
            mOptionMenu.Hide();
            
            if (mSubMainMenu == null)
                NewSubtitleMenu();

            for (int i = 0; i < mSubMenuGroup.length; i++) {
                if(i != mCurrentSubFocus)
                    mSubMenuGroup[i].Show(false);
            }

            if (mSubMenuGroup[mCurrentSubFocus] == mSubSyncMenu){
                mStringBackImg.setVisibility(View.VISIBLE);
                mStringAddImg.setVisibility(View.VISIBLE);
                mBackHint.setVisibility(View.VISIBLE);
                mAddHint.setVisibility(View.VISIBLE);    
            }
            else {
                mStringBackImg.setVisibility(View.INVISIBLE);
                mStringAddImg.setVisibility(View.INVISIBLE);
                mBackHint.setVisibility(View.INVISIBLE);
                mAddHint.setVisibility(View.INVISIBLE);    
            }

            mStringUpkey.setWidth(mSubMenuGroup[mCurrentSubFocus].getWidth());
            mStringDownkey.setWidth(mSubMenuGroup[mCurrentSubFocus].getWidth());
            mStringUpkey.showAtLocation(mStringUpView, Gravity.LEFT| Gravity.TOP, mSubMenuGroup[mCurrentSubFocus].m_x,
                (int)getResources().getDimension(R.dimen.video_subtitle_menu_margin_top) - (int)getResources().getDimension(R.dimen.video_subtitle_updown_key_height));
            mStringDownkey.showAtLocation(mStringDownView, Gravity.LEFT| Gravity.TOP, mSubMenuGroup[mCurrentSubFocus].m_x,
                (int)getResources().getDimension(R.dimen.video_subtitle_menu_margin_top) + (int)getResources().getDimension(R.dimen.video_subtitle_menu_height));
            
            UpdateSubtitleList();
            for (int i = 0; i < mSubMenuGroup.length; i++) {
                mSubMenuGroup[i].update();
            }

            mPlayer.enableABRepeat();
            checkIsTextSubtitle();
            mSubMenuGroup[mCurrentSubFocus].requestFocusFromTouch();
            mSubMenuGroup[mCurrentSubFocus].setFocusable(true);
            mHandler.postDelayed(mUpdateSubtitleMenu, 0);
            return true;
        }
        else if (item.equals("Audio")){
            mOptionMenu.Hide();
            
            if (mAudioMenu == null)
                NewAudioMenu();
            UpdateAudioList();
            mAudioMenu.setBackgroundDrawable(new BitmapDrawable());
            mAudioMenu.Show(true);
            return true;
        }
        else if (item.equals("3D")) {
            mOptionMenu.Hide();
            if(mPlayer.get3DType()==MoviePlayer.MODE_3D_MVC) {
                if(mPlayer.get3DEnable()==false) {
                    mPlayer.setMVC(true);
                    mPlayer.setFormat3d(RtkVoutUtilManager.FORMAT_3D_AUTO_DETECT);
                }
            }
            else if(mPlayer.get3DType()==MoviePlayer.MODE_3D_NOT_MVC) {
                if (m3DOption == null)
                    New3DMenu();

                m3DOption.showAtLocation(m3DView, Gravity.CENTER, 0, 0);
                m3DOption.setFocusable(true);
                m3DOption.setOutsideTouchable(true);
                m3DOption.update();
            }
            //mPlayer.set3dSub(RtkVoutUtilManager.FORMAT_3D_SUB_CENTER);
            return true;
        }
        else if (item.equals("3Dto2D")) {
            mOptionMenu.Hide();
            if (m3Dto2DOption == null)
                New3Dto2DMenu();
            m3Dto2DOption.showAtLocation(m3Dto2DView, Gravity.CENTER, 0, 0);
            m3Dto2DOption.setFocusable(true);
            m3Dto2DOption.setOutsideTouchable(true);
            m3Dto2DOption.update();
            return true;
        }
        else if (item.equals("Video RAW")) {
            mOptionMenu.Hide();
            if(mPlayer.get3DEnable()) {
                reset3DMode();
            }
            else {
                set3Dto2DMode(-1, true);
            }
            return true;
        }
        else if (item.equals("Aspect Ratio")) {
            mOptionMenu.Hide();
            
            if (mAROption == null)
                NewARMenu();
            
            mAROption.showAtLocation(mARView, Gravity.CENTER, 0, 0);
            mAROption.setFocusable(true);
            mAROption.setOutsideTouchable(true);
            setARRescaleMode(mARFocus);
            mAROption.update();
            return true;
        }
        else if(item.equals("Title Menu")){
            mOptionMenu.Hide();
            mPlayer.gotoNaviMenu(MediaPlayer.NAVI_DVD_TITLE_MENU);
            mPlayer.setSubtitleInfo(mSubIndex, 1, getSubtitleEncodeMapping(mSubEncode), getSubtitleColorMapping(mSubColor), mSubSize, (int)(mSubSync*1000), 0);
            return true;
        }
        else if(item.equals("Root Menu")){
            mOptionMenu.Hide();
            mPlayer.gotoNaviMenu(MediaPlayer.NAVI_DVD_ROOT_MENU);
            mPlayer.setSubtitleInfo(mSubIndex, 1, getSubtitleEncodeMapping(mSubEncode), getSubtitleColorMapping(mSubColor), mSubSize, (int)(mSubSync*1000), 0);
            return true;
        }
        else if(item.equals("BD_Title")){
            mOptionMenu.Hide();
            if (mBDInfoMenu == null) {
                NewBDInfoMenu();
            }
            else {
                UpdateBDInfoMenu();
            }
            mBDInfoMenu.setBackgroundDrawable(new BitmapDrawable());
            mBDInfoMenu.Show(true);
            return true;
        }
        else if(item.equals("BD_Chapter")){
            mOptionMenu.Hide();
            if (mBDChapterInfoMenu == null) {
                NewBDChapterInfoMenu();
            }
            else {
                UpdateBDChapterInfoMenu();
            }
            mBDChapterInfoMenu.setBackgroundDrawable(new BitmapDrawable());
            mBDChapterInfoMenu.Show(true);
            return true;
        }
        else if (item.equals("Property")) {
            mOptionMenu.Hide();
            if (mPropertyMenu == null) {
                NewPropertyMenu();
            }
            else {
                UpdatePropertyMenu();
            }
            mPropertyMenu.setBackgroundDrawable(new BitmapDrawable());
            mPropertyMenu.Show(true);
            return true;
        }
        else if (item.startsWith("Save Subtitle")){
            boolean val = getSubtitlePref();
            String str;
            if(val==false) {
                setSubtitlePref(true);
                str = "Save Subtitle: On";
            }
            else {
                setSubtitlePref(false);
                str = "Save Subtitle: Off";
            }
            mOptionMenu.refreshItem(position , str);
        }
        return false;
    }

    private class OptionMenuItemOnKeyListener implements OnKeyListener
    {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            int itemPosition = mOptionMenu.getSelectedItemPosition();
            Log.d("MovieActivity", "Option menu " + mOptionMenu.getItemof(itemPosition)+ " is on key");
            int action = event.getAction();
            if (action == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE) {
                    if(event.getRepeatCount()==0) {
                        mOptionMenu.Hide();
                        //mPlayer.onPlayPause(false, false);
                    }
                    return true;
                }
            }             
            return false;
        }
    }

    public void updateSubtitleIdx(int index) {
        Log.d(TAG, "updateSubtitleIdx = " + index);
        mSubIndex = index;
        setSubtitleInfo();
    }

    private int getSubtitleEncodeMapping(int index)
    {
        /*
        *         TEXT_ENCODING_TABLE -
         *              0 : UTF8
         *              1 : GBK
         *              2 : BIG5
         *              3 : WESTERN
         *              4 : TURKISH
         *              5 : CENTRAL EUROPEAN
         *              6 : GREEK
         *              7 : CYRILLIC
         *              8 : HEBREW
         *              9 : SOUTH EASTERN EUROPEAN
         *             10 : JAPANESE
         *             11 : KOREAN
         *             12 : THAI
         *             13 : ARABIC
         *           1000 : AUTO DETECT
        */

        int encodeMapping = 0;
        if(index==0) { // Auto
            encodeMapping=1000; // Auto
        }
        else if(index>=1 && index<=14) {
            encodeMapping=index-1;
        }
        return encodeMapping;
    }

    private int getSubtitleColorMapping(int index)
    {
        int colorMapping = 0;
        switch(index)
        {
            case 0: // WHITE
                colorMapping=0; // WHITE
                break;
            case 1: // BLACK
                colorMapping=1; // BLACK
                break;
            case 2: // RED
                colorMapping=2; // RED
                break;
            case 3: // GREEN
                colorMapping=3; // GREEN
                break;
            case 4: // BLUE
                colorMapping=4; // BLUE
                break;
            case 5: // YELLOW
                colorMapping=5; // YELLOW
                break;
            default:
                break;
        }
        return colorMapping;
    }

    private void checkIsTextSubtitle()
    {
        if(mSubIndex==0)
            mIsTextSubtitle = MoviePlayer.SUBTITLE_NON_TEXT;
        else
            mIsTextSubtitle = mPlayer.IsTextSubtitle(mSubIndex-1);
        if(mIsTextSubtitle==MoviePlayer.SUBTITLE_TEXT) {
            for (int i = 1; i < mSubMenuGroup.length; i++) {
                mSubMenuGroup[i].setAlpha(0.9f);
            }
        }
        else if(mIsTextSubtitle==MoviePlayer.SUBTITLE_OFFSET) {
            for (int i = 1; i < mSubMenuGroup.length; i++) {
                if(mSubMenuGroup[i] == mSubOffsetMenu)
                    mSubMenuGroup[i].setAlpha(0.9f);
                else
                    mSubMenuGroup[i].setAlpha(0.7f);
            }
        }
        else {
            for (int i = 1; i < mSubMenuGroup.length; i++) {
                mSubMenuGroup[i].setAlpha(0.7f);
            }
        }
    }

    private void setSubMenuGroupFocus(int focusIdx)
    {
        if(mIsTextSubtitle==MoviePlayer.SUBTITLE_NON_TEXT && mCurrentSubFocus == 0) //subtitle off
            return;
        if(mCurrentSubFocus==focusIdx)
            return;

        int preFocus = mCurrentSubFocus;
        mCurrentSubFocus = focusIdx;

        if (mSubMenuGroup[mCurrentSubFocus] == mSubSyncMenu){
            mStringBackImg.setVisibility(View.VISIBLE);
            mStringAddImg.setVisibility(View.VISIBLE);
            mBackHint.setVisibility(View.VISIBLE);
            mAddHint.setVisibility(View.VISIBLE);
        }
        else if (mSubMenuGroup[preFocus] == mSubSyncMenu){
            mStringBackImg.setVisibility(View.INVISIBLE);
            mStringAddImg.setVisibility(View.INVISIBLE);
            mBackHint.setVisibility(View.INVISIBLE);
            mAddHint.setVisibility(View.INVISIBLE);
        }

        mStringUpkey.dismiss();
        mStringDownkey.dismiss();
        mStringUpkey.setWidth(mSubMenuGroup[mCurrentSubFocus].getWidth());
        mStringDownkey.setWidth(mSubMenuGroup[mCurrentSubFocus].getWidth());

        mSubMenuGroup[preFocus].requestFocusFromTouch();
        mSubMenuGroup[preFocus].setFocusable(false);
        mSubMenuGroup[preFocus].update(true);

        mSubMenuGroup[mCurrentSubFocus].requestFocusFromTouch();
        mSubMenuGroup[mCurrentSubFocus].setFocusable(true);
        mSubMenuGroup[mCurrentSubFocus].update(true);

        mStringUpkey.showAtLocation(mStringUpView, Gravity.LEFT| Gravity.TOP, mSubMenuGroup[mCurrentSubFocus].m_x,
            (int)getResources().getDimension(R.dimen.video_subtitle_menu_margin_top) - (int)getResources().getDimension(R.dimen.video_subtitle_updown_key_height));
        mStringDownkey.showAtLocation(mStringDownView, Gravity.LEFT| Gravity.TOP, mSubMenuGroup[mCurrentSubFocus].m_x,
            (int)getResources().getDimension(R.dimen.video_subtitle_menu_margin_top) + (int)getResources().getDimension(R.dimen.video_subtitle_menu_height));
    }

    private void setSubMenuGroupItem()
    {
        if( mCurrentSubFocus == 0 ) { //Subtitle index
            if(mSubMenuGroup[mCurrentSubFocus].getSelectedItemPosition()!=mSubIndex)
                mSubMenuGroup[mCurrentSubFocus].setSelectedItem(mSubIndex);
            checkIsTextSubtitle();
        }
        else if (mCurrentSubFocus == 1) { //Offset
            mSubMenuGroup[mCurrentSubFocus].setSelectedItem(0);
            mSubMenuGroup[1].modifySelectionList(0, Integer.toString(mSubOffset));
        }
        else if (mCurrentSubFocus == 2) { //Size
            mSubMenuGroup[mCurrentSubFocus].setSelectedItem(0);
            mSubMenuGroup[2].modifySelectionList(0, Integer.toString(mSubSize));
        }
        else if (mCurrentSubFocus == 3) { //Sync
            mSubMenuGroup[mCurrentSubFocus].setSelectedItem(0);
            mSubSync = mSubStringTime.floatValue();
            mSubMenuGroup[3].modifySelectionList(0, mSubStringTime.toString());
        }
        else if (mCurrentSubFocus == 4) { //Text encode
            if(mSubMenuGroup[mCurrentSubFocus].getSelectedItemPosition()!=mSubEncode)
                mSubMenuGroup[mCurrentSubFocus].setSelectedItem(mSubEncode);
        }
        else if (mCurrentSubFocus == 5) { //Text color
            if(mSubMenuGroup[mCurrentSubFocus].getSelectedItemPosition()!=mSubColor)
                mSubMenuGroup[mCurrentSubFocus].setSelectedItem(mSubColor);
        }
    }

    private void subMenuUpDown(int keyCode, boolean isMouseEvent)
    {
        if (keyCode == KeyEvent.KEYCODE_DPAD_UP ) {
            if( mCurrentSubFocus == 0 ) { //Subtitle index
                if(isMouseEvent)
                    mSubIndex = (mSubIndex>0)? mSubIndex-1:0;
                else
                    mSubIndex = mSubMenuGroup[mCurrentSubFocus].getSelectedItemPosition();
            }
            else if (mCurrentSubFocus == 1) { //Offset
                mSubOffset= (mSubOffset < 640) ? mSubOffset + 10 : mSubOffset;
            }
            else if (mCurrentSubFocus == 2) { //Size
                mSubSize = (mSubSize < 40 ) ? mSubSize + 1 : mSubSize;
            }
            else if (mCurrentSubFocus == 3) { //Sync
                BigDecimal diff = new BigDecimal("0.1");
                mSubStringTime = mSubStringTime.add(diff);
            }
            else if (mCurrentSubFocus == 4) { //Text encode
                if(isMouseEvent)
                    mSubEncode = (mSubEncode>0)? mSubEncode-1:0;
                else
                    mSubEncode = mSubMenuGroup[mCurrentSubFocus].getSelectedItemPosition();
            }
            else if (mCurrentSubFocus == 5) { //Text color
                if(isMouseEvent)
                    mSubColor = (mSubColor>0)? mSubColor-1:0;
                else
                    mSubColor = mSubMenuGroup[mCurrentSubFocus].getSelectedItemPosition();
            }
            setSubMenuGroupItem();
        }
        else if (keyCode == KeyEvent.KEYCODE_DPAD_DOWN) {
            if( mCurrentSubFocus == 0 ) { //Subtitle index
                if(isMouseEvent)
                    mSubIndex = (mSubIndex<mSubMenuGroup[mCurrentSubFocus].getCount()-1)? mSubIndex+1:mSubMenuGroup[mCurrentSubFocus].getCount()-1;
                else
                    mSubIndex = mSubMenuGroup[mCurrentSubFocus].getSelectedItemPosition();
            }
            else if (mCurrentSubFocus == 1) { //Offset
                mSubOffset = (mSubOffset > -80) ? mSubOffset - 10 : mSubOffset;
            }
            else if (mCurrentSubFocus == 2) { //Size
                mSubSize = (mSubSize > 3 ) ? mSubSize - 1 : mSubSize;
            }
            else if (mCurrentSubFocus == 3) { //Sync
                BigDecimal diff = new BigDecimal("0.1") ;
                mSubStringTime = mSubStringTime.subtract(diff);
            }
            else if (mCurrentSubFocus == 4) { //Text encode
                if(isMouseEvent)
                    mSubEncode = (mSubEncode<mSubMenuGroup[mCurrentSubFocus].getCount()-1)? mSubEncode+1:mSubMenuGroup[mCurrentSubFocus].getCount()-1;
                else
                    mSubEncode = mSubMenuGroup[mCurrentSubFocus].getSelectedItemPosition();
            }
            else if (mCurrentSubFocus == 5) { //Text color
                if(isMouseEvent)
                    mSubColor = (mSubColor<mSubMenuGroup[mCurrentSubFocus].getCount()-1)? mSubColor+1:mSubMenuGroup[mCurrentSubFocus].getCount()-1;
                else
                    mSubColor = mSubMenuGroup[mCurrentSubFocus].getSelectedItemPosition();
            }
            setSubMenuGroupItem();
        }
        else if (keyCode == KeyEvent.KEYCODE_MOVE_END || keyCode == KeyEvent.KEYCODE_MOVE_HOME) {
            if( mCurrentSubFocus == 0 ) { //Subtitle index
                mSubIndex = mSubMenuGroup[mCurrentSubFocus].getSelectedItemPosition();
            }
            else if (mCurrentSubFocus == 4) { //Text encode
                mSubEncode = mSubMenuGroup[mCurrentSubFocus].getSelectedItemPosition();
            }
            else if (mCurrentSubFocus == 5) { //Text color
                mSubColor = mSubMenuGroup[mCurrentSubFocus].getSelectedItemPosition();
            }
            setSubMenuGroupItem();
        }
    }

    public void setSubtitleInfo() {
        int subEnable=0;
        if(mSubIndex>0) {
            subEnable = 1;
        }
        mSubIndex = mPlayer.setSubtitleInfo(mSubIndex, subEnable, getSubtitleEncodeMapping(mSubEncode), getSubtitleColorMapping(mSubColor), mSubSize, (int)(mSubSync*1000), -mSubOffset);
        mUri = mPlayer.getPlayingUri();
        setSubtitleIndexPref(mUri.toString(), mSubIndex);
    }

    private class SubMenuItemOnKeyListener implements OnKeyListener
    {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
	   	{	
            int action = event.getAction();
            if (action == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT || keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
                    int focusIdx=0;
                    if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT) {
                        if(mIsTextSubtitle==MoviePlayer.SUBTITLE_OFFSET)
                            focusIdx = (mCurrentSubFocus == 0 ) ? 1 : 0;
                        else
                            focusIdx = (mCurrentSubFocus == 0 ) ? (mSubMenuGroup.length -1) : (mCurrentSubFocus - 1) ;
                    }
                    else if (keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
                        if(mIsTextSubtitle==MoviePlayer.SUBTITLE_OFFSET)
                            focusIdx = (mCurrentSubFocus == 0 ) ? 1 : 0;
                        else
                            focusIdx = (mCurrentSubFocus + 1) % mSubMenuGroup.length;
                    }
                    setSubMenuGroupFocus(focusIdx);
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE ||
                        keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                    if(mSubMenuGroup[0]!=null)
                        mSubMenuGroup[0].Hide();
                    //mPlayer.onPlayPause(false, false);
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_DPAD_UP) {
                    subMenuUpDown(keyCode, false);
                    return false;
                }
                else if (keyCode == KeyEvent.KEYCODE_DPAD_DOWN) {
                    subMenuUpDown(keyCode, false);
                    return false;
                }
                else if (keyCode == KeyEvent.KEYCODE_MEDIA_FAST_FORWARD || 
                    keyCode == KeyEvent.KEYCODE_F){
                    if (mCurrentSubFocus == 3) { //String
                        BigDecimal diff = new BigDecimal("0.1");
                        mSubStringTime = mSubStringTime.add(diff);
                        setSubMenuGroupItem();
                    }
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_MEDIA_REWIND || 
                    keyCode == KeyEvent.KEYCODE_R){
                    if (mCurrentSubFocus == 3) {//String
                        BigDecimal diff = new BigDecimal("0.1");
                        mSubStringTime = mSubStringTime.subtract(diff);
                        setSubMenuGroupItem();
                    }
                    return true;
                }            
            }
            else if (action == KeyEvent.ACTION_UP) {
                if (keyCode == KeyEvent.KEYCODE_DPAD_UP) {
                    if( mCurrentSubFocus == 0 || mCurrentSubFocus == 4 || mCurrentSubFocus == 5)
                        subMenuUpDown(keyCode, false);
                    if(mCurrentSubFocus==0) {
                        mHandler.removeCallbacks(mSetSubtitleInfo);
                        mHandler.postDelayed(mSetSubtitleInfo, 1000);
                    }
                    else {
                        setSubtitleInfo();
                    }
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_DPAD_DOWN) {
                    if( mCurrentSubFocus == 0 || mCurrentSubFocus == 4 || mCurrentSubFocus == 5)
                        subMenuUpDown(keyCode, false);
                    if(mCurrentSubFocus==0) {
                        mHandler.removeCallbacks(mSetSubtitleInfo);
                        mHandler.postDelayed(mSetSubtitleInfo, 1000);
                    }
                    else {
                        setSubtitleInfo();
                    }
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_MEDIA_FAST_FORWARD || keyCode == KeyEvent.KEYCODE_F) {
                    setSubtitleInfo();
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_MEDIA_REWIND || keyCode == KeyEvent.KEYCODE_R) {
                    setSubtitleInfo();
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_MOVE_END || keyCode == KeyEvent.KEYCODE_MOVE_HOME) {
                    subMenuUpDown(keyCode, false);
                    setSubtitleInfo();
                    return true;
                }
            }
            return false;
        }
    }

    private int getSubMenuGroupIndex(float rawX, float rawY)
    {
        for (int i = 0; i < mSubMenuGroup.length; i++) {
            if( rawX >= mSubMenuGroup[i].m_x && rawX < mSubMenuGroup[i].m_x + mSubMenuGroup[i].getWidth() &&
                rawY >= mSubMenuGroup[i].m_y && rawY < mSubMenuGroup[i].m_y + mSubMenuGroup[i].getHeight()) {
                Log.d(TAG, "getSubMenuGroupIndex = "+i);
                return i;
            }
        }
        return -1;
    }

    private class SubMenuItemClickListener implements OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id)
        {
            Log.d(TAG, "Subtitle menu ("+position+") -- " + mSubMenuGroup[mCurrentSubFocus].getItemof(position)+" is clicked");
            mSubMenuGroup[mCurrentSubFocus].requestFocusFromTouch();
            if( mCurrentSubFocus == 0 ) { //Subtitle index
                mSubIndex = position;
            }
            else if (mCurrentSubFocus == 4) { //Text encode
                mSubEncode = position;
            }
            else if (mCurrentSubFocus == 5) { //Text color
                mSubColor = position;
            }
            setSubMenuGroupItem();
            setSubtitleInfo();
        }
    }

    private class SubMenuItemOnTouchListener implements View.OnTouchListener {
        @Override
        public boolean onTouch(View view, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                if(view==mStringUpView) {
                    subMenuUpDown(KeyEvent.KEYCODE_DPAD_UP, true);
                }
                else if(view==mStringDownView) {
                    subMenuUpDown(KeyEvent.KEYCODE_DPAD_DOWN, true);
                }
                else {
                    int subMenuGroupIndex = getSubMenuGroupIndex(event.getRawX(), event.getRawY());
                    if(subMenuGroupIndex<0) {   //Outside touch event
                        if(mSubMenuGroup[0]!=null)
                            mSubMenuGroup[0].Hide();
                    }
                    else {
                        if(mCurrentSubFocus==subMenuGroupIndex) {
                            mSubMenuGroup[mCurrentSubFocus].requestFocusFromTouch();
                            return false;
                        }
                        else {
                            setSubMenuGroupFocus(subMenuGroupIndex);
                        }
                    }
                }
                return true;
            }
            else if (event.getAction() == MotionEvent.ACTION_UP) {
                if(view==mStringUpView) {
                    setSubtitleInfo();
                }
                else if(view==mStringDownView) {
                    setSubtitleInfo();
                }
            }
            else if (event.getAction() == MotionEvent.ACTION_OUTSIDE) {
                return true;
            }
            return false;
        }
    }

    private class SubMenuOnDismissListener implements PopupWindow.OnDismissListener {
        @Override
        public void onDismiss (){
            Log.d(TAG, "Subtitle menu - OnDismiss");
            if(mPlayer!=null) {
                mPlayer.freshHideSystemUi();
            }
            mPlayer.disableABRepeat();
            if(mStringUpkey!=null)
                mStringUpkey.dismiss();
            if(mStringDownkey!=null)
                mStringDownkey.dismiss();
            for (int i = 0; i < mSubMenuGroup.length; i++) {
                if(mSubMenuGroup[i]!=null)
                    mSubMenuGroup[i].Hide();
            }
        }
    }

    private void reset3DMode() {
        if(mPlayer.get3DEnable()) {
            if(mPlayer.get3DType()==MoviePlayer.MODE_3D_MVC) {
                mPlayer.setMVC(false);
            }
            mPlayer.setFormat3d(RtkVoutUtilManager.FORMAT_3D_NULL);
        }
        m3DFocus = 0;
        m3DSubFocus = 0;
        set3DMode(m3DFocus, false);
        set3DSubMode(m3DSubFocus, false);
    }

    private void set3DMode(int index, boolean enable)
    {
        if(mPlayer.get3DType()==MoviePlayer.MODE_3D_NOT_MVC) {
            if(m3DMenuGroup==null || index>=m3DMenuGroup.length)
                return;
            switch(index) {
                case 0:     //sbs
                    if(enable) {
                        mPlayer.setFormat3d(RtkVoutUtilManager.FORMAT_3D_SIDE_BY_SIDE);
                        m3DMode = RtkVoutUtilManager.FORMAT_3D_SIDE_BY_SIDE;
                    }
                    m3DMenuGroup[0].setImageResource(R.drawable.sub_sbs_side);
                    m3DMenuGroup[1].setImageResource(R.drawable.sub_tb_top_none);
                    break;
                case 1:     //tb
                    if(enable) {
                        mPlayer.setFormat3d(RtkVoutUtilManager.FORMAT_3D_TOP_AND_BOTTOM);
                        m3DMode = RtkVoutUtilManager.FORMAT_3D_TOP_AND_BOTTOM;
                    }
                    m3DMenuGroup[0].setImageResource(R.drawable.sub_sbs_side_none);
                    m3DMenuGroup[1].setImageResource(R.drawable.sub_tb_top);
                    break;
                default:
                    break;
            }
            Log.d(TAG, "set3DMode to "+index+", m3DEnable ="+mPlayer.get3DEnable());
        }
    }

    private class Menu3DItemOnKeyListener implements OnKeyListener
    {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
	   	{	
            int action = event.getAction();
            if (action == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT || keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
                    if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT) {
                        m3DFocus = (m3DFocus == 0 ) ? (m3DMenuGroup.length -1) : (m3DFocus - 1) ;
                    }
                    else if (keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
                        m3DFocus = (m3DFocus + 1) % m3DMenuGroup.length;
                    }
                    set3DMode(m3DFocus, false);
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK ||
                        keyCode == KeyEvent.KEYCODE_ESCAPE || keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                    if(keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                        set3DMode(m3DFocus, true);
                    }
                    m3DOption.setOutsideTouchable(false);
                    m3DOption.dismiss();

                    if(mPlayer.get3DEnable() == true)
                    {
                        New3DSubMenu();
                        m3DSubOption.showAtLocation(m3DSubView, Gravity.CENTER, 0, 0);
                        m3DSubOption.setFocusable(true);
                        m3DSubOption.setOutsideTouchable(true);
                        m3DSubOption.update();
                    }
                    return true;
                }
            }
            return false;
        }
       
    }

    private class Menu3DItemOnTouchListener implements View.OnTouchListener {

        @Override
        public boolean onTouch(View view, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                if(view==m3DMenuGroup[0]) {
                    m3DFocus=0;
                    set3DMode(m3DFocus, true);
                }
                else if(view==m3DMenuGroup[1]) {
                    m3DFocus=1;
                    set3DMode(m3DFocus, true);
                }
                return true;
            }
            else if (event.getAction() == MotionEvent.ACTION_UP) {
                if (m3DOption != null && m3DOption.isShowing()) {
                    m3DOption.dismiss();
                }
                if(mPlayer.get3DEnable() == true)
                {
                    New3DSubMenu();
                    m3DSubOption.showAtLocation(m3DSubView, Gravity.CENTER, 0, 0);
                    m3DSubOption.setFocusable(true);
                    m3DSubOption.setOutsideTouchable(true);
                    m3DSubOption.update();
                }
                return true;
            }
            return true;
        }
    }

    private void set3Dto2DMode(int index, boolean enable)
    {
        if(m3Dto2DMenuGroup==null || index>=m3Dto2DMenuGroup.length)
            return;
        switch(index) {
            case 0:     // sbs
                if(enable) {
                    mPlayer.set3Dto2D(1);
                    m3Dto2DMode = 1;
                }
                m3Dto2DMenuGroup[0].setImageResource(R.drawable.sub_to2d_side);
                m3Dto2DMenuGroup[1].setImageResource(R.drawable.sub_to2d_top_none);
                break;
            case 1:     // tb
                if(enable) {
                    mPlayer.set3Dto2D(2);
                    m3Dto2DMode = 2;
                }
                m3Dto2DMenuGroup[0].setImageResource(R.drawable.sub_to2d_side_none);
                m3Dto2DMenuGroup[1].setImageResource(R.drawable.sub_to2d_top);
                break;
            default:     // 2D
                if(enable) {
                    mPlayer.set3Dto2D(0);
                    m3Dto2DMode = 0;
                }
                m3Dto2DFocus = 0;
                m3DSubFocus = 0;
                set3Dto2DMode(m3Dto2DFocus, false);
                set3DSubMode(m3DSubFocus, false);
                break;
        }
    }

    private class Menu3Dto2DItemOnKeyListener implements OnKeyListener
    {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            int action = event.getAction();
            if (action == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT || keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
                    if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT) {
                        m3Dto2DFocus = (m3Dto2DFocus == 0 ) ? (m3Dto2DMenuGroup.length -1) : (m3Dto2DFocus - 1) ;
                    }
                    else if (keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
                        m3Dto2DFocus = (m3Dto2DFocus + 1) % m3Dto2DMenuGroup.length;
                    }
                    set3Dto2DMode(m3Dto2DFocus, false);
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK ||
                        keyCode == KeyEvent.KEYCODE_ESCAPE || keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                    if(keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                        set3Dto2DMode(m3Dto2DFocus, true);
                    }
                    if (m3Dto2DOption != null && m3Dto2DOption.isShowing()) {
                        m3Dto2DOption.setOutsideTouchable(false);
                        m3Dto2DOption.dismiss();
                    }
                    if(m3Dto2DMode > 0)
                    {
                        New3DSubMenu();
                        m3DSubOption.showAtLocation(m3DSubView, Gravity.CENTER, 0, 0);
                        m3DSubOption.setFocusable(true);
                        m3DSubOption.setOutsideTouchable(true);
                        m3DSubOption.update();
                    }
                    return true;
                }
            }
            return false;
        }
    }

    private class Menu3Dto2DItemOnTouchListener implements View.OnTouchListener {

        @Override
        public boolean onTouch(View view, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                if(view==m3Dto2DMenuGroup[0]) {
                    m3Dto2DFocus=0;
                    set3Dto2DMode(m3Dto2DFocus, true);
                }
                else if(view==m3Dto2DMenuGroup[1]) {
                    m3Dto2DFocus=1;
                    set3Dto2DMode(m3Dto2DFocus, true);
                }
                return true;
            }
            else if (event.getAction() == MotionEvent.ACTION_UP) {
                if (m3Dto2DOption != null && m3Dto2DOption.isShowing()) {
                    m3Dto2DOption.dismiss();
                }
                if(m3Dto2DMode > 0)
                {
                    New3DSubMenu();
                    m3DSubOption.showAtLocation(m3DSubView, Gravity.CENTER, 0, 0);
                    m3DSubOption.setFocusable(true);
                    m3DSubOption.setOutsideTouchable(true);
                    m3DSubOption.update();
                }
                return true;
            }
            return true;
        }
    }

    private void set3DSubMode(int index, boolean enable)
    {
        if(m3DSubMenuGroup==null || index>=m3DSubMenuGroup.length)
            return;
        switch(index) {
            case 0:     //sbs or top-bottom
                if(enable) {
                    m3DSubMode = RtkVoutUtilManager.FORMAT_3D_SUB_INTERNAL;
                    mPlayer.set3dSub(m3DSubMode);
                }
                if(m3DMode == RtkVoutUtilManager.FORMAT_3D_SIDE_BY_SIDE || m3Dto2DMode == 1)
                {
                    m3DSubMenuGroup[0].setImageResource(R.drawable.sub_sbs1_side);
                    m3DSubMenuGroup[1].setImageResource(R.drawable.sub_center_none);
                }
                else
                {
                    m3DSubMenuGroup[0].setImageResource(R.drawable.sub_tb1_top);
                    m3DSubMenuGroup[1].setImageResource(R.drawable.sub_tb1_down_none);
                }
                break;
            case 1:     //center or bottom
                if(enable) {
                    m3DSubMode = RtkVoutUtilManager.FORMAT_3D_SUB_EXTERNAL;
                    mPlayer.set3dSub(m3DSubMode);
                }
                if(m3DMode == RtkVoutUtilManager.FORMAT_3D_SIDE_BY_SIDE || m3Dto2DMode == 1)
                {
                    m3DSubMenuGroup[0].setImageResource(R.drawable.sub_sbs1_side_none);
                    m3DSubMenuGroup[1].setImageResource(R.drawable.sub_center);
                }
                else
                {
                    m3DSubMenuGroup[0].setImageResource(R.drawable.sub_tb1_top_none);
                    m3DSubMenuGroup[1].setImageResource(R.drawable.sub_tb1_down);
                }
                break;
            default:
                break;
        }
        Log.d(TAG, "set3DSubMode to "+index);
    }
    private class Menu3DSubItemOnKeyListener implements OnKeyListener
    {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            int action = event.getAction();
            if (action == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT || keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
                    if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT) {
                        m3DSubFocus = (m3DSubFocus == 0 ) ? (m3DSubMenuGroup.length -1) : (m3DSubFocus - 1) ;
                    }
                    else if (keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
                        m3DSubFocus = (m3DSubFocus + 1) % m3DSubMenuGroup.length;
                    }
                    set3DSubMode(m3DSubFocus, false);
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK ||
                        keyCode == KeyEvent.KEYCODE_ESCAPE || keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                    if(keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                        set3DSubMode(m3DSubFocus, true);
                    }
                    m3DSubOption.setOutsideTouchable(false);
                    m3DSubOption.dismiss();
                    return true;
                }
            }
            return false;
        }

    }

    private class Menu3DSubItemOnTouchListener implements View.OnTouchListener {

        @Override
        public boolean onTouch(View view, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                if(view==m3DSubMenuGroup[0]) {
                    m3DSubFocus=0;
                    set3DSubMode(m3DSubFocus, true);
                }
                else if(view==m3DSubMenuGroup[1]) {
                    m3DSubFocus=1;
                    set3DSubMode(m3DSubFocus, true);
                }
                return true;
            }
            else if (event.getAction() == MotionEvent.ACTION_UP) {
                if (m3DSubOption != null && m3DSubOption.isShowing()) {
                    m3DSubOption.dismiss();
                }
                return true;
            }
            return true;
        }
    }

    public void setAudioInfo() {
        mPlayer.setAudioTrack(mAudioIndex);
    }

    public int getAudioIndex() {
        return mAudioIndex;
    }

    private class AudioMenuItemOnKeyListener implements OnKeyListener {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            int action = event.getAction();
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE) {
                    mAudioMenu.Hide();
                    return true;
                }
            }
            else if(event.getAction() == KeyEvent.ACTION_UP) {
                if (keyCode == KeyEvent.KEYCODE_DPAD_UP || keyCode == KeyEvent.KEYCODE_DPAD_DOWN) {
                    mAudioIndex = mAudioMenu.getSelectedItemPosition();
                    setAudioInfo();
                    return true;
                }
            }
            return false;
        }
    }


    private class AudioMenuItemClickListener implements OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id)
        {
            Log.d(TAG, "Audio menu --" + mAudioMenu.getItemof(position)+" is clicked");
            if(position<mAudioMenu.getCount()) {
                mAudioMenu.requestFocusFromTouch();
                mAudioMenu.setSelectedItem(position);
                mAudioIndex = position;
                setAudioInfo();
            }
        }
    }

    private class MenuOnDismissListener implements PopupWindow.OnDismissListener {
        @Override
        public void onDismiss (){
            if(mPlayer!=null) {
                mPlayer.freshHideSystemUi();
            }
        }
    }

    private void setARRescaleModeBySetting()
    {
        try {
            int aspectRatio = (Integer) Settings.System.getInt(getContentResolver(), Settings.System.REALTEK_SETUP_ASPECT_RATIO);
            if(mRtkVoutUtilManager != null) {
                mRtkVoutUtilManager.setRescaleMode(aspectRatio);
            }
            switch(aspectRatio) {
                case 0:     //FIT_TO_SCREEN
                    mARFocus=0;
                    break;
                case 5:     //FULL_TO_SCREEN
                    mARFocus=1;
                    break;
                default:
                    break;
            }
        } catch (SettingNotFoundException snfe) {
            Log.d(TAG, "Exception found in setARRescaleModeBySetting()");
        }
    }

    private void processARCommand(){
        Log.d("Gallery2","\033[0;32;32m processARCommand \033[m");
        resetZoom();
        mPlayer.relayoutVideoView();
    }

    private void setARRescaleMode(int index)
    {
        if(index>=mARMenuGroup.length)
            return;
        switch(index) {
            case AR_FIT_INDEX:     //FIT_TO_SCREEN
                mARMenuGroup[AR_FIT_INDEX].setImageResource(R.drawable.asp_fit);
                mARMenuGroup[AR_FULL_INDEX].setImageResource(R.drawable.asp_full_none);
                break;
            case AR_FULL_INDEX:     //FULL_TO_SCREEN
                mARMenuGroup[AR_FIT_INDEX].setImageResource(R.drawable.asp_fit_none);
                mARMenuGroup[AR_FULL_INDEX].setImageResource(R.drawable.asp_full);
                break;
            default:
                break;
        }
    }

    private class ARItemOnKeyListener implements OnKeyListener
    {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
	   	{	
            int action = event.getAction();
            if (action == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT || keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
                    if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT) {
                        mARFocus = (mARFocus == 0 ) ? (mARMenuGroup.length -1) : (mARFocus - 1) ;
                        setARRescaleMode(mARFocus);
                        processARCommand();
                    }
                    else if (keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
                        mARFocus = (mARFocus + 1) % mARMenuGroup.length;
                        setARRescaleMode(mARFocus);
                        processARCommand();
                    }
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK ||
                        keyCode == KeyEvent.KEYCODE_ESCAPE || keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                    mAROption.setOutsideTouchable(false);
                    mAROption.dismiss();
                    return true;
                }
            }
             return false;
        }
       
    }

    private class ARItemOnTouchListener implements View.OnTouchListener {

        @Override
        public boolean onTouch(View view, MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                if(view==mARMenuGroup[0]) {
                    mARFocus=0;
                    setARRescaleMode(mARFocus);
                }
                else if(view==mARMenuGroup[1]) {
                    mARFocus=1;
                    setARRescaleMode(mARFocus);
                }
                processARCommand();
                return true;
            }
            else if (event.getAction() == MotionEvent.ACTION_UP) {
                setARRescaleMode(mARFocus);
                if (mAROption != null && mAROption.isShowing()) {
                    mAROption.dismiss();
                }
                return true;
            }
            return true;
        }
    }

    private void NewOptionMenu() {
        View ContentView = getLayoutInflater().inflate(R.layout.optionmenu,null); 
        Display display = getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int w = getResources().getDimensionPixelSize(R.dimen.option_menu_width);
        int h = getResources().getDimensionPixelSize(R.dimen.option_menu_height);
        final int x = screenSize.x-w;
        final int y = (screenSize.y/2)-(h/2);
        mOptionMenu= new RTKOptionMenu(x, y, w, h,
            getResources().getXml(R.xml.video_playback_option), ContentView, mPlayer);
        mOptionMenu.setItemClickListener(new OptionMenuItemClickListener());
        mOptionMenu.setOnKeyListener(new OptionMenuItemOnKeyListener());
    }

    private void UpdateSubtitleList(){
        ArrayList<String> list = new ArrayList<String>();
        list.add(new String("OFF"));
        int subtitleCount = mPlayer.getSubtitleCount();
        for(int count=0;count<subtitleCount;count++) {
            list.add(mPlayer.getSubtitleInfoDisplay(count));
        }
        mSubMainMenu.setSelectionList(list);
    }

    public boolean isSubtitleMenuShowing() {
        return (mSubMainMenu==null)? false:mSubMainMenu.isShowing();
    }

    private void getTextEncodeSetup()
    {
        String res = Settings.System.getString(getContentResolver(), Settings.System.REALTEK_SETUP_TEXT_ENCODING_MODE);
        mSubEncode = 0;

        if(res.equalsIgnoreCase("utf8")){
            mSubEncode = 1;
        }else if(res.equalsIgnoreCase("gbk")){
            mSubEncode = 2;
        }else if(res.equalsIgnoreCase("big5")){
            mSubEncode = 3;
        }
        else {
            mSubEncode = 0;
        }
        Log.d(TAG, "getTextEncodeSetup: " + mSubEncode);
    }

    private void UpdateAudioList(){
        ArrayList<String> list = new ArrayList<String>();
        int audioCount = mPlayer.getAudioCount(true);
        for(int count=0;count<audioCount;count++) {
            list.add(mPlayer.getAudioInfoDisplay(count,true));
        }
        mAudioMenu.setSelectionList(list);
    }

    private void NewSubtitleMenu() {
        View ContentView = getLayoutInflater().inflate(R.layout.vscrollmenu, null); 
        Display display = getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int width_1 = (int)getResources().getDimension(R.dimen.video_subtitle_menu_width_1);
        int width_2 = (int)getResources().getDimension(R.dimen.video_subtitle_menu_width_2);
        int height = (int)getResources().getDimension(R.dimen.video_subtitle_menu_height);
        int margin = getResources().getDimensionPixelSize(R.dimen.video_subtitle_menu_margin_right);
        int x = screenSize.x-(width_1*3 + width_2*3)-margin;
        final int y = getResources().getDimensionPixelSize(R.dimen.video_subtitle_menu_margin_top);

        mSubMainMenu= new RTKVScrollMenu(x, y, width_1, height,
            "Subtitle", null, ContentView);
        ArrayList<String> list = new ArrayList<String>();
        mSubMenuGroup[0] = mSubMainMenu;
        
        ContentView = getLayoutInflater().inflate(R.layout.vscrollmenu_number, null); 
        x += width_1;
        mSubOffsetMenu= new RTKVScrollMenu(x, y, width_2, height,
            "Offset", null, ContentView);
        list = new ArrayList<String>();
        list.add(Integer.toString(mSubOffset));
        mSubOffsetMenu.setSelectionList(list);
        mSubMenuGroup[1] = mSubOffsetMenu;
        
        ContentView = getLayoutInflater().inflate(R.layout.vscrollmenu_number, null); 
        x += width_2;
        mSubSizeMenu= new RTKVScrollMenu(x, y, width_2, height,
            "Size", null, ContentView);
        list = new ArrayList<String>();
        list.add(Integer.toString(mSubSize));
        mSubSizeMenu.setSelectionList(list);
        mSubMenuGroup[2] = mSubSizeMenu;
        
        ContentView = getLayoutInflater().inflate(R.layout.vscrollmenu_string, null); 
        x += width_2;
        mSubSyncMenu= new RTKVScrollMenu(x, y, width_1, height,
            "Sync", getResources().getXml(R.xml.sub_number_option), ContentView);
        mStringBackImg = (ImageView)ContentView.findViewById(R.id.sub_back);
        mStringAddImg = (ImageView)ContentView.findViewById(R.id.sub_add);
        mBackHint = (TextView)ContentView.findViewById(R.id.back_hint);
        mAddHint = (TextView)ContentView.findViewById(R.id.add_hint);
        mBackHint.setLines(2);
        mBackHint.setText("(-)\n0.1Sec");
        mAddHint.setLines(2);
        mAddHint.setText("      (+)\n0.1Sec");
        mSubStringTime = new BigDecimal(Float.toString(mSubSync));
        mSubStringTime.setScale(1);
        mSubSyncMenu.modifySelectionList(0, mSubStringTime.toString());
        mSubMenuGroup[3] = mSubSyncMenu;

        ContentView = getLayoutInflater().inflate(R.layout.vscrollmenu, null); 
        x += width_1;
        mSubEncodeMenu= new RTKVScrollMenu(x, y, width_1, height,
            "Text Encoding", getResources().getXml(R.xml.sub_encode_option), ContentView);
        mSubMenuGroup[4] = mSubEncodeMenu; 
        mSubMenuGroup[4].setSelectedItem(mSubEncode);

        ContentView = getLayoutInflater().inflate(R.layout.vscrollmenu, null); 
        x += width_1;
        mSubColorMenu= new RTKVScrollMenu(x, y, width_2, height,
            "Color", getResources().getXml(R.xml.sub_color_option), ContentView);
        mSubMenuGroup[5] = mSubColorMenu; 

        for (int i = 0; i < mSubMenuGroup.length; i++) {
            mSubMenuGroup[i].setOnKeyListener(new SubMenuItemOnKeyListener());
            mSubMenuGroup[i].setTouchInterceptor(new SubMenuItemOnTouchListener());
            mSubMenuGroup[i].setItemClickListener(new SubMenuItemClickListener());
            mSubMenuGroup[i].setOnDismissListener(new SubMenuOnDismissListener());
        }
        mStringUpView= getLayoutInflater().inflate(R.layout.string_upkey,null);
        mStringUpkey = new PopupWindow(mStringUpView, 280, (int)getResources().getDimension(R.dimen.video_subtitle_updown_key_height));
        mStringDownView= getLayoutInflater().inflate(R.layout.string_downkey,null);
        mStringDownkey = new PopupWindow(mStringDownView, 280, (int)getResources().getDimension(R.dimen.video_subtitle_updown_key_height));
        mStringUpView.setOnTouchListener(new SubMenuItemOnTouchListener());
        mStringDownView.setOnTouchListener(new SubMenuItemOnTouchListener());
        mStringUpkey.setAnimationStyle(R.style.Animation_OnScreenHint);
        mStringDownkey.setAnimationStyle(R.style.Animation_OnScreenHint);
    }

    private void NewAudioMenu() {
        View ContentView = getLayoutInflater().inflate(R.layout.vscrollmenu,null);
        Display display = getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int w = getResources().getDimensionPixelSize(R.dimen.video_audio_menu_width);
        int h = getResources().getDimensionPixelSize(R.dimen.video_audio_menu_height);
        int margin = getResources().getDimensionPixelSize(R.dimen.video_audio_menu_margin_right);
        final int x = screenSize.x-w-margin;
        final int y = getResources().getDimensionPixelSize(R.dimen.video_audio_menu_margin_top);
        mAudioMenu = new RTKVScrollMenu(x, y,
            (int)getResources().getDimension(R.dimen.video_audio_menu_width),
            (int)getResources().getDimension(R.dimen.video_audio_menu_height),
            "Audio", null, ContentView);
        mAudioMenu.setOnKeyListener(new AudioMenuItemOnKeyListener());
        mAudioMenu.setItemClickListener(new AudioMenuItemClickListener());
        mAudioMenu.setOnDismissListener(new MenuOnDismissListener());
    }

    private void New3DMenu() {
        if(mPlayer.get3DType()==MoviePlayer.MODE_3D_NOT_MVC) {
            m3DView = getLayoutInflater().inflate(R.layout.option_3d_type,null);
            m3DOption = new PopupWindow(m3DView, (int)getResources().getDimension(R.dimen.video_ar_menu_width),
                                        (int)getResources().getDimension(R.dimen.video_ar_menu_height));
            m3DOption.setAnimationStyle(R.style.Animation_OnScreenHint);
            m3DMenuGroup= new ImageView[2];
            m3DView.setOnTouchListener(new Menu3DItemOnTouchListener());

            m3DMenuGroup[0] = (ImageView)m3DView.findViewById(R.id.sbs);
            m3DMenuGroup[1] = (ImageView)m3DView.findViewById(R.id.tb);

            for(int count=0;count<m3DMenuGroup.length;count++) {
                m3DMenuGroup[count].setOnKeyListener(new Menu3DItemOnKeyListener());
                m3DMenuGroup[count].setOnTouchListener(new Menu3DItemOnTouchListener());
            }
        }
        m3DOption.setOnDismissListener(new MenuOnDismissListener());
        set3DMode(m3DFocus, false);
    }

    private void New3Dto2DMenu() {
        m3Dto2DView = getLayoutInflater().inflate(R.layout.option_3d_type,null);
        m3Dto2DOption = new PopupWindow(m3Dto2DView, (int)getResources().getDimension(R.dimen.video_ar_menu_width),
                                    (int)getResources().getDimension(R.dimen.video_ar_menu_height));
        m3Dto2DOption.setAnimationStyle(R.style.Animation_OnScreenHint);
        m3Dto2DMenuGroup= new ImageView[2];
        m3Dto2DView.setOnTouchListener(new Menu3DItemOnTouchListener());

        m3Dto2DMenuGroup[0] = (ImageView)m3Dto2DView.findViewById(R.id.sbs);
        m3Dto2DMenuGroup[1] = (ImageView)m3Dto2DView.findViewById(R.id.tb);

        for(int count=0;count<m3Dto2DMenuGroup.length;count++) {
            m3Dto2DMenuGroup[count].setOnKeyListener(new Menu3Dto2DItemOnKeyListener());
            m3Dto2DMenuGroup[count].setOnTouchListener(new Menu3Dto2DItemOnTouchListener());
        }
        m3Dto2DOption.setOnDismissListener(new MenuOnDismissListener());
        set3Dto2DMode(m3Dto2DFocus, false);
    }

    private void New3DSubMenu() {
        if(m3DMode == RtkVoutUtilManager.FORMAT_3D_SIDE_BY_SIDE || m3Dto2DMode == 1)
            m3DSubView = getLayoutInflater().inflate(R.layout.option_3d_sub_type,null);
        else
            m3DSubView = getLayoutInflater().inflate(R.layout.option_3d_sub_type2,null);
        m3DSubOption = new PopupWindow(m3DSubView, (int)getResources().getDimension(R.dimen.video_ar_menu_width),
                                    (int)getResources().getDimension(R.dimen.video_ar_menu_height));
        m3DSubOption.setAnimationStyle(R.style.Animation_OnScreenHint);
        m3DSubMenuGroup= new ImageView[2];
        m3DSubView.setOnTouchListener(new Menu3DSubItemOnTouchListener());
        if(m3DMode == RtkVoutUtilManager.FORMAT_3D_SIDE_BY_SIDE || m3Dto2DMode == 1)
        {
            m3DSubMenuGroup[0] = (ImageView)m3DSubView.findViewById(R.id.sbs_sub);
            m3DSubMenuGroup[1] = (ImageView)m3DSubView.findViewById(R.id.center_sub);
        }
        else
        {
            m3DSubMenuGroup[0] = (ImageView)m3DSubView.findViewById(R.id.tb_sub);
            m3DSubMenuGroup[1] = (ImageView)m3DSubView.findViewById(R.id.bottom_sub);
        }
        for(int count=0;count<m3DSubMenuGroup.length;count++) {
            m3DSubMenuGroup[count].setOnKeyListener(new Menu3DSubItemOnKeyListener());
            m3DSubMenuGroup[count].setOnTouchListener(new Menu3DSubItemOnTouchListener());
        }
        m3DSubOption.setOnDismissListener(new MenuOnDismissListener());
        set3DSubMode(m3DSubFocus, false);
    }
    
    private void NewARMenu() {
        mARView = getLayoutInflater().inflate(R.layout.ar_option,null);
        mAROption = new PopupWindow(mARView, (int)getResources().getDimension(R.dimen.video_ar_menu_width),
                                    (int)getResources().getDimension(R.dimen.video_ar_menu_height));
        mAROption.setAnimationStyle(R.style.Animation_OnScreenHint);
        mARView.setOnTouchListener(new ARItemOnTouchListener());

        mARMenuGroup[AR_FULL_INDEX] = (ImageView)mARView.findViewById(R.id.full);
        mARMenuGroup[AR_FIT_INDEX] = (ImageView)mARView.findViewById(R.id.fit);
        mARMenuGroup[0].setOnKeyListener(new ARItemOnKeyListener());
        mARMenuGroup[1].setOnKeyListener(new ARItemOnKeyListener());
        mARMenuGroup[0].setOnTouchListener(new ARItemOnTouchListener());
        mARMenuGroup[1].setOnTouchListener(new ARItemOnTouchListener());
        mAROption.setOnDismissListener(new MenuOnDismissListener());
        setARRescaleMode(mARFocus);
    }

    private void NewBDInfoMenu() {
        View ContentView = getLayoutInflater().inflate(R.layout.vscrollmenu,null);
        Display display = getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int w = getResources().getDimensionPixelSize(R.dimen.video_audio_menu_width);
        int h = getResources().getDimensionPixelSize(R.dimen.video_audio_menu_height);
        int margin = getResources().getDimensionPixelSize(R.dimen.video_audio_menu_margin_right);
        final int x = screenSize.x-w-margin;
        final int y = getResources().getDimensionPixelSize(R.dimen.video_audio_menu_margin_top);
        mBDInfoMenu = new RTKVScrollMenu(x, y,
            (int)getResources().getDimension(R.dimen.video_audio_menu_width),
            (int)getResources().getDimension(R.dimen.video_audio_menu_height),
            getString(R.string.bd_title_info), null, ContentView);
        mBDInfoMenu.setGravity(Gravity.CENTER);
        mBDInfoMenu.setOnKeyListener(new BDInfoMenuOnKeyListener());
        mBDInfoMenu.setItemClickListener(new BDInfoMenuItemClickListener());
        mBDInfoMenu.setOnDismissListener(new MenuOnDismissListener());
        UpdateBDInfoMenu();
    }

    public void UpdateBDInfoMenu() {
        if(mBDInfoMenu!=null) {
            mBDInfoMenu.setSelectionList(mPlayer.getBDTitleList());
        }
    }

    private class BDInfoMenuItemClickListener implements OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id)
        {
            Log.d(TAG, "BDInfo menu --" + mBDInfoMenu.getItemof(position)+" is clicked");
            if(position<mBDInfoMenu.getCount()) {
                mBDInfoMenu.requestFocusFromTouch();
                mBDInfoMenu.setSelectedItem(position);
                setBDTitleInfo(position);
                mBDInfoMenu.Hide();
            }
        }
    }

    private class BDInfoMenuOnKeyListener implements AdapterView.OnKeyListener {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE) {
                    mBDInfoMenu.Hide();
                    return true;
                }
            }
            return false;
        }
    }

    public void setBDTitleInfo(int position) {
        Log.d(TAG, "setBDTitleInfo: " + position+1);
        mPlayer.setBDTitle(position+1);
    }

    private void NewBDChapterInfoMenu() {
        View ContentView = getLayoutInflater().inflate(R.layout.vscrollmenu,null);
        Display display = getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int w = getResources().getDimensionPixelSize(R.dimen.video_audio_menu_width);
        int h = getResources().getDimensionPixelSize(R.dimen.video_audio_menu_height);
        int margin = getResources().getDimensionPixelSize(R.dimen.video_audio_menu_margin_right);
        final int x = screenSize.x-w-margin;
        final int y = getResources().getDimensionPixelSize(R.dimen.video_audio_menu_margin_top);
        mBDChapterInfoMenu = new RTKVScrollMenu(x, y,
            (int)getResources().getDimension(R.dimen.video_audio_menu_width),
            (int)getResources().getDimension(R.dimen.video_audio_menu_height),
            getString(R.string.bd_chapter_info), null, ContentView);
        mBDChapterInfoMenu.setGravity(Gravity.CENTER);
        mBDChapterInfoMenu.setOnKeyListener(new BDChapterInfoMenuOnKeyListener());
        mBDChapterInfoMenu.setItemClickListener(new BDChapterInfoMenuItemClickListener());
        mBDChapterInfoMenu.setOnDismissListener(new MenuOnDismissListener());
        UpdateBDChapterInfoMenu();
    }

    public void UpdateBDChapterInfoMenu() {
        if(mBDChapterInfoMenu!=null) {
            mBDChapterInfoMenu.setSelectionList(mPlayer.getBDChapterList());
        }
    }

    private class BDChapterInfoMenuItemClickListener implements OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id)
        {
            Log.d(TAG, "BDChapterInfo menu --" + mBDChapterInfoMenu.getItemof(position)+" is clicked");
            if(position<mBDChapterInfoMenu.getCount()) {
                mBDChapterInfoMenu.requestFocusFromTouch();
                mBDChapterInfoMenu.setSelectedItem(position);
                setBDChapterInfo(position);
                mBDChapterInfoMenu.Hide();
            }
        }
    }

    private class BDChapterInfoMenuOnKeyListener implements AdapterView.OnKeyListener {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE) {
                    mBDChapterInfoMenu.Hide();
                    return true;
                }
            }
            return false;
        }
    }

    public void setBDChapterInfo(int position) {
        Log.d(TAG, "setBDChapterInfo: " + position+1);
        mPlayer.setBDChapter(position+1);
    }

    public void processZoom(){
        if(mRtkVoutUtilManager!=null && mPlayer.isPlaying()){
            mRtkVoutUtilManager.nextZoom();
        }
    }

    public boolean isUnderZoomMode(){
        if(mRtkVoutUtilManager==null)
            return false;
        else
            return mRtkVoutUtilManager.isZooming();
    }

    private boolean resetZoom(){
        if(mRtkVoutUtilManager==null)
            return false;
        else if(isUnderZoomMode()==false)
            return false;
        else
            return mRtkVoutUtilManager.resetZoom();
    }

    public void resetActivity() {
        resetZoom();
        if(mSubMenuGroup[0]!=null)
            mSubMenuGroup[0].Hide();
        if(mAudioMenu!=null) {
            mAudioMenu.Hide();
            mAudioMenu=null;
        }
        if(mAROption!=null)
            mAROption.dismiss();
        if(m3DOption!=null)
            m3DOption.dismiss();
        if(m3DSubOption!=null)
            m3DSubOption.dismiss();
        if(mBDInfoMenu!=null)
            mBDInfoMenu.Hide();
        if(mPropertyMenu!=null)
            mPropertyMenu.Hide();
        mSubIndex = 0;
        mCurrentSubFocus = 0;
        mAudioIndex=0;
        mIsTextSubtitle = MoviePlayer.SUBTITLE_NON_TEXT;
        if(m3Dto2DMode>0) {
            set3Dto2DMode(-1, true);
        }
    }

    public void processDirectionKeyInZoomMode(int keycode){
        if(mRtkVoutUtilManager!=null){
            mRtkVoutUtilManager.moveZoom(keycode);
        }
    }

    private void NewPropertyMenu() {
        View mPropertyView = getLayoutInflater().inflate(R.layout.vscrollmenu_property_info,null);
        Display display = getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int w = getResources().getDimensionPixelSize(R.dimen.property_menu_width);
        int h = getResources().getDimensionPixelSize(R.dimen.property_menu_height);
        final int x = (screenSize.x-w)/2;
        final int y = (screenSize.y-h)/2;
        mPropertyMenu = new RTKVScrollMenu(x, y,
            (int)getResources().getDimension(R.dimen.property_menu_width),
            (int)getResources().getDimension(R.dimen.property_menu_height),
            getString(R.string.property), null, mPropertyView);
        mPropertyMenu.setGravity(Gravity.LEFT);
        mPropertyMenu.setOnKeyListener(new PropertyMenuOnKeyListener());
        mPropertyMenu.setItemClickListener(new PropertyMenuItemClickListener());
        UpdatePropertyMenu();
    }

    public void UpdatePropertyMenu() {
        if(mPropertyMenu!=null) {
            mPropertyMenu.setSelectionList(mPlayer.getPropertyList());
        }
    }

    private class PropertyMenuOnKeyListener implements AdapterView.OnKeyListener {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE) {
                    mPropertyMenu.Hide();
                    return true;
                }
            }
            return false;
        }
    }
    private class PropertyMenuItemClickListener implements OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id)
        {
            Log.d(TAG, "Property menu --" + mPropertyMenu.getItemof(position)+" is clicked");
            if(position<mPropertyMenu.getCount()) {
                mPropertyMenu.requestFocusFromTouch();
                mPropertyMenu.setSelectedItem(position);
            }
        }
    }
	
    public boolean showHwcOsd() {
        SystemProperties.set("rtk.hwc.osd.hide", Long.toString(0));
        return true;
    }

    public boolean hideHwcOsd() {
        if(mPlayer==null) {
            return false;
        }
        //limit for hiding HWC OSD
        if(mPlayer.getVideoWidth() < 3600) {
            return false;
        }
        else if(mPlayer.getVideoHeight() < 1800) {
            return false;
        }

        //Check all osd windows is shown or not
        if(mPlayer.mController.isShown()) {
            return false;
        }
        else if(mPlayer.isHintShowing()) {
            return false;
        }
        else if(mOptionMenu!=null && mOptionMenu.isShowing()) {
            return false;
        }
        else if(mSubMainMenu!=null && mSubMainMenu.isShowing()) {
            return false;
        }
        else if(mAudioMenu!=null && mAudioMenu.isShowing()) {
            return false;
        }
        else if(m3DOption!=null && m3DOption.isShowing()) {
            return false;
        }
        else if(m3DSubOption!=null && m3DSubOption.isShowing()) {
            return false;
        }
        else if(mAROption!=null && mAROption.isShowing()) {
            return false;
        }
        else if(mNotificationShown) {
            return false;
        }
        else if(mSystemUIShown>0) {
            return false;
        }
        else if(mControlPanelShown) {
            return false;
        }
        else {
            SystemProperties.set("rtk.hwc.osd.hide.delay.ms", Long.toString(1000));
            SystemProperties.set("rtk.hwc.osd.hide", Long.toString(1));
        }
        return true;
    }

    @Override
    public void finish (){
        // BUG_FIX: 43900
        mPlayer.hideVideoView();
        if(mBroadcastStatus){
            // BUG_FIX: 43406 if MovieActivity is started by DLNA or AirFun, when it's completed, always back to home
            // BUG_FIX: 43830 revert modification for 43406, modify the way how airfun and DMR launch MovieActivity to let MovieActivity back to home on stop
            //Intent intent = new Intent(Intent.ACTION_MAIN);
            //intent.addCategory(Intent.CATEGORY_HOME);
            //startActivity(intent);
            super.finish();
        }else{
            // BUG_FIX: 42659
            // report play position on complete.
            // Toast.makeText(this,"MovieActivity finish",Toast.LENGTH_SHORT).show();
            Intent rst = new Intent();
            int array_len = mPlayer.mVideoUrls.length;
            int index = mPlayer.mCurrentVideoIdx;
            // if playlist is finished, mCurrentVideoIdx would be added by over one, handle this case
            if(index>=array_len)
                index = array_len-1;
            else if(index<0)
                index = 0;

            rst.putExtra("StopPosition",mPlayer.mVideoUrls[index]);
            setResult(Activity.RESULT_OK, rst);
            super.finish();
        }
    }

    public boolean getSubtitlePref() {
        SharedPreferences prefs = getSharedPreferences(SUBTITLE_PREF, Context.MODE_PRIVATE);
        boolean val = prefs.getBoolean(KEY_SAVE_SUBTITLE_PREF, true);
        if(!prefs.contains(KEY_SAVE_SUBTITLE_PREF)) {
            prefs.edit()
                .putBoolean(KEY_SAVE_SUBTITLE_PREF, val)
                .apply();
        }
        Log.d(TAG, "getSubtitlePref: " + val);
        return val;
    }

    public void setSubtitlePref(boolean val){
        SharedPreferences prefs = getSharedPreferences(SUBTITLE_PREF, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();
        if(val==false) {
            editor.clear();
            editor.putBoolean(KEY_SAVE_SUBTITLE_PREF, val);
        }
        else {
            editor.putBoolean(KEY_SAVE_SUBTITLE_PREF, val);
        }
        editor.apply();
        Log.d(TAG, "setSubtitlePref: " + val);
        mUri = mPlayer.getPlayingUri();
        setSubtitleIndexPref(mUri.toString(), mSubIndex);
    }

    public int getSubtitleIndexPref(String path){
        if(getSubtitlePref() == false) {
            return -1;
        }
        SharedPreferences prefs = getSharedPreferences(SUBTITLE_PREF, Context.MODE_PRIVATE);
        int subtitleIndex = -1;
        String videoUrl = GalleryUtils.removePathPrefix(path, false);
        if(prefs.contains(videoUrl)) {
            subtitleIndex = prefs.getInt(videoUrl, 0);
        }
        mSubOffset = prefs.getInt(KEY_SUBTITLE_OFFSET_PREF, 0);
        mSubSize = prefs.getInt(KEY_SUBTITLE_TEXTSIZE_PREF, 20);
        mSubSync = prefs.getFloat(KEY_SUBTITLE_SYNC_PREF, 0.0f);
        mSubEncode = prefs.getInt(KEY_SUBTITLE_TEXTENCODE_PREF, 0);
        mSubColor = prefs.getInt(KEY_SUBTITLE_TEXTCOLOR_PREF, 0);
        Log.d(TAG, "[getSubtitleIndexPref] streamNum= "+subtitleIndex+", SubOffset= "+mSubOffset+
            ", SubSize= "+mSubSize+", SubSync= "+mSubSync+", SubEncode= "+mSubEncode+", SubColor= "+mSubColor);
//        Log.d(TAG, "prefs.getAll(): " + prefs.getAll());
        return subtitleIndex;
    }
    public void setSubtitleIndexPref(String path, int subtitleIndex){
        if(getSubtitlePref() == false) {
            return;
        }
        SharedPreferences prefs = getSharedPreferences(SUBTITLE_PREF, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();
        if(mPlayer.getSubtitleCount()>0) {
            String videoUrl = GalleryUtils.removePathPrefix(path, false);
            editor.putInt(videoUrl, subtitleIndex);
        }
        editor.putInt(KEY_SUBTITLE_OFFSET_PREF, mSubOffset);
        editor.putInt(KEY_SUBTITLE_TEXTSIZE_PREF, mSubSize);
        editor.putFloat(KEY_SUBTITLE_SYNC_PREF, mSubSync);
        editor.putInt(KEY_SUBTITLE_TEXTENCODE_PREF, mSubEncode);
        editor.putInt(KEY_SUBTITLE_TEXTCOLOR_PREF, mSubColor);
        editor.apply();
        Log.d(TAG, "setSubtitleIndexPref: " + subtitleIndex + ", Uri: " + path);
    }

    public int getRepeatModePref() {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        int val = prefs.getInt(KEY_REPEAT_MODE_PREF, 0);
        Log.d(TAG, "getRepeatModePref: " + val);
        return val;
    }

    public void setRepeatModePref(int repeat){
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        SharedPreferences.Editor editor = prefs.edit();
        editor.putInt(KEY_REPEAT_MODE_PREF, repeat);
        editor.apply();
        Log.d(TAG, "setRepeatModePref: " + repeat);
    }
       
    /**
     * When the media file from a samba server will be playbak, we must mount samba server first.
     * This method can mount samba server as a local file system and return video Uri begin with 
     * "file:///tmp/ramfs/mnt/"
     */
    public Uri mountSambaServer(Uri videoUri) {
        Log.d(TAG, "Samba file uri: "+videoUri.toString());
        SmbUtils smbUtils = SmbUtils.getInstance(this);
        return smbUtils.mountSambaServer(videoUri);     
    }
    
    /**
     * unmount samaba server
     */
    public void unmountSambaServer() {
        SmbUtils.deleteInstance();
    } 
}
