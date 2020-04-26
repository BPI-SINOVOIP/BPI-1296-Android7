package com.realtek.server;

import android.util.Slog;
import android.content.Context;
import android.content.ComponentName;
import android.content.ContentUris;
import android.content.ContentResolver;
import android.os.IBinder;
import android.net.Uri;

import android.database.Cursor;
import android.content.Intent;
import android.content.ContentResolver;
import android.provider.MediaStore;
import android.webkit.MimeTypeMap;
import android.media.MediaMetadataRetriever;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.MediaFile;
import android.text.TextUtils;

import org.json.JSONObject;
import org.json.JSONArray;
import org.json.JSONException;
import com.realtek.server.JSONTAG;
import com.realtek.provider.IMediaQueryService;

import java.io.File;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.security.InvalidParameterException;

public class MediaQueryService extends IMediaQueryService.Stub {
	
	public static final String TAG = "MediaQueryService";	

	private static final int 	JSON_OUTPUT_INDENTS = 4;

	private static final int CONTENT_TYPE_MUSIC	= 0;
	private static final int CONTENT_TYPE_PHOTO	= 1;
	private static final int CONTENT_TYPE_VIDEO	= 2;
            
	private static final int FILTER_FLAG_NONE	= 0;
	private static final int FILTER_FLAG_ALBUM	= 1;
	private static final int FILTER_FLAG_ARTIST = 2;
	private static final int FILTER_FLAG_GENRE	= 3;		

    private static final boolean LOCAL_LOGV = true;

    /**
     * Binder context for this service
     */
    private Context mContext;

    /**
     * Constructs a new SettingsService instance.
     * 
     */
    public MediaQueryService(Context context) {
        mContext = context;
        Slog.w(TAG, "MediaQueryService is constructed!");
    }

