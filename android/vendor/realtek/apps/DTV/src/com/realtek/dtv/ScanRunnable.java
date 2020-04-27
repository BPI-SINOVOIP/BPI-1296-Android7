package com.realtek.dtv;

import com.realtek.app.TvManager;
import com.realtek.app.tv.*;

public class ScanRunnable implements Runnable {
    private int bandwidth = 0;
    private int frequency = 0;
    private TvManager mTvManager;
    
    public ScanRunnable(TvManager m, int b, int f) {
        mTvManager = m;
        bandwidth = b;
        frequency = f;
    }

    @Override
    public void run() {
        // TODO Auto-generated method stub

    }

}
