package com.rtk.mediabrowser;

import android.app.SearchManager;
import android.content.*;
import android.database.Cursor;
import android.database.MatrixCursor;
import android.graphics.Bitmap;
import android.net.Uri;
import android.provider.BaseColumns;
import android.provider.MediaStore;
import android.text.TextUtils;
import android.util.Log;
import com.bumptech.glide.Glide;
import com.bumptech.glide.load.engine.DiskCacheStrategy;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.concurrent.ExecutionException;

/**
 * Created by archerho on 2017/4/20.
 */
public class VideoContentProvider extends ContentProvider {
    private static final boolean DEBUG = true;
    private static final String TAG = "VideoContentProvider";
    //The columns we'll include in the video database table
    public static final String KEY_NAME = SearchManager.SUGGEST_COLUMN_TEXT_1; // (required)
    public static final String KEY_DATA_TYPE = SearchManager.SUGGEST_COLUMN_CONTENT_TYPE; // (required)
    public static final String KEY_PRODUCTION_YEAR = SearchManager.SUGGEST_COLUMN_PRODUCTION_YEAR;// (required)
    public static final String KEY_COLUMN_DURATION = SearchManager.SUGGEST_COLUMN_DURATION;
    public static final String KEY_DESCRIPTION = SearchManager.SUGGEST_COLUMN_TEXT_2;
    public static final String KEY_ICON = SearchManager.SUGGEST_COLUMN_RESULT_CARD_IMAGE;
    public static final String KEY_IS_LIVE = SearchManager.SUGGEST_COLUMN_IS_LIVE;
    public static final String KEY_VIDEO_WIDTH = SearchManager.SUGGEST_COLUMN_VIDEO_WIDTH;
    public static final String KEY_VIDEO_HEIGHT = SearchManager.SUGGEST_COLUMN_VIDEO_HEIGHT;
    public static final String KEY_AUDIO_CHANNEL_CONFIG =
            SearchManager.SUGGEST_COLUMN_AUDIO_CHANNEL_CONFIG;
    public static final String KEY_PURCHASE_PRICE = SearchManager.SUGGEST_COLUMN_PURCHASE_PRICE;
    public static final String KEY_RENTAL_PRICE = SearchManager.SUGGEST_COLUMN_RENTAL_PRICE;
    public static final String KEY_RATING_STYLE = SearchManager.SUGGEST_COLUMN_RATING_STYLE;
    public static final String KEY_RATING_SCORE = SearchManager.SUGGEST_COLUMN_RATING_SCORE;
    public static final String KEY_ACTION = SearchManager.SUGGEST_COLUMN_INTENT_ACTION;
    private static UriMatcher URI_MATCHER = new UriMatcher(UriMatcher.NO_MATCH);

    private static final int QUERY_VIDEO = 0;
    private static final int QUERY_AUDIO = 1;
    private static final int QUERY_IMAGE = 2;

    private static final String AUTHORITY = "com.rtk.mediabrowser";
    private static final int SEARCH_SUGGEST = 0;

    static{
        // For suggestions table
        URI_MATCHER.addURI(AUTHORITY, SearchManager.SUGGEST_URI_PATH_QUERY,
                SEARCH_SUGGEST);
        URI_MATCHER.addURI(AUTHORITY, SearchManager.SUGGEST_URI_PATH_QUERY + "/*",
                SEARCH_SUGGEST);
    }

    @Override
    public boolean onCreate() {
        log("onCreate");
        return true;
    }

