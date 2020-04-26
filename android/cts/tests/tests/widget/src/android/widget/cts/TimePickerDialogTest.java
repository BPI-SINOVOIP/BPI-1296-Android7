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

package android.widget.cts;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.DatePickerDialog;
import android.app.TimePickerDialog;
import android.test.ActivityInstrumentationTestCase2;
import android.test.UiThreadTest;
import android.widget.TimePicker;

/**
 * Test {@link TimePickerDialog}.
 */
public class TimePickerDialogTest extends
        ActivityInstrumentationTestCase2<TimePickerDialogCtsActivity> {

    private Activity mActivity;

    public TimePickerDialogTest() {
        super(TimePickerDialogCtsActivity.class);
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        mActivity = getActivity();
    }

    @UiThreadTest
    public void testConstructor() {
        new TimePickerDialog(mActivity, null, 7, 0, true);

        new TimePickerDialog(mActivity, AlertDialog.THEME_TRADITIONAL, null, 7, 0, true);

        // Ensure the picker is shown using the Holo-style layout.
        TimePickerDialog holoDialog = new TimePickerDialog(mActivity, AlertDialog.THEME_HOLO_DARK,
                null, 7, 0, true);
        assertEquals(TimePicker.MODE_SPINNER, holoDialog.getTimePicker().getMode());

        // Ensure the picker is shown using the Material-style layout where available.
        TimePickerDialog holoClockDialog = new TimePickerDialog(mActivity,
                R.style.Theme_Holo_With_Material_Pickers, null, 7, 0, true);
        final int expectedMode = mActivity.getResources().getInteger(R.integer.time_picker_mode);
        assertEquals(expectedMode, holoClockDialog.getTimePicker().getMode());

        new TimePickerDialog(mActivity,
                android.R.style.Theme_Material_Dialog_Alert, null, 7, 0, true);

        try {
            new TimePickerDialog(null, null, 7, 0, true);
            fail("should throw NullPointerException");
        } catch (Exception e) {
        }
    }

}
