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

import com.android.compatibility.common.tradefed.build.CompatibilityBuildHelper;
import com.android.compatibility.common.tradefed.result.SubPlanHelper;
import com.android.compatibility.common.tradefed.testtype.CompatibilityTest;
import com.android.compatibility.common.tradefed.testtype.ModuleRepo;
import com.android.compatibility.common.tradefed.testtype.ISubPlan;
import com.android.compatibility.common.util.IInvocationResult;
import com.android.compatibility.common.util.LightInvocationResult;
import com.android.compatibility.common.util.ResultHandler;
import com.android.compatibility.common.util.TestFilter;
import com.android.tradefed.config.ArgsOptionParser;
import com.android.tradefed.config.ConfigurationException;
import com.android.tradefed.config.Option;
import com.android.tradefed.config.Option.Importance;
import com.android.tradefed.config.OptionCopier;
import com.android.tradefed.config.OptionSetter;
import com.android.tradefed.device.DeviceNotAvailableException;
import com.android.tradefed.device.ITestDevice;
import com.android.tradefed.log.LogUtil.CLog;
import com.android.tradefed.util.ArrayUtil;

import java.io.FileNotFoundException;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * Helper for generating --include-filter and --exclude-filter values on compatibility retry.
 */
public class RetryFilterHelper {

    @Option(name = CompatibilityTest.SUBPLAN_OPTION,
            description = "the subplan to run",
            importance = Importance.IF_UNSET)
    protected String mSubPlan;

    @Option(name = CompatibilityTest.INCLUDE_FILTER_OPTION,
            description = "the include module filters to apply.",
            importance = Importance.ALWAYS)
    protected Set<String> mIncludeFilters = new HashSet<>();

    @Option(name = CompatibilityTest.EXCLUDE_FILTER_OPTION,
            description = "the exclude module filters to apply.",
            importance = Importance.ALWAYS)
    protected Set<String> mExcludeFilters = new HashSet<>();

    @Option(name = CompatibilityTest.ABI_OPTION,
            shortName = 'a',
            description = "the abi to test.",
            importance = Importance.IF_UNSET)
    protected String mAbiName = null;

    @Option(name = CompatibilityTest.MODULE_OPTION,
            shortName = 'm',
            description = "the test module to run.",
            importance = Importance.IF_UNSET)
    protected String mModuleName = null;

    @Option(name = CompatibilityTest.TEST_OPTION,
            shortName = CompatibilityTest.TEST_OPTION_SHORT_NAME,
            description = "the test run.",
            importance = Importance.IF_UNSET)
    protected String mTestName = null;

    @Option(name = CompatibilityTest.RETRY_TYPE_OPTION,
            description = "used with " + CompatibilityTest.RETRY_OPTION
            + ", retry tests of a certain status. Possible values include \"failed\", "
            + "\"not_executed\", and \"custom\".",
            importance = Importance.IF_UNSET)
    protected RetryType mRetryType = null;

    /* Instance variables handy for retreiving the result to be retried */
    private CompatibilityBuildHelper mBuild = null;
    private int mSessionId;

    /* Sets to be populated by retry logic and returned by getter methods */
    private Set<String> mRetryIncludes;
    private Set<String> mRetryExcludes;

    /**
     * Constructor for a {@link RetryFilterHelper}. Requires a CompatibilityBuildHelper for
     * retrieving previous sessions and the ID of the session to retry.
     */
    public RetryFilterHelper(CompatibilityBuildHelper build, int sessionId) {
        mBuild = build;
        mSessionId = sessionId;
    }

    /**
     * Throws an {@link IllegalArgumentException} if the device build fingerprint doesn't match
     * the fingerprint recorded in the previous session's result.
     */
    public void validateBuildFingerprint(ITestDevice device) throws DeviceNotAvailableException {
        String oldBuildFingerprint = new LightInvocationResult(getResult()).getBuildFingerprint();
        String currentBuildFingerprint = device.getProperty("ro.build.fingerprint");
        if (!oldBuildFingerprint.equals(currentBuildFingerprint)) {
            throw new IllegalArgumentException(String.format(
                    "Device build fingerprint must match %s to retry session %d",
                    oldBuildFingerprint, mSessionId));
        }
    }

