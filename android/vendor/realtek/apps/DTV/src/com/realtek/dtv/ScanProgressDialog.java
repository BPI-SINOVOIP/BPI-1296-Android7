package com.realtek.dtv;

import java.util.ArrayList;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.content.DialogInterface.OnShowListener;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;

public class ScanProgressDialog extends DialogFragment {
    final String TAG = "DTV_ScanProgressDialog";
    ProgressBar scanProgress;
    TextView tvListTitle, radioListTitle;
    TextView channelStrength, channelQuality;
    ListView tvListView, radioListView;
    FullscreenActivity.UIHandler uiHandler;
    AlertDialog dialog;
    ArrayList<String> tvChList = new ArrayList<String>();
    ArrayList<String> radioChList = new ArrayList<String>();
    ArrayAdapter<String> tvChAdapter;
    ArrayAdapter<String> radioChAdapter;
    
    public static ScanProgressDialog newInstance(int title) {
        ScanProgressDialog frag = new ScanProgressDialog();
        Bundle args = new Bundle();
        args.putInt("title", title);
        frag.setArguments(args);
        return frag;
    }

    public ScanProgressDialog() {
        // TODO Auto-generated constructor stub
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        int title = getArguments().getInt("title");
        Log.d(TAG, "onCreateDialog");
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        // Get the layout inflater
        LayoutInflater inflater = getActivity().getLayoutInflater();
        View layout = inflater.inflate(R.layout.scanprogress_dialog, null);

        scanProgress = (ProgressBar)layout.findViewById(R.id.scanprogressbar);
        channelStrength = (TextView)layout.findViewById(R.id.channel_strenth);
        channelQuality = (TextView)layout.findViewById(R.id.channel_quality);
        
        
        builder.setTitle(title)
               .setView(layout)
               .setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
                
                   @Override
                   public void onClick(DialogInterface dialog, int id) {
                       Log.d(TAG, "click: OK in progress screen");
                       Message completeMessage = uiHandler.obtainMessage(FullscreenActivity.SCAN_COMPLETE);
                       uiHandler.sendMessage(completeMessage);
                       ScanProgressDialog.this.dismiss();
                   }
               })
                .setNegativeButton(android.R.string.cancel, new DialogInterface.OnClickListener() {
                
                   @Override
                   public void onClick(DialogInterface dialog, int arg1) {
                       Message completeMessage = uiHandler.obtainMessage(FullscreenActivity.STOP_SCAN);
                       uiHandler.sendMessage(completeMessage);
                       dialog.dismiss();
                   }
               });
        
        dialog = builder.create();
        dialog.setOnShowListener(new OnShowListener() {
            @Override
            public void onShow(DialogInterface dialog) {
                if (scanProgress.getProgress() < 100) {
                    ((AlertDialog)dialog).getButton(AlertDialog.BUTTON_POSITIVE).setEnabled(false);
                }
            }
        });
        
        tvChAdapter = new ArrayAdapter<String>(getActivity(), android.R.layout.simple_list_item_1, tvChList);
        tvListView = (ListView)layout.findViewById(R.id.tvChannelList);
        tvListView.setAdapter(tvChAdapter);

        radioChAdapter = new ArrayAdapter<String>(getActivity(), android.R.layout.simple_list_item_1, radioChList);
        radioListView = (ListView)layout.findViewById(R.id.radioChannelList);
        radioListView.setAdapter(radioChAdapter);
        
        tvListTitle    = (TextView)layout.findViewById(R.id.tvChannelListTitle);
        radioListTitle = (TextView)layout.findViewById(R.id.radioChannelListTitle);
        
        return dialog;
        //return builder.create();
    }
/*
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        Log.d(TAG, "onCreateView");
        View view = inflater.inflate(R.layout.activity_fullscreen, container, false);
        scanProgress = (ProgressBar)view.findViewById(R.id.scanprogressbar);
        return view;
    }*/
    public void setStatus(int p, int s, int q, ArrayList<String> tv, ArrayList<String> radio) {
        Log.d(TAG, "setStatus() progress:"+ p + " " + radio.size());

        tvListTitle.setText(getResources().getString(R.string.tv_channel) + " (" + tv.size() + "CHs)");
        radioListTitle.setText(getResources().getString(R.string.radio_channel) + " (" + radio.size() + "CHs)");

        if (scanProgress != null) {
            scanProgress.setProgress(p);
            if (p == 100) {
                dialog.getButton(AlertDialog.BUTTON_POSITIVE).setEnabled(true);
                dialog.getButton(AlertDialog.BUTTON_NEGATIVE).setEnabled(false);
            }
        }
        if (channelStrength != null) {
            channelStrength.setText("Strength: " + s);
        }
        if (channelQuality != null) {
            channelQuality.setText("Quality: " + q);
        }
        tvChList.clear();
        tvChList.addAll(tv);
        tvChAdapter.notifyDataSetChanged();
        
        radioChList.clear();
        radioChList.addAll(radio);
        radioChAdapter.notifyDataSetChanged();
    }

    public void setHandler(FullscreenActivity.UIHandler h) {
        uiHandler = h;
    }
}
