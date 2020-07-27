package com.example.hdmirxdemo;

import android.content.Intent;

public interface BroadcastListener {

    public abstract void handleBroadcast(Intent intent);
    public abstract void refreshRecordwidgets();

}
