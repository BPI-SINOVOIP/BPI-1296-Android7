package com.realtek.suspendhandler;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.util.Log;
import android.media.AudioManager;
import android.os.SystemProperties;

import java.util.List;
import java.util.ArrayList;

import java.util.GregorianCalendar;
import java.util.Calendar;
import java.util.Date;
import java.text.SimpleDateFormat;

public class WakeUpBroadcastReceiver extends BroadcastReceiver {

    private static final String TAG = "RealtekSuspend-WakeUpBroadcastReceiver";

    private static final String KEY_START_TIME_MILLIS = "timeStart";
    private static final String KEY_END_TIME_MILLIS = "timeEnd";

    private static final String WAKE_UP_ADD_BROADCAST = "android.intent.action.WAKE_UP_ADD";
    private static final String WAKE_UP_RM_BROADCAST = "android.intent.action.WAKE_UP_RM";

    class WakeUpTime{
        long mStartTimeMillis;
        long mEndTimeMillis;

        public boolean equals(Object _dst) {
            if(_dst instanceof WakeUpTime){
                WakeUpTime dst = (WakeUpTime) _dst;
                if(mStartTimeMillis==dst.mStartTimeMillis && mEndTimeMillis==dst.mEndTimeMillis){
                    return true;
                }
            }
            return false;
        }

        public WakeUpTime(long start, long end){
            mStartTimeMillis = start;
            mEndTimeMillis = end;
        }

        public String toString() {
            String str = "startTime: " + millisToTimeString(mStartTimeMillis);
            return str;
        }
    }

    private static List<WakeUpTime> sList = new ArrayList<WakeUpTime>();

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        long startTimeMillis = intent.getLongExtra(KEY_START_TIME_MILLIS,-1);
        long endTimeMillis = intent.getLongExtra(KEY_END_TIME_MILLIS,-1);
        Log.d(TAG, "action:"+action+" start:"+millisToTimeString(startTimeMillis)+" end:"+millisToTimeString(endTimeMillis));

        if(action.equals(WAKE_UP_ADD_BROADCAST)){
            insertToList(new WakeUpTime(startTimeMillis,endTimeMillis));
        }else{
            deleteFromList(new WakeUpTime(startTimeMillis,endTimeMillis));
        }

        // for debug
        long wakeUpTime = getWakeUpTime();
        Log.d(TAG, "latest WakeUpTime:"+millisToTimeString(wakeUpTime));
    }

    private static synchronized boolean deleteFromList(WakeUpTime item){
        int index = -1;
        for(int i=0;i<sList.size();i++){
            if(sList.get(i).equals(item)){
                index = i;
                break;
            }
        }
        if(index>=0){
            sList.remove(index);
            return true;
        }else{
            Log.e(TAG, "deleteFromList item not found");
            return false;
        }
    }

    private static synchronized boolean insertToList(WakeUpTime item){

        Log.i(TAG, "insertToList: item - "+item.toString());

        for(int i=0;i<sList.size();i++){
            if(sList.get(i).equals(item)){
                Log.e(TAG, "insertToList found duplicate item["+i+"] "+sList.get(i));
                return false;
            }
        }
        sList.add(item);
        return true;
    }

    public static synchronized long getWakeUpTime(){
        long rst = Long.MAX_VALUE;
        for(int i=0;i<sList.size();i++){
            if(sList.get(i).mStartTimeMillis<rst){
                rst = sList.get(i).mStartTimeMillis;
            }
        }

        if(rst == Long.MAX_VALUE){
            rst = -1;
        }

        return rst;
    }

    private String millisToTimeString(long timeMillis){
        Calendar c = new GregorianCalendar();
        c.setTimeInMillis(timeMillis);
        SimpleDateFormat format1 = new SimpleDateFormat("yyyy.MM.dd 'at' HH:mm:ss");
        String formatted = format1.format(c.getTime());
        return formatted;
    }
}