    @Override
    public String queryMusic(int nOffset, int nMax, int nTarget, int nId) {
        Slog.v(TAG,  "queryMusic, offset = "+nOffset+",max = "+nMax);
        Slog.v(TAG,  "queryMusic, target = "+nTarget+",id = "+nId);

		int nNumItems = nMax;
		Uri uri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
		String[] projection = {
			MediaStore.Audio.Media._ID,
			MediaStore.Audio.Media.ARTIST,
			MediaStore.Audio.Media.TITLE,
			MediaStore.Audio.Media.DISPLAY_NAME,
			MediaStore.Audio.Media.ALBUM,
			MediaStore.Audio.Media.DURATION,
			MediaStore.Audio.Media.SIZE
		};
		String selection = MediaStore.Audio.Media.IS_MUSIC + "!=0 AND " 
                         + MediaStore.Audio.Media.MIME_TYPE + " IN (?,?,?,?,?,?,?,?,?,?)";

        // for MIME type, you must refer to frameworks/base/media/java/android/media/MediaFile.java
		String selectionArgs[]= {   "audio/mpeg", 
                                    "audio/x-ms-wma",
                                    "audio/x-wav", 
                                    "audio/aac",
                                    "audio/aac-adts",
                                    "audio/mp4",
                                    "audio/flac", 
                                    "audio/ogg",
                                    "audio/x-scpls",
                                    "audio/x-mpegurl"};

		switch (nTarget)
		{
		case FILTER_FLAG_ALBUM:
			selection += "AND " + MediaStore.Audio.Media.ALBUM_ID + "=" + nId;
			break;
		case FILTER_FLAG_ARTIST:
			selection += "AND " + MediaStore.Audio.Media.ARTIST_ID + "=" + nId;
			break;
		case FILTER_FLAG_GENRE:
			uri = MediaStore.Audio.Genres.Members.getContentUri("external", nId);
			break;
		case FILTER_FLAG_NONE:
		default:
			uri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
			break;
		}

		Cursor musicCursor = mContext.getContentResolver().query(
									uri,
									projection,
									selection,
									selectionArgs, 	
									null);	


		JSONObject response = new JSONObject();		
		try 
		{
			if (musicCursor != null && musicCursor.moveToPosition(nOffset)) {
				Slog.d(TAG,  "Get music cursor and move to offset = " + nOffset);

				int id, duration;
				long size;
				String artist, title, dispName, album;

				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);
				response.put(JSONTAG.RESPONSE_TYPE, JSONTAG.RESPONSE_TYPE_MUSICLIST);
				
				// product JSON output string
				Slog.d(TAG,  "number of items = " + musicCursor.getCount());
				if (nOffset + nMax > musicCursor.getCount())
					nNumItems = musicCursor.getCount() - nOffset;

				response.put(JSONTAG.TOTAL_ITEM, musicCursor.getCount());
				response.put(JSONTAG.OFFSET, nOffset);
				response.put(JSONTAG.NUM_OF_ITEM, nNumItems);

                Slog.d(TAG,  "number of response items = " + nNumItems);

                if (nNumItems > 0) {
                    JSONArray items = new JSONArray();
                    
                    for (int i = 0; i < nNumItems; i++) {
                        JSONObject item = new JSONObject(); 
                        
                        id = musicCursor.getInt(musicCursor.getColumnIndexOrThrow(MediaStore.Audio.Media._ID));
                        artist = musicCursor.getString(musicCursor.getColumnIndexOrThrow(MediaStore.Audio.Media.ARTIST));  
                        dispName = musicCursor.getString(musicCursor.getColumnIndexOrThrow(MediaStore.Audio.Media.DISPLAY_NAME));
                        album = musicCursor.getString(musicCursor.getColumnIndexOrThrow(MediaStore.Audio.Media.ALBUM));    
                        duration = musicCursor.getInt(musicCursor.getColumnIndexOrThrow(MediaStore.Audio.Media.DURATION)); 
                        size = musicCursor.getLong(musicCursor.getColumnIndexOrThrow(MediaStore.Audio.Media.SIZE));
                    
                        item.putOpt(JSONTAG.ITEM_ID, id);
                        item.putOpt(JSONTAG.ITEM_ARTIST, artist);
                        item.putOpt(JSONTAG.ITEM_DISPNAME, dispName);
                        item.putOpt(JSONTAG.ITEM_ALBUM, album);
                        item.putOpt(JSONTAG.ITEM_DURATION, duration);
                        item.putOpt(JSONTAG.ITEM_SIZE, size);
                    
                        items.put(item);
                        if (musicCursor.moveToNext() == false) {
                            Slog.e(TAG, "Fail to move to next!!");
                        }
                    }
                    response.put(JSONTAG.ITEMS, items);
                }
			}
			else {
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE);
				response.put(JSONTAG.CMD_ERR_DESC, "Fail to get cursor or move to requested offset");				
			}
		} catch (JSONException e) {
			Slog.e(TAG, "JSON exception when querying music!" + e.getMessage());
		} catch (Exception e) {
		    Slog.e(TAG, "Query music exception!" + e.getMessage());
		} finally {
		    if (musicCursor != null) {
                Slog.d(TAG, "Close music cursor");
                musicCursor.close();
            }
		};							

        Slog.d(TAG, "JSON response:" + response.toString());
		return response.toString();
    }

	@Override
	public String queryAlbum(int nOffset, int nMax, int nTarget, int nId) {
		Slog.d(TAG, "queryAlbum, offset = " + nOffset + ", max = " + nMax);
		Slog.v(TAG,  "queryAlbum, target = "+nTarget+",id = "+nId);

		Uri uri;
		int nNumItems = nMax;
		String selection = "";

		switch (nTarget)
		{
		case FILTER_FLAG_ARTIST:
			uri = MediaStore.Audio.Artists.Albums.getContentUri("external", nId);
			break;
		case FILTER_FLAG_NONE:
		default:
			uri = MediaStore.Audio.Albums.EXTERNAL_CONTENT_URI;
			break;
		}

		String[] projection = {
			MediaStore.Audio.Albums._ID,
			MediaStore.Audio.Albums.ALBUM,
			MediaStore.Audio.Albums.ARTIST,
			MediaStore.Audio.Albums.NUMBER_OF_SONGS
		};

		Cursor albumCursor = mContext.getContentResolver().query(
									uri,
									projection,
									selection,
									null, 	// sortOrder,
									null);	// cancellationSignal

		JSONObject response = new JSONObject();
		try {
			if (albumCursor != null && albumCursor.moveToPosition(nOffset) == true) {
				Slog.d(TAG,  "Get album cursor and move to offset = " + nOffset);

				int id, numSongs;
				String album, artist;
				
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);
				response.put(JSONTAG.RESPONSE_TYPE, JSONTAG.RESPONSE_TYPE_ALBUMLIST);

				// product JSON output string
				Slog.d(TAG,  "number of items = " + albumCursor.getCount());
				if (nOffset + nMax > albumCursor.getCount())
					nNumItems = albumCursor.getCount() - nOffset;
				
				response.put(JSONTAG.TOTAL_ITEM, albumCursor.getCount());
				response.put(JSONTAG.OFFSET, nOffset);
				response.put(JSONTAG.NUM_OF_ITEM, nNumItems);

                if (nNumItems > 0) {
                    JSONArray items = new JSONArray();
                    
                    for (int i = 0; i < nNumItems; i++) {
                        JSONObject item = new JSONObject(); 
                        
                        id = albumCursor.getInt(albumCursor.getColumnIndexOrThrow(MediaStore.Audio.Albums._ID));
                        album = albumCursor.getString(albumCursor.getColumnIndexOrThrow(MediaStore.Audio.Albums.ALBUM));
                        artist = albumCursor.getString(albumCursor.getColumnIndexOrThrow(MediaStore.Audio.Albums.ARTIST));
                        numSongs = albumCursor.getInt(albumCursor.getColumnIndex(MediaStore.Audio.Albums.NUMBER_OF_SONGS));
                        
                        item.putOpt(JSONTAG.ITEM_ID, id);
                        item.putOpt(JSONTAG.ITEM_ALBUM, album);
                        item.putOpt(JSONTAG.ITEM_ARTIST, artist);
                        item.putOpt(JSONTAG.ITEM_NUM_OF_SONGS, numSongs);
                    
                        items.put(item);
                        albumCursor.moveToNext();
                    }

                    response.put(JSONTAG.ITEMS, items);
                }				
			}
			else {
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE);
				response.put(JSONTAG.CMD_ERR_DESC, "Fail to get cursor or move to requested offset");				
			}			
		} catch (JSONException e) {
			Slog.e(TAG, "JSON exception when querying album!" + e.getMessage());
		} catch (Exception e) {
		    Slog.e(TAG, "Query album exception!" + e.getMessage());
		} finally {
		    if (albumCursor != null) {
                Slog.d(TAG,  "Close album cursor");
                albumCursor.close();
            }
		};			
		
		return response.toString();									
	}


    @Override
	public byte[] queryAlbumArt(int nId, int nWidth, int nHeight) {
		Bitmap bitmap = null;
        byte[] image = null;
        ByteArrayOutputStream outStream = null;

        Slog.d(TAG, "nId = " + nId + ",nWidth =" + nWidth + ",nHeight = " + nHeight);
        Uri contentUri = ContentUris.withAppendedId(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI, nId);
        
        try {
            outStream = new ByteArrayOutputStream();
            MediaMetadataRetriever mmr = new MediaMetadataRetriever();
            
            mmr.setDataSource(mContext,contentUri);
            byte[] bytes = mmr.getEmbeddedPicture();            
            if (bytes != null) 
            {
                Slog.d(TAG, "bytes != null. Length = " + bytes.length);
                bitmap = BitmapFactory.decodeByteArray(bytes, 0, bytes.length);
            }
            
            if (bitmap != null)
            {
                Slog.d(TAG, "bitmap != null.");
                float wScaleRatio = (float)nWidth / (float)bitmap.getWidth();
                float hScaleRatio = (float)nHeight / (float)bitmap.getHeight();
                if (wScaleRatio > hScaleRatio)
                    nWidth = (int)(bitmap.getWidth()*hScaleRatio);
                else
                    nHeight = (int)(bitmap.getHeight()*wScaleRatio);

                Slog.d(TAG, "Original nWidth =" + bitmap.getWidth() + ",nHeight = " + bitmap.getHeight());
                Slog.d(TAG, "Scaled nWidth =" + nWidth + ",nHeight = " + nHeight);
                Bitmap tmp = Bitmap.createScaledBitmap(bitmap,nWidth,nHeight,true);
                bitmap.recycle();
                bitmap = tmp;
                
                boolean bCompressed = bitmap.compress(Bitmap.CompressFormat.PNG, 100, outStream);
                if (bCompressed == true)
                    image = outStream.toByteArray();
                else
                    image = bytes;

                outStream.flush();
                outStream.close();
                
                Slog.d(TAG, "image size = " + image.length);
            } 
            
        } catch (Exception e) {
            Slog.e(TAG, "Exception, " + e.getMessage());
        } finally {
            try {  
                if(outStream != null)  
                    outStream.close() ;  
            } catch (IOException e) {  
                e.printStackTrace();  
            }          
        }

		return image;
	}

	@Override
	public String queryArtist(int nOffset, int nMax) {
		Slog.d(TAG, "queryArtist, offset = " + nOffset + ", max = " + nMax);
		JSONObject response = new JSONObject();

		int nNumItems = nMax;
		String selection = "";
		String[] projection = {
			MediaStore.Audio.Artists._ID,
			MediaStore.Audio.Artists.ARTIST,
			MediaStore.Audio.Artists.NUMBER_OF_ALBUMS,
			MediaStore.Audio.Artists.NUMBER_OF_TRACKS
		};

		Cursor artistCursor = mContext.getContentResolver().query(
									MediaStore.Audio.Artists.EXTERNAL_CONTENT_URI,
									projection,
									selection,
									null, 	// sortOrder,
									null);	// cancellationSignal

		try {
			if (artistCursor != null && artistCursor.moveToPosition(nOffset) == true) {
				Slog.d(TAG,  "Get artist cursor and move to offset = " + nOffset);

				int id, numAlbums, numSongs;
				String artist;
				
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);
				response.put(JSONTAG.RESPONSE_TYPE, JSONTAG.RESPONSE_TYPE_ARTISTLIST);

				// product JSON output string
				Slog.d(TAG,  "number of items = " + artistCursor.getCount());
				if (nOffset + nMax > artistCursor.getCount())
					nNumItems = artistCursor.getCount() - nOffset;
				
				response.put(JSONTAG.TOTAL_ITEM, artistCursor.getCount());
				response.put(JSONTAG.OFFSET, nOffset);
				response.put(JSONTAG.NUM_OF_ITEM, nNumItems);

                if (nNumItems > 0) {
                    JSONArray items = new JSONArray();

    				for (int i = 0; i < nNumItems; i++) {
    					JSONObject item = new JSONObject(); 

    					id = artistCursor.getInt(artistCursor.getColumnIndexOrThrow(MediaStore.Audio.Artists._ID));
    					artist = artistCursor.getString(artistCursor.getColumnIndexOrThrow(MediaStore.Audio.Artists.ARTIST));
    					numAlbums = artistCursor.getInt(artistCursor.getColumnIndexOrThrow(MediaStore.Audio.Artists.NUMBER_OF_ALBUMS));
    					numSongs = artistCursor.getInt(artistCursor.getColumnIndexOrThrow(MediaStore.Audio.Artists.NUMBER_OF_TRACKS));

    					item.putOpt(JSONTAG.ITEM_ID, id);
    					item.putOpt(JSONTAG.ITEM_ARTIST, artist);
    					item.putOpt(JSONTAG.ITEM_NUM_OF_SONGS, numSongs);
    					item.putOpt(JSONTAG.ITEM_NUM_OF_ALBUMS, numAlbums);
    				
    					items.put(item);
    					artistCursor.moveToNext();
    				}

                    response.put(JSONTAG.ITEMS, items);
                }
			}
			else {
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE);
				response.put(JSONTAG.CMD_ERR_DESC, "Fail to get cursor or move to requested offset");				
			}			
		} catch (JSONException e) {
			Slog.e(TAG, "JSON exception when querying artist!" + e.getMessage());
		} catch (Exception e) {
		    Slog.e(TAG, "Query artist exception!" + e.getMessage());
		} finally {
		    if (artistCursor != null) {
                Slog.d(TAG,  "Close artist cursor");
                artistCursor.close();
            }
		};				
		
		return response.toString();									
	}

	@Override
	public String queryGenre(int nOffset, int nMax) {
		Slog.d(TAG, "queryGenre, offset = " + nOffset + ", max = " + nMax);
		JSONObject response = new JSONObject();

		int nNumItems = nMax;
		String selection = "";
		String[] projection = {
			MediaStore.Audio.Genres._ID,
			MediaStore.Audio.Genres.NAME
		};

		Cursor genreCursor = mContext.getContentResolver().query(
									MediaStore.Audio.Genres.EXTERNAL_CONTENT_URI,
									projection,
									selection,
									null, 	// sortOrder,
									null);	// cancellationSignal

		try {
			if (genreCursor != null && genreCursor.moveToPosition(nOffset) == true) {
				Slog.d(TAG,  "Get genre cursor and move to offset = " + nOffset);

				int id;
				String name;
				
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);
				response.put(JSONTAG.RESPONSE_TYPE, JSONTAG.RESPONSE_TYPE_GENRELIST);

				// product JSON output string
				Slog.d(TAG,  "number of items = " + genreCursor.getCount());
				if (nOffset + nMax > genreCursor.getCount())
					nNumItems = genreCursor.getCount() - nOffset;
				
				response.put(JSONTAG.TOTAL_ITEM, genreCursor.getCount());
				response.put(JSONTAG.OFFSET, nOffset);
				response.put(JSONTAG.NUM_OF_ITEM, nNumItems);

                if (nNumItems > 0) {
                    JSONArray items = new JSONArray();

    				for (int i = 0; i < nNumItems; i++) {
    					JSONObject item = new JSONObject(); 

    					id = genreCursor.getInt(genreCursor.getColumnIndexOrThrow(MediaStore.Audio.Genres._ID));
    					name = genreCursor.getString(genreCursor.getColumnIndexOrThrow(MediaStore.Audio.Genres.NAME));

    					item.putOpt(JSONTAG.ITEM_ID, id);					
    					item.putOpt(JSONTAG.ITEM_NAME, name);
    				
    					items.put(item);
    					genreCursor.moveToNext();
                    }

                    response.put(JSONTAG.ITEMS, items);
                }
			}
			else {
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE);
				response.put(JSONTAG.CMD_ERR_DESC, "Fail to get cursor or move to requested offset");				
			}			
		} catch (JSONException e) {
			Slog.e(TAG, "JSON exception when querying genre!" + e.getMessage());
		} catch (Exception e) {
		    Slog.e(TAG, "Query genre exception!" + e.getMessage());
		} finally {
		    if (genreCursor != null) {
                Slog.d(TAG,  "Close gener cursor");
                genreCursor.close();
            }
		};			
		
		return response.toString();				
	}

	@Override
	public String queryPhoto(int nOffset, int nMax) {
		Slog.d(TAG,  "queryPhoto, offset = "+nOffset+",max = "+nMax);
		JSONObject response = new JSONObject();

		int nNumItems = nMax;

		String selection = MediaStore.Images.Media.MIME_TYPE + " IN (?,?,?,?)";
        
		String selectionArgs[]= {   "image/png", 
                                    "image/gif", 
                                    "image/jpeg", 
                                    "image/x-ms-bmp"};
		
		String[] projection = {
			MediaStore.Images.Media._ID,
			MediaStore.Images.Media.TITLE,
			MediaStore.Images.Media.DISPLAY_NAME,
			MediaStore.Images.Media.BUCKET_ID,
			MediaStore.Images.Media.BUCKET_DISPLAY_NAME,
			MediaStore.Images.Media.HEIGHT,
			MediaStore.Images.Media.WIDTH,
			MediaStore.Images.Media.SIZE,
			MediaStore.Images.Media.DATE_TAKEN				
		};

		Cursor imageCursor = mContext.getContentResolver().query(
									MediaStore.Images.Media.EXTERNAL_CONTENT_URI,
									projection,
									selection,
									selectionArgs, 	
									null);

		try {
			if (imageCursor != null && imageCursor.moveToPosition(nOffset) == true) {
				Slog.d(TAG,  "Get image cursor and move to offset = " + nOffset);

				int id, bucketId, height, width;
				long size, dateTaken;
				String title, dispName, bucketDispName;
				
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);
				response.put(JSONTAG.RESPONSE_TYPE, JSONTAG.RESPONSE_TYPE_PHOTOLIST);

				// product JSON output string
				Slog.d(TAG,  "number of items = " + imageCursor.getCount());
				if (nOffset + nMax > imageCursor.getCount())
					nNumItems = imageCursor.getCount() - nOffset;
				
				response.put(JSONTAG.TOTAL_ITEM, imageCursor.getCount());
				response.put(JSONTAG.OFFSET, nOffset);
				response.put(JSONTAG.NUM_OF_ITEM, nNumItems);

                Slog.d(TAG,  "number of response items = " + nNumItems);
                if (nNumItems > 0) {
                    JSONArray items = new JSONArray();
                    
    				for (int i = 0; i < nNumItems; i++) {
    					JSONObject item = new JSONObject(); 
    					
    					id = imageCursor.getInt(imageCursor.getColumnIndexOrThrow(MediaStore.Images.Media._ID));
    					title = imageCursor.getString(imageCursor.getColumnIndexOrThrow(MediaStore.Images.Media.TITLE));  
    					dispName = imageCursor.getString(imageCursor.getColumnIndexOrThrow(MediaStore.Images.Media.DISPLAY_NAME));
    					bucketId = imageCursor.getInt(imageCursor.getColumnIndexOrThrow(MediaStore.Images.Media.BUCKET_ID));
    					bucketDispName = imageCursor.getString(imageCursor.getColumnIndexOrThrow(MediaStore.Images.Media.BUCKET_DISPLAY_NAME));    
    					height = imageCursor.getInt(imageCursor.getColumnIndexOrThrow(MediaStore.Images.Media.HEIGHT)); 
    					width = imageCursor.getInt(imageCursor.getColumnIndexOrThrow(MediaStore.Images.Media.WIDTH));
    					size = imageCursor.getLong(imageCursor.getColumnIndexOrThrow(MediaStore.Images.Media.SIZE));
    					dateTaken = imageCursor.getLong(imageCursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATE_TAKEN));
                       
    					item.putOpt(JSONTAG.ITEM_ID, id);
    					item.putOpt(JSONTAG.ITEM_TITLE, title);
    					item.putOpt(JSONTAG.ITEM_DISPNAME, dispName);
    					item.putOpt(JSONTAG.ITEM_BUCKETID, bucketId);
    					item.putOpt(JSONTAG.ITEM_BUCKET_DISPNAME, bucketDispName);
    					item.putOpt(JSONTAG.ITEM_HEIGHT, height);
    					item.putOpt(JSONTAG.ITEM_WIDTH, width);
    					item.putOpt(JSONTAG.ITEM_SIZE, size);
    					item.putOpt(JSONTAG.ITEM_DATE_TAKEN, dateTaken);
    				
    					items.put(item);
                        if (imageCursor.moveToNext() == false) {
                            Slog.e(TAG, "Fail to move to next!!");
                        }
    				}
                    response.put(JSONTAG.ITEMS, items);
                }
			}
			else {
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE);
				response.put(JSONTAG.CMD_ERR_DESC, "Fail to get cursor or move to requested offset");				
			}			
		} catch (JSONException e) {
			Slog.e(TAG, "JSON exception when querying photo!" + e.getMessage());
		} catch (Exception e) {
		    Slog.e(TAG, "Query photo exception!" + e.getMessage());
		} finally {
		    if (imageCursor != null) {
                Slog.d(TAG,  "Close image cursor");
                imageCursor.close();
            }
		};			

        Slog.d(TAG, "JSON response:" + response.toString());
		return response.toString();
	}


	@Override
	public String queryVideo(int nOffset, int nMax) {
		Slog.d(TAG,  "queryVideo, offset = "+nOffset+",max = "+nMax);
		JSONObject response = new JSONObject();

		int nNumItems = nMax;
		
		// Sync with android\vendor\realtek\apps\RTKMediaBrowser\res\xml\mimetypes.xml
		String selection = MediaStore.Video.Media.MIME_TYPE + " IN (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
		String selectionArgs[]= {   
			"application/ogg",
			"video/3gp",
			"video/3gpp",
			"video/avi",
			"video/avs",
			"video/avs-video",
			"video/f4v",
			"video/flv",
			"video/iso",
			"video/m2ts",
			"video/m2v",
			"video/mp2p",
			"video/mp2ts",
			"video/mp4",
			"video/mpeg",
			"video/mpg",
			"video/mts",
			"video/ogm",
			"video/rm",
			"video/rmvb",
			"video/tp",
			"video/trp",
			"video/ts",
			"video/vob",
			"video/webm",
			"video/x-flv",
			"video/x-matroska",
			"video/x-ms-wmv",
			"video/x-xvid"
		};
		
		String[] projection = {
				MediaStore.Video.Media._ID,
				MediaStore.Video.Media.TITLE,
				MediaStore.Video.Media.DISPLAY_NAME,
				MediaStore.Video.Media.BUCKET_ID,
				MediaStore.Video.Media.BUCKET_DISPLAY_NAME,
				MediaStore.Video.Media.HEIGHT,
				MediaStore.Video.Media.WIDTH,
				MediaStore.Video.Media.SIZE,
				MediaStore.Video.Media.DATE_TAKEN,
				MediaStore.Video.Media.DURATION
		};

		Cursor videoCursor = mContext.getContentResolver().query(
									MediaStore.Video.Media.EXTERNAL_CONTENT_URI,
									projection,
									selection,
									selectionArgs, 	
									null);	

		try {
			if (videoCursor != null && videoCursor.moveToPosition(nOffset) == true) {
				Slog.d(TAG,  "Get video cursor and move to offset = " + nOffset);

				int id, bucketId, height, width, duration;
				long size, dateTaken;
				String title, dispName, bucketDispName;
				
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);
				response.put(JSONTAG.RESPONSE_TYPE, JSONTAG.RESPONSE_TYPE_VIDEOLIST);

				// product JSON output string
				Slog.d(TAG,  "number of items = " + videoCursor.getCount());
				if (nOffset + nMax > videoCursor.getCount())
					nNumItems = videoCursor.getCount() - nOffset;
				
				response.put(JSONTAG.TOTAL_ITEM, videoCursor.getCount());
				response.put(JSONTAG.OFFSET, nOffset);
				response.put(JSONTAG.NUM_OF_ITEM, nNumItems);
                
				Slog.d(TAG,  "number of response items = " + nNumItems);

                if (nNumItems > 0) {
                    JSONArray items = new JSONArray();
                    
    				for (int i = 0; i < nNumItems; i++) {
    					JSONObject item = new JSONObject(); 
    					
    					id = videoCursor.getInt(videoCursor.getColumnIndexOrThrow(MediaStore.Images.Media._ID));
    					title = videoCursor.getString(videoCursor.getColumnIndexOrThrow(MediaStore.Images.Media.TITLE));  
    					dispName = videoCursor.getString(videoCursor.getColumnIndexOrThrow(MediaStore.Images.Media.DISPLAY_NAME));
    					bucketId = videoCursor.getInt(videoCursor.getColumnIndexOrThrow(MediaStore.Images.Media.BUCKET_ID));
    					bucketDispName = videoCursor.getString(videoCursor.getColumnIndexOrThrow(MediaStore.Images.Media.BUCKET_DISPLAY_NAME));    
    					height = videoCursor.getInt(videoCursor.getColumnIndexOrThrow(MediaStore.Images.Media.HEIGHT)); 
    					width = videoCursor.getInt(videoCursor.getColumnIndexOrThrow(MediaStore.Images.Media.WIDTH));
    					size = videoCursor.getLong(videoCursor.getColumnIndexOrThrow(MediaStore.Images.Media.SIZE));
    					dateTaken = videoCursor.getLong(videoCursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATE_TAKEN));
    					duration = videoCursor.getInt(videoCursor.getColumnIndexOrThrow(MediaStore.Video.Media.DURATION)); 

                        item.putOpt(JSONTAG.ITEM_ID, id);
    					item.putOpt(JSONTAG.ITEM_TITLE, title);
    					item.putOpt(JSONTAG.ITEM_DISPNAME, dispName);
    					item.putOpt(JSONTAG.ITEM_BUCKETID, bucketId);
    					item.putOpt(JSONTAG.ITEM_BUCKET_DISPNAME, bucketDispName);
    					item.putOpt(JSONTAG.ITEM_HEIGHT, height);
    					item.putOpt(JSONTAG.ITEM_WIDTH, width);
    					item.putOpt(JSONTAG.ITEM_SIZE, size);
    					item.putOpt(JSONTAG.ITEM_DATE_TAKEN, dateTaken);
    					item.putOpt(JSONTAG.ITEM_DURATION, duration);
    				
    					items.put(item);
    					if (videoCursor.moveToNext() == false) {
                            Slog.e(TAG, "Fail to move to next!!");
                        }
    				}

                    response.put(JSONTAG.ITEMS, items);
                }
			}
			else {
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE);
				response.put(JSONTAG.CMD_ERR_DESC, "Fail to get cursor or move to requested offset");				
			}			
		} catch (JSONException e) {
			Slog.e(TAG, "JSON exception when querying video!" + e.getMessage());
		} catch (Exception e) {
		    Slog.e(TAG, "Query video exception!" + e.getMessage());
		} finally {
		    if (videoCursor != null) {
                Slog.d(TAG,  "Close video cursor");
                videoCursor.close();
            }
		};			

        Slog.d(TAG, "JSON response:" + response.toString());
		return response.toString();
	}    

    @Override
    public String getContentProperty(int contentType, int nId)
    {
        String filePath = null;
        Uri contentUri = null;
        JSONObject response = new JSONObject();

        Slog.d(TAG, "contentType = " + contentType + ", nId = " + nId);
        switch(contentType) {
        case CONTENT_TYPE_MUSIC:
            contentUri = ContentUris.withAppendedId(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI, nId);
            break;
        case CONTENT_TYPE_PHOTO:
            contentUri = ContentUris.withAppendedId(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, nId);
            break;
        case CONTENT_TYPE_VIDEO:
            contentUri = ContentUris.withAppendedId(MediaStore.Video.Media.EXTERNAL_CONTENT_URI, nId);
            break;
        default:            
        }            
        
        Slog.d(TAG, "URI = "+ contentUri);
        if (contentUri != null && "content".equals(contentUri.getScheme())) {
            Cursor cursor = mContext.getContentResolver().query(contentUri, new String[] { android.provider.MediaStore.Images.ImageColumns.DATA }, null, null, null);
            cursor.moveToFirst();
            filePath = cursor.getString(0);
            cursor.close();
        }
        
        Slog.d(TAG, "File path = "+ filePath);        
        
        try
        {
            response.put(JSONTAG.RESPONSE_TYPE, JSONTAG.RESPONSE_TYPE_GETPROPERTY);
            if(filePath != null)
            {
                response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);
                response.put(JSONTAG.CMD_FILE_PATH, filePath);                
            }
            else
            {
                response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE);
                response.put(JSONTAG.CMD_ERR_DESC, "Fail to get real path");
            }
        } catch (JSONException e) {
            Slog.e(TAG, "JSON exception when gettting the file path!" + e.getMessage());
        }
        return response.toString();
      
    }

    @Override
	public int play(String _url) {
		String extension=null, mimeType=null;
		ComponentName comp = null;
        int ret = 0; // 0: success; -1; fail

		Slog.d(TAG, "Try to play " + _url);
		try {
            QueryFileContent queryFileCnt = new QueryFileContent(_url);
            Uri contentUri = queryFileCnt.getContentUri(mContext);
            if (contentUri == null) {
                //throw new InvalidParameterException("unable to find content for:"
                //                            + _url);
                
                // try to play only this file later
                Slog.w(TAG, "unable to find content for:" + _url);
			}

            Intent intent = new Intent(Intent.ACTION_VIEW);
            if (queryFileCnt.getContentUri() != null) {
                // try to view the file as a Media content to follow the other
                // media app's behavior
                intent.setData(queryFileCnt.getContentUri());
            } else {
                // unable to find the file in Media ContentResolver
                // just launch the corresponding viewer for single file playback
                intent.setDataAndType(queryFileCnt.getMediaFileUri(),
                                        queryFileCnt.getMimeType());
            }
            
			if (queryFileCnt.isVideo()) {
				comp = new ComponentName(
						"com.android.gallery3d",
						"com.android.gallery3d.app.MovieActivity");

				intent.setComponent(comp);
                intent.putExtra( "com.realtek.player.USE_RT_MEDIAPLAYER", true);
			} else if (queryFileCnt.isAudio() || queryFileCnt.isPlaylist()) {
				comp = new ComponentName(
						"com.android.music",
						"com.android.music.MediaPlaybackActivity");

				intent.setComponent(comp);                
				intent.putExtra( MediaStore.EXTRA_FINISH_ON_COMPLETION,	true);
			} else if (queryFileCnt.isImage()) {
				comp = new ComponentName(
						"com.android.gallery3d",
						"com.android.gallery3d.app.GalleryActivity");

				intent.setComponent(comp);
			}
			
			intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
			Slog.d(TAG, "It starts activity");
			mContext.startActivity(intent);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			Slog.e(TAG, "Exception when playing:" + e.getMessage());
            ret = -1;
		}	

        return ret;
	}      

    @Override
    public int playMusic(int nId) {
        int ret = 0; // 0: success; -1; fail
        ComponentName comp = null;
        Uri contentUri = ContentUris.withAppendedId(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI, nId);

		try {
            Slog.d(TAG, "Try to play " + contentUri.toString());

			Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setData(contentUri);
			comp = new ComponentName(
					"com.android.music",
					"com.android.music.MediaPlaybackActivity");

			intent.setComponent(comp);                
			intent.putExtra( MediaStore.EXTRA_FINISH_ON_COMPLETION,	true);

			intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
			Slog.d(TAG, "It starts activity");
			mContext.startActivity(intent);
            
		} catch (Exception e) {
			// TODO Auto-generated catch block
			Slog.e(TAG, "Exception when playing" + e.getMessage());
            ret = -1;
		}	

        return ret;
    }
   
    @Override
	public int playPhoto(int nId) {
        int ret = 0; // 0: success; -1; fail
        ComponentName comp = null;
        Uri contentUri = ContentUris.withAppendedId(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, nId);

		try {
            Slog.d(TAG, "Try to play " + contentUri.toString());

			Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setData(contentUri);
            comp = new ComponentName(
                    "com.android.gallery3d",
                    "com.android.gallery3d.app.GalleryActivity");
            
            intent.setComponent(comp);

			intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
			Slog.d(TAG, "It starts activity");
			mContext.startActivity(intent);
            
		} catch (Exception e) {
			// TODO Auto-generated catch block
			Slog.e(TAG, "Exception when playing" + e.getMessage());
            ret = -1;
		}	

        return ret;	    
    }

    @Override
	public int playVideo(int nId) {
        int ret = 0; // 0: success; -1; fail
        ComponentName comp = null;
        Uri contentUri = ContentUris.withAppendedId(MediaStore.Video.Media.EXTERNAL_CONTENT_URI, nId);

		try {
            Slog.d(TAG, "Try to play " + contentUri.toString());

			Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setData(contentUri);
            comp = new ComponentName(
                    "com.android.gallery3d",
                    "com.android.gallery3d.app.MovieActivity");
            
            intent.setComponent(comp);
            intent.putExtra( "com.realtek.player.USE_RT_MEDIAPLAYER", true);

			intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
			Slog.d(TAG, "It starts activity");
			mContext.startActivity(intent);
            
		} catch (Exception e) {
			// TODO Auto-generated catch block
			Slog.e(TAG, "Exception when playing" + e.getMessage());
            ret = -1;
		}	

        return ret;	 	    
    }
    
    private class QueryFileContent {
        private String mPath;
        private String mMime = null;
        private int mFileType = 0;
        private Uri mContentUri = null;
        private boolean mIsMediaCnt = false;
        
        // constructor
        public QueryFileContent(String _p) {
            if (TextUtils.isEmpty(_p)) {
                throw new IllegalArgumentException("empty path");
            }
            
            mMime = MediaFile.getMimeTypeForFile(_p);
            mFileType = mMime != null ? 
                            MediaFile.getFileTypeForMimeType(mMime) : 0;
            mPath = _p;
            mIsMediaCnt = mFileType != 0 ? 
                            MediaFile.isMimeTypeMedia(mMime) : false;
        }
                
        // getter
        public int getFileType() {
            return mFileType;
        }
        public String getMimeType() {
            return mMime;
        }
        
        // methods
        public boolean isMedia() {
            //return mFileType != 0 ? MediaFile.isMimeTypeMedia(mMime) : false;
            // remove the checking mechanism by MediaFile above
            // coz it has to do only once
            return mIsMediaCnt; 
        }
        public boolean isVideo() {
            return mIsMediaCnt && MediaFile.isVideoFileType(mFileType);
        }
        public boolean isAudio() {
            return mIsMediaCnt && MediaFile.isPlayListFileType(mFileType);
        }
        public boolean isPlaylist() {
            return mIsMediaCnt && MediaFile.isVideoFileType(mFileType);
        }
        public boolean isImage() {
            return mIsMediaCnt && MediaFile.isImageFileType(mFileType);
        }
        
        public Uri getContentUri() {
            return getContentUri(null);
        }
        public Uri getContentUri(Context _context) {
            if (mContentUri != null) {
                return mContentUri;
            }
            if (!(_context instanceof Context)) {
                Slog.w(TAG, "unable to query media without context!");
                return null;
            }
            
            // notice that, DRM files are not treat as media content
            if (!mIsMediaCnt) {
                Slog.w(TAG, "ignore non-media content! (mime:" + mMime + 
                            "; path:" + mPath + ")");
                return null;
            }
            mContentUri = queryContent(_context);
            return mContentUri;
        }
        
        public Uri getMediaFileUri() {
            if (!mIsMediaCnt) {
                throw new InvalidParameterException("not a media file:"
                            + mPath);
            }
            return Uri.fromFile(new File(mPath));
        }
        
        // private functions
        private Uri queryContent(Context _context) {
            // prepare for query CR
            Cursor cr = null;
            String colName = null, filter = null;
            String[] queryPath = {mPath};
            Uri mediaUri = null;
            
            if (MediaFile.isVideoFileType(mFileType)) {
                mediaUri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
                colName = MediaStore.Video.Media._ID;
                filter = MediaStore.Video.Media.DATA;
            } else if (MediaFile.isImageFileType(mFileType)) {
                mediaUri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
                colName = MediaStore.Images.Media._ID;
                filter = MediaStore.Images.Media.DATA;
            } else if (MediaFile.isAudioFileType(mFileType)) {
                mediaUri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                colName = MediaStore.Audio.Media._ID;
                filter = MediaStore.Audio.Media.DATA;
            } else if (MediaFile.isPlayListFileType(mFileType)) {
                mediaUri = MediaStore.Audio.Playlists.EXTERNAL_CONTENT_URI;
                colName = MediaStore.Audio.Playlists._ID;
                filter = MediaStore.Audio.Playlists.DATA;
            }
            cr = _context.getContentResolver().query(
                                mediaUri,
                                new String[] { colName },
                                filter + "=? ",
                                queryPath,
                                null);
            Uri retUri = null;
            if (cr != null && cr.moveToFirst()) {
        		int id = cr.getInt(cr.getColumnIndex(MediaStore.MediaColumns._ID));
        		retUri = Uri.withAppendedPath(mediaUri, Integer.toString(id));
            } else {
                Slog.d(TAG, "unable to query content for:" + mPath);
            }
        	cr.close();
        	return retUri;
        }
    }
    
}
