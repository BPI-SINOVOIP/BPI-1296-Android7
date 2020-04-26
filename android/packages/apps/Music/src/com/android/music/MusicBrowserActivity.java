/*
 * Copyright (C) 2007 The Android Open Source Project
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

package com.android.music;

import com.android.music.MusicUtils.ServiceToken;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.Manifest;
import android.support.v4.content.ContextCompat;
import android.support.v4.app.ActivityCompat;
import android.content.pm.PackageManager;

public class MusicBrowserActivity extends Activity
    implements MusicUtils.Defs {

    private ServiceToken mToken;
    private static final String TAG = "MusicBrowserActivity";
    public final static int  PERMISSION_CONTACTS = 0;
    public MusicBrowserActivity() {
    }

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        Log.d(TAG, "onCreate");
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            Log.d(TAG, "App do not have READ_EXTERNAL_STORAGE permission");
            ActivityCompat.requestPermissions(this, new String[] {Manifest.permission.READ_EXTERNAL_STORAGE}, PERMISSION_CONTACTS);
        } else {
            int activeTab = MusicUtils.getIntPref(this, "activetab", R.id.artisttab);
            if (activeTab != R.id.artisttab
                    && activeTab != R.id.albumtab
                    && activeTab != R.id.songtab
                    && activeTab != R.id.playlisttab) {
                activeTab = R.id.artisttab;
            }
            MusicUtils.activateTab(this, activeTab);

            String shuf = getIntent().getStringExtra("autoshuffle");
            if ("true".equals(shuf)) {
                mToken = MusicUtils.bindToService(this, autoshuffle);
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
            String permissions[], int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode) {
            case PERMISSION_CONTACTS:
                if (grantResults.length > 0
                    && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    Log.d(TAG, "Permissions granted");
                    int activeTab = MusicUtils.getIntPref(this, "activetab", R.id.artisttab);
                    if (activeTab != R.id.artisttab
                            && activeTab != R.id.albumtab
                            && activeTab != R.id.songtab
                            && activeTab != R.id.playlisttab) {
                        activeTab = R.id.artisttab;
                    }
                    MusicUtils.activateTab(this, activeTab);

                    String shuf = getIntent().getStringExtra("autoshuffle");
                    if ("true".equals(shuf)) {
                        mToken = MusicUtils.bindToService(this, autoshuffle);
                    }
                } else {
                    MusicBrowserActivity.this.finish();
                }
                break;
            default:
                break;
        }
    }

    @Override
    public void onDestroy() {
        Log.d(TAG, "onDestroy");
        if (mToken != null) {
            MusicUtils.unbindFromService(mToken);
        }
        super.onDestroy();
    }

    private ServiceConnection autoshuffle = new ServiceConnection() {
        public void onServiceConnected(ComponentName classname, IBinder obj) {
            // we need to be able to bind again, so unbind
            try {
                unbindService(this);
            } catch (IllegalArgumentException e) {
            }
            IMediaPlaybackService serv = IMediaPlaybackService.Stub.asInterface(obj);
            if (serv != null) {
                try {
                    serv.setShuffleMode(MediaPlaybackService.SHUFFLE_AUTO);
                } catch (RemoteException ex) {
                }
            }
        }

        public void onServiceDisconnected(ComponentName classname) {
        }
    };

}

