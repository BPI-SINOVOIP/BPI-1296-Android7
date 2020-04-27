package com.rtk.mediabrowser;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.Fragment;
import android.media.MediaFormat;
import android.media.MediaMetadataRetriever;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.MediaPlayer.TrackInfo;
import android.os.*;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.*;

import com.realtek.Utils.InformPreViewMsg;
import com.realtek.Utils.MbwType;

import com.realtek.Utils.UseRtMediaPlayer;

import java.util.HashMap;
import java.util.Timer;
import java.util.TimerTask;

public class VideoPreViewFragment extends Fragment
{
	private static final boolean DEBUG = true;
	private static final String TAG ="VideoPreViewFragement" ;
	static final boolean VIDEO_PREVIEW_ENABLE = false;
	private RtkVideoView m_VideoView_vv;
    private MediaPlayer m_MediaPlayer;
	private ImageView	m_ImageView_vv;
    private boolean     m_IsVideoMask;
	private FrameLayout	m_VideoArea;
	private ProgressBar m_ProgressBar_video_progress;
	private TextView    m_TextView_media_elapsed_time;
	private TextView    m_TextView_media_dimension;
	private TextView    m_TextView_media_time;
	private TextView    m_TextView_media_type;
	private TextView    m_TextView_media_size;

	private int Max = 0;
	private int Minute = 0;
	private int Hour = 0;
	private int Second = 0;
    private int m_VideoWidth =0;
    private int m_VideoHeight =0;

	private TimerTask task_play = null;
	private TimerTask task_getduration = null;
	private Timer timer = null;
    private String m_VideoPath = null; 
    private int m_FileType=-1;//It should be LOCAL or NETWORK
	private final static int LOCAL=0; 
	private final static int NETWORK=1; 

	private Handler		m_MyHandler;//Handle play/stop preview information
	private HandlerThread		m_MyHandlerThread;//provider looper for handler
	public InformPreView m_MyInformPreView;//It will be used to inform File change/ stop playback by activity whom create this fragment.
    private final int PREVIEW_IDLE = 0;
    private final int PREVIEW_PREPARING = 1;
    private final int PREVIEW_PLAY = 2;
    private int m_InternalState = PREVIEW_IDLE;
	private OnGetVideoInfoListener mGetVideoInfoCallback;

	public interface OnGetVideoInfoListener {
		public void onGetVideoInfo(int width, int height);
	}

