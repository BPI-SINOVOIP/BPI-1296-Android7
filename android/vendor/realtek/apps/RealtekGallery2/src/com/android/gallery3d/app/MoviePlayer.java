/*
 * Copyright (C) 2009 The Android Open Source Project
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
import android.app.AlertDialog;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Configuration;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.media.AudioManager;
import android.media.MediaFormat;
import android.media.MediaPlayer;
import android.media.MediaPlayer.NaviInfo;
import android.media.MediaPlayer.BDInfo;
import android.media.MediaPlayer.TrackInfo;
import android.media.NavigationRecord;
import android.media.audiofx.AudioEffect;
import android.media.audiofx.Virtualizer;
import android.net.ConnectivityManager;
import android.net.EthernetManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.provider.MediaStore;
import android.support.v4.app.TaskStackBuilder;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.RemoteViews;
import android.widget.RtkVideoView;
import android.widget.RtkMediaController;
import android.widget.TextView;
import android.view.LayoutInflater;
import android.widget.LinearLayout;
import com.android.gallery3d.R;
import com.android.gallery3d.common.ApiHelper;
import com.android.gallery3d.common.BlobCache;
import com.android.gallery3d.util.CacheManager;
import com.android.gallery3d.util.GalleryUtils;
import com.realtek.hardware.RtkHDMIManager;
import com.realtek.hardware.RtkVoutUtilManager;
import com.realtek.hardware.RtkTVSystem;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.util.ArrayList;
import java.util.Date;
import android.widget.Toast;
import android.widget.PopupWindow;
import java.text.DecimalFormat;
import android.media.MediaPlayer.NaviType;
import android.provider.Settings;

public class MoviePlayer implements
        MediaPlayer.OnErrorListener, MediaPlayer.OnInfoListener, MediaPlayer.OnCompletionListener, MediaPlayer.OnSpeedChangedListener,
        MediaPlayer.OnSeekResultListener, MediaPlayer.OnSeekCompleteListener, ControllerOverlay.Listener,
        PopupWindow.OnDismissListener{
    @SuppressWarnings("unused")
    private static final String TAG = "MoviePlayer";

    private static final String KEY_VIDEO_POSITION = "video-position";
    private static final String KEY_RESUMEABLE_TIME = "resumeable-timeout";
    private static final String KEY_PLAYLIST = "playlist";
    private static final String KEY_PLAYLIST_TITLE = "playlistTitle";


    // Copied from MediaPlaybackService in the Music Player app.
    private static final String SERVICECMD = "com.android.music.musicservicecommand";
    private static final String CMDNAME = "command";
    private static final String CMDPAUSE = "pause";

    private static final String VIRTUALIZE_EXTRA = "virtualize";
    private static final long BLACK_TIMEOUT = 0;//1000;

    private static final int REPEAT_MODE_OFF = 0;
    private static final int REPEAT_MODE_ONE = 1;
    private static final int REPEAT_MODE_ALL = 2;

    private static final int NAVI_MODE_OFF = 0;
    private static final int NAVI_MODE_PREV = 1;
    private static final int NAVI_MODE_NEXT = 2;

    public static final int MODE_3D_NOT_3DTV = 0;
    public static final int MODE_3D_NOT_MVC = 1;
    public static final int MODE_3D_MVC = 2;

    public static final int SUBTITLE_NON_TEXT = 0;
    public static final int SUBTITLE_OFFSET = 1;
    public static final int SUBTITLE_TEXT = 2;

    public static final int VIDEO_NOTIFICATION_ID = 0x2000;

    private static final int VIDEO_HINT_TIMEOUT = 3000;
    private static final int VIDEO_REMINDER_TIMEOUT = 8000;
    private static final int VIDEO_PREVNEXT_REMINDER_TIMEOUT = 2000;
    private static final int VIDEO_CHANGE_TVSYSTEM_TIMEOUT = 2000;
    private static final int ALERTDIALOG_DISPLAY_TIMEOUT = 8000;

    private static final int VIDEO_TIME_OFFSET_10S = 10000;
    private static final int VIDEO_TIME_OFFSET_30S = 30000;
    private static final int VIDEO_TIME_OFFSET_120S = 120000;
    private static final int VIDEO_TIME_OFFSET_300S = 300000;
    private static final int VIDEO_SEEK_TOLERANCE = 5000;

    // If we resume the acitivty with in RESUMEABLE_TIMEOUT, we will keep playing.
    // Otherwise, we pause the player.
    private static final long RESUMEABLE_TIMEOUT = 3 * 60 * 1000; // 3 mins
    private static final int FF_FR_MAX_SPEED = 32;

    private Context mContext;
    private final View mRootView;
    private final ViewGroup mUIRootView;
    private final RtkVideoView mVideoView;
    private final Bookmarker mBookmarker;
    private Uri mUri;
    private final Handler mHandler = new Handler();
    private final AudioBecomingNoisyReceiver mAudioBecomingNoisyReceiver;
    public final MovieControllerOverlay mController;

    private final boolean CLEAR_VIDEO_FRAME = true;
    private long mResumeableTime = Long.MAX_VALUE;
    private int mVideoPosition = 0;
    private boolean mHasPaused = false;
    private boolean mBroadcastStatus = false;
    private int mLastSystemUiVis = 0;

    private int mVideoTimeOffset = VIDEO_TIME_OFFSET_10S;
	
    // If the time bar is being dragged.
    private boolean mDragging;

    // If the time bar is visible.
    private boolean mShowing;
    private boolean mCompletion = false;

    private Virtualizer mVirtualizer;
    private int mSpeed = 0; 
    private MovieActivity mActivity;
    /* <--- Video playlist support ---- */
    public String mPlayListAddress = null;
    public String mPlayListTitleAddress = null;
    public String[] mVideoUrls = null;
    public String[] mVideoTitles = null;
    public int mCurrentVideoIdx = 0;
    /* ---- Video playlist support ---> */
    private int mRepeatMode = REPEAT_MODE_OFF;
    private int mNaviMode = NAVI_MODE_OFF;
    private boolean mRepeatAllOrNone = false;   // For AirFun
    private String mSourceFrom;
    private String mDvdPlayerExtension = null;
    private boolean mForceStop = false;
    private ViewGroup mMovieHint;
    private ViewGroup mMovieReminder;
    private ViewGroup mPrevNextReminder;
    private Animation mContainerAnimIn;
    private Animation mContainerAnimOut;
    private RtkHDMIManager mRtkHDMIManager;
    private RtkVoutUtilManager mRtkVoutUtilManager;
    private boolean mBUnderABRepeat;
    private int mABRepeatStartPos;
    private DecimalFormat mFmt = new DecimalFormat("00.0");
    private DecimalFormat mTwoDigitFmt = new DecimalFormat("00");
    private RemoteViews mExpandedView;
    private Notification mBuilder;
    private int mWidth = 0;
    private int mHeight = 0;
    private int mSeekToTime = -1;
    private boolean mDelaySeek = false;
    private boolean mSeeking = false;
    private boolean m3DEnable = false;
    private int m3DType = MODE_3D_NOT_3DTV;
    private int mTvSystemFor3DRestored=-1;
    private int mUseRTMediaPlayer;

    private String mDisplayedChapter = "";
    private int mChapterTotalNum=0;
    private int mChapterCurrent=0;
    private AlertDialog mAlertDialog = null;
    private AlertDialog mRecommandDialog = null;
    private int mTvSystemForRestored=-1;
    private int mTvSystemForRecommand=-1;
    private int mCheckTrackInfoCount = 0;
    private Toast mToast;
    private UpdateRecentPlayTask mUpdateRecentPlayTask;

    private boolean mShowChangeTvSystemDialog = true;
    private boolean mShowResumeDialog = true;
	
    public class TrackMap {
        TrackInfo info;
        int index;
    };
    private ArrayList<TrackMap> mAudioList = new ArrayList<TrackMap>();
    private ArrayList<TrackMap> mSubtitleList = new ArrayList<TrackMap>();

    private TrackInfo[] mTrackInfo=null;

    private final Runnable mPlayingChecker = new Runnable() {
        @Override
        public void run() {
            if (mVideoView.isPlaying()) {
                mController.showPlaying(true, true);
                mController.hide();
            } else {
                mHandler.postDelayed(mPlayingChecker, 250);
            }
        }
    };

    private final Runnable mProgressChecker = new Runnable() {
        @Override
        public void run() {
            int pos = setProgress();
            mHandler.removeCallbacks(mProgressChecker);
            if(isPlaying()) {
                mHandler.postDelayed(mProgressChecker, 1000 - (pos % 1000));
            }
            else {
                mHandler.postDelayed(mProgressChecker, 5000);
            }
        }
    };

    private final Runnable mHideSystemUi = new Runnable(){
        @Override
        public void run(){
            if(isPlaying()) {
                showSystemUi(false);
            }
        }
    };

    private final Runnable mHideMovieHint = new Runnable() {
        @Override
        public void run() {
            showMovieHint(false, 0, 0, 0);
        }
    };

    private final Runnable mHideMovieReminder = new Runnable() {
        @Override
        public void run() {
            showMovieReminder(false, 0, 0);
        }
    };

    private final Runnable mHidePrevNextReminder = new Runnable() {
        @Override
        public void run() {
            showPrevNextReminder(false, "", 0);
        }
    };

    private final Runnable mSeekTo = new Runnable() {
        @Override
        public void run() {
            if(mVideoView.isPlaying()==false) {
                Log.d(TAG, "mVideoView.isPlaying() = "+mVideoView.isPlaying());
                mHandler.postDelayed(this,500);
                return;
            }
            else if(mSeeking==true) {
                Log.d(TAG, "mSeeking = "+mSeeking);
                mHandler.postDelayed(this,200);
                return;
            }
            else {
                mDelaySeek=false;
                if(mSeekToTime>=0) {
                    int duration = mVideoView.getDuration();
                    Log.d(TAG, "mSeekToTime = "+mSeekToTime);
                    mSeeking=true;
                    if(mSeekToTime >= duration) {
                        Log.d(TAG, "mSeekToTime: "+mSeekToTime+", duration: "+duration+", mRepeatMode: "+mRepeatMode);
                        if(mRepeatMode == REPEAT_MODE_ONE) {
                            mVideoView.seekTo(0);
                        }
                        else {
                            onNext(false);
                        }
                    }
                    else {
                        mVideoView.seekTo(mSeekToTime);
                    }
                    mVideoTimeOffset = VIDEO_TIME_OFFSET_10S;
                }
            }
        }
    };

    private final Runnable mOnCompletion = new Runnable() {
        @Override
        public void run() {
            onCompletion(null);
        }
    };

    private final Runnable mCheckTrackInfo = new Runnable(){
        @Override
        public void run(){
            mTrackInfo = mVideoView.getTrackInfo();
            mCheckTrackInfoCount++;
            if(checkTrackInfoValid()==false) {
                Log.d(TAG,"checkTrackInfoValid() failed.");
                mHandler.postDelayed(this,500);
            }
            else {
                Log.d(TAG, "mCheckTrackInfo done. broadcast status:"+mBroadcastStatus+", mCheckTrackInfoCount = "+mCheckTrackInfoCount);
//                updateNotificationBuilder();
//                updateVideoNotification();
                mActivity.UpdatePropertyMenu();
                mVideoView.requestLayout();
                check3DType();
                checkSubtitleIdx();
                removeMoviePlayerPendingUpdateRunnable();
                mHandler.post(mUpdateInfoRunnable);
                mHandler.removeCallbacks(mCheckDialog);
                mHandler.post(mCheckDialog);
            }
        }
    };

    private final Runnable mCheckDialog = new Runnable(){
        @Override
        public void run(){
            if(checkTrackInfoValid()==false) {
                mHandler.postDelayed(this,1000);
            }else{
                Log.d(TAG, "mCheckDialog: mCheckTrackInfoCount = "+mCheckTrackInfoCount);
                check3DType();
                if(get3DType()==MODE_3D_MVC) {
                    Log.d(TAG, "Enable 3D MVC automatically");
                    if(get3DEnable()==false) {
                        setMVC(true);
                        setFormat3d(RtkVoutUtilManager.FORMAT_3D_AUTO_DETECT);
                    }
                    checkBookmark();
                }
                else if(getRecommandTvSystem()>=0) {
                    if(mRepeatMode==REPEAT_MODE_ONE)
                        keepRepeatSetting();
                    checkTVSystem();
                }
                else {
                    checkBookmark();
                }
            }
        }
    };

    private final Runnable mUpdateInfoRunnable = new Runnable(){
        @Override
        public void run(){
                Log.d(TAG,"\033[0;31;31m Update Notification - chapter \033[m");
                String currentChapter = getChapterDisplay();

                if(mChapterTotalNum<=1){
                    return;
                }
                if(!currentChapter.equals(mDisplayedChapter)){
                    mDisplayedChapter = currentChapter;
                    mActivity.UpdatePropertyMenu();
                    Log.d(TAG,"\033[0;31;31m update chapter \033[m");
                }
                // register next message
                mHandler.postDelayed(this,5000);
        }
    };

    private final Runnable mHideResumeDialog = new Runnable(){
        @Override
        public void run(){
            if(mAlertDialog!=null && mAlertDialog.isShowing()==true) {
                mAlertDialog.dismiss();
                mAlertDialog=null;
            }
        }
    };

    private final Runnable mHideChangeTvSystemDialog = new Runnable(){
        @Override
        public void run(){
            if(mRecommandDialog!=null && mRecommandDialog.isShowing()) {
                mRecommandDialog.dismiss();
                mRecommandDialog=null;
                checkBookmark();
            }
        }
    };

    private BroadcastReceiver mRemovedReceiver = new BroadcastReceiver()
    {
        public void onReceive(Context context, Intent intent)
        {
            Log.d(TAG, "intent.getAction()="+intent.getAction());
            if (intent.getAction().equals(Intent.ACTION_MEDIA_UNMOUNTED)
                || intent.getAction().equals(Intent.ACTION_MEDIA_REMOVED)
                || intent.getAction().equals(Intent.ACTION_MEDIA_BAD_REMOVAL)) {
                String currentPath = null;
                if(mVideoUrls!=null) {
                    int index = mCurrentVideoIdx;
                    if(index>=mVideoUrls.length)
                        index = mVideoUrls.length-1;
                    if(index<0)
                        index=0;
                    currentPath = mVideoUrls[index];
                }
                else {
                    currentPath = mUri.toString();
                }
                String data = intent.getData().toString();
                currentPath = Uri.decode(currentPath);
                if(currentPath.indexOf(data)>=0 && mForceStop==false) {
                    Log.d(TAG, currentPath+" ) is removed.");
                    mForceStop=true;
                    if(mToast!=null) {
                        mToast.cancel();
                    }
                    mToast = Toast.makeText(mActivity, mActivity.getString(R.string.device_has_been_removed),Toast.LENGTH_LONG);
                    mToast.show();
                    mActivity.finish();
                }
            }
            else if (intent.getAction().equals(ConnectivityManager.CONNECTIVITY_ACTION) ||
                        intent.getAction().equals(ConnectivityManager.INET_CONDITION_ACTION) ||
                        intent.getAction().equals(EthernetManager.NETWORK_STATE_CHANGED_ACTION)) {
                if(mSourceFrom!=null && mSourceFrom.equals("Network") && mForceStop==false) {
                    if(!GalleryUtils.hasNetwork(mActivity)) {
                        mForceStop=true;
                        mActivity.finish();
                    }
                }
            }
        }
    };

    public MoviePlayer(View rootView, final MovieActivity movieActivity,
            Uri videoUri, Bundle savedInstance, boolean canReplay, boolean broadcastStatus, int useRTMediaPlayer) {
        mActivity = movieActivity;
        mContext = movieActivity.getApplicationContext();
        mRootView = rootView;
        mUIRootView = (ViewGroup) mRootView.findViewById(R.id.movie_ui_root);
        mBroadcastStatus = broadcastStatus;
        mVideoView = (RtkVideoView) mRootView.findViewById(R.id.surface_view);

        // set activity reference to RtkMovieVideoView.
        if(mVideoView instanceof RtkMovieVideoView){
            RtkMovieVideoView mRMView = (RtkMovieVideoView) mVideoView;
            mRMView.mActivity = movieActivity;
            mRMView.mPlayer = this;
            mRMView.mRootView = mRootView;
        }

        mBookmarker = new Bookmarker(movieActivity);
        mUri = videoUri;
        mBUnderABRepeat=false;
        mABRepeatStartPos=-1;
        mContainerAnimIn=AnimationUtils.loadAnimation(mContext, R.anim.on_screen_hint_enter);
        mContainerAnimOut=AnimationUtils.loadAnimation(mContext, R.anim.on_screen_hint_exit);

        mRtkHDMIManager = new RtkHDMIManager();
        if(mRtkHDMIManager == null) {
            Log.d(TAG, "mRtkHDMIManager is null!");
        }
        mRtkVoutUtilManager = new RtkVoutUtilManager();
        if(mRtkVoutUtilManager == null) {
            Log.d(TAG, "mRtkVoutUtilManager is null!");
        }

        mController = new MovieControllerOverlay(mContext);
        mUIRootView.addView(mController.getView());

        LayoutInflater inflater = (LayoutInflater) mActivity.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        mMovieHint = (ViewGroup)inflater.inflate(R.layout.movie_hint, mUIRootView, false);
        mUIRootView.addView(mMovieHint);

        mMovieReminder = (ViewGroup)inflater.inflate(R.layout.movie_reminder, mUIRootView, false);
        mUIRootView.addView(mMovieReminder);

        mPrevNextReminder = (ViewGroup)inflater.inflate(R.layout.movie_reminder, mUIRootView, false);
        mUIRootView.addView(mPrevNextReminder);

        mController.setListener(this);
        mController.setCanReplay(canReplay);

        Intent ai = movieActivity.getIntent();
        mDvdPlayerExtension = ai.getStringExtra("MEDIA_BROWSER_EXTRA_DVDPLAYER_EXTENSION");
        mSourceFrom = ai.getStringExtra("SourceFrom");
        if (null != mSourceFrom) {
            Log.d(TAG, "SourceFrom=" + mSourceFrom);
            mVideoView.setSourceFrom(mSourceFrom);
        }
        mUseRTMediaPlayer = useRTMediaPlayer;
        mController.setBroadcastStatus(mBroadcastStatus);
        mVideoView.setBroadcastStatus(broadcastStatus);
        mVideoView.setOnErrorListener(this);
        mVideoView.setOnInfoListener(this);
        mVideoView.setOnCompletionListener(this);
        mVideoView.setOnSeekResultListener(this);
        mVideoView.setOnSeekCompleteListener(this);
        mVideoView.setOnSpeedChangedListener(this);

        boolean virtualize = ai.getBooleanExtra(VIRTUALIZE_EXTRA, false);
        if (virtualize) {
            int session = mVideoView.getAudioSessionId();
            if (session != 0) {
                mVirtualizer = new Virtualizer(0, session);
                mVirtualizer.setEnabled(true);
            } else {
                Log.w(TAG, "no audio session to virtualize");
            }
        }

        if(ai.hasExtra("repeat")) {
            boolean repeat = ai.getBooleanExtra("repeat", false);
            Log.d(TAG, "Has repeat mode in intent :" + repeat);
            if(repeat==false)
                mRepeatMode = REPEAT_MODE_OFF;
            else
                mRepeatMode = REPEAT_MODE_ALL;
        }
        else {
            mRepeatMode = mActivity.getRepeatModePref();
        }

        /* <--- Video playlist support ---- */
        mCurrentVideoIdx = 0;
        mPlayListAddress = ai.getStringExtra("PlayListAddress");
        mPlayListTitleAddress = ai.getStringExtra("PlayListTitleAddress");
        if(mPlayListAddress!=null && !mPlayListAddress.equals("Intent")) {
            mVideoUrls = GalleryUtils.getPlayListFromFile(mPlayListAddress);
            mVideoTitles = GalleryUtils.getPlayListFromFile(mPlayListTitleAddress);
        }
        else {
            mVideoUrls = ai.getStringArrayExtra(KEY_PLAYLIST);
            mVideoTitles = ai.getStringArrayExtra(KEY_PLAYLIST_TITLE);
        }
        useRTMediaPlayer(mUri.toString());

        if(GalleryUtils.hasSubtitlePlayListFromFile(mPlayListAddress)) {
            Uri uri;
            if(null != mDvdPlayerExtension && mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER) {
                uri = Uri.parse(mUri.toString() + "&rtksubtitle=file:/" + mPlayListAddress + " " + mDvdPlayerExtension);
            } else {
                uri = Uri.parse(mUri.toString() + "&rtksubtitle=file:/" + mPlayListAddress);
            }
            mVideoView.setVideoURI(uri);
        }
        else {
            if(null != mDvdPlayerExtension && mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER) {
                Uri uri = Uri.parse(mUri.toString() + " " + mDvdPlayerExtension);
                mVideoView.setVideoURI(uri);
            } else {
                mVideoView.setVideoURI(mUri);
            }          
        }

        if (mVideoUrls != null) {
/*
            for(int count=0;count<mVideoUrls.length;count++)
            {
                if(mUri.toString().equals(mVideoUrls[count]))
                {
                    mCurrentVideoIdx=count;
                    break;
                }
            }
*/
            mCurrentVideoIdx = ai.getIntExtra("index-hint", 0);
        }
        else if ((mVideoUrls = ai.getStringArrayExtra("playlist-url")) != null ) {
            mCurrentVideoIdx = 0;
            mRepeatMode = REPEAT_MODE_OFF;
            mRepeatAllOrNone = true;
            mUri = Uri.parse(mVideoUrls[mCurrentVideoIdx]);
            useRTMediaPlayer(mUri.toString());
            mVideoView.setVideoURI(Uri.parse(mVideoUrls[mCurrentVideoIdx]));
            mVideoView.setContinuousPlay(true);
        }
        else {
            mVideoUrls = new String[]{ mUri.toString() };
        }
        mController.setRepeatMode(mRepeatMode, false);
        UpdateVideoRecentPlay(mUri.toString());
        /* ---- Video playlist support ---> */

        mVideoView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                mController.show(true,true);
                return true;
            }
        });
        mVideoView.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer player) {
                Log.i(TAG, "\033[0;31;31m -------- Receive onPrepared event, hide Controller ---------- \033[m");
                mController.hide();
                if(mVideoView instanceof RtkMovieVideoView){
                    RtkMovieVideoView rv = (RtkMovieVideoView)mVideoView;
                    rv.setOnVideoSizeChangedListener(player);
                }
                if (!mVideoView.canSeekForward() || !mVideoView.canSeekBackward()) {
                    mController.setSeekable(false);
                } else {
                    mController.setSeekable(true);
                }

                if(mRepeatMode==REPEAT_MODE_ONE) {
                    mVideoView.setLooping(true);
                }
                else {
                    mVideoView.setLooping(false);
                }
                mCheckTrackInfoCount=0;
                mHandler.removeCallbacks(mCheckTrackInfo);
                mHandler.removeCallbacks(mProgressChecker);
                mHandler.post(mCheckTrackInfo);
                mHandler.post(mProgressChecker);
                mVideoView.start();
            }
        });

        // The SurfaceView is transparent before drawing the first frame.
        // This makes the UI flashing when open a video. (black -> old screen
        // -> video) However, we have no way to know the timing of the first
        // frame. So, we hide the VideoView for a while to make sure the
        // video has been drawn on it.
        mVideoView.postDelayed(new Runnable() {
            @Override
            public void run() {
                mVideoView.setVisibility(View.VISIBLE);
            }
        }, BLACK_TIMEOUT);

        setOnSystemUiVisibilityChangeListener();
        // Hide system UI by default
        showSystemUi(false);

        mAudioBecomingNoisyReceiver = new AudioBecomingNoisyReceiver();
        mAudioBecomingNoisyReceiver.register();

        Intent i = new Intent(SERVICECMD);
        i.putExtra(CMDNAME, CMDPAUSE);
        movieActivity.sendBroadcast(i);

        if (savedInstance != null) { // this is a resumed activity
            mVideoPosition = savedInstance.getInt(KEY_VIDEO_POSITION, 0);
            mResumeableTime = savedInstance.getLong(KEY_RESUMEABLE_TIME, Long.MAX_VALUE);
            mVideoView.start();
            mVideoView.suspend();
            mHasPaused = true;
        } else {
            startVideo();
            //final Integer bookmark = mBookmarker.getBookmark(mUri);
            //if (bookmark != null && broadcastStatus==false) {
            //    showResumeDialog(movieActivity, bookmark);
            //}
        }

        // debug mVideoView layout changed
        //mVideoView.addOnLayoutChangeListener(new View.OnLayoutChangeListener(){
        //    public void onLayoutChange (View v, int left, int top, int right, int bottom, int oldLeft, int oldTop, int oldRight, int oldBottom){
                //Log.d("Gallery2","\033[0;31;31m mVideoView layout changed \033[m");
                //Log.d("Gallery2","top   :"+top);
                //Log.d("Gallery2","bottom:"+bottom);
        //    }
        //});
    }

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    private void setOnSystemUiVisibilityChangeListener() {
        if (!ApiHelper.HAS_VIEW_SYSTEM_UI_FLAG_HIDE_NAVIGATION) return;

        // When the user touches the screen or uses some hard key, the framework
        // will change system ui visibility from invisible to visible. We show
        // the media control and enable system UI (e.g. ActionBar) to be visible at this point
        mVideoView.setOnSystemUiVisibilityChangeListener(
                new View.OnSystemUiVisibilityChangeListener() {
            @Override
            public void onSystemUiVisibilityChange(int visibility) {
                int diff = mLastSystemUiVis ^ visibility;
                mLastSystemUiVis = visibility;
                if ((diff & View.SYSTEM_UI_FLAG_HIDE_NAVIGATION) != 0
                        && (visibility & View.SYSTEM_UI_FLAG_HIDE_NAVIGATION) == 0) {
                    mRootView.setBackgroundColor(Color.BLACK);
                }
            }
        });
    }

    @SuppressWarnings("deprecation")
    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    private void showSystemUi(boolean visible) {
        if (!ApiHelper.HAS_VIEW_SYSTEM_UI_FLAG_LAYOUT_STABLE) return;

        int flag = View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE;
        if (!visible) {
            // We used the deprecated "STATUS_BAR_HIDDEN" for unbundling
            flag |= View.STATUS_BAR_HIDDEN | View.SYSTEM_UI_FLAG_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
        }
        mVideoView.setSystemUiVisibility(flag);
    }

    public void onSaveInstanceState(Bundle outState) {
        outState.putInt(KEY_VIDEO_POSITION, mVideoPosition);
        outState.putLong(KEY_RESUMEABLE_TIME, mResumeableTime);
    }


    private void checkBookmark() {
        final Integer bookmark = mBookmarker.getBookmark(mUri);
        Log.d(TAG, "checkBookmark: broadcast status:"+mBroadcastStatus+" bookmark:"+bookmark);
        if (bookmark != null && mBroadcastStatus==false) {
            if(mShowResumeDialog) {
                showResumeDialog(mActivity, bookmark);
                mShowResumeDialog = false;
            }
        }
    }

    private void showResumeDialog(Context context, final int bookmark) {
        if(mAlertDialog!=null && mAlertDialog.isShowing())
            return;

        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle(R.string.resume_playing_title);
        builder.setMessage(String.format(
                context.getString(R.string.resume_playing_message),
                GalleryUtils.formatDuration(context, bookmark / 1000)));
        builder.setOnCancelListener(new OnCancelListener() {
            @Override
            public void onCancel(DialogInterface dialog) {
            }
        });
        builder.setPositiveButton(
                R.string.resume_playing_resume, new OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                if(mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER) {
                    boolean applyBookmark = mVideoView.applyBookmark(mUri);
                    Log.d(TAG, "applyBookmark: "+applyBookmark);
                    if(applyBookmark==false)
                        showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
                }
                else {
                    final Integer bookmark = mBookmarker.getBookmark(mUri);
                    delaySeekTo(bookmark, 0);
                    Log.d(TAG, "applyBookmark: "+bookmark);
                }
            }
        });
        builder.setNegativeButton(
                R.string.resume_playing_restart, new OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                delaySeekTo(0, 0);
            }
        });

        builder.setOnKeyListener(new DialogOnKeyListener());

        mAlertDialog = builder.create();
        mAlertDialog.setOnShowListener(new DialogInterface.OnShowListener() {
            @Override
            public void onShow(DialogInterface dialog) {
                mHandler.postDelayed(mHideResumeDialog, ALERTDIALOG_DISPLAY_TIMEOUT);
            }
        });
        mAlertDialog.setOnDismissListener(new DialogInterface.OnDismissListener() {
            @Override
            public void onDismiss(DialogInterface dialog) {
                mHandler.removeCallbacks(mHideResumeDialog);
                freshHideSystemUi();
            }
        });
        mAlertDialog.show();
    }

    private void checkTVSystem() {

        if(getVideoInfo()==null) {
            showMovieReminder(true, R.string.no_video_info, VIDEO_REMINDER_TIMEOUT);
            Log.e(TAG, "Can't get video info");
        }

        if(getRecommandTvSystem()>=0) {
            if(needChangeTvSystemDialog(getRecommandTvSystem())==false)
                changeTVSystem();
            else if(mShowChangeTvSystemDialog) {
                showChangeTvSystemDialog(mActivity);
                mShowChangeTvSystemDialog=false;
            }
        }
    }

    private void showChangeTvSystemDialog(Context context) {
        if(mRecommandDialog!=null && mRecommandDialog.isShowing())
            return;

        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle(R.string.recommand_changing_tv_system);
        builder.setMessage(context.getString(R.string.change_tv_system) +" ( "+getTvSystemString(getRecommandTvSystem())+" ) ");
        builder.setOnCancelListener(new OnCancelListener() {
            @Override
            public void onCancel(DialogInterface dialog) {
                checkBookmark();
            }
        });
        builder.setPositiveButton(
                R.string.confirm, new OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                changeTVSystem();
            }
        });
        builder.setNegativeButton(
                R.string.cancel, new OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                checkBookmark();
            }
        });

        builder.setOnKeyListener(new DialogOnKeyListener());

        mRecommandDialog = builder.create();
        mRecommandDialog.setOnShowListener(new DialogInterface.OnShowListener() {
            @Override
            public void onShow(DialogInterface dialog) {
                mHandler.postDelayed(mHideChangeTvSystemDialog, ALERTDIALOG_DISPLAY_TIMEOUT);
            }
        });
        mRecommandDialog.setOnDismissListener(new DialogInterface.OnDismissListener() {
            @Override
            public void onDismiss(DialogInterface dialog) {
                mHandler.removeCallbacks(mHideChangeTvSystemDialog);
                freshHideSystemUi();
            }
        });
        mRecommandDialog.show();
    }

    private class DialogOnKeyListener implements DialogInterface.OnKeyListener  {
        @Override
        public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event)
        {

            if (keyCode == KeyEvent.KEYCODE_SEARCH) {
                Log.d(TAG, "DialogOnKeyListener receive Google Voice event");
                return true;
            }
            else
                return false;
        }
    }

    public void onPause() {
        mHandler.removeCallbacksAndMessages(null);
        mHasPaused = true;
        mVideoPosition = mVideoView.getCurrentPosition();
        mBookmarker.setBookmark(mUri, mVideoPosition, mVideoView.getDuration());
        mVideoView.suspend();
        mResumeableTime = System.currentTimeMillis() + RESUMEABLE_TIMEOUT;
        NotificationManager notificationManager =
            (NotificationManager) mActivity.getSystemService(Context.NOTIFICATION_SERVICE);
        // mId allows you to update the notification later on.
        notificationManager.cancel(VIDEO_NOTIFICATION_ID);
        removeMoviePlayerPendingUpdateRunnable();
        resetPlayer(true);
        mVideoView.setContinuousPlay(false);
        mVideoView.stopPlayback();
        if(mRecommandDialog!=null && mRecommandDialog.isShowing()) {
            mRecommandDialog.dismiss();
            mRecommandDialog=null;
        }
        if(mAlertDialog!=null && mAlertDialog.isShowing()==true) {
            mAlertDialog.dismiss();
            mAlertDialog=null;
        }
        mActivity.unregisterReceiver(mRemovedReceiver);
    }

    public void onResume() {
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(Intent.ACTION_MEDIA_UNMOUNTED);
        intentFilter.addAction(Intent.ACTION_MEDIA_EJECT);
        intentFilter.addAction(Intent.ACTION_MEDIA_REMOVED);
        intentFilter.addAction(Intent.ACTION_MEDIA_SHARED);
        intentFilter.addAction(Intent.ACTION_MEDIA_BAD_REMOVAL);
        intentFilter.addDataScheme("file");
        mActivity.registerReceiver(mRemovedReceiver, intentFilter);

        intentFilter = new IntentFilter();
        //Network
            //WiFi
        intentFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
        intentFilter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
            //Ethernet
        intentFilter.addAction(EthernetManager.NETWORK_STATE_CHANGED_ACTION);
        intentFilter.addAction(EthernetManager.ETHERNET_STATE_CHANGED_ACTION);
            //Network
        intentFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
        intentFilter.addAction(ConnectivityManager.INET_CONDITION_ACTION);
        mActivity.registerReceiver(mRemovedReceiver, intentFilter);

        if (mHasPaused) {
            mController.showLoading();
            mHandler.removeCallbacks(mPlayingChecker);
            mHandler.postDelayed(mPlayingChecker, 250);
            mVideoView.resume();
            mVideoView.start();

            // If we have slept for too long, pause the play
            if (System.currentTimeMillis() > mResumeableTime) {
                pauseVideo(true, true);
            }
        }
        mHandler.removeCallbacks(mProgressChecker);
        mHandler.post(mProgressChecker);
    }

    public void onDestroy() {
        if (mVirtualizer != null) {
            mVirtualizer.release();
            mVirtualizer = null;
        }
        mAudioBecomingNoisyReceiver.unregister();
    }

    // This updates the time bar display (if necessary). It is called every
    // second by mProgressChecker and also from places where the time bar needs
    // to be updated immediately.
    private int setProgress() {
        if (!mBroadcastStatus && (mDragging || !mShowing || mSeekToTime != -1))
            return 0;

        int position = mVideoView.getCurrentPosition();
        int duration = mVideoView.getDuration();
        mController.setTimes(position, duration, 0, 0);
        return position;
    }

    private void startVideo() {
        mController.showLoading();
        mHandler.removeCallbacks(mPlayingChecker);
        mHandler.postDelayed(mPlayingChecker, 250);
        removeMoviePlayerPendingUpdateRunnable();
        mHandler.removeCallbacks(mCheckTrackInfo);
        mHandler.removeCallbacks(mProgressChecker);
//        nextVideoNotification();
        mCheckTrackInfoCount=0;
        mVideoView.start();
        setProgress();
    }

    private void playVideo(boolean bShowController,boolean bShowControlButtons) {
        mDelaySeek=true;
        mHandler.removeCallbacks(mSeekTo);
        mHandler.post(mSeekTo);
        mVideoView.start();
        mController.showPlaying(bShowController, bShowControlButtons);
        mHandler.removeCallbacks(mProgressChecker);
        mHandler.post(mProgressChecker);
    }

    private void pauseVideo(boolean bShowController,boolean bShowControlButtons) {
        int position = mVideoView.getCurrentPosition();
        int duration = mVideoView.getDuration();
        mController.setTimes(position, duration, 0, 0);
        mVideoView.pause();
        mController.showPaused(bShowController, bShowControlButtons);
        if(mSpeed != 0 && mSpeed != 1) {
            mSpeed=1;
            mVideoView.fastforward(mSpeed);
            showMovieHint(false, 0, 0, 0);
        }
    }

    public int getDuration()
    {
        return mVideoView.getDuration();
    }	

    public TrackInfo getVideoInfo()
    {
        ArrayList<TrackMap> videoList = new ArrayList<TrackMap>();
        videoList = getTrackList(TrackInfo.MEDIA_TRACK_TYPE_VIDEO);
        return (videoList.isEmpty())?null:videoList.get(0).info;
    }

    public void setMVC(boolean bEnabled)
    {
        mVideoView.setMVC(bEnabled);
    }

    private boolean checkTrackInfoValid()
    {
        if(mSpeed != 0 && mSpeed != 1) {
            mCheckTrackInfoCount = 0;
        }

        if(mCheckTrackInfoCount>=10)
            return true;
        if(mTrackInfo==null || (mTrackInfo!=null && mTrackInfo.length==0)) {
            return false;
        }
        boolean ret = true;
        if(checkVideoInfoValid()==false)
            ret = false;
        return ret;
    }

    private boolean checkVideoInfoValid()
    {
        if(mTrackInfo==null || (mTrackInfo!=null && mTrackInfo.length==0)) {
            return false;
        }
        boolean ret = true;
        TrackInfo videoInfo = getVideoInfo();
        if(videoInfo!=null) {
            MediaFormat mf = videoInfo.getFormat();
            if(mf!=null) {
                if(mf.containsKey(MediaFormat.KEY_WIDTH) && mf.containsKey(MediaFormat.KEY_HEIGHT)) {
                    if(mf.getInteger(MediaFormat.KEY_WIDTH)<=0 && mf.getInteger(MediaFormat.KEY_HEIGHT)<=0) {
                        ret = false;
                    }
                }
                else {
                    ret = false;
                }
                if(mRtkHDMIManager!=null && isWantedTVSystem(mRtkHDMIManager.getTVSystem(), 2160, 0)) { //4k tvsystem
                    if(mf.containsKey(MediaFormat.KEY_IS_PROGRESSIVE)) {
                        if(mf.getInteger(MediaFormat.KEY_IS_PROGRESSIVE, -1)<0) {
                            ret = false;
                        }
                    }
                    else {
                        ret = false;
                    }
                }
                if(mf.containsKey(MediaFormat.KEY_FRAME_RATE)) {
                    if(mf.getFloat(MediaFormat.KEY_FRAME_RATE)<=0) {
                        ret = false;
                    }
                }
                else {
                    ret = false;
                }
            }
            else {
                ret = false;
            }
        }
        else {
            ret = false;
        }
        return ret;
    }

    private ArrayList<TrackMap> getTrackList(int trackType)
    {
        if(mTrackInfo==null){
            mTrackInfo = mVideoView.getTrackInfo();
        }
        TrackInfo trackInfo[]=mTrackInfo;
        ArrayList<TrackMap> trackList = new ArrayList<TrackMap>();
        if(trackInfo!=null)
        {
            for(int count=0;count<trackInfo.length;count++)
            {
                if(trackInfo[count].getTrackType()==trackType)
                {
                    TrackMap trackMap = new TrackMap();
                    trackMap.info = trackInfo[count];
                    trackMap.index= count;
                    trackList.add(trackMap);
                }
            }
        }
        return trackList;
    }

    public int getSubtitleCount()
    {
        mVideoView.getSubtitleTrackInfo(true);
        mTrackInfo=null;
        mSubtitleList = getTrackList(TrackInfo.MEDIA_TRACK_TYPE_TIMEDTEXT);
        if(mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER)
            return mSubtitleList.size();
        else
            return 0;
    }

    public int getSubtitleCurrentIndex()
    {
        int ret = mVideoView.getSubtitleCurrentIndex();
        Log.d(TAG, "getSubtitleCurrentIndex, ret: "+ret);
        return ret;
    }

    public int IsTextSubtitle(int index)
    {
        if (mSubtitleList.size()==0)
            return SUBTITLE_NON_TEXT;

        if(index<0 || index>=mSubtitleList.size())
            index=0;
        TrackInfo trackInfo = mSubtitleList.get(index).info;
        if(trackInfo!=null) {
            MediaFormat mf = trackInfo.getFormat();
            int spuFrom=0;
            int spuType=0;
            if(mf!=null) {
                if(mf.containsKey(MediaFormat.KEY_SPU_FROM)) {
                    spuFrom = mf.getInteger(MediaFormat.KEY_SPU_FROM, 0);
                }
                if(mf.containsKey(MediaFormat.KEY_SPU_TYPE)) {
                    spuType = mf.getInteger(MediaFormat.KEY_SPU_TYPE, 0);
                }

                if (spuFrom == 1 /* EXTERNAL_SUBTITLE */
                    && spuType != 0 /* SUBTITLE_IDXSUB */
                    && spuType != 12 /* SUBTITLE_SUP */)
                {
                    return SUBTITLE_TEXT;
                }
                else if (spuFrom == 2 /* EMBEDDED_SUBTITLE */
                    && (spuType == 1 /* MEDIASUBTYPE_SRT */
                    || spuType == 3 /* MEDIASUBTYPE_SSA */
                    || spuType == 4 /* MEDIASUBTYPE_ASS */
                    || spuType == 6 /* MEDIASUBTYPE_TXT */))
                {
                    return SUBTITLE_TEXT;
                }
                else if (spuFrom == 2 /* EMBEDDED_SUBTITLE */ || spuFrom == 3 /* EMBEDDED_TS_DVB_SUBTITLE */)
                {
                    return SUBTITLE_OFFSET;
                }
            }
        }
        return SUBTITLE_NON_TEXT;
    }

    public int setSubtitleInfo(int index,int enable,int textEncoding,int testColor,int fontSize, int syncTime, int offset)
    {
        if(mSubtitleList.size()==0)
            return 0;
        if(checkTrackInfoValid()==false)
            return 0;

        if(checkInNavMenu()) {
            offset = 0 ;
            Log.d(TAG, "NAVI_DOMAIN_MENU subtitle offset is " + offset);
        }

        if(index<1 || index>mSubtitleList.size())
            index = 1;    //index 0 means subtitle off
        int trackIndex = mSubtitleList.get(index-1).index;

        if(mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER) {
            Log.d(TAG, "setSubtitleInfo: "+trackIndex+","+enable+","+textEncoding+","+testColor+","+fontSize+","+syncTime+","+offset);
            mVideoView.setSubtitleInfo(trackIndex, enable, textEncoding, testColor, fontSize, syncTime, offset);
        }
        else {
            Log.d(TAG, "setSubtitleInfo "+trackIndex);
            mVideoView.selectTrack(trackIndex);
        }

        if(enable==0)
            return 0;
        return index;
    }

    public int getAudioCount(boolean bForceUpdate)
    {
        if(bForceUpdate) {
            mVideoView.getAudioTrackInfo(true);
            mTrackInfo=null;
        }
        mAudioList = getTrackList(TrackInfo.MEDIA_TRACK_TYPE_AUDIO);
        return mAudioList.size();
    }

    public void setAudioTrack(int index)
    {
        if(index>=0 && index<mAudioList.size()) {
            int trackIndex = mAudioList.get(index).index;
            Log.d(TAG, "setAudioTrack "+trackIndex);
            mVideoView.selectTrack(trackIndex);
        }
        if(mExpandedView!=null) {
            mExpandedView.setTextViewText(R.id.audio, getAudioInfoDisplay());
//            updateVideoNotification();
        }
    }

    public String getSubtitleInfoDisplay(int index) {
        if(mSubtitleList.size()<=0)
            return "None";
        if(index>=mSubtitleList.size())
            return "Unknown";

        String subLang = mSubtitleList.get(index).info.getLanguage();
        if(subLang==null || subLang.equals("und") || subLang.length()<=1)
            subLang = "subtitle";
        String info = (index+1)+". "+subLang;
        return info;
    }

    public String getAudioInfoDisplay(int index, boolean includeLang) {
        if(mAudioList.size()<=0)
            return "None";
        if(index>=mAudioList.size())
            return "Unknown";

        String info = "";
        MediaFormat mf = mAudioList.get(index).info.getFormat();

        if(mf==null) {
            return "Unknown";
        }
        if(includeLang) {
            info = (index+1)+". ";
            if(mf.getString(MediaFormat.KEY_LANGUAGE)!=null) {
                String lang = mf.getString(MediaFormat.KEY_LANGUAGE);
                if(lang!=null && lang.equals("und"))
                    lang = "audio";
                info += lang + ": ";
            }
        }
        if(mf.getString(MediaFormat.KEY_MIME)!=null) {
            String mime = mf.getString(MediaFormat.KEY_MIME);
            if(mime!=null && mime.startsWith("audio/")) {
                mime = mime.substring(6);
            }
            info += mime + " " ;
        }
        if(mf.containsKey(MediaFormat.KEY_CHANNEL_COUNT)==true) {
            if(mf.getInteger(MediaFormat.KEY_CHANNEL_COUNT)>=0) {
                info += mf.getInteger(MediaFormat.KEY_CHANNEL_COUNT) + "ch ";
            }
        }
        if(mf.containsKey(MediaFormat.KEY_SAMPLE_RATE)==true) {
            if(mf.getInteger(MediaFormat.KEY_SAMPLE_RATE)>=0) {
                float sampleRate = (float)mf.getInteger(MediaFormat.KEY_SAMPLE_RATE)/1000;
                info += mFmt.format(sampleRate) + "KHz";
            }
        }
        return info;
    }

    private void useRTMediaPlayer(String uri) {
        if(uri.toLowerCase().indexOf(".iso") >= 0) {
            mVideoView.useRTMediaPlayer(MediaPlayer.FORCE_RT_MEDIAPLAYER);
        }
        else if(uri.toLowerCase().indexOf(".rm") >= 0 || uri.toLowerCase().indexOf(".rmvb") >= 0) {
            mVideoView.useRTMediaPlayer(MediaPlayer.FORCE_RT_MEDIAPLAYER);
        }
        else {
            mVideoView.useRTMediaPlayer(mUseRTMediaPlayer);
        }
    }

    // Below are notifications from VideoView
    @Override
    public boolean onError(MediaPlayer player, int arg1, int arg2) {
        // VideoView will show an error dialog if we return false, so no need
        // to show more message.
//        mController.showErrorMessage("");
//        return false;
        if (mSourceFrom != null && mSourceFrom.equals("DirectConnect")) {
            Log.d(TAG, "Playing AirFun video URL playlist, ignore error");
        } else {
            mVideoView.stopPlayback();
        }
        if(mToast!=null) {
            mToast.cancel();
        }
        mToast = Toast.makeText(mActivity, mActivity.getString(R.string.video_err),Toast.LENGTH_SHORT);
        mToast.show();
        mHandler.postDelayed(mOnCompletion, 2500);
        return true;
    }

    @Override
    public boolean onInfo(MediaPlayer mp, int what, int extra) {
        if(what==MediaPlayer.MEDIA_INFO_AUDIO_FATAL_ERROR && (extra&MediaPlayer.FATALERR_AUDIO)>0) {
            Log.d(TAG, "Audio Error Info: extra = " + extra);
            if(extra==MediaPlayer.UNKNOWN_FORMAT || extra==MediaPlayer.FATALERR_AUDIO_PASSTHROUGH_ONLY) {
                showMovieReminder(true, R.string.unknown_audio_format, VIDEO_REMINDER_TIMEOUT);
            }
        }
        if (what == MediaPlayer.MEDIA_INFO_BUFFERING_START) {
            mController.showLoading();
        } else if (what == MediaPlayer.MEDIA_INFO_BUFFERING_END) {
            mController.showEnded();
        }

        if(what==MediaPlayer.MEDIA_INFO_DOMAIN_UPDATE || what==MediaPlayer.MEDIA_INFO_TITLE_UPDATE) {
            Log.d(TAG, "MEDIA_INFO_UPDATE: " + what);
            mHandler.removeCallbacks(mCheckTrackInfo);
            mCheckTrackInfoCount=0;
            mHandler.postDelayed(mCheckTrackInfo, 1000);
        }
        return true;
    }

    @Override
    public void onCompletion(MediaPlayer mp) {
        boolean bComplete = true;
        mBookmarker.setBookmark(mUri, 0, mVideoView.getDuration());
        mController.showEnded();
        removeMoviePlayerPendingUpdateRunnable();
        mHandler.removeCallbacks(mCheckTrackInfo);
        mHandler.removeCallbacks(mProgressChecker);
        /* <--- Video playlist support ---- */
        if (mVideoUrls != null) {
            switch (mRepeatMode) {
            case REPEAT_MODE_OFF:
                mCurrentVideoIdx++;
            case REPEAT_MODE_ONE:
                bComplete = false;
                break;
            case REPEAT_MODE_ALL:
                mCurrentVideoIdx++;
                bComplete = false;
                if (mCurrentVideoIdx >= mVideoUrls.length) {
                    mCurrentVideoIdx=0;
                }
                break;
            default:
                break;
            }
        }

        if (bComplete == true || mUri.toString().length()<=7) {
            onCompletion();
        }
        else if (mCurrentVideoIdx >= mVideoUrls.length || mCurrentVideoIdx < 0) {
            if(mCurrentVideoIdx >= mVideoUrls.length) {
                mCurrentVideoIdx = mVideoUrls.length-1;
            }
            else if(mCurrentVideoIdx < 0) {
                mCurrentVideoIdx = 0;
            }
            onCompletion();
        } else if (mVideoUrls != null) {
            if(mRepeatMode!=REPEAT_MODE_ONE) {
                resetPlayer(true);
            }
            else {
                mSpeed=1;
                showMovieHint(false, 0, 0, 0);
                showMovieReminder(false, 0, 0);
            }

            mUri = Uri.parse(mVideoUrls[mCurrentVideoIdx]);
            useRTMediaPlayer(mUri.toString());
            if(GalleryUtils.hasSubtitlePlayListFromFile(mPlayListAddress)) {
                Uri uri;
                if(null != mDvdPlayerExtension && mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER) {
                    uri = Uri.parse(mUri.toString() + "&rtksubtitle=file:/" + mPlayListAddress + " " + mDvdPlayerExtension);
                } else {
                    uri = Uri.parse(mUri.toString() + "&rtksubtitle=file:/" + mPlayListAddress);
                }
                mVideoView.setVideoURI(uri);
            }
            else {
                if(null != mDvdPlayerExtension && mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER) {
                    Uri uri = Uri.parse(mUri.toString() + " " + mDvdPlayerExtension);
                    mVideoView.setVideoURI(uri);
                } else {
                    mVideoView.setVideoURI(mUri);
                }                
                
            }
            UpdateVideoRecentPlay(mUri.toString());
            startVideo();
        }
        /* ---- Video playlist support ---> */
    }

    public void onCompletion() {
        mCompletion=true;
        if (mActivity.mFinishOnCompletion) {
            //Toast.makeText(mActivity,"onCompletion",Toast.LENGTH_SHORT).show();
            //Log.d("MoviePlayer","\033[0;31;31m onCompletion \033[m ");
            removeMoviePlayerPendingUpdateRunnable();
            mVideoView.setContinuousPlay(false);
            mVideoView.stopPlayback();
            mActivity.finish();
        }
    }

    private void resetPlayer(boolean stopPlayback) {
        if(mToast!=null) {
            mToast.cancel();
            mToast = null;
        }
        mController.hide();
        mActivity.resetActivity();
        if(stopPlayback==true && mSourceFrom!=null && false == mSourceFrom.equals("DirectConnect")) {
            mVideoView.stopPlayback();
        }
        mSpeed=1;
        mActivity.mARFocus=MovieActivity.AR_FULL_INDEX;
        showMovieHint(false, 0, 0, 0);
        showMovieReminder(false, 0, 0);
        m3DType = MODE_3D_NOT_3DTV;
        mSeekToTime=-1;
        mHandler.removeCallbacks(mSeekTo);
        mDelaySeek=false;
        mSeeking=false;;
        clearVideoFrame();
        resetTVSystem();
        mShowChangeTvSystemDialog = true;
        mShowResumeDialog = true;
    }

    private void delaySeekTo(int seekToMs, int delayMs)
    {
        Log.d(TAG, "delaySeekTo: "+seekToMs);
        mSeekToTime = seekToMs;
        int duration = mVideoView.getDuration();
        int CurrentPosition = mVideoView.getCurrentPosition();
/*
        if(((mSeekToTime>=CurrentPosition)&&(duration-CurrentPosition)<=10000)){
            showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
            mSeekToTime= -1;
            return;
        }else if(mSeekToTime>=(duration-10000)) {
            mSeekToTime=(duration-10000);
            if(mSeekToTime<0)
                mSeekToTime=0;
        }
*/
        if(mSeekToTime<0) {
            mController.show(false,false);
            return;
        }
        else if(mSeekToTime>=duration && mSeekToTime>0 && duration>0) {
            mSeekToTime = duration;
        }
        mController.setTimes(mSeekToTime, duration, 0, 0);
        mDelaySeek=true;
        mHandler.removeCallbacks(mSeekTo);
        mHandler.postDelayed(mSeekTo, delayMs);
    }

    @Override
    public void onSeekResult(MediaPlayer mp, int result) {
        if(result==0) {
            if(!mActivity.isSubtitleMenuShowing())
                showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
            mSeekToTime = -1;
            setProgress();
        }
        else if(mDelaySeek==false) {
            mSeekToTime = -1;
            setProgress();
        }
        mSeeking=false;
    }

    @Override
    public void onSeekComplete(MediaPlayer mp) {
        Log.d(TAG, "Seek complete.");
        if(mDelaySeek==false) {
            mSeekToTime = -1;
            setProgress();
        }
        mSeeking=false;
    }

    public boolean isPlaying()
    {
        return mVideoView.isPlaying();
    }

    public Uri getPlayingUri()
    {
        return mUri;
    }

    public void hideController(){
        if (mVideoView.isPlaying()) {
            pauseVideo(false, false);
        }
        mController.showPaused(false, false);
    }
    
    // Below are notifications from ControllerOverlay
    @Override
    public void onPlayPause(boolean bShowController,boolean bShowControlButtons) {
        if (mVideoView.isPlaying()) {
            pauseVideo(bShowController, bShowControlButtons);
        } else {
            playVideo(bShowController, bShowControlButtons);
        }
    }

    @Override
    public void onStop() {
        mVideoPosition = mVideoView.getCurrentPosition();
        mBookmarker.setBookmark(mUri, mVideoPosition, mVideoView.getDuration());
        onCompletion();
    }

    @Override
    public void onInsReplay() {
        int pos;
        if(mSeekToTime!=-1) {
            pos = mSeekToTime;
            Log.d(TAG, "onInsReplay mSeekToTime = "+pos);
        }
        else {
            pos = mVideoView.getCurrentPosition();
            Log.d(TAG, "onInsReplay getCurrentPosition = "+pos);
        }
        pos -= mVideoTimeOffset; // milliseconds
        if(pos<0)
            pos=0;
        delaySeekTo(pos, 500);
        mSpeed=1;
        showMovieHint(false, 0, 0, 0);
    }

    @Override
    public void onCmSkip() {
        int pos;
        if(mSeekToTime!=-1)
        {
            pos = mSeekToTime;
            Log.d(TAG, "onCmSkip mSeekToTime = "+pos);
        }
        else {
            pos = mVideoView.getCurrentPosition();
            Log.d(TAG, "onCmSkip getCurrentPosition = "+pos);
        }
        pos += mVideoTimeOffset; // milliseconds
        int duration = mVideoView.getDuration();
        if(pos>duration)
            pos=duration;
        delaySeekTo(pos, 500);
        mSpeed=1;
        showMovieHint(false, 0, 0, 0);
    }

    public boolean setVideoSpeed(int speed) {
        boolean ret = true;
        if(speed<-FF_FR_MAX_SPEED || speed>FF_FR_MAX_SPEED)
        {
            Log.d(TAG, "setVideoSpeed out of range : speed("+speed+") ,Max("+FF_FR_MAX_SPEED+")");
            return false;
        }
        mSpeed = speed;
        if(speed>0) {
            ret = mVideoView.fastforward(speed);
        }
        else {
            ret = mVideoView.fastrewind(-1 * speed);
        }
        if(ret==false) {
            mSpeed=1;
            mVideoView.fastforward(mSpeed);
            showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
        }
        return ret;
    }

    @Override
    public void onFastRewind() {
        if (!mVideoView.isPlaying())
            return;

        int speed = mSpeed;
        if(speed == 0 || speed == 1)
            speed = -1;
        else if(speed > 0 || speed <= -FF_FR_MAX_SPEED)
            speed = 0;
        else
            speed *= 2;
        setVideoSpeed(speed);
    }

    @Override
    public void onFastForward() {
        if (!mVideoView.isPlaying())
            return;

        int speed = mSpeed;
        if(speed < 0)
            speed = 1;
        else if(speed == 0)
            speed = 2;
        else if(speed >= FF_FR_MAX_SPEED)
            speed = 1;
        else
            speed *= 2;
        setVideoSpeed(speed);
    }

    @Override
    public void onSpeedChanged(MediaPlayer mp,int speed) {
        Log.d(TAG, "onSpeedChanged, old Speed = "+mSpeed+", new Speed = "+speed);

        if(speed==0 || speed==1) {
            showMovieHint(true, mContext.getString(R.string.play), "", VIDEO_HINT_TIMEOUT);
        }
        else if (speed < 1) {
            showMovieHint(true, mContext.getString(R.string.fastrewind), (-1 * speed)+"X", 0);
        }
        else { // speed > 1
            showMovieHint(true, mContext.getString(R.string.fastforward), speed+"X", 0);
        }
        if(mSpeed == speed) {
            showMovieReminder(false, 0, 0);
        }
        mSpeed = speed;
    }
    
    @Override
    public void onNext() {
        //TODO: call RtkVideoVew's api
        Log.d(TAG, "OnNext");
        mSpeed=1;
        showMovieHint(false, 0, 0, 0);
        onNext(true);
    }

    private void onNext(boolean checkNaviControl) {
        if(checkNaviControl && setNaviControl(1)) {
            if(mVideoUrls != null && mVideoUrls.length>1) {
                showPrevNextReminder(true, mContext.getString(R.string.press_next_again), VIDEO_PREVNEXT_REMINDER_TIMEOUT);
                mNaviMode = NAVI_MODE_NEXT;
            }
            if(mExpandedView!=null) {
                mExpandedView.setTextViewText(R.id.chapter, getChapterDisplay());
//                updateVideoNotification();
            }
        }
        else if (mVideoUrls != null) {
            mController.hide();
            mCurrentVideoIdx++;
            if (mCurrentVideoIdx >= mVideoUrls.length && mRepeatMode != REPEAT_MODE_ALL) {
                mCurrentVideoIdx = mVideoUrls.length-1;
                onCompletion();
            }
            else {
                resetPlayer(true);
                if(mCurrentVideoIdx >= mVideoUrls.length)
                    mCurrentVideoIdx=0;
                mUri = Uri.parse(mVideoUrls[mCurrentVideoIdx]);
                useRTMediaPlayer(mUri.toString());
                if(GalleryUtils.hasSubtitlePlayListFromFile(mPlayListAddress)) {
                    Uri uri;
                    if(null != mDvdPlayerExtension && mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER) {
                        uri = Uri.parse(mUri.toString() + "&rtksubtitle=file:/" + mPlayListAddress + " " + mDvdPlayerExtension);
                    } else {
                        uri = Uri.parse(mUri.toString() + "&rtksubtitle=file:/" + mPlayListAddress);
                    }
                    mVideoView.setVideoURI(uri);
                }
                else {
                    if(null != mDvdPlayerExtension && mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER) {
                        Uri uri = Uri.parse(mUri.toString() + " " + mDvdPlayerExtension);
                        mVideoView.setVideoURI(uri);
                    } else {
                        mVideoView.setVideoURI(mUri);    
                    }                                      
                }
                UpdateVideoRecentPlay(mUri.toString());
                startVideo();
            }
        }
 }

    @Override
    public void onPrevious() {
        //TODO: call RtkVideoVew's api
        Log.d(TAG, "OnPrevious");
        mSpeed=1;
        showMovieHint(false, 0, 0, 0);
        if(setNaviControl(-1)) {
            if(mVideoUrls != null && mVideoUrls.length>1) {
                showPrevNextReminder(true, mContext.getString(R.string.press_prev_again), VIDEO_PREVNEXT_REMINDER_TIMEOUT);
                mNaviMode = NAVI_MODE_PREV;
            }
            if(mExpandedView!=null) {
                mExpandedView.setTextViewText(R.id.chapter, getChapterDisplay());
//                updateVideoNotification();
            }
        }
        else if (mVideoUrls != null) {
            mController.hide();
            mCurrentVideoIdx--;
            if (mCurrentVideoIdx < 0 && mRepeatMode != REPEAT_MODE_ALL) {
                mCurrentVideoIdx=0;
                onCompletion();
            }
            else {
                resetPlayer(true);
                if(mCurrentVideoIdx < 0)
                    mCurrentVideoIdx=mVideoUrls.length-1;
                mUri = Uri.parse(mVideoUrls[mCurrentVideoIdx]);
                useRTMediaPlayer(mUri.toString());
                if(GalleryUtils.hasSubtitlePlayListFromFile(mPlayListAddress)) {
                    Uri uri;
                    if(null != mDvdPlayerExtension && mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER) {
                        uri = Uri.parse(mUri.toString() + "&rtksubtitle=file:/" + mPlayListAddress + " " + mDvdPlayerExtension);
                    } else {
                        uri = Uri.parse(mUri.toString() + "&rtksubtitle=file:/" + mPlayListAddress);
                    }
                    mVideoView.setVideoURI(uri);
                }
                else {
                    if(null != mDvdPlayerExtension && mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER) {
                        Uri uri = Uri.parse(mUri.toString() + " " + mDvdPlayerExtension);
                        mVideoView.setVideoURI(uri);
                    } else {
                        mVideoView.setVideoURI(mUri);
                    }                                      
                }
                UpdateVideoRecentPlay(mUri.toString());
                startVideo();
            }
        }
    }
    
    @Override
    public void onRepeatModeChange(){
        //TODO
        if (mRepeatAllOrNone == true) {
            // For video URL play, set only repeat-none or repeat-all
            mRepeatMode = (mRepeatMode == REPEAT_MODE_OFF ? REPEAT_MODE_ALL : REPEAT_MODE_OFF);
        } else {
            mRepeatMode = (mRepeatMode+1)%3;
        }
        mActivity.setRepeatModePref(mRepeatMode);
        mController.setRepeatMode(mRepeatMode, true);
        mVideoView.setContinuousPlay((mRepeatMode == REPEAT_MODE_OFF ? false : true));
        Log.d(TAG, "onRepeatModeChange mRepeatMode = "+mRepeatMode);
        switch(mRepeatMode) {
            case REPEAT_MODE_OFF:
                showMovieHint(true, R.string.repeat, R.string.setting_off, VIDEO_HINT_TIMEOUT);
                mVideoView.setLooping(false);
                break;
            case REPEAT_MODE_ONE:
                showMovieHint(true, R.string.repeat, R.string.repeat_one, VIDEO_HINT_TIMEOUT);
                mVideoView.setLooping(true);
                break;
            case REPEAT_MODE_ALL:
                showMovieHint(true, R.string.repeat, R.string.repeat_all, VIDEO_HINT_TIMEOUT);
                mVideoView.setLooping(false);
                break;
            default:
                break;
        }
        if(mExpandedView!=null) {
            mExpandedView.setTextViewText(R.id.repeat_mode, getRepeatModeDisplay());
//            updateVideoNotification();
        }
    }

    private void keepRepeatSetting() {
        if(mRepeatMode!=REPEAT_MODE_ONE)
            return;
        mActivity.setSubtitleInfo();
        mActivity.setAudioInfo();
    }

    public void onZoom(){
        mActivity.processZoom();
    }

    public void removeMoviePlayerPendingUpdateRunnable(){
        if(mHandler!=null){
            Log.d(TAG, "\033[0;31;31m removeMoviePlayerPendingUpdateRunnable \033[m");
            // we are safe to call this function multiple times.
            mHandler.removeCallbacks(mUpdateInfoRunnable);
        }
    }

    private class DialogOnDismissListener implements DialogInterface.OnDismissListener {
        @Override
        public void onDismiss (DialogInterface dialog){
            freshHideSystemUi();
        }
    }

    @Override
    public void onShowOption() {
        mActivity.showOption();
    }
    

    @Override
    public void onSeekStart() {
        mDragging = true;
    }

    @Override
    public void onSeekMove(int time) {
        delaySeekTo(time, 0);
        if(!mVideoView.isPlaying()) {
            playVideo(false, false);
        }
        if(mBroadcastStatus==false) {
            mController.showPlaying(true, true);
        }
        else {
            mController.showPlaying(true, false);
        }
    }

    @Override
    public void onSeekEnd(int time, int start, int end) {
        mDragging = false;
        mSpeed=1;
        showMovieHint(false, 0, 0, 0);
    }

    @Override
    public void onShown() {
        mShowing = true;
        setProgress();
        showSystemUi(true);
    }

    @Override
    public void onHidden() {
        mShowing = false;
        showSystemUi(false);
        checkUIRootView();
    }

    @Override
    public void onReplay() {
        startVideo();
    }

    // Below are key events passed from MovieActivity.
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        Log.d(TAG,"Receive key event: "+keyCode);
        // Some headsets will fire off 7-10 events on a single click
        if (event.getRepeatCount() > 0 && keyCode!=KeyEvent.KEYCODE_DPAD_RIGHT && keyCode!=KeyEvent.KEYCODE_DPAD_LEFT) {
            return isMediaKey(keyCode);
        }
        switch (keyCode) {
            case KeyEvent.KEYCODE_HEADSETHOOK:
            case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
                if(mSpeed==0 || mSpeed==1) {
                    if (mVideoView.isPlaying()) {
                        pauseVideo(true, false);
                    } else {
                        playVideo(true, false);
                    }
                }
                else {
                    mSpeed=1;
                    mVideoView.fastforward(mSpeed);
                    playVideo(true, false);
                    showMovieHint(true, mContext.getString(R.string.play), "", VIDEO_HINT_TIMEOUT);
                }
                return true;
            case KeyEvent.KEYCODE_MEDIA_PAUSE:
                if (mVideoView.isPlaying()) {
                    pauseVideo(true, false);
                }
                return true;
            case KeyEvent.KEYCODE_MEDIA_PLAY:
                if(mSpeed==0 || mSpeed==1) {
                    if (!mVideoView.isPlaying()) {
                        playVideo(true, false);
                    }
                }
                else {
                    mSpeed=1;
                    mVideoView.fastforward(mSpeed);
                    showMovieHint(true, mContext.getString(R.string.play), "", VIDEO_HINT_TIMEOUT);
                }
                return true;
            case KeyEvent.KEYCODE_BACK:
                onStop();
                return false;
            case KeyEvent.KEYCODE_MEDIA_STOP:
                onStop();
                return true;
            case KeyEvent.KEYCODE_MEDIA_PREVIOUS:
                if(mBroadcastStatus==false) {
                    mController.show(true,false);
                    onPrevious();
                }
                return true;
            case KeyEvent.KEYCODE_MEDIA_NEXT:
                if(mBroadcastStatus==false) {
                    mController.show(true,false);
                    onNext();
                }
                return true;
            case KeyEvent.KEYCODE_MEDIA_FAST_FORWARD:
                if(mBroadcastStatus==false && !checkInNavMenu()) {
                    mController.show(true,false);
                    onFastForward();
                }else if(checkInNavMenu())
                    showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
                return true;
            case KeyEvent.KEYCODE_MEDIA_REWIND:
                if(mBroadcastStatus==false && !checkInNavMenu()) {
                    mController.show(true,false);
                    onFastRewind();
                }else if(checkInNavMenu())
                    showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
                return true;
            case KeyEvent.KEYCODE_MEDIA_REPEAT:
                if(mBroadcastStatus==false) {
                    onRepeatModeChange();
                }
                return true;
            case KeyEvent.KEYCODE_DPAD_RIGHT:
                if(checkInNavMenu()) {
                    if(mVideoView.deliverNaviControlCmd(MediaPlayer.NAVI_RIGHT, 0)==false) {
                        showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
                    }
                }
                else {
                    if(mActivity.isUnderZoomMode()){
                        mActivity.processDirectionKeyInZoomMode(keyCode);
                    }else{
                        if(mBroadcastStatus==false) {
                            mController.show(true,false);
                            mVideoTimeOffset = getVideoTimeOffset(event.getRepeatCount());
                            onCmSkip();
                            mHandler.removeCallbacks(mSeekTo);
                        }
                    }
                }
                return true;
            case KeyEvent.KEYCODE_DPAD_LEFT:
                if(checkInNavMenu()) {
                    if(mVideoView.deliverNaviControlCmd(MediaPlayer.NAVI_LEFT, 0)==false) {
                        showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
                    }
                }
                else {
                    if(mActivity.isUnderZoomMode()){
                        mActivity.processDirectionKeyInZoomMode(keyCode);
                    }else{
                        if(mBroadcastStatus==false) {
                            mController.show(true,false);
                            mVideoTimeOffset = getVideoTimeOffset(event.getRepeatCount());
                            onInsReplay();
                            mHandler.removeCallbacks(mSeekTo);
                        }
                    }
                }
                return true;
            case KeyEvent.KEYCODE_DPAD_UP:
                if(checkInNavMenu()) {
                    if(mVideoView.deliverNaviControlCmd(MediaPlayer.NAVI_UP, 0)==false) {
                        showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
                    }
                }
                else if(mActivity.isUnderZoomMode()){
                    mActivity.processDirectionKeyInZoomMode(keyCode);
                }
                else {
                    Log.d(TAG,"NAVI_SWITCH_CHANNEL previous");
                    mVideoView.deliverNaviControlCmd(MediaPlayer.NAVI_SWITCH_CHANNEL, 0);
                }
                return true;
            case KeyEvent.KEYCODE_DPAD_DOWN:
                if(checkInNavMenu()) {
                    if(mVideoView.deliverNaviControlCmd(MediaPlayer.NAVI_DOWN, 0)==false) {
                        showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
                    }
                }
                else if(mActivity.isUnderZoomMode()){
                    mActivity.processDirectionKeyInZoomMode(keyCode);
                }
                else {
                    Log.d(TAG,"NAVI_SWITCH_CHANNEL next");
                    mVideoView.deliverNaviControlCmd(MediaPlayer.NAVI_SWITCH_CHANNEL, 1);
                }
                return true;
            case KeyEvent.KEYCODE_DPAD_CENTER:
                if(checkInNavMenu()) {
                    if(mVideoView.deliverNaviControlCmd(MediaPlayer.NAVI_ENTER, 0)==false) {
                        showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
                    }
                    else {
                    }
                }
                return true;
            case KeyEvent.KEYCODE_SEARCH:
                onZoom();
                return true;
        }
        return false;
    }

    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if (keyCode==KeyEvent.KEYCODE_DPAD_RIGHT || keyCode==KeyEvent.KEYCODE_DPAD_LEFT) {
            if(mDelaySeek==true) {
                delaySeekTo(mSeekToTime, 500);
            }
        }
        return isMediaKey(keyCode);
    }

    private static boolean isMediaKey(int keyCode) {
        return keyCode == KeyEvent.KEYCODE_HEADSETHOOK
                || keyCode == KeyEvent.KEYCODE_MEDIA_PREVIOUS
                || keyCode == KeyEvent.KEYCODE_MEDIA_NEXT
                || keyCode == KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE
                || keyCode == KeyEvent.KEYCODE_MEDIA_PLAY
                || keyCode == KeyEvent.KEYCODE_MEDIA_PAUSE;
    }
    
    private int getVideoTimeOffset(int repeatCount) {
        int offset = 0;
        if(repeatCount < 10)
        	offset = VIDEO_TIME_OFFSET_10S;
        else if(repeatCount < 20)
        	offset = VIDEO_TIME_OFFSET_30S;
        else if(repeatCount < 40)
        	offset = VIDEO_TIME_OFFSET_120S;
        else
        	offset = VIDEO_TIME_OFFSET_300S;
        // Log.d(TAG, "Video_Time_Offset="+offset);
        return offset;
    }
	
    public void UpdateVideoRecentPlay(String path)
    {
        mUpdateRecentPlayTask = new UpdateRecentPlayTask(mActivity, path);
        mUpdateRecentPlayTask.execute();
    }

    private void clearVideoFrame()
    {
        if(mCompletion)
            return;
        if(mUseRTMediaPlayer!=MediaPlayer.FORCE_RT_MEDIAPLAYER && CLEAR_VIDEO_FRAME) {
            Log.d(TAG, "clearVideoFrame [Start]");
//            mVideoView.getHolder().setFormat(PixelFormat.TRANSPARENT);
//            mVideoView.getHolder().setFormat(PixelFormat.OPAQUE);
            mVideoView.setVisibility(View.INVISIBLE);
            mVideoView.postDelayed(new Runnable() {
                @Override
                public void run() {
                    mVideoView.setVisibility(View.VISIBLE);
                }
            }, BLACK_TIMEOUT);
            Log.d(TAG, "clearVideoFrame [End]");
        }
        else {
            Log.d(TAG, "mUseRTMediaPlayer:" + mUseRTMediaPlayer + ", clearVideoFrame:" +CLEAR_VIDEO_FRAME);
        }
    }

    public static class UpdateRecentPlayTask extends AsyncTask<Void, Void, Void> {
        private String mPath;
        private MovieActivity mActivity;

        public UpdateRecentPlayTask(MovieActivity activity, String path)
        {
            this.mActivity = activity;
            this.mPath = path;
        }

        @Override
        protected void onPreExecute()
        {
            super.onPreExecute();
        }

        @Override
        protected Void doInBackground(Void... para)
        {
            Log.d(TAG, "UpdateRecentPlayTask doInBackground [Start]");
            if (!isCancelled())
            {
                UpdateRecentPlay(mPath);
            }
            Log.d(TAG, "UpdateRecentPlayTask doInBackground [End]");
            return null;
        }

        @Override
        protected void onPostExecute (Void rlt)
        {
        }

        @Override
        protected void onCancelled()
        {
            super.onCancelled();
            Log.d(TAG,"UpdateRecentPlayTask onCancelled ");
        }
        @Override
        protected void finalize ()throws Throwable
        {
            super.finalize();
            Log.d(TAG,"UpdateRecentPlayTask finalize()");
        }

        protected void UpdateRecentPlay(String path)
        {
            Uri uri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
            try
            {
                String selection = MediaStore.MediaColumns.DATA + " LIKE ?";
                if (path.startsWith("file://")==true)
                {
                    path = path.substring(7);
                    //Log.d(TAG,"Path:"+path);
                }
                path = Uri.decode(path);
                String[] selectionArgs = { String.valueOf(path) };
                ContentResolver cr = this.mActivity.getContentResolver();
                ContentValues values = new ContentValues();
                Date date = new Date();
                values.put("date_recentplay", (date.getTime() / 1000));
                int row = cr.update(uri, values, selection, selectionArgs);
                Log.d(TAG, "UpdateVideoRecentPlay Path:"+ selectionArgs[0] + " date:"+values.getAsLong("date_recentplay") + " return:"+ row);
            }
            catch (Exception e)
            {
                Log.e(TAG, "Exception", e);
            }
        }
    }

    private String getTitleDisplay() {
        NaviInfo nav = mVideoView.getNavigationInfo();
        NavigationRecord navRecord=null;
        String ret = "1/1";
        if(nav!=null) {
            navRecord = nav.getRecord();
        }
        if(navRecord!=null) {
            int totalNum = (navRecord.containsKey(NavigationRecord.KEY_NUM_OF_TITLES))?navRecord.getInteger(NavigationRecord.KEY_NUM_OF_TITLES, 0):0;
            int current = (navRecord.containsKey(NavigationRecord.KEY_CURRENT_TITLE))?navRecord.getInteger(NavigationRecord.KEY_CURRENT_TITLE, 0):0;
            if(totalNum>0 && current>0)
                ret = current+"/"+totalNum;
        }
        return ret;
    }

    private String getChapterDisplay() {
        NaviInfo nav = mVideoView.getNavigationInfo();
        NavigationRecord navRecord=null;
        String ret = "1/1";
        if(nav!=null) {
            navRecord = nav.getRecord();
        }
        if(navRecord!=null) {
            mChapterTotalNum = (navRecord.containsKey(NavigationRecord.KEY_NUM_OF_CHAPTERS))?navRecord.getInteger(NavigationRecord.KEY_NUM_OF_CHAPTERS, 0):0;
            mChapterCurrent = (navRecord.containsKey(NavigationRecord.KEY_CURRENT_CHAPTER))?navRecord.getInteger(NavigationRecord.KEY_CURRENT_CHAPTER, 0):0;
            if(mChapterTotalNum>0 && mChapterCurrent>0)
                ret = mChapterCurrent+"/"+mChapterTotalNum;
        }
        return ret;
    }

    private long getFolderSize(File f) {
        long size = 0;
        if (f.isDirectory()) {
            for (File file : f.listFiles()) {
                size += getFolderSize(file);
            }
        } else {
            size=f.length();
        }
        return size;
    }

    private String getFileSizeDisplay() {
        String value=null;
        String path = mUri.toString();
        if (path.startsWith("file://")==true)
        {
            path = path.substring(7);
            //Log.d(TAG,"Path:"+path);
        }
        path = Uri.decode(path);
        File file = new File(path);
        long Filesize=getFolderSize(file)/1024;//call function and convert bytes into Kb
        if(Filesize>=1024*1024)
            value=Filesize/(1024*1024)+" GB";
        else if(Filesize>=1024)
            value=Filesize/1024+" MB";
        else if(Filesize==0)
            value=null;
        else
            value=Filesize+" KB";
        return value;
    }

    private String getRepeatModeDisplay() {
        String ret = "";
        switch(mRepeatMode) {
            case REPEAT_MODE_OFF:
                ret=mActivity.getString(R.string.setting_off);
                break;
            case REPEAT_MODE_ONE:
                ret=mActivity.getString(R.string.repeat_one);
                break;
            case REPEAT_MODE_ALL:
                ret=mActivity.getString(R.string.repeat_all);
                break;
        }
        return ret;
    }

    private String getAudioInfoDisplay() {
        getAudioCount(false);
        return getAudioInfoDisplay(mActivity.getAudioIndex(), false);
    }

    private String getVideoInfoDisplay() {
        String ret = "";
        ArrayList<TrackMap> videoList = getTrackList(TrackInfo.MEDIA_TRACK_TYPE_VIDEO);

        if(videoList.size()<1)
            return "None";

        MediaFormat mf = videoList.get(0).info.getFormat();
        if(mf==null) {
            return "Unknown";
        }
        if(mf.containsKey(MediaFormat.KEY_MIME)==true && mf.getString(MediaFormat.KEY_MIME)!=null) {
            String mime = mf.getString(MediaFormat.KEY_MIME);
            if(mime.startsWith("video/")) {
                mime = mime.substring(6);
            }
            ret += mime + " " ;
        }
        if(mf.containsKey(MediaFormat.KEY_WIDTH)==true && mf.containsKey(MediaFormat.KEY_HEIGHT)==true) {
            if(mf.getInteger(MediaFormat.KEY_WIDTH)>0 && mf.getInteger(MediaFormat.KEY_HEIGHT)>0) {
                mWidth = mf.getInteger(MediaFormat.KEY_WIDTH);
                mHeight = mf.getInteger(MediaFormat.KEY_HEIGHT);
                ret += mWidth + "*" + mHeight + " ";
            }else{
                mWidth=-1;
                mHeight=-1;
            }
        }
        else {
            mWidth = -1;
            mHeight = -1;
        }
        if(mf.containsKey(MediaFormat.KEY_FRAME_RATE)==true) {
            float videofps = mf.getFloat(MediaFormat.KEY_FRAME_RATE);
            if(videofps>0) {
                ret += String.format("%.2f", videofps) + " fps ";
            }
        }
        return ret;
    }

    public int getVideoWidth() {
        return mWidth;
    }

    public int getVideoHeight() {
        return mHeight;
    }

    public void relayoutVideoView(){
        mVideoView.requestLayout();
    }

    private void updateNotificationBuilder()
    {
        String video_name;
        if(mVideoTitles!=null && mCurrentVideoIdx >=0 && mCurrentVideoIdx < mVideoTitles.length)
            video_name = mVideoTitles[mCurrentVideoIdx];
        else
            video_name = mUri.toString().substring(mUri.toString().lastIndexOf("/")+1);
/*
        // Creates an explicit intent for an Activity in your app
        Intent resultIntent = new Intent(mActivity, MovieActivity.class);
        resultIntent.setAction(Intent.ACTION_VIEW);
        resultIntent.setData(mUri);
        resultIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
        resultIntent.putExtra("index-hint", mCurrentVideoIdx);
        if(mVideoUrls!=null)
            resultIntent.putExtra(KEY_PLAYLIST, mVideoUrls);
        resultIntent.putExtra("SourceFrom", mSourceFrom);
        // The stack builder object will contain an artificial back stack for the
        // started Activity.
        // This ensures that navigating backward from the Activity leads out of
        // your application to the Home screen.
        TaskStackBuilder stackBuilder = TaskStackBuilder.create(mActivity);
        // Adds the back stack for the Intent (but not the Intent itself)
        stackBuilder.addParentStack(MovieActivity.class);
        // Adds the Intent that starts the Activity to the top of the stack
        stackBuilder.addNextIntent(resultIntent);
        PendingIntent resultPendingIntent = stackBuilder.getPendingIntent(0, PendingIntent.FLAG_UPDATE_CURRENT);
*/
        if(mBuilder==null) {
            mBuilder = new Notification.Builder(mActivity)
                .setSmallIcon(R.drawable.infobar_video)
                .setAutoCancel(false)
                .setOngoing(true)
//                .setContentIntent(resultPendingIntent)
                .setContentTitle("Video")
                .setContentText(video_name)
                .build();
        }

        // Create remote view and set bigContentView.
        if(mExpandedView==null) {
            mExpandedView = new RemoteViews(mActivity.getPackageName(), R.layout.video_notification);
        }
        mExpandedView.setTextViewText(R.id.filepath, video_name);
        mExpandedView.setTextViewText(R.id.audio, getAudioInfoDisplay());
        mExpandedView.setTextViewText(R.id.title, getTitleDisplay());
        mExpandedView.setTextViewText(R.id.chapter, getChapterDisplay());
        mExpandedView.setTextViewText(R.id.video, getVideoInfoDisplay());
        if(mSourceFrom!=null && !mSourceFrom.equals("Network")) {
            String fileSizeDisplay = getFileSizeDisplay();
            if(fileSizeDisplay==null) {
                mExpandedView.setTextViewText(R.id.size_tag, "");
                mExpandedView.setTextViewText(R.id.size, "");
            }
            else {
                mExpandedView.setTextViewText(R.id.size_tag, mContext.getString(R.string.size));
                mExpandedView.setTextViewText(R.id.size, fileSizeDisplay);
            }
        }
        mExpandedView.setTextViewText(R.id.repeat_mode, getRepeatModeDisplay());
        mBuilder.contentView = mExpandedView;
        mBuilder.bigContentView = mExpandedView;
        // BUG_FIX: 43809 add extra info
        Bundle extras = new Bundle();
        extras.putString("RTK_ACTIVITY_INFO","MovieActivity");
        mBuilder.extras=extras;
    }

    private void nextVideoNotification()
    {
        String video_name;
        if(mVideoTitles!=null && mCurrentVideoIdx >=0 && mCurrentVideoIdx < mVideoTitles.length)
            video_name = mVideoTitles[mCurrentVideoIdx];
        else
            video_name = mUri.toString().substring(mUri.toString().lastIndexOf("/")+1);
        mExpandedView = new RemoteViews(mActivity.getPackageName(), R.layout.video_notification);
        mExpandedView.setTextViewText(R.id.filepath, video_name);
        mExpandedView.setTextViewText(R.id.audio, "");
        mExpandedView.setTextViewText(R.id.title, "");
        mExpandedView.setTextViewText(R.id.chapter, "");
        mExpandedView.setTextViewText(R.id.video, "");
        mExpandedView.setTextViewText(R.id.size_tag, "");
        mExpandedView.setTextViewText(R.id.size, "");
    }

    private void updateVideoNotification()
    {
        if(mActivity==null || mBuilder==null)
            return;
        NotificationManager notificationManager =
            (NotificationManager) mActivity.getSystemService(Context.NOTIFICATION_SERVICE);
        // mId allows you to update the notification later on.
        notificationManager.notify(VIDEO_NOTIFICATION_ID, mBuilder);
    }

    private void showMovieHint(boolean enable, String tag, String hint, int delayHideMs)
    {
        if(enable) {
            TextView movieTag = (TextView)mMovieHint.findViewById(R.id.movie_hint_tag);
            TextView movieHint = (TextView)mMovieHint.findViewById(R.id.movie_hint);
            movieTag.setText(tag);
            movieHint.setText(hint);
            if(mMovieHint.getVisibility()!=View.VISIBLE) {
                mMovieHint.clearAnimation();
                mContainerAnimIn.reset();
                mMovieHint.startAnimation(mContainerAnimIn);
                mMovieHint.setVisibility(View.VISIBLE);
            }
            mHandler.removeCallbacks(mHideMovieHint);
            if(delayHideMs > 0)
                mHandler.postDelayed(mHideMovieHint, delayHideMs);
        }
        else {
            if(mMovieHint.getVisibility()!=View.INVISIBLE) {
                mMovieHint.clearAnimation();
                mContainerAnimOut.reset();
                mMovieHint.startAnimation(mContainerAnimOut);
                mMovieHint.setVisibility(View.INVISIBLE);
                if(mSpeed!=0 && mSpeed!=1) {
                    if(mSpeed>1)
                        showMovieHint(true, mContext.getString(R.string.fastforward), mSpeed+"X", 0);
                    else
                        showMovieHint(true, mContext.getString(R.string.fastrewind), (-1 * mSpeed)+"X", 0);
                }
            }
            checkUIRootView();
        }
    }

    private void showMovieHint(boolean enable, int stringIdTag, int stringIdHint, int delayHideMs)
    {
        if(stringIdTag!=0 && stringIdHint!=0)
            showMovieHint(enable, mContext.getString(stringIdTag), mContext.getString(stringIdHint), delayHideMs);
        else
            showMovieHint(enable, "", "", delayHideMs);
    }

    private void showMovieReminder(boolean enable, String reminder, int delayHideMs)
    {
        if(enable) {
            TextView movieReminder = (TextView)mMovieReminder.findViewById(R.id.movie_reminder);
            movieReminder.setText(reminder);
            if(mMovieReminder.getVisibility()!=View.VISIBLE) {
                if(mPrevNextReminder.getVisibility()==View.VISIBLE) {
                    mPrevNextReminder.setVisibility(View.INVISIBLE);
                }
                mMovieReminder.clearAnimation();
                mContainerAnimIn.reset();
                mMovieReminder.startAnimation(mContainerAnimIn);
                mMovieReminder.setVisibility(View.VISIBLE);
            }
            mHandler.removeCallbacks(mHideMovieReminder);
            if(delayHideMs > 0)
                mHandler.postDelayed(mHideMovieReminder, delayHideMs);
        }
        else {
            mNaviMode=NAVI_MODE_OFF;
            if(mMovieReminder.getVisibility()!=View.INVISIBLE) {
                mMovieReminder.clearAnimation();
                mContainerAnimOut.reset();
                mMovieReminder.startAnimation(mContainerAnimOut);
                mMovieReminder.setVisibility(View.INVISIBLE);
            }
            checkUIRootView();
        }
    }

    private void showMovieReminder(boolean enable, int stringIdReminder, int delayHideMs)
    {
        if(stringIdReminder!=0)
            showMovieReminder(enable, mContext.getString(stringIdReminder), delayHideMs);
        else
            showMovieReminder(enable, "", delayHideMs);
    }

    private void showPrevNextReminder(boolean enable, String reminder, int delayHideMs)
    {
        if(enable) {
            TextView prevNextReminder = (TextView)mPrevNextReminder.findViewById(R.id.movie_reminder);
            prevNextReminder.setText(reminder);
            if(mPrevNextReminder.getVisibility()!=View.VISIBLE) {
                if(mMovieReminder.getVisibility()==View.VISIBLE) {
                    mMovieReminder.setVisibility(View.INVISIBLE);
                }
                mPrevNextReminder.clearAnimation();
                mContainerAnimIn.reset();
                mPrevNextReminder.startAnimation(mContainerAnimIn);
                mPrevNextReminder.setVisibility(View.VISIBLE);
            }
            mHandler.removeCallbacks(mHidePrevNextReminder);
            if(delayHideMs > 0)
                mHandler.postDelayed(mHidePrevNextReminder, delayHideMs);
        }
        else {
            mNaviMode=NAVI_MODE_OFF;
            if(mPrevNextReminder.getVisibility()!=View.INVISIBLE) {
                mPrevNextReminder.clearAnimation();
                mContainerAnimOut.reset();
                mPrevNextReminder.startAnimation(mContainerAnimOut);
                mPrevNextReminder.setVisibility(View.INVISIBLE);
            }
            checkUIRootView();
        }
    }

    public boolean isHintShowing() {
        if(mMovieHint!=null && mMovieHint.getVisibility()==View.VISIBLE) {
            return true;
        }
        else if(mMovieReminder!=null && mMovieReminder.getVisibility()==View.VISIBLE) {
            return true;
        }
        else if(mAlertDialog!=null && mAlertDialog.isShowing()) {
            return true;
        }
        else if(mRecommandDialog!=null && mRecommandDialog.isShowing()) {
            return true;
        }
        else if(mToast!=null && mToast.getView().getWindowVisibility()==View.VISIBLE) {
            return true;
        }
        else {
            return false;
        }
    }

    public void onRestart(String videoUrls[]) {
        if (videoUrls != null) {
            mVideoUrls = videoUrls;
        }
        mCurrentVideoIdx = 0;
        useRTMediaPlayer(mUri.toString());
        if(GalleryUtils.hasSubtitlePlayListFromFile(mPlayListAddress)) {
            Uri uri;
            if(null != mDvdPlayerExtension && mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER) {
                uri = Uri.parse(mVideoUrls[mCurrentVideoIdx] + "&rtksubtitle=file:/" + mPlayListAddress + " " + mDvdPlayerExtension);
            } else {
                uri = Uri.parse(mVideoUrls[mCurrentVideoIdx] + "&rtksubtitle=file:/" + mPlayListAddress);
            }
            mVideoView.setVideoURI(uri);
        }
        else {
            if(null != mDvdPlayerExtension && mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER) {
                Uri uri = Uri.parse(mUri.toString() + " " + mDvdPlayerExtension);
                mVideoView.setVideoURI(uri);
            } else {
                mVideoView.setVideoURI(mUri);
            }                       
        }
        startVideo();
    }

    public void onUpdatePlaylist(String videoUrls[]) {
        if (videoUrls != null) {
            mVideoUrls = videoUrls;
        }
    }

    private boolean isWantedTVSystem(int tvSystem, int resolution, int fps)
    {
        int mResolution = 0;
        int mFreq = 0;
        boolean ret = false;
        switch(tvSystem) {
            case RtkTVSystem.TV_SYS_NTSC: // 1
            case RtkTVSystem.TV_SYS_480P: // 3
                mResolution = 480;
                mFreq = 60;
                break;
            case RtkTVSystem.TV_SYS_PAL: // 2
            case RtkTVSystem.TV_SYS_576P: // 4
                mResolution = 576;
                mFreq = 60;
                break;
            case RtkTVSystem.TV_SYS_720P_50HZ: // 5
                mResolution = 720;
                mFreq = 50;
                break;
            case RtkTVSystem.TV_SYS_720P_59HZ: // 16
                mResolution = 720;
                mFreq = 59;
                break;
            case RtkTVSystem.TV_SYS_720P_60HZ: // 6
                mResolution = 720;
                mFreq = 60;
                break;
            case RtkTVSystem.TV_SYS_1080P_23HZ: // 18
                mResolution = 1080;
                mFreq = 23;
                break;
            case RtkTVSystem.TV_SYS_1080P_24HZ: // 15
                mResolution = 1080;
                mFreq = 24;
                break;
            case RtkTVSystem.TV_SYS_1080P_25HZ: // 29
                mResolution = 1080;
                mFreq = 25;
                break;
            case RtkTVSystem.TV_SYS_1080P_30HZ: // 30
                mResolution = 1080;
                mFreq = 30;
                break;
            case RtkTVSystem.TV_SYS_1080I_50HZ: // 7
            case RtkTVSystem.TV_SYS_1080P_50HZ: // 9
                mResolution = 1080;
                mFreq = 50;
                break;
            case RtkTVSystem.TV_SYS_1080I_59HZ: // 17
            case RtkTVSystem.TV_SYS_1080P_59HZ: // 19
                mResolution = 1080;
                mFreq = 59;
                break;
            case RtkTVSystem.TV_SYS_1080I_60HZ: // 8
            case RtkTVSystem.TV_SYS_1080P_60HZ: // 10
                mResolution = 1080;
                mFreq = 60;
                break;
            case RtkTVSystem.TV_SYS_2160P_23HZ: // 20
                mResolution = 2160;
                mFreq = 23;
                break;
            case RtkTVSystem.TV_SYS_2160P_24HZ: // 11 //3840*2160 24Hz
            case RtkTVSystem.TV_SYS_4096_2160P_24HZ: // 14    //4096*2160 24Hz
                mResolution = 2160;
                mFreq = 24;
                break;
            case RtkTVSystem.TV_SYS_2160P_25HZ: // 12 //3840*2160 25Hz
            case RtkTVSystem.TV_SYS_4096_2160P_25HZ: // 26
                mResolution = 2160;
                mFreq = 25;
                break;
            case RtkTVSystem.TV_SYS_2160P_29HZ: // 21
                mResolution = 2160;
                mFreq = 29;
                break;
            case RtkTVSystem.TV_SYS_2160P_30HZ: // 13 //3840*2160 30Hz
            case RtkTVSystem.TV_SYS_4096_2160P_30HZ: // 27
                mResolution = 2160;
                mFreq = 30;
                break;
            case RtkTVSystem.TV_SYS_2160P_50HZ: // 23
            case RtkTVSystem.TV_SYS_4096_2160P_50HZ: // 24
                mResolution = 2160;
                mFreq = 50;
                break;
            case RtkTVSystem.TV_SYS_2160P_59HZ: // 28
                mResolution = 2160;
                mFreq = 59;
                break;
            case RtkTVSystem.TV_SYS_2160P_60HZ: // 22
            case RtkTVSystem.TV_SYS_4096_2160P_60HZ: // 25
                mResolution = 2160;
                mFreq = 60;
                break;
            default:
                break;
        }

        if(resolution==0 || resolution == mResolution) {
            if(fps==0 || fps == mFreq) {
                ret = true;
            }
        }
        Log.d(TAG, "isWantedTVSystem: " + getTvSystemString(tvSystem) +", resolution:" + resolution + ", fps:" + fps + ", ret:" + ret);
        return ret;
    }

    private int getRecommandTvSystem() {
        int isProg=-1;
        int videoFps = 0;
        int recommandTvSystem = -1;

        if(mTvSystemForRecommand!=-1) {
            return mTvSystemForRecommand;
        }

        if(getVideoInfo()!=null) {
            MediaFormat mf = getVideoInfo().getFormat();
            if(mf!=null && mf.containsKey(MediaFormat.KEY_IS_PROGRESSIVE)) {
                isProg = mf.getInteger(MediaFormat.KEY_IS_PROGRESSIVE, -1);
            }
            if(mf!=null && mf.containsKey(MediaFormat.KEY_FRAME_RATE)) {
                float videoFpsFloat = mf.getFloat(MediaFormat.KEY_FRAME_RATE);
                if(Math.round(videoFpsFloat)==25) {
                    videoFpsFloat = 25;
                }
                videoFps = (int)videoFpsFloat;
            }
        }else{
            showMovieReminder(true, R.string.no_video_info, VIDEO_REMINDER_TIMEOUT);
        }

        int tvSystem = mRtkHDMIManager.getTVSystem();
        // change tv system to 1080p60 with interlace video
        if(isWantedTVSystem(tvSystem, 2160, 0)) { //4k tvsystem
            if(isProg==0) //interlace
                recommandTvSystem = RtkTVSystem.TV_SYS_1080P_60HZ;
        }

        // get auto1080P24 setting value:
        int auto1080P24=0;
        try{
            auto1080P24 = Settings.System.getInt(mContext.getContentResolver(), Settings.System.REALTEK_SETUP_AUTO_1080P24);
            Log.e(TAG, "REALTEK_SETUP_AUTO_1080P24: "+auto1080P24);
        }catch(android.provider.Settings.SettingNotFoundException e){
            Log.e(TAG, "setting not found for REALTEK_SETUP_AUTO_1080P24");
        }

        // get auto2997Hz setting value:
        int auto2997Hz=0;
        try{
            auto2997Hz = Settings.System.getInt(mContext.getContentResolver(), Settings.System.REALTEK_SETUP_AUTO_2997HZ);
            Log.e(TAG, "REALTEK_SETUP_AUTO_2997HZ: "+auto2997Hz);
        }catch(android.provider.Settings.SettingNotFoundException e){
            Log.e(TAG, "setting not found for REALTEK_SETUP_AUTO_1080P24");
        }

        // change tv system to 24Hz with 24 frame rate video
        if(auto1080P24==1 && (videoFps==23 || videoFps==24)) {
            if(videoFps==24 && isWantedTVSystem(tvSystem, 0, 24)) {
                recommandTvSystem=-1;
            }
            else if(videoFps==23 && isWantedTVSystem(tvSystem, 0, 23)) {
                recommandTvSystem=-1;
            }
            else if(isWantedTVSystem(tvSystem, 2160, 0)) { //4k tvsystem
                if(videoFps==23 && isTvSystemSupported(RtkTVSystem.TV_SYS_2160P_24HZ)==true) {
                    recommandTvSystem = RtkTVSystem.TV_SYS_2160P_23HZ;
                }
                else if(isTvSystemSupported(RtkTVSystem.TV_SYS_2160P_24HZ)==true)
                    recommandTvSystem = RtkTVSystem.TV_SYS_2160P_24HZ;
                else if(isTvSystemSupported(RtkTVSystem.TV_SYS_4096_2160P_24HZ)==true)
                    recommandTvSystem = RtkTVSystem.TV_SYS_4096_2160P_24HZ;
                else if(videoFps==23 && isTvSystemSupported(RtkTVSystem.TV_SYS_1080P_24HZ)==true) {
                    recommandTvSystem = RtkTVSystem.TV_SYS_1080P_23HZ;
                }
                else if(isTvSystemSupported(RtkTVSystem.TV_SYS_1080P_24HZ)==true)
                    recommandTvSystem = RtkTVSystem.TV_SYS_1080P_24HZ;
            }
            else if(videoFps==23 && isTvSystemSupported(RtkTVSystem.TV_SYS_1080P_24HZ)==true) {
                recommandTvSystem = RtkTVSystem.TV_SYS_1080P_23HZ;
            }
            else if(isTvSystemSupported(RtkTVSystem.TV_SYS_1080P_24HZ)==true) {
                recommandTvSystem = RtkTVSystem.TV_SYS_1080P_24HZ;
            }
        }
        else if(auto2997Hz==1 && (videoFps==29 || videoFps==59)) {
            if(isWantedTVSystem(tvSystem, 0, 59)) {
                recommandTvSystem=-1;
            }
            else if(videoFps==29 && isWantedTVSystem(tvSystem, 0, 29)) {
                recommandTvSystem=-1;
            }
            else if(isWantedTVSystem(tvSystem, 2160, 0)) { //4k tvsystem
                if(videoFps==29 && isTvSystemSupported(RtkTVSystem.TV_SYS_2160P_30HZ)==true)
                    recommandTvSystem = RtkTVSystem.TV_SYS_2160P_29HZ;
                else if(isTvSystemSupported(RtkTVSystem.TV_SYS_2160P_60HZ)==true)
                    recommandTvSystem = RtkTVSystem.TV_SYS_2160P_59HZ;
            }
            else if(isTvSystemSupported(RtkTVSystem.TV_SYS_1080P_60HZ)==true)
                recommandTvSystem = RtkTVSystem.TV_SYS_1080P_59HZ;
            else if(isTvSystemSupported(RtkTVSystem.TV_SYS_1080I_60HZ)==true)
                recommandTvSystem = RtkTVSystem.TV_SYS_1080I_59HZ;
            else if(isTvSystemSupported(RtkTVSystem.TV_SYS_720P_60HZ)==true)
                recommandTvSystem = RtkTVSystem.TV_SYS_720P_59HZ;
        }
        else if(auto2997Hz==1 && (videoFps==25 || videoFps==50)) {
            if(isWantedTVSystem(tvSystem, 0, 50) ) {
                recommandTvSystem=-1;
            }
            else if(videoFps==25 && isWantedTVSystem(tvSystem, 0, 25)) {
                recommandTvSystem=-1;
            }
            else if(isWantedTVSystem(tvSystem, 2160, 0)) { //4k tvsystem
                if(videoFps==25 && isTvSystemSupported(RtkTVSystem.TV_SYS_2160P_25HZ)==true)
                    recommandTvSystem = RtkTVSystem.TV_SYS_2160P_25HZ;
                else if(videoFps==25 && isTvSystemSupported(RtkTVSystem.TV_SYS_4096_2160P_25HZ)==true)
                    recommandTvSystem = RtkTVSystem.TV_SYS_4096_2160P_25HZ;
                else if(isTvSystemSupported(RtkTVSystem.TV_SYS_2160P_50HZ)==true)
                    recommandTvSystem = RtkTVSystem.TV_SYS_2160P_50HZ;
                else if(isTvSystemSupported(RtkTVSystem.TV_SYS_4096_2160P_50HZ)==true)
                    recommandTvSystem = RtkTVSystem.TV_SYS_4096_2160P_50HZ;
            }
            else if(videoFps==25 && isTvSystemSupported(RtkTVSystem.TV_SYS_1080P_25HZ)==true)
                recommandTvSystem = RtkTVSystem.TV_SYS_1080P_25HZ;
            else if(isTvSystemSupported(RtkTVSystem.TV_SYS_1080P_50HZ)==true)
                recommandTvSystem = RtkTVSystem.TV_SYS_1080P_50HZ;
            else if(isTvSystemSupported(RtkTVSystem.TV_SYS_1080I_50HZ)==true)
                recommandTvSystem = RtkTVSystem.TV_SYS_1080I_50HZ;
            else if(isTvSystemSupported(RtkTVSystem.TV_SYS_720P_50HZ)==true)
                recommandTvSystem = RtkTVSystem.TV_SYS_720P_50HZ;
        }

        if(tvSystem==recommandTvSystem) {
            Log.d(TAG, "TVSystem doesn't need to change. ("+tvSystem+")");
            recommandTvSystem=-1;
        }
        mTvSystemForRecommand = recommandTvSystem;
        Log.e(TAG, "getRecommandTvSystem: " + recommandTvSystem);
        return recommandTvSystem;
    }

    private boolean needChangeTvSystemDialog(int tvSystem) {
        boolean ret = true;
        int curTvSystem = mRtkHDMIManager.getTVSystem();
        if(isWantedTVSystem(curTvSystem, 2160, 0) && isWantedTVSystem(tvSystem, 2160, 0)) { //4k tvsystem
            ret=false;
        }
        if(isWantedTVSystem(curTvSystem, 1080, 0) && isWantedTVSystem(tvSystem, 1080, 0)) { //1080 tvsystem
            ret=false;
        }
        return ret;
    }

    private boolean isTvSystemSupported(int tvSystem) {
        boolean ret = false;
        if(mRtkHDMIManager != null && mRtkHDMIManager.checkIfHDMIPlugged()) {
            int[] supportVideoFormat = mRtkHDMIManager.getVideoFormat();
            if(tvSystem<0 || tvSystem>=supportVideoFormat.length) {
                if(tvSystem!=-1)
                    Log.d(TAG, "tvSystem ("+tvSystem+ ") is error");
                return false;
            }
            if(supportVideoFormat[tvSystem]==1) {
                Log.d(TAG, "tvSystem ("+tvSystem+ ") is supported.");
                ret = true;
            }
            else {
                Log.d(TAG, "tvSystem ("+tvSystem+ ") is not supported.");
                ret = false;
            }
        }
        return ret;
    }

    private String getTvSystemString(int tvSystem) {
        String ret="";
        switch(tvSystem) {
            case RtkTVSystem.TV_SYS_NTSC: // 1
                ret+="NTSC";
                break;
            case RtkTVSystem.TV_SYS_PAL: // 2
                ret+="PAL";
                break;
            case RtkTVSystem.TV_SYS_480P: // 3
                ret+="480p";
                break;
            case RtkTVSystem.TV_SYS_576P: // 4
                ret+="576p";
                break;
            case RtkTVSystem.TV_SYS_720P_50HZ: // 5
                ret+="720p 50Hz";
                break;
            case RtkTVSystem.TV_SYS_720P_60HZ: // 6
                ret+="720p 60Hz";
                break;
            case RtkTVSystem.TV_SYS_1080I_50HZ: // 7
                ret+="1080i 50Hz";
                break;
            case RtkTVSystem.TV_SYS_1080I_60HZ: // 8
                ret+="1080i 60Hz";
                break;
            case RtkTVSystem.TV_SYS_1080P_50HZ: // 9
                ret+="1080p 50Hz";
                break;
            case RtkTVSystem.TV_SYS_1080P_60HZ: // 10
                ret+="1080p 60Hz";
                break;
            case RtkTVSystem.TV_SYS_2160P_24HZ:	 // 11 //3840*2160 24Hz
                ret+="2160p 24Hz";
                break;
            case RtkTVSystem.TV_SYS_2160P_25HZ:	 // 12 //3840*2160 25Hz
                ret+="2160p 25Hz";
                break;
            case RtkTVSystem.TV_SYS_2160P_30HZ:	 // 13 //3840*2160 30Hz
                ret+="2160p 30Hz";
                break;
            case RtkTVSystem.TV_SYS_4096_2160P_24HZ: // 14 //4096*2160 24Hz
                ret+="4096*2160p 24Hz";
                break;
            case RtkTVSystem.TV_SYS_1080P_24HZ: // 15
                ret+="1080p 24Hz";
                break;
            case RtkTVSystem.TV_SYS_720P_59HZ: // 16
                ret+="720p 59.94Hz";
                break;
            case RtkTVSystem.TV_SYS_1080I_59HZ: // 17
                ret+="1080i 59.94Hz";
                break;
            case RtkTVSystem.TV_SYS_1080P_23HZ: // 18
                ret+="1080p 23.976Hz";
                break;
            case RtkTVSystem.TV_SYS_1080P_59HZ: // 19
                ret+="1080p 59.94Hz";
                break;
            case RtkTVSystem.TV_SYS_2160P_23HZ: // 20
                ret+="2160p 23.976Hz";
                break;
            case RtkTVSystem.TV_SYS_2160P_29HZ: // 21
                ret+="2160p 29.97Hz";
                break;
            case RtkTVSystem.TV_SYS_2160P_60HZ: // 22
                ret+="2160p 60Hz";
                break;
            case RtkTVSystem.TV_SYS_2160P_50HZ: // 23
                ret+="2160p 50Hz";
                break;
            case RtkTVSystem.TV_SYS_4096_2160P_50HZ: // 24
                ret+="4096*2160p 50Hz";
                break;
            case RtkTVSystem.TV_SYS_4096_2160P_60HZ: // 25
                ret+="4096*2160p 60Hz";
                break;
            case RtkTVSystem.TV_SYS_4096_2160P_25HZ: // 26
                ret+="4096*2160p 25Hz";
                break;
            case RtkTVSystem.TV_SYS_4096_2160P_30HZ: // 27
                ret+="4096*2160p 30Hz";
                break;
            case RtkTVSystem.TV_SYS_2160P_59HZ: // 28
                ret+="2160p 59Hz";
                break;
            case RtkTVSystem.TV_SYS_1080P_25HZ: // 29
                ret+="1080p 25Hz";
                break;
            case RtkTVSystem.TV_SYS_1080P_30HZ: // 30
                ret+="1080p 30Hz";
                break;
            default:
                ret="";
                break;
        }
        Log.d(TAG, "getTvSystemString("+tvSystem+"): "+ret);
        return ret;
    }

    private synchronized void changeTVSystem() {
        if(mRepeatMode==REPEAT_MODE_ONE && mTvSystemForRestored!=-1) {
            Log.d(TAG, "Skip change tvsystem with REPEAT_MODE_ONE and mTvSystemForRecommand="+mTvSystemForRecommand);
            return;
        }

        int recommandTvSystem = getRecommandTvSystem();
        if(mTvSystemForRestored>=0) {
            Log.e(TAG, "mTvSystemForRestored ("+mTvSystemForRestored+") is error !!");
            checkBookmark();
        }
        else if(mRtkHDMIManager!=null && recommandTvSystem>=0) {
            Log.d(TAG, "Change tvsystem to "+ recommandTvSystem);
            mTvSystemForRestored = mRtkHDMIManager.getTVSystemForRestored();
            Log.d(TAG, "Original tvsystem is "+ mTvSystemForRestored);
            if(mVideoView.isPlaying()) {
                Log.d(TAG, "Change tvsystem pause video");
                mVideoView.pause();
            }
            mRtkHDMIManager.setTVSystem(recommandTvSystem);
            mVideoView.postDelayed(new Runnable() {
                @Override
                public void run() {
                    mActivity.hideOption();
                    if(!mVideoView.isPlaying()) {
                        Log.d(TAG, "Change tvsystem start video");
                        mVideoView.start();
                    }
                    checkBookmark();
                    if(mToast!=null) {
                        mToast.cancel();
                    }
                    mToast = Toast.makeText(mActivity, mContext.getString(R.string.tv_system)+" "+getTvSystemString(mRtkHDMIManager.getTVSystem()) , Toast.LENGTH_LONG);
                    mToast.show();
                }
            }, VIDEO_CHANGE_TVSYSTEM_TIMEOUT);
        }
        else {
            Log.e(TAG, "mRtkHDMIManager ("+mRtkHDMIManager+") is error !!");
            checkBookmark();
        }
    }

    private synchronized void resetTVSystem() {
        if(mTvSystemFor3DRestored>=0 && mTvSystemForRestored<0) {
            mTvSystemForRestored = mTvSystemFor3DRestored;
        }
        mTvSystemFor3DRestored = -1;
        mTvSystemForRecommand = -1;

        if(get3DEnable()) {
            if(get3DType()==MoviePlayer.MODE_3D_MVC) {
                setMVC(false);
            }
            setFormat3d(RtkVoutUtilManager.FORMAT_3D_NULL);
        }

        if(mRtkHDMIManager!=null) {
            if(mTvSystemForRestored>=0) {
                Log.d(TAG, "Restore tvsystem to "+mTvSystemForRestored);
                mRtkHDMIManager.setTVSystem(mTvSystemForRestored);
                mTvSystemForRestored=-1;
            }
        }
    }

    public void checkSubtitleIdx() {
        mSubtitleList = getTrackList(TrackInfo.MEDIA_TRACK_TYPE_TIMEDTEXT);
        if(mSubtitleList.size()<=0)
            return;
        int subtitleIdx = mActivity.getSubtitleIndexPref(mUri.toString());
        if(subtitleIdx>=0) {
            mActivity.updateSubtitleIdx(subtitleIdx);
            return;
        }
        else {
            subtitleIdx = getSubtitleCurrentIndex();
        }

        if(subtitleIdx<0 || subtitleIdx>=mSubtitleList.size()) {
            subtitleIdx = 0;
        }
        else if(subtitleIdx>0) {
            Log.d(TAG, "checkSubtitleIdx: getSubtitleCurrentIndex = " + subtitleIdx);
            mActivity.updateSubtitleIdx(subtitleIdx);
            return;
        }

        Configuration conf = mActivity.getResources().getConfiguration();
        String osdLang = conf.locale.getLanguage();
        Log.d(TAG, "getOSDLanguage = " + osdLang);
        if(osdLang==null)
            return;

        for(int count=0;count<mSubtitleList.size();count++) {
            String subLang = mSubtitleList.get(count).info.getLanguage();
            if(subLang==null)
                continue;
            if(osdLang.equals("zh") && (subLang.startsWith("ch") || subLang.startsWith("zh"))) {
                subtitleIdx = count+1;
                break;
            }
            else if(osdLang.equals("ja") && subLang.equals("jpn")) {
                subtitleIdx = count+1;
                break;
            }
            else if(osdLang.equals("ko") && subLang.equals("kor")) {
                subtitleIdx = count+1;
                break;
            }
        }
        Log.d(TAG, "checkSubtitleIdx = " + subtitleIdx);
        if(subtitleIdx<=0 && (osdLang.equals("zh") || osdLang.equals("ja") || osdLang.equals("ko")))
            subtitleIdx=1;
        if(subtitleIdx>0)
            mActivity.updateSubtitleIdx(subtitleIdx);
    }

    public void check3DType() {
        boolean isTVSupport3D = false;
        int isMvc = 0;
        if(getVideoInfo()!=null) {
            MediaFormat mf = getVideoInfo().getFormat();
            if(mf!=null && mf.containsKey(MediaFormat.KEY_IS_MVC)) {
                isMvc = mf.getInteger(MediaFormat.KEY_IS_MVC, 0);
            }
        }
        if(mRtkHDMIManager != null) {
            isTVSupport3D = mRtkHDMIManager.isTVSupport3D();
        }

        if(isTVSupport3D) {
            if(isMvc==1) {
                m3DType = MODE_3D_MVC;
            }
            else {
                m3DType = MODE_3D_NOT_MVC;
            }
        }
        else {
            m3DType = MODE_3D_NOT_3DTV;
        }
        show3DHint();
        Log.d(TAG, "check3DType = " + m3DType);
    }

    public int get3DType() {
        return m3DType;
    }

    public boolean get3DEnable() {
        return m3DEnable;
    }

    public boolean setFormat3d(int mode) {
        boolean ret=true;
        if(mRtkVoutUtilManager != null) {
            int videoFps = 30;
            if(getVideoInfo()!=null) {
                MediaFormat mf = getVideoInfo().getFormat();
                if(mf!=null && mf.containsKey(MediaFormat.KEY_FRAME_RATE)) {
                    videoFps = (int)mf.getFloat(MediaFormat.KEY_FRAME_RATE);
                }
            }
            if(mode!=RtkVoutUtilManager.FORMAT_3D_NULL) {
                mTvSystemFor3DRestored = mRtkHDMIManager.getTVSystemForRestored();
            }
            mRtkHDMIManager.setHDMIFormat3D(mode, videoFps);
            mRtkVoutUtilManager.applyVoutDisplayWindowSetup();
            if(mode==RtkVoutUtilManager.FORMAT_3D_NULL) {
                if(getRecommandTvSystem()>=0 && mTvSystemFor3DRestored != getRecommandTvSystem()) {
                    changeTVSystem();
                }
                else if(mTvSystemFor3DRestored>=0 && mTvSystemFor3DRestored!=mRtkHDMIManager.getTVSystem()) {
                    Log.d(TAG, "Restore 3d tvsystem to "+mTvSystemFor3DRestored);
                    mRtkHDMIManager.setTVSystem(mTvSystemFor3DRestored);
                }
                else {
                    Log.d(TAG, "Doesn't restore 3d tvsystem to "+mTvSystemFor3DRestored);
                }
                mTvSystemFor3DRestored=-1;
                m3DEnable=false;
            }
            else {
                m3DEnable=true;
            }
        }
        return ret;
    }

    public boolean set3Dto2D(int mode) {
        boolean ret=false;
        if(mRtkVoutUtilManager!=null) {
            switch(mode) {
                case 0:     // 2D
                    ret=mRtkVoutUtilManager.set3dto2d(0);
                    break;
                case 1:     // sbs
                    ret=mRtkVoutUtilManager.set3dto2d(1);
                    break;
                case 2:     // tb
                    ret=mRtkVoutUtilManager.set3dto2d(2);
                    break;
                default:
                    break;
            }
        }
        Log.d(TAG, "set3Dto2D mode to "+mode+", ret:" + ret);
        return ret;
    }

    public boolean set3dSub(int mode) {
        boolean ret=false;
        if(mRtkVoutUtilManager != null && getVideoInfo()!=null) {
            ret=mRtkVoutUtilManager.set3dSub(mode);
        }
        return ret;
    }

    public void show3DHint() {
        if(m3DType == MODE_3D_MVC)
            showMovieReminder(true, R.string.the_movie_is_3d_video, VIDEO_REMINDER_TIMEOUT);
    }

    public boolean checkInNavMenu() {
        NaviInfo nav = mVideoView.getNavigationInfo();
        NavigationRecord navRecord=null;
        boolean ret = false;
        if(nav!=null) {
            navRecord = nav.getRecord();
        }
        if(navRecord!=null) {
            int navDomain = (navRecord.containsKey(NavigationRecord.KEY_DOMAIN))?navRecord.getInteger(NavigationRecord.KEY_DOMAIN, 0):0;
            if(navDomain==MediaPlayer.NAVI_DOMAIN_MENU) {
                Log.d(TAG, "Nav domain is NAVI_DOMAIN_MENU");
                ret = true;
            }
        }
        return ret;
    }

    public boolean setNaviControl(int addition) {

        if(mNaviMode==NAVI_MODE_PREV && addition==-1)
            return false;
        if(mNaviMode==NAVI_MODE_NEXT && addition==1)
            return false;

        NaviInfo nav = mVideoView.getNavigationInfo();
        NavigationRecord navRecord=null;
        boolean ret = false;
        if(nav!=null) {
            navRecord = nav.getRecord();
        }
        if(navRecord!=null) {
            int chapterNum = (navRecord.containsKey(NavigationRecord.KEY_NUM_OF_CHAPTERS))?navRecord.getInteger(NavigationRecord.KEY_NUM_OF_CHAPTERS, 0):0;
            if(chapterNum>1) {
                int curChapter = (navRecord.containsKey(NavigationRecord.KEY_CURRENT_CHAPTER))?navRecord.getInteger(NavigationRecord.KEY_CURRENT_CHAPTER, 0):0;
                if(curChapter+addition>0 && curChapter+addition<=chapterNum)
                {
                    Log.d(TAG,"NAVI_CHANGE_CHAPTER add "+addition+" to "+(curChapter+addition)+"/"+chapterNum);
                    if(mVideoView.deliverNaviControlCmd(MediaPlayer.NAVI_CHANGE_CHAPTER, curChapter+addition)==false) {
                        showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
                    }
                    ret = true;
                }
            }
        }
        return ret;
    }

    public ArrayList<String> getBDTitleList() {
        BDInfo bdInfo = mVideoView.getBDInfo();
        ArrayList<String> list = new ArrayList<String>();
        if(bdInfo!=null) {
            String str=new String();
            int hour=0, min=0, sec=0;
            int duration=0;
            int numOfTitles = bdInfo.getSize();
            for(int index=1;index<numOfTitles+1;index++)
            {
                duration = bdInfo.getDuration(index);
                str = mActivity.getString(R.string.bd_title)+":  ";
                str += mTwoDigitFmt.format(index) + "/" + mTwoDigitFmt.format(numOfTitles) + "  ";
                hour = duration/3600;
                min = (duration%3600)/60;
                sec = duration%60;
                str += mTwoDigitFmt.format(hour)+":"+mTwoDigitFmt.format(min)+":"+mTwoDigitFmt.format(sec);
                list.add(new String(str));
//                Log.d(TAG, index + " : " + str);
            }
        }
        return list;
    }

    public boolean setBDTitle(int index) {
        boolean ret = true;
        Log.d(TAG,"NAVI_CHANGE_TITLE:  "+index);
        if(mVideoView.deliverNaviControlCmd(MediaPlayer.NAVI_CHANGE_TITLE, index)==false) {
            ret = false;
            showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
        }
        else {
            resetPlayer(false);
            mHandler.removeCallbacks(mCheckTrackInfo);
            mCheckTrackInfoCount=0;
            mHandler.postDelayed(mCheckTrackInfo, 1000);
        }
        return ret;
    }

    public ArrayList<String> getBDChapterList() {
        BDInfo bdInfo = mVideoView.getBDChapterInfo();
        ArrayList<String> list = new ArrayList<String>();
        if(bdInfo!=null) {
            String str=new String();
            int hour=0, min=0, sec=0;
            int duration=0;
            int numOfTitles = bdInfo.getSize();
            for(int index=1;index<numOfTitles+1;index++)
            {
                duration = bdInfo.getDuration(index);
                str = mActivity.getString(R.string.bd_chapter)+":  ";
                str += mTwoDigitFmt.format(index) + "/" + mTwoDigitFmt.format(numOfTitles) + "  ";
                hour = duration/3600;
                min = (duration%3600)/60;
                sec = duration%60;
                str += mTwoDigitFmt.format(hour)+":"+mTwoDigitFmt.format(min)+":"+mTwoDigitFmt.format(sec);
                list.add(new String(str));
                Log.d(TAG, index + " : " + str);
            }
        }
        return list;
    }

    public boolean setBDChapter(int index) {
        boolean ret = true;
        Log.d(TAG,"NAVI_CHANGE_CHAPTER:  "+index);
        if(mVideoView.deliverNaviControlCmd(MediaPlayer.NAVI_CHANGE_CHAPTER, index)==false) {
            ret = false;
            showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
        }
        return ret;
    }

    // We want to pause when the headset is unplugged.
    private class AudioBecomingNoisyReceiver extends BroadcastReceiver {

        public void register() {
            IntentFilter intentFilter = new IntentFilter();
            intentFilter.addAction("android.intent.action.HDMI_PLUGGED");
            intentFilter.addAction(AudioManager.ACTION_AUDIO_BECOMING_NOISY);
            mContext.registerReceiver(this, intentFilter);
        }

        public void unregister() {
            mContext.unregisterReceiver(this);
        }

        @Override
        public void onReceive(Context context, Intent intent) {
            Log.d(TAG, "AudioBecomingNoisyReceiver: "+intent.getAction());
            boolean hdmiPlugged = intent.getBooleanExtra("state", false);
            Log.d(TAG, "HDMI plugged status: "+hdmiPlugged);
            if (mVideoView.isPlaying()) {
                // get setting value:
                int ignoreHDMIPlugOut=0;
                try{
                    ignoreHDMIPlugOut = Settings.System.getInt(context.getContentResolver(), Settings.System.REALTEK_IGNORE_HDMI_PLUGOUT);
                }catch(android.provider.Settings.SettingNotFoundException e){
                    Log.e(TAG, "setting not found for REALTEK_IGNORE_HDMI_PLUGOUT");
                }

                if(ignoreHDMIPlugOut<=0) {
                    if (intent.getAction().equals(AudioManager.ACTION_AUDIO_BECOMING_NOISY)) {
                        Log.d(TAG, "Pause video when audio becoming noisy");
                        pauseVideo(true, false);
                    }
                    else if (intent.getAction().equals("android.intent.action.HDMI_PLUGGED") && hdmiPlugged==false) {
                        Log.d(TAG, "Pause video when HDMI plug out");
                        pauseVideo(true, false);
                    }
                }
                else {
                    Log.d(TAG, "REALTEK_IGNORE_HDMI_PLUGOUT");
                }
            }
            if(intent.getAction().equals("android.intent.action.HDMI_PLUGGED") && hdmiPlugged==true) {
                mHandler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        if(mActivity.m3Dto2DMode > 0) {
                            set3Dto2D(mActivity.m3Dto2DMode);
                            set3dSub(mActivity.m3DSubMode);
                        }
                    }
                }, 1500);
            }