    /*
    04-20 16:49:16.879 31346 31372 E DatabaseUtils: java.lang.IllegalArgumentException: Unknown Uri: content://com.rtk.mediabrowser.videosuggest/search_suggest_query?limit=10
04-20 16:49:16.884 31893 32375 E SearchableSource: SearchableSource[name=com.rtk.mediabrowser/.PreViewActivity, canonicalName=com.rtk.mediabrowser][abc] failed.
04-20 16:49:16.884 31893 32375 E SearchableSource: java.lang.IllegalArgumentException: Unknown Uri: content://com.rtk.mediabrowser.videosuggest/search_suggest_query?limit=10
     */
    @Override
    public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs,
                        String sortOrder) {
// Use the UriMatcher to see what kind of query we have and format the db query accordingly
        log("query uri="+uri);
        switch (URI_MATCHER.match(uri)) {
            case SEARCH_SUGGEST:
                log("search suggest: " + selectionArgs[0] + " URI: " + uri);
                if (selectionArgs == null) {
                    throw new IllegalArgumentException(
                            "selectionArgs must be provided for the Uri: " + uri);
                }
                return getSuggestions(selectionArgs[0]);
            default:
                log("unknwon uri");
                throw new IllegalArgumentException("Unknown Uri: " + uri);
        }
    }

    private Cursor getSuggestions(String query) {
        query = query.toLowerCase();
        String[] columns = new String[]{
                BaseColumns._ID,
                KEY_NAME,
                KEY_DATA_TYPE,
                KEY_PRODUCTION_YEAR,
                KEY_COLUMN_DURATION,
                SearchManager.SUGGEST_COLUMN_RESULT_CARD_IMAGE,
                SearchManager.SUGGEST_COLUMN_INTENT_DATA,
//                SearchManager.SUGGEST_COLUMN_INTENT_ACTION
//                KEY_DESCRIPTION,
//                KEY_IS_LIVE,
//                KEY_VIDEO_WIDTH,
//                KEY_VIDEO_HEIGHT,
//                KEY_AUDIO_CHANNEL_CONFIG,
//                KEY_PURCHASE_PRICE,
//                KEY_RENTAL_PRICE,
//                KEY_RATING_STYLE,
//                KEY_RATING_SCORE,
//                SearchManager.SUGGEST_COLUMN_INTENT_DATA_ID
        };
        MatrixCursor matrixCursor = new MatrixCursor(columns);
        getVideoMatch(query, matrixCursor);
        getImageMatch(query, matrixCursor);
        getAudioMatch(query, matrixCursor);
        if(matrixCursor.getCount()>0)
            return matrixCursor;
        return null;
    }

    private boolean saveBitmap(Bitmap bmp, String filename){
        FileOutputStream out = null;
        boolean ret = false;
        try {
            out = new FileOutputStream(filename);
            bmp.compress(Bitmap.CompressFormat.PNG, 100, out); // bmp is your Bitmap instance
            // PNG is a lossless format, the compression factor (100) is ignored
            ret = true;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (out != null) {
                    out.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return ret;
    }

    private void getVideoMatch(String query, MatrixCursor matrixCursor) {
        Cursor queryCursor = queryMediaStore(QUERY_VIDEO, query);
        if(queryCursor==null) return;
        if(queryCursor.getCount()>0) {
            while (queryCursor.moveToNext()) {
                String id = queryCursor.getString(queryCursor.getColumnIndex(MediaStore.MediaColumns._ID));
                String fileData = queryCursor.getString(queryCursor.getColumnIndex(MediaStore.MediaColumns.DATA));
                String mimeType = queryCursor.getString(queryCursor.getColumnIndex(MediaStore.MediaColumns.MIME_TYPE));
                String title= queryCursor.getString(queryCursor.getColumnIndex(MediaStore.Video.VideoColumns.TITLE));
                log("getVideoMatch mimeType="+mimeType);
                //prepare card image
                String thumbnailPath = getContext().getExternalCacheDir()+"/"+id+".png";
                if(!new File(thumbnailPath).exists()) {
                    try {
                        Bitmap bitmap = Glide.with(getContext())
                                .load(Uri.fromFile(new File(fileData)))
                                .asBitmap()
                                .diskCacheStrategy(DiskCacheStrategy.RESULT)
                                .into(400, 225)
                                .get();

                        if(!saveBitmap(bitmap, thumbnailPath))
                            thumbnailPath="";
                        log("thumbnailPath=" + thumbnailPath);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                        thumbnailPath = "";
                    } catch (ExecutionException e) {
                        e.printStackTrace();
                        thumbnailPath = "";
                    }
                }

//                Uri thumbnailUri = Uri.fromFile(new File(thumbnailPath));

                Uri thumbnailUri = Util.getUri(getContext(), thumbnailPath);
                log("thumbnailUri="+thumbnailUri);
                grantReadPermission(getContext(), "com.google.android.katniss", new String[]{thumbnailUri.toString()});
                Uri fileUri = Util.getUri(getContext(), fileData);
                grantReadPermission(getContext(), "com.google.android.katniss", new String[]{fileUri.toString()});

                matrixCursor.addRow(
                        new Object[]{
                                id,
                                title,
                                mimeType,
                                0,
                                0,
                                thumbnailUri,
                                fileData
                        });
            }
        }
        queryCursor.close();
    }

    private void getImageMatch(String query, MatrixCursor matrixCursor) {
        Cursor queryCursor = queryMediaStore(QUERY_IMAGE, query);
        if(queryCursor==null) return;
        if(queryCursor.getCount()>0) {
            while (queryCursor.moveToNext()) {
                String id = queryCursor.getString(queryCursor.getColumnIndex(MediaStore.MediaColumns._ID));
                String fileData = queryCursor.getString(queryCursor.getColumnIndex(MediaStore.MediaColumns.DATA));
                String mimeType = queryCursor.getString(queryCursor.getColumnIndex(MediaStore.MediaColumns.MIME_TYPE));
                String title= queryCursor.getString(queryCursor.getColumnIndex(MediaStore.Images.ImageColumns.TITLE));
                log("getImageMatch mimeType="+mimeType);
                //prepare card image
                String thumbnailPath = getContext().getExternalCacheDir()+"/"+id+".png";
                if(!new File(thumbnailPath).exists()) {
                    try {
                        Bitmap bitmap = Glide.with(getContext())
                                .load(Uri.fromFile(new File(fileData)))
                                .asBitmap()
                                .diskCacheStrategy(DiskCacheStrategy.RESULT)
                                .into(400, 225)
                                .get();

                        if(!saveBitmap(bitmap, thumbnailPath))
                            thumbnailPath="";
                        log("thumbnailPath=" + thumbnailPath);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                        thumbnailPath = "";
                    } catch (ExecutionException e) {
                        e.printStackTrace();
                        thumbnailPath = "";
                    }
                }

//                Uri thumbnailUri = Uri.fromFile(new File(thumbnailPath));
                Uri thumbnailUri = Util.getUri(getContext(), thumbnailPath);
                log("thumbnailUri="+thumbnailUri);
                grantReadPermission(getContext(), "com.google.android.katniss", new String[]{thumbnailUri.toString()});
                Uri fileUri = Util.getUri(getContext(), fileData);
                grantReadPermission(getContext(), "com.google.android.katniss", new String[]{fileUri.toString()});

                matrixCursor.addRow(
                        new Object[]{
                                id,
                                title,
                                mimeType,
                                0,
                                0,
                                thumbnailUri,
                                fileData
                        });
            }
        }
        queryCursor.close();
    }

    private void getAudioMatch(String query, MatrixCursor matrixCursor) {
        Cursor queryCursor = queryMediaStore(QUERY_AUDIO, query);
        if(queryCursor==null) return;
        if(queryCursor.getCount()>0) {
            while (queryCursor.moveToNext()) {
                String id = queryCursor.getString(queryCursor.getColumnIndex(MediaStore.MediaColumns._ID));
                String fileData = queryCursor.getString(queryCursor.getColumnIndex(MediaStore.MediaColumns.DATA));
                String mimeType = queryCursor.getString(queryCursor.getColumnIndex(MediaStore.MediaColumns.MIME_TYPE));
                String title= queryCursor.getString(queryCursor.getColumnIndex(MediaStore.Audio.AudioColumns.TITLE));
                log("getAudioMatch mimeType="+mimeType);
                //prepare card image
                String thumbnailPath = "";
//                String thumbnailPath = getContext().getExternalCacheDir()+"/"+id+".png";
//                if(!new File(thumbnailPath).exists()) {
//                    try {
//                        Bitmap bitmap = Glide.with(getContext())
//                                .load(Uri.fromFile(new File(fileData)))
//                                .asBitmap()
//                                .diskCacheStrategy(DiskCacheStrategy.RESULT)
//                                .into(400, 225)
//                                .get();
//
//                        if(!saveBitmap(bitmap, thumbnailPath))
//                            thumbnailPath="";
//                        log("thumbnailPath=" + thumbnailPath);
//                    } catch (InterruptedException e) {
//                        e.printStackTrace();
//                        thumbnailPath = "";
//                    } catch (ExecutionException e) {
//                        e.printStackTrace();
//                        thumbnailPath = "";
//                    }
//                }

//                Uri thumbnailUri = Uri.fromFile(new File(thumbnailPath));
                Uri thumbnailUri = Util.getUri(getContext(), thumbnailPath);
                log("thumbnailUri="+thumbnailUri);
                grantReadPermission(getContext(), "com.google.android.katniss", new String[]{thumbnailUri.toString()});
                Uri fileUri = Util.getUri(getContext(), fileData);
                grantReadPermission(getContext(), "com.google.android.katniss", new String[]{fileUri.toString()});

                matrixCursor.addRow(
                        new Object[]{
                                id,
                                title,
                                mimeType,
                                0,
                                0,
                                thumbnailUri,
                                fileData
                        });
            }
        }
        queryCursor.close();
    }

    private void grantReadPermission(Context ctx, String pkgName, String[] urls) {
        if(urls==null) return;
        Log.d(TAG, "grantReadPermission packageName="+pkgName);
        for(String url : urls) {
            Uri uri = Uri.parse(url);
            ctx.grantUriPermission(pkgName, uri, Intent.FLAG_GRANT_READ_URI_PERMISSION);
        }
    }

    private Cursor queryMediaStore(int queryType, String query) {
        Context ctx = getContext();

        String[] projection = new String[]{
                MediaStore.MediaColumns._ID,
                MediaStore.MediaColumns.DATA,
                MediaStore.MediaColumns.MIME_TYPE,
                MediaStore.Video.VideoColumns.TITLE
        };
        String selection = null;
        String[] selectionArgs = null;
        String[] querys = query.split(" ");
        if (querys != null && querys.length > 0) {
            selection = MediaStore.Files.FileColumns.DATA + " like ?";
            selectionArgs =
                    new String[]{"%" + TextUtils.join("%", querys) + "%"}; // there is no ? in selection so null here
        }

//        Uri qUri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
        Uri qUri;
        switch(queryType){
            case QUERY_AUDIO:
                qUri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                break;
            case QUERY_IMAGE:
                qUri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
                break;
            default:
                qUri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
                break;
        }

        String order = MediaStore.MediaColumns.DATE_MODIFIED;

        Cursor cursor = ctx.getContentResolver()
                .query(qUri,
                        projection,
                        selection,
                        selectionArgs,
                        order + " DESC");
        if (cursor == null) {
            log("no data");
            return null;
        }
        ArrayList<String> ret = null;
        log("begin cursor cursor.getCount()=" + cursor.getCount());
        if (cursor.getCount() > 0) {
            return cursor;
//                ret = new ArrayList<String>();
//                while (cursor.moveToNext()) {
//                    String data = cursor.getString(0);
//                    ret.add(data);
//                }
        }
        log("end cursor");
        return null;
    }

    private void log(String s) {
        if(DEBUG) Log.d(TAG, s);
    }

    @Override
    public String getType(Uri uri) {
        log("getType");
        return SearchManager.SUGGEST_MIME_TYPE;
    }

    @Override
    public Uri insert(Uri uri, ContentValues contentValues) {
        return null;
    }

    @Override
    public int delete(Uri uri, String s, String[] strings) {
        return 0;
    }

    @Override
    public int update(Uri uri, ContentValues contentValues, String s, String[] strings) {
        return 0;
    }
}
