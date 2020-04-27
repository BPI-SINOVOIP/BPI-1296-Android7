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

import android.app.Dialog;
import android.content.BroadcastReceiver;
import android.content.ContentResolver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.content.ComponentName;
import android.content.res.Configuration;
import android.net.ConnectivityManager;
import android.net.EthernetManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.RemoteException;
import android.preference.PreferenceManager;
import android.view.InputDevice;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.KeyEvent;
import android.widget.Toast;

import com.android.gallery3d.R;
import com.android.gallery3d.common.Utils;
import com.android.gallery3d.data.DataManager;
import com.android.gallery3d.data.MediaItem;
import com.android.gallery3d.data.MediaSet;
import com.android.gallery3d.data.Path;
import com.android.gallery3d.picasasource.PicasaSource;
import com.android.gallery3d.util.GalleryUtils;

import com.android.music.IMediaPlaybackService;
import com.android.gallery3d.util.GalleryUtils;

public final class GalleryActivity extends AbstractGalleryActivity implements OnCancelListener {
    public static final String EXTRA_SLIDESHOW = "slideshow";
	public static final String EXTRA_REPEAT = "repeat";
    public static final String EXTRA_DREAM = "dream";
    public static final String EXTRA_CROP = "crop";

    public static final String ACTION_REVIEW = "com.android.camera.action.REVIEW";
    public static final String KEY_GET_CONTENT = "get-content";
    public static final String KEY_GET_ALBUM = "get-album";
    public static final String KEY_TYPE_BITS = "type-bits";
    public static final String KEY_MEDIA_TYPES = "mediaTypes";
    public static final String KEY_DISMISS_KEYGUARD = "dismiss-keyguard";
    public static final String KEY_SOURCE_FROM = "SourceFrom";
    public static final String KEY_BROADCAST_STATUS = "android.intent.playback.broadcast.status";

    private static final String TAG = "GalleryActivity";
    private Dialog mVersionCheckDialog;
    private static final String RTK_PLAYBACK_STATUS = "rtk.provider.playback.STATUS";

    private static final String ACTIVITYCMD = "com.android.gallery3d.photoviewcommand";
    private static final String CMDSTOP = "stop";
    private static final String CMDPREVIOUS = "previous";
    private static final String CMDNEXT = "next";
    private static final String CMDPAUSE = "pause";
    private static final String CMDPLAY = "play";    
    private static final String CMDRESTART = "restart";
    private static final String CMDUPDATEPLAYLIST = "updatePlaylist";
    private static final String CMDREPEAT = "repeat";
    
    private static final String KEY_BG_MUSIC = "BgMusic";
    
    private static final int BGMUSIC = 1;

    private static final int BG_MUSIC_DELAY = 300;

    private IMediaPlaybackService mMediaPlaybackService = null;
    private boolean mMediaPlaybackServiceRegistered = false;
    private boolean mBgMusicReceiverRegistered = false;
    private boolean mEmbeddedBackgroundMusic = false;    

    private boolean mBroadcastStatus = false;
    private String mSourceFrom = null;

