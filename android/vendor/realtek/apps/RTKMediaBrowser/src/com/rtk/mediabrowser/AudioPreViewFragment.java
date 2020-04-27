package com.rtk.mediabrowser;
import java.io.File;
import java.io.IOException;
//import java.lang.reflect.InvocationTargetException;
//import java.lang.reflect.Method;

import java.util.Timer;
import java.util.TimerTask;
import java.util.HashMap;
import com.realtek.DataProvider.AbstractDataProvider;
import com.realtek.DataProvider.FileFilterType;
import com.realtek.Utils.MarqueeTextView;
import com.realtek.Utils.InformPreViewMsg;
import com.realtek.Utils.MbwType;
import com.rtk.mediabrowser.InformPreView;

import android.app.Activity;
import android.app.Fragment;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.AsyncTask;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ProgressBar;
import android.widget.ImageView;
import android.widget.TextView;
import android.media.MediaMetadataRetriever;
import android.net.Uri;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.MediaPlayer.OnErrorListener;

import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.AdapterView.OnItemClickListener;
import android.view.View.OnKeyListener;
import android.provider.Settings;

public class AudioPreViewFragment extends Fragment
{
    private String      TAG ="AudioPreViewFragement" ; 
	private ImageView   m_album_photo;
	private ProgressBar m_ProgressBar_audio_progress;
	private TextView    m_TextView_media_elapsed_time;
	private TextView    m_TextView_media_time;
	private TextView    m_TextView_audio_title;
	private TextView    m_TextView_audio_album;
	private TextView    m_TextView_audio_artist;

	//private MediaPlayer m_player = null;

	private int Max = 0;
	private int Minute = 0;
	private int Hour = 0;
	private int Second = 0;
	private final static int LOCAL=0; 
	private final static int NETWORK=1; 

	private TimerTask task_play = null;
	private TimerTask task_getduration = null;
	private Timer timer = null;

	private Handler		m_MyHandler;//Handle play/stop preview information
	public InformPreView m_MyInformPreView;//It will be used to inform File change/ stop playback by activity whom create this fragment.

    private boolean m_Playing = false;   
    private ID3TagTask m_CurID3Task = null; 

	 private OnPreparedListener audioPreparedListener = new OnPreparedListener(){

			@Override
			public void onPrepared(MediaPlayer mp) {
				// TODO Auto-generated method stub
				
				//m_player.start();
				
				//Max = m_player.getDuration();
				Max = 100000000;
				m_ProgressBar_audio_progress.setMax(Max);
				Max/=1000;
				Minute = Max/60;
				Hour = Minute/60;
				Second = Max%60;
				Minute %= 60;
				m_TextView_media_time.setText(String.format("%02d:%02d:%02d", Hour,Minute,Second));
				m_TextView_media_elapsed_time.setText("00:00:00");
				m_ProgressBar_audio_progress.setProgress(0);
				if (timer != null)
				{		
					timer.cancel();
				}
				timer = new Timer(true);
				task_getduration = new TimerTask(){
		        	
		        	@Override
					public void run() {
						// TODO Auto-generated method stub
		        		m_MyHandler.sendEmptyMessage(InformPreViewMsg.PROGRESS_CHANGED);
					}
		        };
			    timer.schedule(task_getduration, 0, TimerDelay.Has_Played_Time);
			}
	}; 
	private OnCompletionListener audioCompletionListener = new OnCompletionListener(){

		@Override
		public void onCompletion(MediaPlayer mp) {
			// TODO Auto-generated method stub
			m_TextView_media_time.setText(String.format("%02d:%02d:%02d", Hour,Minute,Second));
			m_ProgressBar_audio_progress.setProgress(Max);
		}
		 
	 };
	private OnErrorListener audioErrorListener = new OnErrorListener(){
		@Override
		public boolean onError(MediaPlayer mp, int what, int extra)
		{
			Log.d(TAG,"OnError ERROR CODE:"+what+ "extra:"+extra);
			//m_player.reset();
			return true;
		}
	};

    private String textEncodeConvert(String in){
        String res = Settings.System.getString(getActivity().getContentResolver(), Settings.System.REALTEK_SETUP_TEXT_ENCODING_MODE);
        Log.d("RTK","TEXT ENCODE:"+res);
        if (res ==null){
            return in;
        }
        String charSetString = "UTF8";
        if(res.equalsIgnoreCase("off")||res.equalsIgnoreCase("utf-8")){
            charSetString="UTF8";
        }else if(res.equalsIgnoreCase("big5")){
            charSetString="BIG5";
        }else if(res.equalsIgnoreCase("gbk")){
            charSetString="GBK";
        }
        if(charSetString.equals("UTF8"))
            return in;

        String rst = in;
        try{
            rst = new String(in.getBytes("ISO-8859-1"),charSetString);
        }catch(java.io.UnsupportedEncodingException e){
            e.printStackTrace();
            rst = in;
        }
        return rst;
    }

