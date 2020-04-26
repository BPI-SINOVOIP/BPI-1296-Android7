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
package android.host.retaildemo;

import com.android.compatibility.common.tradefed.build.CompatibilityBuildHelper;
import com.android.ddmlib.testrunner.RemoteAndroidTestRunner;
import com.android.ddmlib.testrunner.TestIdentifier;
import com.android.ddmlib.testrunner.TestResult;
import com.android.ddmlib.testrunner.TestResult.TestStatus;
import com.android.ddmlib.testrunner.TestRunResult;
import com.android.tradefed.build.IBuildInfo;
import com.android.tradefed.device.DeviceNotAvailableException;
import com.android.tradefed.log.LogUtil.CLog;
import com.android.tradefed.result.CollectingTestListener;
import com.android.tradefed.testtype.DeviceTestCase;
import com.android.tradefed.testtype.IBuildReceiver;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Map;

public class BaseTestCase extends DeviceTestCase implements IBuildReceiver {
    private static final String RETAIL_DEMO_TEST_PKG = "com.android.cts.retaildemo";
    private static final String RUNNER = "android.support.test.runner.AndroidJUnitRunner";

    private IBuildInfo mBuildInfo;
    private CompatibilityBuildHelper mBuildHelper;

    private ArrayList<Integer> mTestUsers;

    @Override
    public void setBuild(IBuildInfo buildInfo) {
        mBuildInfo = buildInfo;
        mBuildHelper = new CompatibilityBuildHelper(mBuildInfo);
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        assertNotNull(mBuildInfo); // ensure build has been set before test is run.
        mTestUsers = new ArrayList<>();
    }

    @Override
    protected void tearDown() throws Exception {
        for (int userId : mTestUsers) {
            getDevice().removeUser(userId);
        }
        super.tearDown();
    }

    protected int createDemoUser() throws DeviceNotAvailableException, IllegalStateException {
        final String command = "pm create-user --ephemeral --demo "
                + "TestUser_" + System.currentTimeMillis();
        CLog.d("Starting command: " + command);
        final String output = getDevice().executeShellCommand(command);
        CLog.d("Output for command " + command + ": " + output);

        if (output.startsWith("Success")) {
            try {
                int userId = Integer.parseInt(output.substring(output.lastIndexOf(" ")).trim());
                mTestUsers.add(userId);
                return userId;
            } catch (NumberFormatException e) {
                CLog.e("Failed to parse result: %s", output);
            }
        } else {
            CLog.e("Failed to create demo user: %s", output);
        }
        throw new IllegalStateException();
    }

    protected void installAppAsUser(String appFileName, int userId)
            throws FileNotFoundException, DeviceNotAvailableException {
        CLog.d("Installing app " + appFileName + " for user " + userId);
        File apkFile = new File(mBuildHelper.getTestsDir(), appFileName);
        final String result = getDevice().installPackageForUser(
                apkFile, true, true, userId, "-t");
        assertNull("Failed to install " + appFileName + " for user " + userId + ": " + result,
                result);
    }

    protected boolean runDeviceTestsAsUser(String testClassName, String testMethodName, int userId)
            throws Exception {
        if (testClassName != null && testClassName.startsWith(".")) {
            testClassName = RETAIL_DEMO_TEST_PKG + testClassName;
        }

        RemoteAndroidTestRunner testRunner = new RemoteAndroidTestRunner(
                RETAIL_DEMO_TEST_PKG, RUNNER, getDevice().getIDevice());
        if (testClassName != null && testMethodName != null) {
            testRunner.setMethodName(testClassName, testMethodName);
        } else if (testClassName != null) {
            testRunner.setClassName(testClassName);
        }

        CollectingTestListener listener = new CollectingTestListener();
        assertTrue(getDevice().runInstrumentationTestsAsUser(testRunner, userId, listener));

        TestRunResult runResult = listener.getCurrentRunResults();
        printTestResult(runResult);
        return !runResult.hasFailedTests() && runResult.getNumTestsInState(TestStatus.PASSED) > 0;
    }

    private void printTestResult(TestRunResult runResult) {
        for (Map.Entry<TestIdentifier, TestResult> testEntry :
                runResult.getTestResults().entrySet()) {
            TestResult testResult = testEntry.getValue();
            CLog.d("Test " + testEntry.getKey() + ": " + testResult.getStatus());
            if (testResult.getStatus() != TestStatus.PASSED) {
                CLog.d(testResult.getStackTrace());
            }
        }
    }
}
