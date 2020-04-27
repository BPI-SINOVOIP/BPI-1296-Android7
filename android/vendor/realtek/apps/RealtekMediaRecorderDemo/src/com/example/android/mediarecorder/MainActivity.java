/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.android.mediarecorder;

import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.Context;
import android.content.IntentFilter;
import android.annotation.TargetApi;
import android.app.Activity;
import android.hardware.Camera;
import android.media.CamcorderProfile;
import android.media.MediaRecorder;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.ParcelFileDescriptor;
import android.os.SystemClock;
import android.util.Log;
import android.view.Menu;
import android.view.TextureView;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.ArrayAdapter;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.CheckBox;
import android.widget.Toast;

import android.net.LocalSocket;
import android.net.LocalServerSocket;
import android.net.LocalSocketAddress;

import android.widget.LinearLayout;

import com.example.android.common.media.CameraHelper;

import java.io.IOException;
import java.io.File;
import java.io.InputStream;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.util.List;
import java.net.Socket;
import java.net.DatagramSocket;
import java.net.MulticastSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

import com.realtek.hardware.HDMIRxStatus;

/**
 *  This activity uses the camera/camcorder as the A/V source for the {@link android.media.MediaRecorder} API.
 *  A {@link android.view.TextureView} is used as the camera preview which limits the code to API 14+. This
 *  can be easily replaced with a {@link android.view.SurfaceView} to run on older devices.
 */
public class MainActivity extends Activity {

    private static final boolean USE_MULTICAST = true;

    private static final String IP="239.0.0.1";
    private static final String PORT="7890";

    private static final String LOCAL_SOCKET_NAME="RealtekMediaRecorderDemo.Socket";

    private static final int LOCAL_FILE = 0;
    private static final int LOCAL_FILE_SOCKET = 1;
    private static final int USB_FILE = 2;
    private static final int USB_FILE_SOCKET = 3;
    private static final int UDP_STREAMING = 4;
    private static final int UDP_STREAMING_SOCKET = 5;

    private Camera mCamera;
    //private TextureView mPreview;
    private SurfaceView mPreview;
    private MediaRecorder mMediaRecorder;

    private boolean isRecording = false;
    private static final String TAG = "Recorder";

    private LinearLayout mFileInfoGroup;
    private LinearLayout mUdpInfoGroup;

    private Button captureButton;
    private Spinner mOutputModeSpinner;
    private EditText mUdpIpEditText;
    private EditText mUdpPortEditText;
    private TextView mFileInfoTextView;
    private CheckBox mSurfaceCheckBox;

    private static int OUTPUT_FORMAT_MPEG2TS = 8;

    private int mOutputMode;
    private String mUdpIp;
    private int mUdpPort;
    private String mFilePath;
    private boolean mShowSurfaceView;

    private Socket socket;
    private ParcelFileDescriptor pfd;
    private DatagramSocket udpSocket;
    private MulticastSocket multicastSocket;

    // create a pipe to forward recorded data
    private ParcelFileDescriptor[] mPipe;
    private ParcelFileDescriptor mReadPfd;
    private ParcelFileDescriptor mWritePfd;

    /*
    private LocalSocket mWriteLocalSocket;
    private LocalSocket mReadLocalSocket;
    private LocalServerSocket mLocalServerSocket;
    */

    private boolean mLocalSocketTaskAlive;

    // local file + pipe mode
    private File mLocalSocketOutputFile;
    private FileOutputStream mFileOutputStream;

    // udp streming + pipe mode

    private String mActivieFilePath = "";

    private final Handler mHandler = new Handler();

    final RxBroadcastReceiver mRxBroadcastReceiver = new RxBroadcastReceiver();
    private final HDCPBroadcastReceiver mHDCPBroadcastReceiver = new HDCPBroadcastReceiver();

