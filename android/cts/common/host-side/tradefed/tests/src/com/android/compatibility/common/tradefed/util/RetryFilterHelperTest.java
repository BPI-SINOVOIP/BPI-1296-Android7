/*
 * Copyright (C) 2017 The Android Open Source Project
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
package com.android.compatibility.common.tradefed.util;

import com.android.compatibility.common.tradefed.testtype.CompatibilityTest;
import com.android.tradefed.config.OptionSetter;

import junit.framework.TestCase;

/**
 * Unit tests for {@link RetryFilterHelper}
 */
public class RetryFilterHelperTest extends TestCase {

    private static final String TEST_STRING = "abcd";
    private static final RetryType TEST_RETRY_TYPE = RetryType.FAILED;

    public void testSetAllOptionsFrom() throws Exception {
        RetryFilterHelper helper = new RetryFilterHelper(null, 0);
        RetryFilterHelper otherObj = new RetryFilterHelper(null, 0);
        OptionSetter otherObjSetter = new OptionSetter(otherObj);
        otherObjSetter.setOptionValue(CompatibilityTest.SUBPLAN_OPTION, TEST_STRING);
        helper.setAllOptionsFrom(otherObj);
        assertEquals(TEST_STRING, helper.mSubPlan);
    }

    public void testClearOptions() throws Exception {
        RetryFilterHelper helper = new RetryFilterHelper(null, 0);
        OptionSetter setter = new OptionSetter(helper);
        setter.setOptionValue(CompatibilityTest.SUBPLAN_OPTION, TEST_STRING);
        setter.setOptionValue(CompatibilityTest.INCLUDE_FILTER_OPTION, TEST_STRING);
        setter.setOptionValue(CompatibilityTest.EXCLUDE_FILTER_OPTION, TEST_STRING);
        setter.setOptionValue(CompatibilityTest.ABI_OPTION, TEST_STRING);
        setter.setOptionValue(CompatibilityTest.MODULE_OPTION, TEST_STRING);
        setter.setOptionValue(CompatibilityTest.TEST_OPTION, TEST_STRING);
        setter.setOptionValue(CompatibilityTest.TEST_OPTION, TEST_RETRY_TYPE.name());
        helper.clearOptions();
        assertTrue(helper.mSubPlan == null);
        assertTrue(helper.mIncludeFilters.isEmpty());
        assertTrue(helper.mExcludeFilters.isEmpty());
        assertTrue(helper.mAbiName == null);
        assertTrue(helper.mModuleName == null);
        assertTrue(helper.mTestName == null);
        assertTrue(helper.mRetryType == null);
    }

}
