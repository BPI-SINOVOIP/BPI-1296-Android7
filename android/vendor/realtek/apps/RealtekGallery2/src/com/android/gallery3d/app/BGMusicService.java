package com.android.gallery3d.app;

import android.app.Service;
import android.util.Log;
import android.content.Intent;
import android.os.IBinder;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import java.io.File;
import java.util.ArrayList;

public class BGMusicService extends Service {
    private static final boolean DEBUG=true;
    
    private MediaPlayer mp=null;
    private ArrayList<String> list=null;

    private static final String FOLDER="/storage/emulated/0/Music/";

    private int mPlayIndex = 0;
    private int mPlaylistSize = 0;
    
    //private String path = "/storage/sda1/1.mp3";
    //private String path2 = "/storage/emulated/0/Music/1.mp3";

    private void startPlayBGMusic(){
        mp = new MediaPlayer();
        try{
            mp.setDataSource(getNextTrackPath());
            mp.prepare();
            mp.start();
            mp.setOnCompletionListener(new OnCompletionListener(){
                public void onCompletion (MediaPlayer mp){
                    Log.d("RTK","\033[0;31;31m onComplete....... \033[m");
                    try{
                        mp.reset();
                        mp.setDataSource(getNextTrackPath());
                        mp.prepare();
                        mp.start();
                    }catch(java.io.IOException e){
                        Log.d("RTK","BGMusic Open file failed");
                    }catch (IllegalStateException e){
                        Log.d("RTK","BGMusic illegal state");
                    }
                }
            });
        }catch(java.io.IOException e){
            Log.d("RTK","BGMusic Open file failed");
        }catch (IllegalStateException e){
            Log.d("RTK","BGMusic illegal state");
        }
    }
    

    public static int getBGMusicFolderFileCount(){
        File folder = new File(FOLDER);
        int count=0;

        File[] listOfFiles = folder.listFiles();
        for (int i = 0; i < listOfFiles.length; i++) {
            if (listOfFiles[i].isFile()) {
                count++;
            }
        }
        return count;
    }
    
    private void prepareBGMusicPlaylist(){
        list=new ArrayList<String>();
        File folder = new File(FOLDER);

        File[] listOfFiles = folder.listFiles();
        for (int i = 0; i < listOfFiles.length; i++) {
            if (listOfFiles[i].isFile()) {
                list.add(FOLDER+listOfFiles[i].getName());
            }
        }

        mPlaylistSize = list.size();
        mPlayIndex=0;

        if(DEBUG){
            for(String s:list){
                Log.d("RTK","play file: \033[0;31;31m"+s+"\033[m");
            }
        }
    }

    private String getNextTrackPath(){
        String rst = list.get(mPlayIndex);
        mPlayIndex++;
        if(mPlayIndex>=mPlaylistSize)
            mPlayIndex = 0;
        return rst;
    }

    public void onCreate() {
        // create playlist
        
    }

    public void onDestroy (){
        Log.d("RTK","BackgroundMusicService onDestroy");
        if(mp.isPlaying()){
            mp.stop();
        }else{
            mp.reset();
        }
        mp.release();
        mp = null;
        list=null;
    }

    public int onStartCommand(Intent intent, int flags, int startId) {
        //Log.d("RTK", "Received start id " + startId + ": " + intent);
        prepareBGMusicPlaylist();
        if(mPlaylistSize>0){
            startPlayBGMusic();
        }else{
            Log.d("RTK","onStartCommand of BGMusicService, playlist size error");
        }
        // BUG_FIX: 44058 - do not restart me after crashed
        // return START_STICKY;
        return START_NOT_STICKY;
    }

    public IBinder onBind(Intent intent) {
        return null;
    }
    
}
