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

package com.android.music;

import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.appwidget.AppWidgetManager;
import android.content.ComponentName;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.database.Cursor;
import android.database.sqlite.SQLiteException;
import android.graphics.Bitmap;
import android.media.audiofx.AudioEffect;
import android.media.AudioManager;
import android.media.AudioManager.OnAudioFocusChangeListener;
import android.media.MediaMetadataRetriever;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.RemoteControlClient;
import android.media.RemoteControlClient.MetadataEditor;
import android.net.Uri;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.PowerManager;
import android.os.SystemClock;
import android.os.PowerManager.WakeLock;
import android.os.RemoteException;
import android.provider.MediaStore;
import android.provider.Settings;
import android.util.Log;
import android.widget.RemoteViews;
import android.widget.Toast;
import android.net.NetworkInfo;
import android.net.ConnectivityManager;
import android.net.wifi.WifiManager;
import android.net.EthernetManager;

import java.io.FileDescriptor;
import java.io.IOException;
import java.io.PrintWriter;
import java.lang.ref.WeakReference;
import java.util.Random;
import java.util.Vector;
import java.util.Map;
import java.util.HashMap;
import com.realtek.app.AirFunManager;
import android.os.SystemProperties;

/**
 * Provides "background" audio playback capabilities, allowing the
 * user to switch between activities without stopping playback.
 */
public class MediaPlaybackService extends Service {
    /** used to specify whether enqueue() should start playing
     * the new list of files right away, next or once all the currently
     * queued files have been played
     */
    public static final int NOW = 1;
    public static final int NEXT = 2;
    public static final int LAST = 3;
    public static final int PLAYBACKSERVICE_STATUS = 1;
    
    public static final int SHUFFLE_NONE = 0;
    public static final int SHUFFLE_NORMAL = 1;
    public static final int SHUFFLE_AUTO = 2;
    
    public static final int REPEAT_NONE = 0;
    public static final int REPEAT_CURRENT = 1;
    public static final int REPEAT_ALL = 2;

    public static final String PLAYSTATE_CHANGED = "com.android.music.playstatechanged";
    public static final String REPEATSTATE_CHANGED = "com.android.music.repeatstatechanged";
    public static final String META_CHANGED = "com.android.music.metachanged";
    public static final String QUEUE_CHANGED = "com.android.music.queuechanged";
    public static final String PREPARE_COMPLETED = "com.android.music.prepared";

    public static final String SERVICECMD = "com.android.music.musicservicecommand";
    public static final String CMDNAME = "command";
    public static final String CMDTOGGLEPAUSE = "togglepause";
    public static final String CMDSTOP = "stop";
    public static final String CMDPAUSE = "pause";
    public static final String CMDPLAY = "play";
    public static final String CMDPREVIOUS = "previous";
    public static final String CMDNEXT = "next";
    public static final String CMDREPEAT = "repeat";
    public static final String CMDSEEK = "seek";
    public static final String CMDRESTART = "restart";
    public static final String CMDUPDATEPLAYLIST = "updatePlaylist";

    public static final String TOGGLEPAUSE_ACTION = "com.android.music.musicservicecommand.togglepause";
    public static final String PAUSE_ACTION = "com.android.music.musicservicecommand.pause";
    public static final String PREVIOUS_ACTION = "com.android.music.musicservicecommand.previous";
    public static final String NEXT_ACTION = "com.android.music.musicservicecommand.next";

    public static final String HDMI_PLUG_ACTION = "android.intent.action.HDMI_PLUGGED";
    public static final String SCREEN_OFF_ACTION = "android.intent.action.SCREEN_OFF";

    public static final String STATUS_TRACK_END = "com.android.music.trackend";
    public static final String STATUS_LIST_END = "com.android.music.listend";

    private static final String KEY_SOURCE_FROM = "SourceFrom";
    private static final String RTK_PLAYBACK_STATUS = "rtk.provider.playback.STATUS";
    private static final String RTK_PLAYBACK_STATUS_PLAY = "Play";
    private static final String RTK_PLAYBACK_STATUS_PAUSE = "Pause";
    private static final String RTK_PLAYBACK_STATUS_STOP = "Stop";

    private static final String KEY_MEDIA_CONFIG = "RTMediaPlayer.Config.";

    private static final int TRACK_ENDED = 1;
    private static final int RELEASE_WAKELOCK = 2;
    private static final int SERVER_DIED = 3;
    private static final int FOCUSCHANGE = 4;
    private static final int FADEDOWN = 5;
    private static final int FADEUP = 6;
    private static final int TRACK_WENT_TO_NEXT = 7;
    private static final int UPDATE_TIME_POSITION = 8;
    private static final int MAX_HISTORY_SIZE = 100;
    
    private static final int SOURCE_LOCAL = 0;
    private static final int SOURCE_NETWORK = 1;

    private static final int MEDIA_INFO_AUDIO_FATAL_ERROR = 807;

    private MultiPlayer mPlayer;
    private String mFileToPlay;
    private int mShuffleMode = SHUFFLE_NONE;
    private int mRepeatMode = REPEAT_NONE;
    private int mMediaMountedCount = 0;
    private long [] mAutoShuffleList = null;
    private long [] mIdPlayList = null;
    private String[] mUrlPlayList = null;
    private int mPlayListLen = 0;
    private Vector<Integer> mHistory = new Vector<Integer>(MAX_HISTORY_SIZE);
    private Cursor mCursor;
    private int mPlayPos = -1;
    private int mNextPlayPos = -1;
    private static final String LOGTAG = "MediaPlaybackService";
    private final Shuffler mRand = new Shuffler();
    private int mOpenFailedCounter = 0;
    String[] mCursorCols = new String[] {
            "audio._id AS _id",             // index must match IDCOLIDX below
            MediaStore.Audio.Media.ARTIST,
            MediaStore.Audio.Media.ALBUM,
            MediaStore.Audio.Media.TITLE,
            MediaStore.Audio.Media.DATA,
            MediaStore.Audio.Media.MIME_TYPE,
            MediaStore.Audio.Media.ALBUM_ID,
            MediaStore.Audio.Media.ARTIST_ID,
            MediaStore.Audio.Media.IS_PODCAST, // index must match PODCASTCOLIDX below
            MediaStore.Audio.Media.BOOKMARK    // index must match BOOKMARKCOLIDX below
    };
    String[] mPlaylistMemberCols = new String[] {
            MediaStore.Audio.Playlists.Members._ID,
            MediaStore.Audio.Media.TITLE,
            MediaStore.Audio.Media.DATA,
            MediaStore.Audio.Media.ALBUM,
            MediaStore.Audio.Media.ALBUM_ID,
            MediaStore.Audio.Media.ARTIST,
            MediaStore.Audio.Media.ARTIST_ID,
            MediaStore.Audio.Media.DURATION,
            MediaStore.Audio.Playlists.Members.PLAY_ORDER,
            MediaStore.Audio.Playlists.Members.AUDIO_ID,
            MediaStore.Audio.Media.IS_MUSIC
    };
    private final static int IDCOLIDX = 0;
    private final static int PODCASTCOLIDX = 8;
    private final static int BOOKMARKCOLIDX = 9;
    private BroadcastReceiver mUnmountReceiver = null;
    private BroadcastReceiver mNetworkReceiver = null;
    private WakeLock mWakeLock;
    private int mServiceStartId = -1;
    private boolean mServiceInUse = false;
    private boolean mIsSupposedToBePlaying = false;
    private boolean mQuietMode = false;
    private AudioManager mAudioManager;
    private boolean mQueueIsSaveable = true;
    // used to track what type of audio focus loss caused the playback to pause
    private boolean mPausedByTransientLossOfFocus = false;

    private SharedPreferences mPreferences;
    // We use this to distinguish between different cards when saving/restoring playlists.
    // This will have to change if we want to support multiple simultaneous cards.
    private int mCardId;
    private boolean mPreLoad = false;
    private boolean mIsPreparing = false;
    private MediaAppWidgetProvider mAppWidgetProvider = MediaAppWidgetProvider.getInstance();
    
    // interval after which we stop the service when idle
    private static final int IDLE_DELAY = 60000;

    private RemoteControlClient mRemoteControlClient;
    private String mOpenPath;
    private int mSourceFrom = SOURCE_LOCAL;
    private boolean mWifiAvailable = false;
    private boolean mEthAvailable = false;
    private boolean mBroadcastStatus = false;
    private String mSourceFromString = null;
    private String mMediaConfigString = null;
    private boolean mUseRTMediaPlayer = false;

    private class MusicTrackInfo {
        public String mArtistName = null;
        public String mAlbumName = null;
        public String mTrackName = null;
        
        public MusicTrackInfo(String artistName, String albumName, String trackName)
        {
            mArtistName = artistName;
            mAlbumName = albumName;
            mTrackName = trackName;
        }
    }

    MusicTrackInfo mTrackInfo = null;

    private Handler mMediaplayerHandler = new Handler() {
        float mCurrentVolume = 1.0f;
        @Override
        public void handleMessage(Message msg) {
            Log.d(LOGTAG, "mMediaplayerHandler.handleMessage " + msg.what);
            switch (msg.what) {
                case FADEDOWN:
                    mCurrentVolume -= .05f;
                    if (mCurrentVolume > .2f) {
                        mMediaplayerHandler.sendEmptyMessageDelayed(FADEDOWN, 10);
                    } else {
                        mCurrentVolume = .2f;
                    }
                    mPlayer.setVolume(mCurrentVolume);
                    break;
                case FADEUP:
                    mCurrentVolume += .01f;
                    if (mCurrentVolume < 1.0f) {
                        mMediaplayerHandler.sendEmptyMessageDelayed(FADEUP, 10);
                    } else {
                        mCurrentVolume = 1.0f;
                    }
                    mPlayer.setVolume(mCurrentVolume);
                    break;
                case SERVER_DIED:
                    if (mIsSupposedToBePlaying) {
                        gotoNext(true);
                    } else {
                        // the server died when we were idle, so just
                        // reopen the same song (it will start again
                        // from the beginning though when the user
                        // restarts)
                        openCurrentAndNext(mIdPlayList != null);
                    }
                    break;
                case TRACK_WENT_TO_NEXT:
                    mPlayPos = mNextPlayPos;
                    if (mCursor != null) {
                        mCursor.close();
                        mCursor = null;
                    }
                    if (mIdPlayList != null) {
                        mCursor = getCursorForId(mIdPlayList[mPlayPos]);
                    }
                    if (mUrlPlayList != null) {
                    	mFileToPlay = mUrlPlayList[mNextPlayPos];
                    }
                    notifyChange(META_CHANGED);
                    updateNotification();
                    setNextTrack(mIdPlayList != null);
                    break;
                case TRACK_ENDED:
                    Log.d(LOGTAG, "handleMessage - TRACK_ENDED");
                    gotoNext(false);
                    /*
                    if (mRepeatMode == REPEAT_CURRENT) {
                        seek(0);
                        play();
                        //gotoNext(false);
                    } else {
                        gotoNext(false);
                        Log.d(LOGTAG, "Send STATUS_TRACK_END");
                        notifyTrackEnd(STATUS_TRACK_END);
                    }
                    */
                    break;
                case RELEASE_WAKELOCK:
                    mWakeLock.release();
                    break;

                case FOCUSCHANGE:
                    // This code is here so we can better synchronize it with the code that
                    // handles fade-in
                    switch (msg.arg1) {
                        case AudioManager.AUDIOFOCUS_LOSS:
                            Log.v(LOGTAG, "AudioFocus: received AUDIOFOCUS_LOSS");
                            if(isPlaying()) {
                                mPausedByTransientLossOfFocus = false;
                            }
                            pause();
                            break;
                        case AudioManager.AUDIOFOCUS_LOSS_TRANSIENT_CAN_DUCK:
                            mMediaplayerHandler.removeMessages(FADEUP);
                            mMediaplayerHandler.sendEmptyMessage(FADEDOWN);
                            break;
                        case AudioManager.AUDIOFOCUS_LOSS_TRANSIENT:
                            Log.v(LOGTAG, "AudioFocus: received AUDIOFOCUS_LOSS_TRANSIENT");
                            if(isPlaying()) {
                                mPausedByTransientLossOfFocus = true;
                            }
                            pause();
                            break;
                        case AudioManager.AUDIOFOCUS_GAIN:
                            Log.v(LOGTAG, "AudioFocus: received AUDIOFOCUS_GAIN");
                            if(!isPlaying() && mPausedByTransientLossOfFocus) {
                                mPausedByTransientLossOfFocus = false;
                                mCurrentVolume = 0f;
                                mPlayer.setVolume(mCurrentVolume);
                                play(); // also queues a fade-in
                            } else {
                                mMediaplayerHandler.removeMessages(FADEDOWN);
                                mMediaplayerHandler.sendEmptyMessage(FADEUP);
                            }
                            break;
                        default:
                            Log.e(LOGTAG, "Unknown audio focus change code");
                    }
                    break;

                case UPDATE_TIME_POSITION:
                    {
                        long next = refreshTimePosition();
                        queueNextTimePositionRefresh(next);
                    }
                    break;

                default:
                    break;
            }
        }
    };

