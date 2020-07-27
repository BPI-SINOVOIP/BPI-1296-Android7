package com.example.hdmirxdemo;

import android.widget.LinearLayout;
import android.content.Context;
import android.widget.TextView;
import android.widget.Spinner;
import android.view.View;
import android.util.AttributeSet;
import android.util.Log;
import android.os.Bundle;
import android.graphics.Color;
import android.widget.ArrayAdapter;

import java.util.ArrayList;

public class SpinnerWidget extends LinearLayout {

    private static final String TAG="SpinnerWidget";

    public static final String KEY_TITLE="title";
    public static final String KEY_ITEMS="items";

    private Context mContext = null;
    private TextView mTitleTextView = null;
    private Spinner mSpinner = null;
    private CharSequence[] mItems;

    public SpinnerWidget(Context c) {
        super(c);
        mContext = c;
        Log.d(TAG,"SpinnerWidget(c)");
    }

    public SpinnerWidget(Context c, AttributeSet attrs) {
        super(c,attrs);
        mContext = c;
        Log.d(TAG,"SpinnerWidget(c,attrs)");
    }

    public SpinnerWidget(Context c, AttributeSet attrs, int defStyleAttr) {
        super(c,attrs,defStyleAttr);
        mContext = c;
        Log.d(TAG,"SpinnerWidget(c,attrs, defStyleAttr)");
    }

    public SpinnerWidget(Context c, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(c,attrs,defStyleAttr,defStyleRes);
        mContext = c;
        Log.d(TAG,"SpinnerWidget(c,attrs, defStyleAttr,defStyleRes)");
    }

    private void initMembers() {
        mTitleTextView = (TextView) findViewById(R.id.title_textview);
        mSpinner = (Spinner) findViewById(R.id.item_spinner);
    }

    public void setParameters(Bundle arg) {
        initMembers();

        CharSequence title = arg.getCharSequence(KEY_TITLE,"N/A");
        mTitleTextView.setText(title);
        mItems = arg.getCharSequenceArray(KEY_ITEMS);

        ArrayList<CharSequence> mItemList = new ArrayList<CharSequence>();
        for(int i=0;i<mItems.length;i++) {
            mItemList.add(mItems[i]);
        }

        ArrayAdapter<CharSequence> adapter = new ArrayAdapter<CharSequence> (
                mContext,
                android.R.layout.simple_spinner_item,
                mItemList);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mSpinner.setAdapter(adapter);
    }

    public String getSelectedString() {
        if(mSpinner != null) {
            return (String)mSpinner.getSelectedItem();
        }else{
            return null;
        }
    }

    public int getSelectedPosition() {
        if(mSpinner != null)
            return mSpinner.getSelectedItemPosition();
        else
            return -1;
    }

    public void setSelection(String value){
        if(value==null)
            return;
        int selection = -1;
        for(int i=0;i<mItems.length;i++){
            if(value.compareTo((String)mItems[i]) == 0){
                selection = i;
                break;
            }
        }
        if(selection>=0){
            mSpinner.setSelection(selection);
        }
    }
}