	private void SetVideoInfoViaMediaMetadataRetriever(int Type, String FileName)
	{
		Log.d(TAG,"SetVideoInfoViaMediaMetadataRetriever begin");
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

			String mimeType = mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_MIMETYPE);
			mimeType = mimeType!=null?mimeType:getString(R.string.mb_unknown);
			m_TextView_media_type.setText(mimeType);
		} catch (Exception e) {
			Log.d(TAG,"mmr.setDataSource exception");
		}
		mmr.release();
		Log.d(TAG,"SetVideoInfoViaMediaMetadataRetriever end");
	}

     @TargetApi(Build.VERSION_CODES.KITKAT)
     private void SetVideoInfoViaTrackInfo()
     {
        TrackInfo trackinfo[];
        String mime = null;
        try{
            trackinfo= m_MediaPlayer.getTrackInfo();
        }catch (Exception e){
            Log.d(TAG, "getTrackInfo exception:"+e.getMessage());
            trackinfo = null;
        }

        if (trackinfo != null)
        {
            for(int count=0;count<trackinfo.length;count++)
            {
                if (trackinfo[count].getTrackType()==TrackInfo.MEDIA_TRACK_TYPE_VIDEO)
                {
                    mime = trackinfo[count].getFormat().getString(MediaFormat.KEY_MIME);
                    if (mime != null)
                    {
                        if(mime.startsWith("video/")) {
                            mime = mime.substring(6);
                        }
                    }
                    break;
                }
            }
        }
        //m_TextView_media_type.setText(mime!=null? mime:getString(R.string.mb_unknown));
        m_TextView_media_type.setText(mime!=null? mime:"");
     }
	 private OnPreparedListener videoPreparedListener = new OnPreparedListener(){

			@Override
			public void onPrepared(MediaPlayer mp) {
                if (m_InternalState == PREVIEW_PREPARING)
                {
                    m_MediaPlayer  = mp;
			        // TODO Auto-generated method stub
			        Log.d(TAG,"onPrepared call start() start");
			        m_VideoView_vv.start();
                    m_InternalState = PREVIEW_PLAY;
			        Log.d(TAG,"onPrepared call start() end");
			        mp.setLooping(true);
			        Max = m_VideoView_vv.getDuration();
			        m_ProgressBar_video_progress.setMax(Max);
			        Max/=1000;
			        Minute = Max/60;
			        Hour = Minute/60;
			        Second = Max%60;
			        Minute %= 60;
                    m_VideoWidth = mp.getVideoWidth();
                    m_VideoHeight = mp.getVideoHeight();
                    if (m_VideoWidth >0 && m_VideoHeight > 0)
                    {
			            m_TextView_media_dimension.setText(String.format("%d X %d",m_VideoWidth ,m_VideoHeight));
						if(mGetVideoInfoCallback!=null)
							mGetVideoInfoCallback.onGetVideoInfo(m_VideoWidth, m_VideoHeight);
                    }
                    else
                    {
			            m_TextView_media_dimension.setText("");	
                    }
			        m_TextView_media_time.setText(String.format("%02d:%02d:%02d", Hour,Minute,Second));
			        m_TextView_media_elapsed_time.setText("00:00:00");
			        m_TextView_media_size.setVisibility(View.VISIBLE);
                    //Get video info via mp.gettrackinfo().
                    SetVideoInfoViaTrackInfo();
			        //SetVideoInfoViaMediaMetadataRetriever(m_FileType,m_VideoPath);
			        m_ProgressBar_video_progress.setProgress(0);
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
			}
	}; 
	private OnCompletionListener videoCompletionListener = new OnCompletionListener(){

		@Override
		public void onCompletion(MediaPlayer mp) {
			// TODO Auto-generated method stub
			m_TextView_media_time.setText(String.format("%02d:%02d:%02d", Hour,Minute,Second));
			m_ProgressBar_video_progress.setProgress(Max);
			if(timer!= null)
			{
				timer.cancel();
				timer = null;
			}
			m_VideoView_vv.stopPlayback();
		}
		 
	 };
	private OnErrorListener videoErrorListener = new OnErrorListener() {
		@Override
		public boolean onError(MediaPlayer mp, int what, int extra) {
			Log.d(TAG,"onError");
            m_IsVideoMask = false;
            m_MyHandler.removeMessages(InformPreViewMsg.PROGRESS_CHANGED);
            if(timer!= null)
            {
                timer.cancel();
                timer = null;
            }
			m_VideoView_vv.setVisibility(View.INVISIBLE);
            m_ImageView_vv.setVisibility(View.VISIBLE);
            m_VideoArea.setBackgroundColor(getResources().getColor(R.color.transparent));
			m_ImageView_vv.setImageResource(R.drawable.icon_default_preview);
			m_TextView_media_dimension.setText("? X ?");	
			m_TextView_media_time.setText("--:--:--");
			m_TextView_media_elapsed_time.setText("00:00:00");
		    m_TextView_media_type.setText(getString(R.string.mb_unknown));
			m_ProgressBar_video_progress.setProgress(0);
			return true;
		}
	};

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
        //m_MyHandlerThread.quitSafely();
    }    
    
    @Override
    public void onPause()
    {
       	super.onPause();
    	Log.d(TAG,"onPause Start");
        //Do PLAY_STOP work immediately instead of send command to handler. 
        m_MyHandler.removeCallbacksAndMessages(null);
	    m_VideoView_vv.setOnErrorListener(null);
	    m_VideoView_vv.setOnPreparedListener(null);
	    m_VideoView_vv.setOnCompletionListener(null);
	    m_VideoView_vv.setVisibility(View.INVISIBLE);
		if(m_VideoView_vv.isPlaying())//Should stop Video playback immediately, so don't use handler to do it. 
		{
			m_VideoView_vv.stopPlayback();
		}
        else
        {
            m_VideoView_vv.suspend();
        }
        m_InternalState = PREVIEW_IDLE;
		m_ProgressBar_video_progress.setProgress(0);
		m_TextView_media_elapsed_time.setText("--:--:--");
		m_TextView_media_time.setText("--:--:--");
		m_TextView_media_dimension.setText("");
		m_TextView_media_size.setText("");
		m_TextView_media_type.setText("");
		if(timer != null)
		{	
			timer.cancel();                           		
			timer = null;
		}
        m_MyHandler.removeCallbacksAndMessages(null);
    	Log.d(TAG,"onPause End");
    }
    
    @Override
    public void onAttach(Activity activity) {       
       	super.onAttach(activity);       
    	Log.d(TAG,"onAttach");
    	if(activity instanceof OnGetVideoInfoListener)
			mGetVideoInfoCallback = (OnGetVideoInfoListener)activity;
    }
    
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState)//call by system  
    {
        Log.d(TAG,"onCreateView inflater");
        
        return inflater.inflate(R.layout.fragment_video_preview, container, false);
    }
	private void setTextColor(int MBWType)
	{
		if (MBWType == MbwType.TYPE_DB_VIDEO)
		{
			m_TextView_media_time.setTextColor(getResources().getColor(R.color.BWTitleVideoBlue));
			m_TextView_media_dimension.setTextColor(getResources().getColor(R.color.BWTitleVideoBlue));
			m_TextView_media_elapsed_time.setTextColor(getResources().getColor(R.color.BWTitleVideoBlue));
			m_TextView_media_type.setTextColor(getResources().getColor(R.color.BWTitleVideoBlue));
			m_TextView_media_size.setTextColor(getResources().getColor(R.color.BWTitleVideoBlue));
		}
		else
		{
			m_TextView_media_time.setTextColor(getResources().getColor(R.color.BWTitleGreen));
			m_TextView_media_dimension.setTextColor(getResources().getColor(R.color.BWTitleGreen));
			m_TextView_media_elapsed_time.setTextColor(getResources().getColor(R.color.BWTitleGreen));
			m_TextView_media_type.setTextColor(getResources().getColor(R.color.BWTitleGreen));
			m_TextView_media_size.setTextColor(getResources().getColor(R.color.BWTitleGreen));
		}
	}
    
    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) //call immediately after onCreateView
    {
        super.onViewCreated(view, savedInstanceState);
        Log.d(TAG,"onViewCreated view");
        
		m_VideoView_vv = (RtkVideoView)(view.findViewById(R.id.vv));
		m_ImageView_vv = (ImageView)(view.findViewById(R.id.video_image));
		m_VideoArea = (FrameLayout)(view.findViewById(R.id.VideoArea));
		m_ProgressBar_video_progress = (ProgressBar)(view.findViewById(R.id.video_progress));
		m_TextView_media_elapsed_time = (TextView)(view.findViewById(R.id.media_elapsed_time));
		m_TextView_media_dimension = (TextView)(view.findViewById(R.id.media_dimension));
		m_TextView_media_time = (TextView)(view.findViewById(R.id.media_time));
		m_TextView_media_type = (TextView)(view.findViewById(R.id.media_type));
		m_TextView_media_size = (TextView)(view.findViewById(R.id.media_size));
      
		m_ImageView_vv.setImageResource(R.color.black);

		m_VideoView_vv.setVisibility(View.INVISIBLE);
		m_ImageView_vv.setVisibility(View.INVISIBLE);
		m_ProgressBar_video_progress.setVisibility(View.INVISIBLE);

        m_IsVideoMask = true;
        m_VideoArea.setBackgroundColor(getResources().getColor(R.color.transparent));
        //m_MyHandlerThread = new HandlerThread("VideoPreview");
        //m_MyHandlerThread.start();
		//m_MyHandler = new Handler(m_MyHandlerThread.getLooper()){
        m_MyHandler = new Handler(){
            @Override
            public void handleMessage(Message msg)
            {
                switch(msg.what)
                {
                    case InformPreViewMsg.MEDIA_PLAY:
                        if (msg.arg1==MbwType.TYPE_DEVICE_DMS)
                            m_FileType = NETWORK;
                        else
                            m_FileType = LOCAL;
                        if (msg.obj instanceof String)
                        {
                            if(VIDEO_PREVIEW_ENABLE)
                            {
                                Log.d(TAG,"Try to preview: "+(String)msg.obj);
                                if(UseRtMediaPlayer.getInstance(getActivity()).getUseRtPlayer()==true)
                                    m_VideoView_vv.useRTMediaPlayer(MediaPlayer.FORCE_RT_MEDIAPLAYER);
                                else
                                    m_VideoView_vv.useRTMediaPlayer(MediaPlayer.FORCE_ANDROID_MEDIAPLAYER);
                                m_VideoView_vv.enableVideoScaledSize(true);
                                m_VideoView_vv.setVideoURI(null);//setVideoURI(null) will let setVisibility(View.VISIBLE) do load media again with old VideoView.mUri,
                                m_VideoView_vv.setVisibility(View.VISIBLE);
                                m_VideoArea.setBackgroundColor(getResources().getColor(R.color.black));
                                m_ImageView_vv.setImageResource(R.color.black);
                                m_ImageView_vv.setVisibility(View.VISIBLE);
                                m_ProgressBar_video_progress.setVisibility(View.VISIBLE);
                                m_IsVideoMask = true;
                                setTextColor(msg.arg1);
                                if (m_InternalState !=PREVIEW_IDLE)
                                {
                                    if(m_VideoView_vv.isPlaying())
                                        m_VideoView_vv.stopPlayback();
                                    else
                                        m_VideoView_vv.suspend();
                                    m_InternalState = PREVIEW_IDLE;
                                }
                                m_VideoPath = (String)msg.obj;
                                m_VideoView_vv.setOnErrorListener(videoErrorListener);
                                m_VideoView_vv.setOnPreparedListener(videoPreparedListener);
                                m_VideoView_vv.setOnCompletionListener(videoCompletionListener);
                                m_VideoView_vv.setVideoPath(m_VideoPath);
                                m_InternalState = PREVIEW_PREPARING;
                                Log.d(TAG," SetVideoPath done");
                            }
                            else
                            {
                                Log.d(TAG,"Disable video preview.");
                            }
                            if (msg.peekData()!=null)
                            {
                                Bundle bundle = msg.getData();
                                String Size = bundle.getString("Size");
                                Log.d(TAG,"Get Video Size="+Size);
                                m_TextView_media_size.setVisibility(View.INVISIBLE);
                                m_TextView_media_size.setText(Size);
                            }
                        }
                        else
                        {
                            Log.e(TAG,"Can not load media without no media path");
                        }
                        break;
                    case InformPreViewMsg.PLAY_STOP:
                        if (m_InternalState !=PREVIEW_IDLE)
                        {
                            Log.d(TAG,"Stop video preview: ");
                            m_ImageView_vv.setImageResource(R.color.black);
                            if(m_VideoView_vv.isPlaying())
                            {
                                m_VideoView_vv.stopPlayback();
                            }
                            else
                            {
                                m_VideoView_vv.suspend();
                            }
                            m_ProgressBar_video_progress.setProgress(0);
                            m_TextView_media_elapsed_time.setText("--:--:--");
                            m_TextView_media_time.setText("--:--:--");
                            m_TextView_media_dimension.setText("");
                            m_TextView_media_size.setText("");
                            m_TextView_media_type.setText("");
                            m_VideoView_vv.setVisibility(View.INVISIBLE);
                            //m_VideoArea.setBackgroundColor(getResources().getColor(R.color.black));
                            if(timer != null)
                            {
                                timer.cancel();
                                timer = null;
                            }
                            m_InternalState = PREVIEW_IDLE;
                        }
                        break;
                    case InformPreViewMsg.PROGRESS_CHANGED:
                        int Cur = m_VideoView_vv.getCurrentPosition();
                        m_ProgressBar_video_progress.setProgress(Cur);
                        Cur/= 1000;
                        int minute = Cur/60;
                        int hour = minute/60;
                        int second = Cur%60;
                        minute %= 60;
                        m_TextView_media_elapsed_time.setText(String.format("%02d:%02d:%02d", hour,minute,second));
                        if ((m_VideoWidth <=0 || m_VideoHeight <=0)&& m_VideoView_vv.isPlaying())
                        {
                            m_VideoWidth = m_MediaPlayer.getVideoWidth();
                            m_VideoHeight = m_MediaPlayer.getVideoHeight();
                            Log.d(TAG,"Video Width: "+m_VideoWidth+" Height: "+m_VideoHeight);
                            if (m_VideoWidth >0 && m_VideoHeight > 0)
                            {
                                m_TextView_media_dimension.setText(String.format("%d X %d",m_VideoWidth ,m_VideoHeight));
                            }
                            else
                            {
                                m_TextView_media_dimension.setText("");
                            }
                        }
                        if (m_IsVideoMask){
                            m_ImageView_vv.setVisibility(View.INVISIBLE);
                            m_IsVideoMask = false;
                        }
                        if ((m_TextView_media_type.getText().length()<2) && m_VideoView_vv.isPlaying())
                        {
                            SetVideoInfoViaTrackInfo();
                        }
                        break;
                }
                super.handleMessage(msg);
            }
        };
        m_MyInformPreView = new InformPreView(m_MyHandler);
    }

	public boolean isPlaying(){
		return (m_VideoView_vv!=null && m_VideoView_vv.isPlaying());
	}
}
