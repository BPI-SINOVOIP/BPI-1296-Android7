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
import com.realtek.floatingwindow.rtk.R;

public class SettingWidgets extends LinearLayout {

    public static final String KEY_ENABLED              = "enabled";
    public static final String KEY_VIEWTYPE             = "viewType";
    public static final String KEY_RECORD               = "record";
    public static final String KEY_SIZE                 = "windowSize";
    public static final String KEY_RECORD_PATH          = "recordPath";
    public static final String KEY_FORCE_FPS            = "forceFPS";
    public static final String KEY_REC_RESOLUTION       = "recResolution";
    public static final String KEY_PREVIEW_RESOLUTION   = "previewResolution";
    public static final String KEY_EXIT_BEHAVIOR        = "exitBehavior";
    public static final String KEY_OUTPUT_PATH          = "outputPath";
    public static final String KEY_OUTPUT_FORMAT        = "outputFormat";

    public static final int TYPE_TEXTUREVIEW    = 0;
    public static final int TYPE_SURFACEVIEW    = 1;
    public static final int TYPE_NOPREVIEW      = 2;

    public static final int WINDOW_HD       = 0;
    public static final int WINDOW_SD       = 1;

    public static final int REC_4K2K        = 0;
    public static final int REC_1080P       = 1;

    public static final int EXIT_TO_PIP     = 0;
    public static final int EXIT_TO_HIDE    = 1;

    public static final int OUTPUT_STORAGE  = 0;
    public static final int OUTPUT_UDP      = 1;

    public static final int FORMAT_TS       = 0;
    public static final int FORMAT_MP4      = 1;

    private String mToken;

    private CheckBox mSourceCheckBox;
    private CheckBox mFpsCheckBox;
    private Spinner mViewTypeSpinner;
    private Spinner mWindowSizeSpinner;
    private TextView mInfoTextView;
    private Spinner mRecResolutionSpinner;
    private CheckBox mPreviewResolutionCheckBox;
    private Spinner mExitBehaviorSpinner;
    private Spinner mOutputSpinner;
    private Spinner mRecFormatSpinner;

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

    public void init(String token){
        mSourceCheckBox = (CheckBox) findViewById(R.id.source_checkbox);
        mFpsCheckBox = (CheckBox) findViewById(R.id.fps_checkbox);
        mViewTypeSpinner = (Spinner) findViewById(R.id.view_type_spinner);
        mWindowSizeSpinner = (Spinner) findViewById(R.id.window_size_spinner);
        mInfoTextView = (TextView) findViewById(R.id.info_textview);
        mRecResolutionSpinner = (Spinner) findViewById(R.id.record_resolution_spinner);
        mPreviewResolutionCheckBox = (CheckBox) findViewById(R.id.preview_resolution_checkbox);
        mExitBehaviorSpinner = (Spinner) findViewById(R.id.exit_behavior_spinner);
        mOutputSpinner = (Spinner) findViewById(R.id.record_output_spinner);
        mRecFormatSpinner = (Spinner) findViewById(R.id.record_format_spinner);

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

        adapter = ArrayAdapter.createFromResource(
                mContext,
                R.array.rec_resolution_item,
                android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mRecResolutionSpinner.setAdapter(adapter);

        adapter = ArrayAdapter.createFromResource(
                mContext,
                R.array.exit_behavior_item,
                android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mExitBehaviorSpinner.setAdapter(adapter);

        adapter = ArrayAdapter.createFromResource(
                mContext,
                R.array.rec_output_item,
                android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mOutputSpinner.setAdapter(adapter);

        adapter = ArrayAdapter.createFromResource(
                mContext,
                R.array.rec_format_item,
                android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mRecFormatSpinner.setAdapter(adapter);

        mToken = token;
        mFpsCheckBox.setText("force 30fps");
        mPreviewResolutionCheckBox.setText("Force preview to 1080p if src is 4K2K");
        //mViewTypeSpinner.setEnabled(false);
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
        mViewTypeSpinner.setEnabled(enable /*&& false*/);
        mWindowSizeSpinner.setEnabled(enable);
        mRecResolutionSpinner.setEnabled(enable);
        mFpsCheckBox.setEnabled(enable);
        mPreviewResolutionCheckBox.setEnabled(enable);
        mExitBehaviorSpinner.setEnabled(enable);
        mOutputSpinner.setEnabled(enable);
        mRecFormatSpinner.setEnabled(enable);
    }

    public Bundle getSettingConfig(){
        Bundle bundle = new Bundle();
        boolean enabled = mSourceCheckBox.isChecked();
        boolean forceFps = mFpsCheckBox.isChecked();
        boolean preview1080p = mPreviewResolutionCheckBox.isChecked();
        int viewType = mViewTypeSpinner.getSelectedItemPosition();
        int windowSize = mWindowSizeSpinner.getSelectedItemPosition();
        int recResolution = mRecResolutionSpinner.getSelectedItemPosition();
        int exitBehavior = mExitBehaviorSpinner.getSelectedItemPosition();
        int outputPath = mOutputSpinner.getSelectedItemPosition();
        int recFormat = mRecFormatSpinner.getSelectedItemPosition();

        bundle.putBoolean(KEY_ENABLED,enabled);
        bundle.putInt(KEY_VIEWTYPE,viewType);
        bundle.putInt(KEY_SIZE,windowSize);
        bundle.putBoolean(KEY_FORCE_FPS,forceFps);
        bundle.putInt(KEY_REC_RESOLUTION,recResolution);
        bundle.putBoolean(KEY_PREVIEW_RESOLUTION,preview1080p);
        bundle.putInt(KEY_EXIT_BEHAVIOR,exitBehavior);
        bundle.putInt(KEY_OUTPUT_PATH,outputPath);
        bundle.putInt(KEY_OUTPUT_FORMAT,recFormat);

        return bundle;
    }
}