    private BroadcastReceiver mIntentReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            String cmd = intent.getStringExtra("command");
            Log.d(LOGTAG, "mIntentReceiver.onReceive " + action + " / " + cmd);
            if (CMDNEXT.equals(cmd) || NEXT_ACTION.equals(action)) {
                gotoNext(false);
            } else if (CMDPREVIOUS.equals(cmd) || PREVIOUS_ACTION.equals(action)) {
                prev();
            } else if (CMDTOGGLEPAUSE.equals(cmd) || TOGGLEPAUSE_ACTION.equals(action)) {
                if (isPlaying()) {
                    pause();
                    mPausedByTransientLossOfFocus = false;
                } else {
                    play();
                }
            } else if (CMDPAUSE.equals(cmd) || PAUSE_ACTION.equals(action)) {
                pause();
                mPausedByTransientLossOfFocus = false;
            } else if (CMDPLAY.equals(cmd)) {
                play();
            } else if (CMDSTOP.equals(cmd)) {
                gotoIdleState();
                stop(true);
                if (getPath() != null && getPath().toLowerCase().startsWith("directconnect://")) {
                    notifyTrackEnd(STATUS_LIST_END);
                }
                else {
                    notifyTrackEnd(STATUS_TRACK_END);
                }
                /*
                if (mIsSupposedToBePlaying) {
                    mIsSupposedToBePlaying = false;
                    notifyChange(PLAYSTATE_CHANGED);
                } else {
                    pause();
                    mPausedByTransientLossOfFocus = false;
                    seek(0);
                }
                */
            } else if (CMDSEEK.equals(cmd)) {
                long seekTime = intent.getIntExtra("seektime", -1);
                Log.d("Seek Command", "duration = " +  duration());
                Log.d("Seek Command", "seekTime = " +  seekTime);
                if (seekTime >= 0 && seekTime <= duration())
                {
                    seek(seekTime);
                }
            } else if (CMDREPEAT.equals(cmd)) {
                int repeatMode = (mRepeatMode == REPEAT_NONE ? REPEAT_ALL : REPEAT_NONE);
                setRepeatMode(repeatMode);
                Log.d(LOGTAG, "mIntentReceiver.onReceive - repeatMode = " + repeatMode);
            } else if (CMDRESTART.equals(cmd)) {
                String urlPlayList[] = intent.getStringArrayExtra("playlist-url");
                if (urlPlayList != null) {
                    open(urlPlayList, 0);
                    play();
                }
            } else if (CMDUPDATEPLAYLIST.equals(cmd)) {
                String urlPlayList[] = intent.getStringArrayExtra("playlist-url");
                if (urlPlayList != null) {
                    addToPlayList(urlPlayList, -1);
                }
            }
            else if (MediaAppWidgetProvider.CMDAPPWIDGETUPDATE.equals(cmd)) {
                // Someone asked us to refresh a set of specific widgets, probably
                // because they were just added.
                int[] appWidgetIds = intent.getIntArrayExtra(AppWidgetManager.EXTRA_APPWIDGET_IDS);
                mAppWidgetProvider.performUpdate(MediaPlaybackService.this, appWidgetIds);
            }
            else if (HDMI_PLUG_ACTION.equals(action)) {
                // get setting value:
                int ignoreHDMIPlugOut = 0;
                boolean hdmiPlugged = intent.getBooleanExtra("state", false);
                Log.d(LOGTAG, "HDMI plugged status: "+hdmiPlugged);
                try {
                    ignoreHDMIPlugOut = Settings.System.getInt(context.getContentResolver(), Settings.System.REALTEK_IGNORE_HDMI_PLUGOUT);
                    Log.d(LOGTAG, "mIntentReceiver.onReceive - ignoreHDMIPlugOut = " + ignoreHDMIPlugOut);
                } catch(android.provider.Settings.SettingNotFoundException e) {
                    Log.e(LOGTAG, "setting not found for REALTEK_IGNORE_HDMI_PLUGOUT");
                }

                if (ignoreHDMIPlugOut <= 0 && hdmiPlugged==false) {
                    pause();
                    mPausedByTransientLossOfFocus = false;
                }
            } else if (SCREEN_OFF_ACTION.equals(action)) {
                if ("1".equals(SystemProperties.get("persist.rtk.screenoff.suspend","0"))) {
                    stop(true);
                }
            }
        }
    };

    private OnAudioFocusChangeListener mAudioFocusListener = new OnAudioFocusChangeListener() {
        public void onAudioFocusChange(int focusChange) {
            mMediaplayerHandler.obtainMessage(FOCUSCHANGE, focusChange, 0).sendToTarget();
        }
    };

    public MediaPlaybackService() {
    }

    @Override
    public void onCreate() {
        Log.d(LOGTAG, "onCreate");
        super.onCreate();

        mAudioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
        ComponentName rec = new ComponentName(getPackageName(),
                MediaButtonIntentReceiver.class.getName());
        mAudioManager.registerMediaButtonEventReceiver(rec);

        Intent i = new Intent(Intent.ACTION_MEDIA_BUTTON);
        i.setComponent(rec);
        PendingIntent pi = PendingIntent.getBroadcast(this /*context*/,
                0 /*requestCode, ignored*/, i /*intent*/, 0 /*flags*/);
        mRemoteControlClient = new RemoteControlClient(pi);
        mAudioManager.registerRemoteControlClient(mRemoteControlClient);

        int flags = RemoteControlClient.FLAG_KEY_MEDIA_PREVIOUS
                | RemoteControlClient.FLAG_KEY_MEDIA_NEXT
                | RemoteControlClient.FLAG_KEY_MEDIA_PLAY
                | RemoteControlClient.FLAG_KEY_MEDIA_PAUSE
                | RemoteControlClient.FLAG_KEY_MEDIA_PLAY_PAUSE
                | RemoteControlClient.FLAG_KEY_MEDIA_STOP;
        mRemoteControlClient.setTransportControlFlags(flags);
        
        mPreferences = getSharedPreferences("Music", MODE_PRIVATE);
        mCardId = MusicUtils.getCardId(this);
        Log.d(LOGTAG, "onCreate - mCardId = " + mCardId);
        
        registerExternalStorageListener();
        registerNetworkListener();

        // Needs to be done in this thread, since otherwise ApplicationContext.getPowerManager() crashes.
        mPlayer = new MultiPlayer();
        mPlayer.setHandler(mMediaplayerHandler);

        reloadQueue();
        notifyChange(QUEUE_CHANGED);
        notifyChange(META_CHANGED);

        IntentFilter commandFilter = new IntentFilter();
        commandFilter.addAction(SERVICECMD);
        commandFilter.addAction(TOGGLEPAUSE_ACTION);
        commandFilter.addAction(PAUSE_ACTION);
        commandFilter.addAction(NEXT_ACTION);
        commandFilter.addAction(PREVIOUS_ACTION);
        commandFilter.addAction(HDMI_PLUG_ACTION);
        commandFilter.addAction(SCREEN_OFF_ACTION);
        registerReceiver(mIntentReceiver, commandFilter);
        
        PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);
        mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, this.getClass().getName());
        mWakeLock.setReferenceCounted(false);

        // If the service was idle, but got killed before it stopped itself, the
        // system will relaunch it. Make sure it gets stopped again in that case.
        Message msg = mDelayedStopHandler.obtainMessage();
        mDelayedStopHandler.sendMessageDelayed(msg, IDLE_DELAY);

        ConnectivityManager connectMgr = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = null;
        if (connectMgr != null) {
            networkInfo = connectMgr.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
            if (networkInfo != null) {
                mWifiAvailable = networkInfo.isAvailable();
                Log.d(LOGTAG, "onCreate - mWifiAvailable = " + mWifiAvailable);
            }
            networkInfo = connectMgr.getNetworkInfo(ConnectivityManager.TYPE_ETHERNET);
            if (networkInfo != null) {
                mEthAvailable = networkInfo.isAvailable();
                Log.d(LOGTAG, "onCreate - mEthAvailable = " + mEthAvailable);
            }
        }
    }

    @Override
    public void onDestroy() {
        Log.d(LOGTAG, "onDestroy");
        // Check that we're not being destroyed while something is still playing.
        if (isPlaying()) {
            Log.e(LOGTAG, "Service being destroyed while still playing.");
        }
        // release all MediaPlayer resources, including the native player and wakelocks
        Intent i = new Intent(AudioEffect.ACTION_CLOSE_AUDIO_EFFECT_CONTROL_SESSION);
        i.putExtra(AudioEffect.EXTRA_AUDIO_SESSION, getAudioSessionId());
        i.putExtra(AudioEffect.EXTRA_PACKAGE_NAME, getPackageName());
        sendBroadcast(i);
        mPlayer.release();
        mPlayer = null;

        mAudioManager.abandonAudioFocus(mAudioFocusListener);
        mAudioManager.unregisterRemoteControlClient(mRemoteControlClient);
        
        // make sure there aren't any other messages coming
        mDelayedStopHandler.removeCallbacksAndMessages(null);
        mMediaplayerHandler.removeCallbacksAndMessages(null);

        if (mCursor != null) {
            mCursor.close();
            mCursor = null;
        }

        unregisterReceiver(mIntentReceiver);
        if (mUnmountReceiver != null) {
            unregisterReceiver(mUnmountReceiver);
            mUnmountReceiver = null;
        }
        if (mNetworkReceiver != null) {
            unregisterReceiver(mNetworkReceiver);
            mNetworkReceiver = null;
        }
        mWakeLock.release();
        super.onDestroy();
    }
    
    private final char hexdigits [] = new char [] {
            '0', '1', '2', '3',
            '4', '5', '6', '7',
            '8', '9', 'a', 'b',
            'c', 'd', 'e', 'f'
    };

    private void saveQueue(boolean full) {
        Log.d(LOGTAG, "saveQueue");
        if (!mQueueIsSaveable || mUrlPlayList != null) {
            return;
        }

        Editor ed = mPreferences.edit();
        //long start = System.currentTimeMillis();
        if (full && mIdPlayList != null) {
            StringBuilder q = new StringBuilder();
            
            // The current playlist is saved as a list of "reverse hexadecimal"
            // numbers, which we can generate faster than normal decimal or
            // hexadecimal numbers, which in turn allows us to save the playlist
            // more often without worrying too much about performance.
            // (saving the full state takes about 40 ms under no-load conditions
            // on the phone)
            int len = mPlayListLen;
            for (int i = 0; i < len; i++) {
                long n = mIdPlayList[i];
                if (n < 0) {
                    continue;
                } else if (n == 0) {
                    q.append("0;");
                } else {
                    while (n != 0) {
                        int digit = (int)(n & 0xf);
                        n >>>= 4;
                        q.append(hexdigits[digit]);
                    }
                    q.append(";");
                }
            }
            //Log.i("@@@@ service", "created queue string in " + (System.currentTimeMillis() - start) + " ms");
            ed.putString("queue", q.toString());
            ed.putInt("cardid", mCardId);
            if (mShuffleMode != SHUFFLE_NONE) {
                // In shuffle mode we need to save the history too
                len = mHistory.size();
                q.setLength(0);
                for (int i = 0; i < len; i++) {
                    int n = mHistory.get(i);
                    if (n == 0) {
                        q.append("0;");
                    } else {
                        while (n != 0) {
                            int digit = (n & 0xf);
                            n >>>= 4;
                            q.append(hexdigits[digit]);
                        }
                        q.append(";");
                    }
                }
                ed.putString("history", q.toString());
            }
        }
        ed.putInt("curpos", mPlayPos);
        if (mPlayer.isInitialized()) {
            ed.putLong("seekpos", mPlayer.position());
        }
        ed.putInt("repeatmode", mRepeatMode);
        ed.putInt("shufflemode", mShuffleMode);

        SharedPreferencesCompat.apply(ed);

        //Log.i("@@@@ service", "saved state in " + (System.currentTimeMillis() - start) + " ms");
    }

    private void reloadQueue() {
        Log.d(LOGTAG, "reloadQueue");
        String q = null;
        
        boolean newstyle = false;
        int id = mCardId;
        if (mPreferences.contains("cardid")) {
            newstyle = true;
            id = mPreferences.getInt("cardid", ~mCardId);
        }
        if (id == mCardId) {
            // Only restore the saved playlist if the card is still
            // the same one as when the playlist was saved
            q = mPreferences.getString("queue", "");
        }
        int qlen = q != null ? q.length() : 0;
        if (qlen > 1) {
            //Log.i("@@@@ service", "loaded queue: " + q);
            int plen = 0;
            int n = 0;
            int shift = 0;
            for (int i = 0; i < qlen; i++) {
                char c = q.charAt(i);
                if (c == ';') {
                    ensurePlayListCapacity(plen + 1, true);
                    mIdPlayList[plen] = n;
                    plen++;
                    n = 0;
                    shift = 0;
                } else {
                    if (c >= '0' && c <= '9') {
                        n += ((c - '0') << shift);
                    } else if (c >= 'a' && c <= 'f') {
                        n += ((10 + c - 'a') << shift);
                    } else {
                        // bogus playlist data
                        plen = 0;
                        break;
                    }
                    shift += 4;
                }
            }
            mPlayListLen = plen;

            int pos = mPreferences.getInt("curpos", 0);
            if (pos < 0 || pos >= mPlayListLen) {
                // The saved playlist is bogus, discard it
                mPlayListLen = 0;
                return;
            }
            mPlayPos = pos;
            
            // When reloadQueue is called in response to a card-insertion,
            // we might not be able to query the media provider right away.
            // To deal with this, try querying for the current file, and if
            // that fails, wait a while and try again. If that too fails,
            // assume there is a problem and don't restore the state.
            Cursor crsr = MusicUtils.query(this,
                        MediaStore.Audio.Media.EXTERNAL_CONTENT_URI,
                        new String [] {"_id"}, "_id=" + mIdPlayList[mPlayPos] , null, null);
            if (crsr == null || crsr.getCount() == 0) {
                // wait a bit and try again
                SystemClock.sleep(3000);
                crsr = getContentResolver().query(
                        MediaStore.Audio.Media.EXTERNAL_CONTENT_URI,
                        mCursorCols, "_id=" + mIdPlayList[mPlayPos] , null, null);
            }
            if (crsr != null) {
                crsr.close();
            }

            // Make sure we don't auto-skip to the next song, since that
            // also starts playback. What could happen in that case is:
            // - music is paused
            // - go to UMS and delete some files, including the currently playing one
            // - come back from UMS
            // (time passes)
            // - music app is killed for some reason (out of memory)
            // - music service is restarted, service restores state, doesn't find
            //   the "current" file, goes to the next and: playback starts on its
            //   own, potentially at some random inconvenient time.
            mPreLoad = true;
            mOpenFailedCounter = 20;
            mQuietMode = true;
            openCurrentAndNext(mIdPlayList != null);
            mQuietMode = false;
            /*
            if (!mPlayer.isInitialized()) {
                // couldn't restore the saved state
                mPlayListLen = 0;
                return;
            }
            */
            long seekpos = mPreferences.getLong("seekpos", 0);
            seek(seekpos >= 0 && seekpos < duration() ? seekpos : 0);
            Log.d(LOGTAG, "restored queue, currently at position "
                    + position() + "/" + duration()
                    + " (requested " + seekpos + ")");
            
            int repmode = mPreferences.getInt("repeatmode", REPEAT_NONE);
            if (repmode != REPEAT_ALL && repmode != REPEAT_CURRENT) {
                repmode = REPEAT_NONE;
            }
            mRepeatMode = repmode;

            int shufmode = mPreferences.getInt("shufflemode", SHUFFLE_NONE);
            if (shufmode != SHUFFLE_AUTO && shufmode != SHUFFLE_NORMAL) {
                shufmode = SHUFFLE_NONE;
            }
            if (shufmode != SHUFFLE_NONE) {
                // in shuffle mode we need to restore the history too
                q = mPreferences.getString("history", "");
                qlen = q != null ? q.length() : 0;
                if (qlen > 1) {
                    plen = 0;
                    n = 0;
                    shift = 0;
                    mHistory.clear();
                    for (int i = 0; i < qlen; i++) {
                        char c = q.charAt(i);
                        if (c == ';') {
                            if (n >= mPlayListLen) {
                                // bogus history data
                                mHistory.clear();
                                break;
                            }
                            mHistory.add(n);
                            n = 0;
                            shift = 0;
                        } else {
                            if (c >= '0' && c <= '9') {
                                n += ((c - '0') << shift);
                            } else if (c >= 'a' && c <= 'f') {
                                n += ((10 + c - 'a') << shift);
                            } else {
                                // bogus history data
                                mHistory.clear();
                                break;
                            }
                            shift += 4;
                        }
                    }
                }
            }
            if (shufmode == SHUFFLE_AUTO) {
                if (! makeAutoShuffleList()) {
                    shufmode = SHUFFLE_NONE;
                }
            }
            mShuffleMode = shufmode;
        }
    }
    
    @Override
    public IBinder onBind(Intent intent) {
        Log.d(LOGTAG, "onBind");
        mDelayedStopHandler.removeCallbacksAndMessages(null);
        mServiceInUse = true;
        return mBinder;
    }

    @Override
    public void onRebind(Intent intent) {
        Log.d(LOGTAG, "onRebind");
        mDelayedStopHandler.removeCallbacksAndMessages(null);
        mServiceInUse = true;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(LOGTAG, "onStartCommand");
        mServiceStartId = startId;
        mDelayedStopHandler.removeCallbacksAndMessages(null);

        if (intent != null) {
            String action = intent.getAction();
            String cmd = intent.getStringExtra("command");
            Log.d(LOGTAG, "onStartCommand " + action + " / " + cmd);

            if (CMDNEXT.equals(cmd) || NEXT_ACTION.equals(action)) {
                if (mBroadcastStatus == false) {
                    gotoNext(false);
                }
            } else if (CMDPREVIOUS.equals(cmd) || PREVIOUS_ACTION.equals(action)) {
                if (mBroadcastStatus == false) {
                    if (position() < 2000) {
                        prev();
                    } else if (duration() <= 0) {
                        prev();
                    } else {
                        seek(0);
                        play();
                    }
                }
            } else if (CMDTOGGLEPAUSE.equals(cmd) || TOGGLEPAUSE_ACTION.equals(action)) {
                if (isPlaying()) {
                    pause();
                    mPausedByTransientLossOfFocus = false;
                } else {
                    play();
                }
            } else if (CMDPAUSE.equals(cmd) || PAUSE_ACTION.equals(action)) {
                pause();
                mPausedByTransientLossOfFocus = false;
            } else if (CMDPLAY.equals(cmd)) {
                play();
            } else if (CMDSTOP.equals(cmd)) {
                //pause();
                //mPausedByTransientLossOfFocus = false;
                //seek(0);
                stop(true);
                notifyTrackEnd(STATUS_LIST_END);
            }
        }
        
        // make sure the service will shut down on its own if it was
        // just started but not bound to and nothing is playing
        mDelayedStopHandler.removeCallbacksAndMessages(null);
        Message msg = mDelayedStopHandler.obtainMessage();
        mDelayedStopHandler.sendMessageDelayed(msg, IDLE_DELAY);
        return START_STICKY;
    }
    
    @Override
    public boolean onUnbind(Intent intent) {
        Log.d(LOGTAG, "onUnbind");
        mServiceInUse = false;

        // Take a snapshot of the current playlist
        saveQueue(true);

        if (isPlaying() || mPausedByTransientLossOfFocus) {
            // something is currently playing, or will be playing once 
            // an in-progress action requesting audio focus ends, so don't stop the service now.
            return true;
        }
        
        // If there is a playlist but playback is paused, then wait a while
        // before stopping the service, so that pause/resume isn't slow.
        // Also delay stopping the service if we're transitioning between tracks.
        if (mPlayListLen > 0  || mMediaplayerHandler.hasMessages(TRACK_ENDED)) {
            Message msg = mDelayedStopHandler.obtainMessage();
            mDelayedStopHandler.sendMessageDelayed(msg, IDLE_DELAY);
            return true;
        }
        
        // No active playlist, OK to stop the service right now
        stopSelf(mServiceStartId);
        return true;
    }
    
    private Handler mDelayedStopHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            // Check again to make sure nothing is playing right now
            if (isPlaying() || mPausedByTransientLossOfFocus || mServiceInUse
                    || mMediaplayerHandler.hasMessages(TRACK_ENDED)) {
                return;
            }
            // save the queue again, because it might have changed
            // since the user exited the music app (because of
            // party-shuffle or because the play-position changed)
            saveQueue(true);
            stopSelf(mServiceStartId);
        }
    };

    /**
     * Called when we receive a ACTION_MEDIA_EJECT notification.
     *
     * @param storagePath path to mount point for the removed media
     */
    public void closeExternalStorageFiles(String storagePath) {
        // stop playback and clean up if the SD card is going to be unmounted.
        Log.d(LOGTAG, "closeExternalStorageFiles");
        stop(true);
        notifyChange(QUEUE_CHANGED);
        notifyChange(META_CHANGED);
    }

    /**
     * Registers an intent to listen for ACTION_MEDIA_EJECT notifications.
     * The intent will call closeExternalStorageFiles() if the external media
     * is going to be ejected, so applications can clean up any files they have open.
     */
    public void registerExternalStorageListener() {
        Log.d(LOGTAG, "registerExternalStorageListener");
        if (mUnmountReceiver == null) {
            mUnmountReceiver = new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                    Log.d(LOGTAG, "registerExternalStorageListener - onReceive");
                    String action = intent.getAction();
                    if (action.equals(Intent.ACTION_MEDIA_EJECT)) {
                        Log.d(LOGTAG, "registerExternalStorageListener - Intent.ACTION_MEDIA_EJECT");
                        if (mSourceFrom == SOURCE_LOCAL) {
                            saveQueue(true);
                            mQueueIsSaveable = false;
                            closeExternalStorageFiles(intent.getData().getPath());
                        }
                    } else if (action.equals(Intent.ACTION_MEDIA_MOUNTED)) {
                        Log.d(LOGTAG, "registerExternalStorageListener - Intent.ACTION_MEDIA_MOUNTED");
                        mMediaMountedCount++;
                        mCardId = MusicUtils.getCardId(MediaPlaybackService.this);
                        reloadQueue();
                        mQueueIsSaveable = true;
                        notifyChange(QUEUE_CHANGED);
                        notifyChange(META_CHANGED);
                    }
                }
            };
            IntentFilter iFilter = new IntentFilter();
            iFilter.addAction(Intent.ACTION_MEDIA_EJECT);
            iFilter.addAction(Intent.ACTION_MEDIA_MOUNTED);
            iFilter.addDataScheme("file");
            registerReceiver(mUnmountReceiver, iFilter);
        }
    }

    public void registerNetworkListener() {
        Log.d(LOGTAG, "registerNetworkListener()");
        if (mNetworkReceiver == null) {
            mNetworkReceiver = new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                    Log.d(LOGTAG, "registerNetworkListener()-onReceive");
                    String action = intent.getAction();
                    Log.d(LOGTAG, "registerNetworkListener()-onReceive action = " + action);
                    if (action.equals(EthernetManager.NETWORK_STATE_CHANGED_ACTION)) {
                        Log.d(LOGTAG, "EthernetManager.NETWORK_STATE_CHANGED_ACTION");
                    } else if (action.equals(EthernetManager.ETHERNET_STATE_CHANGED_ACTION)) {
                        Log.d(LOGTAG, "EthernetManager.ETHERNET_STATE_CHANGED_ACTION");
                    } else if (action.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) {
                        Log.d(LOGTAG, "WifiManager.WIFI_STATE_CHANGED_ACTION");
                    } else if (action.equals(WifiManager.NETWORK_STATE_CHANGED_ACTION)) {
                        Log.d(LOGTAG, "WifiManager.NETWORK_STATE_CHANGED_ACTION");
                    } else if (action.equals(ConnectivityManager.CONNECTIVITY_ACTION)) {
                        Log.d(LOGTAG, "ConnectivityManager.CONNECTIVITY_ACTION");
                        if (mSourceFrom == SOURCE_NETWORK) {
                            NetworkInfo networkInfo = intent.getParcelableExtra(ConnectivityManager.EXTRA_NETWORK_INFO);
                            if ((mEthAvailable && networkInfo.getType() == ConnectivityManager.TYPE_ETHERNET) || 
                                (mWifiAvailable && networkInfo.getType() == ConnectivityManager.TYPE_WIFI)) {
                                if (networkInfo.getState() == NetworkInfo.State.DISCONNECTED) {
                                    Log.d(LOGTAG, "registerNetworkListener()-NetworkInfo.State.DISCONNECTED");
                                    saveQueue(true);
                                    mQueueIsSaveable = false;
                                    closeExternalStorageFiles(null);
                                }
                            }
                        }
                    } //else if (action.equals(ConnectivityManager.INET_CONDITION_ACTION)) {
                        //Log.d(LOGTAG, "ConnectivityManager.INET_CONDITION_ACTION");
                    //}
                }
            };
            IntentFilter iFilter = new IntentFilter();
            // WiFi
            iFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
            iFilter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
            // Ethernet
            iFilter.addAction(EthernetManager.NETWORK_STATE_CHANGED_ACTION);
            iFilter.addAction(EthernetManager.ETHERNET_STATE_CHANGED_ACTION);
            // Network
            iFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
            //iFilter.addAction(ConnectivityManager.INET_CONDITION_ACTION);
            registerReceiver(mNetworkReceiver, iFilter);
        }
    }

    /**
     * Notify the change-receivers that something has changed.
     * The intent that is sent contains the following data
     * for the currently playing track:
     * "id" - Integer: the database row ID
     * "artist" - String: the name of the artist
     * "album" - String: the name of the album
     * "track" - String: the name of the track
     * The intent has an action that is one of
     * "com.android.music.metachanged"
     * "com.android.music.queuechanged",
     * "com.android.music.playbackcomplete"
     * "com.android.music.playstatechanged"
     * respectively indicating that a new track has
     * started playing, that the playback queue has
     * changed, that playback has stopped because
     * the last file in the list has been played,
     * or that the play-state changed (paused/resumed).
     */
    private void notifyChange(String what) {
        Log.d(LOGTAG, "notifyChange - what = " + what);
        Intent i = new Intent(what);
        i.putExtra("id", Long.valueOf(getAudioId()));
        i.putExtra("artist", getArtistName());
        i.putExtra("album",getAlbumName());
        i.putExtra("track", getTrackName());
        i.putExtra("playing", isPlaying());
        sendStickyBroadcast(i);

        if (what.equals(PLAYSTATE_CHANGED)) {
            mRemoteControlClient.setPlaybackState(isPlaying() ?
                    RemoteControlClient.PLAYSTATE_PLAYING : RemoteControlClient.PLAYSTATE_PAUSED);
            //// Broadcast playback status for DMR usage////////////
            broadcastPlaybackStatus(isPlaying() ? RTK_PLAYBACK_STATUS_PLAY : RTK_PLAYBACK_STATUS_PAUSE);
            ////////////////////////////////////////////////
        } else if (what.equals(META_CHANGED)) {
            RemoteControlClient.MetadataEditor ed = mRemoteControlClient.editMetadata(true);
            ed.putString(MediaMetadataRetriever.METADATA_KEY_TITLE, getTrackName());
            ed.putString(MediaMetadataRetriever.METADATA_KEY_ALBUM, getAlbumName());
            ed.putString(MediaMetadataRetriever.METADATA_KEY_ARTIST, getArtistName());
            ed.putLong(MediaMetadataRetriever.METADATA_KEY_DURATION, duration());
            Bitmap b = MusicUtils.getArtwork(this, getAudioId(), getAlbumId(), false);
            if (b != null) {
                ed.putBitmap(MetadataEditor.BITMAP_KEY_ARTWORK, b);
            }
            ed.apply();
            broadcastDuration();
        }

        if (what.equals(QUEUE_CHANGED)) {
            saveQueue(true);
        } else {
            saveQueue(false);
        }
        
        // Share this notification directly with our widgets
        mAppWidgetProvider.notifyChange(this, what);
    }

    private void notifyTrackEnd(String what)
    {
        Intent i = new Intent(what);
        sendBroadcast(i);

        //// Broadcast playback status for DMR usage////////////
        broadcastPlaybackStatus(RTK_PLAYBACK_STATUS_STOP);
        ////////////////////////////////////////////////
    }

    private void ensurePlayListCapacity(int size, boolean isId) {
        Log.d(LOGTAG, "ensurePlayListCapacity");
        Log.d(LOGTAG, "ensurePlayListCapacity - size = " + size);
        Log.d(LOGTAG, "ensurePlayListCapacity - isId = " + isId);
        if (isId)
        {
            if (mIdPlayList == null || size > mIdPlayList.length) {
                // reallocate at 2x requested size so we don't
                // need to grow and copy the array for every
                // insert
                long [] newlist = new long[size * 2];
                int len = mIdPlayList != null ? mIdPlayList.length : mPlayListLen;
                for (int i = 0; i < len; i++) {
                    newlist[i] = mIdPlayList[i];
                }
                mIdPlayList = newlist;
            }
        }
        else
        {
             if (mUrlPlayList == null || size > mUrlPlayList.length) {
                // reallocate at 2x requested size so we don't
                // need to grow and copy the array for every
                // insert
                String [] newlist = new String[size * 2];
                int len = mUrlPlayList != null ? mUrlPlayList.length : mPlayListLen;
                for (int i = 0; i < len; i++) {
                    newlist[i] = mUrlPlayList[i];
                }
                mUrlPlayList = newlist;
            }
        }
        
        // FIXME: shrink the array when the needed size is much smaller
        // than the allocated size
    }
    
    // insert the list of songs at the specified position in the playlist
    private void addToPlayList(long [] list, int position) {
        int addlen = list.length;
        if (position < 0) { // overwrite
            mPlayListLen = 0;
            position = 0;
        }
        ensurePlayListCapacity(mPlayListLen + addlen, true);
        if (position > mPlayListLen) {
            position = mPlayListLen;
        }
        
        // move part of list after insertion point
        int tailsize = mPlayListLen - position;
        for (int i = tailsize ; i > 0 ; i--) {
            mIdPlayList[position + i] = mIdPlayList[position + i - addlen]; 
        }
        
        // copy list into playlist
        for (int i = 0; i < addlen; i++) {
            mIdPlayList[position + i] = list[i];
        }
        mPlayListLen += addlen;
        if (mPlayListLen == 0) {
            mCursor.close();
            mCursor = null;
            notifyChange(META_CHANGED);
        }
    }

    // insert the list of songs at the specified position in the playlist
    private void addToPlayList(String [] list, int position) {
        Log.d(LOGTAG, "addToPlayList");
        int addlen = list.length;
        if (position < 0) { // overwrite
            mPlayListLen = 0;
            position = 0;
        }
        ensurePlayListCapacity(mPlayListLen + addlen, false);
        if (position > mPlayListLen) {
            position = mPlayListLen;
        }
        
        // move part of list after insertion point
        int tailsize = mPlayListLen - position;
        for (int i = tailsize ; i > 0 ; i--) {
            mUrlPlayList[position + i] = mUrlPlayList[position + i - addlen]; 
        }
        
        // copy list into playlist
        for (int i = 0; i < addlen; i++) {
            mUrlPlayList[position + i] = list[i];
        }
        mPlayListLen += addlen;
        mCursor = null;
        if (mPlayListLen == 0) {
            //mCursor.close();
            notifyChange(META_CHANGED);
        }
    }
    
    /**
     * Appends a list of tracks to the current playlist.
     * If nothing is playing currently, playback will be started at
     * the first track.
     * If the action is NOW, playback will switch to the first of
     * the new tracks immediately.
     * @param list The list of tracks to append.
     * @param action NOW, NEXT or LAST
     */
    public void enqueue(long [] list, int action) {
        synchronized(this) {
            if (action == NEXT && mPlayPos + 1 < mPlayListLen) {
                addToPlayList(list, mPlayPos + 1);
                notifyChange(QUEUE_CHANGED);
            } else {
                // action == LAST || action == NOW || mPlayPos + 1 == mPlayListLen
                addToPlayList(list, Integer.MAX_VALUE);
                notifyChange(QUEUE_CHANGED);
                if (action == NOW) {
                    mPlayPos = mPlayListLen - list.length;
                    openCurrentAndNext(true);
                    play();
                    notifyChange(META_CHANGED);
                    return;
                }
            }
            if (mPlayPos < 0) {
                mPlayPos = 0;
                openCurrentAndNext(true);
                play();
                notifyChange(META_CHANGED);
            }
        }
    }

    /**
     * Replaces the current playlist with a new list,
     * and prepares for starting playback at the specified
     * position in the list, or a random position if the
     * specified position is 0.
     * @param list The new list of tracks.
     */
    public void open(long [] list, int position) {
        synchronized (this) {
            if (mShuffleMode == SHUFFLE_AUTO) {
                mShuffleMode = SHUFFLE_NORMAL;
            }
            long oldId = getAudioId();
            int listlength = list.length;
            boolean newlist = true;
            mUrlPlayList = null;
            if (mPlayListLen == listlength && mIdPlayList != null) {
                // possible fast path: list might be the same
                newlist = false;
                for (int i = 0; i < listlength; i++) {
                    if (list[i] != mIdPlayList[i]) {
                        newlist = true;
                        break;
                    }
                }
            }
            if (newlist) {
                addToPlayList(list, -1);
                notifyChange(QUEUE_CHANGED);
            }
            int oldpos = mPlayPos;
            if (position >= 0) {
                mPlayPos = position;
            } else {
                mPlayPos = mRand.nextInt(mPlayListLen);
            }
            mHistory.clear();

            saveBookmarkIfNeeded();
            openCurrentAndNext(true);
            if (oldId != getAudioId()) {
                notifyChange(META_CHANGED);
            }
        }
    }


    public void open(String [] list, int position) {
        synchronized (this) {
            if (mShuffleMode == SHUFFLE_AUTO) {
                mShuffleMode = SHUFFLE_NORMAL;
            }
            long oldId = getAudioId();
            int listlength = list.length;
            boolean newlist = true;
            mIdPlayList = null;
            if (mPlayListLen == listlength && mUrlPlayList != null) {
                // possible fast path: list might be the same
                newlist = false;
                for (int i = 0; i < listlength; i++) {
                    if (list[i] != mUrlPlayList[i]) {
                        newlist = true;
                        break;
                    }
                }
            }
            if (newlist) {
                addToPlayList(list, -1);
                notifyChange(QUEUE_CHANGED);
                // Always set repeat mode to none when opening a new URL playlist
                setRepeatMode(REPEAT_NONE);
            }
            int oldpos = mPlayPos;
            if (position >= 0) {
                mPlayPos = position;
            } else {
                mPlayPos = mRand.nextInt(mPlayListLen);
            }
            mHistory.clear();

            saveBookmarkIfNeeded();
            openCurrentAndNext(false);
            if (newlist) {
                notifyChange(META_CHANGED);
            }
        }
    }
    
    /**
     * Moves the item at index1 to index2.
     * @param index1
     * @param index2
     */
    public void moveQueueItem(int index1, int index2) {
        synchronized (this) {
            if (index1 >= mPlayListLen) {
                index1 = mPlayListLen - 1;
            }
            if (index2 >= mPlayListLen) {
                index2 = mPlayListLen - 1;
            }
            if (index1 < index2) {
                long tmp = mIdPlayList[index1];
                for (int i = index1; i < index2; i++) {
                    mIdPlayList[i] = mIdPlayList[i+1];
                }
                mIdPlayList[index2] = tmp;
                if (mPlayPos == index1) {
                    mPlayPos = index2;
                } else if (mPlayPos >= index1 && mPlayPos <= index2) {
                        mPlayPos--;
                }
            } else if (index2 < index1) {
                long tmp = mIdPlayList[index1];
                for (int i = index1; i > index2; i--) {
                    mIdPlayList[i] = mIdPlayList[i-1];
                }
                mIdPlayList[index2] = tmp;
                if (mPlayPos == index1) {
                    mPlayPos = index2;
                } else if (mPlayPos >= index2 && mPlayPos <= index1) {
                        mPlayPos++;
                }
            }
            notifyChange(QUEUE_CHANGED);
        }
    }

    /**
     * Returns the current play list
     * @return An array of integers containing the IDs of the tracks in the play list
     */
    public long [] getIdQueue() {
        synchronized (this) {
            int len = mPlayListLen;
            if (len == 0)
            {
                return null;
            }
            else if (mIdPlayList == null)
            {
                return null;
            }
            else
            {
                long [] list = new long[len];
                for (int i = 0; i < len; i++) {
                    list[i] = mIdPlayList[i];
                }
                return list;
            }
        }
    }

     public String [] getUrlQueue() {
        synchronized (this) {
            int len = mPlayListLen;
            if (len == 0)
            {
                return null;
            }
            else if (mUrlPlayList == null)
            {
                return null;
            }
            else
            {
                String [] list = new String[len];
                for (int i = 0; i < len; i++) {
                    list[i] = mUrlPlayList[i];
                }
                return list;
            }
        }
    }

    private Cursor getCursorForId(long lid) {
        String id = String.valueOf(lid);

        Cursor c = getContentResolver().query(
                MediaStore.Audio.Media.EXTERNAL_CONTENT_URI,
                mCursorCols, "_id=" + id , null, null);
        if (c != null) {
            c.moveToFirst();
        }
        return c;
    }

    private void openCurrentAndNext(boolean isId) {
        synchronized (this) {
           
            if (mPlayListLen == 0) {
                return;
            }
            Log.d(LOGTAG, "openCurrentAndNext - isId = " + isId);
            Log.d(LOGTAG, "openCurrentAndNext->stop");
            stop(false);

            if (isId) {
                if (mCursor != null) {
                   mCursor.close();
                   mCursor = null;
                }
                Log.d(LOGTAG, "openCurrentAndNext - mIdPlayList[mPlayPos] = " + mIdPlayList[mPlayPos]);
                mCursor = getCursorForId(mIdPlayList[mPlayPos]);
                while(true) {
                    if (mCursor != null && mCursor.getCount() != 0 &&
                            open(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI + "/" +
                                    mCursor.getLong(IDCOLIDX))) {
                        break;
                    }
                    // if we get here then opening the file failed. We can close the cursor now, because
                    // we're either going to create a new one next, or stop trying
                    if (mCursor != null) {
                        mCursor.close();
                        mCursor = null;
                    }
                    if (mOpenFailedCounter++ < 10 &&  mPlayListLen > 1) {
                        int pos = getNextPosition(false);
                        if (pos < 0) {
                            gotoIdleState();
                            if (mIsSupposedToBePlaying) {
                                mIsSupposedToBePlaying = false;
                                notifyChange(PLAYSTATE_CHANGED);
                            }
                            return;
                        }
                        mPlayPos = pos;
                        stop(false);
                        mPlayPos = pos;
                        mCursor = getCursorForId(mIdPlayList[mPlayPos]);
                    } else {
                        mOpenFailedCounter = 0;
                        if (!mQuietMode) {
                            Toast.makeText(this, R.string.playback_failed, Toast.LENGTH_SHORT).show();
                        }
                        Log.d(LOGTAG, "Failed to open file for playback");
                        mPreLoad = false;
                        gotoIdleState();
                        if (mIsSupposedToBePlaying) {
                            mIsSupposedToBePlaying = false;
                            notifyChange(PLAYSTATE_CHANGED);
                        }
                        return;
                    }
                }

                // go to bookmark if needed
                if (isPodcast()) {
                    long bookmark = getBookmark();
                    // Start playing a little bit before the bookmark,
                    // so it's easier to get back in to the narrative.
                    seek(bookmark - 5000);
                }
            }
            else {
                while(true) {
                    mCursor = null;
                    if ( open(mUrlPlayList[mPlayPos]) ) {
                        break;
                    }
                    if (mOpenFailedCounter++ < 10 &&  mPlayListLen > 1) {
                        int pos = getNextPosition(false);
                        if (pos < 0) {
                            gotoIdleState();
                            if (mIsSupposedToBePlaying) {
                                mIsSupposedToBePlaying = false;
                                notifyChange(PLAYSTATE_CHANGED);
                            }
                            return;
                        }
                        mPlayPos = pos;
                        Log.d(LOGTAG, "openCurrentAndNext->stop, mOpenFailedCounter = " + mOpenFailedCounter);
                        stop(false);
                        mPlayPos = pos;
                    }
                    else {
                        mOpenFailedCounter = 0;
                        if (!mQuietMode) {
                            Toast.makeText(this, R.string.playback_failed, Toast.LENGTH_SHORT).show();
                        }
                        Log.d(LOGTAG, "Failed to open file for playback");
                        mPreLoad = false;
                        gotoIdleState();
                        if (mIsSupposedToBePlaying) {
                            mIsSupposedToBePlaying = false;
                            notifyChange(PLAYSTATE_CHANGED);
                        }
                        return;
                    }
                }
            }
            setNextTrack(isId);
        }
    }

    private void setNextTrack(boolean isID) {
        mNextPlayPos = getNextPosition(false);
        Log.d(LOGTAG, "setNextTrack - isID =" + isID);
        Log.d(LOGTAG, "setNextTrack - mNextPlayPos =" + mNextPlayPos);
        if (mNextPlayPos >= 0) {
            if (isID) {
                long id = mIdPlayList[mNextPlayPos];
                mPlayer.setNextDataSource(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI + "/" + id);
            }
            else {
                mPlayer.setNextDataSource(mUrlPlayList[mNextPlayPos]);
            }
        }
    }



    /**
     * Opens the specified file and readies it for playback.
     *
     * @param path The full path of the file to be opened.
     */
    public boolean open(String path) {
        Log.d(LOGTAG, "open - open path = " + path);
        Log.d(LOGTAG, "open - mCursor = " + mCursor);
        synchronized (this) {
            if (path == null) {
                return false;
            }
            boolean bPlaylist = false;
            mTrackInfo = null;

            // if mCursor is null, try to associate path with a database cursor
            if (mCursor == null) {
                boolean bQuery = false;
                ContentResolver resolver = getContentResolver();
                Uri uri = null ;
                String where = null;
                String colName = null;
                String[] selectionArgs = null;
                int playlistId = 0;
                if (path.startsWith("content://media/")) {
                    mSourceFrom = SOURCE_LOCAL;
                    if (path.indexOf("playlists") > 0) {
                        bPlaylist = true;
                        int index = path.lastIndexOf('/');
                        String playlist = path.substring(index+1);
                        Log.d(LOGTAG, "playlist = " + playlist);
                        uri = MediaStore.Audio.Playlists.Members.getContentUri("external", Long.valueOf(playlist));
                        where = null;
                        selectionArgs = null;
                        bQuery = true;
                    }
                    else {
                        uri = Uri.parse(path);
                        where = null;
                        selectionArgs = null;
                        bQuery = true;
                    }
                    Log.d(LOGTAG, "open - uri = " + uri);
                }
                else if (path.toLowerCase().startsWith("file://") || 
                        path.toLowerCase().startsWith("content://")) {  //already in database
                    path = Uri.parse(path).getPath();
                    // for Android 7
                    String prefix = "/rtk_media/";
                    int index = path.indexOf(prefix);
                    if (index >= 0) {
                        path = path.substring(index + prefix.length() - 1);
                    }

                    if (path.toLowerCase().startsWith("/storage/")) {
                        mSourceFrom = SOURCE_LOCAL;
                        if (path.toLowerCase().endsWith("m3u") || path.toLowerCase().endsWith("pls")) {
                            bPlaylist = true;
                            uri = MediaStore.Audio.Playlists.EXTERNAL_CONTENT_URI;
                            colName = MediaStore.Audio.Playlists._ID;
                            where = MediaStore.Audio.Playlists.DATA + "=?";
                            selectionArgs = new String[] { path };
                            mCursor = resolver.query(uri, new String[] { colName }, where,	selectionArgs, null);
                            if (mCursor != null) {
                                mCursor.moveToFirst();
                                playlistId = mCursor.getInt(mCursor.getColumnIndex(MediaStore.MediaColumns._ID));
                                Log.d(LOGTAG, "playlistId = " + playlistId);

                                //uri = Uri.withAppendedPath(MediaStore.Audio.Playlists.EXTERNAL_CONTENT_URI, Integer.toString(playlistId));
                                //Log.d(LOGTAG, "uri = " + uri);
                                uri = MediaStore.Audio.Playlists.Members.getContentUri("external", Long.valueOf(playlistId));
                                where = null;
                                selectionArgs = null;
                                bQuery = true;
                            }
                        }
                        else {
                            //uri = MediaStore.Audio.Media.getContentUriForPath(path);
                            uri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                            if(path.toLowerCase().startsWith("/rtk_media/"))
                                path = path.substring(path.indexOf("/storage"));
                            Log.d(LOGTAG, "path = " + path);
                            Log.d(LOGTAG, "uri = " + uri);
                            where = MediaStore.Audio.Media.DATA + "=?";
                            selectionArgs = new String[] { path };
                            bQuery = true;
                        }
                    }
					else {
                        mSourceFrom = SOURCE_NETWORK;
                        mOpenPath = path;
                        new Thread() {
                            @Override
                            public void run() {
                                MediaMetadataRetriever mmr = new MediaMetadataRetriever();
                                try {
                                    mmr.setDataSource(mOpenPath);
                                    String title = mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_TITLE);
                                    String album = mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_ALBUM);
                                    String artist= mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_ARTIST);
                                    Log.d(LOGTAG, "new MusicTrackInfo(" + artist + ", " + album + ", " + title + ")");
                                    mTrackInfo = new MusicTrackInfo(artist, album, title);
                                } catch (Exception e) {
                                    Log.d(LOGTAG, "MediaMetadataRetriever setDataSource exception");
                                } finally {
                                    mmr.release();
                                }
                            }
                        }.start();
                    }
                }
                else if (path.toLowerCase().startsWith("directconnect://")) {
                    mSourceFrom = SOURCE_NETWORK;
                    String[] metadata = new AirFunManager().extractMetadata(path);
                    mTrackInfo = new MusicTrackInfo(metadata[0], metadata[1], metadata[2]);
                }
                else {
                    mSourceFrom = SOURCE_NETWORK;
                    mOpenPath = path;
                    new Thread() {
                        @Override
                        public void run() {
                            MediaMetadataRetriever mmr = new MediaMetadataRetriever();
                            try {
                                if (mOpenPath.toLowerCase().startsWith("http://"))
                                    mmr.setDataSource(mOpenPath, new HashMap<String, String>());
                                else //other type of local file, like samba
                                    mmr.setDataSource(mOpenPath);

                                String title = mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_TITLE);
                                String album = mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_ALBUM);
                                String artist= mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_ARTIST);
                                Log.d(LOGTAG, "new MusicTrackInfo(" + artist + ", " + album + ", " + title + ")");
                                mTrackInfo = new MusicTrackInfo(artist, album, title);
                            } catch (Exception e) {
                                Log.d(LOGTAG, "MediaMetadataRetriever setDataSource exception");
                            } finally {
                                mmr.release();
                            }
                        }
                    }.start();
                }
     
                if (bQuery)
                {
                    if (bPlaylist) {
                        mCursor = resolver.query(uri, mPlaylistMemberCols, where, selectionArgs, null);
                        if (mCursor != null) {
                            Log.d(LOGTAG, "mCursor.getCount() = " + mCursor.getCount());
                            if (mCursor.getCount() == 0) {
                                mCursor.close();
                                mCursor = null;
                            }
                            else {
                                mCursor.moveToFirst();
                                ensurePlayListCapacity(mCursor.getCount(), false);
                                for (int i=0 ; i<mCursor.getCount() ; i++) {
                                    Log.d(LOGTAG, "data = " + mCursor.getString(mCursor.getColumnIndexOrThrow(MediaStore.Audio.Playlists.DATA)));
                                    Log.d(LOGTAG, "id = " + mCursor.getString(mCursor.getColumnIndexOrThrow(MediaStore.Audio.Playlists._ID)));
                                    mUrlPlayList[i] = mCursor.getString(mCursor.getColumnIndexOrThrow(MediaStore.Audio.Playlists.DATA));
                                    mCursor.moveToNext();
                                }
                                mCursor.moveToFirst();
                                path = mUrlPlayList[0];
                                mPlayListLen = mCursor.getCount();
                                mPlayPos = 0;
                            }
                        }
                    }
                    else {
                        try {
                            mCursor = resolver.query(uri, mCursorCols, where, selectionArgs, null);
                            if  (mCursor != null) {
                                Log.d(LOGTAG, "mCursor.getCount() = " + mCursor.getCount());
                                Log.d(LOGTAG, "mIdPlayList = " + mIdPlayList);
                                if (mCursor.getCount() == 0) {
                                    mCursor.close();
                                    mCursor = null;
                                }
                                else if (mIdPlayList == null) {
                                    mCursor.moveToFirst();
                                    // Trigger by AirFun USB Browser (play one selected file)
                                    if (mUrlPlayList == null) {
                                        ensurePlayListCapacity(1, false);
                                        mUrlPlayList[0] = path;
                                        mPlayListLen = 1;
                                        mPlayPos = 0;
                                    }
                                    Log.d(LOGTAG, "data = " + mCursor.getString(mCursor.getColumnIndexOrThrow(MediaStore.Audio.Media.DATA)));
                                    Log.d(LOGTAG, "id = " + mCursor.getString(mCursor.getColumnIndexOrThrow(MediaStore.Audio.Media._ID)));
                                }
                                else {
                                    mCursor.moveToNext();
                                    ensurePlayListCapacity(1, true);
                                    mPlayListLen = 1;
                                    mIdPlayList[0] = mCursor.getLong(IDCOLIDX);
                                    mPlayPos = 0;
                                }
                            }
                        } catch (UnsupportedOperationException ex) {
                        }
                    }
                }
            }
            mFileToPlay = path;
            mPlayer.setDataSource(mFileToPlay);
            return true;
            /*
            if (mPlayer.isInitialized()) {
                mOpenFailedCounter = 0;
                return true;
            }
            stop(true);
            return false;
            */
        }
    }

    /**
     * Starts playback of a previously opened file.
     */
    public void play() {
        Log.d(LOGTAG, "play");
        mAudioManager.requestAudioFocus(mAudioFocusListener, AudioManager.STREAM_MUSIC,
                AudioManager.AUDIOFOCUS_GAIN);
        mAudioManager.registerMediaButtonEventReceiver(new ComponentName(this.getPackageName(),
                MediaButtonIntentReceiver.class.getName()));

        if (mPlayer.isInitialized()) {
            // if we are at the end of the song, go to the next song first
            long duration = mPlayer.duration();
            if (mRepeatMode != REPEAT_CURRENT && duration > 2000 &&
                mPlayer.position() >= duration - 2000) {
                gotoNext(true);
            }

            mPlayer.start();
            // make sure we fade in, in case a previous fadein was stopped because
            // of another focus loss
            mMediaplayerHandler.removeMessages(FADEDOWN);
            mMediaplayerHandler.sendEmptyMessage(FADEUP);

            updateNotification();
            if (!mIsSupposedToBePlaying) {
                mIsSupposedToBePlaying = true;
                notifyChange(PLAYSTATE_CHANGED);
            }
            broadcastDuration();
            long next = refreshTimePosition();
            queueNextTimePositionRefresh(next);
        }
        // Fix bug 43129 (null exception or infinite loop)
        /*
        else if (mPlayListLen <= 0) {
            // This is mostly so that if you press 'play' on a bluetooth headset
            // without every having played anything before, it will still play
            // something.
            setShuffleMode(SHUFFLE_AUTO);
        }
        */
        else {
            if (mIsPreparing == false) {
                if (mIdPlayList != null || mUrlPlayList != null) {
                    if (mPlayPos < 0 || mPlayPos >= mPlayListLen - 1) {
                        mPlayPos = 0;
                    }
                    saveBookmarkIfNeeded();
                    stop(false);
                    openCurrentAndNext(mIdPlayList != null);
                }
            }
        }
    }

    private void updateNotification() {
        Log.d(LOGTAG, "updateNotification");
        RemoteViews views = new RemoteViews(getPackageName(), R.layout.statusbar);
        views.setImageViewResource(R.id.icon, R.drawable.stat_notify_musicplayer);
        /*
        if (getAudioId() < 0) {
            // streaming
            views.setTextViewText(R.id.trackname, getPath());
            views.setTextViewText(R.id.artistalbum, null);
            Log.d(LOGTAG, "updateNotification - track = " + getPath());
        }
        */

        String artist = getArtistName();
        views.setTextViewText(R.id.trackname, getTrackName());
        if (artist == null || artist.equals(MediaStore.UNKNOWN_STRING)) {
            artist = getString(R.string.unknown_artist_name);
        }
        String album = getAlbumName();
        if (album == null || album.equals(MediaStore.UNKNOWN_STRING)) {
            album = getString(R.string.unknown_album_name);
        }

        Log.d(LOGTAG, "updateNotification - artist = " + artist);
        Log.d(LOGTAG, "updateNotification - album = " + album);
        Log.d(LOGTAG, "updateNotification - track = " + getTrackName());
        views.setTextViewText(R.id.artistalbum,
                getString(R.string.notification_artist_album, artist, album));

        Notification status = new Notification();
        status.contentView = views;
        status.flags |= Notification.FLAG_ONGOING_EVENT;
        status.icon = R.drawable.stat_notify_musicplayer;
        status.contentIntent = PendingIntent.getActivity(this, 0,
                new Intent("com.android.music.PLAYBACK_VIEWER")
                .addFlags(Intent.FLAG_ACTIVITY_NEW_TASK), 0);
        startForeground(PLAYBACKSERVICE_STATUS, status);
    }

    private void stop(boolean remove_status_icon) {
        Log.d(LOGTAG, "stop");
        if (mPlayer != null && mPlayer.isInitialized()) {
            mPlayer.stop();
        }
        mFileToPlay = null;
        if (mCursor != null) {
            mCursor.close();
            mCursor = null;
        }
        if (remove_status_icon) {
            gotoIdleState();
        } else {
            stopForeground(false);
        }
        if (remove_status_icon) {
            mIsSupposedToBePlaying = false;
        }
        mIsPreparing = false;
    }

    /**
     * Stops playback.
     */
    public void stop() {
        stop(true);
        broadcastPlaybackStatus(RTK_PLAYBACK_STATUS_STOP);
        // reset broadcast status and source string
        mBroadcastStatus = false;
        mSourceFromString = null;
    }

    /**
     * Pauses playback (call play() to resume)
     */
    public void pause() {
        synchronized(this) {
            mMediaplayerHandler.removeMessages(FADEUP);
            if (isPlaying()) {
                mPlayer.pause();
                gotoIdleState();
                mIsSupposedToBePlaying = false;
                notifyChange(PLAYSTATE_CHANGED);
                saveBookmarkIfNeeded();
            }
        }
    }

    /** Returns whether something is currently playing
     *
     * @return true if something is playing (or will be playing shortly, in case
     * we're currently transitioning between tracks), false if not.
     */
    public boolean isPlaying() {
        return mIsSupposedToBePlaying;
    }

    /*
      Desired behavior for prev/next/shuffle:

      - NEXT will move to the next track in the list when not shuffling, and to
        a track randomly picked from the not-yet-played tracks when shuffling.
        If all tracks have already been played, pick from the full set, but
        avoid picking the previously played track if possible.
      - when shuffling, PREV will go to the previously played track. Hitting PREV
        again will go to the track played before that, etc. When the start of the
        history has been reached, PREV is a no-op.
        When not shuffling, PREV will go to the sequentially previous track (the
        difference with the shuffle-case is mainly that when not shuffling, the
        user can back up to tracks that are not in the history).

        Example:
        When playing an album with 10 tracks from the start, and enabling shuffle
        while playing track 5, the remaining tracks (6-10) will be shuffled, e.g.
        the final play order might be 1-2-3-4-5-8-10-6-9-7.
        When hitting 'prev' 8 times while playing track 7 in this example, the
        user will go to tracks 9-6-10-8-5-4-3-2. If the user then hits 'next',
        a random track will be picked again. If at any time user disables shuffling
        the next/previous track will be picked in sequential order again.
     */

    public void prev() {
        synchronized (this) {
            Log.d(LOGTAG, "prev");
            if (mShuffleMode == SHUFFLE_NORMAL) {
                Log.d(LOGTAG, "prev - mShuffleMode == SHUFFLE_NORMAL");
                // go to previously-played track and remove it from the history
                int histsize = mHistory.size();
                if (histsize == 0) {
                    // prev is a no-op
                    return;
                }
                Integer pos = mHistory.remove(histsize - 1);
                mPlayPos = pos.intValue();
            } else {
                if (mPlayPos > 0) {
                    mPlayPos--;
                }
                else {
                    if (mRepeatMode == REPEAT_CURRENT) {
                        // keep original position
                        if (mPlayPos < 0)
                            mPlayPos = 0;
                    }
                    else if (mRepeatMode == REPEAT_NONE) {
                        mPlayPos = -1;
                    }
                    else {
                        mPlayPos = mPlayListLen - 1;
                    }
                }

                if (mPlayPos < 0) {
                    //pause();
                    stop(true);
                    notifyTrackEnd(STATUS_LIST_END);
                    return;
                }
            }
            saveBookmarkIfNeeded();
            stop(false);
            openCurrentAndNext(mIdPlayList != null);
            //play();
            //notifyChange(META_CHANGED);
        }
    }

    /**
     * Get the next position to play. Note that this may actually modify mPlayPos
     * if playback is in SHUFFLE_AUTO mode and the shuffle list window needed to
     * be adjusted. Either way, the return value is the next value that should be
     * assigned to mPlayPos;
     */
    private int getNextPosition(boolean force) {
        Log.d(LOGTAG, "getNextPosition");
        if (mRepeatMode == REPEAT_CURRENT) {
            Log.d(LOGTAG, "getNextPosition - mRepeatMode == REPEAT_CURRENT");
            if (mPlayPos < 0) return 0;
            return mPlayPos;
        } else if (mShuffleMode == SHUFFLE_NORMAL) {
            Log.d(LOGTAG, "getNextPosition - mShuffleMode == SHUFFLE_NORMAL");
            // Pick random next track from the not-yet-played ones
            // TODO: make it work right after adding/removing items in the queue.

            // Store the current file in the history, but keep the history at a
            // reasonable size
            if (mPlayPos >= 0) {
                mHistory.add(mPlayPos);
            }
            if (mHistory.size() > MAX_HISTORY_SIZE) {
                mHistory.removeElementAt(0);
            }

            int numTracks = mPlayListLen;
            int[] tracks = new int[numTracks];
            for (int i=0;i < numTracks; i++) {
                tracks[i] = i;
            }

            int numHistory = mHistory.size();
            int numUnplayed = numTracks;
            for (int i=0;i < numHistory; i++) {
                int idx = mHistory.get(i).intValue();
                if (idx < numTracks && tracks[idx] >= 0) {
                    numUnplayed--;
                    tracks[idx] = -1;
                }
            }

            // 'numUnplayed' now indicates how many tracks have not yet
            // been played, and 'tracks' contains the indices of those
            // tracks.
            if (numUnplayed <=0) {
                // everything's already been played
                if (mRepeatMode == REPEAT_ALL || force) {
                    //pick from full set
                    numUnplayed = numTracks;
                    for (int i=0;i < numTracks; i++) {
                        tracks[i] = i;
                    }
                } else {
                    // all done
                    return -1;
                }
            }
            int skip = mRand.nextInt(numUnplayed);
            int cnt = -1;
            while (true) {
                while (tracks[++cnt] < 0)
                    ;
                skip--;
                if (skip < 0) {
                    break;
                }
            }
            return cnt;
        } else if (mShuffleMode == SHUFFLE_AUTO) {
            Log.d(LOGTAG, "getNextPosition - mShuffleMode == SHUFFLE_AUTO");
            doAutoShuffleUpdate();
            return mPlayPos + 1;
        } else {
            if (mPlayPos >= mPlayListLen - 1) {
                // we're at the end of the list
                if (mRepeatMode == REPEAT_NONE && !force) {
                    // all done
                    return -1;
                } else if (mRepeatMode == REPEAT_ALL || force) {
                    return 0;
                }
                return -1;
            } else {
                return mPlayPos + 1;
            }
        }
    }

    public void gotoNext(boolean force) {
        Log.d(LOGTAG, "gotoNext");
        Log.d(LOGTAG, "gotoNext - force = " + force);
        synchronized (this) {
            if (mPlayListLen <= 0) {
                Log.d(LOGTAG, "No play queue");
                //pause();
                stop(true);
                notifyTrackEnd(STATUS_LIST_END);
                return;
            }

            int pos = getNextPosition(force);
            Log.d(LOGTAG, "gotoNext - pos = " + pos);
            if (pos < 0) {
                //pause();
                stop(true);
                notifyTrackEnd(STATUS_LIST_END);
                return;
            }
            mPlayPos = pos;
            saveBookmarkIfNeeded();
            Log.d(LOGTAG, "gotoNext->stop");
            stop(false);
            Log.d(LOGTAG, "gotoNext->openCurrentAndNext");
            openCurrentAndNext(mIdPlayList != null);
            //Log.d(LOGTAG, "gotoNext->play");
            //play();
            //notifyChange(META_CHANGED);
        }
    }
    
    private void gotoIdleState() {
        mDelayedStopHandler.removeCallbacksAndMessages(null);
        Message msg = mDelayedStopHandler.obtainMessage();
        mDelayedStopHandler.sendMessageDelayed(msg, IDLE_DELAY);
        stopForeground(true);
    }
    
    private void showToastMsg(int resId, int duration) {
        Toast.makeText(this, resId, duration).show();
    }

    private void saveBookmarkIfNeeded() {
        try {
            if (isPodcast()) {
                long pos = position();
                long bookmark = getBookmark();
                long duration = duration();
                if ((pos < bookmark && (pos + 10000) > bookmark) ||
                        (pos > bookmark && (pos - 10000) < bookmark)) {
                    // The existing bookmark is close to the current
                    // position, so don't update it.
                    return;
                }
                if (pos < 15000 || (pos + 10000) > duration) {
                    // if we're near the start or end, clear the bookmark
                    pos = 0;
                }
                
                // write 'pos' to the bookmark field
                ContentValues values = new ContentValues();
                values.put(MediaStore.Audio.Media.BOOKMARK, pos);
                Uri uri = ContentUris.withAppendedId(
                        MediaStore.Audio.Media.EXTERNAL_CONTENT_URI, mCursor.getLong(IDCOLIDX));
                getContentResolver().update(uri, values, null, null);
            }
        } catch (SQLiteException ex) {
        }
    }

    // Make sure there are at least 5 items after the currently playing item
    // and no more than 10 items before.
    private void doAutoShuffleUpdate() {
        Log.d(LOGTAG, "doAutoShuffleUpdate");
        boolean notify = false;

        // remove old entries
        if (mPlayPos > 10) {
            removeTracks(0, mPlayPos - 9);
            notify = true;
        }
        // add new entries if needed
        int to_add = 7 - (mPlayListLen - (mPlayPos < 0 ? -1 : mPlayPos));
        for (int i = 0; i < to_add; i++) {
            // pick something at random from the list

            int lookback = mHistory.size();
            int idx = -1;
            while(true) {
                idx = mRand.nextInt(mAutoShuffleList.length);
                if (!wasRecentlyUsed(idx, lookback)) {
                    break;
                }
                lookback /= 2;
            }
            mHistory.add(idx);
            if (mHistory.size() > MAX_HISTORY_SIZE) {
                mHistory.remove(0);
            }
            ensurePlayListCapacity(mPlayListLen + 1, mIdPlayList != null);
            mIdPlayList[mPlayListLen++] = mAutoShuffleList[idx];
            notify = true;
        }
        if (notify) {
            notifyChange(QUEUE_CHANGED);
        }
    }

    // check that the specified idx is not in the history (but only look at at
    // most lookbacksize entries in the history)
    private boolean wasRecentlyUsed(int idx, int lookbacksize) {

        // early exit to prevent infinite loops in case idx == mPlayPos
        if (lookbacksize == 0) {
            return false;
        }

        int histsize = mHistory.size();
        if (histsize < lookbacksize) {
            Log.d(LOGTAG, "lookback too big");
            lookbacksize = histsize;
        }
        int maxidx = histsize - 1;
        for (int i = 0; i < lookbacksize; i++) {
            long entry = mHistory.get(maxidx - i);
            if (entry == idx) {
                return true;
            }
        }
        return false;
    }

    // A simple variation of Random that makes sure that the
    // value it returns is not equal to the value it returned
    // previously, unless the interval is 1.
    private static class Shuffler {
        private int mPrevious;
        private Random mRandom = new Random();
        public int nextInt(int interval) {
            int ret;
            do {
                ret = mRandom.nextInt(interval);
            } while (ret == mPrevious && interval > 1);
            mPrevious = ret;
            return ret;
        }
    };

    private boolean makeAutoShuffleList() {
        Log.d(LOGTAG, "makeAutoShuffleList");
        ContentResolver res = getContentResolver();
        Cursor c = null;
        try {
            c = res.query(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI,
                    new String[] {MediaStore.Audio.Media._ID}, MediaStore.Audio.Media.IS_MUSIC + "=1",
                    null, null);
            if (c == null || c.getCount() == 0) {
                return false;
            }
            int len = c.getCount();
            long [] list = new long[len];
            for (int i = 0; i < len; i++) {
                c.moveToNext();
                list[i] = c.getLong(0);
            }
            mAutoShuffleList = list;
            return true;
        } catch (RuntimeException ex) {
        } finally {
            if (c != null) {
                c.close();
            }
        }
        return false;
    }
    
    /**
     * Removes the range of tracks specified from the play list. If a file within the range is
     * the file currently being played, playback will move to the next file after the
     * range. 
     * @param first The first file to be removed
     * @param last The last file to be removed
     * @return the number of tracks deleted
     */
    public int removeTracks(int first, int last) {
        int numremoved = removeTracksInternal(first, last);
        if (numremoved > 0) {
            notifyChange(QUEUE_CHANGED);
        }
        return numremoved;
    }
    
    private int removeTracksInternal(int first, int last) {
        synchronized (this) {
            if (last < first) return 0;
            if (first < 0) first = 0;
            if (last >= mPlayListLen) last = mPlayListLen - 1;

            boolean gotonext = false;
            if (first <= mPlayPos && mPlayPos <= last) {
                mPlayPos = first;
                gotonext = true;
            } else if (mPlayPos > last) {
                mPlayPos -= (last - first + 1);
            }
            int num = mPlayListLen - last - 1;
            for (int i = 0; i < num; i++) {
                mIdPlayList[first + i] = mIdPlayList[last + 1 + i];
            }
            mPlayListLen -= last - first + 1;
            
            if (gotonext) {
                if (mPlayListLen == 0) {
                    stop(true);
                    mPlayPos = -1;
                    if (mCursor != null) {
                        mCursor.close();
                        mCursor = null;
                    }
                } else {
                    if (mPlayPos >= mPlayListLen) {
                        mPlayPos = 0;
                    }
                    boolean wasPlaying = isPlaying();
                    stop(false);
                    openCurrentAndNext(mIdPlayList != null);
                    if (wasPlaying) {
                        play();
                    }
                }
                notifyChange(META_CHANGED);
            }
            return last - first + 1;
        }
    }
    
    /**
     * Removes all instances of the track with the given id
     * from the playlist.
     * @param id The id to be removed
     * @return how many instances of the track were removed
     */
    public int removeTrack(long id) {
        int numremoved = 0;
        synchronized (this) {
            for (int i = 0; i < mPlayListLen; i++) {
                if (mIdPlayList[i] == id) {
                    numremoved += removeTracksInternal(i, i);
                    i--;
                }
            }
        }
        if (numremoved > 0) {
            notifyChange(QUEUE_CHANGED);
        }
        return numremoved;
    }
    
    public void setShuffleMode(int shufflemode) {
        Log.d(LOGTAG, "setShuffleMode");
        synchronized(this) {
            if (mShuffleMode == shufflemode && mPlayListLen > 0) {
                return;
            }
            mShuffleMode = shufflemode;
            if (mShuffleMode == SHUFFLE_AUTO) {
                if (makeAutoShuffleList()) {
                    mPlayListLen = 0;
                    doAutoShuffleUpdate();
                    mPlayPos = 0;
                    openCurrentAndNext(mIdPlayList != null);
                    play();
                    notifyChange(META_CHANGED);
                    return;
                } else {
                    // failed to build a list of files to shuffle
                    mShuffleMode = SHUFFLE_NONE;
                }
            }
            saveQueue(false);
        }
    }
    public int getShuffleMode() {
        return mShuffleMode;
    }
    
    public void setRepeatMode(int repeatmode) {
        synchronized(this) {
            mRepeatMode = repeatmode;
            notifyChange(REPEATSTATE_CHANGED);
            setNextTrack(mIdPlayList != null);
            saveQueue(false);
        }
    }
    public int getRepeatMode() {
        return mRepeatMode;
    }

    public int getMediaMountedCount() {
        return mMediaMountedCount;
    }

    /**
     * Returns the path of the currently playing file, or null if
     * no file is currently playing.
     */
    public String getPath() {
        return mFileToPlay;
    }

    public String getUrlPlayListPath() {
        if (mUrlPlayList != null) {
            if (mPlayPos < 0) {
                mPlayPos = 0;
            }
            Log.d(LOGTAG,"getUrlPlayListPath urlpath="+mUrlPlayList[mPlayPos]);
            return mUrlPlayList[mPlayPos];
        }
        return null;
    }
    /**
     * Returns the rowid of the currently playing file, or -1 if
     * no file is currently playing.
     */
    public long getAudioId() {
        synchronized (this) {
            if (mIdPlayList == null)
                return -1;
            if (mPlayPos >= 0 && mPlayer.isInitialized()) {
                return mIdPlayList[mPlayPos];
            }
        }
        return -1;
    }
    
    /**
     * Returns the position in the queue 
     * @return the position in the queue
     */
    public int getQueuePosition() {
        synchronized(this) {
            return mPlayPos;
        }
    }
    
    /**
     * Starts playing the track at the given position in the queue.
     * @param pos The position in the queue of the track that will be played.
     */
    public void setQueuePosition(int pos) {
        synchronized(this) {
            stop(false);
            mPlayPos = pos;
            openCurrentAndNext(mIdPlayList != null);
            play();
            notifyChange(META_CHANGED);
            if (mShuffleMode == SHUFFLE_AUTO) {
                doAutoShuffleUpdate();
            }
        }
    }

    public String getArtistName() {
        synchronized(this) {
            if (mTrackInfo != null){
                return mTrackInfo.mArtistName;
            }
            if (mCursor == null) {
                return null;
            }
            
            return mCursor.getString(mCursor.getColumnIndexOrThrow(MediaStore.Audio.Media.ARTIST));
        }
    }
    
    public long getArtistId() {
        synchronized (this) {
            if (mCursor == null) {
                return -1;
            }
            return mCursor.getLong(mCursor.getColumnIndexOrThrow(MediaStore.Audio.Media.ARTIST_ID));
        }
    }

    public String getAlbumName() {
        synchronized (this) {
            if (mTrackInfo != null)
                return mTrackInfo.mAlbumName;
            
            if (mCursor == null) {
                return null;
            }
            
            return mCursor.getString(mCursor.getColumnIndexOrThrow(MediaStore.Audio.Media.ALBUM));
        }
    }

    public long getAlbumId() {
        synchronized (this) {
            if (mCursor == null) {
                return -1;
            }
            return mCursor.getLong(mCursor.getColumnIndexOrThrow(MediaStore.Audio.Media.ALBUM_ID));
        }
    }

    public String getTrackName() {
        synchronized (this) {
            if (mTrackInfo != null)
                return mTrackInfo.mTrackName;
            
            if (mCursor == null) {
                return null;
            }
            return mCursor.getString(mCursor.getColumnIndexOrThrow(MediaStore.Audio.Media.TITLE));
        }
    }

    private boolean isPodcast() {
        synchronized (this) {
            if (mCursor == null) {
                return false;
            }
            return (mCursor.getInt(PODCASTCOLIDX) > 0);
        }
    }
    
    private long getBookmark() {
        synchronized (this) {
            if (mCursor == null) {
                return 0;
            }
            return mCursor.getLong(BOOKMARKCOLIDX);
        }
    }
    
    /**
     * Returns the duration of the file in milliseconds.
     * Currently this method returns -1 for the duration of MIDI files.
     */
    public long duration() {
        if (mPlayer.isInitialized()) {
            return mPlayer.duration();
        }
        return -1;
    }

    /**
     * Returns the current playback position in milliseconds
     */
    public long position() {
        if (mPlayer.isInitialized()) {
            return mPlayer.position();
        }
        return -1;
    }

    /**
     * Seeks to the position specified.
     *
     * @param pos The position to seek to, in milliseconds
     */
    public long seek(long pos) {
        if (mPlayer.isInitialized()) {
            if (pos < 0) pos = 0;
            if (pos > mPlayer.duration()) pos = mPlayer.duration();
            return mPlayer.seek(pos);
        }
        return -1;
    }

    /**
     * Sets the audio session ID.
     *
     * @param sessionId: the audio session ID.
     */
    public void setAudioSessionId(int sessionId) {
        synchronized (this) {
            mPlayer.setAudioSessionId(sessionId);
        }
    }

    /**
     * Returns the audio session ID.
     */
    public int getAudioSessionId() {
        synchronized (this) {
            return mPlayer.getAudioSessionId();
        }
    }

    public void setBroadcastStatus(boolean broadcastStatus) {
        Log.d(LOGTAG, "setBroadcastStatus: " + broadcastStatus);
        mBroadcastStatus = broadcastStatus;
    }

    public boolean getBroadcastStatus() {
        Log.d(LOGTAG, "getBroadcastStatus: " + mBroadcastStatus);
        return mBroadcastStatus;
    }

    public void setSourceFromString(String sourceFromString) {
        Log.d(LOGTAG, "setSourceFromString: " + sourceFromString);
        mSourceFromString = sourceFromString;
    }

    public String getSourceFromString() {
        Log.d(LOGTAG, "getSourceFromString: " + mSourceFromString);
        return mSourceFromString;
    }

    public void setMediaConfigString(String mediaConfigString) {
        Log.d(LOGTAG, "setMediaConfigString: " + mediaConfigString);
        mMediaConfigString = mediaConfigString;
    }

    public void setUseRTMediaPlayer(boolean useRTMediaPlayer) {
        Log.d(LOGTAG, "setUseRTMediaPlayer: " + useRTMediaPlayer);
        mUseRTMediaPlayer = useRTMediaPlayer;
    }

    /**
     * Provides a unified interface for dealing with midi files and
     * other media files.
     */
    private class MultiPlayer {
        private CompatMediaPlayer mCurrentMediaPlayer = new CompatMediaPlayer();
        private CompatMediaPlayer mNextMediaPlayer;
        private Handler mHandler;
        private boolean mIsInitialized = false;

        public MultiPlayer() {
            mCurrentMediaPlayer.setWakeMode(MediaPlaybackService.this, PowerManager.PARTIAL_WAKE_LOCK);
        }

        public void setDataSource(String path) {
            Log.d(LOGTAG, "MultiPlayer.setDataSource, path = " + path);
            //mIsInitialized = setDataSourceImpl(mCurrentMediaPlayer, path);
            setDataSourceImpl(mCurrentMediaPlayer, path);
            if (mIsInitialized) {
                setNextDataSource(null);
            }
        }

        private boolean setDataSourceImpl(MediaPlayer player, String path) {
            try {
                Log.d(LOGTAG, "MultiPlayer.setDataSourceImpl, path = " + path);
                player.reset();
                player.setOnPreparedListener(preparedListener);
                if(mUseRTMediaPlayer==true) {
                    player.useRTMediaPlayer(MediaPlayer.FORCE_BY_SCORE);
                }
                else {
                    player.useRTMediaPlayer(MediaPlayer.FORCE_NONE);
                }
                if (path.startsWith("content://")) {
                    player.setDataSource(MediaPlaybackService.this, Uri.parse(path));
                } else {
                    if (mMediaConfigString != null) {
                        String[] mediaInfoStr = null;
                        String[] mediaConfigArray = mMediaConfigString.split(",");
                        Map<String, String> mediaConfigList = new HashMap<String, String> ();
                        for (int i=0 ; i<mediaConfigArray.length ; i++) {
                            mediaInfoStr = mediaConfigArray[i].split("=");
                            mediaConfigList.put(KEY_MEDIA_CONFIG + mediaInfoStr[0], mediaInfoStr[1]);
                            //Log.d(LOGTAG, "MultiPlayer.setDataSourceImpl, mediaInfoStr[0] = " + mediaInfoStr[0]);
                            //Log.d(LOGTAG, "MultiPlayer.setDataSourceImpl, mediaInfoStr[1] = " + mediaInfoStr[1]);
                        }
                        Log.d(LOGTAG, "MultiPlayer.setDataSourceImpl, mediaConfigList = " + mediaConfigList);
                        player.setDataSource(MediaPlaybackService.this, Uri.parse(path), mediaConfigList);
                    }
                    else {
                        player.setDataSource(path);
                    }
                }
                player.setAudioStreamType(AudioManager.STREAM_MUSIC);
                //player.prepare();
                player.prepareAsync();
                mIsPreparing = true;
            } catch (IOException ex) {
                // TODO: notify the user why the file couldn't be opened
                Log.d(LOGTAG, "MultiPlayer.setDataSourceImpl - catch IOException");
                gotoNext(false);
                return false;
            } catch (IllegalArgumentException ex) {
                // TODO: notify the user why the file couldn't be opened
                Log.d(LOGTAG, "MultiPlayer.setDataSourceImpl - catch IllegalArgumentException");
                gotoNext(false);
                return false;
            }
            player.setOnCompletionListener(listener);
            player.setOnErrorListener(errorListener);
            player.setOnInfoListener(infoListener);
            Intent i = new Intent(AudioEffect.ACTION_OPEN_AUDIO_EFFECT_CONTROL_SESSION);
            i.putExtra(AudioEffect.EXTRA_AUDIO_SESSION, getAudioSessionId());
            i.putExtra(AudioEffect.EXTRA_PACKAGE_NAME, getPackageName());
            sendBroadcast(i);
            return true;
        }

        public void setNextDataSource(String path) {
            // will enhance RTMediaPlayer in the future ...
            Log.d(LOGTAG, "Limitation : Do not create next media player !!!");
            /*
            Log.d(LOGTAG, "MultiPlayer.setNextDataSource - path = " + path);
            mCurrentMediaPlayer.setNextMediaPlayer(null);
            if (mNextMediaPlayer != null) {
                mNextMediaPlayer.release();
                mNextMediaPlayer = null;
            }
            if (path == null) {
                return;
            }
            if( path.startsWith("directconnect://") == true ) {
                // Directly return here, so that "directconnect" can play next normally.
                return;
            }
            mNextMediaPlayer = new CompatMediaPlayer();
            mNextMediaPlayer.setWakeMode(MediaPlaybackService.this, PowerManager.PARTIAL_WAKE_LOCK);
            mNextMediaPlayer.setAudioSessionId(getAudioSessionId());
            if (setDataSourceImpl(mNextMediaPlayer, path)) {
                mCurrentMediaPlayer.setNextMediaPlayer(mNextMediaPlayer);
            } else {
                // failed to open next, we'll transition the old fashioned way,
                // which will skip over the faulty file
                mNextMediaPlayer.release();
                mNextMediaPlayer = null;
            }
            */
        }
        
        public boolean isInitialized() {
            return mIsInitialized;
        }

        public void start() {
            Log.d(LOGTAG, "MultiPlayer.start");
            MusicUtils.debugLog(new Exception("MultiPlayer.start called"));
            mCurrentMediaPlayer.start();
        }

        public void stop() {
            Log.d(LOGTAG, "MultiPlayer.stop");
            mCurrentMediaPlayer.reset();
            mIsInitialized = false;
        }

        /**
         * You CANNOT use this player anymore after calling release()
         */
        public void release() {
            Log.d(LOGTAG, "MultiPlayer.release");
            stop();
            mCurrentMediaPlayer.release();
        }
        
        public void pause() {
            Log.d(LOGTAG, "MultiPlayer.pause");
            mCurrentMediaPlayer.pause();
        }
        
        public void setHandler(Handler handler) {
            Log.d(LOGTAG, "MultiPlayer.setHandler");
            mHandler = handler;
        }

        MediaPlayer.OnPreparedListener preparedListener = new MediaPlayer.OnPreparedListener() {
            public void onPrepared(MediaPlayer player) {
                Log.d(LOGTAG, "MultiPlayer.onPrepared");
                mOpenFailedCounter = 0;
                mIsInitialized = true;
                mIsPreparing = false;
                if (mPreLoad == true) {
                    mPreLoad = false;
                    return;
                }
                Intent i = new Intent(PREPARE_COMPLETED);
                sendBroadcast(i);
                play();
                notifyChange(META_CHANGED);
            }
        };

        MediaPlayer.OnCompletionListener listener = new MediaPlayer.OnCompletionListener() {
            public void onCompletion(MediaPlayer mp) {
                Log.d(LOGTAG, "MultiPlayer.onCompletion");
                if (mp == mCurrentMediaPlayer && mNextMediaPlayer != null) {
                    mCurrentMediaPlayer.release();
                    mCurrentMediaPlayer = mNextMediaPlayer;
                    mNextMediaPlayer = null;
                    Log.d(LOGTAG, "mHandler.sendEmptyMessage(TRACK_WENT_TO_NEXT)");
                    mHandler.sendEmptyMessage(TRACK_WENT_TO_NEXT);
                } else {
                    // Acquire a temporary wakelock, since when we return from
                    // this callback the MediaPlayer will release its wakelock
                    // and allow the device to go to sleep.
                    // This temporary wakelock is released when the RELEASE_WAKELOCK
                    // message is processed, but just in case, put a timeout on it.
                    /*
                    if (mIsInitialized == false && mSourceFromString == null) {		// fix bug 47952, bug 48179
                        Log.d(LOGTAG, "MediaPlaybackService already stop, skip it");
                        return;
                    }
                    */
                    mWakeLock.acquire(30000);
                    Log.d(LOGTAG, "mHandler.sendEmptyMessage(TRACK_ENDED)");
                    mHandler.sendEmptyMessage(TRACK_ENDED);
                    Log.d(LOGTAG, "mHandler.sendEmptyMessage(RELEASE_WAKELOCK)");
                    mHandler.sendEmptyMessage(RELEASE_WAKELOCK);
                }
            }
        };

        MediaPlayer.OnErrorListener errorListener = new MediaPlayer.OnErrorListener() {
            public boolean onError(MediaPlayer mp, int what, int extra) {
                Log.d(LOGTAG, "MultiPlayer.onError");
                switch (what) {
                case MediaPlayer.MEDIA_ERROR_SERVER_DIED:
                    mIsInitialized = false;
                    mCurrentMediaPlayer.release();
                    // Creating a new MediaPlayer and settings its wakemode does not
                    // require the media service, so it's OK to do this now, while the
                    // service is still being restarted
                    mCurrentMediaPlayer = new CompatMediaPlayer(); 
                    mCurrentMediaPlayer.setWakeMode(MediaPlaybackService.this, PowerManager.PARTIAL_WAKE_LOCK);
                    mHandler.sendMessageDelayed(mHandler.obtainMessage(SERVER_DIED), 2000);
                    return true;
                default:
                    Log.d("MultiPlayer", "Error: " + what + "," + extra);
                    break;
                }
                return true;
           }
        };

        MediaPlayer.OnInfoListener infoListener = new MediaPlayer.OnInfoListener() {
            public boolean onInfo(MediaPlayer mp, int what, int extra) {
                //Log.d(LOGTAG, "MultiPlayer.onInfo");
                if ((what == MEDIA_INFO_AUDIO_FATAL_ERROR) && (extra == MediaPlayer.UNKNOWN_FORMAT)) {
                    showToastMsg(R.string.playback_failed, Toast.LENGTH_SHORT);
                    gotoNext(false);
                }
                return true;
           }
        };

        public long duration() {
            Log.d(LOGTAG, "MultiPlayer.duration");
            if (isInitialized()) {
                return mCurrentMediaPlayer.getDuration();
            }
            return -1;
        }

        public long position() {
            Log.d(LOGTAG, "MultiPlayer.position");
            if (isInitialized()) {
                return mCurrentMediaPlayer.getCurrentPosition();
            }
            return -1;
        }

        public long seek(long whereto) {
            Log.d(LOGTAG, "MultiPlayer.seek");
            if (isInitialized()) {
                mCurrentMediaPlayer.seekTo((int) whereto);
                return whereto;
            }
            return -1;
        }

        public void setVolume(float vol) {
            Log.d(LOGTAG, "MultiPlayer.setVolume");
            mCurrentMediaPlayer.setVolume(vol, vol);
        }

        public void setAudioSessionId(int sessionId) {
            Log.d(LOGTAG, "MultiPlayer.setAudioSessionId");
            mCurrentMediaPlayer.setAudioSessionId(sessionId);
        }

        public int getAudioSessionId() {
            Log.d(LOGTAG, "MultiPlayer.getAudioSessionId");
            return mCurrentMediaPlayer.getAudioSessionId();
        }
    }

    static class CompatMediaPlayer extends MediaPlayer implements OnCompletionListener {

        private boolean mCompatMode = true;
        private MediaPlayer mNextPlayer;
        private OnCompletionListener mCompletion;

        public CompatMediaPlayer() {
            try {
                MediaPlayer.class.getMethod("setNextMediaPlayer", MediaPlayer.class);
                mCompatMode = false;
            } catch (NoSuchMethodException e) {
                mCompatMode = true;
                super.setOnCompletionListener(this);
            }
        }

        public void setNextMediaPlayer(MediaPlayer next) {
            Log.d(LOGTAG, "CompatMediaPlayer.setNextMediaPlayer");
            if (mCompatMode) {
                mNextPlayer = next;
            } else {
                super.setNextMediaPlayer(next);
            }
        }

        @Override
        public void setOnCompletionListener(OnCompletionListener listener) {
            Log.d(LOGTAG, "CompatMediaPlayer.setOnCompletionListener");
            if (mCompatMode) {
                mCompletion = listener;
            } else {
                super.setOnCompletionListener(listener);
            }
        }

        @Override
        public void onCompletion(MediaPlayer mp) {
            Log.d(LOGTAG, "CompatMediaPlayer.onCompletion");
            if (mNextPlayer != null) {
                // as it turns out, starting a new MediaPlayer on the completion
                // of a previous player ends up slightly overlapping the two
                // playbacks, so slightly delaying the start of the next player
                // gives a better user experience
                SystemClock.sleep(50);
                mNextPlayer.start();
            }
            mCompletion.onCompletion(this);
        }
    }

    /*
     * By making this a static class with a WeakReference to the Service, we
     * ensure that the Service can be GCd even when the system process still
     * has a remote reference to the stub.
     */
    static class ServiceStub extends IMediaPlaybackService.Stub {
        WeakReference<MediaPlaybackService> mService;
        
        ServiceStub(MediaPlaybackService service) {
            mService = new WeakReference<MediaPlaybackService>(service);
        }

        public void openFile(String path)
        {
            mService.get().open(path);
        }
        public void openIdList(long [] list, int position) {
            mService.get().open(list, position);
        }
        public void openUrlList(String [] list, int position) {
            mService.get().open(list, position);
        }
        public int getQueuePosition() {
            return mService.get().getQueuePosition();
        }
        public void setQueuePosition(int index) {
            mService.get().setQueuePosition(index);
        }
        public boolean isPlaying() {
            return mService.get().isPlaying();
        }
        public void stop() {
            mService.get().stop();
        }
        public void pause() {
            mService.get().pause();
        }
        public void play() {
            mService.get().play();
        }
        public void prev() {
            mService.get().prev();
        }
        public void next() {
            mService.get().gotoNext(false);
        }
        public String getTrackName() {
            return mService.get().getTrackName();
        }
        public String getAlbumName() {
            return mService.get().getAlbumName();
        }
        public long getAlbumId() {
            return mService.get().getAlbumId();
        }
        public String getArtistName() {
            return mService.get().getArtistName();
        }
        public long getArtistId() {
            return mService.get().getArtistId();
        }
        public void enqueue(long [] list , int action) {
            mService.get().enqueue(list, action);
        }
        public long [] getIdQueue() {
            return mService.get().getIdQueue();
        }
         public String [] getUrlQueue() {
            return mService.get().getUrlQueue();
        }
        public void moveQueueItem(int from, int to) {
            mService.get().moveQueueItem(from, to);
        }
        public String getPath() {
            return mService.get().getPath();
        }
        public long getAudioId() {
            return mService.get().getAudioId();
        }
        public long position() {
            return mService.get().position();
        }
        public long duration() {
            return mService.get().duration();
        }
        public long seek(long pos) {
            return mService.get().seek(pos);
        }
        public void setShuffleMode(int shufflemode) {
            mService.get().setShuffleMode(shufflemode);
        }
        public int getShuffleMode() {
            return mService.get().getShuffleMode();
        }
        public int removeTracks(int first, int last) {
            return mService.get().removeTracks(first, last);
        }
        public int removeTrack(long id) {
            return mService.get().removeTrack(id);
        }
        public void setRepeatMode(int repeatmode) {
            mService.get().setRepeatMode(repeatmode);
        }
        public int getRepeatMode() {
            return mService.get().getRepeatMode();
        }
        public int getMediaMountedCount() {
            return mService.get().getMediaMountedCount();
        }
        public int getAudioSessionId() {
            return mService.get().getAudioSessionId();
        }
        public void setBroadcastStatus(boolean broadcastStatus) {
            mService.get().setBroadcastStatus(broadcastStatus);
        }
        public boolean getBroadcastStatus() {
            return mService.get().getBroadcastStatus();
        }
        public void setSourceFromString(String sourceFromString) {
            mService.get().setSourceFromString(sourceFromString);
        }
        public String getSourceFromString() {
            return mService.get().getSourceFromString();
        }
        public void setMediaConfigString(String mediaConfigString) {
            mService.get().setMediaConfigString(mediaConfigString);
        }

        public void setUseRTMediaPlayer(boolean useRTMediaPlayer) {
            mService.get().setUseRTMediaPlayer(useRTMediaPlayer);
        }

        public String getUrlPlayListPath() {
            return mService.get().getUrlPlayListPath();
        }
    }

    @Override
    protected void dump(FileDescriptor fd, PrintWriter writer, String[] args) {
        writer.println("" + mPlayListLen + " items in queue, currently at index " + mPlayPos);
        writer.println("Currently loaded:");
        writer.println(getArtistName());
        writer.println(getAlbumName());
        writer.println(getTrackName());
        writer.println(getPath());
        writer.println("playing: " + mIsSupposedToBePlaying);
        writer.println("actual: " + mPlayer.mCurrentMediaPlayer.isPlaying());
        writer.println("shuffle mode: " + mShuffleMode);
        MusicUtils.debugDump(writer);
    }

    private final IBinder mBinder = new ServiceStub(this);

    private long refreshTimePosition() {
        //// Broadcast playback status for DMR usage////////////
        long next = -1;
        if (mBroadcastStatus) {
            next = 500;
            long pos = position();
            if (-1 != pos) {
                //Log.v(LOGTAG, "Broadcast TimePosition = " + pos / 1000);
                Intent i = new Intent(RTK_PLAYBACK_STATUS)
                                .putExtra("TimePosition", (int)pos / 1000);
                sendBroadcast(i);

                next = 1000 - (pos % 1000);
            }
        }
        return next;
        ////////////////////////////////////////////////
    }

    private void broadcastPlaybackStatus(String status) {
        //// Broadcast playback status for DMR usage////////////
        if (mBroadcastStatus) {
            Log.d(LOGTAG, "Broadcast PlayMode: " + status);
            Intent i = new Intent(RTK_PLAYBACK_STATUS)
                            .putExtra("PlayMode", status);
            i.putExtra("PlayType", "Audio");
            if (null != mSourceFromString) {
                Log.d(LOGTAG, "Source from: " + mSourceFromString);
                i.putExtra(KEY_SOURCE_FROM, mSourceFromString);
            }
            sendBroadcast(i);
        }
        ////////////////////////////////////////////////
    }

    private void broadcastDuration() {
        //// Broadcast playback status for DMR usage////////////
        if (mBroadcastStatus) {
            Log.d(LOGTAG, "Broadcast Duration = " + duration() / 1000);
            Intent i = new Intent(RTK_PLAYBACK_STATUS)
                            .putExtra("Duration", (int)duration() / 1000);
            sendBroadcast(i);
        }
        ////////////////////////////////////////////////
    }

    private void queueNextTimePositionRefresh(long delay) {
        if (-1 != delay && isPlaying()) {
            Message msg = mMediaplayerHandler.obtainMessage(UPDATE_TIME_POSITION);
            mMediaplayerHandler.removeMessages(UPDATE_TIME_POSITION);
            mMediaplayerHandler.sendMessageDelayed(msg, delay);
        }
    }
}