    /**
     * Copy all applicable options from an input object to this instance of RetryFilterHelper.
     */
    public void setAllOptionsFrom(Object obj) {
        clearOptions(); // Remove existing options first
        OptionCopier.copyOptionsNoThrow(obj, this);
    }

    /**
     * Set a single option on this instance of RetryFilterHelper
     * @throws {@link ConfigurationException} if the option cannot be set.
     */
    public void setOption(String option, String value) throws ConfigurationException {
        OptionSetter setter = new OptionSetter(this);
        setter.setOptionValue(option, value);
    }

    /**
     * Clear all option values of this RetryFilterHelper.
     */
    public void clearOptions() {
        mSubPlan = null;
        mIncludeFilters = new HashSet<>();
        mExcludeFilters = new HashSet<>();
        mModuleName = null;
        mTestName = null;
        mRetryType = null;
        mAbiName = null;
    }

    /**
     * Using command-line arguments from the previous session's result, set the input object's
     * option values to the values applied in the previous session.
     */
    public void setCommandLineOptionsFor(Object obj) {
        // only need light version to retrieve command-line args
        IInvocationResult result = new LightInvocationResult(getResult());
        String retryCommandLineArgs = result.getCommandLineArgs();
        if (retryCommandLineArgs != null) {
            try {
                // parse the command-line string from the result file and set options
                ArgsOptionParser parser = new ArgsOptionParser(obj);
                parser.parse(OptionHelper.getValidCliArgs(retryCommandLineArgs, obj));
            } catch (ConfigurationException e) {
                throw new RuntimeException(e);
            }
        }
    }