    private class RxBroadcastReceiver extends BroadcastReceiver {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if(action.equals("android.intent.action.HDMIRX_PLUGGED")){
                if (!(intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_PLUGGED_STATE, false))){
                    Toast.makeText(context,"no HDMIRx connected",Toast.LENGTH_SHORT).show();
                    finish();
                }else{
                }
            }
        }
    }
    
    private class HDCPBroadcastReceiver extends BroadcastReceiver {
        public void onReceive(Context context, Intent intent) {

            String action = intent.getAction();
            if(action.equals("android.intent.action.HDMIRX_HDCP_STATUS")){
                if (intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_HDCP_STATUS, false)){

					//Toast.makeText(context,"HDCP on",Toast.LENGTH_SHORT).show();
                    Log.d(TAG, "HDCP on");
                    if(isRecording&&mMediaRecorder!=null)
                    {
                        captureButton.setEnabled(false);

                        // stop recording and release camera
                        mMediaRecorder.stop();  // stop the recording
                        releaseMediaRecorder(); // release the MediaRecorder object
                        mCamera.lock();         // take camera access back from MediaRecorder

                        // inform the user that recording has stopped
                        setCaptureButtonText("Wait...");
                        isRecording = false;
                        releaseCamera();
                        // END_INCLUDE(stop_release_media_recorder)

                        if(mOutputMode>USB_FILE){
                      
							new DatagramSocketControlTask().execute(null, null, null);
                        }
                        // stop LocalSocket thread at least
                        mLocalSocketTaskAlive = false;
                        //captureButton.setEnabled(true);
                        startSuperFastClickProtector(captureButton,"Capture");                                                                                  
                    
						Toast.makeText(context,"record  off for HDCP on",Toast.LENGTH_SHORT).show();
					}
                }else{
                    Log.d(TAG, "HDCP off");
                    //Toast.makeText(context,"HDCP off",Toast.LENGTH_SHORT).show();
                }
            }
        }
    }
    private void startSuperFastClickProtector(View v, String str){
        final View v2 = v;
        final String str2 = str;
        // set disable the view first
        v2.setEnabled(false);
        mHandler.postDelayed(new Runnable() {
                public void run() {
                    v2.setEnabled(true);
                    setCaptureButtonText(str2);
                    //v2.setText(str2);
                }
            },3500);
    }

    private int getHDMIRxState(){
        try {
            BufferedReader br = new BufferedReader(new FileReader("/sys/devices/virtual/switch/rx_video/state"));
            String line = br.readLine().trim();
            Log.d(TAG, "getHDMIRxState line "+line);
            br.close();
            return Integer.parseInt(line);
        } catch (FileNotFoundException e){
            Log.e(TAG, "FileNotFoundException /sys/devices/virtual/switch/rx_video/state");
            return -1;
        } catch (IOException e) {
            Log.e(TAG, "IOException /sys/devices/virtual/switch/rx_video/state");
            return -1;
        } catch (NumberFormatException e) {
            Log.e(TAG, "NumberFormatException /sys/devices/virtual/switch/rx_video/state");
            return -1;
        }
    }


    protected void onResume(){
        super.onResume();
        int hdmiRxState = getHDMIRxState();
        if(hdmiRxState<=0){
            Toast.makeText(this,"no HDMIRx connected",Toast.LENGTH_SHORT).show();
            finish();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.sample_main);

        //mPreview = (TextureView) findViewById(R.id.surface_view);
        mPreview = (SurfaceView) findViewById(R.id.surface_view);
        captureButton = (Button) findViewById(R.id.button_capture);
        mOutputModeSpinner = (Spinner) findViewById(R.id.output_mode_spinner);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
                                    this,
                                    R.array.output_mode_item,
                                    android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mOutputModeSpinner.setAdapter(adapter);

        mUdpIpEditText = (EditText) findViewById(R.id.udp_ip_edittext);
        mUdpPortEditText = (EditText) findViewById(R.id.udp_port_edittext);

        if(USE_MULTICAST){
            mUdpIpEditText.setText(IP,TextView.BufferType.EDITABLE);
            mUdpPortEditText.setText(PORT,TextView.BufferType.EDITABLE);
        }else{
            mUdpIpEditText.setText("172.21.165.195",TextView.BufferType.EDITABLE);
            mUdpPortEditText.setText("1234",TextView.BufferType.EDITABLE);
        }

        mFileInfoTextView = (TextView) findViewById(R.id.file_info_textview);
        mFileInfoTextView.setText("");

        mFileInfoGroup = (LinearLayout) findViewById(R.id.file_control_group);
        mUdpInfoGroup = (LinearLayout) findViewById(R.id.udp_control_group);
        mSurfaceCheckBox = (CheckBox) findViewById(R.id.render_checkbox);
        mSurfaceCheckBox.setChecked(true);
        mSurfaceCheckBox.setVisibility(View.INVISIBLE);

        mOutputModeSpinner.setOnItemSelectedListener(new OnItemSelectedListener(){
            @Override
            public void onItemSelected(
                            AdapterView<?> parentView,
                            View selectedItemView,
                            int position,
                            long id) {
                //Log.d(TAG, "spinner selected position:"+position);

                if(position<= USB_FILE_SOCKET){
                    mFileInfoGroup.setVisibility(View.VISIBLE);
                    mUdpInfoGroup.setVisibility(View.INVISIBLE);
                }else{
                    mFileInfoGroup.setVisibility(View.INVISIBLE);
                    mUdpInfoGroup.setVisibility(View.VISIBLE);
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> parentView) {
            }
        });

        mOutputMode = mOutputModeSpinner.getSelectedItemPosition();

        if(mOutputMode<= USB_FILE){
            mFileInfoGroup.setVisibility(View.VISIBLE);
            mUdpInfoGroup.setVisibility(View.INVISIBLE);
        }else{
            mFileInfoGroup.setVisibility(View.INVISIBLE);
            mUdpInfoGroup.setVisibility(View.VISIBLE);
        }

        IntentFilter filter = new IntentFilter("android.intent.action.HDMIRX_PLUGGED");
        registerReceiver(mRxBroadcastReceiver,filter);
        IntentFilter filter_hdcp = new IntentFilter("android.intent.action.HDMIRX_HDCP_STATUS");    
        registerReceiver(mHDCPBroadcastReceiver,filter_hdcp);
    }

    protected void onDestroy(){
        super.onDestroy();
        unregisterReceiver(mRxBroadcastReceiver);   
        unregisterReceiver(mHDCPBroadcastReceiver);
    }

    private void updateRecordSettings(){
        mOutputMode = mOutputModeSpinner.getSelectedItemPosition();
        // udp ip, port
        mUdpIp = mUdpIpEditText.getText().toString();
        mUdpPort = Integer.parseInt(mUdpPortEditText.getText().toString());
        // SurfaceView
        mShowSurfaceView = mSurfaceCheckBox.isChecked();
        Log.d(TAG, "mOutputMode:"+mOutputMode+" UDP IP:"+mUdpIp+" Port:"+mUdpPort+" showSurfaceView:"+mShowSurfaceView);
    }

    /**
     * The capture button controls all user interaction. When recording, the button click
     * stops recording, releases {@link android.media.MediaRecorder} and {@link android.hardware.Camera}. When not recording,
     * it prepares the {@link android.media.MediaRecorder} and starts recording.
     *
     * @param view the view generating the event.
     */
    public void onCaptureClick(View view) {
        if (isRecording) {
            // BEGIN_INCLUDE(stop_release_media_recorder)
            captureButton.setEnabled(false);

            // stop recording and release camera
            mMediaRecorder.stop();  // stop the recording
            releaseMediaRecorder(); // release the MediaRecorder object
            mCamera.lock();         // take camera access back from MediaRecorder

            // inform the user that recording has stopped
            setCaptureButtonText("Wait...");
            isRecording = false;
            releaseCamera();
            // END_INCLUDE(stop_release_media_recorder)

            if(mOutputMode>USB_FILE){
                new DatagramSocketControlTask().execute(null, null, null);
            }
            // recover SurfaceView
            mPreview.setVisibility(View.VISIBLE);
            // stop LocalSocket thread at least
            mLocalSocketTaskAlive = false;
            //captureButton.setEnabled(true);
            startSuperFastClickProtector(captureButton,"Capture");
        } else {
            captureButton.setEnabled(false);
            captureButton.setText("Wait...");
            // UPDATE_RECORD_SETTING (do in main thread)
            updateRecordSettings();

            // update SurfaceView
            if(mShowSurfaceView)
                mPreview.setVisibility(View.VISIBLE);
            else
                mPreview.setVisibility(View.INVISIBLE);

            // BEGIN_INCLUDE(prepare_start_media_recorder)
            new MediaPrepareTask().execute(null, null, null);

            // END_INCLUDE(prepare_start_media_recorder)
            startSuperFastClickProtector(captureButton,"Stop");

        }
    }

    private void setCaptureButtonText(String title) {
        captureButton.setText(title);
    }

    @Override
    protected void onPause() {
        super.onPause();
        // if we are using MediaRecorder, release it first
        releaseMediaRecorder();
        // release the camera immediately on pause event
        releaseCamera();
        mLocalSocketTaskAlive = false;
        if(mOutputMode>USB_FILE){
            new DatagramSocketControlTask().execute(null, null, null);
        }
        finish();
    }

    private void releaseMediaRecorder(){
        if (mMediaRecorder != null) {
            // clear recorder configuration
            mMediaRecorder.reset();
            // release the recorder object
            mMediaRecorder.release();
            mMediaRecorder = null;
            // Lock camera for later use i.e taking it back from MediaRecorder.
            // MediaRecorder doesn't need it anymore and we will release it if the activity pauses.
            mCamera.lock();
        }
    }

    private void releaseCamera(){
        if (mCamera != null){
            // release the camera for other applications
            mCamera.release();
            mCamera = null;
        }
    }

    private boolean prepareIO(){
        // prepare pipe
        if(mOutputMode == LOCAL_FILE_SOCKET || mOutputMode == UDP_STREAMING_SOCKET || mOutputMode == USB_FILE_SOCKET){

            Log.d(TAG, "mode LOCAL_FILE_SOCKET or UDP_STREAMING_SOCKET USB_FILE_SOCKET, create pipe");

            try{
                mPipe = ParcelFileDescriptor.createPipe();
            }catch(IOException e){
                e.printStackTrace();
                return false;
            }

            mReadPfd = mPipe[0];
            mWritePfd = mPipe[1];

            mLocalSocketTaskAlive = true;

            /*
            try{
                mLocalServerSocket = new LocalServerSocket(LOCAL_SOCKET_NAME);
                mWriteLocalSocket = new LocalSocket();
                mWriteLocalSocket.connect(new LocalSocketAddress(LOCAL_SOCKET_NAME));
                mReadLocalSocket = mLocalServerSocket.accept();
                mLocalSocketTaskAlive = true;
            }catch(IOException e){
                e.printStackTrace();
                return false;
            }
            */
        }

        // file mode, no pipe
        if(mOutputMode == LOCAL_FILE || mOutputMode == USB_FILE){
            boolean external = false;
            if(mOutputMode == USB_FILE)
                external = true;

            mActivieFilePath = CameraHelper.getOutputMediaFile(
                                    CameraHelper.MEDIA_TYPE_VIDEO,
                                    external,
                                    this).toString();
            mMediaRecorder.setOutputFile(mActivieFilePath);
        }

        // udp mode (pipe or direct mode)
        if(mOutputMode == UDP_STREAMING || mOutputMode == UDP_STREAMING_SOCKET){
            if(USE_MULTICAST){
                try{
                    multicastSocket = new MulticastSocket(mUdpPort);
                    multicastSocket.setBroadcast(true);
                    multicastSocket.connect(InetAddress.getByName(mUdpIp),mUdpPort);
                    pfd = ParcelFileDescriptor.fromDatagramSocket(multicastSocket);
                }catch(SocketException e){
                    e.printStackTrace();
                    return false;
                }catch(UnknownHostException e){
                    e.printStackTrace();
                    return false;
                }catch(IOException e){
                    e.printStackTrace();
                    return false;
                }
            }else{
                try{
                    udpSocket = new DatagramSocket(mUdpPort);
                    udpSocket.setBroadcast(true);
                    udpSocket.connect(InetAddress.getByName(mUdpIp),mUdpPort);
                    pfd = ParcelFileDescriptor.fromDatagramSocket(udpSocket);
                }catch(SocketException e){
                    e.printStackTrace();
                    return false;
                }catch(UnknownHostException e){
                    e.printStackTrace();
                    return false;
                }
            }
            // direct mode
            if(mOutputMode == UDP_STREAMING){
                mMediaRecorder.setOutputFile(pfd.getFileDescriptor());
            }else{
                // pipe mode
                mMediaRecorder.setOutputFile(mWritePfd.getFileDescriptor());
                // create FileOutputStream from multicastSocket
                mFileOutputStream = new FileOutputStream(pfd.getFileDescriptor());

                // start read thread
                new LocalSocketReadTask().executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
            }
        }

        // local file + pipe / usb file + pipe
        if(mOutputMode == LOCAL_FILE_SOCKET || mOutputMode == USB_FILE_SOCKET){
            boolean external = (mOutputMode==LOCAL_FILE_SOCKET)?false:true;
            mActivieFilePath = CameraHelper.getOutputMediaFile(
                                    CameraHelper.MEDIA_TYPE_VIDEO,
                                    external,
                                    this).toString();

            // prepare FileOutputStream
            try{
                // open file
                mLocalSocketOutputFile = new File(mActivieFilePath);
                mFileOutputStream = new FileOutputStream(mLocalSocketOutputFile);
            }catch(IOException e){
                e.printStackTrace();
                return false;
            }

            // pass fd of mWritePfd to MediaRecorder
            mMediaRecorder.setOutputFile(mWritePfd.getFileDescriptor());
            // and create read task
            new LocalSocketReadTask().executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
        }

        return true;
    }

    @TargetApi(Build.VERSION_CODES.HONEYCOMB)
    private boolean prepareVideoRecorder(){

        Log.d(TAG, "prepareVideoRecorder");

        // BEGIN_INCLUDE (configure_preview)
        mCamera = CameraHelper.getDefaultCameraInstance();

        // We need to make sure that our preview and recording video size are supported by the
        // camera. Query camera to find all the sizes and choose the optimal size given the
        // dimensions of our preview surface.
        Camera.Parameters parameters = mCamera.getParameters();
        List<Camera.Size> mSupportedPreviewSizes = parameters.getSupportedPreviewSizes();
        //Camera.Size optimalSize = CameraHelper.getOptimalPreviewSize(mSupportedPreviewSizes, mPreview.getWidth(), mPreview.getHeight());
        Camera.Size optimalSize = CameraHelper.getOptimalPreviewSize(mSupportedPreviewSizes, 1920, 1080);
        int previewFrameRate = parameters.getPreviewFrameRate();
        int encodeBitRate = 5000000;

        int audioChannels = 2;
        int audioSampleRate = 48000;
        int audioBitRate = 64000;

        // Use the same size for recording profile.
        //CamcorderProfile profile = CamcorderProfile.get(CamcorderProfile.QUALITY_HIGH);
        //profile.videoFrameWidth = optimalSize.width;
        //profile.videoFrameHeight = optimalSize.height;

        // Realtek specific start
        //profile.videoFrameRate = 60;
        //profile.fileFormat = MediaRecorder.OutputFormat.OUTPUT_FORMAT_MPEG2TS;
        // Realtek specific finish

        //Log.d(TAG, "profile info fps:"+profile.videoFrameRate);

        // likewise for the camera object itself.
        // Bruce disable it right now...
        parameters.setPreviewSize(optimalSize.width, optimalSize.height);
        mCamera.setParameters(parameters);
        try {
            // Requires API level 11+, For backward compatibility use {@link setPreviewDisplay}
            // with {@link SurfaceView}
            //mCamera.setPreviewTexture(mPreview.getSurfaceTexture());
            mCamera.setPreviewDisplay(mPreview.getHolder());
        } catch (IOException e) {
            e.printStackTrace();
            Log.e(TAG, "Surface texture is unavailable or unsuitable:" + e.getMessage());
            return false;
        }
        // END_INCLUDE (configure_preview)


        // BEGIN_INCLUDE (configure_media_recorder)
        mMediaRecorder = new MediaRecorder();

        // Step 1: Unlock and set camera to MediaRecorder
        mCamera.unlock();
        mMediaRecorder.setCamera(mCamera);

        // Step 2: Set sources
        mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.HDMIRX);
        //mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.DEFAULT);

        mMediaRecorder.setVideoSource(MediaRecorder.VideoSource.CAMERA);

        // Step 3: Set a CamcorderProfile (requires API Level 8 or higher)
        //mMediaRecorder.setProfile(profile);
        mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.OUTPUT_FORMAT_MPEG2TS);

        // Step 4: Setup output
        boolean ioRst = prepareIO();

        if(ioRst==false)
            return false;

        mMediaRecorder.setVideoSize(optimalSize.width, optimalSize.height);
        mMediaRecorder.setVideoFrameRate(previewFrameRate);
        mMediaRecorder.setVideoEncodingBitRate(encodeBitRate);
        mMediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264);

        mMediaRecorder.setAudioChannels(audioChannels);
        mMediaRecorder.setAudioSamplingRate(audioSampleRate);
        mMediaRecorder.setAudioEncodingBitRate(audioBitRate);
        mMediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AAC);

        // END_INCLUDE (configure_media_recorder)

        // Step 5: Prepare configured MediaRecorder
        try {
            mMediaRecorder.prepare();
        } catch (IllegalStateException e) {
            Log.d(TAG, "IllegalStateException preparing MediaRecorder: " + e.getMessage());
            releaseMediaRecorder();
            return false;
        } catch (IOException e) {
            Log.d(TAG, "IOException preparing MediaRecorder: " + e.getMessage());
            releaseMediaRecorder();
            return false;
        }
        return true;
    }

    class LocalSocketReadTask extends AsyncTask<Void, Void, Boolean> {
        @Override
        protected Boolean doInBackground(Void... voids){
            Log.d(TAG, "LocalSocketReadTask doInBackground..");
            byte[] buffer = new byte[2048];
            // create InputStream from mReadPfd.
            InputStream ips = new ParcelFileDescriptor.AutoCloseInputStream(mReadPfd);

            while(mLocalSocketTaskAlive){
                // read from pipe read pfd
                try{
                    int data = ips.available();

                    if(data>0){
                        int readSize = ips.read(buffer);
                        if(/*mOutputMode == LOCAL_FILE_SOCKET &&*/ readSize>0){
                            mFileOutputStream.write(buffer,0,readSize);
                        }
                    }
                }catch(IOException e){
                    e.printStackTrace();
                    mLocalSocketTaskAlive = false;
                }
            }

            try{
                // always close FileOutputStream
                Log.d(TAG, "close file");
                mFileOutputStream.close();

                /*
                Log.d(TAG, "close read socket");
                mReadLocalSocket.close();
                SystemClock.sleep(50);
                Log.d(TAG, "close serversocket");
                mLocalServerSocket.close();
                */

                Log.d(TAG, "close pipe");
                mWritePfd.close();
                mReadPfd.close();

            }catch(IOException e){
                e.printStackTrace();
                return false;
            }
            return true;
        }
        @Override
        protected void onPostExecute(Boolean result){
            Log.d(TAG, "LocalSocketReadTask onPostExecute");
        }
    }

    /*
    class LocalSocketWriteTask extends AsyncTask<Void, Void, Boolean> {
        @Override
        protected Boolean doInBackground(Void... voids) {
            Log.d(TAG, "LocalSocketWriteTask doInBackground..");
            String message = "MediaRecorderTest";
            while(mLocalSocketTaskAlive){
                try{
                    mWriteLocalSocket.getOutputStream().write(message.getBytes());
                    // make it slow...
                    //Log.d(TAG, "Write LocalSocket complete");
                    SystemClock.sleep(1000);
                }catch(IOException e){
                    e.printStackTrace();
                    return false;
                }
            }

            try{
                mWriteLocalSocket.close();
            }catch(IOException e){
                e.printStackTrace();
            }
            return true;
        }

        @Override
        protected void onPostExecute(Boolean result){
        }
    }
    */

    class DatagramSocketControlTask extends AsyncTask<Void, Void, Boolean> {
        @Override
        protected Boolean doInBackground(Void... voids) {
            Log.d(TAG, "DatagramSocketControlTask doInBackground");
            if(USE_MULTICAST){
                if(multicastSocket!=null){
                    multicastSocket.disconnect();
                    multicastSocket.close();
                    multicastSocket = null;
                }
            }else{
                if(udpSocket!=null){
                    udpSocket.disconnect();
                    udpSocket.close();
                    udpSocket = null;
                }
            }
            return true;
        }

        @Override
        protected void onPostExecute(Boolean result) {
        }
    }

    /**
     * Asynchronous task for preparing the {@link android.media.MediaRecorder} since it's a long blocking
     * operation.
     */
    class MediaPrepareTask extends AsyncTask<Void, Void, Boolean> {

        @Override
        protected Boolean doInBackground(Void... voids) {

            Log.d(TAG, "MediaPrepareTask doInBackground");

            // initialize video camera
            if (prepareVideoRecorder()) {
                // Camera is available and unlocked, MediaRecorder is prepared,
                // now you can start recording
                mMediaRecorder.start();

                isRecording = true;
            } else {
                // prepare didn't work, release the camera
                releaseMediaRecorder();
                return false;
            }
            return true;
        }

        @Override
        protected void onPostExecute(Boolean result) {
            if (!result) {
                MainActivity.this.finish();
            }
            // inform the user that recording has started
            //setCaptureButtonText("Wait...");
            //captureButton.setEnabled(true);
            if(mOutputMode <= USB_FILE){
                mFileInfoTextView.setText(mActivieFilePath);
            }
        }
    }

}
