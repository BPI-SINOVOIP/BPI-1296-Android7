package com.example.hdmirxdemo;

import android.content.Context;
import android.widget.RelativeLayout;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.view.View;
import android.util.AttributeSet;
import android.util.Log;
import android.os.Bundle;
import android.graphics.Color;

public class SchRecListViewItem extends LinearLayout /*RelativeLayout*/ implements View.OnClickListener {

    private static final String TAG="SchRecListViewItem";
    private static final boolean DEBUG=true;

    public static final String KEY_DATE_TEXT="date";
    public static final String KEY_TIME_TEXT="time";
    public static final String KEY_DUR_TEXT="duration";
    public static final String KEY_FMT_TEXT="fmt";
    public static final String KEY_PATH_TEXT="path";

    public static final int FMT_TS=1;
    public static final int FMT_MP4=2;

    private Context mContext;

    private TextView mDateTextView;
    private TextView mTimeTextView;
    private TextView mDurationTextView;

    //private TextView mFormatTextView;
    //private TextView mPathTextView;

    private ImageView mRemoveButton;

    public SchRecListViewItem(Context c) {
        super(c);
        mContext = c;
    }

    public SchRecListViewItem(Context c, AttributeSet attrs) {
        super(c,attrs);
        mContext = c;
    }

    public SchRecListViewItem(Context c, AttributeSet attrs, int defStyleAttr) {
        super(c,attrs,defStyleAttr);
        mContext = c;
    }

    public SchRecListViewItem(Context c, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(c,attrs,defStyleAttr,defStyleRes);
        mContext = c;
    }

    public void onClick (View v) {
        Log.d(TAG, "onClick - "+v);
    }

    private void initMembers() {
        mDateTextView = (TextView) findViewById(R.id.date_text_view);
        mTimeTextView = (TextView) findViewById(R.id.time_text_view);
        mDurationTextView = (TextView) findViewById(R.id.duration_text_view);

        mRemoveButton = (ImageView) findViewById(R.id.remove_btn);
        //mFormatTextView = (TextView) findViewById(R.id.format_text_view);
        //mPathTextView = (TextView) findViewById(R.id.path_text_view);

        // Just for experiment...
        //setOnClickListener(this);
    }

    public void setParameters(Bundle arg) {
        initMembers();

        mDateTextView.setText(arg.getCharSequence(KEY_DATE_TEXT,"N/A"));
        mTimeTextView.setText(arg.getCharSequence(KEY_TIME_TEXT,"N/A"));
        mDurationTextView.setText(arg.getCharSequence(KEY_DUR_TEXT,"N/A"));
        //mFormatTextView.setText(arg.getCharSequence(KEY_FMT_TEXT,"N/A"));
        //mPathTextView.setText(arg.getCharSequence(KEY_PATH_TEXT,"N/A"));
    }
}