    /**
     * Retrieve an instance of the result to retry using the instance variables referencing
     * the build and the desired session ID. While it is faster to load this result once and
     * store it as an instance variable, {@link IInvocationResult} objects are large, and
     * memory is of greater concern.
     */
    public IInvocationResult getResult() {
        IInvocationResult result = null;
        try {
            result = ResultHandler.findResult(mBuild.getResultsDir(), mSessionId);
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
        if (result == null) {
            throw new IllegalArgumentException(String.format(
                    "Could not find session with id %d", mSessionId));
        }
        return result;
    }

    /**
     * Populate mRetryIncludes and mRetryExcludes based on the options and the result set for
     * this instance of RetryFilterHelper.
     */
    public void populateRetryFilters() {
        mRetryIncludes = new HashSet<>(mIncludeFilters); // reset for each population
        mRetryExcludes = new HashSet<>(mExcludeFilters); // reset for each population
        if (RetryType.CUSTOM.equals(mRetryType)) {
            Set<String> customIncludes = new HashSet<>(mIncludeFilters);
            Set<String> customExcludes = new HashSet<>(mExcludeFilters);
            if (mSubPlan != null) {
                ISubPlan retrySubPlan = SubPlanHelper.getSubPlanByName(mBuild, mSubPlan);
                customIncludes.addAll(retrySubPlan.getIncludeFilters());
                customExcludes.addAll(retrySubPlan.getExcludeFilters());
            }
            // If includes were added, only use those includes. Also use excludes added directly
            // or by subplan. Otherwise, default to normal retry.
            if (!customIncludes.isEmpty()) {
                mRetryIncludes.clear();
                mRetryIncludes.addAll(customIncludes);
                mRetryExcludes.addAll(customExcludes);
                return;
            }
        }
        // remove any extra filtering options
        // TODO(aaronholden) remove non-plan includes (e.g. those in cts-vendor-interface)
        // TODO(aaronholden) remove non-known-failure excludes
        mModuleName = null;
        mTestName = null;
        mSubPlan = null;
        populateFiltersBySubPlan();
        populatePreviousSessionFilters();
    }

    /* Generation of filters based on previous sessions is implemented thoroughly in SubPlanHelper,
     * and retry filter generation is just a subset of the use cases for the subplan retry logic.
     * Use retry type to determine which result types SubPlanHelper targets. */
    private void populateFiltersBySubPlan() {
        SubPlanHelper retryPlanCreator = new SubPlanHelper();
        retryPlanCreator.setResult(getResult());
        if (RetryType.FAILED.equals(mRetryType)) {
            // retry only failed tests
            retryPlanCreator.addResultType(SubPlanHelper.FAILED);
        } else if (RetryType.NOT_EXECUTED.equals(mRetryType)){
            // retry only not executed tests
            retryPlanCreator.addResultType(SubPlanHelper.NOT_EXECUTED);
        } else {
            // retry both failed and not executed tests
            retryPlanCreator.addResultType(SubPlanHelper.FAILED);
            retryPlanCreator.addResultType(SubPlanHelper.NOT_EXECUTED);
        }
        try {
            ISubPlan retryPlan = retryPlanCreator.createSubPlan(mBuild);
            mRetryIncludes.addAll(retryPlan.getIncludeFilters());
            mRetryExcludes.addAll(retryPlan.getExcludeFilters());
        } catch (ConfigurationException e) {
            throw new RuntimeException ("Failed to create subplan for retry", e);
        }
    }

    /* Retrieves the options set via command-line on the previous session, and generates/adds
     * filters accordingly */
    private void populatePreviousSessionFilters() {
        // Temporarily store options from this instance in another instance
        RetryFilterHelper tmpHelper = new RetryFilterHelper(mBuild, mSessionId);
        tmpHelper.setAllOptionsFrom(this);
        // Copy command-line args from previous session to this RetryFilterHelper's options
        setCommandLineOptionsFor(this);

        mRetryIncludes.addAll(mIncludeFilters);
        mRetryExcludes.addAll(mExcludeFilters);
        if (mSubPlan != null) {
            ISubPlan retrySubPlan = SubPlanHelper.getSubPlanByName(mBuild, mSubPlan);
            mRetryIncludes.addAll(retrySubPlan.getIncludeFilters());
            mRetryExcludes.addAll(retrySubPlan.getExcludeFilters());
        }
        if (mModuleName != null) {
            try {
                List<String> modules = ModuleRepo.getModuleNamesMatching(
                        mBuild.getTestsDir(), mModuleName);
                if (modules.size() == 0) {
                    throw new IllegalArgumentException(
                            String.format("No modules found matching %s", mModuleName));
                } else if (modules.size() > 1) {
                    throw new IllegalArgumentException(String.format(
                            "Multiple modules found matching %s:\n%s\nWhich one did you mean?\n",
                            mModuleName, ArrayUtil.join("\n", modules)));
                } else {
                    String module = modules.get(0);
                    cleanFilters(mRetryIncludes, module);
                    cleanFilters(mRetryExcludes, module);
                    mRetryIncludes.add(new TestFilter(mAbiName, module, mTestName).toString());
                }
            } catch (FileNotFoundException e) {
                throw new RuntimeException(e);
            }
        } else if (mTestName != null) {
            throw new IllegalArgumentException(
                "Test name given without module name. Add --module <module-name>");
        }

        // Copy options for current session back to this instance
        setAllOptionsFrom(tmpHelper);
    }

    /* Helper method designed to remove filters in a list not applicable to the given module */
    private static void cleanFilters(Set<String> filters, String module) {
        Set<String> cleanedFilters = new HashSet<String>();
        for (String filter : filters) {
            if (module.equals(TestFilter.createFrom(filter).getName())) {
                cleanedFilters.add(filter); // Module name matches, filter passes
            }
        }
        filters.clear();
        filters.addAll(cleanedFilters);
    }

    /** Retrieve include filters to be applied on retry */
    public Set<String> getIncludeFilters() {
        return new HashSet<>(mRetryIncludes);
    }

    /** Retrieve exclude filters to be applied on retry */
    public Set<String> getExcludeFilters() {
        return new HashSet<>(mRetryExcludes);
    }

    /** Clears retry filters and internal storage of options, except buildInfo and session ID */
    public void tearDown() {
        clearOptions();
        mRetryIncludes = null;
        mRetryExcludes = null;
        // keep references to buildInfo and session ID
    }
}
