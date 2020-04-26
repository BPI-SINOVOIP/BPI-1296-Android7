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

package android.util;

import android.content.ContentResolver;
import android.content.Context;
import android.content.res.Resources;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.SntpClient;
import android.os.SystemClock;
import android.provider.Settings;
import android.text.TextUtils;

import java.util.concurrent.atomic.AtomicInteger;

/**
 * {@link TrustedTime} that connects with a remote NTP server as its trusted
 * time source.
 *
 * @hide
 */
public class NtpTrustedTime implements TrustedTime {
    private static final String TAG = "NtpTrustedTime";
    private static final boolean LOGD = true;

    private static NtpTrustedTime sSingleton;
    private static Context sContext;

    private final String mServer;
    private final long mTimeout;

    private ConnectivityManager mCM;

    private boolean mHasCache;
    private long mCachedNtpTime;
    private long mCachedNtpElapsedRealtime;
    private long mCachedNtpCertainty;

    private NtpTrustedTime(String server, long timeout) {
        if (LOGD) Log.d(TAG, "creating NtpTrustedTime using " + server);
        mServer = server;
        mTimeout = timeout;
    }

    public static synchronized NtpTrustedTime getInstance(Context context) {
        if (sSingleton == null) {
            final Resources res = context.getResources();
            final ContentResolver resolver = context.getContentResolver();

            final String defaultServer = res.getString(
                    com.android.internal.R.string.config_ntpServer);
            final long defaultTimeout = res.getInteger(
                    com.android.internal.R.integer.config_ntpTimeout);

            final String secureServer = Settings.Global.getString(
                    resolver, Settings.Global.NTP_SERVER);
            final long timeout = Settings.Global.getLong(
                    resolver, Settings.Global.NTP_TIMEOUT, defaultTimeout);

            final String server = secureServer != null ? secureServer : defaultServer;
            sSingleton = new NtpTrustedTime(server, timeout);
            sContext = context;
        }

        return sSingleton;
    }

    @Override
    public boolean forceRefresh() {
        if (TextUtils.isEmpty(mServer)) {
            // missing server, so no trusted time available
            return false;
        }

        // We can't do this at initialization time: ConnectivityService might not be running yet.
        synchronized (this) {
            if (mCM == null) {
                mCM = (ConnectivityManager) sContext.getSystemService(Context.CONNECTIVITY_SERVICE);
            }
        }

        final NetworkInfo ni = mCM == null ? null : mCM.getActiveNetworkInfo();
        if (ni == null || !ni.isConnected()) {
            if (LOGD) Log.d(TAG, "forceRefresh: no connectivity");
            return false;
        }


        if (LOGD) Log.d(TAG, "forceRefresh() from cache miss");
        final SntpClient client = getActiveSntpClient();
        if (client.getNtpTime()>0) {
            mCachedNtpTime = client.getNtpTime();
            if (LOGD) Log.d(TAG, "mCachedNtpTime="+mCachedNtpTime);
            mCachedNtpElapsedRealtime = client.getNtpTimeReference();
            if (LOGD) Log.d(TAG, "mCachedNtpElapsedRealtime="+mCachedNtpElapsedRealtime);
            mCachedNtpCertainty = client.getRoundTripTime() / 2;
            if (LOGD) Log.d(TAG, "mCachedNtpCertainty="+mCachedNtpCertainty);
            mHasCache = true;
            return true;
        } else {
            return false;
        }
    }

    /**
     * [BUG_FIX] DHCKYLIN-2131, ntp time sync too slow sometimes
     * [ROOT_CAUSE] Default ntp servers may be unreliable to request ntp time.
     * [SOLUTION] Using multiple threads to request multiple ntp server, return the first success SntpClient.
     */
    private SntpClient getActiveSntpClient(){
        String[] ntpServers = {
            "2.android.pool.ntp.org",
            "0.android.pool.ntp.org",
            "1.android.pool.ntp.org",
            "3.android.pool.ntp.org"
        };
        final SntpClient[] clients = new SntpClient[ntpServers.length];
        Thread[] threads = new Thread[ntpServers.length];
        final AtomicInteger idx = new AtomicInteger(-1);
        for (int i=0;i<ntpServers.length;i++) {
            final int j = i;
            final String server = ntpServers[j];
            final long timeout = mTimeout;
            clients[j] = new SntpClient();
            threads[j] = new Thread(new Runnable(){
                public void run() {
                    if (clients[j].requestTime(server, (int) timeout)) {
                        if(idx.get()==-1)
                            idx.set(j);
                    }
                }
            });
            threads[j].start();
            try{Thread.sleep(250);}catch(InterruptedException e){}
            if(idx.get()!=-1) {
                if (LOGD) Log.d(TAG, "getActiveSntpClient first ret index="+idx.get());
                return clients[idx.get()];
            }
        }
        //Wait 5 sec timeout for result
        final int TIMEOUT = 5000;
        final int SLEEP_INTERVAL = 100;
        final int TICK_COUNT = TIMEOUT/SLEEP_INTERVAL;
        boolean isAllThreadsAlive = false;
        for (int i=0;i<TICK_COUNT;i++){
            if(idx.get()!=-1) break;
            isAllThreadsAlive = false;
            for(int j=0;j<ntpServers.length;j++)
                isAllThreadsAlive|=threads[j].isAlive();
            if(!isAllThreadsAlive) {
                if (LOGD) Log.d(TAG, "getActiveSntpClient allThreadsFinished");
                break;
            }
            try{Thread.sleep(SLEEP_INTERVAL);}catch(InterruptedException e){}
        }
        if(idx.get()<0) {
            if (LOGD) Log.d(TAG, "getActiveSntpClient finished with no result");
            return clients[0];
        } else {
            if (LOGD) Log.d(TAG, "getActiveSntpClient ret index="+idx.get());
            return clients[idx.get()];
        }
    }

    @Override
    public boolean hasCache() {
        return mHasCache;
    }

    @Override
    public long getCacheAge() {
        if (mHasCache) {
            return SystemClock.elapsedRealtime() - mCachedNtpElapsedRealtime;
        } else {
            return Long.MAX_VALUE;
        }
    }

    @Override
    public long getCacheCertainty() {
        if (mHasCache) {
            return mCachedNtpCertainty;
        } else {
            return Long.MAX_VALUE;
        }
    }

    @Override
    public long currentTimeMillis() {
        if (!mHasCache) {
            throw new IllegalStateException("Missing authoritative time source");
        }
        if (LOGD) Log.d(TAG, "currentTimeMillis() cache hit");

        // current time is age after the last ntp cache; callers who
        // want fresh values will hit makeAuthoritative() first.
        return mCachedNtpTime + getCacheAge();
    }

    public long getCachedNtpTime() {
        if (LOGD) Log.d(TAG, "getCachedNtpTime() cache hit");
        return mCachedNtpTime;
    }

    public long getCachedNtpTimeReference() {
        return mCachedNtpElapsedRealtime;
    }
}
