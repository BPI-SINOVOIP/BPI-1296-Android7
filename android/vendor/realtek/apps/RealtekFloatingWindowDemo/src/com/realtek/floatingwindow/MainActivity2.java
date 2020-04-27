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
import com.realtek.hardware.RtkHDMIRxManager;
import com.realtek.hardware.HDMIRxStatus;

public class MainActivity2 extends Activity {

    private static final String TAG = "FloatingWindow2";

    private static final String START_SERVICE = "start floating window service";
    private static final String STOP_SERVICE = "stop floating window service";

    private final Handler mHandler = new Handler();

    private Context mContext;

    private SettingWidgets mHDMIRxWidgets;
    private SettingWidgets mUSBCamWidgets;
    private SettingWidgets2 mMediaPlayerWidgets2;

    private Button mButton;

    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        mContext = this;
        setContentView(R.layout.activity_main_2);

        mHDMIRxWidgets = (SettingWidgets) findViewById(R.id.hdmi_rx_setting);
        mUSBCamWidgets = (SettingWidgets) findViewById(R.id.usb_camera_setting);
        mMediaPlayerWidgets2 = (SettingWidgets2) findViewById(R.id.media_player_setting_2);

        mButton = (Button) findViewById(R.id.demo_button);

        mHDMIRxWidgets.init(true,"HDMIRx");
        mUSBCamWidgets.init(true,"USBCam");
        mMediaPlayerWidgets2.init();

        mHDMIRxWidgets.setLabelText("HDMIRx");
        mUSBCamWidgets.setLabelText("USB Camera");
        // button setting
        if(!FloatingWindowService2.bServiceIsAlive){
            mButton.setText(START_SERVICE);
        }else{
            mButton.setText(STOP_SERVICE);
        }
        mButton.setOnClickListener(new ButtonClickListener());
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

    /**
     * Floating window button listener
     */
    class ButtonClickListener implements View.OnClickListener {
        @Override
        public void onClick(View v) {

            if(!FloatingWindowService2.bServiceIsAlive){
                // get settings
                Bundle mRxConfig = mHDMIRxWidgets.getSettingConfig();
                Bundle mUSBCamConfig = mUSBCamWidgets.getSettingConfig();
                Bundle mMPConfig = mMediaPlayerWidgets2.getSettingConfig();


                // start service
                Intent serviceIntent = new Intent(
                    mContext, com.realtek.floatingwindow.FloatingWindowService2.class);

                serviceIntent.putExtra("rx",mRxConfig);
                serviceIntent.putExtra("usbcam",mUSBCamConfig);
                serviceIntent.putExtra("mp",mMPConfig);

                mContext.startService(serviceIntent);
                mButton.setText(STOP_SERVICE);
                startSuperFastClickProtector(v);

                // currently, info text only shows record path
                boolean rxEnabled = mRxConfig.getBoolean(SettingWidgets.KEY_ENABLED);
                if(rxEnabled){
                    String rxInfo = mRxConfig.getString(SettingWidgets.KEY_RECORD_PATH);
                    mHDMIRxWidgets.setInfoText(rxInfo);
                }

                boolean camEnabled = mUSBCamConfig.getBoolean(SettingWidgets.KEY_ENABLED);
                if(camEnabled){
                    String camInfo = mUSBCamConfig.getString(SettingWidgets.KEY_RECORD_PATH);
                    mUSBCamWidgets.setInfoText(camInfo);
                }

                mHDMIRxWidgets.setEnabled(false);
                mUSBCamWidgets.setEnabled(false);
                mMediaPlayerWidgets2.setEnabled(false);
            }else{
                // stop service
                new StopServiceTask().execute(null,null,null);
                mButton.setText(START_SERVICE);
                startSuperFastClickProtector(v);
                mHDMIRxWidgets.setInfoText("");
                mUSBCamWidgets.setInfoText("");

                mHDMIRxWidgets.setEnabled(true);
                mUSBCamWidgets.setEnabled(true);
                mMediaPlayerWidgets2.setEnabled(true);
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
}
