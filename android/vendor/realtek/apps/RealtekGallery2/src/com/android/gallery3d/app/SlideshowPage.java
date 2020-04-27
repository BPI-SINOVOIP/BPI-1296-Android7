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

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningServiceInfo;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.media.AudioManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.preference.PreferenceManager;
import android.provider.Settings;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.android.gallery3d.R;
import com.android.gallery3d.common.Utils;
import com.android.gallery3d.data.ContentListener;
import com.android.gallery3d.data.MediaItem;
import com.android.gallery3d.data.MediaObject;
import com.android.gallery3d.data.MediaSet;
import com.android.gallery3d.data.Path;
import com.android.gallery3d.glrenderer.GLCanvas;
import com.android.gallery3d.ui.GLView;
import com.android.gallery3d.ui.SlideshowView;
import com.android.gallery3d.ui.SynchronizedHandler;
import com.android.gallery3d.util.Future;
import com.android.gallery3d.util.FutureListener;
import com.android.gallery3d.util.GalleryUtils;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

public class SlideshowPage extends ActivityState implements SharedPreferences.OnSharedPreferenceChangeListener{
    private static final String TAG = "SlideshowPage";

    private static final boolean DEBUG=false;

    public static final String KEY_SET_PATH = "media-set-path";
    public static final String KEY_ITEM_PATH = "media-item-path";
    public static final String KEY_PHOTO_INDEX = "photo-index";
    public static final String KEY_RANDOM_ORDER = "random-order";
    public static final String KEY_REPEAT = "repeat";
    public static final String KEY_PLAYLIST = "playlist";
    public static final String KEY_PLAYLIST_ADDR = "PlayListAddress";
    public static final String KEY_PLAYLIST_TITLE = "playlistTitle";
    public static final String KEY_PLAYLIST_TITLE_ADDR = "PlayListTitleAddress";
    public static final String KEY_DREAM = "dream";
    public static final String KEY_SOURCE_FROM = "SourceFrom";
    public static final String KEY_SLIDESHOW_TIMING = "slideshow-timing";
    public static final String KEY_NAVI_BY_COMMAND = "navi-by-command";
    public static final String KEY_BROADCAST_STATUS = "android.intent.playback.broadcast.status";
    public static final String KEY_EFFECT_MODE = "effect-mode";
    public static final int DEFAULT_SLIDESHOW_EFFECT = GalleryUtils.EFFECT_OFF;

//    private static final long SLIDESHOW_DELAY = 3000; // 3 seconds

    private static final int MSG_LOAD_NEXT_BITMAP = 1;
    private static final int MSG_SHOW_PENDING_BITMAP = 2;
    private static final int MSG_HIDE_SLIDESHOW_INFO = 3;

    private Animation mContainerAnimIn = new AlphaAnimation(0f, 1f);
    private Animation mContainerAnimOut = new AlphaAnimation(1f, 0f);

    public static interface Model {
        public void pause();

        public void resume();

        public Future<Slide> nextSlide(FutureListener<Slide> listener);

        public Future<Slide> previousSlide(FutureListener<Slide> listener);

        public void setPreLoad(boolean bPreLoad);

        public void setLoadIndex(int index);

        public void toggleRepeat();

        public void updatePlayList(GalleryApp application, String[] playlist);

        public boolean isActive();

        public boolean isDone();
    }

    public static class Slide {
        public Bitmap bitmap;
        public MediaItem item;
        public int index;

        public Slide(MediaItem item, int index, Bitmap bitmap) {
            this.bitmap = bitmap;
            this.item = item;
            this.index = index;
        }
    }

    private Handler mHandler;
    private Model mModel;
    private SlideshowView mSlideshowView;

    private Slide mPendingSlide = null;
    private boolean mIsActive = false;
    private final Intent mResultIntent = new Intent();
    private int mSlideshowTiming=5;
    private boolean mIsNaviByCommand = false;
    private String mSourceFrom;
    private String[] mPlaylist = null;
    private String[] mPlayListTitle = null;
    private boolean mBroadcastStatus = false;
    private int mStartIndex = 0;
    private String mStartItemPath = null;
    private ViewGroup mContainer;
    private ImageView mFloatHoverArea;
    private Bundle mData;
    private boolean mBgMusicEnable = false;
    private AudioBecomingNoisyReceiver mAudioBecomingNoisyReceiver;
    private Toast mToast;

    @Override
    protected int getBackgroundColorId() {
        return R.color.slideshow_background;
    }

    private final GLView mRootPane = new GLView() {
        @Override
        protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
            mSlideshowView.layout(0, 0, right - left, bottom - top);
        }

