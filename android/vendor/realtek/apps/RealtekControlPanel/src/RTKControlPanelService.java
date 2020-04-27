package com.rtk.ControlPanel;
 
import android.app.ActivityManager;
import android.app.Service;
import android.app.Instrumentation;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Configuration;
import android.hardware.input.InputManager;
import android.os.Handler;
import android.os.IBinder;
import android.os.SystemClock;
import android.os.SystemProperties;
import android.util.Log;
import android.view.Gravity;
import android.view.InputDevice;
import android.view.KeyCharacterMap;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnHoverListener;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
import android.widget.ImageView;
import java.util.List;

public class RTKControlPanelService extends Service
{
    private static final String TAG = "ControlPanelService";
    private static final String CONTROL_PANEL_ACTION = "com.rtk.ControlPanel.ControlPanel_Action";
    private static final String CONTROL_PANEL_SHOW_ACTION = "com.rtk.ControlPanel.show";
    private static final String CONTROL_PANEL_HIDE_ACTION = "com.rtk.ControlPanel.hide";

    /**
     * RTKCOMMENT: intent to switch source between DTV/HDMI/Camera
     */
    public static final String SWITCH_SURFACEVIEW_WIDGET_SOURCE_INTENT="com.realtek.launcher.action.SWITCH_SURFACEVIEW_WIDGET_SOURCE";
    public static final String SOURCE_EXTRA_NAME=".source";

    private ControlPanelServiceReceiver controlPanelServiceReceiver;
    private View controlPanel;
    private long currentTimeMillis=0;
    private WindowManager wm=null;
    private WindowManager.LayoutParams wmParams=new WindowManager.LayoutParams();
    private final Handler mHandler = new Handler();
    private static final int GUI_DELAY_DURATION_MS = 3000;

    private ImageView panel_left_return;
    private ImageView panel_left_home;
    private ImageView panel_left_option;
    private ImageView panel_left_pip;
    private ImageView panel_left_source;
    private ImageView panel_left_search;

    private final Runnable mDisplayChecker = new Runnable() {
        @Override
        public void run() {
            if (controlPanel.isShown()) {
                showControlPanel(false);
            }
        }
    };

    @Override
    public void onCreate()
    {
        // TODO Auto-generated method stub
        super.onCreate();
        controlPanelServiceReceiver = new ControlPanelServiceReceiver();
        createView();
    }

    private void createView()
    {
        wm = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
        LayoutInflater li = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        controlPanel=li.inflate(R.layout.controlpanel, null);

        wmParams = new WindowManager.LayoutParams();
        wmParams.x=0;
        wmParams.y=0;
        wmParams.width = WindowManager.LayoutParams.MATCH_PARENT;
        wmParams.height = WindowManager.LayoutParams.MATCH_PARENT;
        wmParams.type=WindowManager.LayoutParams.TYPE_SYSTEM_ALERT;
        wmParams.format=1;
        wmParams.gravity=Gravity.LEFT|Gravity.TOP;
        wmParams.flags = WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL
        | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
        | WindowManager.LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH;
        wmParams.alpha=1f;//0.5f;
        wmParams.windowAnimations = R.style.PopupAnimation;

        setListener();
    }

    @Override
    public IBinder onBind(Intent arg0)
    {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId)
    {
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(CONTROL_PANEL_ACTION);
        registerReceiver(controlPanelServiceReceiver, intentFilter);
        return Service.START_STICKY;
    }

    @Override
    public void onDestroy()
    {
        this.unregisterReceiver(controlPanelServiceReceiver);
        showControlPanel(false);
        super.onDestroy();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        showControlPanel(false);
        createView();
    }

    public void setListener()
    {
        panel_left_return = (ImageView)controlPanel.findViewById(R.id.panel_left_return);
        panel_left_home = (ImageView)controlPanel.findViewById(R.id.panel_left_home);
        panel_left_option = (ImageView)controlPanel.findViewById(R.id.panel_left_option);
        panel_left_pip = (ImageView)controlPanel.findViewById(R.id.panel_left_pip);
        panel_left_source = (ImageView)controlPanel.findViewById(R.id.panel_left_source);
        panel_left_search = (ImageView)controlPanel.findViewById(R.id.panel_left_search);

        panel_left_return.setOnClickListener(new ControlPanelClickListener());
        panel_left_home.setOnClickListener(new ControlPanelClickListener());
        panel_left_option.setOnClickListener(new ControlPanelClickListener());
        panel_left_pip.setOnClickListener(new ControlPanelClickListener());
        panel_left_source.setOnClickListener(new ControlPanelClickListener());
        panel_left_search.setOnClickListener(new ControlPanelClickListener());

        panel_left_return.setOnHoverListener(new ControlPanelHoverListener());
        panel_left_home.setOnHoverListener(new ControlPanelHoverListener());
        panel_left_option.setOnHoverListener(new ControlPanelHoverListener());
        panel_left_pip.setOnHoverListener(new ControlPanelHoverListener());
        panel_left_source.setOnHoverListener(new ControlPanelHoverListener());
        panel_left_search.setOnHoverListener(new ControlPanelHoverListener());

        controlPanel.setOnTouchListener(new ControlPanelTouchListener());
    }

