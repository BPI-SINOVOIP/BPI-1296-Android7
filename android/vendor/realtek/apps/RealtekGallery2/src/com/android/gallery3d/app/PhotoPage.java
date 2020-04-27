/*
 * Copyright (C) 2010 The Android Open Source Project
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
import android.app.ActionBar.OnMenuVisibilityListener;
import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.Point;
import android.graphics.Rect;
import android.net.Uri;
import android.nfc.NfcAdapter;
import android.nfc.NfcAdapter.CreateBeamUrisCallback;
import android.nfc.NfcEvent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.provider.MediaStore;
import android.view.Display;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.RelativeLayout;
import android.widget.ShareActionProvider;
import android.widget.Toast;

import com.android.gallery3d.R;
import com.android.gallery3d.common.ApiHelper;
import com.android.gallery3d.data.ComboAlbum;
import com.android.gallery3d.data.DataManager;
import com.android.gallery3d.data.FilterDeleteSet;
import com.android.gallery3d.data.FilterSource;
import com.android.gallery3d.data.LocalImage;
import com.android.gallery3d.data.MediaDetails;
import com.android.gallery3d.data.MediaItem;
import com.android.gallery3d.data.MediaObject;
import com.android.gallery3d.data.MediaObject.PanoramaSupportCallback;
import com.android.gallery3d.data.MediaSet;
import com.android.gallery3d.data.Path;
import com.android.gallery3d.data.SecureAlbum;
import com.android.gallery3d.data.SecureSource;
import com.android.gallery3d.data.SnailAlbum;
import com.android.gallery3d.data.SnailItem;
import com.android.gallery3d.data.SnailSource;
import com.android.gallery3d.filtershow.FilterShowActivity;
import com.android.gallery3d.filtershow.crop.CropActivity;
import com.android.gallery3d.picasasource.PicasaSource;
import com.android.gallery3d.ui.DetailsHelper;
import com.android.gallery3d.ui.DetailsHelper.CloseListener;
import com.android.gallery3d.ui.DetailsHelper.DetailsSource;
import com.android.gallery3d.ui.GLRootView;
import com.android.gallery3d.ui.GLView;
import com.android.gallery3d.ui.MenuExecutor;
import com.android.gallery3d.ui.PhotoView;
import com.android.gallery3d.ui.SelectionManager;
import com.android.gallery3d.ui.SynchronizedHandler;
import com.android.gallery3d.util.GalleryUtils;
import com.android.gallery3d.util.UsageStatistics;

import java.util.ArrayList;
import java.util.Date;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import com.realtek.bitmapfun.util.HeifReader;

public abstract class PhotoPage extends ActivityState implements
        PhotoView.Listener, AppBridge.Server, ShareActionProvider.OnShareTargetSelectedListener,
        PhotoPageBottomControls.Delegate, GalleryActionBar.OnAlbumModeSelectedListener {
    private static final String TAG = "PhotoPage";

    private static final int MSG_HIDE_BARS = 1;
    private static final int MSG_ON_FULL_SCREEN_CHANGED = 4;
    private static final int MSG_UPDATE_ACTION_BAR = 5;
    private static final int MSG_UNFREEZE_GLROOT = 6;
    private static final int MSG_WANT_BARS = 7;
    private static final int MSG_REFRESH_BOTTOM_CONTROLS = 8;
    private static final int MSG_ON_CAMERA_CENTER = 9;
    private static final int MSG_ON_PICTURE_CENTER = 10;
    private static final int MSG_REFRESH_IMAGE = 11;
    private static final int MSG_UPDATE_PHOTO_UI = 12;
    private static final int MSG_UPDATE_DEFERRED = 14;
    private static final int MSG_UPDATE_SHARE_URI = 15;
    private static final int MSG_UPDATE_PANORAMA_UI = 16;
    private static final int MSG_ENABLE_SLIDESHOW = 17;
    private static final int MSG_DISABLE_SLIDESHOW = 18;
    private static final int MSG_SHOW_OPTIONMENU = 19;
    private static final int MSG_STOP = 20;
    private static final int MSG_SHOW_REPEATMENU = 21;
    private static final int MSG_UPDATE_PHOTO_INFO = 22;
    private static final int MSG_SHOW_TRANSITION_EFFECT_MENU = 23;
    private static final int MSG_SHOW_SLIDESHOW_TIMING_MENU = 24;
    private static final int MSG_SHOW_BG_NUSIC_MENU = 25;
    private static final int MSG_SHOW_EXIF_MENU = 26;
    private static final int MSG_SHOW_SLIDESHOW_MENU = 27;

    private static final int HIDE_BARS_TIMEOUT = 3500;
    private static final int UNFREEZE_GLROOT_TIMEOUT = 250;

    private static final int REQUEST_SLIDESHOW = 1;
    private static final int REQUEST_CROP = 2;
    private static final int REQUEST_CROP_PICASA = 3;
    private static final int REQUEST_EDIT = 4;
    private static final int REQUEST_PLAY_VIDEO = 5;
    private static final int REQUEST_TRIM = 6;

    public static final String KEY_MEDIA_SET_PATH = "media-set-path";
    public static final String KEY_MEDIA_ITEM_PATH = "media-item-path";
    public static final String KEY_MEDIA_CONTENT_TYPE = "content-type";
    public static final String KEY_INDEX_HINT = "index-hint";
    public static final String KEY_OPEN_ANIMATION_RECT = "open-animation-rect";
    public static final String KEY_APP_BRIDGE = "app-bridge";
    public static final String KEY_TREAT_BACK_AS_UP = "treat-back-as-up";
    public static final String KEY_START_IN_FILMSTRIP = "start-in-filmstrip";
    public static final String KEY_RETURN_INDEX_HINT = "return-index-hint";
    public static final String KEY_SHOW_WHEN_LOCKED = "show_when_locked";
    public static final String KEY_IN_CAMERA_ROLL = "in_camera_roll";
    public static final String KEY_READONLY = "read-only";
    public static final String KEY_PLAYLIST = "playlist";
    public static final String KEY_PLAYLIST_ADDR = "PlayListAddress";
    public static final String KEY_PLAYLIST_TITLE = "playlistTitle";
    public static final String KEY_PLAYLIST_TITLE_ADDR = "PlayListTitleAddress";
    public static final String KEY_SOURCE_FROM = "SourceFrom";
    public static final String KEY_BROADCAST_STATUS = "android.intent.playback.broadcast.status";
    public static final String KEY_SHOW_OPTION = "ShowOption";
    private static final String RTK_PLAYBACK_STATUS = "rtk.provider.playback.STATUS";
    public static final String KEY_ALBUMPAGE_TRANSITION = "albumpage-transition";
    public static final int MSG_ALBUMPAGE_NONE = 0;
    public static final int MSG_ALBUMPAGE_STARTED = 1;
    public static final int MSG_ALBUMPAGE_RESUMED = 2;
    public static final int MSG_ALBUMPAGE_PICKED = 4;

    public static final String ACTION_NEXTGEN_EDIT = "action_nextgen_edit";
    public static final String ACTION_SIMPLE_EDIT = "action_simple_edit";

    public static final String KEY_SLIDESHOW_PREF = "Slideshow_Pref";
    public static final String KEY_SLIDESHOW_TIMING_PREF = "SlideshowTiming_Pref";
    public static final String KEY_SLIDESHOW_REPEAT_MODE_PREF = "SlideshowRepeatMode_Pref";
    public static final String KEY_SLIDESHOW_EFFECT_PREF = "SlideshowEffect_Pref";
    public static final String KEY_BG_MUSIC_PREF = "BG_Music_Pref";

    private GalleryApp mApplication;
    private SelectionManager mSelectionManager;

    private PhotoView mPhotoView;
    private PhotoPage.Model mModel;
    private DetailsHelper mDetailsHelper;
    private boolean mShowDetails;

    // mMediaSet could be null if there is no KEY_MEDIA_SET_PATH supplied.
    // E.g., viewing a photo in gmail attachment
    private FilterDeleteSet mMediaSet;

    // The mediaset used by camera launched from secure lock screen.
    private SecureAlbum mSecureAlbum;

    private int mCurrentIndex = 0;
    private Handler mHandler;
    private boolean mShowBars = true;
    private volatile boolean mActionBarAllowed = true;
    private GalleryActionBar mActionBar;
    private boolean mIsMenuVisible;
    private boolean mHaveImageEditor;
    private PhotoPageBottomControls mBottomControls;
    private PhotoPageProgress mProgressControl;
    private MediaItem mCurrentPhoto = null;
    private MenuExecutor mMenuExecutor;
    private boolean mIsActive;
    private boolean mShowSpinner;
    private String mSetPathString;
    // This is the original mSetPathString before adding the camera preview item.
    private boolean mReadOnlyView = false;
    private String mOriginalSetPathString;
    private AppBridge mAppBridge;
    private SnailItem mScreenNailItem;
    private SnailAlbum mScreenNailSet;
    private OrientationManager mOrientationManager;
    private boolean mTreatBackAsUp;
    private boolean mStartInFilmstrip;
    private boolean mHasCameraScreennailOrPlaceholder = false;
    private boolean mRecenterCameraOnResume = true;

    // These are only valid after the panorama callback
    private boolean mIsPanorama;
    private boolean mIsPanorama360;

    private long mCameraSwitchCutoff = 0;
    private boolean mSkipUpdateCurrentPhoto = false;
    private static final long CAMERA_SWITCH_CUTOFF_THRESHOLD_MS = 300;

    private static final long DEFERRED_UPDATE_MS = 250;
    private boolean mDeferredUpdateWaiting = false;
    private long mDeferUpdateUntil = Long.MAX_VALUE;

    // The item that is deleted (but it can still be undeleted before commiting)
    private Path mDeletePath;
    private boolean mDeleteIsFocus;  // whether the deleted item was in focus

    private Uri[] mNfcPushUris = new Uri[1];
    private String[] mPlayList = null;
    private String[] mPlayListTitle = null;
    private String mSourceFrom;
    private RTKOptionMenu  mOptionMenu = null;
    private RTKVScrollMenu mSlideshowMenu = null;
    private RTKVScrollMenu mSlideshowTimingMenu = null;
    private RTKVScrollMenu mSlideshowRepeatMenu = null;
    private RTKVScrollMenu mSlideshowEffectMenu = null;
    private RTKVScrollMenu mBgMusicMenu = null;
    private RTKVScrollMenu mEXIFMenu = null;
    private static final int REPEAT_MODE_OFF = 0;
    private static final int REPEAT_MODE_SHUFFLE = 1;
    private static final int REPEAT_MODE_ALL = 2;

    private boolean mSlideshow = false;
    private int mSlideshowTiming = 5;
    private int mSlideshowRepeatMode = REPEAT_MODE_OFF;
    private int mSlideshowEffect = SlideshowPage.DEFAULT_SLIDESHOW_EFFECT;
    private boolean mSlideshowBGMusic = false;

    private boolean mBroadcastStatus = false;
    private static final int ZOOM_IN_SCROLL_PIXEL = 50;
    private static int mUpdatePhotoInfoCount = 0;

    private final MyMenuVisibilityListener mMenuVisibilityListener =
            new MyMenuVisibilityListener();

    private int mLastSystemUiVis = 0;

    private final PanoramaSupportCallback mUpdatePanoramaMenuItemsCallback = new PanoramaSupportCallback() {
        @Override
        public void panoramaInfoAvailable(MediaObject mediaObject, boolean isPanorama,
                boolean isPanorama360) {
            if (mediaObject == mCurrentPhoto) {
                mHandler.obtainMessage(MSG_UPDATE_PANORAMA_UI, isPanorama360 ? 1 : 0, 0,
                        mediaObject).sendToTarget();
            }
        }
    };

    private final PanoramaSupportCallback mRefreshBottomControlsCallback = new PanoramaSupportCallback() {
        @Override
        public void panoramaInfoAvailable(MediaObject mediaObject, boolean isPanorama,
                boolean isPanorama360) {
            if (mediaObject == mCurrentPhoto) {
                mHandler.obtainMessage(MSG_REFRESH_BOTTOM_CONTROLS, isPanorama ? 1 : 0, isPanorama360 ? 1 : 0,
                        mediaObject).sendToTarget();
            }
        }
    };

    private final PanoramaSupportCallback mUpdateShareURICallback = new PanoramaSupportCallback() {
        @Override
        public void panoramaInfoAvailable(MediaObject mediaObject, boolean isPanorama,
                boolean isPanorama360) {
            if (mediaObject == mCurrentPhoto) {
                mHandler.obtainMessage(MSG_UPDATE_SHARE_URI, isPanorama360 ? 1 : 0, 0, mediaObject)
                        .sendToTarget();
            }
        }
    };

    public static interface Model extends PhotoView.Model {
        public void resume();
        public void pause();
        public boolean isEmpty();
        public void setCurrentPhoto(Path path, int indexHint);
        public void setMediaDataSet(AbstractGalleryActivity activity, String[] playlist);
    }

    private class MyMenuVisibilityListener implements OnMenuVisibilityListener {
        @Override
        public void onMenuVisibilityChanged(boolean isVisible) {
            mIsMenuVisible = isVisible;
            refreshHidingMessage();
        }
    }

    @Override
    protected int getBackgroundColorId() {
        return R.color.photo_background;
    }

    private final GLView mRootPane = new GLView() {
        @Override
        protected void onLayout(
                boolean changed, int left, int top, int right, int bottom) {
            mPhotoView.layout(0, 0, right - left, bottom - top);
            if (mShowDetails) {
                mDetailsHelper.layout(left, mActionBar.getHeight(), right, bottom);
            }
        }
    };

    @Override
    public void onCreate(Bundle data, Bundle restoreState) {
        Log.d(TAG, "onCreate");
        super.onCreate(data, restoreState);
        mActionBar = mActivity.getGalleryActionBar();
        mSelectionManager = new SelectionManager(mActivity, false);
        mMenuExecutor = new MenuExecutor(mActivity, mSelectionManager);

        mPhotoView = new PhotoView(mActivity);
        mPhotoView.setListener(this);
        mRootPane.addComponent(mPhotoView);
        mApplication = (GalleryApp) ((Activity) mActivity).getApplication();
        mOrientationManager = mActivity.getOrientationManager();
        mActivity.getGLRoot().setOrientationSource(mOrientationManager);

        mHandler = new SynchronizedHandler(mActivity.getGLRoot()) {
            @Override
            public void handleMessage(Message message) {
                switch (message.what) {
                    case MSG_HIDE_BARS: {
                        hideBars();
                        break;
                    }
                    case MSG_REFRESH_BOTTOM_CONTROLS: {
                        if (mCurrentPhoto == message.obj && mBottomControls != null) {
                            mIsPanorama = message.arg1 == 1;
                            mIsPanorama360 = message.arg2 == 1;
                            mBottomControls.refresh();
                        }
                        break;
                    }
                    case MSG_ON_FULL_SCREEN_CHANGED: {
                        if (mAppBridge != null) {
                            mAppBridge.onFullScreenChanged(message.arg1 == 1);
                        }
                        //// Broadcast playback status ////////////
                        if (true == mBroadcastStatus) {
                            boolean isFromDMR = mSourceFrom != null && mSourceFrom.startsWith("DMR");
                            if (isFromDMR) {
                               Log.d(TAG, "Broadcast PlayMode: Play");
                               Intent i = new Intent(RTK_PLAYBACK_STATUS)
                                   .putExtra("PlayMode", "Play");
                                   i.putExtra("PlayType", "Image");
                               if (null != mSourceFrom) {
                                   i.putExtra("SourceFrom", mSourceFrom);
                               }
                               mActivity.sendBroadcast(i);
                            }
                        }
                        ////////////////////////////////////////////////
                        break;
                    }
                    case MSG_UPDATE_ACTION_BAR: {
                        updateBars();
                        break;
                    }
                    case MSG_WANT_BARS: {
                        wantBars();
                        break;
                    }
                    case MSG_UNFREEZE_GLROOT: {
                        mActivity.getGLRoot().unfreeze();
                        break;
                    }
                    case MSG_UPDATE_DEFERRED: {
                        long nextUpdate = mDeferUpdateUntil - SystemClock.uptimeMillis();
                        if (nextUpdate <= 0) {
                            mDeferredUpdateWaiting = false;
                            updateUIForCurrentPhoto();
                        } else {
                            mHandler.sendEmptyMessageDelayed(MSG_UPDATE_DEFERRED, nextUpdate);
                        }
                        break;
                    }
                    case MSG_ON_CAMERA_CENTER: {
                        mSkipUpdateCurrentPhoto = false;
                        boolean stayedOnCamera = false;
                        if (!mPhotoView.getFilmMode()) {
                            stayedOnCamera = true;
                        } else if (SystemClock.uptimeMillis() < mCameraSwitchCutoff &&
                                mMediaSet.getMediaItemCount() > 1) {
                            mPhotoView.switchToImage(1);
                        } else {
                            if (mAppBridge != null) mPhotoView.setFilmMode(false);
                            stayedOnCamera = true;
                        }

                        if (stayedOnCamera) {
                            if (mAppBridge == null && mMediaSet.getTotalMediaItemCount() > 1) {
                                launchCamera();
                                /* We got here by swiping from photo 1 to the
                                   placeholder, so make it be the thing that
                                   is in focus when the user presses back from
                                   the camera app */
                                mPhotoView.switchToImage(1);
                            } else {
                                updateBars();
                                updateCurrentPhoto(mModel.getMediaItem(0));
                            }
                        }
                        break;
                    }
                    case MSG_ON_PICTURE_CENTER: {
                        if (!mPhotoView.getFilmMode() && mCurrentPhoto != null
                                && (mCurrentPhoto.getSupportedOperations() & MediaObject.SUPPORT_ACTION) != 0) {
                            mPhotoView.setFilmMode(true);
                        }
                        break;
                    }
                    case MSG_REFRESH_IMAGE: {
                        final MediaItem photo = mCurrentPhoto;
                        mCurrentPhoto = null;
                        updateCurrentPhoto(photo);
                        break;
                    }
                    case MSG_UPDATE_PHOTO_UI: {
                        updateUIForCurrentPhoto();
                        break;
                    }
                    case MSG_UPDATE_SHARE_URI: {
                        if (mCurrentPhoto == message.obj) {
                            boolean isPanorama360 = message.arg1 != 0;
                            Uri contentUri = mCurrentPhoto.getContentUri();
                            Intent panoramaIntent = null;
                            if (isPanorama360) {
                                panoramaIntent = createSharePanoramaIntent(contentUri);
                            }
                            Intent shareIntent = createShareIntent(mCurrentPhoto);

                            mActionBar.setShareIntents(panoramaIntent, shareIntent, PhotoPage.this);
                            setNfcBeamPushUri(contentUri);
                        }
                        break;
                    }
                    case MSG_UPDATE_PANORAMA_UI: {
                        if (mCurrentPhoto == message.obj) {
                            boolean isPanorama360 = message.arg1 != 0;
                            updatePanoramaUI(isPanorama360);
                        }
                        break;
                    }
                    case MSG_ENABLE_SLIDESHOW:
                    {
                        if(mProgressControl!=null && mProgressControl.slideshowEnable(true)==true) {
                            slideshowEnable();
                        }
                        break;
                    }
                    case MSG_DISABLE_SLIDESHOW:
                    {
                        if(mProgressControl!=null && mProgressControl.slideshowEnable(false)==true) {
                            if(mActivity.getStateManager().hasStateClass(SlideshowPage.class)) {
                                mActivity.getStateManager().onBackPressed();
                            }
                        }
                        break;
                    }
                    case MSG_SHOW_OPTIONMENU:
                    {
                        showOption();
                        break;
                    }
                    case MSG_STOP:
                    {
                        if(mActivity.getStateManager().hasStateClass(SlideshowPage.class)) {
                            if(mProgressControl != null) {
                                mProgressControl.show(false);
                                mProgressControl=null;
                            }
                            mActivity.getStateManager().finishState(mActivity.getStateManager().getTopState());
                        }
                        mActivity.getStateManager().onBackPressed();
                        break;
                    }
                    case MSG_SHOW_REPEATMENU:
                    {
                        if (mSlideshowRepeatMenu == null) {
                            NewRepeatMenu();
                        }
                        mSlideshowRepeatMenu.setBackgroundDrawable(new BitmapDrawable());
                        mSlideshowRepeatMenu.Show(true);
                        mSlideshowRepeatMenu.setSelectedItem(mSlideshowRepeatMode);
                        break;
                    }
                    case MSG_UPDATE_PHOTO_INFO:
                    {
                        if(mUpdatePhotoInfoCount++<=2)
                            updatePhotoInfoDisplay();
                        break;
                    }
                    case MSG_SHOW_TRANSITION_EFFECT_MENU:
                    {
                        if (mSlideshowEffectMenu == null) {
                            NewEffectMenu();
                        }
                        mSlideshowEffectMenu.setBackgroundDrawable(new BitmapDrawable());
                        mSlideshowEffectMenu.Show(true);
                        mSlideshowEffectMenu.setSelectedItem(mSlideshowEffect);
                        break;
                    }
                    case MSG_SHOW_SLIDESHOW_TIMING_MENU:
                    {
                        if (mSlideshowTimingMenu == null) {
                            NewSlideshowTimingMenu();
                        }
                        mSlideshowTimingMenu.setBackgroundDrawable(new BitmapDrawable());
                        mSlideshowTimingMenu.Show(true);
                        switch(mSlideshowTiming)
                        {
                            case 2:
                                mSlideshowTimingMenu.setSelectedItem(0);
                                break;
                            case 5:
                                mSlideshowTimingMenu.setSelectedItem(1);
                                break;
                            case 10:
                                mSlideshowTimingMenu.setSelectedItem(2);
                                break;
                            case 30:
                                mSlideshowTimingMenu.setSelectedItem(3);
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                    case MSG_SHOW_BG_NUSIC_MENU:
                    {
                        if (mBgMusicMenu == null) {
                            NewBgMusicMenu();
                        }
                        mBgMusicMenu.setBackgroundDrawable(new BitmapDrawable());
                        mBgMusicMenu.Show(true);
                        // Display BG Music setting
                        if(mSlideshowBGMusic)
                            mBgMusicMenu.setSelectedItem(1);
                        else
                            mBgMusicMenu.setSelectedItem(0);
                        break;
                    }
                    case MSG_SHOW_EXIF_MENU:
                    {
                        if (mEXIFMenu == null) {
                            NewEXIFMenu();
                        }
                        else {
                            getExifList();
                        }
                        mEXIFMenu.setBackgroundDrawable(new BitmapDrawable());
                        mEXIFMenu.Show(true);
                        break;
                    }
                    case MSG_SHOW_SLIDESHOW_MENU:
                    {
                        if (mSlideshowMenu == null) {
                            NewSlideshowMenu();
                        }
                        mSlideshowMenu.setBackgroundDrawable(new BitmapDrawable());
                        mSlideshowMenu.Show(true);
                        // Display slideshow setting
                        if(mSlideshow)
                            mSlideshowMenu.setSelectedItem(1);
                        else
                            mSlideshowMenu.setSelectedItem(0);
                        break;
                    }
                    default: throw new AssertionError(message.what);
                }
            }
        };
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(mActivity.getAndroidContext());
        mSlideshow = prefs.getBoolean(KEY_SLIDESHOW_PREF, mSlideshow);
        mSlideshowEffect = prefs.getInt(KEY_SLIDESHOW_EFFECT_PREF, mSlideshowEffect);
        mSlideshowTiming = prefs.getInt(KEY_SLIDESHOW_TIMING_PREF, mSlideshowTiming);
        mSlideshowRepeatMode = prefs.getInt(KEY_SLIDESHOW_REPEAT_MODE_PREF, mSlideshowRepeatMode);
        mSlideshowBGMusic = prefs.getBoolean(KEY_BG_MUSIC_PREF, mSlideshowBGMusic);

        mBroadcastStatus = data.getBoolean(KEY_BROADCAST_STATUS, false);
        mSourceFrom = data.getString(SlideshowPage.KEY_SOURCE_FROM);
        mSetPathString = data.getString(KEY_MEDIA_SET_PATH);
        mReadOnlyView = data.getBoolean(KEY_READONLY);
        mOriginalSetPathString = mSetPathString;
        setupNfcBeamPush();
        String itemPathString = data.getString(KEY_MEDIA_ITEM_PATH);
        Path itemPath = itemPathString != null ?
                Path.fromString(data.getString(KEY_MEDIA_ITEM_PATH)) :
                    null;
        mTreatBackAsUp = data.getBoolean(KEY_TREAT_BACK_AS_UP, false);
        mStartInFilmstrip = data.getBoolean(KEY_START_IN_FILMSTRIP, false);
        mPlayListTitle= data.getStringArray(KEY_PLAYLIST_TITLE);
        boolean inCameraRoll = data.getBoolean(KEY_IN_CAMERA_ROLL, false);
        mCurrentIndex = data.getInt(KEY_INDEX_HINT, 0);
        if (mSetPathString != null) {
            mShowSpinner = true;
            mAppBridge = (AppBridge) data.getParcelable(KEY_APP_BRIDGE);
            if (mAppBridge != null) {
                mShowBars = false;
                mHasCameraScreennailOrPlaceholder = true;
                mAppBridge.setServer(this);

                // Get the ScreenNail from AppBridge and register it.
                int id = SnailSource.newId();
                Path screenNailSetPath = SnailSource.getSetPath(id);
                Path screenNailItemPath = SnailSource.getItemPath(id);
                mScreenNailSet = (SnailAlbum) mActivity.getDataManager()
                        .getMediaObject(screenNailSetPath);
                mScreenNailItem = (SnailItem) mActivity.getDataManager()
                        .getMediaObject(screenNailItemPath);
                mScreenNailItem.setScreenNail(mAppBridge.attachScreenNail());

                if (data.getBoolean(KEY_SHOW_WHEN_LOCKED, false)) {
                    // Set the flag to be on top of the lock screen.
                    mFlags |= FLAG_SHOW_WHEN_LOCKED;
                }

                // Don't display "empty album" action item for capture intents.
                if (!mSetPathString.equals("/local/all/0")) {
                    // Check if the path is a secure album.
                    if (SecureSource.isSecurePath(mSetPathString)) {
                        mSecureAlbum = (SecureAlbum) mActivity.getDataManager()
                                .getMediaSet(mSetPathString);
                        mShowSpinner = false;
                    }
                    mSetPathString = "/filter/empty/{"+mSetPathString+"}";
                }

                // Combine the original MediaSet with the one for ScreenNail
                // from AppBridge.
                mSetPathString = "/combo/item/{" + screenNailSetPath +
                        "," + mSetPathString + "}";

                // Start from the screen nail.
                itemPath = screenNailItemPath;
            } else if (inCameraRoll && GalleryUtils.isCameraAvailable(mActivity)) {
                mSetPathString = "/combo/item/{" + FilterSource.FILTER_CAMERA_SHORTCUT +
                        "," + mSetPathString + "}";
                mCurrentIndex++;
                mHasCameraScreennailOrPlaceholder = true;
            }

            MediaSet originalSet = mActivity.getDataManager()
                    .getMediaSet(mSetPathString);
            if (mHasCameraScreennailOrPlaceholder && originalSet instanceof ComboAlbum) {
                // Use the name of the camera album rather than the default
                // ComboAlbum behavior
                ((ComboAlbum) originalSet).useNameOfChild(1);
            }
            mSelectionManager.setSourceMediaSet(originalSet);
            mSetPathString = "/filter/delete/{" + mSetPathString + "}";
            mMediaSet = (FilterDeleteSet) mActivity.getDataManager()
                    .getMediaSet(mSetPathString);
            if (mMediaSet == null) {
                Log.w(TAG, "failed to restore " + mSetPathString);
            }

            PhotoDataAdapter pda;
            mPlayList = data.getStringArray(KEY_PLAYLIST);
            if(mPlayList==null)
            {
                if (itemPath == null) {
                    int mediaItemCount = mMediaSet.getMediaItemCount();
                    if (mediaItemCount > 0) {
                        if (mCurrentIndex >= mediaItemCount) mCurrentIndex = 0;
                        itemPath = mMediaSet.getMediaItem(mCurrentIndex, 1)
                            .get(0).getPath();
                    } else {
                        // Bail out, PhotoPage can't load on an empty album
                        return;
                    }
                }

                pda = new PhotoDataAdapter(
                        mActivity, mPhotoView, mMediaSet, itemPath, mCurrentIndex,
                        mAppBridge == null ? -1 : 0,
                        mAppBridge == null ? false : mAppBridge.isPanorama(),
                        mAppBridge == null ? false : mAppBridge.isStaticCamera());
                mModel = pda;
            }
            else
            {
                pda = new PhotoDataAdapter(
                        mActivity, mPhotoView, mPlayList, itemPath, mCurrentIndex,
                        mAppBridge == null ? -1 : 0,
                        mAppBridge == null ? false : mAppBridge.isPanorama(),
                        mAppBridge == null ? false : mAppBridge.isStaticCamera());
                mModel = pda;
            }
            mPhotoView.setModel(mModel);
            updateProgress(mPlayList);
            pda.setDataListener(new PhotoDataAdapter.DataListener() {

                @Override
                public void onPhotoChanged(int index, Path item) {
                    int oldIndex = mCurrentIndex;
                    mCurrentIndex = index;

                    if (mHasCameraScreennailOrPlaceholder) {
                        if (mCurrentIndex > 0) {
                            mSkipUpdateCurrentPhoto = false;
                        }

                        if (oldIndex == 0 && mCurrentIndex > 0
                                && !mPhotoView.getFilmMode()) {
                            mPhotoView.setFilmMode(true);
                            if (mAppBridge != null) {
                                UsageStatistics.onEvent("CameraToFilmstrip",
                                        UsageStatistics.TRANSITION_SWIPE, null);
                            }
                        } else if (oldIndex == 2 && mCurrentIndex == 1) {
                            mCameraSwitchCutoff = SystemClock.uptimeMillis() +
                                    CAMERA_SWITCH_CUTOFF_THRESHOLD_MS;
                            mPhotoView.stopScrolling();
                        } else if (oldIndex >= 1 && mCurrentIndex == 0) {
                            mPhotoView.setWantPictureCenterCallbacks(true);
                            mSkipUpdateCurrentPhoto = true;
                        }
                    }
                    if (!mSkipUpdateCurrentPhoto) {
                        if (item != null) {
                            MediaItem photo = mModel.getMediaItem(0);
                            if (photo != null) updateCurrentPhoto(photo);
                        }
                        updateBars();
                        mUpdatePhotoInfoCount=0;
                        updatePhotoInfoDisplay();
                    }
                    // Reset the timeout for the bars after a swipe
                    refreshHidingMessage();
                }

                @Override
                public void onLoadingFinished(boolean loadingFailed) {
                    if (!mModel.isEmpty()) {
                        MediaItem photo = mModel.getMediaItem(0);
                        if (photo != null) updateCurrentPhoto(photo);
                    } else if (mIsActive) {
                        // We only want to finish the PhotoPage if there is no
                        // deletion that the user can undo.
                        if (mMediaSet.getNumberOfDeletions() == 0) {
                            mActivity.getStateManager().finishState(
                                    PhotoPage.this);
                        }
                    }
                }

                @Override
                public void onLoadingStarted() {
                }
            });
        } else {
            // Get default media set by the URI
            Uri itemUri = mActivity.getDataManager().getContentUri(itemPath);
            String contentType = data.getString(KEY_MEDIA_CONTENT_TYPE);
            String itemPathStr = (contentType==null)?GalleryUtils.getItemPathString(mApplication, itemUri.toString())
                                            :GalleryUtils.getItemPathString(mApplication, itemUri.toString(), contentType);
            MediaItem mediaItem = (MediaItem)
                    mActivity.getDataManager().getMediaObject(itemPathStr);
            mModel = new SinglePhotoDataAdapter(mActivity, mPhotoView, mediaItem);
            mPhotoView.setModel(mModel);
            updateCurrentPhoto(mediaItem);
            updateProgress(null);
            mUpdatePhotoInfoCount=0;
            updatePhotoInfoDisplay();
            mShowSpinner = false;
            String[] playList = {mediaItem.getFilePath()};
            updateProgress(playList);
        }
        mFlags |= FLAG_SCREEN_ON_ALWAYS;

        mPhotoView.setFilmMode(mStartInFilmstrip && mMediaSet.getMediaItemCount() > 1);
        RelativeLayout galleryRoot = (RelativeLayout) ((Activity) mActivity)
                .findViewById(mAppBridge != null ? R.id.content : R.id.gallery_root);
        if (galleryRoot != null) {
            if (mSecureAlbum == null) {
                mBottomControls = new PhotoPageBottomControls(this, mActivity, galleryRoot);
            }
        }

        ((GLRootView) mActivity.getGLRoot()).setOnSystemUiVisibilityChangeListener(
                new View.OnSystemUiVisibilityChangeListener() {
                @Override
                    public void onSystemUiVisibilityChange(int visibility) {
                        int diff = mLastSystemUiVis ^ visibility;
                        mLastSystemUiVis = visibility;
                        if ((diff & View.SYSTEM_UI_FLAG_FULLSCREEN) != 0
                                && (visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
                            showBars();
                        }
                    }
                });

        if(data.getString(KEY_SHOW_OPTION)!=null) {
            if(data.getString(KEY_SHOW_OPTION).equals("OptionMenu")) {
                mHandler.obtainMessage(MSG_SHOW_OPTIONMENU, 0, 0, null).sendToTarget();
            }
            else if(data.getString(KEY_SHOW_OPTION).equals("RepeatMenu")){
                mHandler.obtainMessage(MSG_SHOW_REPEATMENU, 0, 0, null).sendToTarget();
            }
        }
    }

    @Override
    public void onPictureCenter(boolean isCamera) {
        isCamera = isCamera || (mHasCameraScreennailOrPlaceholder && mAppBridge == null);
        mPhotoView.setWantPictureCenterCallbacks(false);
        mHandler.removeMessages(MSG_ON_CAMERA_CENTER);
        mHandler.removeMessages(MSG_ON_PICTURE_CENTER);
        mHandler.sendEmptyMessage(isCamera ? MSG_ON_CAMERA_CENTER : MSG_ON_PICTURE_CENTER);
    }

    @Override
    public boolean canDisplayBottomControls() {
        return mIsActive && !mPhotoView.canUndo();
    }

    @Override
    public boolean canDisplayBottomControl(int control) {
        if (mCurrentPhoto == null) {
            return false;
        }
        switch(control) {
            case R.id.photopage_bottom_control_edit:
                return mHaveImageEditor && mShowBars && !mReadOnlyView
                        && !mPhotoView.getFilmMode()
                        && (mCurrentPhoto.getSupportedOperations() & MediaItem.SUPPORT_EDIT) != 0
                        && mCurrentPhoto.getMediaType() == MediaObject.MEDIA_TYPE_IMAGE;
            case R.id.photopage_bottom_control_panorama:
                return mIsPanorama;
            case R.id.photopage_bottom_control_tiny_planet:
                return mHaveImageEditor && mShowBars
                        && mIsPanorama360 && !mPhotoView.getFilmMode();
            default:
                return false;
        }
    }

    @Override
    public void onBottomControlClicked(int control) {
        switch(control) {
            case R.id.photopage_bottom_control_edit:
                launchPhotoEditor();
                return;
            case R.id.photopage_bottom_control_panorama:
                mActivity.getPanoramaViewHelper()
                        .showPanorama(mCurrentPhoto.getContentUri());
                return;
            case R.id.photopage_bottom_control_tiny_planet:
                launchTinyPlanet();
                return;
            default:
                return;
        }
    }

    @TargetApi(ApiHelper.VERSION_CODES.JELLY_BEAN)
    private void setupNfcBeamPush() {
        if (!ApiHelper.HAS_SET_BEAM_PUSH_URIS) return;

        NfcAdapter adapter = NfcAdapter.getDefaultAdapter(mActivity);
        if (adapter != null) {
            adapter.setBeamPushUris(null, mActivity);
            adapter.setBeamPushUrisCallback(new CreateBeamUrisCallback() {
                @Override
                public Uri[] createBeamUris(NfcEvent event) {
                    return mNfcPushUris;
                }
            }, mActivity);
        }
    }

    private void setNfcBeamPushUri(Uri uri) {
        mNfcPushUris[0] = uri;
    }

    private static Intent createShareIntent(MediaObject mediaObject) {
        int type = mediaObject.getMediaType();
        return new Intent(Intent.ACTION_SEND)
                .setType(MenuExecutor.getMimeType(type))
                .putExtra(Intent.EXTRA_STREAM, mediaObject.getContentUri())
                .addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
    }

    private static Intent createSharePanoramaIntent(Uri contentUri) {
        return new Intent(Intent.ACTION_SEND)
                .setType(GalleryUtils.MIME_TYPE_PANORAMA360)
                .putExtra(Intent.EXTRA_STREAM, contentUri)
                .addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
    }

    private void overrideTransitionToEditor() {
        ((Activity) mActivity).overridePendingTransition(android.R.anim.fade_in,
                android.R.anim.fade_out);
    }

    private void launchTinyPlanet() {
        // Deep link into tiny planet
        MediaItem current = mModel.getMediaItem(0);
        Intent intent = new Intent(FilterShowActivity.TINY_PLANET_ACTION);
        intent.setClass(mActivity, FilterShowActivity.class);
        intent.setDataAndType(current.getContentUri(), current.getMimeType())
            .setFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
        intent.putExtra(FilterShowActivity.LAUNCH_FULLSCREEN,
                mActivity.isFullscreen());
        mActivity.startActivityForResult(intent, REQUEST_EDIT);
        overrideTransitionToEditor();
    }

    private void launchCamera() {
        mRecenterCameraOnResume = false;
        GalleryUtils.startCameraActivity(mActivity);
    }

    private void launchPhotoEditor() {
        MediaItem current = mModel.getMediaItem(0);
        if (current == null || (current.getSupportedOperations()
                & MediaObject.SUPPORT_EDIT) == 0) {
            return;
        }

        Intent intent = new Intent(ACTION_NEXTGEN_EDIT);

        intent.setDataAndType(current.getContentUri(), current.getMimeType())
                .setFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
        if (mActivity.getPackageManager()
                .queryIntentActivities(intent, PackageManager.MATCH_DEFAULT_ONLY).size() == 0) {
            intent.setAction(Intent.ACTION_EDIT);
        }
        intent.putExtra(FilterShowActivity.LAUNCH_FULLSCREEN,
                mActivity.isFullscreen());
        ((Activity) mActivity).startActivityForResult(Intent.createChooser(intent, null),
                REQUEST_EDIT);
        overrideTransitionToEditor();
    }

    private void launchSimpleEditor() {
        MediaItem current = mModel.getMediaItem(0);
        if (current == null || (current.getSupportedOperations()
                & MediaObject.SUPPORT_EDIT) == 0) {
            return;
        }

        Intent intent = new Intent(ACTION_SIMPLE_EDIT);

        intent.setDataAndType(current.getContentUri(), current.getMimeType())
                .setFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
        if (mActivity.getPackageManager()
                .queryIntentActivities(intent, PackageManager.MATCH_DEFAULT_ONLY).size() == 0) {
            intent.setAction(Intent.ACTION_EDIT);
        }
        intent.putExtra(FilterShowActivity.LAUNCH_FULLSCREEN,
                mActivity.isFullscreen());
        ((Activity) mActivity).startActivityForResult(Intent.createChooser(intent, null),
                REQUEST_EDIT);
        overrideTransitionToEditor();
    }

    private void requestDeferredUpdate() {
        mDeferUpdateUntil = SystemClock.uptimeMillis() + DEFERRED_UPDATE_MS;
        if (!mDeferredUpdateWaiting) {
            mDeferredUpdateWaiting = true;
            mHandler.sendEmptyMessageDelayed(MSG_UPDATE_DEFERRED, DEFERRED_UPDATE_MS);
        }
    }

    private void updateUIForCurrentPhoto() {
        if (mCurrentPhoto == null) return;

        // If by swiping or deletion the user ends up on an action item
        // and zoomed in, zoom out so that the context of the action is
        // more clear
        if ((mCurrentPhoto.getSupportedOperations() & MediaObject.SUPPORT_ACTION) != 0
                && !mPhotoView.getFilmMode()) {
            mPhotoView.setWantPictureCenterCallbacks(true);
        }

        updateMenuOperations();
        refreshBottomControlsWhenReady();
        if (mShowDetails) {
            mDetailsHelper.reloadDetails();
        }
        if ((mSecureAlbum == null)
                && (mCurrentPhoto.getSupportedOperations() & MediaItem.SUPPORT_SHARE) != 0) {
            mCurrentPhoto.getPanoramaSupport(mUpdateShareURICallback);
        }
    }

    private void updatePhotoInfoDisplay() {
        if(mProgressControl != null)
        {
            if(mCurrentPhoto != null)
            {
                String photo_name;
                String dimension;
                String count;
                if(mPlayListTitle!=null && mCurrentIndex >=0 && mCurrentIndex < mPlayListTitle.length)
                    photo_name = mPlayListTitle[mCurrentIndex];
                else
                    photo_name = mCurrentPhoto.getFilePath().substring(mCurrentPhoto.getFilePath().lastIndexOf("/")+1);

                if(mCurrentPhoto.getFilePath().toLowerCase().endsWith("heic") || mCurrentPhoto.getFilePath().toLowerCase().endsWith("heic")) {
                    HeifReader heifReader = new HeifReader(mActivity.getAndroidContext(), mCurrentPhoto.getFilePath(), null);
                    dimension = "Dimension "+heifReader.getWidth()+"*"+heifReader.getHeight();
                }
                else {
                    if(mCurrentPhoto.getWidth()>0 && mCurrentPhoto.getHeight()>0)
                    {
                        dimension = "Dimension "+mCurrentPhoto.getWidth()+"*"+mCurrentPhoto.getHeight();
                    }
                    else
                    {
                        dimension = "Unknown";
                        if(mCurrentPhoto.getWidth()==0 && mCurrentPhoto.getHeight()==0) {
                            Message msg = mHandler.obtainMessage(MSG_UPDATE_PHOTO_INFO, 0, 0, null);
                            mHandler.sendMessageDelayed(msg, 1000);
                        }
                    }
                }
                mProgressControl.setPhotoInfo(photo_name, dimension, mCurrentIndex+1);
//                GalleryUtils.updatePhotoNotification(photo_name, mActivity);
            }
            mProgressControl.setPhotoListSelection(mCurrentIndex);
            mProgressControl.showPhotoList(true);
        }
    }

    private void updateProgress(String[] photolist) {

        RelativeLayout galleryRoot = (RelativeLayout) ((Activity) mActivity)
            .findViewById(mAppBridge != null ? R.id.content : R.id.gallery_root);
        if(galleryRoot != null && mProgressControl == null)
        {
            mProgressControl = new PhotoPageProgress(mActivity, galleryRoot, mModel);
        }

        if(mProgressControl != null)
        {
            mProgressControl.setClickListener(new PhotoProgressClickListener());
            mProgressControl.setKeyListener(new PhotoProgressKeyListener());

            if(photolist!=null)
            {
                mProgressControl.setPhotoList(photolist);
            }
            else if(mMediaSet!=null)
            {
                int mediaItemCount = mMediaSet.getMediaItemCount();
                ArrayList<String> allMediaPathList = new ArrayList<String>();
                if (mediaItemCount > 0)
                {
                    ArrayList<MediaItem> allMedia = mMediaSet.getMediaItem(0, mediaItemCount);
                    for(int count=0;count<allMedia.size() ;count++)
                    {
                        String fileName = allMedia.get(count).getFilePath();
                        allMediaPathList.add( fileName );
                    }
                }
                else
                {
                    return;
                }
                String[] photoList = allMediaPathList.toArray(new String[allMediaPathList.size()]);
                mProgressControl.setPhotoList(photoList);
            }
        }
    }

    private void updateCurrentPhoto(MediaItem photo) {
        if (mCurrentPhoto == photo) return;

        UpdatePhotoRecentPlay(mActivity, photo.getFilePath());

        mCurrentPhoto = photo;
        if (mPhotoView.getFilmMode()) {
            requestDeferredUpdate();
        } else {
            updateUIForCurrentPhoto();
        }
    }

    public String getCurrentPhotoPath() {
        return (mCurrentPhoto==null)?null:mCurrentPhoto.getFilePath();
    }

    private void updateMenuOperations() {
        Menu menu = mActionBar.getMenu();

        // it could be null if onCreateActionBar has not been called yet
        if (menu == null) return;

        MenuItem item = menu.findItem(R.id.action_slideshow);
        if (item != null) {
            item.setVisible((mSecureAlbum == null) && canDoSlideShow());
        }
        if (mCurrentPhoto == null) return;

        int supportedOperations = mCurrentPhoto.getSupportedOperations();
        if (mReadOnlyView) {
            supportedOperations ^= MediaObject.SUPPORT_EDIT;
        }
        if (mSecureAlbum != null) {
            supportedOperations &= MediaObject.SUPPORT_DELETE;
        } else {
            mCurrentPhoto.getPanoramaSupport(mUpdatePanoramaMenuItemsCallback);
            if (!mHaveImageEditor) {
                supportedOperations &= ~MediaObject.SUPPORT_EDIT;
            }
        }
        MenuExecutor.updateMenuOperation(menu, supportedOperations);
    }

    private boolean canDoSlideShow() {
        if (mMediaSet == null || mCurrentPhoto == null) {
            return false;
        }
        if (mCurrentPhoto.getMediaType() != MediaObject.MEDIA_TYPE_IMAGE) {
            return false;
        }
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //  Action Bar show/hide management
    //////////////////////////////////////////////////////////////////////////

    private void showBars() {
        if (true/*mShowBars*/) return;
        mShowBars = true;
        mOrientationManager.unlockOrientation();
        mActionBar.show();
        mActivity.getGLRoot().setLightsOutMode(false);
        refreshHidingMessage();
        refreshBottomControlsWhenReady();
    }

    private void hideBars() {
        if (!mShowBars) return;
        mShowBars = false;
        mActionBar.hide();
        mActivity.getGLRoot().setLightsOutMode(true);
        mHandler.removeMessages(MSG_HIDE_BARS);
        refreshBottomControlsWhenReady();
    }

    private void refreshHidingMessage() {
        mHandler.removeMessages(MSG_HIDE_BARS);
        if (!mIsMenuVisible && !mPhotoView.getFilmMode()) {
            mHandler.sendEmptyMessageDelayed(MSG_HIDE_BARS, HIDE_BARS_TIMEOUT);
        }
    }

    private boolean canShowBars() {
        // No bars if we are showing camera preview.
        if (mAppBridge != null && mCurrentIndex == 0
                && !mPhotoView.getFilmMode()) return false;

        // No bars if it's not allowed.
        if (!mActionBarAllowed) return false;

        Configuration config = mActivity.getResources().getConfiguration();
        if (config.touchscreen == Configuration.TOUCHSCREEN_NOTOUCH) {
            return false;
        }

        return true;
    }

    private void wantBars() {
        if (canShowBars()) showBars();
    }

    private void toggleBars() {
        if (mShowBars) {
            hideBars();
        } else {
            if (canShowBars()) showBars();
        }
    }

    private void updateBars() {
        if (!canShowBars()) {
            hideBars();
        }
    }

    @Override
    protected void onBackPressed() {
        showBars();
        if (mShowDetails) {
            hideDetails();
        } else if (mAppBridge == null || !switchWithCaptureAnimation(-1)) {
            // We are leaving this page. Set the result now.
            setResult();
            if (mStartInFilmstrip && !mPhotoView.getFilmMode()) {
                mPhotoView.setFilmMode(true);
            } else if (mTreatBackAsUp) {
                onUpPressed();
            } else {
                super.onBackPressed();
            }
        }
    }

    private void onUpPressed() {
        if ((mStartInFilmstrip || mAppBridge != null)
                && !mPhotoView.getFilmMode()) {
            mPhotoView.setFilmMode(true);
            return;
        }

        if (mActivity.getStateManager().getStateCount() > 1) {
            setResult();
            super.onBackPressed();
            return;
        }

        if (mOriginalSetPathString == null) return;

        if (mAppBridge == null) {
            // We're in view mode so set up the stacks on our own.
            Bundle data = new Bundle(getData());
            data.putString(AlbumPage.KEY_MEDIA_PATH, mOriginalSetPathString);
            data.putString(AlbumPage.KEY_PARENT_MEDIA_PATH,
                    mActivity.getDataManager().getTopSetPath(
                            DataManager.INCLUDE_ALL));
            mActivity.getStateManager().switchState(this, AlbumPage.class, data);
        } else {
            GalleryUtils.startGalleryActivity(mActivity);
        }
    }

    private void setResult() {
        Intent result = null;
        result = new Intent();
        result.putExtra(KEY_RETURN_INDEX_HINT, mCurrentIndex);
        setStateResult(Activity.RESULT_OK, result);
    }

    //////////////////////////////////////////////////////////////////////////
    //  AppBridge.Server interface
    //////////////////////////////////////////////////////////////////////////

    @Override
    public void setCameraRelativeFrame(Rect frame) {
        mPhotoView.setCameraRelativeFrame(frame);
    }

    @Override
    public boolean switchWithCaptureAnimation(int offset) {
        return mPhotoView.switchWithCaptureAnimation(offset);
    }

    @Override
    public void setSwipingEnabled(boolean enabled) {
        mPhotoView.setSwipingEnabled(enabled);
    }

    @Override
    public void notifyScreenNailChanged() {
        mScreenNailItem.setScreenNail(mAppBridge.attachScreenNail());
        mScreenNailSet.notifyChange();
    }

    @Override
    public void addSecureAlbumItem(boolean isVideo, int id) {
        mSecureAlbum.addMediaItem(isVideo, id);
    }

    @Override
    protected boolean onCreateActionBar(Menu menu) {
        mActionBar.createActionBarMenu(R.menu.photo, menu);
        mHaveImageEditor = GalleryUtils.isEditorAvailable(mActivity, "image/*");
        updateMenuOperations();
        mActionBar.setTitle(mMediaSet != null ? mMediaSet.getName() : "");
        return true;
    }

    private MenuExecutor.ProgressListener mConfirmDialogListener =
            new MenuExecutor.ProgressListener() {
        @Override
        public void onProgressUpdate(int index) {}

        @Override
        public void onProgressComplete(int result) {}

        @Override
        public void onConfirmDialogShown() {
            mHandler.removeMessages(MSG_HIDE_BARS);
        }

        @Override
        public void onConfirmDialogDismissed(boolean confirmed) {
            refreshHidingMessage();
        }

        @Override
        public void onProgressStart() {}
    };

    private void switchToGrid() {
        if (mActivity.getStateManager().hasStateClass(AlbumPage.class)) {
            onUpPressed();
        } else {
            if (mOriginalSetPathString == null) return;
            Bundle data = new Bundle(getData());
            data.putString(AlbumPage.KEY_MEDIA_PATH, mOriginalSetPathString);
            data.putString(AlbumPage.KEY_PARENT_MEDIA_PATH,
                    mActivity.getDataManager().getTopSetPath(
                            DataManager.INCLUDE_ALL));

            // We only show cluster menu in the first AlbumPage in stack
            // TODO: Enable this when running from the camera app
            boolean inAlbum = mActivity.getStateManager().hasStateClass(AlbumPage.class);
            data.putBoolean(AlbumPage.KEY_SHOW_CLUSTER_MENU, !inAlbum
                    && mAppBridge == null);

            data.putBoolean(PhotoPage.KEY_APP_BRIDGE, mAppBridge != null);

            // Account for live preview being first item
            mActivity.getTransitionStore().put(KEY_RETURN_INDEX_HINT,
                    mAppBridge != null ? mCurrentIndex - 1 : mCurrentIndex);

            if (mHasCameraScreennailOrPlaceholder && mAppBridge != null) {
                mActivity.getStateManager().startState(AlbumPage.class, data);
            } else {
                mActivity.getStateManager().switchState(this, AlbumPage.class, data);
            }
        }
    }

    @Override
    protected boolean onItemSelected(MenuItem item) {
        if (mModel == null) return true;
        refreshHidingMessage();
        MediaItem current = mModel.getMediaItem(0);

        // This is a shield for monkey when it clicks the action bar
        // menu when transitioning from filmstrip to camera
        if (current instanceof SnailItem) return true;
        // TODO: We should check the current photo against the MediaItem
        // that the menu was initially created for. We need to fix this
        // after PhotoPage being refactored.
        if (current == null) {
            // item is not ready, ignore
            return true;
        }
        int currentIndex = mModel.getCurrentIndex();
        Path path = current.getPath();

        DataManager manager = mActivity.getDataManager();
        int action = item.getItemId();
        String confirmMsg = null;
        switch (action) {
            case android.R.id.home: {
                onUpPressed();
                return true;
            }
            case R.id.action_slideshow: {
                Bundle data = new Bundle();
                data.putString(SlideshowPage.KEY_SET_PATH, mMediaSet.getPath().toString());
                data.putString(SlideshowPage.KEY_ITEM_PATH, path.toString());
                data.putInt(SlideshowPage.KEY_PHOTO_INDEX, currentIndex);
                data.putBoolean(SlideshowPage.KEY_REPEAT, true);
                mActivity.getStateManager().startStateForResult(
                        SlideshowPage.class, REQUEST_SLIDESHOW, data);
                return true;
            }
            case R.id.action_crop: {
                Activity activity = mActivity;
                Intent intent = new Intent(CropActivity.CROP_ACTION);
                intent.setClass(activity, CropActivity.class);
                intent.setDataAndType(manager.getContentUri(path), current.getMimeType())
                    .setFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
                activity.startActivityForResult(intent, PicasaSource.isPicasaImage(current)
                        ? REQUEST_CROP_PICASA
                        : REQUEST_CROP);
                return true;
            }
            case R.id.action_trim: {
                Intent intent = new Intent(mActivity, TrimVideo.class);
                intent.setData(manager.getContentUri(path));
                // We need the file path to wrap this into a RandomAccessFile.
                intent.putExtra(KEY_MEDIA_ITEM_PATH, current.getFilePath());
                mActivity.startActivityForResult(intent, REQUEST_TRIM);
                return true;
            }
            case R.id.action_mute: {
                MuteVideo muteVideo = new MuteVideo(current.getFilePath(),
                        manager.getContentUri(path), mActivity);
                muteVideo.muteInBackground();
                return true;
            }
            case R.id.action_edit: {
                launchPhotoEditor();
                return true;
            }
            case R.id.action_simple_edit: {
                launchSimpleEditor();
                return true;
            }
            case R.id.action_details: {
                if (mShowDetails) {
                    hideDetails();
                } else {
                    showDetails();
                }
                return true;
            }
            case R.id.print: {
                mActivity.printSelectedImage(manager.getContentUri(path));
                return true;
            }
            case R.id.action_delete:
                confirmMsg = mActivity.getResources().getQuantityString(
                        R.plurals.delete_selection, 1);
            case R.id.action_setas:
            case R.id.action_rotate_ccw:
            case R.id.action_rotate_cw:
            case R.id.action_show_on_map:
                mSelectionManager.deSelectAll();
                mSelectionManager.toggle(path);
                mMenuExecutor.onMenuClicked(item, confirmMsg, mConfirmDialogListener);
                return true;
            default :
                return false;
        }
    }

    private void hideDetails() {
        mShowDetails = false;
        mDetailsHelper.hide();
    }

    private void showDetails() {
        mShowDetails = true;
        if (mDetailsHelper == null) {
            mDetailsHelper = new DetailsHelper(mActivity, mRootPane, new MyDetailsSource());
            mDetailsHelper.setCloseListener(new CloseListener() {
                @Override
                public void onClose() {
                    hideDetails();
                }
            });
        }
        mDetailsHelper.show();
    }

    ////////////////////////////////////////////////////////////////////////////
    //  Callbacks from PhotoView
    ////////////////////////////////////////////////////////////////////////////
    @Override
    public void onSingleTapUp(int x, int y) {
        if (mAppBridge != null) {
            if (mAppBridge.onSingleTapUp(x, y)) return;
        }

        MediaItem item = mModel.getMediaItem(0);
        if (item == null || item == mScreenNailItem) {
            // item is not ready or it is camera preview, ignore
            return;
        }

        int supported = item.getSupportedOperations();
        boolean playVideo = ((supported & MediaItem.SUPPORT_PLAY) != 0);
        boolean unlock = ((supported & MediaItem.SUPPORT_UNLOCK) != 0);
        boolean goBack = ((supported & MediaItem.SUPPORT_BACK) != 0);
        boolean launchCamera = ((supported & MediaItem.SUPPORT_CAMERA_SHORTCUT) != 0);

        if (playVideo) {
            // determine if the point is at center (1/6) of the photo view.
            // (The position of the "play" icon is at center (1/6) of the photo)
            int w = mPhotoView.getWidth();
            int h = mPhotoView.getHeight();
            playVideo = (Math.abs(x - w / 2) * 12 <= w)
                && (Math.abs(y - h / 2) * 12 <= h);
        }

        if (playVideo) {
            if (mSecureAlbum == null) {
                playVideo(mActivity, item.getPlayUri(), item.getName());
            } else {
                mActivity.getStateManager().finishState(this);
            }
        } else if (goBack) {
            onBackPressed();
        } else if (unlock) {
            Intent intent = new Intent(mActivity, GalleryActivity.class);
            intent.putExtra(GalleryActivity.KEY_DISMISS_KEYGUARD, true);
            mActivity.startActivity(intent);
        } else if (launchCamera) {
            launchCamera();
        } else {
            toggleBars();
        }
    }

    @Override
    public void onActionBarAllowed(boolean allowed) {
        mActionBarAllowed = allowed;
        mHandler.sendEmptyMessage(MSG_UPDATE_ACTION_BAR);
    }

    @Override
    public void onActionBarWanted() {
        mHandler.sendEmptyMessage(MSG_WANT_BARS);
    }

    @Override
    public void onFullScreenChanged(boolean full) {
        Message m = mHandler.obtainMessage(
                MSG_ON_FULL_SCREEN_CHANGED, full ? 1 : 0, 0);
        m.sendToTarget();
    }

    // How we do delete/undo:
    //
    // When the user choose to delete a media item, we just tell the
    // FilterDeleteSet to hide that item. If the user choose to undo it, we
    // again tell FilterDeleteSet not to hide it. If the user choose to commit
    // the deletion, we then actually delete the media item.
    @Override
    public void onDeleteImage(Path path, int offset) {
        onCommitDeleteImage();  // commit the previous deletion
        mDeletePath = path;
        mDeleteIsFocus = (offset == 0);
        mMediaSet.addDeletion(path, mCurrentIndex + offset);
    }

    @Override
    public void onUndoDeleteImage() {
        if (mDeletePath == null) return;
        // If the deletion was done on the focused item, we want the model to
        // focus on it when it is undeleted.
        if (mDeleteIsFocus) mModel.setFocusHintPath(mDeletePath);
        mMediaSet.removeDeletion(mDeletePath);
        mDeletePath = null;
    }

    @Override
    public void onCommitDeleteImage() {
        if (mDeletePath == null) return;
        mMenuExecutor.startSingleItemAction(R.id.action_delete, mDeletePath);
        mDeletePath = null;
    }

    public void playVideo(Activity activity, Uri uri, String title) {
        try {
            //Toast.makeText(activity,"PlayVideo:"+mApplication.getIsDecoding(),Toast.LENGTH_SHORT).show();
            Intent intent = new Intent(Intent.ACTION_VIEW)
                    .setDataAndType(uri, "video/*")
                    .putExtra(Intent.EXTRA_TITLE, title)
                    .putExtra(MovieActivity.KEY_TREAT_UP_AS_BACK, true);
            activity.startActivityForResult(intent, REQUEST_PLAY_VIDEO);
        } catch (ActivityNotFoundException e) {
            Toast.makeText(activity, activity.getString(R.string.video_err),
                    Toast.LENGTH_SHORT).show();
        }
    }

    private void setCurrentPhotoByIntent(Intent intent) {
        if (intent == null) return;
        Path path = mApplication.getDataManager()
                .findPathByUri(intent.getData(), intent.getType());
        if (path != null) {
            Path albumPath = mApplication.getDataManager().getDefaultSetOf(path);
            if (albumPath == null) {
                return;
            }
            if (!albumPath.equalsIgnoreCase(mOriginalSetPathString)) {
                // If the edited image is stored in a different album, we need
                // to start a new activity state to show the new image
                Bundle data = new Bundle(getData());
                data.putString(KEY_MEDIA_SET_PATH, albumPath.toString());
                data.putString(PhotoPage.KEY_MEDIA_ITEM_PATH, path.toString());
                mActivity.getStateManager().startState(SinglePhotoPage.class, data);
                return;
            }
            mModel.setCurrentPhoto(path, mCurrentIndex);
        }
    }

    @Override
    protected void onStateResult(int requestCode, int resultCode, Intent data) {
        if (resultCode == Activity.RESULT_CANCELED) {
            // This is a reset, not a canceled
            return;
        }
        mRecenterCameraOnResume = false;
        switch (requestCode) {
            case REQUEST_EDIT:
                setCurrentPhotoByIntent(data);
                break;
            case REQUEST_CROP:
                if (resultCode == Activity.RESULT_OK) {
                    setCurrentPhotoByIntent(data);
                }
                break;
            case REQUEST_CROP_PICASA: {
                if (resultCode == Activity.RESULT_OK) {
                    Context context = mActivity.getAndroidContext();
                    String message = context.getString(R.string.crop_saved,
                            context.getString(R.string.folder_edited_online_photos));
                    Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
                }
                break;
            }
            case REQUEST_SLIDESHOW: {
                Log.d(TAG, "onStateResult: REQUEST_SLIDESHOW");
                if (data == null) break;
                String path = data.getStringExtra(SlideshowPage.KEY_ITEM_PATH);
                int index = data.getIntExtra(SlideshowPage.KEY_PHOTO_INDEX, 0);
                Log.d(TAG, "("+index+"): "+path);
                if (path != null) {
                    mModel.setCurrentPhoto(Path.fromString(path), index);
                }
            }
        }
    }

    private void onPrevious()
    {
        if( mModel instanceof PhotoDataAdapter)
        {
/*        
            if( mCurrentIndex>0 )
            {
                Log.i(TAG, " onPrevious()");
                mModel.moveTo(mCurrentIndex-1);
            }
*/
            if(mProgressControl!=null)
                mProgressControl.onPrevious();
        }
    }

    private void onNext()
    {
        if( mModel instanceof PhotoDataAdapter)
        {
/*
            if(mProgressControl!=null && mCurrentIndex<mProgressControl.getPhotoList().length-1)
            {
                Log.i(TAG, " onNext()");
                mModel.moveTo(mCurrentIndex+1);
            }
*/            
            if(mProgressControl!=null)
                mProgressControl.onNext();

        }
    }

    private void rotate(int degree)
    {
        if(mModel!=null)
        {
            MediaItem item = mModel.getMediaItem(0);
            if(item!=null && (item.getSupportedOperations()&MediaItem.SUPPORT_ROTATE)!=0)
            {
                item.rotate(degree);
                if( mPhotoView !=null )
                {
                    mPhotoView.notifyImageChangeWithoutAnim(0);
                }
            }
        }
    }

    private class PhotoProgressClickListener implements View.OnClickListener
    {
        @Override
        public void onClick(View v)
        {
            switch( v.getId() )
            {
                case R.id.photo_progessbar_slideshow_play:
                    if(mProgressControl != null && mProgressControl.getSlideshowEnable()==false)
                    {
                        mHandler.obtainMessage(MSG_ENABLE_SLIDESHOW, 0, 0, null).sendToTarget();
                    }
                    break;
                case R.id.photo_progessbar_fr:
                    onPrevious();
                    break;
                case R.id.photo_progessbar_ff:
                    onNext();
                    break;
                case R.id.photo_progessbar_rotation_left:
                    if(mModel.getLoadingState(0)==Model.LOADING_COMPLETE) {
                        rotate(-90);
                    }
                    break;
                case R.id.photo_progessbar_rotation_right:
                    if(mModel.getLoadingState(0)==Model.LOADING_COMPLETE) {
                        rotate(90);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    private class PhotoProgressKeyListener implements View.OnKeyListener
    {
        @Override
        public boolean onKey(View view, int keyCode, KeyEvent event)
        {
            if(event.getAction()== KeyEvent.ACTION_DOWN)
            {
                switch(event.getKeyCode())
                {
                    case KeyEvent.KEYCODE_MEDIA_PREVIOUS:
		    case KeyEvent.KEYCODE_MEDIA_REWIND:
                        if(mProgressControl != null && mProgressControl.getSlideshowEnable()==false) {
                            onPrevious();
                        }
                        return true;
                    case KeyEvent.KEYCODE_MEDIA_NEXT:
		    case KeyEvent.KEYCODE_MEDIA_FAST_FORWARD:
                        if(mProgressControl != null && mProgressControl.getSlideshowEnable()==false) {
                            onNext();
                        }
                        return true;
                    case KeyEvent.KEYCODE_DPAD_LEFT:
                        if(mProgressControl != null && mProgressControl.getSlideshowEnable()==false) {
                            if(mPhotoView.isAtMinimalScale()==true) {
                                onPrevious();
                            }
                            else if(event.getRepeatCount()==0) {
                                mPhotoView.scrollPage(ZOOM_IN_SCROLL_PIXEL, 0);
                            }
                        }
                        return true;
                    case KeyEvent.KEYCODE_DPAD_RIGHT:
                        if(mProgressControl != null && mProgressControl.getSlideshowEnable()==false) {
                            if(mPhotoView.isAtMinimalScale()==true) {
                                onNext();
                            }
                            else if(event.getRepeatCount()==0) {
                                mPhotoView.scrollPage(-ZOOM_IN_SCROLL_PIXEL, 0);
                            }
                        }
                        return true;
                    case KeyEvent.KEYCODE_DPAD_UP:
                        if(mProgressControl != null && mProgressControl.getSlideshowEnable()==false) {
                            if(mPhotoView.isAtMinimalScale()==true && mModel.getLoadingState(0)==Model.LOADING_COMPLETE) {
                                rotate(-90);
                            }
                            else if(event.getRepeatCount()==0) {
                                mPhotoView.scrollPage(0, ZOOM_IN_SCROLL_PIXEL);
                            }
                        }
                        return true;
                    case KeyEvent.KEYCODE_DPAD_DOWN:
                        if(mProgressControl != null && mProgressControl.getSlideshowEnable()==false) {
                            if(mPhotoView.isAtMinimalScale()==true && mModel.getLoadingState(0)==Model.LOADING_COMPLETE) {
                                rotate(90);
                            }
                            else if(event.getRepeatCount()==0) {
                                mPhotoView.scrollPage(0, -ZOOM_IN_SCROLL_PIXEL);
                            }
                        }
                        return true;
                    case KeyEvent.KEYCODE_MEDIA_PLAY:
                        if(mProgressControl != null && mProgressControl.getSlideshowEnable()==false) {
                            mHandler.obtainMessage(MSG_ENABLE_SLIDESHOW, 0, 0, null).sendToTarget();
                        }
                        return true;
                    case KeyEvent.KEYCODE_SPACE:
                    case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
                        if(mProgressControl != null) {
                            if(mProgressControl.getSlideshowEnable()==false) {
                                mHandler.obtainMessage(MSG_ENABLE_SLIDESHOW, 0, 0, null).sendToTarget();
                            }
                            else {
                                mHandler.obtainMessage(MSG_DISABLE_SLIDESHOW, 0, 0, null).sendToTarget();
                            }
                        }
                        return true;
                    case KeyEvent.KEYCODE_MEDIA_STOP:
                        mHandler.obtainMessage(MSG_STOP, 0, 0, null).sendToTarget();
                        return true;
                    case KeyEvent.KEYCODE_MENU:
                        if(mProgressControl!=null && mProgressControl.getSlideshowEnable()==true) {
                            mHandler.obtainMessage(MSG_DISABLE_SLIDESHOW, 0, 0, null).sendToTarget();
                        }
                        mHandler.obtainMessage(MSG_SHOW_OPTIONMENU, 0, 0, null).sendToTarget();
                        return true;
                    case KeyEvent.KEYCODE_MEDIA_REPEAT:
                        if(mProgressControl!=null && mProgressControl.getSlideshowEnable()==true) {
                            mHandler.obtainMessage(MSG_DISABLE_SLIDESHOW, 0, 0, null).sendToTarget();
                        }
                        mHandler.obtainMessage(MSG_SHOW_REPEATMENU, 0, 0, null).sendToTarget();
                        return true;
                    case KeyEvent.KEYCODE_SEARCH:
                        if(mProgressControl != null && mProgressControl.getSlideshowEnable()==false) {
                            if(mCurrentPhoto.getFilePath().endsWith("gif")) {
                                return true;
                            }
                            mPhotoView.toggleZoomIn();
                        }
                        return true;
                    default:
                        break;
                }
            }
            return false;
        }
    }

    public Boolean slideshowEnable()
    {
        if (mModel == null) return true;

        MediaItem current = mModel.getMediaItem(0);

        // This is a shield for monkey when it clicks the action bar
        // menu when transitioning from filmstrip to camera
        if (current instanceof SnailItem) return true;
        // TODO: We should check the current photo against the MediaItem
        // that the menu was initially created for. We need to fix this
        // after PhotoPage being refactored.
        if (current == null) {
            // item is not ready, ignore
            return true;
        }
        int currentIndex = mModel.getCurrentIndex();
        Path path = current.getPath();

        Bundle data = new Bundle();
        data.putString(SlideshowPage.KEY_SET_PATH, mMediaSet.getPath().toString());
        data.putString(SlideshowPage.KEY_ITEM_PATH, path.toString());
        data.putInt(SlideshowPage.KEY_PHOTO_INDEX, currentIndex);
        if(mSlideshowRepeatMode==REPEAT_MODE_OFF) {
            data.putBoolean(SlideshowPage.KEY_REPEAT, false);
            data.putBoolean(SlideshowPage.KEY_RANDOM_ORDER, false);
        }
        else if(mSlideshowRepeatMode==REPEAT_MODE_SHUFFLE) {
            data.putBoolean(SlideshowPage.KEY_REPEAT, true);
            data.putBoolean(SlideshowPage.KEY_RANDOM_ORDER, true);
        }
        else if(mSlideshowRepeatMode==REPEAT_MODE_ALL) {
            data.putBoolean(SlideshowPage.KEY_REPEAT, true);
            data.putBoolean(SlideshowPage.KEY_RANDOM_ORDER, false);
        }
        data.putInt(SlideshowPage.KEY_SLIDESHOW_TIMING, mSlideshowTiming);
        data.putInt(SlideshowPage.KEY_EFFECT_MODE, mSlideshowEffect);
        if(mProgressControl!=null && mPlayList!=null)
        {
            data.putStringArray(SlideshowPage.KEY_PLAYLIST, mProgressControl.getPhotoList());
        }
        // Add BG music enabled setting.
        data.putBoolean(KEY_BROADCAST_STATUS, mBroadcastStatus);
        data.putString(SlideshowPage.KEY_SOURCE_FROM, mSourceFrom);
        mActivity.getStateManager().startStateForResult(
                SlideshowPage.class, REQUEST_SLIDESHOW, data);
        return true;
    }

    @Override
    public void onPause() {
        Log.d(TAG, "onPause");
        super.onPause();
        mIsActive = false;

        mActivity.getGLRoot().unfreeze();
        mHandler.removeMessages(MSG_UNFREEZE_GLROOT);

        DetailsHelper.pause();
        // Hide the detail dialog on exit
        if (mShowDetails) hideDetails();
        mPhotoView.pause();
        if (mModel != null) {
            mModel.pause();
        }
        mHandler.removeMessages(MSG_HIDE_BARS);
        mHandler.removeMessages(MSG_REFRESH_BOTTOM_CONTROLS);
        refreshBottomControlsWhenReady();
        mActionBar.removeOnMenuVisibilityListener(mMenuVisibilityListener);
        if (mShowSpinner) {
            mActionBar.disableAlbumModeMenu(true);
        }
        onCommitDeleteImage();
        mMenuExecutor.pause();
        if (mMediaSet != null) mMediaSet.clearDeletion();

        if(mProgressControl != null)
        {
            mProgressControl.cancelWork();
            mProgressControl.show(false);
        }
//        GalleryUtils.cancelPhotoNotification(mActivity);
    }

    @Override
    public void onCurrentImageUpdated() {
        mActivity.getGLRoot().unfreeze();
    }

    @Override
    public void onFilmModeChanged(boolean enabled) {
        refreshBottomControlsWhenReady();
        if (mShowSpinner) {
            if (enabled) {
                mActionBar.enableAlbumModeMenu(
                        GalleryActionBar.ALBUM_FILMSTRIP_MODE_SELECTED, this);
            } else {
                mActionBar.disableAlbumModeMenu(true);
            }
        }
        if (enabled) {
            mHandler.removeMessages(MSG_HIDE_BARS);
            UsageStatistics.onContentViewChanged(
                    UsageStatistics.COMPONENT_GALLERY, "FilmstripPage");
        } else {
            refreshHidingMessage();
            if (mAppBridge == null || mCurrentIndex > 0) {
                UsageStatistics.onContentViewChanged(
                        UsageStatistics.COMPONENT_GALLERY, "SinglePhotoPage");
            } else {
                UsageStatistics.onContentViewChanged(
                        UsageStatistics.COMPONENT_CAMERA, "Unknown"); // TODO
            }
        }
    }

    private void transitionFromAlbumPageIfNeeded() {
        TransitionStore transitions = mActivity.getTransitionStore();

        int albumPageTransition = transitions.get(
                KEY_ALBUMPAGE_TRANSITION, MSG_ALBUMPAGE_NONE);

        if (albumPageTransition == MSG_ALBUMPAGE_NONE && mAppBridge != null
                && mRecenterCameraOnResume) {
            // Generally, resuming the PhotoPage when in Camera should
            // reset to the capture mode to allow quick photo taking
            mCurrentIndex = 0;
            mPhotoView.resetToFirstPicture();
        } else {
            int resumeIndex = transitions.get(KEY_INDEX_HINT, -1);
            if (resumeIndex >= 0) {
                if (mHasCameraScreennailOrPlaceholder) {
                    // Account for preview/placeholder being the first item
                    resumeIndex++;
                }
                if (resumeIndex < mMediaSet.getMediaItemCount()) {
                    mCurrentIndex = resumeIndex;
                    mModel.moveTo(mCurrentIndex);
                }
            }
        }

        if (albumPageTransition == MSG_ALBUMPAGE_RESUMED) {
            mPhotoView.setFilmMode(mStartInFilmstrip || mAppBridge != null);
        } else if (albumPageTransition == MSG_ALBUMPAGE_PICKED) {
            mPhotoView.setFilmMode(false);
        }
    }

    @Override
    protected void onResume() {
        Log.d(TAG, "onResume");
        super.onResume();

        if (mModel == null) {
            mActivity.getStateManager().finishState(this);
            return;
        }
        transitionFromAlbumPageIfNeeded();

        mActivity.getGLRoot().freeze();
        mIsActive = true;
        setContentPane(mRootPane);

        if(mPlayList!=null)
            mModel.setMediaDataSet(mActivity, mPlayList);
        mModel.resume();
        mPhotoView.resume();
        mActionBar.setDisplayOptions(
                ((mSecureAlbum == null) && (mSetPathString != null)), false);
        mActionBar.addOnMenuVisibilityListener(mMenuVisibilityListener);
        refreshBottomControlsWhenReady();
        if (mShowSpinner && mPhotoView.getFilmMode()) {
            mActionBar.enableAlbumModeMenu(
                    GalleryActionBar.ALBUM_FILMSTRIP_MODE_SELECTED, this);
        }
        if (!mShowBars) {
            mActionBar.hide();
            mActivity.getGLRoot().setLightsOutMode(true);
        }
        boolean haveImageEditor = GalleryUtils.isEditorAvailable(mActivity, "image/*");
        if (haveImageEditor != mHaveImageEditor) {
            mHaveImageEditor = haveImageEditor;
            updateMenuOperations();
        }

        mRecenterCameraOnResume = true;
        mHandler.sendEmptyMessageDelayed(MSG_UNFREEZE_GLROOT, UNFREEZE_GLROOT_TIMEOUT);

        if(mProgressControl!=null)
        {
            mProgressControl.show(true);
        }
    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "onDestroy");
        if (mAppBridge != null) {
            mAppBridge.setServer(null);
            mScreenNailItem.setScreenNail(null);
            mAppBridge.detachScreenNail();
            mAppBridge = null;
            mScreenNailSet = null;
            mScreenNailItem = null;
        }
        mActivity.getGLRoot().setOrientationSource(null);
        if (mBottomControls != null) mBottomControls.cleanup();

        if(mProgressControl != null)
        {
            mProgressControl.setClickListener(null);
            mProgressControl.setKeyListener(null);
        }
        // Remove all pending messages.
        mHandler.removeCallbacksAndMessages(null);
        super.onDestroy();
    }

    private class MyDetailsSource implements DetailsSource {

        @Override
        public MediaDetails getDetails() {
            return mModel.getMediaItem(0).getDetails();
        }

        @Override
        public int size() {
            return mMediaSet != null ? mMediaSet.getMediaItemCount() : 1;
        }

        @Override
        public int setIndex() {
            return mModel.getCurrentIndex();
        }
    }

    @Override
    public void onAlbumModeSelected(int mode) {
        if (mode == GalleryActionBar.ALBUM_GRID_MODE_SELECTED) {
            switchToGrid();
        }
    }

    @Override
    public void refreshBottomControlsWhenReady() {
        if (mBottomControls == null) {
            return;
        }
        MediaObject currentPhoto = mCurrentPhoto;
        if (currentPhoto == null) {
            mHandler.obtainMessage(MSG_REFRESH_BOTTOM_CONTROLS, 0, 0, currentPhoto).sendToTarget();
        } else {
            currentPhoto.getPanoramaSupport(mRefreshBottomControlsCallback);
        }
    }

    private void updatePanoramaUI(boolean isPanorama360) {
        Menu menu = mActionBar.getMenu();

        // it could be null if onCreateActionBar has not been called yet
        if (menu == null) {
            return;
        }

        MenuExecutor.updateMenuForPanorama(menu, isPanorama360, isPanorama360);

        if (isPanorama360) {
            MenuItem item = menu.findItem(R.id.action_share);
            if (item != null) {
                item.setShowAsAction(MenuItem.SHOW_AS_ACTION_NEVER);
                item.setTitle(mActivity.getResources().getString(R.string.share_as_photo));
            }
        } else if ((mCurrentPhoto.getSupportedOperations() & MediaObject.SUPPORT_SHARE) != 0) {
            MenuItem item = menu.findItem(R.id.action_share);
            if (item != null) {
                item.setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM);
                item.setTitle(mActivity.getResources().getString(R.string.share));
            }
        }
    }

    @Override
    public void onUndoBarVisibilityChanged(boolean visible) {
        refreshBottomControlsWhenReady();
    }

    @Override
    public boolean onShareTargetSelected(ShareActionProvider source, Intent intent) {
        final long timestampMillis = mCurrentPhoto.getDateInMs();
        final String mediaType = getMediaTypeString(mCurrentPhoto);
        UsageStatistics.onEvent(UsageStatistics.COMPONENT_GALLERY,
                UsageStatistics.ACTION_SHARE,
                mediaType,
                        timestampMillis > 0
                        ? System.currentTimeMillis() - timestampMillis
                        : -1);
        return false;
    }

    private static String getMediaTypeString(MediaItem item) {
        if (item.getMediaType() == MediaObject.MEDIA_TYPE_VIDEO) {
            return "Video";
        } else if (item.getMediaType() == MediaObject.MEDIA_TYPE_IMAGE) {
            return "Photo";
        } else {
            return "Unknown:" + item.getMediaType();
        }
    }

    static public void UpdatePhotoRecentPlay(Activity activity, String path)
    {
        Uri uri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
        try
        {
            String selection = MediaStore.MediaColumns.DATA + " LIKE ?";
            String[] selectionArgs = { String.valueOf(path) };
            ContentResolver cr = activity.getContentResolver();
            ContentValues values = new ContentValues();
            Date date = new Date();
            values.put("date_recentplay", (date.getTime() / 1000));
            cr.update(uri, values, selection, selectionArgs);
        }
        catch (Exception e)
        {
            Log.e(TAG, "Exception", e);
        }
    }

    public void showOption() {
        if(mProgressControl!=null && mProgressControl.getSlideshowEnable()==true) {
            return;
        }
        if(mBroadcastStatus==true)
            return;
        if(mOptionMenu == null)
            NewOptionMenu();
        mOptionMenu.Show();
    }

    private void NewOptionMenu() {
        View ContentView = mActivity.getLayoutInflater().inflate(R.layout.optionmenu,null);
        Display display = mActivity.getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int w = mActivity.getResources().getDimensionPixelSize(R.dimen.option_menu_width);
        int h = mActivity.getResources().getDimensionPixelSize(R.dimen.option_menu_height);
        final int x = screenSize.x-w;
        final int y = (screenSize.y/2)-(h/2);
        mOptionMenu = new RTKOptionMenu(x, y, w, h,
            mActivity.getResources().getXml(R.xml.photo_playback_option), ContentView, null);
        mOptionMenu.setItemClickListener(new OptionMenuItemClickListener());
        mOptionMenu.setOnKeyListener(new OptionMenuItemOnKeyListener());
    }

    private class OptionMenuItemClickListener implements AdapterView.OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id)
        {
            Log.d("GalleryActivity", "Option menu " + mOptionMenu.getItemof(position)+" is clicked");
            optionMenuHandler(mOptionMenu.getItemof(position));
        }
    }

    private class OptionMenuItemOnKeyListener implements AdapterView.OnKeyListener
    {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            int itemPosition = mOptionMenu.getSelectedItemPosition();
            Log.d("GalleryActivity", "Option menu " + mOptionMenu.getItemof(itemPosition)+ " is on key");
            int action = event.getAction();

            if (action == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE) {
                    mOptionMenu.Hide();
                    return true;
                }
                else if (isMediaKey(keyCode)) {
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                    return optionMenuHandler(mOptionMenu.getItemof(itemPosition));
                }
            }
            return false;
        }
    }

    private static boolean isMediaKey(int keyCode) {
        return keyCode == KeyEvent.KEYCODE_HEADSETHOOK
                || keyCode == KeyEvent.KEYCODE_MEDIA_PREVIOUS
                || keyCode == KeyEvent.KEYCODE_MEDIA_NEXT
                || keyCode == KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE
                || keyCode == KeyEvent.KEYCODE_MEDIA_PLAY
                || keyCode == KeyEvent.KEYCODE_MEDIA_PAUSE
                || keyCode == KeyEvent.KEYCODE_MEDIA_STOP;
    }
    public boolean optionMenuHandler(String item){
        mOptionMenu.Hide();
        if(mProgressControl!=null && mProgressControl.getSlideshowEnable()==true) {
            return false;
        }
        if(item==null) {
            return false;
        }

        if (item.equals("Slideshow")){
            mHandler.obtainMessage(MSG_SHOW_SLIDESHOW_MENU, 0, 0, null).sendToTarget();
            return true;
        }
        else if (item.equals("Slideshow timing")){
            mHandler.obtainMessage(MSG_SHOW_SLIDESHOW_TIMING_MENU, 0, 0, null).sendToTarget();
            return true;
        }
        else if (item.equals("Slideshow repeat mode")){
            mHandler.obtainMessage(MSG_SHOW_REPEATMENU, 0, 0, null).sendToTarget();
            return true;
        }
        else if (item.equals("Transition effect")){
            mHandler.obtainMessage(MSG_SHOW_TRANSITION_EFFECT_MENU, 0, 0, null).sendToTarget();
            return true;
        }
        else if (item.equals("Background music")){
            mHandler.obtainMessage(MSG_SHOW_BG_NUSIC_MENU, 0, 0, null).sendToTarget();
            return true;
        }
        else if (item.equals("EXIF info")) {
            mHandler.obtainMessage(MSG_SHOW_EXIF_MENU, 0, 0, null).sendToTarget();
            return true;
        }
        return false;
    }

    private void NewSlideshowMenu() {
        View ContentView = mActivity.getLayoutInflater().inflate(R.layout.vscrollmenu,null);
        Display display = mActivity.getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int w = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_width);
        int h = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_height);
        int margin = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_margin_right);
        final int x = screenSize.x-w-margin;
        final int y = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_margin_top);
        mSlideshowMenu = new RTKVScrollMenu(x, y,
            (int)mActivity.getResources().getDimension(R.dimen.photo_scroll_menu_width),
            (int)mActivity.getResources().getDimension(R.dimen.photo_scroll_menu_height),
            mActivity.getString(R.string.slideshow), null, ContentView);
        mSlideshowMenu.setOnKeyListener(new SlideshowItemOnKeyListener());
        mSlideshowMenu.setItemClickListener(new SlideshowItemOnClickListener());
        ArrayList<String> list = new ArrayList<String>();
        list.add(new String(mActivity.getString(R.string.setting_off)));
        list.add(new String(mActivity.getString(R.string.setting_on)));
        mSlideshowMenu.setSelectionList(list);
        mSlideshowMenu.setSelectedItem(0);
    }

    private void NewSlideshowTimingMenu() {
        View ContentView = mActivity.getLayoutInflater().inflate(R.layout.vscrollmenu,null);
        Display display = mActivity.getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int w = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_width);
        int h = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_height);
        int margin = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_margin_right);
        final int x = screenSize.x-w-margin;
        final int y = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_margin_top);
        mSlideshowTimingMenu = new RTKVScrollMenu(x, y,
            (int)mActivity.getResources().getDimension(R.dimen.photo_scroll_menu_width),
            (int)mActivity.getResources().getDimension(R.dimen.photo_scroll_menu_height),
            mActivity.getString(R.string.slideshow_timing), null, ContentView);
        mSlideshowTimingMenu.setOnKeyListener(new SlideshowTimingItemOnKeyListener());
        mSlideshowTimingMenu.setItemClickListener(new SlideshowTimingItemOnClickListener());
        ArrayList<String> list = new ArrayList<String>();
        list.add(new String(mActivity.getString(R.string.pref_video_time_lapse_frame_interval_2000)));
        list.add(new String(mActivity.getString(R.string.pref_video_time_lapse_frame_interval_5000)));
        list.add(new String(mActivity.getString(R.string.pref_video_time_lapse_frame_interval_10000)));
        list.add(new String(mActivity.getString(R.string.pref_video_time_lapse_frame_interval_30000)));
        mSlideshowTimingMenu.setSelectionList(list);
        mSlideshowTimingMenu.setSelectedItem(1);
    }

    private void NewBgMusicMenu() {
        View ContentView = mActivity.getLayoutInflater().inflate(R.layout.vscrollmenu,null);
        Display display = mActivity.getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int w = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_width);
        int h = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_height);
        int margin = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_margin_right);
        final int x = screenSize.x-w-margin;
        final int y = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_margin_top);
        mBgMusicMenu = new RTKVScrollMenu(x, y,
            (int)mActivity.getResources().getDimension(R.dimen.photo_scroll_menu_width),
            (int)mActivity.getResources().getDimension(R.dimen.photo_scroll_menu_height),
            mActivity.getString(R.string.background_music), null, ContentView);
        mBgMusicMenu.setOnKeyListener(new BgMusicItemOnKeyListener());
        mBgMusicMenu.setItemClickListener(new BgMusicItemOnClickListener());
        ArrayList<String> list = new ArrayList<String>();
        list.add(new String(mActivity.getString(R.string.setting_off)));
        list.add(new String(mActivity.getString(R.string.setting_on)));
        mBgMusicMenu.setSelectionList(list);
        mBgMusicMenu.setSelectedItem(0);
    }

    private void NewEXIFMenu() {
        View mEXIFView = mActivity.getLayoutInflater().inflate(R.layout.vscrollmenu_exif_info,null);
        Display display = mActivity.getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int w = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_exif_menu_width);
        int h = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_exif_menu_height);
        final int x = (screenSize.x-w)/2;
        final int y = (screenSize.y-h)/2;
        mEXIFMenu = new RTKVScrollMenu(x, y, w, h,
            mActivity.getString(R.string.exif_info), null, mEXIFView);
        mEXIFMenu.setGravity(Gravity.LEFT);
        mEXIFMenu.setOnKeyListener(new mEXIFMenuOnKeyListener());
        getExifList();
    }

    private void getExifList()
    {
        ArrayList<String> list = new ArrayList<String>();
        if(mCurrentPhoto!=null)
        {
            String str=new String();
            int[] indexArray = { MediaDetails.INDEX_TITLE, MediaDetails.INDEX_DESCRIPTION, MediaDetails.INDEX_DATETIME, MediaDetails.INDEX_LOCATION, 
                MediaDetails.INDEX_WIDTH, MediaDetails.INDEX_HEIGHT, MediaDetails.INDEX_ORIENTATION, MediaDetails.INDEX_DURATION, MediaDetails.INDEX_MIMETYPE, 
                MediaDetails.INDEX_SIZE, MediaDetails.INDEX_MAKE, MediaDetails.INDEX_MODEL, MediaDetails.INDEX_FLASH, MediaDetails.INDEX_FOCAL_LENGTH, 
                MediaDetails.INDEX_WHITE_BALANCE, MediaDetails.INDEX_APERTURE, MediaDetails.INDEX_SHUTTER_SPEED, MediaDetails.INDEX_EXPOSURE_TIME, 
                MediaDetails.INDEX_ISO, MediaDetails.INDEX_PATH };
            MediaDetails details = mCurrentPhoto.getDetails();

            for(int index=0;index<indexArray.length;index++)
            {
                if(details.getDetail(indexArray[index])!=null)
                {
                    switch(indexArray[index])
                    {
                        case MediaDetails.INDEX_TITLE:
                            str = mActivity.getString(R.string.title);
                            break;
                        case MediaDetails.INDEX_DESCRIPTION:
                            str = mActivity.getString(R.string.description);
                            break;
                        case MediaDetails.INDEX_DATETIME:
                            str = mActivity.getString(R.string.time);
                            break;
                        case MediaDetails.INDEX_LOCATION:
                            str = mActivity.getString(R.string.location);
                            break;
                        case MediaDetails.INDEX_WIDTH:
                            str = mActivity.getString(R.string.dimension);
                            break;
                        case MediaDetails.INDEX_HEIGHT:
                            continue;
                        case MediaDetails.INDEX_ORIENTATION:
                            str = mActivity.getString(R.string.orientation);
                            break;
                        case MediaDetails.INDEX_DURATION:
                            str = mActivity.getString(R.string.duration);
                            break;
                        case MediaDetails.INDEX_MIMETYPE:
                            str = mActivity.getString(R.string.mimetype);
                            break;
                        case MediaDetails.INDEX_SIZE:
                            str = mActivity.getString(R.string.file_size);
                            break;
                        case MediaDetails.INDEX_MAKE:
                            str = mActivity.getString(R.string.maker);
                            break;
                        case MediaDetails.INDEX_MODEL:
                            str = mActivity.getString(R.string.model);
                            break;
                        case MediaDetails.INDEX_FLASH:
                            str = mActivity.getString(R.string.flash);
                            break;
                        case MediaDetails.INDEX_FOCAL_LENGTH:
                            str = mActivity.getString(R.string.focal_length);
                            break;
                        case MediaDetails.INDEX_WHITE_BALANCE:
                            str = mActivity.getString(R.string.white_balance);
                            break;
                        case MediaDetails.INDEX_APERTURE:
                            str = mActivity.getString(R.string.aperture);
                            break;
                        case MediaDetails.INDEX_SHUTTER_SPEED:
                            str = mActivity.getString(R.string.exposure_time);
                            break;
                        case MediaDetails.INDEX_EXPOSURE_TIME:
                            str = mActivity.getString(R.string.exposure_time);
                            break;
                        case MediaDetails.INDEX_ISO:
                            str = mActivity.getString(R.string.iso);
                            break;
                        case MediaDetails.INDEX_PATH:
                            str = mActivity.getString(R.string.path);
                            break;
                        default:
                            str = "";
                            break;
                    }
                    str +=" : \t\t";

                    if(indexArray[index]==MediaDetails.INDEX_FLASH)
                    {
                        MediaDetails.FlashState state = (MediaDetails.FlashState)details.getDetail(indexArray[index]);
                        Boolean isFlashFired = state.isFlashFired();
                        if(isFlashFired==true)
                            str += mActivity.getString(R.string.flash_on);
                        else
                            str += mActivity.getString(R.string.flash_off);
                    }
                    else if(indexArray[index]==MediaDetails.INDEX_WIDTH)
                    {
                        str += details.getDetail(MediaDetails.INDEX_WIDTH) + " * " + details.getDetail(MediaDetails.INDEX_HEIGHT) + " pixels";
                    }
                    else if(indexArray[index]==MediaDetails.INDEX_SIZE)
                    {
                        long fileSize = Long.valueOf(details.getDetail(indexArray[index]).toString());
                        if(fileSize >= 1024*1024*1024)
                            str += (fileSize/(1024*1024*1024)) + " GB";
                        else if(fileSize >= 1024*1024)
                            str += (fileSize/(1024*1024)) + " MB";
                        else if(fileSize >= 1024)
                            str += (fileSize/(1024)) + " KB";
                        else
                            str += (fileSize) + " Bytes";
                    }
                    else
                    {
                        str += details.getDetail(indexArray[index]);
                    }
                    if(details.hasUnit(indexArray[index])==true) {
                        str += " " + mActivity.getString(details.getUnit(indexArray[index]));
                    }
                    list.add(new String(str));
                }
            }
        }
        mEXIFMenu.setSelectionList(list);
    }

    private void NewRepeatMenu() {
        View ContentView = mActivity.getLayoutInflater().inflate(R.layout.vscrollmenu,null);
        Display display = mActivity.getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int w = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_width);
        int h = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_height);
        int margin = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_margin_right);
        final int x = screenSize.x-w-margin;
        final int y = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_margin_top);
        mSlideshowRepeatMenu = new RTKVScrollMenu(x, y,
            (int)mActivity.getResources().getDimension(R.dimen.photo_scroll_menu_width),
            (int)mActivity.getResources().getDimension(R.dimen.photo_scroll_menu_height),
            mActivity.getString(R.string.repeat_mode), null, ContentView);
        mSlideshowRepeatMenu.setOnKeyListener(new SlideshowRepeatItemOnKeyListener());
        mSlideshowRepeatMenu.setItemClickListener(new SlideshowRepeatItemOnClickListener());
        ArrayList<String> list = new ArrayList<String>();
        list.add(new String(mActivity.getString(R.string.setting_off)));
        list.add(new String(mActivity.getString(R.string.repeat_shuffle)));
        list.add(new String(mActivity.getString(R.string.repeat_all)));
        mSlideshowRepeatMenu.setSelectionList(list);
        mSlideshowRepeatMenu.setSelectedItem(mSlideshowRepeatMode);
    }

    private void NewEffectMenu() {
        View ContentView = mActivity.getLayoutInflater().inflate(R.layout.vscrollmenu,null);
        Display display = mActivity.getWindowManager().getDefaultDisplay();
        Point screenSize = new Point();
        display.getSize(screenSize);
        int w = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_width);
        int h = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_height);
        int margin = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_margin_right);
        final int x = screenSize.x-w-margin;
        final int y = mActivity.getResources().getDimensionPixelSize(R.dimen.photo_scroll_menu_margin_top);
        mSlideshowEffectMenu= new RTKVScrollMenu(x, y,
            (int)mActivity.getResources().getDimension(R.dimen.photo_scroll_menu_width),
            (int)mActivity.getResources().getDimension(R.dimen.photo_scroll_menu_height),
            mActivity.getString(R.string.transition_effect), null, ContentView);
        mSlideshowEffectMenu.setOnKeyListener(new SlideshowEffectItemOnKeyListener());
        mSlideshowEffectMenu.setItemClickListener(new SlideshowEffectItemOnClickListener());
        ArrayList<String> list = new ArrayList<String>();
        list.add(new String(mActivity.getString(R.string.setting_off)));
        list.add(new String(mActivity.getString(R.string.ken_burns)));
        mSlideshowEffectMenu.setSelectionList(list);
        mSlideshowEffectMenu.setSelectedItem(mSlideshowEffect);
    }

    private void setSlideshowTiming(int position)
    {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(mActivity.getAndroidContext());
        SharedPreferences.Editor editor = prefs.edit();
        switch(position)
        {
            case 0:
                mSlideshowTiming=2;
                editor.putInt(KEY_SLIDESHOW_TIMING_PREF, mSlideshowTiming);
                editor.commit();
                break;
            case 1:
                mSlideshowTiming=5;
                editor.putInt(KEY_SLIDESHOW_TIMING_PREF, mSlideshowTiming);
                editor.commit();
                break;
            case 2:
                mSlideshowTiming=10;
                editor.putInt(KEY_SLIDESHOW_TIMING_PREF, mSlideshowTiming);
                editor.commit();
                break;
            case 3:
                mSlideshowTiming=30;
                editor.putInt(KEY_SLIDESHOW_TIMING_PREF, mSlideshowTiming);
                editor.commit();
                break;
            default:
                break;
        }
        mSlideshowTimingMenu.Hide();
    }

    private void setSlideshow(int position)
    {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(mActivity.getAndroidContext());
        SharedPreferences.Editor editor = prefs.edit();
        switch(position)
        {
            case 0:
                mSlideshow = false;
                editor.putBoolean(KEY_SLIDESHOW_PREF,mSlideshow);
                editor.commit();
                break;
            case 1:
                mSlideshow = true;
                editor.putBoolean(KEY_SLIDESHOW_PREF,mSlideshow);
                editor.commit();
                break;
            default:
                break;
        }
        mSlideshowMenu.Hide();
    }

    private class SlideshowItemOnKeyListener implements AdapterView.OnKeyListener {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE) {
                    mSlideshowMenu.Hide();
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                    int position = mSlideshowMenu.getSelectedItemPosition();
                    setSlideshow(position);
                    return true;
                }
            }
            return false;
        }
    }

    private class SlideshowItemOnClickListener implements AdapterView.OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id)
        {
            setSlideshow(position);
        }
    }

    private class SlideshowTimingItemOnKeyListener implements AdapterView.OnKeyListener {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE) {
                    mSlideshowTimingMenu.Hide();
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                    int position = mSlideshowTimingMenu.getSelectedItemPosition();
                    setSlideshowTiming(position);
                    return true;
                }
            }
            return false;
        }
    }

    private class SlideshowTimingItemOnClickListener implements AdapterView.OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id)
        {
            setSlideshowTiming(position);
        }
    }

    private void setSlideshowRepeatMode(int position)
    {
        Log.d("GalleryActivity", "setSlideshowRepeatMode " + position);
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(mActivity.getAndroidContext());
        SharedPreferences.Editor editor = prefs.edit();
        switch(position)
        {
            case 0: //Rpeat off
                mSlideshowRepeatMode=REPEAT_MODE_OFF;
                editor.putInt(KEY_SLIDESHOW_REPEAT_MODE_PREF, mSlideshowRepeatMode);
                editor.commit();
                break;
            case 1: //Rpeat shuffle
                mSlideshowRepeatMode=REPEAT_MODE_SHUFFLE;
                editor.putInt(KEY_SLIDESHOW_REPEAT_MODE_PREF, mSlideshowRepeatMode);
                editor.commit();
                break;
            case 2: //Rpeat all
                mSlideshowRepeatMode=REPEAT_MODE_ALL;
                editor.putInt(KEY_SLIDESHOW_REPEAT_MODE_PREF, mSlideshowRepeatMode);
                editor.commit();
                break;
            default:
                break;
        }
        mSlideshowRepeatMenu.Hide();
    }

    private class SlideshowRepeatItemOnKeyListener implements AdapterView.OnKeyListener {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK ||
                    keyCode == KeyEvent.KEYCODE_ESCAPE || keyCode == KeyEvent.KEYCODE_MEDIA_REPEAT) {
                    mSlideshowRepeatMenu.Hide();
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                    int position = mSlideshowRepeatMenu.getSelectedItemPosition();
                    setSlideshowRepeatMode(position);
                    return true;
                }
            }
            return false;
        }
    }

    private class SlideshowRepeatItemOnClickListener implements AdapterView.OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id)
        {
            setSlideshowRepeatMode(position);
        }
    }

    private void setSlideshowEffectMode(int position)
    {
        Log.d("GalleryActivity", "setSlideshowEffect " + position);
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(mActivity.getAndroidContext());
        SharedPreferences.Editor editor = prefs.edit();
        switch(position)
        {
            case 0: //Effect off
                mSlideshowEffect=GalleryUtils.EFFECT_OFF;
                editor.putInt(KEY_SLIDESHOW_EFFECT_PREF, mSlideshowEffect);
                editor.commit();
                break;
            case 1: //Effect Ken Burns
                mSlideshowEffect=GalleryUtils.EFFECT_KEN_BURNS;
                editor.putInt(KEY_SLIDESHOW_EFFECT_PREF, mSlideshowEffect);
                editor.commit();
                break;
            default:
                break;
        }
        mSlideshowEffectMenu.Hide();
    }

    private class SlideshowEffectItemOnKeyListener implements AdapterView.OnKeyListener {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK ||
                    keyCode == KeyEvent.KEYCODE_ESCAPE) {
                    mSlideshowEffectMenu.Hide();
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                    int position = mSlideshowEffectMenu.getSelectedItemPosition();
                    setSlideshowEffectMode(position);
                    return true;
                }
            }
            return false;
        }
    }

    private class SlideshowEffectItemOnClickListener implements AdapterView.OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id)
        {
            setSlideshowEffectMode(position);
        }
    }

    private void setBgMusic(int position)
    {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(mActivity.getAndroidContext());
        SharedPreferences.Editor editor = prefs.edit();
        switch(position)
        {
            case 0:
                //mActivity.stopBGMusicService();
                mSlideshowBGMusic = false;
                editor.putBoolean(KEY_BG_MUSIC_PREF,mSlideshowBGMusic);
                editor.commit();
                break;
            case 1:
                //mActivity.startBGMusicService();
                mSlideshowBGMusic = true;
                editor.putBoolean(KEY_BG_MUSIC_PREF,mSlideshowBGMusic);
                editor.commit();
                break;
            default:
                break;
        }
        mBgMusicMenu.Hide();
    }

    private class BgMusicItemOnKeyListener implements AdapterView.OnKeyListener {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE) {
                    mBgMusicMenu.Hide();
                    return true;
                }
                else if (keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                    int position = mBgMusicMenu.getSelectedItemPosition();
                    setBgMusic(position);
                    return true;
                }
            }
            return false;
        }
    }

    private class BgMusicItemOnClickListener implements AdapterView.OnItemClickListener
    {
        @Override
        public void onItemClick(AdapterView<?> parent, View v, int position, long id)
        {
            setBgMusic(position);
        }
    }

    private class mEXIFMenuOnKeyListener implements AdapterView.OnKeyListener {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event)
        {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_MENU || keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE) {
                    mEXIFMenu.Hide();
                    return true;
                }
            }
            return false;
        }
    }
}
