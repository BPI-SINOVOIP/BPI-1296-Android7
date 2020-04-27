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

public class MainActivity extends Activity {

    private static final String TAG = "FloatingWindow";

    private static final String START_SERVICE = "start floating window service";
    private static final String STOP_SERVICE = "stop floating window service";

    private static final String START_RECORD_SERVICE = "start background record service";
    private static final String STOP_RECORD_SERVICE = "stop background record service";

    private static final int DEMO_FLOATING = 0;
    private static final int DEMO_RECORD = 1;

    private Spinner mViewTypeSpinner;
    private Spinner mCameraTypeSpinner;
    private Spinner mDemoTypeSpinner;
    private Button mStartServiceButton;
    private Button mStartBackgroundRecordServiceButton;
    private CheckBox mRecordCheckBox;
    private CheckBox mShowWindowCheckBox;

    private TextView mViewTypeTextView;

    private ViewGroup mFloatingGroup;
    private ViewGroup mRecordGroup;

    private Context mContext;

    private final Handler mHandler = new Handler();
    private RtkHDMIRxManager mRxManager = new RtkHDMIRxManager();

    private final BroadcastReceiver mRxBroadcastReceiver = new RxBroadcastReceiver();

    class DemoSpinnerListener implements OnItemSelectedListener {
        public void onItemSelected (AdapterView<?> parent, 
                View view, 
                int position, 
                long id) {
            if(position==DEMO_FLOATING){
                mFloatingGroup.setVisibility(View.VISIBLE);
                mRecordGroup.setVisibility(View.INVISIBLE);
            }else{
                mFloatingGroup.setVisibility(View.INVISIBLE);
                mRecordGroup.setVisibility(View.VISIBLE);
            }
        }

        public void onNothingSelected (AdapterView<?> parent) {
            // do nothing..
        }
    }

    class StopServiceTask extends AsyncTask<Void, Void, Boolean> {
        protected Boolean doInBackground(Void... voids){

            if(BackgroundRecordService.bServiceIsAlive){
                Intent serviceIntent = new Intent(
                    mContext, com.realtek.floatingwindow.BackgroundRecordService.class);
                mContext.stopService(serviceIntent);
            }

            if(FloatingWindowService.bServiceIsAlive){
                // stop service
                Intent serviceIntent = new Intent(
                    mContext, com.realtek.floatingwindow.FloatingWindowService.class);
                mContext.stopService(serviceIntent);
            }
            return true;
        }

        protected void onPostExecute(Boolean result){
            Log.d(TAG, "StopServiceTask onPostExecute");
        }
    }

    private class RxBroadcastReceiver extends BroadcastReceiver {
        public void onReceive(Context context, Intent intent) {

            String action = intent.getAction();

            if(action.equals("android.intent.action.HDMIRX_PLUGGED")){
                if (!(intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_PLUGGED_STATE, false))){
                    Toast.makeText(context, "Rx is plugged out, goodbye...",Toast.LENGTH_SHORT).show();
                    //Log.e(TAG, "Rx is plugged out...");
                    // teardown any service
                    if(BackgroundRecordService.bServiceIsAlive){
                        Intent serviceIntent = new Intent(
                            mContext, com.realtek.floatingwindow.BackgroundRecordService.class);
                        mContext.stopService(serviceIntent);
                    }

                    if(FloatingWindowService.bServiceIsAlive){
                        // stop service
                        Intent serviceIntent = new Intent(
                            mContext, com.realtek.floatingwindow.FloatingWindowService.class);
                        mContext.stopService(serviceIntent);
                    }
                    // terminate app
                    finish();
                }
            }

            if(action.equals("android.hardware.usb.action.USB_CAMERA_DISCONNECTED")){
                Toast.makeText(context, "Camera is plugged out, goodbye...",Toast.LENGTH_SHORT).show();
                //Log.e(TAG, "Rx is plugged out...");
                // teardown any service
                if(BackgroundRecordService.bServiceIsAlive){
                    Intent serviceIntent = new Intent(
                        mContext, com.realtek.floatingwindow.BackgroundRecordService.class);
                    mContext.stopService(serviceIntent);
                }

                if(FloatingWindowService.bServiceIsAlive){
                    // stop service
                    Intent serviceIntent = new Intent(
                        mContext, com.realtek.floatingwindow.FloatingWindowService.class);
                    mContext.stopService(serviceIntent);
                }
                // terminate app
                finish();
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mContext = this;

        mFloatingGroup = (ViewGroup) findViewById(R.id.floating_window_group);
        mRecordGroup = (ViewGroup) findViewById(R.id.background_record_group);

        {
            mDemoTypeSpinner = (Spinner) findViewById(R.id.demo_spinner);
            ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
                                        this,
                                        R.array.demo_type_item,
                                        android.R.layout.simple_spinner_item);
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            mDemoTypeSpinner.setAdapter(adapter);
            mDemoTypeSpinner.setOnItemSelectedListener(new DemoSpinnerListener());
            // unselectable.
            mDemoTypeSpinner.setEnabled(false);
        }

        {
            mViewTypeTextView = (TextView) findViewById(R.id.view_type_textview);
            mViewTypeSpinner = (Spinner) findViewById(R.id.view_type_spinner);
            ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
                                        this,
                                        R.array.view_type_item,
                                        android.R.layout.simple_spinner_item);
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            mViewTypeSpinner.setAdapter(adapter);
            mViewTypeSpinner.setVisibility(View.INVISIBLE);
            mViewTypeTextView.setVisibility(View.INVISIBLE);
        }

