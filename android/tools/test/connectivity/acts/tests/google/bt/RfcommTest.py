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
Test script to execute Bluetooth basic functionality test cases.
This test was designed to be run in a shield box.
"""

import threading
import time

from queue import Empty
from acts.test_utils.bt.BluetoothBaseTest import BluetoothBaseTest
from acts.test_utils.bt.bt_test_utils import clear_bonded_devices
from acts.test_utils.bt.bt_test_utils import kill_bluetooth_process
from acts.test_utils.bt.bt_test_utils import orchestrate_rfcomm_connection
from acts.test_utils.bt.bt_test_utils import reset_bluetooth
from acts.test_utils.bt.bt_test_utils import setup_multiple_devices_for_bt_test
from acts.test_utils.bt.bt_test_utils import take_btsnoop_logs
from acts.test_utils.bt.bt_test_utils import write_read_verify_data
from acts.test_utils.bt.bt_test_utils import verify_server_and_client_connected

from acts.test_utils.bt.BtEnum import RfcommUuid


class RfcommTest(BluetoothBaseTest):
    default_timeout = 10
    rf_client_th = 0
    scan_discovery_time = 5
    message = (
        "Space: the final frontier. These are the voyages of "
        "the starship Enterprise. Its continuing mission: to explore "
        "strange new worlds, to seek out new life and new civilizations,"
        " to boldly go where no man has gone before.")

    def __init__(self, controllers):
        BluetoothBaseTest.__init__(self, controllers)
        self.client_ad = self.android_devices[0]
        self.server_ad = self.android_devices[1]

    def setup_class(self):
        return setup_multiple_devices_for_bt_test(self.android_devices)

    def setup_test(self):
        for a in self.android_devices:
            if not clear_bonded_devices(a):
                return False
        for a in self.android_devices:
            a.ed.clear_all_events()
        return True

    def teardown_test(self):
        self.client_ad.droid.bluetoothRfcommCloseClientSocket()
        self.server_ad.droid.bluetoothRfcommCloseServerSocket()
        return True

    def on_fail(self, test_name, begin_time):
        take_btsnoop_logs(self.android_devices, self, test_name)
        reset_bluetooth(self.android_devices)

    def teardown_test(self):
        if verify_server_and_client_connected(self.client_ad, self.server_ad):
            self.client_ad.droid.bluetoothRfcommStop()
            self.server_ad.droid.bluetoothRfcommStop()

    @BluetoothBaseTest.bt_test_wrap
    def test_rfcomm_connection(self):
        """Test bluetooth RFCOMM connection

        Test RFCOMM though establishing a basic connection.

        Steps:
        1. Get the mac address of the server device.
        2. Establish an RFCOMM connection from the client to the server AD.
        3. Verify that the RFCOMM connection is active from both the client and
        server.

        Expected Result:
        RFCOMM connection is established then disconnected succcessfully.

        Returns:
          Pass if True
          Fail if False

        TAGS: Classic, RFCOMM
        Priority: 1
        """
        if not orchestrate_rfcomm_connection(self.client_ad, self.server_ad):
            return False

        self.client_ad.droid.bluetoothRfcommStop()
        self.server_ad.droid.bluetoothRfcommStop()
        return True

    @BluetoothBaseTest.bt_test_wrap
    def test_rfcomm_connection_write_ascii(self):
        """Test bluetooth RFCOMM writing and reading ascii data

        Test RFCOMM though establishing a connection.

        Steps:
        1. Get the mac address of the server device.
        2. Establish an RFCOMM connection from the client to the server AD.
        3. Verify that the RFCOMM connection is active from both the client and
        server.
        4. Write data from the client and read received data from the server.
        5. Verify data matches from client and server
        6. Disconnect the RFCOMM connection.

        Expected Result:
        RFCOMM connection is established then disconnected succcessfully.

        Returns:
          Pass if True
          Fail if False

        TAGS: Classic, RFCOMM
        Priority: 1
        """
        if not orchestrate_rfcomm_connection(self.client_ad, self.server_ad):
            return False
        if not write_read_verify_data(self.client_ad, self.server_ad,
                                      self.message, False):
            return False
        if not verify_server_and_client_connected(self.client_ad,
                                                  self.server_ad):
            return False

        self.client_ad.droid.bluetoothRfcommStop()
        self.server_ad.droid.bluetoothRfcommStop()
        return True

    @BluetoothBaseTest.bt_test_wrap
    def test_rfcomm_write_binary(self):
        """Test bluetooth RFCOMM writing and reading binary data

        Test profile though establishing an RFCOMM connection.

        Steps:
        1. Get the mac address of the server device.
        2. Establish an RFCOMM connection from the client to the server AD.
        3. Verify that the RFCOMM connection is active from both the client and
        server.
        4. Write data from the client and read received data from the server.
        5. Verify data matches from client and server
        6. Disconnect the RFCOMM connection.

        Expected Result:
        RFCOMM connection is established then disconnected succcessfully.

        Returns:
          Pass if True
          Fail if False

        TAGS: Classic, RFCOMM
        Priority: 1
        """
        if not orchestrate_rfcomm_connection(self.client_ad, self.server_ad):
            return False
        binary_message = "11010101"
        if not write_read_verify_data(self.client_ad, self.server_ad,
                                      binary_message, True):
            return False

        if not verify_server_and_client_connected(self.client_ad,
                                                  self.server_ad):
            return False

        self.client_ad.droid.bluetoothRfcommStop()
        self.server_ad.droid.bluetoothRfcommStop()
        return True

    @BluetoothBaseTest.bt_test_wrap
    def test_rfcomm_accept_timeout(self):
        """Test bluetooth RFCOMM accept socket timeout

        Verify that RFCOMM connections are unsuccessful if
        the socket timeout is exceeded.

        Steps:
        1. Get the mac address of the server device.
        2. Establish an RFCOMM connection from the client to the server AD.
        3. Verify that the RFCOMM connection is active from both the client and
        server.

        Expected Result:
        RFCOMM connection is established then disconnected succcessfully.

        Returns:
          Pass if True
          Fail if False

        TAGS: Classic, RFCOMM
        Priority: 1
        """
        # Socket timeout set to 999ms
        short_socket_timeout = 999
        # Wait time in seconds before attempting a connection
        wait_time_before_connect_attempt = 1
        self.server_ad.droid.bluetoothStartPairingHelper()
        self.client_ad.droid.bluetoothStartPairingHelper()
        self.server_ad.droid.bluetoothRfcommBeginAcceptThread(
            RfcommUuid.DEFAULT_UUID.value, short_socket_timeout)
        time.sleep(wait_time_before_connect_attempt)

        # Try to connect
        self.client_ad.droid.bluetoothRfcommBeginConnectThread(
            self.server_ad.droid.bluetoothGetLocalAddress())
        # Give the connection time to fail
        #time.sleep(self.default_timeout)
        time.sleep(2)
        if verify_server_and_client_connected(self.client_ad, self.server_ad):
            return False
        self.log.info("No active connections found as expected")
        # AcceptThread has finished, kill hanging ConnectThread
        self.client_ad.droid.bluetoothRfcommKillConnThread()
        reset_bluetooth(self.android_devices)
        return True
