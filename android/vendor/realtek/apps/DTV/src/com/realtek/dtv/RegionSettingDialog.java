package com.realtek.dtv;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;

public class RegionSettingDialog extends DialogFragment {
    public interface RegionSettingDialogListener {
        public void onRegionSet(DialogFragment dialog);
    }
    RegionSettingDialogListener mListener;
    private int region_id;
    private int tempRegion_id;

    public RegionSettingDialog() {
    }

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        
        try {
            mListener = (RegionSettingDialogListener) activity;
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString()
                    + " must implement RegionSettingDialogListener");
        }
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setTitle(R.string.region)
               .setSingleChoiceItems(R.array.regionArray, region_id - 1, new DialogInterface.OnClickListener() {
                   @Override
                   public void onClick(DialogInterface dialog, int which) {
                       tempRegion_id = which + 1;
                   }
               })
               .setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
                
                   @Override
                   public void onClick(DialogInterface dialog, int id) {
                       Log.d("RegionSettingDialog", "click:" + tempRegion_id);
                       region_id = tempRegion_id;
                       //FullscreenActivity activity = (FullscreenActivity)getActivity();
                       //activity.setRegionID(region_id);
                       
                       mListener.onRegionSet(RegionSettingDialog.this);
                       RegionSettingDialog.this.dismiss();
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
    
    public void setDefaultRegionID(int id) {
        region_id = id;
    }
    
    public int getRegionID() {
        return region_id;
    }
}
