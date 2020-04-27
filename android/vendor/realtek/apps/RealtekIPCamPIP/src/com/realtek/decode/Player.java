package com.realtek.decode; 
  
import java.io.IOException;
import java.util.Date;

import android.icu.text.SimpleDateFormat;
import android.media.AudioManager;  
import android.media.MediaPlayer;  
import android.media.MediaPlayer.OnBufferingUpdateListener;  
import android.media.MediaPlayer.OnCompletionListener; 
import android.media.SyncParams; 
import android.os.Handler;  
import android.os.Message;  
import android.util.Log;  
import android.view.View;
import android.view.SurfaceHolder;  
import android.view.SurfaceView; 
import android.widget.ProgressBar;

public class Player implements OnBufferingUpdateListener,  
        OnCompletionListener, MediaPlayer.OnPreparedListener {  
    public static final String TAG = "Player";
    public MediaPlayer mediaPlayer;
    private SurfaceHolder surfaceHolder;  
    private int index;
    
    public Player(SurfaceView surfaceView, int index) 
    {  
        surfaceHolder=surfaceView.getHolder();
        this.index = index;
    }  
        
    //*****************************************************  
    public void initPlayer() 
    {
        try {  
            mediaPlayer = new MediaPlayer();
            mediaPlayer.setDisplay(surfaceHolder);  
            mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);  
            mediaPlayer.setOnBufferingUpdateListener(this);
            mediaPlayer.setOnPreparedListener(this);
        } catch (Exception e) {  
            Log.e(TAG, "error", e);  
        }         
    } 
 
      
    public void playUrl(String videoUrl)
    {  
        try {  
            mediaPlayer.reset();
            //mediaPlayer.useRTMediaPlayer(MediaPlayer.FORCE_RT_MEDIAPLAYER);
            mediaPlayer.setDataSource(videoUrl);  
            //mediaPlayer.prepare();
            SyncParams syncParams = mediaPlayer.getSyncParams();
            syncParams.setSyncSource(SyncParams.SYNC_SOURCE_DEFAULT);
            syncParams.setAudioAdjustMode(SyncParams.AUDIO_ADJUST_MODE_DEFAULT);
            mediaPlayer.setSyncParams(syncParams);
            mediaPlayer.prepareAsync();
            //mediaPlayer.start();  
        } catch (IllegalArgumentException e) {  
            // TODO Auto-generated catch block  
            e.printStackTrace();
        } catch (IllegalStateException e) {  
            // TODO Auto-generated catch block  
            e.printStackTrace();  
        } catch (IOException e) {  
            // TODO Auto-generated catch block  
            e.printStackTrace();
        }  
    }  
  
    public void start()  
    {  
        mediaPlayer.start();  
    }       
    public void pause()  
    {  
        if (mediaPlayer != null && mediaPlayer.isPlaying()) {
            mediaPlayer.pause();
        }
    }
      
    public void stop()  
    {  
        if (mediaPlayer != null && mediaPlayer.isPlaying()) {
            mediaPlayer.stop();  
            mediaPlayer.release();   
            mediaPlayer = null;   
        }   
    }    
    
    public int getCurrentPosition()
    {
        int position = -1;
        if (mediaPlayer != null){
            position = mediaPlayer.getCurrentPosition();
        }
        return position;
    }
    public void setScreenOnWhilePlaying(boolean screenOn) {
        if (mediaPlayer != null){
            mediaPlayer.setScreenOnWhilePlaying(screenOn);
        }        
    }
    public void setDisplay (SurfaceHolder sh) {
        if (mediaPlayer != null){
            mediaPlayer.setDisplay(sh);
        }         
    }
    public void setVolume(float leftVolume, float rightVolume) {
        if (mediaPlayer != null) {
            mediaPlayer.setVolume(leftVolume, rightVolume);
        }
    }
      
    @Override  
    /**  
     * onPrepared -- start playback
     */
    public void onPrepared(MediaPlayer mp) { 
        Log.d(TAG, "onPrepared MediaPlayer #"+index);    
        mp.start();
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS"); 
        String time = format.format(new Date());
        Log.d(TAG, "Start MediaPlayer #"+index+" : "+time);
    }  
  
    @Override  
    public void onCompletion(MediaPlayer mp) {
        // TODO Auto-generated method stub
        Log.d(TAG, "onCompletion");
        
    }  
  
    @Override  
    public void onBufferingUpdate(MediaPlayer mp, int bufferingProgress) {  
        Log.d(TAG, "onBufferingUpdate");
    }  
  
}  