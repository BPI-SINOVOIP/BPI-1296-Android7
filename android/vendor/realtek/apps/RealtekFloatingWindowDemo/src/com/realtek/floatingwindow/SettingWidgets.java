package com.realtek.floatingwindow;

import android.widget.LinearLayout;
import android.content.Context;
import android.util.AttributeSet;
import android.widget.TextView;
import android.widget.Spinner;
import android.widget.CheckBox;
import android.widget.ArrayAdapter;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.os.Bundle;
import android.view.View;

public class SettingWidgets extends LinearLayout {

    public static final String KEY_ENABLED      = "enabled";
    public static final String KEY_VIEWTYPE     = "viewType";
    public static final String KEY_RECORD       = "record";
    public static final String KEY_SIZE         = "windowSize";
    public static final String KEY_RECORD_PATH  = "recordPath";

    public static final int TYPE_SURFACEVIEW    = 0;
    public static final int TYPE_TEXTUREVIEW    = 1;
    public static final int TYPE_NOPREVIEW      = 2;

    public static final int WINDOW_HD_1920 = 2;
    public static final int WINDOW_HD_1280 = 1;
    public static final int WINDOW_SD = 0;

    private boolean bCanRecord;
    private String mToken;

    private CheckBox mSourceCheckBox;
    private Spinner mViewTypeSpinner;
    private CheckBox mRecordCheckBox;
    private Spinner mWindowSizeSpinner;
    private TextView mInfoTextView;

    private Context mContext;

    public SettingWidgets(Context context){
        super(context);
        mContext = context;
    }

    public SettingWidgets(Context context, AttributeSet attrs){
        super(context, attrs);
        mContext = context;
    }

    public SettingWidgets(Context context, AttributeSet attrs, int defStyleAttr){
        super(context, attrs, defStyleAttr);
        mContext = context;
    }

    public SettingWidgets(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes){
        super(context, attrs, defStyleAttr, defStyleRes);
        mContext = context;
    }

    public void init(boolean recordEn, String token){
        mSourceCheckBox = (CheckBox) findViewById(R.id.source_checkbox);
        mRecordCheckBox = (CheckBox) findViewById(R.id.record_checkbox);
        mViewTypeSpinner = (Spinner) findViewById(R.id.view_type_spinner);
        mWindowSizeSpinner = (Spinner) findViewById(R.id.window_size_spinner);
        mInfoTextView = (TextView) findViewById(R.id.info_textview);

        mRecordCheckBox.setText("Record enabled");
        mInfoTextView.setText("Info:");

        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
                mContext,
                R.array.view_type_item_2,
                android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mViewTypeSpinner.setAdapter(adapter);

        adapter = ArrayAdapter.createFromResource(
                mContext,
                R.array.window_size_item,
                android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mWindowSizeSpinner.setAdapter(adapter);

        bCanRecord = recordEn;
        mToken = token;

        if(!bCanRecord){
            mRecordCheckBox.setEnabled(false);
        }

    }

    public void setInfoText(String text){
        if(text == null) {text = "";}
        mInfoTextView.setText("Info: "+text);
    }

    public void setLabelText(String sourceText /*, String recordText*/ ){
        mSourceCheckBox.setText(sourceText);
    }

    public void setEnabled(boolean enable){
        super.setEnabled(enable);
        mSourceCheckBox.setEnabled(enable);
        mViewTypeSpinner.setEnabled(enable);
        mRecordCheckBox.setEnabled(enable && bCanRecord);
        mWindowSizeSpinner.setEnabled(enable);
    }

    public Bundle getSettingConfig(){
        Bundle bundle = new Bundle();
        boolean enabled = mSourceCheckBox.isChecked();
        int viewType = mViewTypeSpinner.getSelectedItemPosition();
        boolean record = mRecordCheckBox.isChecked();
        int windowSize = mWindowSizeSpinner.getSelectedItemPosition();

        bundle.putBoolean(KEY_ENABLED,enabled);
        bundle.putInt(KEY_VIEWTYPE,viewType);
        bundle.putBoolean(KEY_RECORD,record);
        bundle.putInt(KEY_SIZE,windowSize);

        if(record){
            String path = 
                CameraHelper.getOutputMediaFile(CameraHelper.MEDIA_TYPE_VIDEO,mToken).toString();
            bundle.putString(KEY_RECORD_PATH,path);
        }

        return bundle;
    }
}
