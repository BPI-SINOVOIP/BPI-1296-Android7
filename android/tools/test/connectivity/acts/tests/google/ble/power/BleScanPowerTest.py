#/usr/bin/env python3.4
#
# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.
"""
This test script exercises power test scenarios for different scan modes.
This test script was designed with this setup in mind:
Shield box one: Android Device and Monsoon tool box
"""

import json
import os

from acts import asserts
from acts.controllers import monsoon
from acts.test_utils.bt.BluetoothBaseTest import BluetoothBaseTest
from acts.test_utils.bt.BleEnum import ScanSettingsScanMode
from acts.test_utils.bt.bt_test_utils import bluetooth_enabled_check
from acts.test_utils.bt.bt_test_utils import generate_ble_scan_objects
from acts.test_utils.tel.tel_test_utils import set_phone_screen_on
from acts.test_utils.wifi import wifi_test_utils as wutils
from acts.utils import create_dir
from acts.utils import force_airplane_mode
from acts.utils import get_current_human_time
from acts.utils import set_location_service
from acts.utils import set_adaptive_brightness
from acts.utils import set_ambient_display
from acts.utils import set_auto_rotate
from acts.utils import sync_device_time

# Monsoon output Voltage in V
MONSOON_OUTPUT_VOLTAGE = 4.2
# Monsoon output max current in A
MONSOON_MAX_CURRENT = 7.8

# Power mesaurement sampling rate in Hz
BLE_SCAN_POWER_SAMPLING_RATE = 20
# Power mesaurement sample duration in seconds
BLE_SCAN_POWER_SAMPLE_TIME = 60
# Power mesaurement start time in seconds
BLE_SCAN_START_TIME = 30
# BLE scanning time in seconds
BLE_SCAN_DURATION = 5
# BLE no scanning time in seconds
BLE_NOSCAN_DURATION = 5

start_pmc_cmd = ("am start -n com.android.pmc/com.android.pmc."
                 "PMCMainActivity")
pmc_base_cmd = ("am broadcast -a com.android.pmc.BLESCAN --es ScanMode ")


