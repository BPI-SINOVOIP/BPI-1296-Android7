/*
 * Copyright (c) 2015 The Android Open Source Project
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

package com.realtek.recommendation;

import android.app.IntentService;
import android.app.Notification;
import android.app.NotificationManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.app.recommendation.ContentRecommendation;
import android.util.Log;
import com.bumptech.glide.Glide;
import com.bumptech.glide.load.data.DataFetcher;
import com.bumptech.glide.load.engine.DiskCacheStrategy;
import com.bumptech.glide.load.model.ModelLoader;
import com.bumptech.glide.load.model.stream.StreamModelLoader;
import com.bumptech.glide.load.resource.drawable.GlideDrawable;
import com.realtek.Utils.UseRtMediaPlayer;
import com.rtk.mediabrowser.R;
import com.rtk.mediabrowser.Util;

//import com.bumptech.glide.Glide;

import java.io.File;
import java.io.InputStream;
import java.util.concurrent.ExecutionException;

import static com.rtk.mediabrowser.AbstractFBViewActivity.MEDIA_BROWSER_USE_RT_MEDIA_PLAYER;
import com.realtek.bitmapfun.util.*;
import static com.rtk.mediabrowser.GenericContentViewFragment.IMAGE_CACHE_DIR;
/*
 * This class builds up to MAX_RECOMMENDATIONS of ContentRecommendations and defines what happens
 * when they're selected from Recommendations section on the Home screen by creating an Intent.
 */
public class UpdateRecommendationsService extends IntentService {
    private static final String TAG = "MediaBrowserRecommendation";
    private static final boolean DEBUG = true;
    private static final int MAX_RECOMMENDATIONS = 3;
//    private static final VideoCursorMapper mVideoCursorMapper = new VideoCursorMapper();

    private NotificationManager mNotifManager;
    private PhotoLoader mModelLoader = new PhotoLoader();
    private ImageWorker mImageWorker;
    public class PhotoLoader implements ModelLoader<String, Bitmap> {

        @Override
        public DataFetcher<Bitmap> getResourceFetcher(String s, int i, int i1) {
            return null;
        }
    }

    public UpdateRecommendationsService() {
        super(TAG);
    }

    @Override
    public void onCreate() {
        super.onCreate();

        if (mNotifManager == null) {
            mNotifManager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        }
        mImageWorker = new ImageFetcher(getApplication(), null, 300, 200);
        mImageWorker.setImageCache(ImageCache.findOrCreateCache(getApplication(), IMAGE_CACHE_DIR));
        mImageWorker.setImageFadeIn(false);
    }

    private static final boolean DISABLE_RECOMMENDATION = true;
    @Override
    protected void onHandleIntent(Intent intent) {
        log("onHandleIntent");
        // Generate recommendations, but only if recommendations are enabled
        if (DISABLE_RECOMMENDATION) {
            log("Recommendations disabled");
            mNotifManager.cancelAll();
            return;
        }
        ContentRecommendation.Builder builder = new ContentRecommendation.Builder()
                .setBadgeIcon(R.drawable.icon_allfile_v);
//
        Cursor cursor = getContentResolver().query(
                MediaStore.Video.Media.EXTERNAL_CONTENT_URI,
                null, // projection
                null, // selection
                null, // selection clause
                "RANDOM() LIMIT " + MAX_RECOMMENDATIONS // sort order
        );
//
        if (cursor != null && cursor.moveToNext()) {
//            try {
                do {

//                    Video video = (Video) mVideoCursorMapper.convert(cursor);
                    int id = cursor.getInt(cursor.getColumnIndex(MediaStore.Video.VideoColumns._ID));
                    String title = cursor.getString(cursor.getColumnIndex(MediaStore.Video.VideoColumns.DISPLAY_NAME));
                    String filePath = cursor.getString(cursor.getColumnIndex(MediaStore.Video.VideoColumns.DATA));
                    String coverPath = cursor.getString(cursor.getColumnIndex(MediaStore.Video.VideoColumns.COVER_PATH));

                    if (title != null) {
                        builder.setIdTag("Video" + id)
                                .setTitle(title)
                                .setText("Popular videos")
                                .setContentIntentData(ContentRecommendation.INTENT_TYPE_ACTIVITY,
                                        buildPendingIntent(filePath, id), 0, null);

//                    Bitmap bitmap = Glide.with(getApplication())
//                            .using(mModelLoader, Bitmap.class)
//                            .load(filePath)
//                            .load(Uri.fromFile( new File( filePath ) ))
//                            .asBitmap()
//                            .diskCacheStrategy(DiskCacheStrategy.RESULT)
//                            .into(300, 200) // Only use for synchronous .get()
//                            .get();
                        Bitmap bitmap = mImageWorker.loadRecommendationImage(id, title, filePath, coverPath);
                        if (bitmap != null) {
                            builder.setContentImage(bitmap);

                            // Create an object holding all the information used to recommend the content.
                            ContentRecommendation rec = builder.build();
                            Notification notification = rec.getNotificationObject(getApplicationContext());

                            log("Recommending video " + title);

                            // Recommend the content by publishing the notification.
                            mNotifManager.notify(id, notification);
                        }
                    }
                } while (cursor.moveToNext());
//            } catch (InterruptedException e) {
//                e.printStackTrace();
//            } catch (ExecutionException e) {
//                e.printStackTrace();
//            } finally {
                cursor.close();
//            }
        } else {
            log("no cursor data");
        }
    }

    private void log(String s) {
        if(DEBUG)
            Log.d(TAG, s);
    }

    private Intent buildPendingIntent(String videoPath, int id) {
//        Intent detailsIntent = new Intent(this, VideoDetailsActivity.class);
//        detailsIntent.putExtra(VideoDetailsActivity.VIDEO, videoPath);
//        detailsIntent.putExtra(VideoDetailsActivity.NOTIFICATION_ID, id);
//        detailsIntent.setAction(Long.toString(video.id));
//
//        return detailsIntent;

        final Intent intentGallery = new Intent();
        Uri uri = Util.getUri(this, videoPath);
        ComponentName comp = new ComponentName("com.android.gallery3d", "com.android.gallery3d.app.MovieActivity");
        intentGallery.setComponent(comp);
        intentGallery.putExtra("SourceFrom","Local");
        intentGallery.putExtra(MEDIA_BROWSER_USE_RT_MEDIA_PLAYER, UseRtMediaPlayer.getInstance(this).getUseRtPlayer());
        intentGallery.setAction(Intent.ACTION_VIEW);
        intentGallery.setData(uri);
//        intentGallery.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK| Intent.FLAG_ACTIVITY_NO_ANIMATION);
        return intentGallery;
    }
}
