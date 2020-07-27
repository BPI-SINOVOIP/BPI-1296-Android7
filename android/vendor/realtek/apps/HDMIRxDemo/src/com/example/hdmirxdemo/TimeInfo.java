package com.example.hdmirxdemo;

import java.util.GregorianCalendar;
import java.util.Calendar;
import java.util.Date;

public class TimeInfo {
    public int mHour;
    public int mMinute;

    public int mTimeMinute;

    public TimeInfo(int hour, int minute) {
        mHour = hour;
        mMinute = minute;
        mTimeMinute = (mHour*60)+mMinute;
    }

    public boolean equals(Object _item){

        if(_item==null) return false;

        if(_item instanceof TimeInfo){
            TimeInfo item = (TimeInfo)_item;
            return (mTimeMinute==item.mTimeMinute);
        }else{
            return false;
        }
    }

    public static TimeInfo getCurrentTime(){

        Calendar calendar = new GregorianCalendar();
        Date trialTime = new Date();
        calendar.setTime(trialTime);

        TimeInfo rst = new TimeInfo(calendar.get(Calendar.HOUR_OF_DAY),
                        calendar.get(Calendar.MINUTE));
        return rst;
    }

}