        @Override
        protected boolean onTouch(MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_UP) {
                backToPhotoPage(null);
            }
            return true;
        }

        @Override
        protected void renderBackground(GLCanvas canvas) {
            canvas.clearBuffer(getBackgroundColor());
        }
    };

    @Override
    public void onCreate(Bundle data, Bundle restoreState) {
        Log.d(TAG, "onCreate");
        super.onCreate(data, restoreState);
        mData = data;
        mFlags |= (FLAG_HIDE_ACTION_BAR | FLAG_HIDE_STATUS_BAR);
        if (data.getBoolean(KEY_DREAM)) {
            // Dream screensaver only keeps screen on for plugged devices.
            mFlags |= FLAG_SCREEN_ON_WHEN_PLUGGED | FLAG_SHOW_WHEN_LOCKED;
        } else {
            // User-initiated slideshow would always keep screen on.
            mFlags |= FLAG_SCREEN_ON_ALWAYS;
        }

        mHandler = new SynchronizedHandler(mActivity.getGLRoot()) {
            @Override
            public void handleMessage(Message message) {
                switch (message.what) {
                    case MSG_SHOW_PENDING_BITMAP:
                        showPendingBitmap();
                        break;
                    case MSG_LOAD_NEXT_BITMAP:
                        loadNextBitmap();
                        break;
                    case MSG_HIDE_SLIDESHOW_INFO:
                        showSlideshowInfo(false);
                        break;
                    default: throw new AssertionError();
                }
            }
        };
        initializeViews();
        initializeData(data);
        mContainerAnimIn.setDuration(400);
        mContainerAnimOut.setDuration(400);
        showSlideshowInfo(true);
    }

    private void loadNextBitmap() {
        if(mSourceFrom!=null && mSourceFrom.equals("Network")) {
            ConnectivityManager connManager = (ConnectivityManager)mActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo info = connManager.getActiveNetworkInfo();
            if(info==null || !info.isConnected()) {
                if(info==null)
                    Log.d(TAG, "Can not get ActiveNetworkInfo");
                else
                    Log.d(TAG, info.getType() + "is disconnected.");
                mActivity.getStateManager().finishState(SlideshowPage.this);
                return;
            }
        }

        mModel.nextSlide(new FutureListener<Slide>() {
            @Override
            public void onFutureDone(Future<Slide> future) {
                mPendingSlide = future.get();
                mHandler.sendEmptyMessage(MSG_SHOW_PENDING_BITMAP);
            }
        });
    }

    private void loadPreviousBitmap() {
        mModel.previousSlide(new FutureListener<Slide>() {
            @Override
            public void onFutureDone(Future<Slide> future) {
                mPendingSlide = future.get();
                mHandler.sendEmptyMessage(MSG_SHOW_PENDING_BITMAP);
            }
        });
    }

    private void showPendingBitmap() {
        // mPendingBitmap could be null, if
        // 1.) there is no more items
        // 2.) mModel is paused
        Slide slide = mPendingSlide;
        if (slide == null) {
            if (mIsActive && mModel.isDone()==true) {
                Log.d(TAG, "slide is null and mModel is done. Finish state.");
                mActivity.getStateManager().finishState(SlideshowPage.this);
            }
            return;
        }

        String itemPath = slide.item.getFilePath();
        // For directconnect://, check the bitmap inside the slide
        if (-1 != itemPath.indexOf("directconnect://") && null == slide.bitmap) {
            Log.w(TAG, "Null bitmap from directconnect");
            if(mToast!=null) mToast.cancel();
            mToast = Toast.makeText(mActivity.getAndroidContext(), R.string.fail_to_load_image, Toast.LENGTH_SHORT);
            mToast.show();
            mHandler.sendEmptyMessageDelayed(MSG_LOAD_NEXT_BITMAP, mSlideshowTiming*1000);
            return;
        }

        PhotoPage.UpdatePhotoRecentPlay(mActivity, slide.item.getFilePath());
        String photo_name;
        if(mPlayListTitle!=null && slide.index >=0 && slide.index < mPlayListTitle.length)
            photo_name = mPlayListTitle[slide.index];
        else
            photo_name = slide.item.getFilePath().substring(slide.item.getFilePath().lastIndexOf("/")+1);
//        GalleryUtils.updatePhotoNotification(photo_name, mActivity);
        mSlideshowView.setSlideshowDuration(mSlideshowTiming*1000);
        mSlideshowView.next(slide.bitmap, slide.item.getRotation());
        Log.d(TAG, "showImage: "+itemPath);
        setStateResult(Activity.RESULT_OK, mResultIntent
                .putExtra(KEY_ITEM_PATH, slide.item.getFilePath().toString())
                .putExtra(KEY_PHOTO_INDEX, slide.index));
        mHandler.sendEmptyMessageDelayed(MSG_LOAD_NEXT_BITMAP, mSlideshowTiming*1000);
    }

    @Override
    public void onPause() {
        Log.d(TAG, "onPause");
        mAudioBecomingNoisyReceiver.unregister();
        mIsActive = false;
        mModel.pause();
        if (null != mSlideshowView) {
            if (true == mIsNaviByCommand) {
                mSlideshowView.forceStop();
            } else {
                mSlideshowView.release();
            }
        }

        mHandler.removeMessages(MSG_LOAD_NEXT_BITMAP);
        mHandler.removeMessages(MSG_SHOW_PENDING_BITMAP);
        if( mContainer != null ) {
            mContainer.setVisibility(View.INVISIBLE);
        }

        // BG Music related:
        stopBGMusicService();
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(mActivity.getAndroidContext());
        prefs.unregisterOnSharedPreferenceChangeListener(this);
        if(DEBUG){
            Toast.makeText(mActivity.getAndroidContext(),"onPause - unregisterOnSharedPreferenceChangeListener",Toast.LENGTH_SHORT).show();
        }
        if(mBroadcastStatus==false) {
//            GalleryUtils.cancelPhotoNotification(mActivity);
        }
        super.onPause();
    }

    @Override
    public void onResume() {
        Log.d(TAG, "onResume");
        super.onResume();
        mAudioBecomingNoisyReceiver = new AudioBecomingNoisyReceiver();
        mAudioBecomingNoisyReceiver.register();
        mIsActive = true;

        if (false == mModel.isActive()) {
            mModel.resume();
        }

        if (mPendingSlide != null) {
            mHandler.sendEmptyMessageDelayed(MSG_SHOW_PENDING_BITMAP, 2000);
        } else {
            mHandler.sendEmptyMessageDelayed(MSG_LOAD_NEXT_BITMAP, 2000);
        }

        // BG music related:
        mBgMusicEnable=false;
        // 1. get BG music from saved file
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(mActivity.getAndroidContext());
        boolean val = prefs.getBoolean(PhotoPage.KEY_BG_MUSIC_PREF,false);
        if(DEBUG){
            Toast.makeText(mActivity.getAndroidContext(),"onResume - BG music setting:"+val,Toast.LENGTH_SHORT).show();
        }
        if(val){
            startBGMusicService();
        }

        // 2. register itself as sharedPreferencesChangedListener
        prefs.registerOnSharedPreferenceChangeListener(this);
    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "onDestroy");
        super.onDestroy();
    }

    private void initializeData(Bundle data) {
        boolean random = data.getBoolean(KEY_RANDOM_ORDER, false);

        // We only want to show slideshow for images only, not videos.
        String mediaPath = data.getString(KEY_SET_PATH);
//        Log.d(TAG, "input mediaPath = " + mediaPath);
        mediaPath = FilterUtils.newFilterPath(mediaPath, FilterUtils.FILTER_IMAGE_ONLY);
//        Log.d(TAG, "filter mediaPath = " + mediaPath);
        MediaSet mediaSet = mActivity.getDataManager().getMediaSet(mediaPath);
        boolean repeat = data.getBoolean(KEY_REPEAT);
        mSourceFrom = data.getString(SlideshowPage.KEY_SOURCE_FROM);
        mSlideshowTiming = data.getInt(KEY_SLIDESHOW_TIMING, 5);
        int slideshowEffect = data.getInt(KEY_EFFECT_MODE, DEFAULT_SLIDESHOW_EFFECT);
        if(mSlideshowView!=null)
            mSlideshowView.setSlideshowEffect(slideshowEffect);
        mPlaylist = data.getStringArray(KEY_PLAYLIST);
        mPlayListTitle = data.getStringArray(KEY_PLAYLIST_TITLE);
        mIsNaviByCommand = data.getBoolean(KEY_NAVI_BY_COMMAND);
        mBroadcastStatus = data.getBoolean(KEY_BROADCAST_STATUS, false);
        mStartIndex = data.getInt(KEY_PHOTO_INDEX, 0);
        mStartItemPath = data.getString(KEY_ITEM_PATH);

        if (random) {
            if (mPlaylist != null)
                mModel = new SlideshowDataAdapter(mActivity, new ShuffleSource(mActivity.getDataManager().getApp(), mPlaylist, repeat), 0, null);
            else
                mModel = new SlideshowDataAdapter(mActivity, new ShuffleSource(mediaSet, repeat), 0, null);
            setStateResult(Activity.RESULT_OK, mResultIntent.putExtra(KEY_PHOTO_INDEX, 0));
        } else {
            if (mPlaylist != null) {
                mModel = new SlideshowDataAdapter(mActivity, new SequentialSource(mActivity.getDataManager().getApp(), mPlaylist, mStartIndex, repeat), mStartIndex, mStartItemPath);
                if (-1 != mStartItemPath.indexOf("directconnect://")) {
                    mModel.setPreLoad(false);
                }
            }
            else {
                mModel = new SlideshowDataAdapter(mActivity, new SequentialSource(mediaSet, repeat), mStartIndex, mStartItemPath);
            }
            setStateResult(Activity.RESULT_OK, mResultIntent.putExtra(KEY_PHOTO_INDEX, mStartIndex));
        }
    }

    private void initializeViews() {
        mSlideshowView = new SlideshowView();
        mRootPane.addComponent(mSlideshowView);
        setContentPane(mRootPane);
        addFloatArea();
    }

    private void addFloatArea() {
        RelativeLayout galleryRoot = (RelativeLayout) ((Activity) mActivity)
            .findViewById(R.id.gallery_root);
        if(galleryRoot != null) {
            if(!mActivity.getStateManager().hasStateClass(SinglePhotoPage.class)) {
                LayoutInflater inflater = (LayoutInflater) mActivity.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                mContainer = (ViewGroup) galleryRoot.findViewById(R.id.slideshowpage_progress);
                if( mContainer == null )  {
                    mContainer = (ViewGroup)inflater.inflate(R.layout.slideshowpage_progress, galleryRoot, false);
                    galleryRoot.addView(mContainer);
                }
                if( mContainer != null ) {
                    mContainer.setVisibility(View.VISIBLE);
                    mFloatHoverArea = (ImageView)mContainer.findViewById(R.id.floatHoverArea);
                }
                if(mFloatHoverArea!=null) {
                    mFloatHoverArea.setOnKeyListener(new FloatAreaKeyListener());
                    mFloatHoverArea.setFocusable(true);
                    mFloatHoverArea.setFocusableInTouchMode(true);
                    mFloatHoverArea.requestFocus();
                }
            }
        }
    }

    private void showSlideshowInfo(Boolean enable) {
        RelativeLayout galleryRoot = (RelativeLayout) ((Activity) mActivity)
            .findViewById(R.id.gallery_root);
        if(galleryRoot != null) {
            ViewGroup slideshowContainer = (ViewGroup) galleryRoot.findViewById(R.id.slideshow);
            if(enable==true)
            {
                slideshowContainer.clearAnimation();
                mContainerAnimIn.reset();
                slideshowContainer.startAnimation(mContainerAnimIn);
                slideshowContainer.setVisibility(View.VISIBLE);
                mHandler.sendEmptyMessageDelayed(MSG_HIDE_SLIDESHOW_INFO, 4000);
            }
            else
            {
                slideshowContainer.clearAnimation();
                mContainerAnimOut.reset();
                slideshowContainer.startAnimation(mContainerAnimOut);
                slideshowContainer.setVisibility(View.INVISIBLE);
            }
        }
    }

    private class FloatAreaKeyListener implements View.OnKeyListener
    {
        @Override
        public boolean onKey(View view, int keyCode, KeyEvent event)
        {
            if(event.getAction()== KeyEvent.ACTION_DOWN)
            {
                switch(event.getKeyCode())
                {
                    case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
                    case KeyEvent.KEYCODE_MEDIA_PAUSE:
                        backToPhotoPage(null);
                        return true;
                    case KeyEvent.KEYCODE_MEDIA_STOP:
                        onBackPressed();
                        return true;
                    case KeyEvent.KEYCODE_MENU:
                        backToPhotoPage("OptionMenu");
                        return true;
                    case KeyEvent.KEYCODE_MEDIA_REPEAT:
                        backToPhotoPage("RepeatMenu");
                        return true;
                    case KeyEvent.KEYCODE_SEARCH:
                        return true;
                    default:
                        break;
                }
            }
            return false;
        }
    }

    public void backToPhotoPage(String showMenu)
    {
        if(mSourceFrom!=null && mSourceFrom.equals("DirectConnect")) {
            Log.d(TAG, "Can not switch to PhotoPage in AirFun playback");
            return;
        }

        if(mActivity.getStateManager().hasStateClass(SinglePhotoPage.class)) {
            onBackPressed();
        }
        else {
            Bundle data = new Bundle();
            if(mPendingSlide!=null) {
                data.putInt(PhotoPage.KEY_INDEX_HINT, mPendingSlide.index);
                data.putString(PhotoPage.KEY_MEDIA_ITEM_PATH,
                        mPendingSlide.item.getPath().toString());
            }
            else {
                data.putInt(PhotoPage.KEY_INDEX_HINT, mStartIndex);
                data.putString(PhotoPage.KEY_MEDIA_ITEM_PATH, GalleryUtils.getItemPathString((GalleryApp)mActivity.getApplication(), mStartItemPath));
            }
            data.putStringArray(PhotoPage.KEY_PLAYLIST, mPlaylist);
            data.putStringArray(PhotoPage.KEY_PLAYLIST_TITLE, mPlayListTitle);
            data.putString(PhotoPage.KEY_SOURCE_FROM, mData.getString(PhotoPage.KEY_SOURCE_FROM));
            data.putString(PhotoPage.KEY_MEDIA_SET_PATH, mData.getString(PhotoPage.KEY_MEDIA_SET_PATH));
            data.putBoolean(PhotoPage.KEY_BROADCAST_STATUS, data.getBoolean(KEY_BROADCAST_STATUS, false));
            if(showMenu!=null)
                data.putString(PhotoPage.KEY_SHOW_OPTION, showMenu);

            mActivity.getStateManager().switchState(this, SinglePhotoPage.class, data);
        }
    }

    public String getCurrentPhotoPath() {
        return (mPendingSlide==null)?null:mPendingSlide.item.getFilePath();
    }

    private static MediaItem findMediaItem(MediaSet mediaSet, int index) {
        for (int i = 0, n = mediaSet.getSubMediaSetCount(); i < n; ++i) {
            MediaSet subset = mediaSet.getSubMediaSet(i);
            int count = subset.getTotalMediaItemCount();
            if (index < count) {
                return findMediaItem(subset, index);
            }
            index -= count;
        }
        ArrayList<MediaItem> list = mediaSet.getMediaItem(index, 1);
        return list.isEmpty() ? null : list.get(0);
    }

    private static class ShuffleSource implements SlideshowDataAdapter.SlideshowSource {
        private static final int RETRY_COUNT = 5;
        private final MediaSet mMediaSet;
        private final Random mRandom = new Random();
        private int mOrder[] = new int[0];
        private ArrayList<MediaItem> mData = new ArrayList<MediaItem>();
        private boolean mRepeat;
        private long mSourceVersion = MediaSet.INVALID_DATA_VERSION;
        private int mLastIndex = -1;
        private GalleryUtils.MediaItemListTask mMediaItemListTask;

        public ShuffleSource(MediaSet mediaSet, boolean repeat) {
            mMediaSet = Utils.checkNotNull(mediaSet);
            mRepeat = repeat;
        }

        public ShuffleSource(GalleryApp application,String[] playlist, boolean repeat) {
            mRepeat = repeat;
            mMediaSet = null;
/*
            for (int i = 0; i < playlist.length; i++) {
                String path = GalleryUtils.getItemPathString(application, playlist[i]);
                mData.add((MediaItem)application.getDataManager().getMediaObject(path));
//                Log.d("ShuffleSource", i + ") mData.getFilePath() =" + mData.get(i).getFilePath());
            }
*/
            mData = new ArrayList(Arrays.asList(new MediaItem[playlist.length]));
            if(playlist.length>GalleryUtils.MEDIAITEM_LIST_TASK_THRESHOLD) {
                GalleryUtils.setMediaItemArray(application, playlist, 0, GalleryUtils.MEDIAITEM_LIST_TASK_THRESHOLD, mData);
                mMediaItemListTask = new GalleryUtils.MediaItemListTask(TAG, application, mData, playlist);
//                mMediaItemListTask.execute();
                mMediaItemListTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
            }
            else {
                GalleryUtils.setMediaItemArray(application, playlist, 0, playlist.length, mData);
            }
        }

        @Override
        public int findItemIndex(String path, int hint) {
            return hint;
        }
        @Override
        public int findItemIndex(Path path, int hint) {
            return hint;
        }

        @Override
        public MediaItem getMediaItem(int index) {
            if (!mRepeat && index >= mOrder.length) return null;
            if (mOrder.length == 0) return null;
            mLastIndex = mOrder[index % mOrder.length];
            MediaItem item;
            if(mMediaSet!=null)
                item = findMediaItem(mMediaSet, mLastIndex);
            else
                item = mData.get(mLastIndex);
            int retryCount = RETRY_COUNT;
            if(mMediaItemListTask!=null && mMediaItemListTask.getStatus()==AsyncTask.Status.RUNNING){
                retryCount = RETRY_COUNT*20;
            }
            for (int i = 0; i < retryCount && item == null; ++i) {
                Log.w(TAG, "fail to find image: " + mLastIndex);
                if(mMediaItemListTask!=null && mMediaItemListTask.getStatus()==AsyncTask.Status.RUNNING){
                    try {
                        Thread.sleep(100);
                    }
                    catch (InterruptedException e) {
                    }
                }
                mLastIndex = mRandom.nextInt(mOrder.length);
                if(mMediaSet!=null)
                    item = findMediaItem(mMediaSet, mLastIndex);
                else if (mMediaItemListTask.getCurrentCount() == 0) {
                    item = null;
                }
                else {
                    if(mMediaItemListTask!=null && mMediaItemListTask.getStatus()==AsyncTask.Status.RUNNING) {
                        mLastIndex = mLastIndex%mMediaItemListTask.getCurrentCount();
                    }
                    item = mData.get(mLastIndex);
                }
            }

            if(item==null) {
                Log.w(TAG, "item = null");
                if(mMediaItemListTask!=null)
                    Log.w(TAG, "mMediaItemListTask.getStatus() = " + mMediaItemListTask.getStatus());
            }
            return item;
        }

        @Override
        public long reload() {
            if (mMediaSet != null) {
                long version = mMediaSet.reload();
                if (version != mSourceVersion) {
                    mSourceVersion = version;
                    int count = mMediaSet.getTotalMediaItemCount();
                    if (count != mOrder.length) generateOrderArray(count);
                }
            }
            else {
                int count = mData.size();
                if (count != mOrder.length) generateOrderArray(count);
            }
            return mSourceVersion;
        }

        @Override
        public void stopLoad() {
            if (mMediaItemListTask != null) {
                mMediaItemListTask.cancel(true);
            }
        }

        private void generateOrderArray(int totalCount) {
            if (mOrder.length != totalCount) {
                mOrder = new int[totalCount];
                for (int i = 0; i < totalCount; ++i) {
                    mOrder[i] = i;
                }
            }
            for (int i = totalCount - 1; i > 0; --i) {
                Utils.swap(mOrder, i, mRandom.nextInt(i + 1));
            }
            if (mOrder[0] == mLastIndex && totalCount > 1) {
                Utils.swap(mOrder, 0, mRandom.nextInt(totalCount - 1) + 1);
            }
        }

        @Override
        public void addContentListener(ContentListener listener) {
            if (mMediaSet != null)
                mMediaSet.addContentListener(listener);
        }

        @Override
        public void removeContentListener(ContentListener listener) {
            if (mMediaSet != null)
                mMediaSet.removeContentListener(listener);
        }

        @Override
        public void toggleRepeat() {
            mRepeat = !mRepeat;
            Log.d(TAG, "Toggle repeat to: " + mRepeat);
        }

        @Override
        public boolean getRepeatMode() {
            return mRepeat;
        }

        @Override
        public void updatePlayList(GalleryApp application, String[] playlist, int loadIndex) {
        }

        @Override
        public int getSize() {
            if (null == mMediaSet) {
                return mData.size();
            } else {
                return 0;
            }
        }
    }

    private static class SequentialSource implements SlideshowDataAdapter.SlideshowSource {
        private static final int DATA_SIZE = 32;

        private ArrayList<MediaItem> mData = new ArrayList<MediaItem>();
        private int mDataStart = 0;
        private long mDataVersion = MediaObject.INVALID_DATA_VERSION;
        private final MediaSet mMediaSet;
        private boolean mRepeat;
        private GalleryUtils.MediaItemListTask mMediaItemListTask;
        public SequentialSource(MediaSet mediaSet, boolean repeat) {
            mMediaSet = mediaSet;
            mRepeat = repeat;
        }
		
        public SequentialSource(GalleryApp application,String[] playlist, int startIndex, boolean repeat) {
            mRepeat = repeat;
            mMediaSet = null;
/*
            for (int i = 0; i < playlist.length; i++) {
                String path = GalleryUtils.getItemPathString(application, playlist[i]);
                mData.add((MediaItem)application.getDataManager().getMediaObject(path));
//                Log.d("SequentialSource", i + ") mData.getFilePath() =" + mData.get(i).getFilePath());
            }
*/
            mData = new ArrayList(Arrays.asList(new MediaItem[playlist.length]));
            if(playlist.length>GalleryUtils.MEDIAITEM_LIST_TASK_THRESHOLD) {
                GalleryUtils.setMediaItemArray(application, playlist, startIndex, startIndex+GalleryUtils.MEDIAITEM_LIST_TASK_THRESHOLD, mData);
                mMediaItemListTask = new GalleryUtils.MediaItemListTask(TAG, application, mData, playlist);
//                mMediaItemListTask.execute();
                mMediaItemListTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
            }
            else {
                GalleryUtils.setMediaItemArray(application, playlist, 0, playlist.length, mData);
            }
        }

        @Override
        public int findItemIndex(String path, int hint) {
            return hint;
        }

        @Override
        public int findItemIndex(Path path, int hint) {
            if (mMediaSet != null)
                return mMediaSet.getIndexOfItem(path, hint);
            else
                return hint;
        }

        @Override
        public MediaItem getMediaItem(int index) {
            int dataEnd = mDataStart + mData.size();
            int count = 0;

            if (mMediaSet == null) {
                count = mData.size();
            }
            else {
                count = mMediaSet.getTotalMediaItemCount();
            }
            if (count == 0) {
                Log.d(TAG, "getMediaItem error : " + (index+1) +"/"+count);
                return null;
            }
                
            if (mRepeat) {
                index = index % count;
            }
            Log.d(TAG, "getMediaItem : " + (index+1) +"/"+count);
            if (index < mDataStart || index >= dataEnd) {
                if (mMediaSet != null) {
                    mData = mMediaSet.getMediaItem(index, DATA_SIZE);
                    mDataStart = index;
                    dataEnd = index + mData.size();
                }
            }

            MediaItem item = (index < mDataStart || index >= dataEnd) ? null : mData.get(index - mDataStart);

            if(item == null && mMediaSet != null) { // case: need to get item from SubMediaSet
                item = findMediaItem(mMediaSet, index);
            }

            if(item==null) {
                Log.w(TAG, "item = null");
                if(mMediaItemListTask!=null)
                    Log.w(TAG, "mMediaItemListTask.getStatus() = " + mMediaItemListTask.getStatus());
            }
            return item;
        }

        @Override
        public long reload() {
            if (mMediaSet != null)
            {
                long version = mMediaSet.reload();
                if (version != mDataVersion) {
                    mDataVersion = version;
                    mData.clear();
                }
			}
            return mDataVersion;
        }

        @Override
        public void stopLoad() {
            if (mMediaItemListTask != null) {
                mMediaItemListTask.cancel(true);
            }
        }

        @Override
        public void addContentListener(ContentListener listener) {
            if(mMediaSet != null)
            	mMediaSet.addContentListener(listener);
        }

        @Override
        public void removeContentListener(ContentListener listener) {
            if(mMediaSet != null)
            	mMediaSet.removeContentListener(listener);
        }

        @Override
        public void toggleRepeat() {
            mRepeat = !mRepeat;
        }

        @Override
        public boolean getRepeatMode() {
            return mRepeat;
        }

        @Override
        public void updatePlayList(GalleryApp application, String[] playlist, int loadIndex) {
            mData = new ArrayList(Arrays.asList(new MediaItem[playlist.length]));
            if(playlist.length>GalleryUtils.MEDIAITEM_LIST_TASK_THRESHOLD) {
                if(mMediaItemListTask!=null && mMediaItemListTask.getStatus()==AsyncTask.Status.RUNNING){
                    mMediaItemListTask.cancel(true);
                }
                GalleryUtils.setMediaItemArray(application, playlist, loadIndex, loadIndex+GalleryUtils.MEDIAITEM_LIST_TASK_THRESHOLD, mData);
                mMediaItemListTask = new GalleryUtils.MediaItemListTask(TAG, application, mData, playlist);
//                mMediaItemListTask.execute();
                mMediaItemListTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
            }
            else {
                GalleryUtils.setMediaItemArray(application, playlist, 0, playlist.length, mData);
            }
        }

        @Override
        public int getSize() {
            if (null == mMediaSet) {
                return mData.size();
            } else {
                return 0;
            }
        }
    }

    public void onNext() {
        Log.d(TAG, "onNext");
        mModel.pause();
        mSlideshowView.forceStop();
        mHandler.removeMessages(MSG_LOAD_NEXT_BITMAP);
        mHandler.removeMessages(MSG_SHOW_PENDING_BITMAP);
        mModel.resume();
        loadNextBitmap();
    }

    public void onPrevious() {
        Log.d(TAG, "onPrevious");
        mModel.pause();
        mSlideshowView.forceStop();
        mHandler.removeMessages(MSG_LOAD_NEXT_BITMAP);
        mHandler.removeMessages(MSG_SHOW_PENDING_BITMAP);
        mModel.resume();
        loadPreviousBitmap();
    }

    public void play() {
        Log.d(TAG, "play");
        mHandler.removeMessages(MSG_LOAD_NEXT_BITMAP);
        mHandler.removeMessages(MSG_SHOW_PENDING_BITMAP);
        mHandler.sendEmptyMessageDelayed(MSG_LOAD_NEXT_BITMAP, 1000);
    }

    public void pause() {
        Log.d(TAG, "pause");
        mHandler.removeMessages(MSG_LOAD_NEXT_BITMAP);
        mHandler.removeMessages(MSG_SHOW_PENDING_BITMAP);
    }

    public void startBGMusicService(){
        // prevent more than one playback flow.
        //if(hasLivingMusicPlayback()){
        //    return;
        //}
        // Check if android music service is foreground.
        if(hasRtkMusicPlaybackService() || mBroadcastStatus==true){
            return;
        }

        Context context = mActivity.getAndroidContext();
        if(mBgMusicEnable){
            if(mToast!=null) mToast.cancel();
            mToast = Toast.makeText(context,R.string.bg_music_is_on,Toast.LENGTH_SHORT);
            mToast.show();
            return;
        }

        int playlistSize = BGMusicService.getBGMusicFolderFileCount();
        if(playlistSize>0){
            Intent i= new Intent(context, BGMusicService.class);
            context.startService(i);
            mBgMusicEnable=true;
        }else{
            mBgMusicEnable=false;
            if(mToast!=null) mToast.cancel();
            mToast = Toast.makeText(context,R.string.bg_musid_in_na,Toast.LENGTH_SHORT);
            mToast.show();
        }
    }
    // BUG_FIX: 44245
    public boolean hasRtkMusicPlaybackService(){
        Context context = mActivity.getAndroidContext();
        ActivityManager manager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        for (RunningServiceInfo service : manager.getRunningServices(Integer.MAX_VALUE)) {
            //Log.i("BGMusic","\033[0;31;31m running service:"+service.service.getClassName()+" isforground:"+service.foreground+"\033[m");
            if ("com.android.music.MediaPlaybackService".equals(service.service.getClassName())&&service.foreground){
                Log.i("BGMusic","\033[0;31;31m MusicPlayback is running \033[m");
                return true;
            }
        }
        Log.i("BGMusic","\033[0;31;31m No MusicPlayback is running \033[m");
        return false;
    }

    public boolean hasLivingMusicPlayback(){
        Context context = mActivity.getAndroidContext();
        AudioManager mgr = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        if(mgr.isMusicActive()){
            Log.d("RTK","AudioManager is Activated");
            return true;
        }else{
            Log.d("RTK","AudioManager is not Activated");
            return false;
        }
    }

    public void stopBGMusicService(){
        if(mBgMusicEnable){
            Context context = mActivity.getAndroidContext();
            Intent i= new Intent(context, BGMusicService.class);
            context.stopService(i);
            mBgMusicEnable=false;
            if(DEBUG){
                Toast.makeText(mActivity.getAndroidContext(),"StopBGMusicService",Toast.LENGTH_SHORT).show();
            }
        }
    }

    // register sharedPreferencedChangedListener, but according to current design, it is not reachable.
    public void onSharedPreferenceChanged (SharedPreferences sharedPreferences, String key){
        if(!mIsActive) return;

        if(key.equals(PhotoPage.KEY_BG_MUSIC_PREF)){
            boolean val = sharedPreferences.getBoolean(key,false);
            if(val){
                startBGMusicService();
            }else{
                stopBGMusicService();
            }
        }
    }
    
    public void onUpdatePlaylist(String[] playlist) {
        Log.d(TAG, "onUpdatePlaylist");
        int index = mResultIntent.getIntExtra(KEY_PHOTO_INDEX, 0);
        String itemPath = this.getData().getString(KEY_ITEM_PATH);
        boolean repeat = this.getData().getBoolean(KEY_REPEAT);

        mPlaylist = playlist;
        mModel.updatePlayList(mActivity.getDataManager().getApp(), playlist);
    }
    
    public void onRestart(String[] playlist) {
        Log.d(TAG, "onRestart");
        mIsActive = false;
        mModel.pause();
        mHandler.removeMessages(MSG_LOAD_NEXT_BITMAP);
        mHandler.removeMessages(MSG_SHOW_PENDING_BITMAP);        
        mModel.setLoadIndex(0);
        mIsActive = true;
        mModel.resume();       
        loadNextBitmap();
    }

    public void onRepeat() {
        mModel.toggleRepeat();
    }

    // We want to pause when the headset is unplugged.
    private class AudioBecomingNoisyReceiver extends BroadcastReceiver {

        public void register() {
            IntentFilter intentFilter = new IntentFilter();
            intentFilter.addAction("android.intent.action.HDMI_PLUGGED");
            intentFilter.addAction(AudioManager.ACTION_AUDIO_BECOMING_NOISY);
            mActivity.getAndroidContext().registerReceiver(this, intentFilter);
        }

        public void unregister() {
            mActivity.getAndroidContext().unregisterReceiver(this);
        }

        @Override
        public void onReceive(Context context, Intent intent) {
            Log.d(TAG, "AudioBecomingNoisyReceiver: "+intent.getAction());
            boolean hdmiPlugged = intent.getBooleanExtra("state", false);
            Log.d(TAG, "HDMI plugged status: "+hdmiPlugged);

            // get setting value:
            int ignoreHDMIPlugOut=0;
            try{
                ignoreHDMIPlugOut = Settings.System.getInt(context.getContentResolver(), Settings.System.REALTEK_IGNORE_HDMI_PLUGOUT);
            }catch(android.provider.Settings.SettingNotFoundException e){
                Log.e(TAG, "setting not found for REALTEK_IGNORE_HDMI_PLUGOUT");
            }
            if(ignoreHDMIPlugOut<=0) {
                if (intent.getAction().equals(AudioManager.ACTION_AUDIO_BECOMING_NOISY)) {
                    Log.d(TAG, "Pause slideshow when audio becoming noisy");
                    backToPhotoPage(null);
                }
                else if (intent.getAction().equals("android.intent.action.HDMI_PLUGGED") && hdmiPlugged==false) {
                    Log.d(TAG, "Pause slideshow when HDMI plug out");
                    backToPhotoPage(null);
                }
            }
            else {
                Log.d(TAG, "REALTEK_IGNORE_HDMI_PLUGOUT");
            }
        }
    }
}