/*
            if(intent.getAction().equals("android.intent.action.HDMI_PLUGGED") && hdmiPlugged==false) {
                resetTVSystem();
                mTvSystemFor3DRestored=-1;
                m3DEnable=false;
            }
*/
        }

    }

    public void onDismiss (){
        Log.d(TAG, "RTKOptionMenu - OnDismiss");
        if(!mVideoView.isPlaying()) {
            playVideo(false, false);
        }
    }

    private final Runnable mABRepeatRunnable = new Runnable(){
        public void run (){
            if(mBUnderABRepeat && mABRepeatStartPos!=-1){
                mVideoView.seekTo(mABRepeatStartPos);
                mHandler.postDelayed(this,15000);
            }
        }
    };

    public void enableABRepeat(){
        mBUnderABRepeat=true;
        mABRepeatStartPos = mVideoView.getCurrentPosition();
        mHandler.removeCallbacks(mABRepeatRunnable);
        mHandler.postDelayed(mABRepeatRunnable,15000);
    }

    public void disableABRepeat(){
        mHandler.removeCallbacks(mABRepeatRunnable);
        mBUnderABRepeat=false;
        mABRepeatStartPos=-1;
    }

    public boolean bHasNaviMenu(){
        NaviType type = mVideoView.getNavigationType();
        boolean bDvd = false;
        if(type!=null && type.getType()!=null)
            bDvd = type.getType().isDvdMedia();
        Log.i(TAG, "\033[0;31;31m bHasNaviMenu:"+bDvd+"\033[m");
        return bDvd;
    }

    public void gotoNaviMenu(int menu){
        if(menu == MediaPlayer.NAVI_DVD_TITLE_MENU) {
            Log.d(TAG,"goto NAVI_DVD_TITLE_MENU");
            if(mVideoView.deliverNaviControlCmd(MediaPlayer.NAVI_DVD_TITLE_MENU, 0)==false) {
                showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
            }
        }
        else if(menu == MediaPlayer.NAVI_DVD_ROOT_MENU) {
            Log.d(TAG,"goto NAVI_DVD_ROOT_MENU");
            if(mVideoView.deliverNaviControlCmd(MediaPlayer.NAVI_DVD_ROOT_MENU, 0)==false) {
                showMovieReminder(true, R.string.video_couldnt_support_this_operation, VIDEO_HINT_TIMEOUT);
            }
        }
    }

    public void hideVideoView(){
        mVideoView.setVisibility(View.INVISIBLE);
    }

    public void freshHideSystemUi(){
        mHandler.removeCallbacks(mHideSystemUi);
        mHandler.postDelayed(mHideSystemUi, 5000);
    }

    private final Runnable mCheckUIRootView = new Runnable() {
        @Override
        public void run() {
            checkUIRootVisibility();
        }
    };

    private void checkUIRootView(){
        mHandler.removeCallbacks(mCheckUIRootView);
        mHandler.postDelayed(mCheckUIRootView, 500);
    }

    private void checkUIRootVisibility(){
        if(mUIRootView==null)
            return;
        boolean visibility = false;
        for(int i=0;i<mUIRootView.getChildCount();i++) {
            View child = mUIRootView.getChildAt(i);
            if(child.getVisibility()==View.VISIBLE) {
                visibility = true;
            }
        }
        if(visibility==false) {
            mUIRootView.setVisibility(View.GONE);
            mUIRootView.setVisibility(View.VISIBLE);
        }
        else {
            checkUIRootView();
        }
    }

    public ArrayList<String> getPropertyList()
    {
        mTrackInfo=null;
        ArrayList<String> list = new ArrayList<String>();
        String str=new String();

        str = mActivity.getString(R.string.file_name);
        str += " : \t\t";
        if(mVideoTitles!=null && mCurrentVideoIdx >=0 && mCurrentVideoIdx < mVideoTitles.length)
            str += mVideoTitles[mCurrentVideoIdx];
        else
            str += mUri.toString().substring(mUri.toString().lastIndexOf("/")+1);
        list.add(new String(str));

        str = mActivity.getString(R.string.video);
        str += " : \t\t";
        str += getVideoInfoDisplay();
        list.add(new String(str));

        str = mActivity.getString(R.string.audio);
        str += " : \t\t";
        str += getAudioInfoDisplay();
        list.add(new String(str));

        str = mActivity.getString(R.string.title);
        str += " : \t\t";
        str += getTitleDisplay();
        list.add(new String(str));

        str = mActivity.getString(R.string.chapter);
        str += " : \t\t";
        str += getChapterDisplay();
        list.add(new String(str));

        if(mSourceFrom!=null && !mSourceFrom.equals("Network")) {
            String fileSizeDisplay = getFileSizeDisplay();
            if(fileSizeDisplay!=null) {
                str = mActivity.getString(R.string.size);
                str += " : \t\t";
                str += fileSizeDisplay;
                list.add(new String(str));
            }
        }
        str = mActivity.getString(R.string.repeat_mode);
        str += " : \t\t";
        str += getRepeatModeDisplay();
        list.add(new String(str));
        return list;
    }

    class Bookmarker {
        private static final String TAG = "Bookmarker";

        private static final String BOOKMARK_CACHE_FILE = "bookmark";
        private static final int BOOKMARK_CACHE_MAX_ENTRIES = 100;
        private static final int BOOKMARK_CACHE_MAX_BYTES = 10 * 1024;
        private static final int BOOKMARK_CACHE_VERSION = 1;

        private static final int HALF_MINUTE = 30 * 1000;
        private static final int TWO_MINUTES = 4 * HALF_MINUTE;

        private final Context mContext;

        public Bookmarker(Context context) {
            mContext = context;
        }

        public void setBookmark(Uri uri, int bookmark, int duration) {
            if(bookmark<0 || duration<0) {
                return;
            }

            try {
                BlobCache cache = CacheManager.getCache(mContext,
                        BOOKMARK_CACHE_FILE, BOOKMARK_CACHE_MAX_ENTRIES,
                        BOOKMARK_CACHE_MAX_BYTES, BOOKMARK_CACHE_VERSION);

                ByteArrayOutputStream bos = new ByteArrayOutputStream();
                DataOutputStream dos = new DataOutputStream(bos);
                dos.writeUTF(uri.toString());
                dos.writeInt(bookmark);
                dos.writeInt(duration);
                dos.flush();
                cache.insert(uri.hashCode(), bos.toByteArray());
                if(bookmark<=0) {
                    mVideoView.removeBookmark(uri);
                }
                else {
                    mVideoView.setBookmark(uri);
                }
                Log.d(TAG, "setBookmark: "+bookmark);
            } catch (Throwable t) {
                Log.w(TAG, "setBookmark failed", t);
            }
        }

        public Integer getBookmark(Uri uri) {
            try {
                BlobCache cache = CacheManager.getCache(mContext,
                        BOOKMARK_CACHE_FILE, BOOKMARK_CACHE_MAX_ENTRIES,
                        BOOKMARK_CACHE_MAX_BYTES, BOOKMARK_CACHE_VERSION);

                byte[] data = cache.lookup(uri.hashCode());
                if (data == null) {
                    Log.d(TAG, "getBookmark data is null.");
                    return null;
                }

                DataInputStream dis = new DataInputStream(
                        new ByteArrayInputStream(data));

                String uriString = DataInputStream.readUTF(dis);
                int bookmark = dis.readInt();
                int duration = dis.readInt();
                Log.d(TAG, "getBookmark(), bookmark:"+bookmark+", duration:"+duration);

                if (!uriString.equals(uri.toString())) {
                    Log.d(TAG, "uriString:"+uriString+", uri.toString():"+uri.toString());
                    return null;
                }

                if ((bookmark < HALF_MINUTE) || (duration < TWO_MINUTES)
                        || (bookmark > (duration - HALF_MINUTE))) {
                    Log.d(TAG, "getBookmark failed, bookmark:"+bookmark+", duration:"+duration);
                    return null;
                }
                if(mUseRTMediaPlayer == MediaPlayer.FORCE_RT_MEDIAPLAYER) {
                    boolean checkBookmark = mVideoView.checkBookmark(uri);
                    if(checkBookmark==false) {
                        Log.d(TAG, "checkBookmark: "+checkBookmark);
                        return null;
                    }
                }
                Log.d(TAG, "getBookmark(): "+bookmark);
                return Integer.valueOf(bookmark);
            } catch (Throwable t) {
                Log.w(TAG, "getBookmark failed", t);
            }
            return null;
        }
    }
}

