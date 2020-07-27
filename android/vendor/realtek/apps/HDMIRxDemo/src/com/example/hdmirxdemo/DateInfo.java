package com.example.hdmirxdemo;

import java.util.GregorianCalendar;
import java.util.Calendar;
import java.util.Date;

public class DateInfo {

    public int mYear;
    public int mMonth;  // zero index base
    public int mDate;
    public int mTimeDay;

    public DateInfo(int year, int month, int date) {
        mMonth = month;
        mDate = date;
        mYear = year;
        mTimeDay = ((mYear-2016)*365)+(mMonth*31)+mDate;
    }

    public boolean equals(Object _item){

        if(_item == null) return false;

        if(_item instanceof DateInfo){
            DateInfo item = (DateInfo)_item;
            return (mTimeDay==item.mTimeDay);
        }else{
            return false;
        }
    }

    public static DateInfo getCurrentDate(){
        // get current time
        Calendar calendar = new GregorianCalendar();
        Date trialTime = new Date();
        calendar.setTime(trialTime);

        DateInfo date = new DateInfo(calendar.get(Calendar.YEAR),
                        calendar.get(Calendar.MONTH),
                        calendar.get(Calendar.DATE));
        return date;
    }

}
