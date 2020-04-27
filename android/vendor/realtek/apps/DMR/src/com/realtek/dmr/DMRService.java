
package com.realtek.dmr;

import com.realtek.DLNA_DMR.DLNA_DMR;
import com.realtek.dmr.VolumeControl.VolumeChangeListener;

import java.lang.ref.WeakReference;
import java.util.Date;
import java.util.UUID;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.provider.MediaStore;
import android.provider.Settings;
import android.util.Log;

public class DMRService extends Service implements VolumeChangeListener {
    private static DLNA_DMR mDMR = null;
    private Boolean isPlaying = false;
    private static String TAG = "DMRService";
    private static String SharedPreferencesName = "DMRService";
    private BroadcastReceiver playbackReceiver = null;
    private BroadcastReceiver deviceNameChangedReceiver = null;
    private String mPlayType = null;
    private String mSourceFrom = null;
    private VolumeControl volCtrl;
    private int seekingTime = -1;
    private int curElapsedTime = -1;

    private static final int CAN_PAUSE_FLAG     = 0x01;
    private static final int CAN_SEEK_FWD_FLAG  = 0x02;
    private static final int CAN_SEEK_BWD_FLAG  = 0x04;
    private static final int CAN_SEEK           = 0x08;

    private static final String MEDIA_BROWSER_USE_RT_MEDIA_PLAYER = "MEDIA_BROWSER_USE_RT_MEDIA_PLAYER";

    private BroadcastReceiver pbBroadcastReceiver = new BroadcastReceiver() {

        @Override
        public void onReceive(Context context, Intent intent) {
            if (mDMR == null)
                return;
            String playMode = intent.getStringExtra("PlayMode");
            if (playMode != null) {
                String playType = intent.getStringExtra("PlayType");
                if (!(playType != null && mPlayType != null && playType.matches(mPlayType)))
                {
                    Log.v(TAG, "PlayType is not match. Cur:" + mPlayType + ", Received:" + playType);
                    return;
                }
                String SourceFrom = intent.getStringExtra("SourceFrom");
                if (!(SourceFrom != null && mSourceFrom != null && SourceFrom.matches(mSourceFrom)))
                {
                    Log.v(TAG, "SourceFrom is not match. Cur:" + mSourceFrom+ ", Received:" + SourceFrom);
                    return;
                }
                Log.v(TAG, String.format("PlayMode is received. - %s, curPlayType: %s, recvPlayType: %s", playMode, mPlayType, playType));
                if (playMode.equalsIgnoreCase("Play")) {
                    mDMR.UpdatePlaybackStatus(
                            DLNA_DMR.PLAYBACKSTATUS_TRANSPORTSTATE,
                            DLNA_DMR.TRANSPORTSTATE_Playing);
                } else if (playMode.equalsIgnoreCase("Stop")) {
                    mDMR.UpdatePlaybackStatus(
                            DLNA_DMR.PLAYBACKSTATUS_TRANSPORTSTATE,
                            DLNA_DMR.TRANSPORTSTATE_Stopped);
                    isPlaying = false;
                } else if (playMode.equalsIgnoreCase("Pause")) {
                    mDMR.UpdatePlaybackStatus(
                            DLNA_DMR.PLAYBACKSTATUS_TRANSPORTSTATE,
                            DLNA_DMR.TRANSPORTSTATE_Paused);
                }
            }

            int duration = intent.getIntExtra("Duration", -1);
            if (duration != -1) {
                mDMR.UpdatePlaybackStatus(DLNA_DMR.PLAYBACKSTATUS_DURATION,
                        duration * 1000);
            }

            int timePosition = intent.getIntExtra("TimePosition", -1);
            if (timePosition > 0) {
                if (!isInSeekingMode(timePosition) && isPlaying) {
                    mDMR.UpdatePlaybackStatus(
                            DLNA_DMR.PLAYBACKSTATUS_TIMEPOSITION,
                            timePosition * 1000);
                    curElapsedTime = timePosition;
                }
                else
                {
                    Log.v(TAG, String.format("TimePosition is not updated. isPlaying:[%b]", isPlaying));
                }
            }

            int seekSupported = intent.getIntExtra("SeekSupported", -1);
            if (seekSupported != -1) {
                boolean canSeek = (seekSupported&CAN_SEEK)==CAN_SEEK;
                Log.v(TAG, String.format("SeekSupported[%d]: %s", seekSupported, canSeek));
                mDMR.UpdatePlaybackStatus(DLNA_DMR.PLAYBACKSTATUS_SEEK,
                        canSeek ? 1 : 0);
            }
        }
    };

