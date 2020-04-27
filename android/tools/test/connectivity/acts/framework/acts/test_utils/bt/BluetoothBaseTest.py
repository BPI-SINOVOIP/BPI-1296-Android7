#!/usr/bin/env python3.4
#
#   Copyright 2016 - Google
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
"""
    Base Class for Defining Common Bluetooth Test Functionality
"""

import os
import time
import traceback
from acts import utils
from acts.base_test import BaseTestClass
from acts.signals import TestSignal
from acts.utils import set_location_service
from acts.controllers import android_device
from acts.test_utils.bt.bt_test_utils import (
    reset_bluetooth, setup_multiple_devices_for_bt_test, take_btsnoop_logs)
from acts.utils import sync_device_time
import threading


class BluetoothBaseTest(BaseTestClass):
    DEFAULT_TIMEOUT = 10
    start_time = 0
    timer_list = []

    def __init__(self, controllers):
        BaseTestClass.__init__(self, controllers)

    # Use for logging in the test cases to facilitate
    # faster log lookup and reduce ambiguity in logging.
    @staticmethod
    def bt_test_wrap(fn):
        def _safe_wrap_test_case(self, *args, **kwargs):
            test_id = "{}:{}:{}".format(self.__class__.__name__, fn.__name__,
                                        time.time())
            log_string = "[Test ID] {}".format(test_id)
            self.log.info(log_string)
            try:
                for ad in self.android_devices:
                    ad.droid.logI("Started " + log_string)
                result = fn(self, *args, **kwargs)
                for ad in self.android_devices:
                    ad.droid.logI("Finished " + log_string)
                if result is not True and "bt_auto_rerun" in self.user_params:
                    self.teardown_test()
                    log_string = "[Rerun Test ID] {}. 1st run failed.".format(
                        test_id)
                    self.log.info(log_string)
                    self.setup_test()
                    for ad in self.android_devices:
                        ad.droid.logI("Rerun Started " + log_string)
                    result = fn(self, *args, **kwargs)
                    if result is True:
                        self.log.info("Rerun passed.")
                    elif result is False:
                        self.log.info("Rerun failed.")
                    else:
                        # In the event that we have a non-bool or null
                        # retval, we want to clearly distinguish this in the
                        # logs from an explicit failure, though the test will
                        # still be considered a failure for reporting purposes.
                        self.log.info("Rerun indeterminate.")
                        result = False
                return result
            except TestSignal:
                raise
            except Exception as e:
                self.log.error(traceback.format_exc())
                self.log.error(str(e))
                return False
            return fn(self, *args, **kwargs)

        return _safe_wrap_test_case

    def _reboot_device(self, ad):
        self.log.info("Rebooting device {}.".format(ad.serial))
        ad = ad.reboot()

    def setup_class(self):
        if "reboot_between_test_class" in self.user_params:
            threads = []
            for a in self.android_devices:
                thread = threading.Thread(
                    target=self._reboot_device, args=([a]))
                threads.append(thread)
                thread.start()
            for t in threads:
                t.join()
        for a in self.android_devices:
            set_location_service(a, False)
            sync_device_time(a)
        return setup_multiple_devices_for_bt_test(self.android_devices)

    def setup_test(self):
        self.timer_list = []
        for a in self.android_devices:
            a.ed.clear_all_events()
        return True

    def teardown_test(self):
        return True

    def on_fail(self, test_name, begin_time):
        self.log.debug(
            "Test {} failed. Gathering bugreport and btsnoop logs".format(
                test_name))
        take_btsnoop_logs(self.android_devices, self, test_name)
        self._take_bug_report(test_name, begin_time)
        for _ in range(5):
            if reset_bluetooth(self.android_devices):
                break
            else:
                self.log.error("Failed to reset Bluetooth... retrying.")
        return

    def _take_bug_report(self, test_name, begin_time):
        if "no_bug_report_on_fail" in self.user_params:
            return

        # magical sleep to ensure the runtime restart or reboot begins
        time.sleep(1)
        for ad in self.android_devices:
            try:
                ad.adb.wait_for_device()
                ad.take_bug_report(test_name, begin_time)
                tombstone_path = os.path.join(
                    ad.log_path, "BugReports",
                    "{},{}".format(begin_time, ad.serial).replace(' ', '_'))
                utils.create_dir(tombstone_path)
                ad.adb.pull('/data/tombstones/', tombstone_path)
            except:
                self.log.error("Failed to take a bug report for {}, {}"
                               .format(ad.serial, test_name))

    def _get_time_in_milliseconds(self):
        return int(round(time.time() * 1000))

    def start_timer(self):
        self.start_time = self._get_time_in_milliseconds()

    def end_timer(self):
        total_time = self._get_time_in_milliseconds() - self.start_time
        self.timer_list.append(total_time)
        self.start_time = 0
        return total_time

    def log_stats(self):
        if self.timer_list:
            self.log.info("Overall list {}".format(self.timer_list))
            self.log.info("Average of list {}".format(
                sum(self.timer_list) / float(len(self.timer_list))))
            self.log.info("Maximum of list {}".format(max(self.timer_list)))
            self.log.info("Minimum of list {}".format(min(self.timer_list)))
            self.log.info("Total items in list {}".format(len(
                self.timer_list)))
        self.timer_list = []
