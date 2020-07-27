package com.example.hdmirxdemo;

import android.widget.ListView;
import android.content.Context;
import android.util.AttributeSet;
import android.graphics.Rect;
import android.util.Log;
import android.widget.AdapterView;
import android.view.View;
import android.view.KeyEvent;

public class SchRecListView extends ListView implements AdapterView.OnItemSelectedListener,
                                                    View.OnKeyListener,
                                                    AdapterView.OnItemClickListener {

    private static final String TAG="SchRecListView";
    private Context mContext;
    private SchRecListViewListener mListener = null;

    public SchRecListView(Context c) {
        super(c);
        mContext = c;
    }

    public SchRecListView(Context c, AttributeSet attrs) {
        super(c,attrs);
        mContext = c;
    }

    public SchRecListView(Context c, AttributeSet attrs, int defStyleAttr) {
        super(c,attrs,defStyleAttr);
        mContext = c;
    }

    public SchRecListView(Context c, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(c,attrs,defStyleAttr,defStyleRes);
        mContext = c;
    }

    public void setListener(SchRecListViewListener l) {
        mListener = l;
    }

    public boolean onKey (View v, int keyCode, KeyEvent event) {

        //Log.d(TAG, "onKey:"+keyCode);
        if(keyCode == KeyEvent.KEYCODE_DPAD_CENTER || keyCode == KeyEvent.KEYCODE_ENTER){
            if(event.getAction() == KeyEvent.ACTION_UP) {
                int position = getSelectedItemPosition();
                if(mListener != null) {
                    mListener.removeSchRecItem2(position);
                }
            }
            return true;
        }else{
            return false;
        }
    }

    public void onItemClick (AdapterView<?> parent,
                View view,
                int position,
                long id) {
        Log.d(TAG, "onItemClick "+position);
        if(mListener != null) {
            mListener.removeSchRecItem2(position);
        }
    }

    public void onItemSelected (AdapterView<?> parent, 
                View view, 
                int position, 
                long id) {

        Log.d(TAG, "onItemSelected "+position);

        /*
         * following routine is replaced by selector
        // first, restore all view
        restoreAllChildViews();

        if(view instanceof SchRecListViewItem) {
            ((SchRecListViewItem)view).updateButton(true);
        }
        */
    }

    public void onNothingSelected (AdapterView<?> parent) {
        Log.d(TAG,"onNothingSelected");
    }

    /*
    public void restoreAllChildViews(){
        int childCount = getChildCount();
        for(int i=0;i<childCount;i++) {
            View v = getChildAt(i);
            //Log.d(TAG, "child at "+i+" view:"+v);
            if(v instanceof SchRecListViewItem) {
                ((SchRecListViewItem)v).updateButton(false);
            }
        }
    }
    */
}