    private DLNA_DMR.OnEventListener DMREventListener = new DLNA_DMR.OnEventListener() {

        @Override
        public int onEvent(int notifyID, String info) {
            if (mDMR == null)
                return -1;
            String idString = null;
            switch (notifyID) {
                case DLNA_DMR.NOTIFY_ID_QUERYFORCONNECTION:
                    idString = "QueryForConnection";
                    break;
                case DLNA_DMR.NOTIFY_ID_SETAVTRANSPORTURI:
                    idString = "SetAVTransportURI";
                    mPlayType = null;
                    mSourceFrom = null;
                    break;
                case DLNA_DMR.NOTIFY_ID_LOADMEDIA:
                    idString = "LoadMedia";
                    isPlaying = false;
                    curElapsedTime = -1;
                    seekingTime = -1;
                    break;
                case DLNA_DMR.NOTIFY_ID_PLAY:
                    if (!isPlaying) {
                        String uriStr = mDMR.GetAVTransportURI();
                        String mimeType = mDMR.GetMIMEType();
                        String protocolInfo = mDMR.GetMediaProtocolInfo();
                        idString = "Play for URI: "
                                + uriStr
                                + " , MimeType: "
                                + mimeType
                                + " , Protocol Info: "
                                + protocolInfo
                        ;

                        Uri uri = Uri.parse(uriStr);
                        //append protocol info into query part of URI if this is a dtcpip media.
                        if (protocolInfo != null && protocolInfo.contains("application/x-dtcp1")) {
                            Uri.Builder builder = uri.buildUpon();
                            builder.appendQueryParameter("DtcpipInfo", protocolInfo);
                            uri = builder.build();
                            Log.v(TAG, "Modified URI for dtcpip: " + uri.toString());
                        }

                        Intent intent = new Intent(Intent.ACTION_VIEW);
                        if (mimeType != null) {
                            intent.setDataAndType(uri, mimeType);
                            if (mimeType.startsWith("video/")) {
                                ComponentName comp = new ComponentName(
                                        "com.android.gallery3d",
                                        "com.android.gallery3d.app.MovieActivity");
                                intent.setComponent(comp);
                                intent.putExtra(
                                        "android.intent.playback.broadcast.status",
                                        true);
                                intent.putExtra(
                                        MEDIA_BROWSER_USE_RT_MEDIA_PLAYER,
                                        true);
                                mPlayType = "Video";
                                String[] fileName = {mDMR.GetMediaInfo(mDMR.MedialInfoType_Filename)};
                                intent.putExtra("playlistTitle", fileName);
                                intent.putExtra("repeat", false);
                            } else if (mimeType.startsWith("audio/")) {
                                ComponentName comp = new ComponentName(
                                        "com.android.music",
                                        "com.android.music.MediaPlaybackActivity");
                                intent.setComponent(comp);
                                intent.putExtra(
                                        MediaStore.EXTRA_FINISH_ON_COMPLETION,
                                        true);
                                intent.putExtra(
                                        "android.intent.playback.broadcast.status",
                                        true);
                                intent.putExtra(
                                        MEDIA_BROWSER_USE_RT_MEDIA_PLAYER,
                                        true);
                                processPlaybackInfo(intent, mDMR.GetExtraPlaybackInfo());
                                mPlayType = "Audio";
                            } else if (mimeType.startsWith("image/")) {
                                ComponentName comp = new ComponentName(
                                        "com.android.gallery3d",
                                        "com.android.gallery3d.app.GalleryActivity");
                                intent.setComponent(comp);
                                intent.putExtra(
                                        "android.intent.playback.broadcast.status",
                                        true);
                                mPlayType = "Image";
                                String[] fileName = {mDMR.GetMediaInfo(mDMR.MedialInfoType_Filename)};
                                intent.putExtra("playlistTitle", fileName);
                            }
                            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK
                                    | Intent.FLAG_ACTIVITY_CLEAR_TASK
                                    | Intent.FLAG_ACTIVITY_TASK_ON_HOME); // BUG_FIX: 43406, 43830
                            if (mPlayType != null)
                            {
                                Date date = new Date();
                                mSourceFrom = "DMR:"+mPlayType+":" + date.getTime();
                                intent.putExtra("SourceFrom", mSourceFrom);
                            }
                            startActivity(intent);
                            isPlaying = true;
                        }

                        hookPlaybackStatus(true);
                    } else {
                        idString = "Play";
                        ExecuteCommand("play");
                    }
                    break;
                case DLNA_DMR.NOTIFY_ID_PAUSE: {
                    idString = "Pause";
                    ExecuteCommand("pause");
                }
                    break;
                case DLNA_DMR.NOTIFY_ID_STOP: {
                    idString = "Stop";
                    StopPlayback();
                }
                    break;
                case DLNA_DMR.NOTIFY_ID_SEEK:
                    idString = "Seek";
                    if (info != null) {
                        idString += " (" + info + ") ";

                        Intent i = createCommand("seek");
                        if (i != null) {
                            seekingTime = Integer.parseInt(info);
                            i.putExtra("seektime",
                                    seekingTime * 1000);
                            sendBroadcast(i);
                        }
                    }
                    break;
                case DLNA_DMR.NOTIFY_ID_SETRATE:
                    idString = "SetRate";
                    if (info != null) {
                        idString += " (" + info + ") ";

                        Intent i = createCommand("rate");
                        if (i != null) {
                            int rateValue = Integer.parseInt(info);
                            i.putExtra("rate", rateValue);
                            sendBroadcast(i);
                        }
                    }
                    break;
                case DLNA_DMR.NOTIFY_ID_FACTORY_DEFAULT:
                    idString = "Factory Default";
                    if (info != null)
                        idString += " (" + info + ") ";
                    break;
                case DLNA_DMR.NOTIFY_ID_SETVOLUME:
                    idString = "Set Volume";
                    if (info != null)
                    {
                        idString += " (" + info + ") ";

                        if (volCtrl != null)
                        {
                            volCtrl.setVolume(Integer.parseInt(info));
                        }
                    }
                    break;
                case DLNA_DMR.NOTIFY_ID_SETMUTE:
                    idString = "Set Mute";
                    if (info != null)
                    {
                        idString += " (" + info + ") ";
                        if (volCtrl != null)
                        {
                            volCtrl.setMute(Integer.parseInt(info) != 0);
                        }
                    }
                    break;
                case DLNA_DMR.NOTIFY_ID_SETCONTRAST:
                    idString = "Set Contrast";
                    if (info != null)
                        idString += " (" + info + ") ";
                    break;
                case DLNA_DMR.NOTIFY_ID_SETBRIGHTNESS:
                    idString = "Set Brightness";
                    if (info != null)
                        idString += " (" + info + ") ";
                    break;
                case DLNA_DMR.NOTIFY_ID_RESTART_DMR:
                    idString = "The DMR restart is needed";
                    if (info != null)
                        idString += " (" + info + ") ";

                    if (mHandler != null)
                    {
                        Log.v(TAG, "The MESSAGE_RESTART_DMR is send");
                        mHandler.sendMessage(mHandler.obtainMessage(MESSAGE_RESTART_DMR));
                    }
                    break;
            }
            if (idString == null) {
                idString = Integer.toString(notifyID);
            }

            Log.v(TAG, "The activity " + idString);
            return 0;
        }
    };

    private static final int MESSAGE_RESTART_DMR = 0;

    private static class InternalHandler extends Handler {
        private final WeakReference<DMRService> mDMRService;

        public InternalHandler(DMRService service) {
            mDMRService = new WeakReference<DMRService>(service);
        }

        @Override
        public void handleMessage(Message msg) {
            DMRService service = mDMRService.get();
            if (service == null)
                return;
            Log.v(TAG, "mHandler : Get the message " + msg.what);
            switch (msg.what)
            {
                case MESSAGE_RESTART_DMR:
                    Log.v(TAG, "It is to stop playback and restart DMR.");
                    service.StopPlayback();
                    service.StopDMR();
                    service.StartDMR();
                    break;
                default:
                    break;
            }
        }
    }

    private final InternalHandler mHandler = new InternalHandler(this);

    @Override
    public IBinder onBind(Intent arg0) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.v(TAG, "The onCreate is called.");
        StartDMR();
        volCtrl = new VolumeControl(this);
        if (volCtrl != null)
        {
            volCtrl.setVolumeChangeListener(this);
            volCtrl.startMonitor();
        }
    }

    private Intent createCommand(String strCMD) {
        String mimeType = mDMR.GetMIMEType();
        Intent i = null;
        if (mimeType != null) {
            if (mimeType.startsWith("video/")) {
                i = new Intent("com.android.gallery3d.movieactivitycommand");
            } else if (mimeType.startsWith("audio/")) {
                i = new Intent("com.android.music.musicservicecommand");
            } else if (mimeType.startsWith("image/")) {
                i = new Intent("com.android.gallery3d.photoviewcommand");
            }
            if (i != null) {
                i.putExtra("command", strCMD);
            }
        }

        return i;
    }

    private void ExecuteCommand(String strCMD) {
        Intent i = createCommand(strCMD);
        if (i != null) {
            sendBroadcast(i);
        }
    }

    private void hookPlaybackStatus(boolean bEnable) {
        Log.v(TAG, "hookPlaybackStatus " + bEnable);
        if (bEnable) {
            if (playbackReceiver == null)
            {
                IntentFilter intentFilter = new IntentFilter(
                        "rtk.provider.playback.STATUS");
                playbackReceiver = pbBroadcastReceiver;
                registerReceiver(playbackReceiver, intentFilter);
                Log.v(TAG, "hookPlaybackStatus register");
            }
        } else {
            if (playbackReceiver != null)
            {
                unregisterReceiver(playbackReceiver);
                playbackReceiver = null;
                Log.v(TAG, "hookPlaybackStatus unregister");
            }
        }
    }

    private void hookDeviceNameChangedStatus(boolean bEnable) {
        Log.v(TAG, "hookDeviceNameChangedStatus " + bEnable);
        if (bEnable) {
            if (deviceNameChangedReceiver == null)
            {
                IntentFilter intentFilter = new IntentFilter(
                        "com.android.settings.name.DeviceManager.DEVICE_NAME_UPDATE");
                deviceNameChangedReceiver = new BroadcastReceiver() {
                    @Override
                    public void onReceive(Context context, Intent intent) {
                        if (mDMR == null)
                            return;
                        Log.v(TAG, "receive the com.android.settings.name.DeviceManager.DEVICE_NAME_UPDATE, restart the DMR.");
                        mHandler.sendMessage(mHandler.obtainMessage(MESSAGE_RESTART_DMR));
                   }
                };
                registerReceiver(deviceNameChangedReceiver, intentFilter);
                Log.v(TAG, "hookDeviceNameChangedStatus register");
            }
        } else {
            if (deviceNameChangedReceiver != null)
            {
                unregisterReceiver(deviceNameChangedReceiver);
                deviceNameChangedReceiver = null;
                Log.v(TAG, "hookDeviceNameChangedStatus unregister");
            }
        }
    }

    @Override
    public void onDestroy() {
        Log.v(TAG, "The onDestroy is called.");
        StopDMR();
        if (volCtrl != null)
        {
            volCtrl.stopMonitor();
            volCtrl = null;
        }
    }

    @Override
    public void onStart(Intent intent, int startId) {
        Log.v(TAG, "The onStart is called");
    }

    private String getDeviceName() {
        String setupDeviceName = "device_name"; // Settings.Global.DEVICE_NAME;
        String strDevice = Settings.Global.getString(getContentResolver(), setupDeviceName);

        if (strDevice == null)
        {
            SharedPreferences dmrData = getSharedPreferences(SharedPreferencesName, 0);
            strDevice = dmrData.getString("DeviceName", null);
            if (strDevice == null)
            {
                Log.v(TAG, "The device name is gengerated");
                strDevice = "DMR for android - "
                        + Integer.toOctalString((int) (Math.random() * 100 + 1));
                dmrData.edit().putString("DeviceName", strDevice).commit();
            }
        }

        Log.v(TAG, "The device name is " + strDevice);
        return strDevice;
    }

    private String getUUID() {
        SharedPreferences dmrData = getSharedPreferences(SharedPreferencesName, 0);
        String strUUID = dmrData.getString("UUID", null);
        if (strUUID == null)
        {
            Log.v(TAG, "The UUID is gengerated");
            strUUID = UUID.randomUUID().toString();
            dmrData.edit().putString("UUID", strUUID).commit();
        }
        Log.v(TAG, "The UUID is " + strUUID);
        return strUUID;
    }

    private void StartDMR() {
        Log.v(TAG, "StartDMR is called.");
        mDMR = new DLNA_DMR();

        if (mDMR != null)
        {
            mDMR.Start(getDeviceName(), getUUID());
            mDMR.SetOnEventListener(DMREventListener);
            hookDeviceNameChangedStatus(true);
        }
    }

    private void StopDMR() {
        Log.v(TAG, "The StopDMR is called");
        hookPlaybackStatus(false);
        hookDeviceNameChangedStatus(false);

        if (mDMR != null)
        {
            mDMR.SetOnEventListener(null);
            mDMR.Stop();
            mDMR = null;
        }
        Log.v(TAG, "The StopDMR is called - done");
    }

    @Override
    public void volumeChanged(int vol) {
        Log.v(TAG, String.format("The Volume Change is received. [%d]", vol));
        if (mDMR != null && vol > 0)
        {
            mDMR.UpdatePlaybackStatus(DLNA_DMR.PLAYBACKSTATUS_VOLUME, vol);
        }
    }

    @Override
    public void muteChanged(boolean bMute) {
        Log.v(TAG, String.format("The Mute Change is received. [%b]", bMute));
        if (mDMR != null)
        {
            mDMR.UpdatePlaybackStatus(DLNA_DMR.PLAYBACKSTATUS_MUTE, bMute? 1 : 0);
        }
    }

    private Boolean isInSeekingMode(int curTime) {
        final int SEEK_TIME_GAP = 2;
        final int SEEK_TIME_GAP_RANGE = 5;
        if (seekingTime != -1)
        {
            //Log.v(TAG, String.format("It is in isInSeekingMode.. curTime[%d] prevTime:%d, seekingTime:%d", curTime, curElapsedTime, seekingTime));
            int diffToTime = curTime - seekingTime;
            int diffFromTime = Math.abs(curElapsedTime - curTime);
            if ((SEEK_TIME_GAP < diffFromTime && diffFromTime < SEEK_TIME_GAP_RANGE) ||
                (0 < diffToTime))
            {
                seekingTime = -1;
                curElapsedTime = -1;
                Log.v(TAG, String.format("It is InSeekingMode reset. [%d]", curTime));
            }
            else {
                Log.v(TAG, String.format("It is InSeekingMode. [%d]", curTime));
                return true;
            }
        }

        return false;
    }

    private void processPlaybackInfo(Intent intent, String strPlaybackInfo) {
        if (intent == null || strPlaybackInfo == null)
            return;

        Log.v(TAG, String.format("PlaybackInfo is [%s]", strPlaybackInfo));
        intent.putExtra("RTMediaPlayer.Config", strPlaybackInfo);
    }

    private void StopPlayback() {
        Log.v(TAG, "The StopPlayback is called");
        if (isPlaying)
        {
            Log.v(TAG, "ExecuteCommand: stop");
            ExecuteCommand("stop");
            mPlayType = null;
            mSourceFrom = null;
            isPlaying = false;
        }
    }

}
