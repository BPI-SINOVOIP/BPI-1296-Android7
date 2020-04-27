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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import android.os.Bundle;
import android.support.test.InstrumentationRegistry;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;
import org.junit.Before;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

@RunWith(Parameterized.class)
public class VersionCheckingTest {

    protected static final String OLD_VERSION = "/sdcard/otatest/version.old";
    protected static final String NEW_VERSION = "/sdcard/otatest/version.new";
    protected static final String KEY_BUILD_ID = "ro.build.version.incremental";
    protected static final String KEY_BOOTLOADER = "ro.bootloader";
    protected static final String KEY_BASEBAND = "ro.build.expect.baseband";
    protected static final String KEY_BASEBAND_GSM = "gsm.version.baseband";
    protected static final String PATH_NAME = "path_name";

    protected VersionInfo mOldVersion;
    protected VersionInfo mNewVersion;

    protected String mTestPath;

    public VersionCheckingTest(String testPath) {
        mTestPath = testPath;
    }

    @Before
    public void setUp() throws Exception {
        try {
            mOldVersion = VersionInfo.parseFromFile(OLD_VERSION);
            mNewVersion = VersionInfo.parseFromFile(NEW_VERSION);
        } catch (IOException e) {
            throw new RuntimeException(
                    "Couldn't find version file; was this test run with VersionCachePreparer?", e);
        }
    }

    @Parameters(name = "{0}")
    public static Iterable<? extends Object> getOtaPathName() {
        Bundle args = InstrumentationRegistry.getArguments();
        if (args.containsKey(PATH_NAME)) {
            return Arrays.asList(args.getString(PATH_NAME));
        }
        return Arrays.asList("unnamed path");
    }

    protected void assertNotUpdated() throws IOException {
        assertEquals(mOldVersion.getBuildId(), getProp(KEY_BUILD_ID));
        assertEquals(mOldVersion.getBootloaderVersion(), getProp(KEY_BOOTLOADER));
        assertTrue(mOldVersion.getBasebandVersion().equals(getProp(KEY_BASEBAND))
                || mOldVersion.getBasebandVersion().equals(getProp(KEY_BASEBAND_GSM)));
    }

    protected void assertUpdated() throws IOException {
        assertEquals(mNewVersion.getBuildId(), getProp(KEY_BUILD_ID));
        assertEquals(mNewVersion.getBootloaderVersion(), getProp(KEY_BOOTLOADER));
        // Due to legacy property names (an old meaning to gsm.version.baseband),
        // the KEY_BASEBAND and KEY_BASEBAND_GSM properties may not match each other.
        // At least one of them will always match the baseband version recorded by
        // NEW_VERSION.
        assertTrue(mNewVersion.getBasebandVersion().equals(getProp(KEY_BASEBAND))
                || mNewVersion.getBasebandVersion().equals(getProp(KEY_BASEBAND_GSM)));
    }

    private String getProp(String key) throws IOException {
        Process p = Runtime.getRuntime().exec("getprop " + key);
        BufferedReader r = new BufferedReader(new InputStreamReader(p.getInputStream()));
        String ret = r.readLine().trim();
        r.close();
        return ret;
    }
}