    private BroadcastReceiver mIntentReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            String cmd = intent.getStringExtra("command");
            Log.d(TAG, "mIntentReceiver.onReceive " + action + " / " + cmd);
            if (CMDSTOP.equals(cmd)) {
                finish();
            } else if (CMDPREVIOUS.equals(cmd)) {
                ActivityState topState = getStateManager().getTopState();
                Bundle data = topState.getData();
                if (null != data) {
                    String[] playlist = data.getStringArray(SlideshowPage.KEY_PLAYLIST);
                    if (null != playlist && data.getBoolean(SlideshowPage.KEY_NAVI_BY_COMMAND)) {
                        // Process previous command
                        SlideshowPage slidePage = (SlideshowPage)topState;
                        //// Broadcast playback status for DMR usage////////////
                        if (true == mBroadcastStatus) {
                            Log.d(TAG, "Broadcast PlayMode: Play");
                            Intent i = new Intent(RTK_PLAYBACK_STATUS)
                                            .putExtra("PlayMode", "Play");
                            i.putExtra("PlayType", "Image");
                            if (null != mSourceFrom) {
                                i.putExtra("SourceFrom", mSourceFrom);
                            }
                            sendBroadcast(i);
                        }
                        slidePage.onPrevious();
                    }
                }
            } else if (CMDNEXT.equals(cmd)) {
                ActivityState topState = getStateManager().getTopState();
                Bundle data = topState.getData();
                if (null != data) {
                    String[] playlist = data.getStringArray(SlideshowPage.KEY_PLAYLIST);
                    if (null != playlist && data.getBoolean(SlideshowPage.KEY_NAVI_BY_COMMAND)) {
                        // Process next command
                        SlideshowPage slidePage = (SlideshowPage)topState;
                        //// Broadcast playback status for DMR usage////////////
                        if (true == mBroadcastStatus) {
                            Log.d(TAG, "Broadcast PlayMode: Play");
                            Intent i = new Intent(RTK_PLAYBACK_STATUS)
                                            .putExtra("PlayMode", "Play");
                            i.putExtra("PlayType", "Image");
                            if (null != mSourceFrom) {
                                i.putExtra("SourceFrom", mSourceFrom);
                            }
                            sendBroadcast(i);
                        }
                        slidePage.onNext();
                    }
                }
            } else if (CMDPAUSE.equals(cmd)) {
                if (mSourceFrom.equals("DirectConnect")) {
                    SlideshowPage slidePage = (SlideshowPage)(getStateManager().getTopState());
                    slidePage.pause();
//                    getStateManager().pause();
                }
                //// Broadcast playback status for DMR usage////////////
                if (true == mBroadcastStatus) {
                    Log.d(TAG, "Broadcast PlayMode: Pause");
                    Intent i = new Intent(RTK_PLAYBACK_STATUS)
                                    .putExtra("PlayMode", "Pause");
                    i.putExtra("PlayType", "Image");
                    if (null != mSourceFrom) {
                        i.putExtra("SourceFrom", mSourceFrom);
                    }
                    sendBroadcast(i);
                }
                ////////////////////////////////////////////////
            } else if (CMDPLAY.equals(cmd)) {
                if (mSourceFrom.equals("DirectConnect")) {
                    SlideshowPage slidePage = (SlideshowPage)(getStateManager().getTopState());
                    slidePage.play();
//                    getStateManager().resume();
                }
                //// Broadcast playback status for DMR usage////////////
                if (true == mBroadcastStatus) {
                    Log.d(TAG, "Broadcast PlayMode: Play");
                    Intent i = new Intent(RTK_PLAYBACK_STATUS)
                                    .putExtra("PlayMode", "Play");
                    i.putExtra("PlayType", "Image");
                    if (null != mSourceFrom) {
                        i.putExtra("SourceFrom", mSourceFrom);
                    }
                    sendBroadcast(i);
                }
                ////////////////////////////////////////////////
            } else if (CMDUPDATEPLAYLIST.equals(cmd)) {
                ActivityState topState = getStateManager().getTopState();
                String playlist[] = intent.getStringArrayExtra("playlist-url");
                if (true == getStateManager().hasStateClass(SlideshowPage.class) && null != playlist) {
                    ((SlideshowPage)topState).onUpdatePlaylist(playlist);
                }
            } else if (CMDRESTART.equals(cmd)) {
                ActivityState topState = getStateManager().getTopState();
                String playlist[] = intent.getStringArrayExtra("playlist-url");
                if (true == getStateManager().hasStateClass(SlideshowPage.class) && null != playlist) {
                    ((SlideshowPage)topState).onRestart(playlist);
                }
            } else if (CMDREPEAT.equals(cmd)) {
                ActivityState topState = getStateManager().getTopState();
                Bundle data = topState.getData();
                if (null != data) {
                    String[] playlist = data.getStringArray(SlideshowPage.KEY_PLAYLIST);
                    if (null != playlist && data.getBoolean(SlideshowPage.KEY_NAVI_BY_COMMAND)) {
                        // Process next command
                        SlideshowPage slidePage = (SlideshowPage)topState;
                        slidePage.onRepeat();
                    }
                }
            }
        }
    };
    
    private BroadcastReceiver mBgMusicReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
            String[] urls = null;
            String sourceFrom = null;
            boolean bBroadcastStatus = false;
            if ((urls = intent.getStringArrayExtra("playlist-url")) != null ) {
                Bundle bundle = new Bundle();
                bundle.putStringArray(KEY_BG_MUSIC, urls);
                sourceFrom = intent.getStringExtra(KEY_SOURCE_FROM);
                if (null != sourceFrom) {
                    bundle.putString(KEY_SOURCE_FROM, sourceFrom);
                }
                bBroadcastStatus = intent.getBooleanExtra(KEY_BROADCAST_STATUS, false);
                bundle.putBoolean(KEY_BROADCAST_STATUS, bBroadcastStatus);
                Message msg = mHandler.obtainMessage(BGMUSIC);
                msg.setData(bundle);
                mHandler.removeMessages(BGMUSIC);
                mHandler.sendMessageDelayed(msg, BG_MUSIC_DELAY);
                Log.d(TAG, "Receive background music request. bBroadcastStatus=" + bBroadcastStatus + " sourceFrom=" + sourceFrom);
            }
            removeStickyBroadcast(intent);
        }
    };

    private ServiceConnection mMediaPlaybackServiceConn = new ServiceConnection() {
        @Override
        public void onServiceDisconnected(ComponentName name) {
            Log.d(TAG, "mMediaPlaybackServiceConn: disconnected");
            mMediaPlaybackService = null;
        }
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Log.d(TAG, "mMediaPlaybackServiceConn: connected");
            String urlList[] = null;
            
            mMediaPlaybackService = IMediaPlaybackService.Stub.asInterface(service);
            
            try {
                // Check if there is URL playlist in MediaPlaybackService
                urlList = mMediaPlaybackService.getUrlQueue();
                if (urlList != null && true == mMediaPlaybackService.isPlaying()) {
                    Log.d(TAG, "mMediaPlaybackServiceConn: has URL list");
                    // Check URL type
                    if (true == urlList[0].startsWith("directconnect://") && 0 == mSourceFrom.compareTo("DirectConnect")) {
                        if (false == mMediaPlaybackServiceRegistered)
                        {
                            IntentFilter f = new IntentFilter();
                            f.addAction("com.android.music.playstatechanged");
                            f.addAction("com.android.music.metachanged");
                            f.addAction("com.android.music.trackend");
                            f.addAction("com.android.music.listend");
                            registerReceiver(mMediaPlaybackServiceReceiver, new IntentFilter(f));

                            mMediaPlaybackServiceRegistered = true;
                        }
                    }
                }
            } catch (RemoteException ex) {
            }
            
        }
    };

    private BroadcastReceiver mMediaPlaybackServiceReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (action.equals("com.android.music.listend")) {
                Log.d(TAG, "PLAYBACK_LIST and Receive STATUS_LIST_END");
                try {
                    // Stop background music
                    mMediaPlaybackService.stop();
                } catch (RemoteException ex) {
                }
            }
        }
    };

    private BroadcastReceiver mRemovedReceiver = new BroadcastReceiver()
    {
        public void onReceive(Context context, Intent intent)
        {
            Log.d(TAG, "intent.getAction()="+intent.getAction());
            if (intent.getAction().equals(Intent.ACTION_MEDIA_REMOVED)
                || intent.getAction().equals(Intent.ACTION_MEDIA_BAD_REMOVAL)) {
                String currentPath = null;
                ActivityState topState = getStateManager().getTopState();
                if(getStateManager().hasStateClass(SlideshowPage.class) == true) {
                    currentPath = ((SlideshowPage)topState).getCurrentPhotoPath();
                }
                else if(getStateManager().hasStateClass(PhotoPage.class) == true) {
                    currentPath = ((PhotoPage)topState).getCurrentPhotoPath();
                }

                if(currentPath!=null && !currentPath.startsWith("file://")) {
                    currentPath = "file://" + currentPath;
                }
                String data = intent.getData().toString();

                if(currentPath!=null && currentPath.indexOf(data)>=0) {
                    Log.d(TAG, currentPath+" is removed.");
                    finish();
                }
            }
            else if (intent.getAction().equals(ConnectivityManager.CONNECTIVITY_ACTION) ||
                        intent.getAction().equals(ConnectivityManager.INET_CONDITION_ACTION) ||
                        intent.getAction().equals(EthernetManager.NETWORK_STATE_CHANGED_ACTION)) {
                if(mSourceFrom!=null && mSourceFrom.equals("Network")) {
                    ActivityState topState = getStateManager().getTopState();
                    if(!GalleryUtils.hasNetwork(topState.mActivity)) {
                        finish();
                    }
                }
            }
        }
    };

    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case BGMUSIC:
                    {
                        Bundle bundle = msg.getData();
                        String urls[] = bundle.getStringArray(KEY_BG_MUSIC);
                        String sourceFrom = bundle.getString(KEY_SOURCE_FROM);
                        boolean bBroadcastStatus = bundle.getBoolean(KEY_BROADCAST_STATUS);
                        if (null != mMediaPlaybackService) {
                            try {
                                Log.d(TAG, "Play background music. bBroadcastStatus=" + bBroadcastStatus + " sourceFrom=" + sourceFrom);
                                mEmbeddedBackgroundMusic = true;
                                if (false == mMediaPlaybackServiceRegistered)
                                {
                                    IntentFilter f = new IntentFilter();
                                    f.addAction("com.android.music.trackend");
                                    f.addAction("com.android.music.listend");
                                    registerReceiver(mMediaPlaybackServiceReceiver, new IntentFilter(f));

                                    mMediaPlaybackServiceRegistered = true;
                                }

                                mMediaPlaybackService.stop();
                                if (null != sourceFrom) {
                                    mMediaPlaybackService.setSourceFromString(sourceFrom);
                                }
                                mMediaPlaybackService.setBroadcastStatus(bBroadcastStatus);
                                mMediaPlaybackService.openUrlList(urls, 0);
                                // Bug 48262. MediaPlaybackService has modified to asynchronous mode, do not execute "play" after open url.
                                // TODO : Maybe need to provide new API.
                                //mMediaPlaybackService.play();
                            } catch (RemoteException ex) {
                            }
                        } else {
                            Log.e(TAG, "mMediaPlaybackService is null");
                            Bundle newBundle = new Bundle();
                            newBundle.putStringArray(KEY_BG_MUSIC, urls);
                            newBundle.putString(KEY_SOURCE_FROM, sourceFrom);
                            newBundle.putBoolean(KEY_BROADCAST_STATUS, bBroadcastStatus);
                            Message newMsg = mHandler.obtainMessage(BGMUSIC);
                            newMsg.setData(newBundle);
                            mHandler.removeMessages(BGMUSIC);
                            mHandler.sendMessageDelayed(newMsg, BG_MUSIC_DELAY);
                        }
                    }
                    break;

                default:
                    break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.w(TAG, "onCreate");
        super.onCreate(savedInstanceState);
        GalleryUtils.awakenDreams();
        requestWindowFeature(Window.FEATURE_ACTION_BAR);
        requestWindowFeature(Window.FEATURE_ACTION_BAR_OVERLAY);

        if (getIntent().getBooleanExtra(KEY_DISMISS_KEYGUARD, false)) {
            getWindow().addFlags(
                    WindowManager.LayoutParams.FLAG_DISMISS_KEYGUARD);
        }

        //Make app to full screen
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
            WindowManager.LayoutParams.FLAG_FULLSCREEN);
        // Hide both the navigation bar and the status bar.
        int uiOptions = View.SYSTEM_UI_FLAG_LAYOUT_STABLE