class BleScanPowerTest(BluetoothBaseTest):
    SCREEN_TIME_OFF = 10

    def setup_class(self):
        self.ad = self.android_devices[0]
        self.mon = self.monsoons[0]
        self.mon.set_voltage(MONSOON_OUTPUT_VOLTAGE)
        self.mon.set_max_current(MONSOON_MAX_CURRENT)
        # Monsoon phone
        self.mon.attach_device(self.ad)
        self.monsoon_log_path = os.path.join(self.log_path, "MonsoonLog")
        create_dir(self.monsoon_log_path)

        asserts.assert_true(
            self.mon.usb("auto"),
            "Failed to turn USB mode to auto on monsoon.")

        sync_device_time(self.ad)

        asserts.assert_true(
            force_airplane_mode(self.ad, True),
            "Can not turn on airplane mode on: %s" % self.ad.serial)
        asserts.assert_true(
            bluetooth_enabled_check(self.ad),
            "Failed to set Bluetooth state to enabled")
        set_location_service(self.ad, False)
        set_adaptive_brightness(self.ad, False)
        set_ambient_display(self.ad, False)
        self.ad.adb.shell("settings put system screen_brightness 0")
        set_auto_rotate(self.ad, False)
        set_phone_screen_on(self.log, self.ad, self.SCREEN_TIME_OFF)

        # Start pmc app.
        self.ad.adb.shell(start_pmc_cmd)
        self.ad.adb.shell("setprop log.tag.PMC VERBOSE")
        wutils.wifi_toggle_state(self.ad, False)

    def _save_logs_for_power_test(self, monsoon_result):
        """utility function to save power data into log file.

        "whether monsoon_log_for_power_test" needs to be set
        in config file in order to save power data into a file
        If bug_report_for_power_test is defined in test config file
        it will also save a bug report.

        Steps:
        1. Save power data into a file if being configed.
        2. Create a bug report if being configed

        Args:
            monsoon_result: power data object

        Returns:
            If success, return None.
            if fail, exception will be thrown
        """
        current_time = get_current_human_time()
        file_name = "%s_%s" % (self.current_test_name, current_time)
        monsoon_result.save_to_text_file(
            [monsoon_result], os.path.join(self.monsoon_log_path, file_name))

        self.ad.take_bug_report(self.current_test_name, current_time)

    def _test_power_for_scan(self, scan_mode):
        """utility function for power test with BLE scan.

        Steps:
        1. Prepare adb shell command
        2. Send the adb shell command to PMC
        3. PMC start first alarm to start scan
        4. After first alarm triggered it start the second alarm to stop scan
        5. Save the power usage data into log file

        Args:
            scan_mode: scan mode

        Returns:
            If success, return None.
            if fail, error will be logged.
        """

        msg = "%s%s --es StartTime %d --es ScanTime %d" % (
            pmc_base_cmd, scan_mode, BLE_SCAN_START_TIME,
            BLE_SCAN_POWER_SAMPLE_TIME)
        self.ad.log.info("Send broadcast message: %s", msg)
        self.ad.adb.shell(msg)
        # Start the power measurement
        result = self.mon.measure_power(
            BLE_SCAN_POWER_SAMPLING_RATE, BLE_SCAN_POWER_SAMPLE_TIME,
            self.current_test_name, BLE_SCAN_START_TIME)

        self._save_logs_for_power_test(result)

    @BluetoothBaseTest.bt_test_wrap
    def test_power_for_scan_w_low_latency(self):
        """Test power usage when scan with low latency.

        Tests power usage when the device scans with low latency mode
        for 60 seconds where there are no advertisements.

        Steps:
        1. Prepare adb shell command
        2. Send the adb shell command to PMC
        3. PMC start first alarm to start scan
        4. After first alarm triggered it start the second alarm to stop scan
        5. Save the power usage data into log file

        Expected Result:
        power consumption results

        TAGS: LE, Scanning, Power
        Priority: 3
        """
        self._test_power_for_scan(
            ScanSettingsScanMode.SCAN_MODE_LOW_LATENCY.value)

    @BluetoothBaseTest.bt_test_wrap
    def test_power_for_scan_w_balanced(self):
        """Test power usage when scan with balanced mode.

        Tests power usage when the device scans with balanced mode
        for 60 seconds where there are no advertisements.

        Steps:
        1. Prepare adb shell command
        2. Send the adb shell command to PMC
        3. PMC start first alarm to start scan
        4. After first alarm triggered it start the second alarm to stop scan
        5. Save the power usage data into log file

        Expected Result:
        power consumption results

        TAGS: LE, Scanning, Power
        Priority: 3
        """
        self._test_power_for_scan(
            ScanSettingsScanMode.SCAN_MODE_BALANCED.value)

    @BluetoothBaseTest.bt_test_wrap
    def test_power_for_scan_w_low_power(self):
        """Test power usage when scan with low power.

        Tests power usage when the device scans with low power mode
        for 60 seconds where there are no advertisements.

        Steps:
        1. Prepare adb shell command
        2. Send the adb shell command to PMC
        3. PMC start first alarm to start scan
        4. After first alarm triggered it start the second alarm to stop scan
        5. Save the power usage data into log file

        Expected Result:
        power consumption results

        TAGS: LE, Scanning, Power
        Priority: 3
        """
        self._test_power_for_scan(
            ScanSettingsScanMode.SCAN_MODE_LOW_POWER.value)

    @BluetoothBaseTest.bt_test_wrap
    def test_power_for_intervaled_scans_w_balanced(self):
        """Test power usage when 12 intervaled scans with balanced mode

        Tests power usage when the device perform 12 intervaled scans with
        balanced mode for 5 seconds each where there are no advertisements.

        Steps:
        1. Prepare adb shell command
        2. Send the adb shell command to PMC
        3. PMC start first alarm to start scan
        4. After first alarm triggered it starts the second alarm to stop scan
        5. After second alarm triggered it starts the third alarm to start scan
        6. Repeat the alarms until 12 scans are done

        Expected Result:
        power consumption results

        TAGS: LE, Scanning, Power
        Priority: 3
        """
        msg1 = "%s%s --es StartTime %d --es ScanTime %d" % (
            pmc_base_cmd, ScanSettingsScanMode.SCAN_MODE_BALANCED.value,
            BLE_SCAN_START_TIME, BLE_SCAN_DURATION)
        msg = "%s --es NoScanTime %d --es Repetitions %d" % (
            msg1, BLE_NOSCAN_DURATION, 12)

        self.ad.log.info("Send broadcast message: %s", msg)
        self.ad.adb.shell(msg)
        # Start the power measurement
        result = self.mon.measure_power(
            BLE_SCAN_POWER_SAMPLING_RATE,
            (BLE_SCAN_DURATION + BLE_NOSCAN_DURATION) * 12,
            self.current_test_name, BLE_SCAN_START_TIME)

        self._save_logs_for_power_test(result)
