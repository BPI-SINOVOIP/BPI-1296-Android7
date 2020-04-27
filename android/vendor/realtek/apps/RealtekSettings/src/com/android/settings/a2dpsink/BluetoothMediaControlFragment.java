package com.android.settings.a2dpsink;

import java.util.Set;
import java.util.List;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.bluetooth.BluetoothA2dp;
import android.bluetooth.BluetoothA2dpSink;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothAvrcp;
import android.bluetooth.BluetoothAvrcpController;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothProfile;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.ShapeDrawable;
import android.media.MediaMetadata;
import android.media.MediaDescription;
import android.media.session.PlaybackState;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.android.settings.R;

public final class BluetoothMediaControlFragment extends DialogFragment {

    private AlertDialog mAlertDialog;
    static final String TAG = "BluetoothMediaControlFragment";

    public static final int A2DP_SOURCE_STATE_DISCONNECT = 0;
    public static final int A2DP_SOURCE_STATE_CONNECTING = 1;
    public static final int A2DP_SOURCE_STATE_CONNECTED = 2;
    public static final int A2DP_SOURCE_STATE_DISCONNECTING = 3;
    public static final int A2DP_SOURCE_STATE_UNKNOWN = 4;
    private int BluetoothAvrcpPress = 0;
    private int BluetoothAvrcpRelease = 1;

    public static final int PLAYING = 3;
    public static final int PAUSE = 2;

    private BluetoothAdapter mAdapter;
    private BluetoothA2dp mBluetoothA2dp;
    private BluetoothA2dpSink mBluetoothA2dpSink;
    private BluetoothAvrcpController mBluetoothAvrcp_contrl;
    public static BluetoothDevice mBluetoothdev;
    private MediaMetadata mMetaData = null;
    private PlaybackState mState = null;

    private CDView mCdView;
    private TextView mMusicTitle;
    private TextView mSingerName;
    private SeekBar mPlaySeekBar;
    private TextView mCurMusicTime;
    private TextView mTotalMusicTime;
    private ImageButton mPreviousButton;
    private ImageButton mNextButton;
    private ImageButton mPlayPauseButton;

    private int screenWidth = 0;
    private String musicTitle = null;
    private String singerName = null;
    private String UNKNOWN_SONG_NAME = "Unknown Song";
    private String UNKNOWN_SINGER = "Unknown Singer";
    private long duration = 0;
    private long curpos = 0l;

    private boolean isPlaying = true;
    private Bitmap bmp = null;
    private int max = 0;
    private boolean isHasPos = true;

