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

import com.android.settings.R;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.security.Credentials;
import android.security.KeyStore;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.EditText;
import android.net.InterfaceConfiguration;
import android.os.INetworkManagementService;
import android.util.Log;

import java.net.InetAddress;

import android.net.IpConfiguration;
import android.net.IpConfiguration.IpAssignment;
import android.net.IpConfiguration.ProxySettings;
import android.net.StaticIpConfiguration;
import java.net.InetAddress;
import java.net.Inet4Address;
import android.net.NetworkUtils;
import android.net.LinkAddress;
import android.text.TextUtils;
import android.content.SharedPreferences;

public class EthernetDialog extends AlertDialog implements TextWatcher,
        View.OnClickListener, AdapterView.OnItemSelectedListener {

    public static final String TAG = "EthernetDialog";
    private final Context mContext;
    private final KeyStore mKeyStore = KeyStore.getInstance();
    private final DialogInterface.OnClickListener mListener;
    private IpConfiguration m_ipConfiguration;

    private boolean mEditing;

    private View mView;

    private CheckBox mDhcp_choice;
    private EditText mIpaddr;
    private EditText mMask;
    private EditText mDns1;
    private EditText mDns2;
    private EditText mGw;
    private SharedPreferences pref;
    private static final String PREF_DATA = "data_eth";
    private static final String Field_IP = "eth_ip";
    private static final String Field_PREFIX = "eth_prefix";
    private static final String Field_GW = "eth_gateway";
    private static final String Field_DNS1 = "eth_dns1";
    private static final String Field_DNS2 = "eth_dns2";

    public EthernetDialog(Context context, DialogInterface.OnClickListener listener, IpConfiguration ipConfiguration, boolean editing) {
        super(context);
        mContext = context;
        mListener = listener;
        mEditing = editing;
        m_ipConfiguration = ipConfiguration;
    }

    @Override
    protected void onCreate(Bundle savedState) {
        mView = getLayoutInflater().inflate(R.layout.ethernet_dialog, null);
        setTitle(R.string.eth_advanced_configure);
        setView(mView);
        setInverseBackgroundForced(true);
        pref = mContext.getSharedPreferences(PREF_DATA, 0);

        // First, find out all the fields.
        mDhcp_choice = (CheckBox) mView.findViewById(R.id.dhcp_choice);
        mIpaddr = (EditText) mView.findViewById(R.id.ipaddr_edit);
        mMask = (EditText) mView.findViewById(R.id.prefix_edit);
        mGw = (EditText) mView.findViewById(R.id.gw_edit);
        mDns1 = (EditText) mView.findViewById(R.id.dns1_edit);
        mDns2 = (EditText) mView.findViewById(R.id.dns2_edit);

        // Second, copy values from the profile.
        if(m_ipConfiguration.getIpAssignment() == IpAssignment.DHCP){
            mDhcp_choice.setChecked(true);
            mIpaddr.setText(pref.getString(Field_IP, ""));
            mMask.setText(pref.getString(Field_PREFIX, ""));
            mGw.setText(pref.getString(Field_GW, ""));
            mDns1.setText(pref.getString(Field_DNS1, ""));
            mDns2.setText(pref.getString(Field_DNS2, ""));
            mIpaddr.setEnabled(false);
            mMask.setEnabled(false);
            mGw.setEnabled(false);
            mDns1.setEnabled(false);
            mDns2.setEnabled(false);
        }else{
            mDhcp_choice.setChecked(false);
            mIpaddr.setText(getIp());
            mMask.setText(getMask());
            mGw.setText(getGateway());
            mDns1.setText(getDns1());
            mDns2.setText(getDns2());
        }

        // Third, add listeners to required fields.
        mDhcp_choice.setOnClickListener(this);
        mIpaddr.addTextChangedListener(this);
        mMask.addTextChangedListener(this);
        mGw.addTextChangedListener(this);
        mDns1.addTextChangedListener(this);
        mDns2.addTextChangedListener(this);

        setButton(DialogInterface.BUTTON_NEGATIVE, mContext.getString(R.string.eth_cancel), mListener);
        setButton(DialogInterface.BUTTON_POSITIVE, mContext.getString(R.string.eth_ok), mListener);
        super.onCreate(savedState);
    }

    @Override
    public void afterTextChanged(Editable field) {
        //getButton(DialogInterface.BUTTON_POSITIVE).setEnabled(validate(mEditing));
    }

    @Override
    public void beforeTextChanged(CharSequence s, int start, int count, int after) {
    }

    @Override
    public void onTextChanged(CharSequence s, int start, int before, int count) {
    }

    @Override
    public void onClick(View v) {

        if(mDhcp_choice.isChecked()){
            mIpaddr.setEnabled(false);
            mMask.setEnabled(false);
            mDns1.setEnabled(false);
            mDns2.setEnabled(false);
            mGw.setEnabled(false);
        }else{
            mIpaddr.setEnabled(true);
            mMask.setEnabled(true);
            mDns1.setEnabled(true);
            mDns2.setEnabled(true);
            mGw.setEnabled(true);
        }
    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent) {
    }

    private boolean validate(boolean editing) {
        /*
        if (!editing) {
            return mUsername.getText().length() != 0 && mPassword.getText().length() != 0;
        }
        if (mName.getText().length() == 0 || mServer.getText().length() == 0 ||
                !validateAddresses(mDnsServers.getText().toString(), false) ||
                !validateAddresses(mRoutes.getText().toString(), true)) {
            return false;
        }
        switch (mType.getSelectedItemPosition()) {
            case VpnProfile.TYPE_PPTP:
            case VpnProfile.TYPE_IPSEC_HYBRID_RSA:
                return true;

            case VpnProfile.TYPE_L2TP_IPSEC_PSK:
            case VpnProfile.TYPE_IPSEC_XAUTH_PSK:
                return mIpsecSecret.getText().length() != 0;

            case VpnProfile.TYPE_L2TP_IPSEC_RSA:
            case VpnProfile.TYPE_IPSEC_XAUTH_RSA:
                return mIpsecUserCert.getSelectedItemPosition() != 0;
        }
        */
        return false;
    }

    private boolean validateAddresses(String addresses, boolean cidr) {
        try {
            for (String address : addresses.split(" ")) {
                if (address.isEmpty()) {
                    continue;
                }
                // Legacy VPN currently only supports IPv4.
                int prefixLength = 32;
                if (cidr) {
                    String[] parts = address.split("/", 2);
                    address = parts[0];
                    prefixLength = Integer.parseInt(parts[1]);
                }
                byte[] bytes = InetAddress.parseNumericAddress(address).getAddress();
                int integer = (bytes[3] & 0xFF) | (bytes[2] & 0xFF) << 8 |
                        (bytes[1] & 0xFF) << 16 | (bytes[0] & 0xFF) << 24;
                if (bytes.length != 4 || prefixLength < 0 || prefixLength > 32 ||
                        (prefixLength < 32 && (integer << prefixLength) != 0)) {
                    return false;
                }
            }
        } catch (Exception e) {
            return false;
        }
        return true;
    }

    boolean isEditing() {
        return mEditing;
    }

    public int saveIpConfig() {
        if(mEditing){

            if(!mDhcp_choice.isChecked()){
                StaticIpConfiguration staticConfig = new StaticIpConfiguration();

                Inet4Address inetAddr = null;
                try {
                    inetAddr = (Inet4Address) NetworkUtils.numericToInetAddress(mIpaddr.getText().toString());
                } catch (IllegalArgumentException|ClassCastException e) {
                    Log.d(TAG, "IP Address Error!");
                    return R.string.wifi_ip_settings_invalid_ip_address;
                }

                int networkPrefixLength = -1;
                try {
                    networkPrefixLength = Integer.parseInt(mMask.getText().toString());
                    if (networkPrefixLength < 0 || networkPrefixLength > 32) {
                        Log.d(TAG, "network prefix Error!");
                        return R.string.wifi_ip_settings_invalid_network_prefix_length;
                    }
                    staticConfig.ipAddress = new LinkAddress(inetAddr, networkPrefixLength);
                } catch (NumberFormatException e) {
                        Log.d(TAG, "network prefix Error: " + e);
                        return R.string.wifi_ip_settings_invalid_ip_address;
                }

                String gateway = mGw.getText().toString();
                if (!TextUtils.isEmpty(gateway)) {
                    try {
                        staticConfig.gateway =
                            (Inet4Address) NetworkUtils.numericToInetAddress(gateway);
                    } catch (IllegalArgumentException|ClassCastException e) {
                        Log.d(TAG, "gateway Error: " + e);
                        return R.string.wifi_ip_settings_invalid_gateway;
                    }
                } else {
                        return R.string.wifi_ip_settings_invalid_gateway;
                }

                String dns1 = mDns1.getText().toString();
                if (!TextUtils.isEmpty(dns1)) {
                    try {
                        staticConfig.dnsServers.add(
                            (Inet4Address) NetworkUtils.numericToInetAddress(dns1));
                    } catch (IllegalArgumentException|ClassCastException e) {
                        Log.d(TAG, "Dns1 Error: " + e);
                        return R.string.wifi_ip_settings_invalid_dns;
                    }
                } else {
                        return R.string.wifi_ip_settings_invalid_dns;
                }

                String dns2 = mDns2.getText().toString();
                if (!TextUtils.isEmpty(dns2)) {
                    try {
                        staticConfig.dnsServers.add(
                            (Inet4Address) NetworkUtils.numericToInetAddress(dns2));
                    } catch (IllegalArgumentException|ClassCastException e) {
                        Log.d(TAG, "Dns1 Error: " + e);
                        return R.string.wifi_ip_settings_invalid_dns;
                    }
                }

                m_ipConfiguration.setIpAssignment(mDhcp_choice.isChecked() ? IpAssignment.DHCP : IpAssignment.STATIC);
                m_ipConfiguration.setStaticIpConfiguration(staticConfig);

                pref = mContext.getSharedPreferences(PREF_DATA, 0);
                pref.edit()
                    .putString(Field_IP, mIpaddr.getText().toString())
                    .putString(Field_PREFIX, mMask.getText().toString())
                    .putString(Field_GW, mGw.getText().toString())
                    .putString(Field_DNS1, mDns1.getText().toString())
                    .putString(Field_DNS2, mDns2.getText().toString())
                    .commit();
            }else{
                m_ipConfiguration.setIpAssignment(mDhcp_choice.isChecked() ? IpAssignment.DHCP : IpAssignment.STATIC);
                m_ipConfiguration.setStaticIpConfiguration(null);
            }
        }

        return 0;
    }

    private String getIp() {
        try {
            return m_ipConfiguration.getStaticIpConfiguration().ipAddress.getAddress().getHostAddress();
        } catch (NullPointerException e) {
            return "";
        }
    }

    private String getMask() {
        try {
            return Integer.toString(m_ipConfiguration.getStaticIpConfiguration().ipAddress.getNetworkPrefixLength());
        } catch (NullPointerException e) {
            return "";
        }
    }

    private String getGateway() {
        try {
            return m_ipConfiguration.getStaticIpConfiguration().gateway.getHostAddress();
        } catch (NullPointerException e) {
            return "";
        }
    }

    private String getDns1() {
        try {
            if (m_ipConfiguration.getStaticIpConfiguration().dnsServers.size() > 0) {
                return m_ipConfiguration.getStaticIpConfiguration().dnsServers.get(0).getHostAddress();
            } else {
                return "";
            }
        } catch (NullPointerException e) {
            return "";
        }
    }

    private String getDns2() {
        try {
            if (m_ipConfiguration.getStaticIpConfiguration().dnsServers.size() > 1) {
                return m_ipConfiguration.getStaticIpConfiguration().dnsServers.get(1).getHostAddress();
            } else {
                return "";
            }
        } catch (NullPointerException e) {
            return "";
        }
    }

}