        {
            mCameraTypeSpinner = (Spinner) findViewById(R.id.camera_spinner);
            ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
                                        this,
                                        R.array.camera_type_item,
                                        android.R.layout.simple_spinner_item);
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            mCameraTypeSpinner.setAdapter(adapter);
        }

        mStartServiceButton = (Button) findViewById(R.id.button_start_service);
        mStartServiceButton.setOnClickListener(new ButtonClickListener());

        mStartBackgroundRecordServiceButton = 
            (Button) findViewById(R.id.button_start_background_record_service);
        mStartBackgroundRecordServiceButton.setOnClickListener(new RecordButtonClickListener());
        //mStartBackgroundRecordServiceButton.setVisibility(View.INVISIBLE);

        mRecordCheckBox = (CheckBox) findViewById(R.id.record_checkbox);
        mRecordCheckBox.setVisibility(View.INVISIBLE);

        mShowWindowCheckBox = (CheckBox) findViewById(R.id.show_view_checkbox);
        mShowWindowCheckBox.setOnCheckedChangeListener(new ShowViewCheckBoxListener());
        mShowWindowCheckBox.setVisibility(View.INVISIBLE);

        /*
        IntentFilter filter = new IntentFilter("android.intent.action.HDMIRX_PLUGGED");
        filter.addAction("android.hardware.usb.action.USB_CAMERA_DISCONNECTED");
        registerReceiver(mRxBroadcastReceiver,filter);
        */
    }

    public void onDestroy() {
        super.onDestroy();
        /*
        unregisterReceiver(mRxBroadcastReceiver);
        */
    }

    protected void onResume(){
        super.onResume();
        // FIX : DHCKYLIN-648
        /*
        if(mRxManager.isHDMIRxPlugged() == false){
            Toast.makeText(this,"No HDMI is plugged, goodbye...",Toast.LENGTH_SHORT).show();
            finish();
        }
        */

        int demoType = mDemoTypeSpinner.getSelectedItemPosition();
        if(demoType==DEMO_FLOATING){
            mRecordGroup.setVisibility(View.INVISIBLE);
        }else{
            mFloatingGroup.setVisibility(View.INVISIBLE);
        }

        // update floating window button text
        if(FloatingWindowService.bServiceIsAlive){
            mStartServiceButton.setText(STOP_SERVICE);
        }else{
            mStartServiceButton.setText(START_SERVICE);
        }

        // update background record button text
        if(BackgroundRecordService.bServiceIsAlive){
            mStartBackgroundRecordServiceButton.setText(STOP_RECORD_SERVICE);
        }else{
            mStartBackgroundRecordServiceButton.setText(START_RECORD_SERVICE);
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

    class ShowViewCheckBoxListener implements CompoundButton.OnCheckedChangeListener{
        @Override
        public void onCheckedChanged(CompoundButton buttonView,boolean isChecked) {

            Log.d(TAG, "buttonView "+buttonView+" isChecked:"+isChecked);
            if(BackgroundRecordService.bServiceIsAlive){
                BackgroundRecordService.toggleShowView(isChecked);
            }
        }
    }

    /**
     * Background record button listener
     */
    class RecordButtonClickListener implements View.OnClickListener {
        @Override
        public void onClick(View v) {
            if(BackgroundRecordService.bServiceIsAlive){

                new StopServiceTask().execute(null,null,null);
                //Intent serviceIntent = new Intent(
                //    mContext, com.realtek.floatingwindow.BackgroundRecordService.class);
                //mContext.stopService(serviceIntent);
                mStartBackgroundRecordServiceButton.setText(START_RECORD_SERVICE);
                startSuperFastClickProtector(v);
            }else{
                Intent serviceIntent = new Intent(
                    mContext, com.realtek.floatingwindow.BackgroundRecordService.class);
                mContext.startService(serviceIntent);
                mStartBackgroundRecordServiceButton.setText(STOP_RECORD_SERVICE);
            }
        }
    }

    /**
     * Floating window button listener
     */
    class ButtonClickListener implements View.OnClickListener {
        @Override
        public void onClick(View v) {
            if(FloatingWindowService.bServiceIsAlive){
                // stop service
                new StopServiceTask().execute(null,null,null);
                //Intent serviceIntent = new Intent(
                //    mContext, com.realtek.floatingwindow.FloatingWindowService.class);
                //mContext.stopService(serviceIntent);
                mStartServiceButton.setText(START_SERVICE);
                startSuperFastClickProtector(v);
            }else{
                // start service
                Intent serviceIntent = new Intent(
                    mContext, com.realtek.floatingwindow.FloatingWindowService.class);

                int surfaceType = mViewTypeSpinner.getSelectedItemPosition();
                int redordOn = mRecordCheckBox.isChecked()?1:0;
                int cameraType = mCameraTypeSpinner.getSelectedItemPosition();

                serviceIntent.putExtra("surface",surfaceType);
                serviceIntent.putExtra("record",redordOn);
                serviceIntent.putExtra("camera",cameraType);

                mContext.startService(serviceIntent);
                mStartServiceButton.setText(STOP_SERVICE);
            }
        }
    }
}
