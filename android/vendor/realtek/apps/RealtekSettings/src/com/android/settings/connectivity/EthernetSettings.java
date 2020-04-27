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

package com.android.settings.connectivity;

//import static android.net.ethernet.EthernetManager.ETHERNET_STATE_DISABLED;
//import static android.net.ethernet.EthernetManager.ETHERNET_STATE_ENABLED;

import com.android.settings.R;
import com.android.settings.SettingsPreferenceFragment;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.preference.Preference;
import android.preference.PreferenceScreen;
import android.util.Log;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.MenuItem;
import android.view.View;
import android.os.AsyncTask;
import android.view.LayoutInflater;
import android.widget.EditText;
import android.widget.Toast;
import android.provider.Settings;
import android.widget.CheckBox;
import android.app.ProgressDialog;

import com.android.server.pppoe.PppoeUtil;
import com.android.internal.logging.MetricsLogger;
import com.android.internal.logging.MetricsProto.MetricsEvent;

public class EthernetSettings extends SettingsPreferenceFragment implements
        ConnectivityListener.Listener,
        Preference.OnPreferenceClickListener, Preference.OnPreferenceChangeListener,
        DialogInterface.OnClickListener, DialogInterface.OnDismissListener {

    private static final String TAG = "EthernetSettings";

    private ConnectivityListener mConnectivityListener;

    //*******************************************************//
    private static final String KEY_CONNECTION = "eth_connection";
    private static final String KEY_ETH_CONFIG = "eth_configure";
    private static final String KEY_IP_PRE = "current_ip_address";
    private static final String KEY_MAC_PRE = "mac_address";
    private static final String KEY_PPPOE = "pppoe_conf";

    private Preference mEthConnection;
    private Preference mMacPreference;
    private Preference mIpPreference;
    private Preference mEthConfigure;
    private EthernetDialog mDialog;
   private Preference mPppoeConfigure;
    private EthernetConfig mConfiguration;
   private PppoeUtil mPppoe;

    public EthernetSettings() {}

    @Override
    public void onCreate(Bundle savedInstanceState) {
        mConnectivityListener = new ConnectivityListener(getActivity(), this);
        mConnectivityListener.start();
        mConfiguration = new EthernetConfig(getActivity());
        mConfiguration.load();

        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.ethernet_settings);
        mEthConnection = findPreference(KEY_CONNECTION);
        mEthConfigure = findPreference(KEY_ETH_CONFIG);
        mIpPreference = findPreference(KEY_IP_PRE);
        mMacPreference= findPreference(KEY_MAC_PRE);
       mPppoeConfigure = findPreference(KEY_PPPOE);

       mPppoe = PppoeUtil.getInstance(getActivity().getApplicationContext());
        updateAllOptions();
    }

    @Override
    public void onSaveInstanceState(Bundle savedState) {

    }

    @Override
    public void onStop() {
        mConnectivityListener.stop();
        super.onStop();
    }

    @Override
    public void onResume() {
        mConnectivityListener.start();
        super.onResume();
    }

    @Override
    public void onPause() {
        mConnectivityListener.stop();
        super.onPause();
    }

    @Override
    public void onDestroy() {
        mConnectivityListener = null;
        super.onDestroy();
    }

    @Override
    public void onDismiss(DialogInterface dialog) {
        // Here is the exit of a dialog.
        mDialog = null;
    }

    @Override
    public void onClick(DialogInterface dialog, int whichbutton) {
        /**  get the information form dialog.  **/
        int result = mDialog.saveIpConfig();
        if (result != 0) {
            Toast.makeText(getActivity(), result, Toast.LENGTH_LONG).show();
            return;
        }
        Log.d(TAG, "ipConfig:" + mConfiguration.getIpConfiguration().toString());
        if (whichbutton == DialogInterface.BUTTON_POSITIVE) {

            new AsyncTask<Void, Void, Void>(){
                protected void onPreExecute(){
                    //Disable button
                    //mEthEnable.setEnabled(false);
                    mEthConfigure.setEnabled(false);
                    //mEthDevices.setEnabled(false);
                }

                @Override
                protected Void doInBackground(Void... unused){
                    try{
                        mConfiguration.setIpConfiguration(mConfiguration.getIpConfiguration());
                        mConfiguration.save(null);
                        Thread.sleep(1000);
                    }catch(Exception e){
                    }
                    return null;
                }

                protected void onProgressUpdate(Void... unused){
                }

                protected void onPostExecute(Void unused) {
                    mEthConfigure.setEnabled(true);
                }
            }.execute();

        }else if(whichbutton == DialogInterface.BUTTON_NEUTRAL){
            //mDialog.cancel();
            //mDialog = new EthernetDialog(getActivity(), this, devIfo, true);
            //mDialog.setOnDismissListener(this);
            //mDialog.show();
        }
    }

    @Override
    public void onCreateContextMenu(ContextMenu menu, View view, ContextMenuInfo info) {

    }

    @Override
    public boolean onContextItemSelected(MenuItem item) {
        return false;
    }

    @Override
    public boolean onPreferenceClick(Preference preference) {
        /*
        if (preference instanceof EthPreference) {
            mDialog = new EthernetDialog(getActivity(), this, 
                            ((EthPreference)preference).getConfigure(), false);
            mDialog.setOnDismissListener(this);
            mDialog.show();
        }*/
        return true;
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue){
        Log.d(TAG, "onPreferenceChange = setEthEnabled=" + newValue);
        setEthEnabled((Boolean)newValue);
        return false;
    }

    private void setEthEnabled(final boolean enable){

        new AsyncTask<Void, Void, Void>(){

            protected void onPreExecute(){
                //Disable button
                Log.d(TAG, "setEthEnabled : onPreExecute() : enter");
                mEthConfigure.setEnabled(false);
            }

            @Override
            protected Void doInBackground(Void... unused){
                return null;
            }

            protected void onProgressUpdate(Void... unused){
            }

            protected void onPostExecute(Void unused) {
                Log.d(TAG, "setEthEnabled : onPostExecute() : enter ");
                mEthConfigure.setEnabled(true);
            }
        }.execute();
    }

    @Override
    public boolean onPreferenceTreeClick(Preference preference) {
        if (preference == mEthConfigure) {
            mDialog = new EthernetDialog(getActivity(), this, mConfiguration.getIpConfiguration(), true);
            mDialog.setOnDismissListener(this);
            mDialog.show();
        }
        if (preference == mPppoeConfigure) {
            showPppoeDialog();
        }
        return super.onPreferenceTreeClick(preference);
    }

    // ConnectivityListener.Listener overrides.
    @Override
    public void onConnectivityChange(Intent intent) {
        //TODO: update connection status
        updateAllOptions();
    }

    @Override
    protected int getMetricsCategory() {
        return MetricsEvent.MAIN_SETTINGS;
    }

    private void handleEvent(Context context, Intent intent) {
        /*
        String action = intent.getAction();
        if (EthernetManager.ETHERNET_STATE_CHANGED_ACTION.equals(action)) {
            final EthernetDevInfo devinfo = (EthernetDevInfo)
                                intent.getParcelableExtra(EthernetManager.EXTRA_ETHERNET_INFO);
            final int event = intent.getIntExtra(EthernetManager.EXTRA_ETHERNET_STATE,
                                                                    EthernetManager.EVENT_NEWDEV);

            if(event == EthernetManager.EVENT_NEWDEV || event == EthernetManager.EVENT_DEVREM){
                if(mSelected != null){
                    upDeviceList(mSelected.getConfigure());
                }else{
                    upDeviceList(null);
                }
            }
        } else if (EthernetManager.NETWORK_STATE_CHANGED_ACTION.equals(action)) {
            final NetworkInfo networkInfo = (NetworkInfo)
                    intent.getParcelableExtra(EthernetManager.EXTRA_NETWORK_INFO);
            final LinkProperties linkProperties = (LinkProperties)
                    intent.getParcelableExtra(EthernetManager.EXTRA_LINK_PROPERTIES);
            final int event = intent.getIntExtra(EthernetManager.EXTRA_ETHERNET_STATE,
                                                EthernetManager.EVENT_CONFIGURATION_SUCCEEDED);

            switch(event){
            case EthernetManager.EVENT_CONFIGURATION_SUCCEEDED:
                for(LinkAddress l : linkProperties.getLinkAddresses()){
                    mIpPreference.setSummary(l.getAddress().getHostAddress());
                }
                EthernetDevInfo saveInfo = mEthManager.getSavedConfig();
                if((mSelected != null) && (saveInfo != null)){
                    upDeviceList(saveInfo);
                    mEthEnable.setSummaryOn(context.getString(R.string.eth_dev_summaryon)
                            + mSelected.getConfigure().getIfName());
                }
                break;
            case EthernetManager.EVENT_CONFIGURATION_FAILED:
                mIpPreference.setSummary("0.0.0.0");
                break;
            case EthernetManager.EVENT_DISCONNECTED:
                if(mEthEnable.isChecked())
                    mEthEnable.setSummaryOn(context.getString(R.string.eth_dev_summaryoff));
                else
                    mEthEnable.setSummaryOn(context.getString(R.string.eth_dev_summaryoff));
                break;
            default:
                break;
            }
        } */
    }

    public void updateAllOptions() {
        if (mConnectivityListener != null) {
            ConnectivityListener.ConnectivityStatus status = mConnectivityListener.getConnectivityStatus();
            if (status.isEthernetConnected()) {
                mEthConnection.setSummary(getActivity().getString(R.string.connected));
                mEthConfigure.setEnabled(true);
                try {
                    mIpPreference.setSummary(mConnectivityListener.getEthernetIpAddress());
                }catch(Exception e){
                    mIpPreference.setSummary("");
                    Log.d(TAG, "Can Not get IP: " + e);
                }
            } else {
                mEthConnection.setSummary(getActivity().getString(R.string.not_connected));
                mIpPreference.setSummary("0.0.0.0");
            }
            try {
                mMacPreference.setSummary(mConnectivityListener.getEthernetMacAddress());
            }catch(Exception e){
                Log.d(TAG, "Can not get MAC address: " + e);
            }
        }

        if (1 == mPppoe.isUp()) {
            mPppoeConfigure.setSummary(getActivity().getString(R.string.connected));
        } else {
            mPppoeConfigure.setSummary(getActivity().getString(R.string.not_connected));
        }
    }

    private void showPppoeDialog() {
        LayoutInflater li = getActivity().getLayoutInflater();
        View prompt = li.inflate(R.layout.dialog_pppoe, null);
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(getActivity());
        alertDialogBuilder.setView(prompt);

        String pppoe_name = Settings.Global.getString(getContentResolver(), Settings.Global.PPPOE_USER);
        String pppoe_pass = Settings.Global.getString(getContentResolver(), Settings.Global.PPPOE_PASS);
        Integer pppoe_on = Settings.Global.getInt(getContentResolver(), Settings.Global.PPPOE_ON, 0);

        final EditText user = (EditText) prompt.findViewById(R.id.pppoe_username);
        final EditText pass = (EditText) prompt.findViewById(R.id.pppoe_password);
        final CheckBox enable_pppoe = (CheckBox) prompt.findViewById(R.id.pppoe_enable);
        if (!"".equals(pppoe_name)) user.setText(pppoe_name);
        if (!"".equals(pppoe_pass)) pass.setText(pppoe_pass);
        if (pppoe_on != null && (pppoe_on == 1))
            enable_pppoe.setChecked(true);
        else
            enable_pppoe.setChecked(false);

        alertDialogBuilder.setTitle("Set PPPoE Account");
        alertDialogBuilder.setCancelable(false)
            .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int id)
                {
                    String password = pass.getText().toString();
                    String username = user.getText().toString();
                    try
                    {
                        if ( username.length()<2 || password.length()<2)
                        {
                            Toast.makeText(getActivity(), "Invalid username or password", Toast.LENGTH_LONG).show();
                            showPppoeDialog();
                        }
                        else
                        {
                            if (enable_pppoe.isChecked()) {
                                Settings.Global.putInt(getContentResolver(), Settings.Global.PPPOE_ON, 1);
                                Settings.Global.putString(getContentResolver(), Settings.Global.PPPOE_USER, username);
                                Settings.Global.putString(getContentResolver(), Settings.Global.PPPOE_PASS, password);
                                if (mPppoe.isUp() != 1) {
                                    new PppoeTask().execute(username, password);
                                }
                            } else {
                                Settings.Global.putInt(getContentResolver(), Settings.Global.PPPOE_ON, 0);
                                new StopPPPoE().execute();
                            }
                        }
                    }catch(Exception e)
                    {
                    }
                }
            });

        alertDialogBuilder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int id)
            {
                dialog.cancel();
            }
        });

        alertDialogBuilder.show();
    }

    public class PppoeTask extends AsyncTask<String, Void, Boolean> {
        private ProgressDialog psDialog;
        @Override
        protected void onPreExecute() {
            psDialog = ProgressDialog.show(getActivity(), null, "Connecting...");
        }

        @Override
        protected Boolean doInBackground(String... params) {
            if (params.length != 2) return false;

            int ret = mPppoe.setup("eth0", params[0], params[1]);
            if (ret < 0) return false;

            ret = mPppoe.start();
            if (ret < 0) return false;

            return true;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            psDialog.dismiss();
            if (!success) {
                Toast.makeText(getActivity(), "PPPoE connection fail !!", Toast.LENGTH_LONG).show();
                Settings.Global.putInt(getContentResolver(), Settings.Global.PPPOE_ON, 0);
            }
            updateAllOptions();
        }
    }

    public class StopPPPoE extends AsyncTask<Void, Void, Void> {
        @Override
        protected void onPreExecute() {
            mPppoeConfigure.setEnabled(false);
        }

        @Override
        protected Void doInBackground(Void... unused) {
            if (1 == mPppoe.isUp()) {
                mPppoe.stop();
            }
            return null;
        }

        protected void onPostExecute(Void unused) {
            updateAllOptions();
            mPppoeConfigure.setEnabled(true);
        }
    }

}
