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

import android.graphics.Bitmap;

import com.android.gallery3d.app.SlideshowPage.Slide;
import com.android.gallery3d.data.ContentListener;
import com.android.gallery3d.data.MediaItem;
import com.android.gallery3d.data.MediaObject;
import com.android.gallery3d.data.Path;
import com.android.gallery3d.data.UriImage;
import com.android.gallery3d.util.Future;
import com.android.gallery3d.util.FutureListener;
import com.android.gallery3d.util.ThreadPool;
import com.android.gallery3d.util.ThreadPool.Job;
import com.android.gallery3d.util.ThreadPool.JobContext;

import java.util.LinkedList;
import java.util.concurrent.atomic.AtomicBoolean;

public class SlideshowDataAdapter implements SlideshowPage.Model {
    @SuppressWarnings("unused")
    private static final String TAG = "SlideshowDataAdapter";

    private static final int IMAGE_QUEUE_CAPACITY = 3;

    public interface SlideshowSource {
        public void addContentListener(ContentListener listener);
        public void removeContentListener(ContentListener listener);
        public long reload();
        public void stopLoad();
        public MediaItem getMediaItem(int index);
        public int findItemIndex(String path, int hint);
        public int findItemIndex(Path path, int hint);
        public void toggleRepeat();
        public boolean getRepeatMode();
        public void updatePlayList(GalleryApp application, String[] playlist, int loadIndex);
        public int getSize();
    }

    private final SlideshowSource mSource;

    private int mLoadIndex = 0;
    private int mNextOutput = 0;
    private int mImageQueueCapicity = IMAGE_QUEUE_CAPACITY;
    private boolean mIsActive = false;
    private boolean mNeedReset;
    private boolean mDataReady;
    private boolean mIsPreLoad = true;
    private String mInitialPath;

    private final LinkedList<Slide> mImageQueue = new LinkedList<Slide>();

    private Future<Void> mReloadTask;
    private final ThreadPool mThreadPool;

    private long mDataVersion = MediaObject.INVALID_DATA_VERSION;
    private final AtomicBoolean mNeedReload = new AtomicBoolean(false);
    private final SourceListener mSourceListener = new SourceListener();

    // The index is just a hint if initialPath is set
    public SlideshowDataAdapter(GalleryContext context, SlideshowSource source, int index,
            String initialPath) {
        mSource = source;
        mInitialPath = initialPath;
        mLoadIndex = index;
        mNextOutput = index;
        mThreadPool = context.getThreadPool();
    }

    private MediaItem loadItem() {
        if (mNeedReload.compareAndSet(true, false)) {
            long v = mSource.reload();
            if (v != mDataVersion) {
                mDataVersion = v;
                mNeedReset = true;
                return null;
            }
        }
        int index = mLoadIndex;
        if (mInitialPath != null) {
//            Log.d(TAG, "mInitialPath = " + mInitialPath);
            index = mSource.findItemIndex(mInitialPath, index);
//            Log.d(TAG, "findindex = " + index);
            if (index != -1)
                mLoadIndex = index;
            mInitialPath = null;
        }
        return mSource.getMediaItem(mLoadIndex);
    }

    private class ReloadTask implements Job<Void> {
        @Override
        public Void run(JobContext jc) {
            while (true) {
                synchronized (SlideshowDataAdapter.this) {
                    while (mIsActive && (!mDataReady
                            || mImageQueue.size() >= mImageQueueCapicity)) {
                        try {
                            SlideshowDataAdapter.this.wait();
                        } catch (InterruptedException ex) {
                            // ignored.
                        }
                        continue;
                    }
                }
                if (!mIsActive) return null;
                mNeedReset = false;

                MediaItem item = loadItem();

                if (mNeedReset) {
                    synchronized (SlideshowDataAdapter.this) {
                        mImageQueue.clear();
                        mLoadIndex = mNextOutput;
                    }
                    continue;
                }

                if (item == null) {
                    synchronized (SlideshowDataAdapter.this) {
                        if (!mNeedReload.get()) mDataReady = false;
                        SlideshowDataAdapter.this.notifyAll();
                    }
                    continue;
                }
                String itemPath = item.getFilePath();
                if (false == mIsPreLoad) {
                    item.getPath().clearObject();
                    //Log.d(TAG, "item path = " + itemPath);

                    if (-1 != itemPath.indexOf("directconnect://")) {
                        UriImage urimage = (UriImage)item;
                        urimage.resetState();
                    }
                }
                if (!mIsActive) return null;

                if(-1 != itemPath.indexOf("directconnect://")) {
                    if(Math.abs(mLoadIndex-mNextOutput)>0) {
                        mLoadIndex = mNextOutput;
                        continue;
                    }
                }

//                Log.d(TAG, "itemPath = " + item.getFilePath());
                Bitmap bitmap = item
                        .requestImage(MediaItem.TYPE_THUMBNAIL)
                        .run(jc);

                // For directconnect, add new slide even if the bitmap is null.
                // Null bitmap will be checked and prompted in SlideShowPage's showPendingBitmap()
                if (bitmap != null || -1 != itemPath.indexOf("directconnect://")) {
                    synchronized (SlideshowDataAdapter.this) {
                        mImageQueue.addLast(
                                new Slide(item, mLoadIndex, bitmap));
                        if (mImageQueue.size() == 1) {
                            SlideshowDataAdapter.this.notifyAll();
                        }
                    }
                }
                if (!mIsActive) return null;

                ++mLoadIndex;
                if(mSource.getRepeatMode()==true) {
                    int size = mSource.getSize();
                    if (0 != size) mLoadIndex = mLoadIndex % size;
                }
                if(-1 == itemPath.indexOf("directconnect://")) {
                    if(Math.abs(mLoadIndex-mNextOutput)>0) {
                        try {
                            Thread.sleep(500);
                        }
                        catch (InterruptedException e) {
                        }
                    }
                }
                if (!mIsActive) return null;
            }
        }
    }

