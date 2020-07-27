package com.example.hdmirxdemo;

import android.widget.LinearLayout;
import android.content.Context;
import android.widget.TextView;
import android.view.View;
import android.util.AttributeSet;
import android.util.Log;
import android.os.Bundle;
import android.graphics.Color;
import android.widget.ImageView;

import java.util.ArrayList;

public class RecordInfoWindowView extends LinearLayout {

    private static final String TAG="RecordInfoWindowView";

    public static final int ICON_REC=1;
    public static final int ICON_UDP=2;

    private Context mContext = null;
    private TextView mInfoTextView = null;
    private ImageView mInfoImageView = null;

    public RecordInfoWindowView(Context c) {
        super(c);
        mContext = c;
        Log.d(TAG,"RecordInfoWindowView(c)");
    }

    public RecordInfoWindowView(Context c, AttributeSet attrs) {
        super(c,attrs);
        mContext = c;
        Log.d(TAG,"RecordInfoWindowView(c,attrs)");
    }

    public RecordInfoWindowView(Context c, AttributeSet attrs, int defStyleAttr) {
        super(c,attrs,defStyleAttr);
        mContext = c;
        Log.d(TAG,"RecordInfoWindowView(c,attrs, defStyleAttr)");
    }

    public RecordInfoWindowView(Context c, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(c,attrs,defStyleAttr,defStyleRes);
        mContext = c;
        Log.d(TAG,"RecordInfoWindowView(c,attrs, defStyleAttr,defStyleRes)");
    }

    public void initMembers(){
        if(mInfoTextView==null)
            mInfoTextView = (TextView) findViewById(R.id.info_textview);
        if(mInfoImageView==null)
            mInfoImageView = (ImageView) findViewById(R.id.info_imageview);
    }

    public void setIcon(int icon){
        initMembers();
        if(icon==ICON_REC)
            mInfoImageView.setImageResource(R.drawable.info_rec);
        else
            mInfoImageView.setImageResource(R.drawable.info_udp);
    }

    public void setText(String text) {
        initMembers();
        mInfoTextView.setText(text);
    }
}
