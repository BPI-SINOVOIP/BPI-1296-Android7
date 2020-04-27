/*
 * Copyright (C) 2016 The Android Open Source Project
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

package com.android.functional.otatests;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import android.app.Instrumentation;
import android.content.Intent;
import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;
import android.support.test.uiautomator.By;
import android.support.test.uiautomator.UiDevice;
import android.support.test.uiautomator.Until;
import org.junit.After;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;

/**
 * Tests functionality related to the System Update app.
 *
 * Precondition: an OTA must be available to the device.
 */
@RunWith(AndroidJUnit4.class)
public class SystemUpdateAppTest {

    private static final String SYSTEM_UPDATE_INTENT = "android.settings.SYSTEM_UPDATE_SETTINGS";
    private static final String POLICY_AUTH_PACKAGE = "com.google.android.gms.policy_auth";
    private static final long TIMEOUT_MS = 2000;

    private UiDevice mDevice;
    private Instrumentation mInstrumentation;

    @BeforeClass
    public void setUp() throws Exception {
        mInstrumentation = InstrumentationRegistry.getInstrumentation();
        mDevice = UiDevice.getInstance(mInstrumentation);
    }

    @After
    public void tearDown() throws Exception {
        mDevice.pressHome();
    }

    @Test
    public void testUpdateIsAvailable() {
        mInstrumentation.getContext().startActivity(new Intent(SYSTEM_UPDATE_INTENT));
        String mainText = mDevice.wait(Until.findObject(
                By.res(POLICY_AUTH_PACKAGE, "suw_layout_title")), TIMEOUT_MS).getText();
        assertTrue(mainText.contains("available"));
        assertNotNull(mDevice.findObject(By.res(POLICY_AUTH_PACKAGE, "suw_items_title")));
    }
}