    private class SourceListener implements ContentListener {
        @Override
        public void onContentDirty() {
            synchronized (SlideshowDataAdapter.this) {
                mNeedReload.set(true);
                mDataReady = true;
                SlideshowDataAdapter.this.notifyAll();
            }
        }
    }

    private synchronized Slide innerNextBitmap() {
        while (mIsActive && mDataReady && mImageQueue.isEmpty()) {
            try {
                if (false == mIsPreLoad) {
                    this.notifyAll();
                }
                wait();
            } catch (InterruptedException t) {
                throw new AssertionError();
            }
        }
        if (mImageQueue.isEmpty()) return null;
        mNextOutput++;
        if(mSource.getRepeatMode()==true) {
            int size = mSource.getSize();
            if (0 != size) mNextOutput = mNextOutput % size;
        }
        if (true == mIsPreLoad) {
            this.notifyAll();
        }
        return mImageQueue.removeFirst();
    }

    @Override
    public Future<Slide> nextSlide(FutureListener<Slide> listener) {
        //Log.d(TAG, "nextSlide");
        return mThreadPool.submit(new Job<Slide>() {
            @Override
            public Slide run(JobContext jc) {
                jc.setMode(ThreadPool.MODE_NONE);
                return innerNextBitmap();
            }
        }, listener);
    }

    @Override
    public Future<Slide> previousSlide(FutureListener<Slide> listener) {
        //Log.d(TAG, "previousSlide");
        mLoadIndex -= 2;
        if (mLoadIndex < 0) {
            mLoadIndex = 0;
        }
        mNextOutput = mLoadIndex;
        return mThreadPool.submit(new Job<Slide>() {
            @Override
            public Slide run(JobContext jc) {
                jc.setMode(ThreadPool.MODE_NONE);
                return innerNextBitmap();
            }
        }, listener);
    }

    @Override
    public void pause() {
        synchronized (this) {
            mIsActive = false;
            notifyAll();
        }
        if(mSource!=null) {
            mSource.stopLoad();
            mSource.removeContentListener(mSourceListener);
        }
        if (null != mReloadTask) {
            mReloadTask.cancel();
            mReloadTask.waitDone();
            mReloadTask = null;
        }
    }

    @Override
    public synchronized void resume() {
        mIsActive = true;
        mSource.addContentListener(mSourceListener);
        mNeedReload.set(true);
        mDataReady = true;
        mReloadTask = mThreadPool.submit(new ReloadTask());
    }

    @Override
    public void setPreLoad(boolean bPreload) {
        Log.d(TAG, "setPreLoad:" + bPreload);
        mIsPreLoad = bPreload;
        mImageQueueCapicity = 1; // Restrict image queue capacity to one.
    }

    @Override
    public void setLoadIndex(int index) {
        mLoadIndex  = index;
        if (mLoadIndex < 0) {
            mLoadIndex = 0;
        }
        mNextOutput = mLoadIndex;
    }

    @Override
    public void toggleRepeat() {
        mSource.toggleRepeat();
    }

    @Override
    public void updatePlayList(GalleryApp application, String[] playlist) {
        if(mSource!=null) {
            int size = mSource.getSize();
            if (0 != size) mLoadIndex = mLoadIndex % size;
            mSource.updatePlayList(application, playlist, mLoadIndex);
        }
    }

    @Override
    public boolean isActive() {
        return mIsActive;
    }

    @Override
    public boolean isDone() {
        if(mSource.getRepeatMode()==false) {
            int size = mSource.getSize();
            if (0 != size && mNextOutput>=size) return true;
        }
        return false;
    }
}
