package com.example.hdmirxdemo;

import java.util.GregorianCalendar;
import java.util.Calendar;
import java.util.Date;
import android.os.Bundle;
import android.util.Log;

public class SchRecItem2 {

    private static final String TAG="SchRecItem2";

    private static final String[] MONTH_TEXT = {
        "Jan","Feb","Mar","Apr","May","Jun",
        "Jul","Aug","Sep","Oct","Nov","Dec"};

    public static final int YEAR = Calendar.YEAR;
    public static final int MONTH = Calendar.MONTH;
    public static final int DATE = Calendar.DATE;
    public static final int HOUR = Calendar.HOUR_OF_DAY;
    public static final int MINUTE = Calendar.MINUTE;

    private Bundle mBundle;

    public Calendar mStartTime;
    public Calendar mEndTime;
    public String mRecordPath;
    public int mWidth;
    public int mHeight;
    public int mFormat;

    private long mCompareTime;
    public long mCompareTimeInDb;

    public SchRecItem2(Calendar startTime,
                    Calendar endTime,
                    String path,
                    int width,
                    int height,
                    int format){

        mStartTime = startTime;
        mEndTime = endTime;
        mRecordPath = path;
        mWidth = width;
        mHeight = height;
        mFormat = format;
        mCompareTime = mStartTime.getTimeInMillis()/(1000*60);
        mCompareTimeInDb = 0;
    }

    public Bundle getBundle(){
        if(mBundle==null) {
            mBundle = new Bundle();
            mBundle.putCharSequence(SchRecListViewItem.KEY_DATE_TEXT,getDateString(getDateInfo(mStartTime)));

            mBundle.putCharSequence( SchRecListViewItem.KEY_TIME_TEXT,
                getTimeString( getTimeInfo(mStartTime), getTimeInfo(mEndTime) ));

            mBundle.putCharSequence(SchRecListViewItem.KEY_DUR_TEXT,
                    getDurationString( getTimeInfo(mStartTime), getTimeInfo(mEndTime) ));
            //mBundle.putCharSequence(SchRecListViewItem.KEY_FMT_TEXT,getFormatString(mFormat));
            //mBundle.putCharSequence(SchRecListViewItem.KEY_PATH_TEXT,mPath);
        }
        return mBundle;
    }

    private TimeInfo getTimeInfo(Calendar c){
        return new TimeInfo(c.get(HOUR), c.get(MINUTE));
    }

    private DateInfo getDateInfo(Calendar c){
        return new DateInfo(c.get(YEAR), c.get(MONTH), c.get(DATE));
    }

    public static SchRecItem2 getCurrentTimeSchRecItem2() {
        // get current time
        Calendar calendar = new GregorianCalendar();
        Date trialtime = new Date();
        calendar.setTime(trialtime);

        SchRecItem2 item = new SchRecItem2(calendar, calendar, null, -1, -1, -1);
        return item;
    }

    public long getCompareTime() {

        long rst = mStartTime.getTimeInMillis()/(1000 * 60);
        Log.d(TAG,"getCompareTime:"+rst +" dft:"+mCompareTime+"   sec:"+mStartTime.get(Calendar.SECOND));

        // unit second
        return rst;
    }

    public boolean isOverlapping(SchRecItem2 item){
        /*
        long myStartSec = mStartTime.getTimeInMillis()/1000;
        long myEndSec = mEndTime.getTimeInMillis()/1000;

        long dstStartSec = item.mStartTime.getTimeInMillis()/1000;
        long dstEndSec = item.mEndTime.getTimeInMillis()/1000;

        if(myEndSec<dstStartSec)
            return false;
        if(dstEndSec<myStartSec)
            return false;

        return true;
        */

        long myStartMin = mStartTime.getTimeInMillis()/(1000*60);
        long myEndMin = mEndTime.getTimeInMillis()/(1000*60);

        long dstStartMin = item.mStartTime.getTimeInMillis()/(1000*60);
        long dstEndMin = item.mEndTime.getTimeInMillis()/(1000*60);

        if(myEndMin<=dstStartMin)
            return false;
        if(dstEndMin<=myStartMin)
            return false;

        return true;
    }

    public int getDurationMin() {

        long start_sec = mStartTime.getTimeInMillis()/1000;
        long end_sec = mEndTime.getTimeInMillis()/1000;

        long start_min = start_sec/60;
        long end_min = end_sec/60;
        int dur_min = (int)(end_min - start_min);
        return dur_min;
    }

    public boolean isBiggerThenCurrentTime() {

        SchRecItem2 currentTimeItem = SchRecItem2.getCurrentTimeSchRecItem2();

        if(getCompareTime()>currentTimeItem.getCompareTime())
            return true;
        else
            return false;
    }

    public String getDisplayStringForToast(){

        DateInfo date = getDateInfo(mStartTime);
        TimeInfo st = getTimeInfo(mStartTime);
        TimeInfo et = getTimeInfo(mEndTime);

        String rst=""+getDateString(date)+" "+getTimeString(st,et);
        return rst;
    }

    public String getDisplayString() {

        DateInfo date = getDateInfo(mStartTime);
        TimeInfo st = getTimeInfo(mStartTime);
        TimeInfo et = getTimeInfo(mEndTime);

        String rst="mCmpDb:"+mCompareTimeInDb+" mCmp:"+mCompareTime+" "+getDateString(date)+" "+getTimeString(st,et);
        return rst;
    }

    public boolean equals(Object _item){

        if(_item==null) return false;

        if(_item instanceof SchRecItem2){
            SchRecItem2 item = (SchRecItem2)_item;

            long my_start_time = mStartTime.getTimeInMillis();
            long my_end_time = mEndTime.getTimeInMillis();

            long dst_start_time = item.mStartTime.getTimeInMillis();
            long dst_end_time = item.mEndTime.getTimeInMillis();

            return ((my_start_time==dst_start_time) && (my_end_time == dst_end_time));

        }else{
            return false;
        }
    }

    private String getDateString(DateInfo date) {
        String rst = ""+date.mDate+" "+MONTH_TEXT[date.mMonth]+", "+date.mYear;
        return rst;
    }

    private String get2DigitMinString(int minute){
        String rst="";
        if(minute<10)
            rst=rst+"0";
        rst=rst+minute;
        return rst;
    }

    private String getTimeString(TimeInfo start, TimeInfo end) {
        String rst = ""+start.mHour+":"+
            get2DigitMinString(start.mMinute)+
            " - "+end.mHour+":"+
            get2DigitMinString(end.mMinute);

        return rst;
    }

    private String getDurationString(TimeInfo start, TimeInfo end) {
        int start_min = start.mTimeMinute;
        int end_min = end.mTimeMinute;

        int dur_min = end_min - start_min;
        if(dur_min < 0 ) return "N/A";
        int hour = dur_min /60;
        int min = dur_min % 60;

        String rst = "";
        if(hour > 0)
            rst = rst + hour+ ((hour>1)?" hours ":" hour ");
        if(min > 0)
            rst = rst + min+ ((min>1)?" mins":" min");
        return rst;
    }
}