    public void showControlPanel(boolean bool)
    {
        if(bool==true)
        {
            if( controlPanel.isShown()==false )
            {
                Log.d(TAG,"Show RTK ControlPanel");
                SystemProperties.set("rtk.hwc.osd.hide", Long.toString(0));
                // send broadcast
                Intent intent = new Intent(CONTROL_PANEL_SHOW_ACTION);
                sendBroadcast(intent);
                if(checkZoomActivity()) {
                    panel_left_search.setImageResource(R.drawable.panel_left_zoom); 
                }
                else {
                    panel_left_search.setImageResource(R.drawable.panel_left_search); 
                }
                wm.addView(controlPanel, wmParams);
                mHandler.removeCallbacks(mDisplayChecker);
                mHandler.postDelayed(mDisplayChecker, GUI_DELAY_DURATION_MS);
            }
        }
        else
        {
            if( controlPanel.isShown()==true )
            {
                Log.d(TAG,"Hide RTK ControlPanel");
                wm.removeView(controlPanel);
                // send broadcast
                Intent intent = new Intent(CONTROL_PANEL_HIDE_ACTION);
                sendBroadcast(intent);
            }
        }
    }

    public boolean checkZoomActivity() {
        ActivityManager am = (ActivityManager) getSystemService(ACTIVITY_SERVICE);
        if(am==null)
            return false;

        // get the info from the currently running task
        List< ActivityManager.RunningTaskInfo > taskInfo = am.getRunningTasks(1);
        if(taskInfo==null || taskInfo.get(0)==null || taskInfo.get(0).topActivity==null)
            return false;

        String topActivity = taskInfo.get(0).topActivity.getClassName();
        if(topActivity!=null && topActivity.equals("com.android.gallery3d.app.GalleryActivity")) {
            return true;
        }
        else if(topActivity!=null && topActivity.equals("com.android.gallery3d.app.MovieActivity")) {
            return true;
        }
        return false;
    }

    public class ControlPanelServiceReceiver extends BroadcastReceiver
    {
        @Override
        public void onReceive(Context arg0, Intent arg1)
        {
            // TODO Auto-generated method stub
            if( arg1.getAction() == CONTROL_PANEL_ACTION )
            {
                long t2 = System.currentTimeMillis();
                if( t2-currentTimeMillis < 200 )
                {
                    return;
                }
                currentTimeMillis=t2;

                if( controlPanel.isShown()==false )
                {
                    showControlPanel(true);
                }
                else
                {
                    showControlPanel(false);
                }
            }
        }
    }

    private class ControlPanelTouchListener implements OnTouchListener
    {
        @Override
        public boolean onTouch(View v, MotionEvent event)
        {
            if(event.getAction()==MotionEvent.ACTION_DOWN)
            {
                switch (v.getId())
                {
                    case R.id.panel_left_return:
                    case R.id.panel_left_home:
                    case R.id.panel_left_option:
                    case R.id.panel_left_pip:
                    case R.id.panel_left_source:
                    case R.id.panel_left_search:
                        break;
                    default:
                        showControlPanel(false);
                        return true;
                }
            }
            return false;
        }
    }

