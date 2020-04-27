/*
 * Copyright (c) 2015 The Android Open Source Project
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

package com.realtek.recommendation;

import android.app.UiModeManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Handler;
import android.util.Log;

import static android.content.Context.UI_MODE_SERVICE;

/*
 * This class extends BroadcastReceiver and publishes Recommendations when received.
 */
public class RecommendationReceiver extends BroadcastReceiver {
    private static final long INITIAL_DELAY = 3000;
    private static final String TAG = "RecommendationReceiver";
    private Handler mHandler = new Handler();

    @Override
    public void onReceive(Context context, Intent intent) {
        if (intent.getAction().equals(Intent.ACTION_BOOT_COMPLETED)) {
            UiModeManager uiModeManager = (UiModeManager) context.getSystemService(UI_MODE_SERVICE);
            if (uiModeManager.getCurrentModeType() == Configuration.UI_MODE_TYPE_TELEVISION) {
                Log.i(TAG, "Running on a TV Device, start recommendation update");
                mHandler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        Intent recommendationIntent = new Intent(context, UpdateRecommendationsService.class);
                        context.startService(recommendationIntent);
                    }
                }, INITIAL_DELAY);
            } else {
                Log.i(TAG, "Running on a non-TV Device");
            }
        }
    }
}
