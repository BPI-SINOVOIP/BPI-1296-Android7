package com.rtk.debug;

import android.content.Context;
import android.os.SystemProperties;

/**
 * Created by archerho on 2016/12/29.
 */
public class DumpDvdplayer implements IDump{
    final String prop = "persist.rtk.dvdlog";
    private static DumpDvdplayer mIns;
    public static IDump getInstance(Context ctx) {
        if(mIns==null)
            mIns = new DumpDvdplayer();
        return mIns;
    }

    @Override
    public void on() {
        SystemProperties.set(prop, "1");
    }

    @Override
    public void off() {
        SystemProperties.set(prop, "0");
    }

    @Override
    public boolean isOn() {
        String s = SystemProperties.get(prop);
        return "1".equals(s);
    }
}
