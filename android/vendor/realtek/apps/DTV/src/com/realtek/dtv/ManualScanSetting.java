package com.realtek.dtv;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

public class ManualScanSetting extends DialogFragment {
    public interface ManualSettingListener {
        public void onManualScanClick(DialogFragment dialog);
    }
    
    ManualSettingListener mListener;
    
    final String TAG = "DTV ManualScanSetting";
    int bandwidth = 0;
    int frequency = 0;
    
    Spinner bandWidthSpinner;
    EditText frequencyText;

    public ManualScanSetting() {
    }

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        
        try {
            mListener = (ManualSettingListener) activity;
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString()
                    + " must implement NoticeDialogListener");
        }
    }
    
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState){
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        // Get the layout inflater
        LayoutInflater inflater = getActivity().getLayoutInflater();
        View layout = inflater.inflate(R.layout.manual_scan_setting, null);

        frequencyText = (EditText)layout.findViewById(R.id.frequencytext);
        frequencyText.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            
            @Override
            public boolean onEditorAction(TextView v, int arg1, KeyEvent arg2) {
                String f = frequencyText.getText().toString();
                if (f == null || f.trim().equals("")) {
                    Log.d(TAG, "setError");
                    frequencyText.setError(getString(R.string.wrong_frequency)); // log printed but can't see setError()
                }

                frequency = (int)(Float.parseFloat(frequencyText.getText().toString()) * 1000000f);

                Log.d(TAG, "frequency:" + frequency);
                return false;
            }
        });

        bandWidthSpinner = (Spinner)layout.findViewById(R.id.bandwidthSpinner);
        bandWidthSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                Log.d(TAG, "ms click:" + position + " id:" + id);
                bandwidth = position + 6;
            }
            public void onNothingSelected(AdapterView<?> arg0) {
                Toast.makeText(getActivity(), "nothing is selected", Toast.LENGTH_LONG).show();
            }
        });

        builder.setTitle(R.string.manual_scan)
                .setView(layout)
                .setPositiveButton(R.string.scan, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int id) {
                        String f = frequencyText.getText().toString();
                        if (f == null || f.trim().equals("")) {
                            Log.d(TAG, "setError");
                            frequencyText.setError(getString(R.string.wrong_frequency)); // log printed but can't see setError()
                        }
                        frequency = (int)(Float.parseFloat(frequencyText.getText().toString()) * 1000000f);
                        //Log.d(TAG, "frequency:" + frequency);
                        mListener.onManualScanClick(ManualScanSetting.this);
                    }
                })
                .setNegativeButton(android.R.string.cancel, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int arg1) {
                        dialog.dismiss();                        
                    }
                });


        return builder.create();
    }
    
    public int getFrequency() {
        return frequency;
    }
    
    public int getBandwidth() {
        return bandwidth;
    }
}
