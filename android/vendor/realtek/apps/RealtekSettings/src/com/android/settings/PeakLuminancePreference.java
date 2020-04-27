/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.settings;

import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.os.*;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.widget.SeekBar;
import com.android.settings.SeekBarDialogPreference;
import android.view.LayoutInflater;
import android.widget.TextView;
import com.android.settings.CustomDialogPreference;
import com.realtek.hardware.RtkVoutUtilManager;

public class PeakLuminancePreference extends CustomDialogPreference implements
        SeekBar.OnSeekBarChangeListener {
    private static final String TAG = "PeakLuminancePreference";
    private SeekBar mSeekBar;
    private TextView text0;
    private TextView text1;
    private TextView text2;
    private TextView text3;
    private TextView text4;
    private TextView text5;
    private TextView text6;
    private TextView text7;
    private TextView text8;
    private TextView text9;
    private final int MAX_LEVEL = 9;

    private int mOldLevel;
    private final String PROPERTY_PEAK_LUMINANCE = "persist.rtk.video.peakluminance";
    private boolean mRestoredOldState = false;

    private boolean mTouchInProgress;
    private final LayoutInflater mLayoutInflater;
    public PeakLuminancePreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        mLayoutInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        setDialogLayoutResource(R.layout.preference_peak_luminance);
    }

    @Override
    protected void onPrepareDialogBuilder(Builder builder, DialogInterface.OnClickListener listener) {
        super.onPrepareDialogBuilder(builder, listener);
        View view = mLayoutInflater.inflate(R.layout.preference_peak_luminance, null);
        text0 = (TextView)view.findViewById(R.id.txt_0);
        text1 = (TextView)view.findViewById(R.id.txt_1);
        text2 = (TextView)view.findViewById(R.id.txt_2);
        text3 = (TextView)view.findViewById(R.id.txt_3);
        text4 = (TextView)view.findViewById(R.id.txt_4);
        text5 = (TextView)view.findViewById(R.id.txt_5);
        text6 = (TextView)view.findViewById(R.id.txt_6);
        text7 = (TextView)view.findViewById(R.id.txt_7);
        text8 = (TextView)view.findViewById(R.id.txt_8);
        text9 = (TextView)view.findViewById(R.id.txt_9);
        text0.setText("0");
        text1.setText("1");
        text2.setText("2");
        text3.setText("3");
        text4.setText("4");
        text5.setText("5");
        text6.setText("6");
        text7.setText("7");
        text8.setText("8");
        text9.setText("9");
        mSeekBar = (SeekBar)view.findViewById(R.id.peak_seekbar);
        mSeekBar.setMax(MAX_LEVEL);
        mOldLevel = SystemProperties.getInt(PROPERTY_PEAK_LUMINANCE, 4);
        mSeekBar.setProgress(mOldLevel);
        mSeekBar.setOnSeekBarChangeListener(this);
        Log.d(TAG, "Old Level=" + mOldLevel);
        builder.setView(view);
    }

/*    @Override
    protected void onBindDialogView(View view) {
        super.onBindDialogView(view);

        mSeekBar = getSeekBar(view);
        mSeekBar.setMax(MAX_LEVEL);
        mOldLevel = SystemProperties.getInt(PROPERTY_PEAK_LUMINANCE, 0);
        mSeekBar.setProgress(mOldLevel);
        mSeekBar.setOnSeekBarChangeListener(this);
        Log.d(TAG, "Old Level=" + mOldLevel);
    }*/

    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromTouch) {
        if (!mTouchInProgress) {
            Log.d(TAG, "new Level=" + mSeekBar.getProgress());
        }
    }

    public void onStartTrackingTouch(SeekBar seekBar) {
        mTouchInProgress = true;
    }

    public void onStopTrackingTouch(SeekBar seekBar) {
        mTouchInProgress = false;
    }

    @Override
    protected void onDialogClosed(boolean positiveResult) {
        super.onDialogClosed(positiveResult);
        if (positiveResult) {
            int mNewLevel = mSeekBar.getProgress();
            Log.d(TAG, "Save Level =" + mNewLevel);
            SystemProperties.set(PROPERTY_PEAK_LUMINANCE, Integer.toString(mNewLevel));
            new RtkVoutUtilManager().setPeakLuminance(mNewLevel);
        } else {
            restoreOldState();
        }
    }

    private void restoreOldState() {
        if (mRestoredOldState) return;

        mRestoredOldState = true;
    }

    @Override
    protected Parcelable onSaveInstanceState() {
        final Parcelable superState = super.onSaveInstanceState();
        if (getDialog() == null || !getDialog().isShowing()) return superState;

        // Save the dialog state
        final SavedState myState = new SavedState(superState);
        myState.progress = mSeekBar.getProgress();
        myState.oldLevel = mOldLevel;

        // Restore the old state when the activity or dialog is being paused
        restoreOldState();
        return myState;
    }

    @Override
    protected void onRestoreInstanceState(Parcelable state) {
        if (state == null || !state.getClass().equals(SavedState.class)) {
            // Didn't save state for us in onSaveInstanceState
            super.onRestoreInstanceState(state);
            return;
        }

        SavedState myState = (SavedState) state;
        super.onRestoreInstanceState(myState.getSuperState());
        mOldLevel = myState.oldLevel;
        mSeekBar.setProgress(myState.progress);
    }

    private static class SavedState extends BaseSavedState {
        int progress;
        int oldLevel;

        SavedState(Parcel source) {
            super(source);
            progress = source.readInt();
            oldLevel = source.readInt();
        }

        @Override
        public void writeToParcel(Parcel dest, int flags) {
            super.writeToParcel(dest, flags);
            dest.writeInt(progress);
            dest.writeInt(oldLevel);
        }

        SavedState(Parcelable superState) {
            super(superState);
        }

        public static final Parcelable.Creator<SavedState> CREATOR =
                new Parcelable.Creator<SavedState>() {

            public SavedState createFromParcel(Parcel in) {
                return new SavedState(in);
            }

            public SavedState[] newArray(int size) {
                return new SavedState[size];
            }
        };
    }
}

