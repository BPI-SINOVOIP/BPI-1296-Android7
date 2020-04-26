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
package com.android.cts.deviceowner;

import android.app.admin.SecurityLog.SecurityEvent;
import android.os.Parcel;
import android.os.SystemClock;

import java.util.Arrays;
import java.util.List;
import java.util.concurrent.TimeUnit;

public class SecurityLoggingTest extends BaseDeviceOwnerTest {

    private static final String MESSAGE_ONLY_ONE_MANAGED_USER_ALLOWED =
            "There should only be one user, managed by Device Owner";

    /**
     * Test: setting security logging can only be done if there's one user on the device.
     */
    public void testSetSecurityLoggingEnabledNotPossibleIfMoreThanOneUserPresent() {
        try {
            mDevicePolicyManager.setSecurityLoggingEnabled(getWho(), true);
            fail("did not throw expected SecurityException");
        } catch (SecurityException e) {
            assertEquals(e.getMessage(), MESSAGE_ONLY_ONE_MANAGED_USER_ALLOWED);
        }
    }

    /**
     * Test: retrieving security logs can only be done if there's one user on the device.
     */
    public void testRetrievingSecurityLogsNotPossibleIfMoreThanOneUserPresent() {
        try {
            mDevicePolicyManager.retrieveSecurityLogs(getWho());
            fail("did not throw expected SecurityException");
        } catch (SecurityException e) {
            assertEquals(e.getMessage(), MESSAGE_ONLY_ONE_MANAGED_USER_ALLOWED);
        }
    }

    /**
     * Test: retrieving previous security logs can only be done if there's one user on the device.
     */
    public void testRetrievingPreviousSecurityLogsNotPossibleIfMoreThanOneUserPresent() {
        try {
            mDevicePolicyManager.retrievePreRebootSecurityLogs(getWho());
            fail("did not throw expected SecurityException");
        } catch (SecurityException e) {
            assertEquals(e.getMessage(), MESSAGE_ONLY_ONE_MANAGED_USER_ALLOWED);
        }
    }

    /**
     * Test: retrieving security logs. This test has should be called when security logging is
     * enabled and directly after reboot with device owner installed so that security logging
     * actually takes place and fetching the logs isn't subject to rate limiting.
     */
    public void testGetSecurityLogs() {
        List<SecurityEvent> events = mDevicePolicyManager.retrieveSecurityLogs(getWho());

        // There must be at least some events, e.g. PackageManager logs all process launches.
        assertTrue("Unable to get events", events != null && events.size() > 0);

        // We don't know much about the events, so just call public API methods and do a simple
        // sanity check of timestamps.

        // Check that timestamps are between system start and current time.
        long systemStartedNanos = TimeUnit.MILLISECONDS.toNanos(System.currentTimeMillis())
                - SystemClock.elapsedRealtimeNanos();
        long nowNanos = TimeUnit.MILLISECONDS.toNanos(System.currentTimeMillis() + 1);

        for (int i = 0; i < events.size(); i++) {
            SecurityEvent event = events.get(i);
            long currentTimestampNanos = event.getTimeNanos();
            assertTrue("Logged event predates boot", currentTimestampNanos >= systemStartedNanos);
            assertTrue("Last logged event is in future", currentTimestampNanos <= nowNanos);

            // Test parcelling: flatten to a parcel.
            Parcel p = Parcel.obtain();
            event.writeToParcel(p, 0);
            p.setDataPosition(0);

            // Restore from parcel and check contents.
            SecurityEvent restored = SecurityEvent.CREATOR.createFromParcel(p);
            p.recycle();

            // For some events data is encapsulated into Object array.
            if (event.getData() instanceof Object[]) {
                assertTrue("Parcelling changed the array returned by getData",
                        Arrays.equals((Object[]) event.getData(), (Object[]) restored.getData()));
            } else {
                assertEquals("Parcelling changed the result of getData",
                        event.getData(), restored.getData());
            }
            assertEquals("Parcelling changed the result of getTag",
                    event.getTag(), restored.getTag());
            assertEquals("Parcelling changed the result of getTimeNanos",
                    event.getTimeNanos(), restored.getTimeNanos());
            assertEquals("Parcelling changed the result of describeContents",
                    event.describeContents(), restored.describeContents());
        }
    }

    /**
     * Test: Test enabling security logging. This test should be executed after installing a device
     * owner so that we check that logging is not enabled by default. This test has a side effect:
     * security logging is enabled after its execution.
     */
    public void testEnablingSecurityLogging() {
        assertFalse(mDevicePolicyManager.isSecurityLoggingEnabled(getWho()));
        mDevicePolicyManager.setSecurityLoggingEnabled(getWho(), true);
        assertTrue(mDevicePolicyManager.isSecurityLoggingEnabled(getWho()));
    }

    /**
     * Test: Test disabling security logging. This test has a side effect: security logging is
     * disabled after its execution.
     */
    public void testDisablingSecurityLogging() {
        mDevicePolicyManager.setSecurityLoggingEnabled(getWho(), false);
        assertFalse(mDevicePolicyManager.isSecurityLoggingEnabled(getWho()));
    }

    /**
     * Test: retrieving security logs should be rate limited - subsequent attempts should return
     * null.
     */
    public void testRetrievingSecurityLogsNotPossibleImmediatelyAfterPreviousSuccessfulRetrieval() {
        List<SecurityEvent> logs = mDevicePolicyManager.retrieveSecurityLogs(getWho());
        // if logs is null it means that that attempt was rate limited => test PASS
        if (logs != null) {
            assertNull(mDevicePolicyManager.retrieveSecurityLogs(getWho()));
            assertNull(mDevicePolicyManager.retrieveSecurityLogs(getWho()));
        }
    }
}