    private final BroadcastReceiver mA2dpSourceStateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(BluetoothA2dpSink.ACTION_CONNECTION_STATE_CHANGED)) {
                int A2dpSourceState = intent.getIntExtra(BluetoothProfile.EXTRA_STATE,A2DP_SOURCE_STATE_UNKNOWN);
                Log.e(TAG, "receive a2dp sink intent state:" + A2dpSourceState);
                if(A2dpSourceState == BluetoothProfile.STATE_CONNECTED){
                    Log.e(TAG, "sink device state connect");
                }

                if(A2dpSourceState == BluetoothProfile.STATE_DISCONNECTED){
                    Log.e(TAG, "sink device state disconnect");
                    Toast.makeText(getContext(), "Connected device disconnect,Please connect again", Toast.LENGTH_LONG).show();
                    dismiss();
                }
            }
            if (intent.getAction().equals(BluetoothAvrcpController.ACTION_TRACK_EVENT)) {
                mMetaData = (MediaMetadata)intent.getExtra(BluetoothAvrcpController.EXTRA_METADATA, null);
                mState = (PlaybackState)intent.getExtra(BluetoothAvrcpController.EXTRA_PLAYBACK,null);
                if (mMetaData != null) {
                    //Log.d(TAG,"metadata change, media info is "+mMetaData.getDescription());
                    updateSongdec(mMetaData);
                }
                if (mState != null) {
                    updatePlayPauseButton(mState);
                }
            }
        }
    };

    private void updateSongdec(MediaMetadata mMetaData) {
        if (mMetaData != null && mBluetoothdev != null) {
            musicTitle = (String)mMetaData.getDescription().getTitle();
            singerName = (String)mMetaData.getDescription().getSubtitle();
            mMusicTitle.setText(musicTitle != null? musicTitle : UNKNOWN_SONG_NAME);
            mSingerName.setText(singerName != null? singerName : UNKNOWN_SINGER);
            duration = (long)mMetaData.getLong(MediaMetadata.METADATA_KEY_DURATION);
            curpos = mBluetoothAvrcp_contrl.getPlaybackState(mBluetoothdev).getPosition();
            if (duration > 1000) {
                if (isHasPos) {
                    mCurMusicTime.setText(changeToTimeFormat(curpos));
                    mTotalMusicTime.setText(changeToTimeFormat(duration));
                } else {
                    isHasPos = true;
                    mPlaySeekBar.setVisibility(View.VISIBLE);
                    mCurMusicTime.setVisibility(View.VISIBLE);
                    mTotalMusicTime.setVisibility(View.VISIBLE);
                    mCurMusicTime.setText(changeToTimeFormat(curpos));
                    mTotalMusicTime.setText(changeToTimeFormat(duration));
                }
            }
            bmp =  mBluetoothAvrcp_contrl.getMetadata(mBluetoothdev).getDescription().getIconBitmap();
            if(bmp == null) bmp = BitmapFactory.decodeResource(getResources(), R.drawable.cd_default);
            mCdView.setImage(Bitmap.createScaledBitmap(bmp, (int)(screenWidth * 0.05), (int)(screenWidth * 0.05), true));
        }
    }

    private void updatePlayPauseButton(PlaybackState mState) {
        if (mBluetoothAvrcp_contrl != null) {
            int state = mState.getState();
            curpos = mState.getPosition();
            if (mBluetoothdev != null) {
                if ((mBluetoothAvrcp_contrl.getMetadata(mBluetoothdev)) != null) {
                    duration = mBluetoothAvrcp_contrl.getMetadata(mBluetoothdev).getLong(MediaMetadata.METADATA_KEY_DURATION);
                } else {
                    duration = 0;
                }
                if (curpos > 0 && duration > 0){
                    if (!isHasPos) {
                        isHasPos = true;
                        mPlaySeekBar.setVisibility(View.VISIBLE);
                        mCurMusicTime.setVisibility(View.VISIBLE);
                        mTotalMusicTime.setVisibility(View.VISIBLE);
                    }
                    mPlaySeekBar.setProgress((int)(max * curpos / duration));
                    mCurMusicTime.setText(changeToTimeFormat(curpos));
                } else {
                    if (isHasPos) {
                        isHasPos = false;
                        mPlaySeekBar.setVisibility(View.INVISIBLE);
                        mCurMusicTime.setVisibility(View.INVISIBLE);
                        mTotalMusicTime.setVisibility(View.INVISIBLE);
                    }
                }
                if (state == PlaybackState.STATE_PLAYING) {
                    if (!isPlaying) {
                        isPlaying = true;
                        mCdView.start();
                        if (mPlayPauseButton.isFocused()){
                            mPlayPauseButton.setImageResource(R.drawable.cursor_bar_pause_focus);
                        }else {
                            mPlayPauseButton.setImageResource(R.drawable.cursor_bar_pause);
                        }
                    }
                } else {
                    if (isPlaying) {
                        isPlaying = false;
                        mCdView.pause();
                        if (mPlayPauseButton.isFocused()) {
                            mPlayPauseButton.setImageResource(R.drawable.cursor_bar_play_focus);
                        }else {
                            mPlayPauseButton.setImageResource(R.drawable.cursor_bar_play);
                        }
                    }
                }
            }
        }
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        mAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mAdapter != null) {
            mAdapter.getProfileProxy(getActivity().getApplicationContext(), mA2dpProfileListener, BluetoothProfile.A2DP);
            mAdapter.getProfileProxy(getActivity().getApplicationContext(), mA2dpSinkProfileListener, BluetoothProfile.A2DP_SINK);
            mAdapter.getProfileProxy(getActivity().getApplicationContext(), mAvrcpProfileListener, BluetoothProfile.AVRCP_CONTROLLER);
        }
        mAlertDialog = new AlertDialog.Builder(getActivity())
                .setView(createDialogView())
                .create();

        WindowManager windowManager = getActivity().getWindowManager();
        Display display = windowManager.getDefaultDisplay();
        screenWidth = display.getWidth();
        return mAlertDialog;
    }

    private View createDialogView() {
        final LayoutInflater layoutInflater = (LayoutInflater)getActivity()
            .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View view = layoutInflater.inflate(R.layout.bt_media_controller, null);

        IntentFilter mA2dpSourceStateFilter = new IntentFilter();
        mA2dpSourceStateFilter.addAction(BluetoothA2dpSink.ACTION_CONNECTION_STATE_CHANGED);
        mA2dpSourceStateFilter.addAction(BluetoothAvrcpController.ACTION_TRACK_EVENT);

        getActivity().registerReceiver(mA2dpSourceStateReceiver, mA2dpSourceStateFilter);

        mMusicTitle = (TextView) view.findViewById(R.id.tv_music_title);
        mSingerName = (TextView) view.findViewById(R.id.play_singer);
        mCdView = (CDView) view.findViewById(R.id.play_cdview);
        mPlaySeekBar = (SeekBar) view.findViewById(R.id.video_controlprogress);
        mCurMusicTime = (TextView) view.findViewById(R.id.curplace);
        mTotalMusicTime = (TextView) view.findViewById(R.id.duration);
        mPlaySeekBar.setFocusable(false);
        mPlaySeekBar.setEnabled(false);
        max = mPlaySeekBar.getMax();
        mPlayPauseButton = (ImageButton)view.findViewById(R.id.play_pause_button);
        mPreviousButton = (ImageButton)view.findViewById(R.id.previous_button);
        mNextButton = (ImageButton)view.findViewById(R.id.next_button);

        ButtonStartListener start = new ButtonStartListener();
        ButtonForwardListener forward = new ButtonForwardListener();
        ButtonBackwardListener backward = new ButtonBackwardListener();

        mPlayPauseButton.setOnClickListener(start);
        mPreviousButton.setOnClickListener(backward);
        mNextButton.setOnClickListener(forward);

        mPlayPauseButton.setOnFocusChangeListener(
            new View.OnFocusChangeListener(){
                public void onFocusChange(View arg0, boolean isFocused){
                    if (isFocused==true) {
                        mPlayPauseButton.setImageResource(isPlaying ? R.drawable.cursor_bar_pause_focus:
                                R.drawable.cursor_bar_play_focus);
                    } else {
                        mPlayPauseButton.setImageResource(isPlaying ? R.drawable.cursor_bar_pause:
                                R.drawable.cursor_bar_play);
                    }
                }
            }
        );
        return view;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d(TAG,"onDestroy");
        mAlertDialog = null;
        if (mAdapter != null) {
            mAdapter.closeProfileProxy(BluetoothProfile.A2DP, mBluetoothA2dp);
            mAdapter.closeProfileProxy(BluetoothProfile.AVRCP_CONTROLLER, mBluetoothAvrcp_contrl);
            mAdapter.closeProfileProxy(BluetoothProfile.A2DP_SINK, mBluetoothA2dpSink);
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.d(TAG,"onResume");
    }

    @Override
    public void onPause() {
        super.onPause();
        Log.d(TAG,"onPause");
        if (mAdapter != null) {
            mAdapter.closeProfileProxy(BluetoothProfile.A2DP, mBluetoothA2dp);
            mAdapter.closeProfileProxy(BluetoothProfile.AVRCP_CONTROLLER, mBluetoothAvrcp_contrl);
            mAdapter.closeProfileProxy(BluetoothProfile.A2DP_SINK, mBluetoothA2dpSink);
        }
        getActivity().unregisterReceiver(mA2dpSourceStateReceiver);
    }

    class ButtonStartListener implements OnClickListener {
        public void onClick(View v) {
            if(mBluetoothAvrcp_contrl!= null){
                Log.d(TAG, "start button clicked");
                mPlayPauseButton.setImageResource(isPlaying ? R.drawable.cursor_bar_pause_focus:
                        R.drawable.cursor_bar_play_focus);
                if (isPlaying) {
                    mBluetoothAvrcp_contrl.sendPassThroughCmd(mBluetoothdev,BluetoothAvrcp.PASSTHROUGH_ID_PAUSE,BluetoothAvrcpPress);
                    mBluetoothAvrcp_contrl.sendPassThroughCmd(mBluetoothdev,BluetoothAvrcp.PASSTHROUGH_ID_PAUSE,BluetoothAvrcpRelease);
                } else {
                    mBluetoothAvrcp_contrl.sendPassThroughCmd(mBluetoothdev,BluetoothAvrcp.PASSTHROUGH_ID_PLAY,BluetoothAvrcpPress);
                    mBluetoothAvrcp_contrl.sendPassThroughCmd(mBluetoothdev,BluetoothAvrcp.PASSTHROUGH_ID_PLAY,BluetoothAvrcpRelease);
                }
            }
        }
    }

    class ButtonForwardListener implements OnClickListener {
        public void onClick(View v) {
            if(mBluetoothAvrcp_contrl != null) {
                Log.d(TAG, "forward button clicked");
                mBluetoothAvrcp_contrl.sendPassThroughCmd(mBluetoothdev,BluetoothAvrcp.PASSTHROUGH_ID_FORWARD,BluetoothAvrcpPress);
                mBluetoothAvrcp_contrl.sendPassThroughCmd(mBluetoothdev,BluetoothAvrcp.PASSTHROUGH_ID_FORWARD,BluetoothAvrcpRelease);
            }
        }
    }

    class ButtonBackwardListener implements OnClickListener {
        public void onClick(View v) {
            if(mBluetoothAvrcp_contrl != null) {
                Log.d(TAG, "backward button clicked");
                mBluetoothAvrcp_contrl.sendPassThroughCmd(mBluetoothdev,BluetoothAvrcp.PASSTHROUGH_ID_BACKWARD,BluetoothAvrcpPress);
                mBluetoothAvrcp_contrl.sendPassThroughCmd(mBluetoothdev,BluetoothAvrcp.PASSTHROUGH_ID_BACKWARD,BluetoothAvrcpRelease);
            }
        }
    }

    private BluetoothProfile.ServiceListener mA2dpProfileListener =
            new BluetoothProfile.ServiceListener() {
        public void onServiceConnected(int profile, BluetoothProfile proxy) {
            Log.d(TAG, "bluetooth a2dp service connected mBluetoothA2dp");
            mBluetoothA2dp = (BluetoothA2dp) proxy;
        }
        public void onServiceDisconnected(int profile) {
            Log.d(TAG, "bluetooth a2dp service disconnected");
            mBluetoothA2dp = null;
        }
    };

    private BluetoothProfile.ServiceListener mA2dpSinkProfileListener =
            new BluetoothProfile.ServiceListener() {
        public void onServiceConnected(int profile, BluetoothProfile proxy) {
            Log.d(TAG, "bluetooth a2dpsink service connected mBluetoothA2dpSink");
            mBluetoothA2dpSink = (BluetoothA2dpSink) proxy;
            if(mBluetoothA2dpSink != null) {
                List<BluetoothDevice> mDevices = mBluetoothA2dpSink.getConnectedDevices();
                if (mDevices != null && mDevices.size() > 0) {
                    for (BluetoothDevice device : mDevices) {
                        mBluetoothdev = device;
                        break;
                    }
                }
            }
        }
        public void onServiceDisconnected(int profile) {
            Log.d(TAG, "bluetooth a2dpsink service disconnected");
            mBluetoothA2dpSink = null;
        }
    };

    private BluetoothProfile.ServiceListener mAvrcpProfileListener =
            new BluetoothProfile.ServiceListener() {
        public void onServiceConnected(int profile, BluetoothProfile proxy) {
            Log.d(TAG, "bluetooth a2dp service connected mBluetoothAvrcp_contrl");
            mBluetoothAvrcp_contrl= (BluetoothAvrcpController) proxy;

            isPlaying = (mBluetoothAvrcp_contrl.getPlaybackState(mBluetoothdev).getState() ==  PlaybackState.STATE_PLAYING);
            curpos = mBluetoothAvrcp_contrl.getPlaybackState(mBluetoothdev).getPosition();
            duration = mBluetoothAvrcp_contrl.getMetadata(mBluetoothdev).getLong(MediaMetadata.METADATA_KEY_DURATION);
            mMetaData = mBluetoothAvrcp_contrl.getMetadata(mBluetoothdev);
            musicTitle = (String)mMetaData.getDescription().getTitle();
            singerName = (String)mMetaData.getDescription().getSubtitle();
            mMusicTitle.setText(musicTitle != null? musicTitle : UNKNOWN_SONG_NAME);
            mSingerName.setText(singerName != null? singerName : UNKNOWN_SINGER);
            if (curpos <= 0) {
                isHasPos = false;
                mPlaySeekBar.setVisibility(View.INVISIBLE);
                mCurMusicTime.setVisibility(View.INVISIBLE);
                mTotalMusicTime.setVisibility(View.INVISIBLE);
            } else if (duration > 0 && curpos > 0){
                mPlaySeekBar.setProgress((int)(max * curpos / duration));
                mCurMusicTime.setText(changeToTimeFormat(curpos));
                mTotalMusicTime.setText(changeToTimeFormat(duration));
            }
            bmp =  mBluetoothAvrcp_contrl.getMetadata(mBluetoothdev).getBitmap(MediaMetadata.METADATA_KEY_DISPLAY_ICON);
            if(bmp == null) {
                bmp = BitmapFactory.decodeResource(getResources(), R.drawable.cd_default);
            }

            mCdView.setImage(Bitmap.createScaledBitmap(bmp, (int)(screenWidth * 0.05), (int)(screenWidth * 0.05), true));
            if (isPlaying) {
                mCdView.start();
            } else {
                mCdView.pause();
            }
            Log.d(TAG,"getmetadata"+mBluetoothAvrcp_contrl.getMetadata(mBluetoothdev)+" musicTitle " +musicTitle);
            Log.d(TAG,"dec " +mBluetoothAvrcp_contrl.getMetadata(mBluetoothdev).getDescription());
            mPlayPauseButton.setImageResource(isPlaying ? R.drawable.cursor_bar_pause : R.drawable.cursor_bar_play);
        }
        public void onServiceDisconnected(int profile) {
            Log.d(TAG, "bluetooth avrcp service disconnected");
            mBluetoothAvrcp_contrl= null;
        }
    };

    public void onConfigurationChanged(Configuration newConfig, CharSequence s) {
        super.onConfigurationChanged(newConfig);

    }

    public static String changeToTimeFormat(long microseconds) {
        long seconds= microseconds / 1000;
        long minus=seconds/60;
        long second=seconds%60;
        if (second < 10 && minus < 10)
            return "0" + minus + ":0" + second;
        else {
            if (second >= 10 && minus < 10)
                return "0"+ minus + ":" + second;
            else {
                if(second<10 && minus >=10)
                return minus + ":0" + second;
                else
                return minus + ":"+second;
            }
        }
    }

    /* Not used */
    public void beforeTextChanged(CharSequence s, int start, int count, int after) {
    }

    /* Not used */
    public void onTextChanged(CharSequence s, int start, int before, int count) {
    }
}
