package com.rtk.debug;

import android.app.*;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import android.os.SystemProperties;
import android.util.Log;
import com.rtk.debug.helper.RuntimeHelper;
import android.support.v4.app.NotificationCompat;
import com.rtk.debug.util.Utils;

import java.io.IOException;
import java.util.Arrays;

import static com.rtk.debug.IntentReceiver.KEY_BOOT_START_LOG;

/**
 * Created by archerho on 2017/2/8.
 */
public class LogcatService extends Service {
    private static final String TAG = "LogcatService";
    static final String EXTRA_TASK = "EXTRA_TASK";
    static final String EXTRA_SAVE_PATH = "EXTRA_SAVE_PATH";
    private static final String EXTRA_CLEAR = "EXTRA_CLEAR";
    int SERVICE_ID = 100;
    CharSequence mContentTitle = "RtkDebugTool";
    private NotificationManager mNM;
    private BroadcastReceiver mReceiver;

    @Override
    public void onCreate() {
        super.onCreate();
        mNM = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        init();
        regReceiver();
    }

    private boolean isLogOnBeforeSuspend = false;

    private void regReceiver() {
        IntentFilter filter = new IntentFilter();
        filter.addAction(Intent.ACTION_SCREEN_OFF);
        filter.addAction(Intent.ACTION_SCREEN_ON);
        filter.addAction("rtk.intent.action.POWER_SUSPEND");
        mReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                log("onReceive "+action);
                AutoBootLog bootLog = AutoBootLog.getInstance(context.getApplicationContext());
                switch(action){
                    case Intent.ACTION_SCREEN_OFF:
                        int isSuspend = SystemProperties.getInt("persist.rtk.screenoff.suspend",0);
                        if(isSuspend==1) {
                            isLogOnBeforeSuspend = bootLog.isOn();
                            if(isLogOnBeforeSuspend) {
                                log("os suspend, stop log");
                                bootLog.off();
                            }
                        }
                        break;
                    case Intent.ACTION_SCREEN_ON:
                        if(isLogOnBeforeSuspend) {
                            log("os resume, start log");
                            bootLog.on();
                            isLogOnBeforeSuspend = false;
                        }
                        break;
                    case "rtk.intent.action.POWER_SUSPEND":
                        isLogOnBeforeSuspend = bootLog.isOn();
                        if(isLogOnBeforeSuspend) {
                            log("power suspend, stop log");
                            bootLog.off();
                        }
                        break;
                }
            }
        };
        this.registerReceiver(mReceiver, filter);
    }

    void init(){};

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        handleIntent(intent);
//        mContentTitle = getText(R.string.app_name);

        Intent playIntent = new Intent(this, RtkDebugActivity.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0,
                playIntent, 0);
        // Set the icon, scrolling text and timestamp
        NotificationCompat.Builder builder =
                new NotificationCompat.Builder(this)
                        .setSmallIcon(R.drawable.ic_launcher)
                        .setContentIntent(pendingIntent)
                        .setContentTitle(mContentTitle);

        Notification notification = builder.build();
        log("startForeground "+mContentTitle);
        startForeground(SERVICE_ID, notification);
        return START_REDELIVER_INTENT;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if(mReceiver!=null)
            unregisterReceiver(mReceiver);
        log("notification cancel: "+ SERVICE_ID);
        mNM.cancel(SERVICE_ID);
        log("destroy "+mContentTitle);
        try {
            Thread.sleep(300);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    protected void log(String s){
        Log.d(TAG, s);
    }

    void handleIntent(Intent intent){
        if(intent==null) return;
        String task = intent.getStringExtra(EXTRA_TASK);
        log("handleIntent, task="+task);
        if(DumpLog.TAG.equals(task)) {
            String savePath = intent.getStringExtra(EXTRA_SAVE_PATH);
            boolean isClearOld = intent.getBooleanExtra(EXTRA_CLEAR, false);
            log("isClearOld="+isClearOld);
            try {
                if(isClearOld) RuntimeHelper.exec(Arrays.asList("logcat", "-c"));
                if(savePath!=null)
                    DumpLog.getInstance(this).startDaemon(savePath);
                else
                    DumpLog.getInstance(this).startDaemon();
            } catch (IOException e) {
                e.printStackTrace();
            }
        } else if(DumpKernelLog.TAG.equals(task)){
            try {
                DumpKernelLog.getInstance(this).startDaemon();
            } catch (IOException e) {
                e.printStackTrace();
            }
        } else if(DumpTcpdump.TAG.equals(task)){
            try {
                DumpTcpdump.getInstance(this).startDaemon();
            } catch (IOException e) {
                e.printStackTrace();
            }
        } else if(task!=null && task.startsWith("Stop")){
            String stopTask = task.substring(4);
            if(stopTask.equals(DumpLog.TAG))
                DumpLog.getInstance(this).stopDaemon();
            else if(stopTask.equals(DumpKernelLog.TAG))
                DumpKernelLog.getInstance(this).stopDaemon();
            else if(stopTask.equals(DumpTcpdump.TAG))
                DumpTcpdump.getInstance(this).stopDaemon();

            if(!DumpLog.getInstance(this).isOn()
                    && !DumpKernelLog.getInstance(this).isOn()
                    && !DumpTcpdump.getInstance(this).isOn()
                    && !isLogOnBeforeSuspend) {
                stopSelf();
            }
        } else if(IntentReceiver.TAG.equals(task)) {

        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
}
