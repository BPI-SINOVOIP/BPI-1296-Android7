package com.android.music.content;

import android.content.ContentProvider;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.net.Uri;
import android.util.Log;

public class ContentProviderMusicUrlPath extends ContentProvider {
    private static final String TAG = "MusicUrlPathProvider";
    private DBOpenHelper dbOpenHelper;

    private static final UriMatcher MATCHER = new UriMatcher(UriMatcher.NO_MATCH);
    private static final int PATHS = 1;
    private static final int PATH = 2;
    static{
        MATCHER.addURI("com.android.music.ContentProvider.MusicUrlPathprovider", "urlpath", PATHS);
        MATCHER.addURI("com.android.music.ContentProvider.MusicUrlPathprovider", "urlpath/#", PATH);
    }
    @Override
    public int delete(Uri uri, String selection, String[] selectionArgs) {
        SQLiteDatabase db = dbOpenHelper.getWritableDatabase();
        int count = 0;
        switch (MATCHER.match(uri)) {
        case PATHS:
            count = db.delete("urlpath", selection, selectionArgs);
            return count;
        case PATH:
            long id = ContentUris.parseId(uri);
            String where = "id = "+ id;
            if(selection!=null && !"".equals(selection)){
                where = selection + " and " + where;
            }
            count = db.delete("urlpath", where, selectionArgs);
            return count;
        default:
            throw new IllegalArgumentException("Unkwon Uri:"+ uri.toString());
        }
    }

    @Override
    public String getType(Uri uri) {
        switch (MATCHER.match(uri)) {
        case PATHS:
            return "vnd.android.cursor.dir/urlpath";

        case PATH:
            return "vnd.android.cursor.item/urlpath";

        default:
            throw new IllegalArgumentException("Unkwon Uri:"+ uri.toString());
        }
    }

    @Override
    public Uri insert(Uri uri, ContentValues values) {
        Log.d(TAG,"insert uri="+uri);
        SQLiteDatabase db = dbOpenHelper.getWritableDatabase();
        switch (MATCHER.match(uri)) {
        case PATHS:
            long rowid = db.insert("urlpath", "path", values);
            Uri insertUri = ContentUris.withAppendedId(uri, rowid);
            this.getContext().getContentResolver().notifyChange(uri, null);
            return insertUri;

        default:
            throw new IllegalArgumentException("Unkwon Uri:"+ uri.toString());
        }
    }

    @Override
    public boolean onCreate() {
        this.dbOpenHelper = new DBOpenHelper(this.getContext());
        return false;
    }

    @Override
    public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs,
            String sortOrder) {
        Log.d(TAG, "query uri="+uri);
        SQLiteDatabase db = dbOpenHelper.getReadableDatabase();
        switch (MATCHER.match(uri)) {
        case PATHS:
            return db.query("urlpath", projection, selection, selectionArgs, null, null, sortOrder);
        case PATH:
            long id = ContentUris.parseId(uri);
            String where = "id = "+ id;
            if(selection!=null && !"".equals(selection)){
                where = selection + " and " + where;
            }
            return db.query("urlpath", projection, where, selectionArgs, null, null, sortOrder);
        default:
            throw new IllegalArgumentException("Unkwon Uri:"+ uri.toString());
        }
    }

    @Override
    public int update(Uri uri, ContentValues values, String selection, String[] selectionArgs) {
        Log.d(TAG, "update uri="+uri);
        SQLiteDatabase db = dbOpenHelper.getWritableDatabase();
        int count = 0;
        switch (MATCHER.match(uri)) {
        case PATH:
            long id = ContentUris.parseId(uri);
            Log.d(TAG, "update uri="+uri+" PATH id="+id);
            String where = "id = "+ id;
            if(selection!=null && !"".equals(selection)){
                where = selection + " and " + where;
            }
            count = db.update("urlpath", values, where, selectionArgs);
            return count;
        default:
            throw new IllegalArgumentException("Unkwon Uri:"+ uri.toString());
        }
    }
}