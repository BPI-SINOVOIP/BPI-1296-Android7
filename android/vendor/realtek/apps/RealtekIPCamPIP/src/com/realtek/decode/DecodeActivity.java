package com.realtek.decode;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;

import android.app.Activity;
import android.content.Intent;
import android.icu.text.SimpleDateFormat;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnBufferingUpdateListener;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemProperties;
import android.util.Log;
import android.view.Display;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout; 
import android.widget.ProgressBar;
import android.widget.Toast;

public class DecodeActivity extends Activity implements SurfaceHolder.Callback {
	public static final String TAG = "DecodeActivity";
    private static final int MAX_GRID_NUM = 4;
	private int gridNum = 4;
	private int surfaceCount = 0;
    private int windowWight;
    private int windowHeight;
    private String defaultsearchI = null;
    private int selectedVideoIndex = -1;
	private int surfaceViewArray[] = new int[MAX_GRID_NUM];
	private SurfaceView mSurfaceView[] = new SurfaceView[MAX_GRID_NUM];
	private SurfaceHolder mSurfaceHolder[] = new SurfaceHolder[MAX_GRID_NUM];
	private Player mediaPlayer[] = new Player[MAX_GRID_NUM];
	private ArrayList<String> playList = null;
	private String urlList[] = new String[MAX_GRID_NUM];
	private Handler mHandler = null;
    private int ProgressBarArray[] = new int[MAX_GRID_NUM];
    private ProgressBar videoProgressBar[] = new ProgressBar[MAX_GRID_NUM];
    private int relativeLayoutArray[] = new int[MAX_GRID_NUM];
    private RelativeLayout mRelativeLayout[] = new RelativeLayout[MAX_GRID_NUM];
    private int searchI = 0;
    
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate");
        
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
                             WindowManager.LayoutParams.FLAG_FULLSCREEN);
                             
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, 
                             WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
                             
        Display display = getWindowManager().getDefaultDisplay();
        if(display != null) {
            windowWight = display.getWidth();
            windowHeight = display.getHeight();
        }
        
        //SystemProperties.set("persist.media.USE_RTMediaPlayer", "true");
        
        mHandler = new Handler();
        Intent it = getIntent();
        Bundle bundle = it.getExtras();
        if(bundle != null) {
            gridNum = bundle.getInt("DATA_INT");
            playList = bundle.getStringArrayList("DATA_URL");
            searchI = bundle.getInt("SEARCH_I");
        }
        if(searchI == 1) {
            defaultsearchI = SystemProperties.get("persist.rtk.searchI");
            SystemProperties.set("persist.rtk.searchI", Long.toString(1));            
        }
        for(int i=0; i<MAX_GRID_NUM; i++) {
            if(playList != null) {
                urlList[i] = playList.get(i);
            } else {
                urlList[i] = "invalid url";
            }
            Log.d(TAG, "Source " + i + " = " + urlList[i]);
        }
               
        switch (gridNum) {
        	case 1:
        		setContentView(R.layout.recgrid1);
        		break;
        	case 2:
        		setContentView(R.layout.recgrid2);
        		break;
        	case 3:
        		setContentView(R.layout.recgrid3);
        		break;
        	case 4:
        		setContentView(R.layout.recgrid4);
        		break;
        	default:
        		setContentView(R.layout.recgrid4);
        		break;
        }
        
        setupViewArray();
        setupViewComponent();
    }
    @Override
    protected void onPause() {
        // TODO Auto-generated method stub
        for(int i=0; i<gridNum; ++i) {
            mediaPlayer[i].stop();
        }
        if(searchI == 1) {
            SystemProperties.set("persist.rtk.searchI", defaultsearchI);
        }
        super.onPause();
    }
    
    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
    }
    private void setupViewArray() {

        if (gridNum >= 1) {
            surfaceViewArray[0] = R.id.surfaceView1;
            ProgressBarArray[0] = R.id.video_progressBar1;
            relativeLayoutArray[0] = R.id.layout1;
        }

        if (gridNum >= 2) {
            surfaceViewArray[1] = R.id.surfaceView2;
            ProgressBarArray[1] = R.id.video_progressBar2;
            relativeLayoutArray[1] = R.id.layout2;
        }
        if (gridNum >= 3) {
            surfaceViewArray[2] = R.id.surfaceView3;
            ProgressBarArray[2] = R.id.video_progressBar3;
            relativeLayoutArray[2] = R.id.layout3;
        }
        if (gridNum >= 4) {       
            surfaceViewArray[3] = R.id.surfaceView4;
            ProgressBarArray[3] = R.id.video_progressBar4;
            relativeLayoutArray[3] = R.id.layout4;
        }
    }
    private void setupViewComponent() {
        Log.d(TAG, "gridNum " + gridNum);
        for (int i = 0; i < gridNum; i++) {
            mSurfaceView[i] = (SurfaceView) findViewById(surfaceViewArray[i]);
            mSurfaceHolder[i] = mSurfaceView[i].getHolder();
            mSurfaceHolder[i].setFixedSize(1920, 1080);
            mSurfaceHolder[i].addCallback(this); 
            mSurfaceHolder[i].setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
            
            videoProgressBar[i] = (ProgressBar) findViewById(ProgressBarArray[i]);           
            mRelativeLayout[i] = (RelativeLayout) findViewById(relativeLayoutArray[i]);
            if(gridNum == 1)
                setLayoutSize(i, windowWight, windowHeight);
            if(gridNum == 2)
                setLayoutSize(i, windowWight/2, windowHeight);
            if(gridNum == 3)
                setLayoutSize(i, windowWight/3, windowHeight);
            if(gridNum == 4)
                setLayoutSize(i, windowWight/2, windowHeight/2);            
        }
    }
    private void setLayoutSize(int index, int width, int height) {  
        if(gridNum == 4) {    
            RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams(width, height);       
            switch(index) {
                case 0:
                    param.addRule(RelativeLayout.ALIGN_PARENT_LEFT, 1);
                    param.addRule(RelativeLayout.ALIGN_PARENT_TOP, 1);
                    break;
                case 1:
                    param.addRule(RelativeLayout.ALIGN_PARENT_LEFT, 1);
                    param.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, 1);
                    break;
                case 2:
                    param.addRule(RelativeLayout.ALIGN_PARENT_RIGHT, 1);
                    param.addRule(RelativeLayout.ALIGN_PARENT_TOP, 1);
                    break;
                case 3:
                    param.addRule(RelativeLayout.ALIGN_PARENT_RIGHT, 1);
                    param.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, 1);
                    break;       
            }
            mRelativeLayout[index].setLayoutParams(param);
        }
        else {
            RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams(width, height);
            switch(index) {
                case 0:
                    param.addRule(RelativeLayout.ALIGN_PARENT_LEFT, 1);
                    param.addRule(RelativeLayout.ALIGN_PARENT_TOP, 1);
                    break;
                case 1:
                    param.addRule(RelativeLayout.RIGHT_OF, relativeLayoutArray[0]);
                    //param.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, 1);
                    break;
                case 2:
                    param.addRule(RelativeLayout.RIGHT_OF, relativeLayoutArray[1]);
                    //param.addRule(RelativeLayout.ALIGN_PARENT_TOP, 1);
                    break;
                case 3:
                    param.addRule(RelativeLayout.RIGHT_OF, relativeLayoutArray[2]);
                    //param.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, 1);
                    break;       
            }
            mRelativeLayout[index].setLayoutParams(param);            
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
    	Log.d(TAG, "surfaceCreated ......... !!!");
        
        if(surfaceCount == gridNum)
            return;

        new decodeThread(surfaceCount).start();
        surfaceCount = surfaceCount + 1;
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
    	Log.d(TAG, "surfaceChanged ......... !!!");

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {   
        Log.d(TAG, "SufaceDestroyed ......... !!!");
    }
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // TODO Auto-generated method stub
        int action = event.getAction();
        if (action == KeyEvent.ACTION_DOWN) {
            switch(keyCode) {
                case KeyEvent.KEYCODE_1:
                case KeyEvent.KEYCODE_NUMPAD_1:
                case KeyEvent.KEYCODE_DPAD_UP:
                    if(selectedVideoIndex >= 0)
                        break;
                    if(gridNum < 1) {
                        Toast.makeText(DecodeActivity.this, "Please select 1 to "+gridNum, Toast.LENGTH_SHORT).show();
                        break;
                    }

                    selectedVideoIndex = 0;
                    for (int i = 0; i < gridNum; i++) {
                        if(i == 0) {
                            setLayoutSize(0, windowWight, windowHeight);
                        } else {
                            mediaPlayer[i].setVolume(0, 0);
                            setLayoutSize(i, 0, 0);                            
                        }
                    }                    
                    break;
                case KeyEvent.KEYCODE_2:
                case KeyEvent.KEYCODE_NUMPAD_2:
                case KeyEvent.KEYCODE_DPAD_DOWN:
                    if(selectedVideoIndex >= 0)
                        break;                
                    if(gridNum < 2) {
                        Toast.makeText(DecodeActivity.this, "Please select 1 to "+gridNum, Toast.LENGTH_SHORT).show();
                        break;
                    }

                    selectedVideoIndex = 1; 
                    for (int i = 0; i < gridNum; i++) {
                        if(i == 1) {
                            setLayoutSize(1, windowWight, windowHeight);
                        } else {
                            mediaPlayer[i].setVolume(0, 0);
                            setLayoutSize(i, 0, 0);                            
                        }
                    }                    
                    break;
                case KeyEvent.KEYCODE_3:
                case KeyEvent.KEYCODE_NUMPAD_3:
                case KeyEvent.KEYCODE_DPAD_LEFT:
                    if(selectedVideoIndex >= 0)
                        break;                
                    if(gridNum < 3) {
                        Toast.makeText(DecodeActivity.this, "Please select 1 to "+gridNum, Toast.LENGTH_SHORT).show();
                        break;
                    }

                    selectedVideoIndex = 2;
                    for (int i = 0; i < gridNum; i++) {
                        if(i == 2) {
                            setLayoutSize(2, windowWight, windowHeight);
                        } else {
                            mediaPlayer[i].setVolume(0, 0);
                            setLayoutSize(i, 0, 0);                            
                        }
                    }                    
                    break;
                case KeyEvent.KEYCODE_4:
                case KeyEvent.KEYCODE_NUMPAD_4:
                case KeyEvent.KEYCODE_DPAD_RIGHT:
                    if(selectedVideoIndex >= 0)
                        break;
                    if(gridNum < 4) {
                        Toast.makeText(DecodeActivity.this, "Please select 1 to "+gridNum, Toast.LENGTH_SHORT).show();
                        break;
                    }

                    selectedVideoIndex = 3;
                    for (int i = 0; i < gridNum; i++) {
                        if(i == 3) {
                            setLayoutSize(3, windowWight, windowHeight);
                        } else {
                            mediaPlayer[i].setVolume(0, 0);
                            setLayoutSize(i, 0, 0);                            
                        }
                    }
                    break;
                case KeyEvent.KEYCODE_BACK:
                case KeyEvent.KEYCODE_ESCAPE:
                    if(selectedVideoIndex >= 0) {                       
                        for (int i = 0; i < gridNum; i++) {
                            if(i != selectedVideoIndex)
                                mediaPlayer[i].setVolume(1, 1);
                            if(gridNum == 1)
                                setLayoutSize(i, windowWight, windowHeight);
                            if(gridNum == 2)
                                setLayoutSize(i, windowWight/2, windowHeight);
                            if(gridNum == 3)
                                setLayoutSize(i, windowWight/3, windowHeight);
                            if(gridNum == 4)
                                setLayoutSize(i, windowWight/2, windowHeight/2);            
                        }
                        selectedVideoIndex = -1;
                        return true;
                    }
                    break;
                default:
                    if(selectedVideoIndex >= 0)
                        break;
                    if(keyCode != KeyEvent.KEYCODE_BACK && keyCode != KeyEvent.KEYCODE_ESCAPE) {
                        Toast.makeText(DecodeActivity.this, "Please select 1 to "+gridNum, Toast.LENGTH_SHORT).show();
                    }                   
            }
        }
        
        return super.onKeyDown(keyCode, event);
    }
    public class decodeThread extends Thread {
        private int index;
        public decodeThread(int index) {
            this.index = index;
        }
        @Override
        public void run() {        	
    		try {                  
    			mediaPlayer[index] = new Player(mSurfaceView[index], index+1);
    			mediaPlayer[index].initPlayer();
                mediaPlayer[index].playUrl(urlList[index]);
                mHandler.post(new ShowProgressBar(index));
                //mediaPlayer[index].start();
    		}catch (Exception e) {
    			e.printStackTrace();
    		}
            
            while(mediaPlayer[index].getCurrentPosition() < 20) {
                try {
                    Thread.sleep(2);
                    //Log.d(TAG, "wait loading..." + mediaPlayer[i].getCurrentPosition());
                } catch(Exception e) {
                    e.printStackTrace();
                }
                
            }
            mHandler.post(new ShutProgressBar(index));
            SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS"); 
            String time = format.format(new Date());
            int num = index +1;
            Log.d(TAG, "getCurrentPosition() >=20ms MediaPlayer #"+num+" : "+time);
        }
    }
    
    public class ShutProgressBar extends Thread {
        private int index;
        public ShutProgressBar(int index) {
            this.index = index;
        }
        @Override
        public void run() {
            Log.d(TAG, "ShutProgressBar:"+index);
            videoProgressBar[index].setVisibility(View.GONE);
        }
    }
    public class ShowProgressBar extends Thread {
        private int index;
        public ShowProgressBar(int index) {
            this.index = index;
        }
        @Override
        public void run() {
            Log.d(TAG, "ShowProgressBar:"+index);
            videoProgressBar[index].setVisibility(View.VISIBLE);
        }
    }
}