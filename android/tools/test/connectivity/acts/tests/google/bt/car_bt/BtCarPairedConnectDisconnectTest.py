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
Test script to test connect and disconnect sequence between two devices which can run
SL4A. The script does the following:
  Setup:
    Clear up the bonded devices on both bluetooth adapters and bond the DUTs to each other.
  Test (NUM_TEST_RUNS times):
    1. Connect A2dpSink and HeadsetClient
      1.1. Check that devices are connected.
    2. Disconnect A2dpSink and HeadsetClient
      2.1 Check that devices are disconnected.
"""

import time

from acts.base_test import BaseTestClass
from acts.test_utils.bt import bt_test_utils
from acts.test_utils.bt import BtEnum
from acts import asserts

class BtCarPairedConnectDisconnectTest(BaseTestClass):
    def setup_class(self):
        self.car = self.android_devices[0]
        self.ph = self.android_devices[1]
        self.car_bt_addr = self.car.droid.bluetoothGetLocalAddress()
        self.ph_bt_addr = self.ph.droid.bluetoothGetLocalAddress()

    def setup_test(self):
        # Reset the devices in a clean state.
        bt_test_utils.setup_multiple_devices_for_bt_test(self.android_devices)
        bt_test_utils.reset_bluetooth(self.android_devices)
        for a in self.android_devices:
            a.ed.clear_all_events()

        # Pair the devices.
        # This call may block until some specified timeout in bt_test_utils.py.
        result = bt_test_utils.pair_pri_to_sec(self.car.droid, self.ph.droid)

        asserts.assert_true(result, "pair_pri_to_sec returned false.");

        # Check for successful setup of test.
        devices = self.car.droid.bluetoothGetBondedDevices()
        asserts.assert_equal(len(devices), 1, "pair_pri_to_sec succeeded but no bonded devices.")

    def on_fail(self, test_name, begin_time):
        bt_test_utils.take_btsnoop_logs(self.android_devices, self, test_name)

    def test_connect_disconnect_paired(self):
        """
        Tests if we can connect two devices over Headset, A2dp and then disconnect them with success

        Precondition:
        1. Devices are paired.

        Steps:
        1. Initiate connection over A2dp Sink and Headset client profiles.
        2. Check if the connection succeeded.

        Returns:
          Pass if True
          Fail if False

        Priority: 0
        """

        NUM_TEST_RUNS = 2
        failure = 0
        for i in range(NUM_TEST_RUNS):
            self.log.info("Running test [" + str(i) + "/" + str(NUM_TEST_RUNS) + "]")
            success = bt_test_utils.connect_pri_to_sec(
                self.log, self.car, self.ph.droid,
                set([BtEnum.BluetoothProfile.HEADSET_CLIENT.value,
                     BtEnum.BluetoothProfile.A2DP_SINK.value]))

            # Check if we got connected.
            if not success:
                self.log.info("Not all profiles connected.")
                failure = failure + 1
                continue

            # Disconnect the devices.
            self.log.info("Attempt to disconnect.")
            self.car.droid.bluetoothDisconnectConnected(self.ph_bt_addr)

            end_time = time.time() + 10
            disconnected = False
            # Busy loop to check if we have successfully disconnected from the
            # device
            while time.time() < end_time:
                connectedDevices = self.car.droid.bluetoothGetConnectedDevices()
                exists = False
                connected_devices = \
                    self.car.droid.bluetoothGetConnectedDevices()
                for d in connected_devices:
                  if d['address'] == self.ph_bt_addr:
                      exists = True
                      break
                if exists is False:
                    disconnected = True
                    break
                time.sleep(1)

            if disconnected is False:
                self.log.info("Still connected devices.")
                failure = failure + 1
                continue
        self.log.info("Failure {} total tests {}".format(failure, NUM_TEST_RUNS))
        asserts.assert_equal(failure, 0, "")