    private class ControlPanelClickListener implements OnClickListener
    {
        @Override
        public void onClick(View v)
        {
            switch(v.getId())
            {
                case R.id.panel_left_return:
                    Log.d(TAG,"onClick BACK");
                    //Send back key
                    sendKeyEvent(KeyEvent.KEYCODE_BACK);
                    mHandler.removeCallbacks(mDisplayChecker);
                    mHandler.postDelayed(mDisplayChecker, GUI_DELAY_DURATION_MS);
                    break;
                case R.id.panel_left_home:
                    Log.d(TAG,"onClick HOME");
                    //Send home key
                    sendKeyEvent(KeyEvent.KEYCODE_HOME);
                    showControlPanel(false);
                    break;
                case R.id.panel_left_option:
                    Log.d(TAG,"onClick MENU");
                    //Send menu key
                    sendKeyEvent(KeyEvent.KEYCODE_MENU);
                    showControlPanel(false);
                    break;
                case R.id.panel_left_pip:
                    Log.d(TAG,"onClick PIP");
                    mHandler.removeCallbacks(mDisplayChecker);
                    mHandler.postDelayed(mDisplayChecker, GUI_DELAY_DURATION_MS);
                    break;
                case R.id.panel_left_source:
                    Log.d(TAG,"onClick SOURCE");
                    Intent i = new Intent(SWITCH_SURFACEVIEW_WIDGET_SOURCE_INTENT);
                    i.putExtra(SOURCE_EXTRA_NAME, "");
                    sendBroadcast(i);
                    mHandler.removeCallbacks(mDisplayChecker);
                    mHandler.postDelayed(mDisplayChecker, GUI_DELAY_DURATION_MS);
                    break;
                case R.id.panel_left_search:
                    Log.d(TAG,"onClick SEARCH");
                    sendKeyEvent(KeyEvent.KEYCODE_SEARCH);

                    showControlPanel(false);
                    break;
                default:
                    break;
            }
        }
    }

    private class ControlPanelHoverListener implements OnHoverListener
    {
        @Override
        public boolean onHover(View v, MotionEvent event)
        {
            boolean hasFocus=false;
            if(event.getAction() == MotionEvent.ACTION_HOVER_ENTER)
            {
                hasFocus=true;
            }
            else if(event.getAction() == MotionEvent.ACTION_HOVER_EXIT)
            {
                hasFocus=false;
            }
            else
            {
                return false;
            }

            switch( v.getId() )
            {
                case R.id.panel_left_return:
                    if( hasFocus )
                        panel_left_return.setImageResource(R.drawable.panel_left_return_focus);
                    else
                        panel_left_return.setImageResource(R.drawable.panel_left_return);
                    break;
                case R.id.panel_left_home:
                    if( hasFocus )
                        panel_left_home.setImageResource(R.drawable.panel_left_home_focus);
                    else
                        panel_left_home.setImageResource(R.drawable.panel_left_home);
                    break;
                case R.id.panel_left_option:
                    if( hasFocus )
                        panel_left_option.setImageResource(R.drawable.panel_left_option_focus);
                    else
                        panel_left_option.setImageResource(R.drawable.panel_left_option);
                    break;
                case R.id.panel_left_pip:
                    if( hasFocus )
                        panel_left_pip.setImageResource(R.drawable.panel_left_pip_focus);
                    else
                        panel_left_pip.setImageResource(R.drawable.panel_left_pip);
                    break;
                case R.id.panel_left_source:
                    if( hasFocus )
                        panel_left_source.setImageResource(R.drawable.panel_left_source_focus);
                    else
                        panel_left_source.setImageResource(R.drawable.panel_left_source);
                    break;
                case R.id.panel_left_search:
                    if(checkZoomActivity()) {
                        if( hasFocus )
                            panel_left_search.setImageResource(R.drawable.panel_left_zoom_focus);
                        else
                            panel_left_search.setImageResource(R.drawable.panel_left_zoom);
                    }
                    else {
                        if( hasFocus )
                            panel_left_search.setImageResource(R.drawable.panel_left_search_focus);
                        else
                            panel_left_search.setImageResource(R.drawable.panel_left_search);
                    }
                    break;
                default:
                    break;
            }
            return true;
        }
    }

    public void simulateKey(final int KeyCode) {
        new Thread() {
            @Override
            public void run()
            {
                try {
                    Instrumentation inst = new Instrumentation();
                    inst.sendKeyDownUpSync(KeyCode);
                } catch (Exception e) {
                    Log.e("Exception when sendKeyDownUpSync", e.toString());
                }
            }
        }.start();
    }

    void sendKeyEvent(int KeyCode) {
        sendEvent(KeyEvent.ACTION_DOWN, KeyCode);
        sendEvent(KeyEvent.ACTION_UP, KeyCode);
    }

    void sendEvent(int action, int KeyCode) {
        final KeyEvent ev = new KeyEvent(SystemClock.uptimeMillis(), SystemClock.uptimeMillis(), action, KeyCode, 0,
                0, KeyCharacterMap.VIRTUAL_KEYBOARD, 0,
                KeyEvent.FLAG_FROM_SYSTEM | KeyEvent.FLAG_VIRTUAL_HARD_KEY,
                InputDevice.SOURCE_KEYBOARD);
        InputManager.getInstance().injectInputEvent(ev,
                InputManager.INJECT_INPUT_EVENT_MODE_ASYNC);
    }
}