    private class ID3TagTask extends AsyncTask<Object, Void, Void>
    {
        private int Type;
        private final TextView titleText;
        private final TextView albumText;
        private final TextView artistText;
        private final ImageView albumImage;
	    private String MMR_title;
	    private String MMR_album;
	    private String MMR_artist;
	    private byte[] MMR_Picture;

        public ID3TagTask(int type, TextView title, TextView album, TextView artist, ImageView albumArt)
        {
            Type = type;
            titleText =title;
            albumText =album;
            artistText =artist;
            albumImage =albumArt;
        }
        protected Void doInBackground(Object... data)
        {
            if (!isCancelled()){
                final String FileName = String.valueOf(data[0]);
	            Log.d(TAG,"SetID3TagViaMediaMetadataRetriever begin");
	            MediaMetadataRetriever mmr = new MediaMetadataRetriever();
	            try
	            {
	            	if (Type ==	LOCAL)
	            	{ 
	            		mmr.setDataSource(FileName);// It's work for local path file(USB,SMB), but failed with network file(http).
	            	}
	            	else
	            	{
	            		mmr.setDataSource(FileName, new HashMap<String, String>());// It's work for network file (http), but failed with local path file(USB,SMB).
	            	}

	            	MMR_title = mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_TITLE);
	            	MMR_album = mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_ALBUM);
	            	MMR_artist= mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_ARTIST);
	            	MMR_Picture = mmr.getEmbeddedPicture();
	            	MMR_title = MMR_title!=null?MMR_title:"unknow";
	            	MMR_album = MMR_album!=null?MMR_album:"unknow";
	            	MMR_artist = MMR_artist!=null?MMR_artist:"unknow";
	            } catch (Exception e) {
	            	Log.d(TAG,"mmr.setDataSource exception");
	            }
	            mmr.release();
	            Log.d(TAG,"SetID3TagViaMediaMetadataRetriever end");
            }
            return null;
        }
        protected void onPostExecute(Void rlt)
        {
            if (!isCancelled()){
	            titleText.setText(getString(R.string.mb_title)+":"+textEncodeConvert(MMR_title));
	            albumText.setText(getString(R.string.mb_album)+":"+textEncodeConvert(MMR_album));
	            artistText.setText(getString(R.string.mb_artist)+":"+textEncodeConvert(MMR_artist));

	            if(MMR_Picture != null)
	            {
	            	albumImage.setImageBitmap(BitmapFactory.decodeByteArray(MMR_Picture, 0, MMR_Picture.length));
	            }
	            else
	            {
	            	albumImage.setImageResource(R.drawable.icon_default_audio);
	            }
	            albumImage.setVisibility(View.VISIBLE);
            }
        }
    }
    /*
	private void SetID3TagViaMediaMetadataRetriever(int Type, String FileName)
	{
		Log.d(TAG,"SetID3TagViaMediaMetadataRetriever begin");
		MediaMetadataRetriever mmr = new MediaMetadataRetriever();
		try
		{
			if (Type ==	LOCAL)
			{ 
				mmr.setDataSource(FileName);// It's work for local path file(USB,SMB), but failed with network file(http).
			}
			else
			{
				mmr.setDataSource(FileName, new HashMap<String, String>());// It's work for network file (http), but failed with local path file(USB,SMB).
			}

			m_MMR_title = mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_TITLE);
			m_MMR_album = mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_ALBUM);
			m_MMR_artist= mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_ARTIST);
			m_MMR_Picture = mmr.getEmbeddedPicture();
			m_MMR_title = m_MMR_title!=null?m_MMR_title:"unknow";
			m_MMR_album = m_MMR_album!=null?m_MMR_album:"unknow";
			m_MMR_artist = m_MMR_artist!=null?m_MMR_artist:"unknow";
			m_TextView_audio_title.setText(getString(R.string.mb_title)+":"+textEncodeConvert(m_MMR_title));
			m_TextView_audio_album.setText(getString(R.string.mb_album)+":"+textEncodeConvert(m_MMR_album));
			m_TextView_audio_artist.setText(getString(R.string.mb_artist)+":"+textEncodeConvert(m_MMR_artist));

			if(m_MMR_Picture != null)
			{
				m_album_photo.setImageBitmap(BitmapFactory.decodeByteArray(m_MMR_Picture, 0, m_MMR_Picture.length));
			}
			else
			{
				m_album_photo.setImageResource(R.drawable.icon_default_audio);
			}
			m_album_photo.setVisibility(View.VISIBLE);
		} catch (Exception e) {
			Log.d(TAG,"mmr.setDataSource exception");
		}
		mmr.release();
		Log.d(TAG,"SetID3TagViaMediaMetadataRetriever end");
	}
    */
    @Override
    public void onResume()
    {
        super.onResume();
        Log.d(TAG,"onResume:");
    }
    
    @Override
    public void onDestroyView()
    {
    	super.onDestroyView();
    	Log.d(TAG,"onDestroyView");
    }    
    
    @Override
    public void onPause()
    {
       	super.onPause();
    	Log.d(TAG,"onPause");
        //Do PLAY_STOP work immediately instead of send command to handler. 
        m_MyHandler.removeCallbacksAndMessages(null);
        if (m_CurID3Task != null){
            if(!m_CurID3Task.isCancelled())
                m_CurID3Task.cancel(true);
            m_CurID3Task = null;
        }
		//m_MyHandler.sendEmptyMessage(InformPreViewMsg.PLAY_STOP);
		if(timer!= null)
		{
			timer.cancel();
			timer = null;
		}
    }
    
    @Override
    public void onAttach(Activity activity) {       
       	super.onAttach(activity);       
    	Log.d(TAG,"onAttach");
    }
    
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState)//call by system  
    {
        Log.d(TAG,"onCreateView inflater");
        
        return inflater.inflate(R.layout.fragment_audio_preview, container, false);
    }
    
    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) //call immediately after onCreateView
    {
        super.onViewCreated(view, savedInstanceState);
        Log.d(TAG,"onViewCreated view");
        
		//m_player = new MediaPlayer();
		m_album_photo = (ImageView) (view.findViewById(R.id.album_photo));
		m_ProgressBar_audio_progress = (ProgressBar)(view.findViewById(R.id.audio_progress));
        m_ProgressBar_audio_progress.setVisibility(View.INVISIBLE);
		m_TextView_media_elapsed_time = (TextView)(view.findViewById(R.id.media_elapsed_time));
		m_TextView_media_time = (TextView)(view.findViewById(R.id.media_time));
        m_TextView_media_elapsed_time.setVisibility(View.INVISIBLE);
        m_TextView_media_time.setVisibility(View.INVISIBLE);
		m_TextView_audio_title = (TextView)(view.findViewById(R.id.audio_title));
		m_TextView_audio_album = (TextView)(view.findViewById(R.id.audio_album));
		m_TextView_audio_artist = (TextView)(view.findViewById(R.id.audio_artist));
      
	    //m_player.setOnPreparedListener(audioPreparedListener);
	    //m_player.setOnCompletionListener(audioCompletionListener);
	    //m_player.setOnErrorListener(audioErrorListener);
		m_MyHandler = new Handler(){
			@Override
			public void handleMessage(Message msg)
			{
				switch(msg.what)
				{
					case InformPreViewMsg.MEDIA_PLAY:
						if (msg.obj instanceof String)
						{
                            m_Playing = true;
							Log.d(TAG,"Try to preview: "+(String)msg.obj);
							/*
							//m_player.reset();//Wen: As Develop note say, It may cause unpredictable result while it's preparing. 
							try{
								//m_player.setDataSource((String)msg.obj);
							}catch (IOException e1) {
								// TODO Auto-generated catch block
								e1.printStackTrace();
							}
							*/
							//m_player.setLooping(true);
							//m_player.prepareAsync();
							if (msg.arg1==MbwType.TYPE_DEVICE_DMS)
							{
                                m_CurID3Task = new ID3TagTask(NETWORK, m_TextView_audio_title, m_TextView_audio_album, m_TextView_audio_artist, m_album_photo);
                                m_CurID3Task.execute((String)msg.obj);
								//SetID3TagViaMediaMetadataRetriever(NETWORK,(String)msg.obj);
							}
							else
							{
                                m_CurID3Task = new ID3TagTask(LOCAL, m_TextView_audio_title, m_TextView_audio_album, m_TextView_audio_artist, m_album_photo);
                                m_CurID3Task.execute((String)msg.obj);
								//SetID3TagViaMediaMetadataRetriever(LOCAL,(String)msg.obj);
							}
						}
						else
						{
							Log.e(TAG,"Can not load media without no media path");
						}
					break;
					case InformPreViewMsg.PLAY_STOP:
                            if (m_Playing)
                            {
                                m_Playing = false;
						        Log.d(TAG,"Stop audio preview: ");
						        //m_player.stop();
						        m_ProgressBar_audio_progress.setProgress(0);
						        m_TextView_media_elapsed_time.setText("--:--:--");
						        m_TextView_media_time.setText("--:--:--");
                                if (m_CurID3Task != null){
                                    if(!m_CurID3Task.isCancelled())
                                        m_CurID3Task.cancel(true);
                                    m_CurID3Task = null;
                                }
						        m_TextView_audio_title.setText("");
						        m_TextView_audio_album.setText("");
						        m_TextView_audio_artist.setText("");
						        m_album_photo.setVisibility(View.INVISIBLE);
						        if(timer != null)
						        {	
						        	timer.cancel();                           		
						        	timer = null;
						        }
                            }
					break;
					case InformPreViewMsg.PROGRESS_CHANGED:
						//int Cur = m_player.getCurrentPosition();
						int Cur = 1000000;
						m_ProgressBar_audio_progress.setProgress(Cur);
						Cur/= 1000;
						int minute = Cur/60;
						int hour = minute/60;
						int second = Cur%60;
						minute %= 60;
						m_TextView_media_elapsed_time.setText(String.format("%02d:%02d:%02d", hour,minute,second));
					break;
				}
				super.handleMessage(msg);
			}
		};
		m_MyInformPreView = new InformPreView(m_MyHandler);
    }
}
