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

package com.android.settings.name;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.text.Editable;
import android.text.InputFilter;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.android.settings.R;

/**
 * Dialog fragment for renaming the device.
 */
public final class DeviceNameDialogFragment extends DialogFragment implements TextWatcher {

    private AlertDialog mAlertDialog;
    private Button mOkButton;

    // accessed from inner class (not private to avoid thunks)
    static final String TAG = "DeviceNameDialogFragment";
    EditText mDeviceNameView;

    // This flag is set when the name is updated by code, to distinguish from user changes
    private boolean mDeviceNameUpdated;

    // This flag is set when the user edits the name (preserved on rotation)
    private boolean mDeviceNameEdited;

    // Key to save the edited name and edit status for restoring after rotation
    private static final String KEY_NAME = "device_name";
    private static final String KEY_NAME_EDITED = "device_name_edited";

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        String deviceName = DeviceManager.getDeviceName(getActivity());
        if (savedInstanceState != null) {
            deviceName = savedInstanceState.getString(KEY_NAME, deviceName);
            mDeviceNameEdited = savedInstanceState.getBoolean(KEY_NAME_EDITED, false);
        }
        final LayoutInflater layoutInflater = (LayoutInflater)getActivity().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View view = layoutInflater.inflate(R.layout.dialog_edittext, null);
        mAlertDialog = new AlertDialog.Builder(getActivity())
                .setTitle(getString(R.string.device_rename_title, Build.MODEL))
                .setView(createDialogView(deviceName))
                .setPositiveButton(R.string.bluetooth_rename_button,
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                                String deviceName = mDeviceNameView.getText().toString();
                                DeviceManager.setDeviceName(getActivity(),  deviceName);
                            }
                        })
                .setNegativeButton(android.R.string.cancel, null)
                .create();
        mAlertDialog.getWindow().setSoftInputMode(
                WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);

        return mAlertDialog;
    }


    @Override
    public void onSaveInstanceState(Bundle outState) {
        outState.putString(KEY_NAME, mDeviceNameView.getText().toString());
        outState.putBoolean(KEY_NAME_EDITED, mDeviceNameEdited);
    }

    private View createDialogView(String deviceName) {
        final LayoutInflater layoutInflater = (LayoutInflater)getActivity()
            .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View view = layoutInflater.inflate(R.layout.dialog_edittext, null);
        mDeviceNameView = (EditText) view.findViewById(R.id.edittext);
        mDeviceNameView.setText(deviceName);    // set initial value before adding listener
        mDeviceNameView.addTextChangedListener(this);
        mDeviceNameView.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
                if (actionId == EditorInfo.IME_ACTION_DONE) {
                    DeviceManager.setDeviceName(getActivity(), v.getText().toString());
                    mAlertDialog.dismiss();
                    return true;    // action handled
                } else {
                    return false;   // not handled
                }
            }
        });
        return view;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        mAlertDialog = null;
        mDeviceNameView = null;
        mOkButton = null;
    }

    @Override
    public void onResume() {
        super.onResume();
        if (mOkButton == null) {
            mOkButton = mAlertDialog.getButton(DialogInterface.BUTTON_POSITIVE);
            mOkButton.setEnabled(mDeviceNameEdited);    // Ok button enabled after user edits
        }
    }

    @Override
    public void onPause() {
        super.onPause();
    }

    public void afterTextChanged(Editable s) {
        if (mDeviceNameUpdated) {
            // Device name changed by code; disable Ok button until edited by user
            mDeviceNameUpdated = false;
            mOkButton.setEnabled(false);
        } else {
            mDeviceNameEdited = true;
            if (mOkButton != null) {
                mOkButton.setEnabled(s.toString().trim().length() != 0);
            }
        }
    }

    public void onConfigurationChanged(Configuration newConfig, CharSequence s) {
        super.onConfigurationChanged(newConfig);
        if (mOkButton != null) {
            mOkButton.setEnabled(s.length() != 0 && !(s.toString().trim().isEmpty()));
        }
    }

    /* Not used */
    public void beforeTextChanged(CharSequence s, int start, int count, int after) {
    }

    /* Not used */
    public void onTextChanged(CharSequence s, int start, int before, int count) {
    }
}