//                | View.SYSTEM_UI_FLAG_IMMERSIVE
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
        getWindow().getDecorView().setSystemUiVisibility(uiOptions);

        setContentView(R.layout.main);

        GalleryUtils.checkReal4KPhoto();

        if (savedInstanceState != null) {
            getStateManager().restoreFromState(savedInstanceState);
        } else {
            initializeByIntent();
        }
    }

    private void initializeByIntent() {
        Intent intent = getIntent();
        String action = intent.getAction();

        mBroadcastStatus = intent.getBooleanExtra(KEY_BROADCAST_STATUS, false);
        mSourceFrom = intent.getStringExtra("SourceFrom");

        if (Intent.ACTION_GET_CONTENT.equalsIgnoreCase(action)) {
            startGetContent(intent);
        } else if (Intent.ACTION_PICK.equalsIgnoreCase(action)) {
            // We do NOT really support the PICK intent. Handle it as
            // the GET_CONTENT. However, we need to translate the type
            // in the intent here.
            Log.w(TAG, "action PICK is not supported");
            String type = Utils.ensureNotNull(intent.getType());
            if (type.startsWith("vnd.android.cursor.dir/")) {
                if (type.endsWith("/image")) intent.setType("image/*");
                if (type.endsWith("/video")) intent.setType("video/*");
            }
            startGetContent(intent);
        } else if (Intent.ACTION_VIEW.equalsIgnoreCase(action)
                || ACTION_REVIEW.equalsIgnoreCase(action)){
            startViewAction(intent);
        } else {
            startDefaultPage();
        }
    }

    public void startDefaultPage() {
        PicasaSource.showSignInReminder(this);
        Bundle data = new Bundle();
        data.putString(AlbumSetPage.KEY_MEDIA_PATH,
                getDataManager().getTopSetPath(DataManager.INCLUDE_ALL));
        getStateManager().startState(AlbumSetPage.class, data);
        mVersionCheckDialog = PicasaSource.getVersionCheckDialog(this);
        if (mVersionCheckDialog != null) {
            mVersionCheckDialog.setOnCancelListener(this);
        }
    }

    private void startGetContent(Intent intent) {
        Bundle data = intent.getExtras() != null
                ? new Bundle(intent.getExtras())
                : new Bundle();
        data.putBoolean(KEY_GET_CONTENT, true);
        int typeBits = GalleryUtils.determineTypeBits(this, intent);
        data.putInt(KEY_TYPE_BITS, typeBits);
        data.putString(AlbumSetPage.KEY_MEDIA_PATH,
                getDataManager().getTopSetPath(typeBits));
        getStateManager().startState(AlbumSetPage.class, data);
    }

    private String getContentType(Intent intent) {
        String type = intent.getType();
        if (type != null) {
            return GalleryUtils.MIME_TYPE_PANORAMA360.equals(type)
                ? MediaItem.MIME_TYPE_JPEG : type;
        }

        Uri uri = intent.getData();
        try {
            return getContentResolver().getType(uri);
        } catch (Throwable t) {
            Log.w(TAG, "get type fail", t);
            return null;
        }
    }

    private void startViewAction(Intent intent) {
//        Boolean slideshow = intent.getBooleanExtra(EXTRA_SLIDESHOW, false);
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getAndroidContext());
        Boolean slideshow = prefs.getBoolean(PhotoPage.KEY_SLIDESHOW_PREF, false);
        if (slideshow) {
            if(getActionBar()!=null)
            	getActionBar().hide();
            DataManager manager = getDataManager();
            Path path = manager.findPathByUri(intent.getData(), intent.getType());
            if (path == null || manager.getMediaObject(path)
                    instanceof MediaItem) {
                path = Path.fromString(
                        manager.getTopSetPath(DataManager.INCLUDE_IMAGE));
            }
            Bundle data = new Bundle();
            data.putString(SlideshowPage.KEY_SET_PATH, path.toString());
            data.putInt(SlideshowPage.KEY_PHOTO_INDEX, intent.getIntExtra(PhotoPage.KEY_INDEX_HINT, 0));
            data.putString(SlideshowPage.KEY_ITEM_PATH, intent.getData().toString());
            int slideshowTiming = prefs.getInt(PhotoPage.KEY_SLIDESHOW_TIMING_PREF, 5);
            data.putInt(SlideshowPage.KEY_SLIDESHOW_TIMING, slideshowTiming);
            int slideshowEffect = prefs.getInt(PhotoPage.KEY_SLIDESHOW_EFFECT_PREF, SlideshowPage.DEFAULT_SLIDESHOW_EFFECT);
            data.putInt(SlideshowPage.KEY_EFFECT_MODE, slideshowEffect);
            int slideshowRepeatMode = prefs.getInt(PhotoPage.KEY_SLIDESHOW_REPEAT_MODE_PREF, 0);
            switch(slideshowRepeatMode)
            {
                case 0:
                    data.putBoolean(SlideshowPage.KEY_REPEAT, false);
                    data.putBoolean(SlideshowPage.KEY_RANDOM_ORDER, false);
                    break;
                case 1:
                    data.putBoolean(SlideshowPage.KEY_REPEAT, true);
                    data.putBoolean(SlideshowPage.KEY_RANDOM_ORDER, true);
                    break;
                case 2:
                    data.putBoolean(SlideshowPage.KEY_REPEAT, true);
                    data.putBoolean(SlideshowPage.KEY_RANDOM_ORDER, false);
                    break;
                default:
                    data.putBoolean(SlideshowPage.KEY_REPEAT, false);
                    data.putBoolean(SlideshowPage.KEY_RANDOM_ORDER, false);
                    break;
            }
            data.putString(SlideshowPage.KEY_SOURCE_FROM, intent.getStringExtra(SlideshowPage.KEY_SOURCE_FROM));
            data.putBoolean(KEY_BROADCAST_STATUS, mBroadcastStatus);
            String PlayListAddress = intent.getStringExtra(SlideshowPage.KEY_PLAYLIST_ADDR);
            String[] playlist = null;
            if(PlayListAddress!=null && !PlayListAddress.equals("Intent")) {
                playlist = GalleryUtils.getPlayListFromFile(PlayListAddress);
            }
            else {
                playlist = intent.getStringArrayExtra(SlideshowPage.KEY_PLAYLIST);
            }
            playlist = GalleryUtils.removePathPrefix(playlist, true);

            data.putStringArray(SlideshowPage.KEY_PLAYLIST, playlist);
            String PlayListTitleAddress = intent.getStringExtra(SlideshowPage.KEY_PLAYLIST_TITLE_ADDR);
            String[] playListTitle = null;
            if(PlayListTitleAddress!=null && !PlayListTitleAddress.equals("Intent")) {
                playListTitle = GalleryUtils.getPlayListFromFile(PlayListTitleAddress);
            }
            else {
                playListTitle = intent.getStringArrayExtra(SlideshowPage.KEY_PLAYLIST_TITLE);
            }
            data.putStringArray(SlideshowPage.KEY_PLAYLIST_TITLE, playListTitle);
            boolean naviByCommand = false;
            if (-1 != intent.getData().toString().indexOf("directconnect://")) {
                naviByCommand = true;
            }
            data.putBoolean(SlideshowPage.KEY_NAVI_BY_COMMAND, naviByCommand);
            if (intent.getBooleanExtra(EXTRA_DREAM, false)) {
                data.putBoolean(SlideshowPage.KEY_DREAM, true);
            }
            //// Broadcast playback status ////////////
            if (true == mBroadcastStatus) {
                Log.d(TAG, "Broadcast PlayMode: Play");
                Intent i = new Intent(RTK_PLAYBACK_STATUS)
                                .putExtra("PlayMode", "Play");
                i.putExtra("PlayType", "Image");
                if (null != mSourceFrom) {
                    i.putExtra("SourceFrom", mSourceFrom);
                }
                sendBroadcast(i);
            }
            ////////////////////////////////////////////////

            ////Bind service & Register intent filter//////////////////////
            Intent it = new Intent();
            it.setClassName("com.android.music", "com.android.music.MediaPlaybackService");
            String packageName = it.getComponent().getPackageName();
            Log.d(TAG, "grantReadPermission packageName="+packageName);
            Uri uri = Uri.parse("content://media/external/fs_id");
            grantUriPermission(packageName, uri, Intent.FLAG_GRANT_READ_URI_PERMISSION);

            bindService(it, mMediaPlaybackServiceConn, Context.BIND_AUTO_CREATE);

            IntentFilter f = new IntentFilter("rtk.gallery3d.background.MUSIC");
            registerReceiver(mBgMusicReceiver, f);
            mBgMusicReceiverRegistered = true;
            ////////////////////////////////////////////////
            getStateManager().startState(SlideshowPage.class, data);
        } else {
            Bundle data = new Bundle();
            DataManager dm = getDataManager();
            Uri uri = intent.getData();
            String contentType = getContentType(intent);
            Log.d(TAG, "uri = " + uri);
            Log.d(TAG, "contentType = " +  contentType);

            if (contentType == null) {
                Toast.makeText(this,
                        R.string.no_such_item, Toast.LENGTH_LONG).show();
                finish();
                return;
            }
            if (uri == null) {
                int typeBits = GalleryUtils.determineTypeBits(this, intent);
                data.putInt(KEY_TYPE_BITS, typeBits);
                data.putString(AlbumSetPage.KEY_MEDIA_PATH,
                        getDataManager().getTopSetPath(typeBits));
                getStateManager().startState(AlbumSetPage.class, data);
            } else if (contentType.startsWith(
                    ContentResolver.CURSOR_DIR_BASE_TYPE)) {
                int mediaType = intent.getIntExtra(KEY_MEDIA_TYPES, 0);
                if (mediaType != 0) {
                    uri = uri.buildUpon().appendQueryParameter(
                            KEY_MEDIA_TYPES, String.valueOf(mediaType))
                            .build();
                }
                Path setPath = dm.findPathByUri(uri, null);
                MediaSet mediaSet = null;
                if (setPath != null) {
                    mediaSet = (MediaSet) dm.getMediaObject(setPath);
                }
                if (mediaSet != null) {
                    if (mediaSet.isLeafAlbum()) {
                        data.putString(AlbumPage.KEY_MEDIA_PATH, setPath.toString());
                        data.putString(AlbumPage.KEY_PARENT_MEDIA_PATH,
                                dm.getTopSetPath(DataManager.INCLUDE_ALL));
                        getStateManager().startState(AlbumPage.class, data);
                    } else {
                        data.putString(AlbumSetPage.KEY_MEDIA_PATH, setPath.toString());
                        getStateManager().startState(AlbumSetPage.class, data);
                    }
                } else {
                    startDefaultPage();
                }
            } else {
                Path itemPath = dm.findPathByUri(uri, contentType);
                Path albumPath = dm.getDefaultSetOf(itemPath);

                data.putString(PhotoPage.KEY_MEDIA_ITEM_PATH, itemPath.toString());
                data.putString(PhotoPage.KEY_MEDIA_CONTENT_TYPE, contentType);
                data.putBoolean(PhotoPage.KEY_READONLY, true);
                data.putString(SlideshowPage.KEY_SOURCE_FROM, intent.getStringExtra(SlideshowPage.KEY_SOURCE_FROM));
                data.putBoolean(KEY_BROADCAST_STATUS, mBroadcastStatus);
                String PlayListAddress = intent.getStringExtra(PhotoPage.KEY_PLAYLIST_ADDR);
                String[] playlist = null;
                if(PlayListAddress!=null && !PlayListAddress.equals("Intent")) {
                    playlist = GalleryUtils.getPlayListFromFile(PlayListAddress);
                }
                else {
                    playlist = intent.getStringArrayExtra(PhotoPage.KEY_PLAYLIST);
                }
                playlist = GalleryUtils.removePathPrefix(playlist, true);

                if (playlist != null && playlist.length>1) {
                    albumPath = Path.fromString(dm.getTopSetPath(DataManager.INCLUDE_ALL));
                }
                String PlayListTitleAddress = intent.getStringExtra(PhotoPage.KEY_PLAYLIST_TITLE_ADDR);
                String[] playListTitle = null;
                if(PlayListTitleAddress!=null && !PlayListTitleAddress.equals("Intent")) {
                    playListTitle = GalleryUtils.getPlayListFromFile(PlayListTitleAddress);
                }
                else {
                    playListTitle = intent.getStringArrayExtra(PhotoPage.KEY_PLAYLIST_TITLE);
                }
                data.putStringArray(PhotoPage.KEY_PLAYLIST_TITLE, playListTitle);
                boolean singleItemOnly = (albumPath == null)
                        || intent.getBooleanExtra("SingleItemOnly", false);
                Log.d(TAG, "singleItemOnly = " +  singleItemOnly);

                if (!singleItemOnly) {
                    data.putString(PhotoPage.KEY_MEDIA_SET_PATH, albumPath.toString());
                    data.putStringArray(PhotoPage.KEY_PLAYLIST, playlist);
                    data.putInt(PhotoPage.KEY_INDEX_HINT, intent.getIntExtra(PhotoPage.KEY_INDEX_HINT, 0));
                    // when FLAG_ACTIVITY_NEW_TASK is set, (e.g. when intent is fired
                    // from notification), back button should behave the same as up button
                    // rather than taking users back to the home screen
                    if (intent.getBooleanExtra(PhotoPage.KEY_TREAT_BACK_AS_UP, false)) {
//                            || ((intent.getFlags() & Intent.FLAG_ACTIVITY_NEW_TASK) != 0)) {
                        data.putBoolean(PhotoPage.KEY_TREAT_BACK_AS_UP, true);
                    }
                }

                getStateManager().startState(SinglePhotoPage.class, data);
                //// Broadcast playback status ////////////
                if (true == mBroadcastStatus) {
                    boolean isFromDMR = mSourceFrom != null && mSourceFrom.startsWith("DMR");
                    if (!isFromDMR) {
                        Log.d(TAG, "Broadcast PlayMode: Play");
                        Intent i = new Intent(RTK_PLAYBACK_STATUS)
                            .putExtra("PlayMode", "Play");
                            i.putExtra("PlayType", "Image");
                        if (null != mSourceFrom) {
                            i.putExtra("SourceFrom", mSourceFrom);
                        }
                        sendBroadcast(i);
                    }
                }
                ////////////////////////////////////////////////
            }
        }
    }

    @Override
    protected void onResume() {
        Log.w(TAG, "onResume");
        Utils.assertTrue(getStateManager().getStateCount() >= 0);
        super.onResume();
        if (mVersionCheckDialog != null) {
            mVersionCheckDialog.show();
        }
        IntentFilter commandFilter = new IntentFilter();
        commandFilter.addAction(ACTIVITYCMD);
        registerReceiver(mIntentReceiver, commandFilter);

        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(Intent.ACTION_MEDIA_UNMOUNTED);
        intentFilter.addAction(Intent.ACTION_MEDIA_EJECT);
        intentFilter.addAction(Intent.ACTION_MEDIA_REMOVED);
        intentFilter.addAction(Intent.ACTION_MEDIA_SHARED);
        intentFilter.addAction(Intent.ACTION_MEDIA_BAD_REMOVAL);
        intentFilter.addDataScheme("file");
        registerReceiver(mRemovedReceiver, intentFilter);
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
        registerReceiver(mRemovedReceiver, intentFilter);
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
    protected void onPause() {
        Log.w(TAG, "onPause");
        super.onPause();
        if (mVersionCheckDialog != null) {
            mVersionCheckDialog.dismiss();
        }
        unregisterReceiver(mIntentReceiver);
        unregisterReceiver(mRemovedReceiver);
    }

    @Override
    public void onCancel(DialogInterface dialog) {
        if (dialog == mVersionCheckDialog) {
            mVersionCheckDialog = null;
        }
    }

    protected void onStop (){
        //Toast.makeText(this,"onStop - call finish",Toast.LENGTH_SHORT).show();
        // BUG_FIX: 43771
        super.onStop();
        finish();
    }

	@Override
    public void onDestroy() {
        Log.w(TAG, "onDestroy");
        if (mMediaPlaybackServiceRegistered == true) {
            try {
                // Stop background music
                mMediaPlaybackService.stop();
            } catch (RemoteException ex) {
            }
        }

		//// Broadcast playback status ////////////
        if (true == mBroadcastStatus) {
            Log.d(TAG, "Broadcast PlayMode: Stop");
            Intent i = new Intent(RTK_PLAYBACK_STATUS)
                            .putExtra("PlayMode", "Stop");
            i.putExtra("PlayType", "Image");
            if (null != mSourceFrom) {
                i.putExtra("SourceFrom", mSourceFrom);
            }
            sendBroadcast(i);
        }
		////////////////////////////////////////////////
        if (true == mBgMusicReceiverRegistered) {
            unbindService(mMediaPlaybackServiceConn);
            unregisterReceiver(mBgMusicReceiver);
            mBgMusicReceiverRegistered = false;
        }
        if (true == mMediaPlaybackServiceRegistered) {
            unregisterReceiver(mMediaPlaybackServiceReceiver);
            mMediaPlaybackServiceRegistered = false;
        }                
        super.onDestroy();
    }

    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {
        final boolean isTouchPad = (event.getSource()
                & InputDevice.SOURCE_CLASS_POSITION) != 0;
        if (isTouchPad) {
            float maxX = event.getDevice().getMotionRange(MotionEvent.AXIS_X).getMax();
            float maxY = event.getDevice().getMotionRange(MotionEvent.AXIS_Y).getMax();
            View decor = getWindow().getDecorView();
            float scaleX = decor.getWidth() / maxX;
            float scaleY = decor.getHeight() / maxY;
            float x = event.getX() * scaleX;
            //x = decor.getWidth() - x; // invert x
            float y = event.getY() * scaleY;
            //y = decor.getHeight() - y; // invert y
            MotionEvent touchEvent = MotionEvent.obtain(event.getDownTime(),
                    event.getEventTime(), event.getAction(), x, y, event.getMetaState());
            return dispatchTouchEvent(touchEvent);
        }
        return super.onGenericMotionEvent(event);
    }

    @Override
    public void onUserLeaveHint() {
        Log.w(TAG, "onUserLeaveHint");
        if (null != mSourceFrom &&
            0 == mSourceFrom.compareTo("DirectConnect")) {
            finish();
        }
        super.onUserLeaveHint();
    }
}
