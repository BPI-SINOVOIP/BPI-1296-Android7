package com.realtek.floatingwindow;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;
import android.app.Activity;
import android.os.Handler;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.SystemClock;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.ArrayAdapter;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.Toast;
import android.view.KeyEvent;
import com.realtek.hardware.RtkHDMIRxManager;
import com.realtek.hardware.HDMIRxStatus;
import com.realtek.floatingwindow.rtk.R;

public class MainActivity2 extends Activity {

    private static final String TAG = "FloatingWindow2";

    private static final String START_SERVICE = "start demo";
    private static final String STOP_SERVICE = "stop demo";

    private final Handler mHandler = new Handler();

    private Context mContext;

    private SettingWidgets mHDMIRxWidgets2;

    private Button mRecordButton;
    private Button mPreviewButton;
    private Button mButton;

    private ButtonClickListener mListener;

    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        mContext = this;
        setContentView(R.layout.activity_main_2);

        mHDMIRxWidgets2 = (SettingWidgets) findViewById(R.id.hdmi_rx_setting_2);

        mRecordButton = (Button) findViewById(R.id.record_button);
        mPreviewButton = (Button) findViewById(R.id.preview_button);
        mButton = (Button) findViewById(R.id.demo_button);

        mHDMIRxWidgets2.init("HDMIRx2");
        mHDMIRxWidgets2.setLabelText("HDMIRx (RtkHDMIRxManager)");

        // button setting
        if(!FloatingWindowService2.bServiceIsAlive){
            mButton.setText(START_SERVICE);
        }else{
            mButton.setText(STOP_SERVICE);
        }

        mRecordButton.setText("toggle record");
        mPreviewButton.setText("toggle preview");

        mListener = new ButtonClickListener();
        mButton.setOnClickListener(mListener);
        mRecordButton.setOnClickListener(mListener);
        mPreviewButton.setOnClickListener(mListener);
    }

    class StopServiceTask extends AsyncTask<Void, Void, Boolean> {
        protected Boolean doInBackground(Void... voids){
            if(FloatingWindowService2.bServiceIsAlive){
                // stop floating window service
                Intent serviceIntent = new Intent(
                    mContext, com.realtek.floatingwindow.FloatingWindowService2.class);
                mContext.stopService(serviceIntent);
            }
            return true;
        }

        protected void onPostExecute(Boolean result){
            Log.d(TAG, "StopServiceTask onPostExecute");
        }
    }

    private void processServiceButton(){
        if(!FloatingWindowService2.bServiceIsAlive){
            // get settings
            Bundle mRxConfig2 = mHDMIRxWidgets2.getSettingConfig();

            // start service
            Intent serviceIntent = new Intent(
                    mContext, com.realtek.floatingwindow.FloatingWindowService2.class);

            serviceIntent.putExtra("rx2",mRxConfig2);

            mContext.startService(serviceIntent);
            mButton.setText(STOP_SERVICE);
            startSuperFastClickProtector(mButton);

            mHDMIRxWidgets2.setEnabled(false);
        }else{
            // stop service
            new StopServiceTask().execute(null,null,null);
            mButton.setText(START_SERVICE);
            startSuperFastClickProtector(mButton);

            mHDMIRxWidgets2.setInfoText("");

            mHDMIRxWidgets2.setEnabled(true);
        }
    }

    /**
     * Floating window button listener
     */
    class ButtonClickListener implements View.OnClickListener {
        @Override
        public void onClick(View v) {
            if(v.equals(mButton)){
                processServiceButton();
            }else if(v.equals(mRecordButton)){
                Log.d(TAG, "toggle record");
                FloatingWindowService2.toggleRecord();
            }else if(v.equals(mPreviewButton)){
                Log.d(TAG, "toggle preview");
                FloatingWindowService2.togglePreview();
            }
        }
    }

    private void startSuperFastClickProtector(View v){
        final View v2 = v;
        // set disable the view first
        v2.setEnabled(false);
        mHandler.postDelayed(new Runnable() {
                public void run() {
                    v2.setEnabled(true);
                }
            },500);
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event){
        boolean retval = false;
        // toggle preview
        if( event.getKeyCode() == KeyEvent.KEYCODE_DPAD_LEFT){
            if(event.getAction() == KeyEvent.ACTION_UP){
                FloatingWindowService2.togglePreview();
            }
            //retval = true;
        }
        // toggle record
        if( event.getKeyCode() == KeyEvent.KEYCODE_DPAD_RIGHT){
            if(event.getAction() == KeyEvent.ACTION_UP){
                FloatingWindowService2.toggleRecord();
            }
            //retval = true;
        }
        if(retval==false){
            retval = super.dispatchKeyEvent(event);
        }
        return retval;
    }

    public void onPause(){
        super.onPause();
        FloatingWindowService2.onActivityPauseResume(false);
    }

    public void onResume(){
        super.onResume();
        FloatingWindowService2.onActivityPauseResume(true);
    }
}
