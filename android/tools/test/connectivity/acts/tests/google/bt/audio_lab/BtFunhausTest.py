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
Test script to automate the Bluetooth Audio Funhaus.
"""
from acts.keys import Config
from acts.test_utils.bt.BluetoothBaseTest import BluetoothBaseTest
from acts.test_utils.bt.bt_test_utils import bluetooth_enabled_check
from acts.utils import bypass_setup_wizard
from acts.utils import create_dir
from acts.utils import exe_cmd
from acts.utils import sync_device_time
from queue import Empty
import json
import time
import os


class BtFunhausTest(BluetoothBaseTest):
    music_file_to_play = ""
    device_fails_to_connect_list = []

    def __init__(self, controllers):
        BluetoothBaseTest.__init__(self, controllers)

    def on_fail(self, test_name, begin_time):
        self._collect_bluetooth_manager_dumpsys_logs(self.android_devices)
        super(BluetoothBaseTest, self).on_fail(test_name, begin_time)

    def setup_class(self):
        for ad in self.android_devices:
            sync_device_time(ad)
            # Disable Bluetooth HCI Snoop Logs for audio tests
            ad.droid.bluetoothConfigHciSnoopLog(False)
            if not bypass_setup_wizard(ad):
                self.log.debug(
                    "Failed to bypass setup wizard, continuing test.")
        if not "bt_config" in self.user_params.keys():
            self.log.error("Missing mandatory user config \"bt_config\"!")
            return False
        bt_config_map_file = self.user_params["bt_config"]
        return self._setup_bt_config(bt_config_map_file)

    def _setup_bt_config(self, bt_config_map_file):
        bt_config_map = {}
        bt_conf_path = "/data/misc/bluedroid/bt_config.conf"
        if not os.path.isfile(bt_config_map_file):
            bt_config_map_file = os.path.join(
                self.user_params[Config.key_config_path], bt_config_map_file)
            if not os.path.isfile(bt_config_map_file):
                self.log.error("Unable to load bt_config file {}.".format(
                    bt_config_map_file))
                return False
        try:
            f = open(bt_config_map_file, 'r')
            bt_config_map = json.load(f)
            f.close()
        except FileNotFoundError:
            self.log.error("File not found: {}.".format(bt_config_map_file))
            return False
        # Connected devices return all upper case mac addresses.
        # Make the peripheral_info address attribute upper case
        # in order to ensure the BT mac addresses match.
        for serial in bt_config_map.keys():
            mac_address = bt_config_map[serial]["peripheral_info"][
                "address"].upper()
            bt_config_map[serial]["peripheral_info"]["address"] = mac_address
        for ad in self.android_devices:
            serial = ad.serial

            # Verify serial number in bt_config_map
            self.log.info("Verify serial number of Android device in config.")
            if serial not in bt_config_map.keys():
                self.log.error(
                    "Missing android device serial {} in bt_config.".format(
                        serial))
                return False
            # Push the bt_config.conf file to Android device
            self.log.info("Pushing bt_config.conf file to Android device.")
            config_path = bt_config_map[serial]["config_path"]
            if not os.path.isfile(config_path):
                config_path = os.path.join(
                    self.user_params[Config.key_config_path], config_path)
                if not os.path.isfile(config_path):
                    self.log.error("Unable to load bt_config file {}.".format(
                        config_path))
                    return False
            ad.adb.push("{} {}".format(config_path, bt_conf_path))

            # Add music to the Android device
            self.log.info("Pushing music to the Android device.")
            music_path_str = "music_path"
            android_music_path = "/sdcard/Music/"
            if music_path_str not in self.user_params:
                self.log.error("Need music for audio testcases...")
                return False

            music_path = self.user_params[music_path_str]
            if not os.path.isdir(music_path):
                music_path = os.path.join(
                    self.user_params[Config.key_config_path], music_path)
                if not os.path.isdir(music_path):
                    self.log.error("Unable to find music directory {}.".format(
                        music_path))
                    return False
            self._add_music_to_primary_android_device(ad, music_path,
                                                      android_music_path)
            ad.reboot()

            # Verify Bluetooth is enabled
            self.log.info("Verifying Bluetooth is enabled on Android Device.")
            if not bluetooth_enabled_check(ad):
                self.log.error("Failed to toggle on Bluetooth on device {}".
                               format(serial))
                return False

            # Verify Bluetooth device is connected
            self.log.info(
                "Waiting up to 10 seconds for device to reconnect...")
            connected_devices = ad.droid.bluetoothGetConnectedDevices()
            start_time = time.time()
            wait_time = 10
            result = False
            while time.time() < start_time + wait_time:
                connected_devices = ad.droid.bluetoothGetConnectedDevices()
                if len(connected_devices) > 0:
                    if bt_config_map[serial]["peripheral_info"]["address"] in {
                            d['address']
                            for d in connected_devices
                    }:
                        result = True
                        break
                else:
                    try:
                        ad.droid.bluetoothConnectBonded(bt_config_map[serial][
                            "peripheral_info"]["address"])
                    except Exception as err:
                        self.log.error(
                            "Failed to connect bonded. Err: {}".format(err))
            if not result:
                self.log.info("Connected Devices: {}".format(
                    connected_devices))
                self.log.info("Bonded Devices: {}".format(
                    ad.droid.bluetoothGetBondedDevices()))
                self.log.error(
                    "Failed to connect to peripheral name: {}, address: {}".
                    format(bt_config_map[serial]["peripheral_info"]["name"],
                           bt_config_map[serial]["peripheral_info"][
                               "address"]))
                self.device_fails_to_connect_list.append("{}:{}".format(
                    serial, bt_config_map[serial]["peripheral_info"]["name"]))
        if len(self.device_fails_to_connect_list) == len(self.android_devices):
            self.log.error("All devices failed to reconnect.")
            return False
        return True

    def _add_music_to_primary_android_device(self, ad, music_path,
                                             android_music_path):
        for dirname, dirnames, filenames in os.walk(music_path):
            for filename in filenames:
                self.music_file_to_play = filename
                file = os.path.join(dirname, filename)
                #TODO: Handle file paths with spaces
                ad.adb.push("{} {}".format(file, android_music_path))
        return True

    def _collect_bluetooth_manager_dumpsys_logs(self, ads):
        for ad in ads:
            serial = ad.serial
            out_name = "{}_{}".format(serial, "bluetooth_dumpsys.txt")
            dumpsys_path = ''.join((ad.log_path, "/BluetoothDumpsys"))
            create_dir(dumpsys_path)
            cmd = ''.join(
                ("adb -s ", serial, " shell dumpsys bluetooth_manager > ",
                 dumpsys_path, "/", out_name))
            exe_cmd(cmd)

    @BluetoothBaseTest.bt_test_wrap
    def test_run_bt_audio_12_hours(self):
        """Test audio quality over 12 hours.

        This test is designed to run Bluetooth audio for 12 hours
        and collect relavant logs. If all devices disconnect during
        the test or Bluetooth is off on all devices, then fail the
        test.

        Steps:
        1. For each Android device connected run steps 2-5.
        2. Open and play media file of music pushed to device
        3. Set media to loop indefintely.
        4. After 12 hours collect bluetooth_manager dumpsys information
        5. Stop media player

        Expected Result:
        Audio plays for 12 hours over Bluetooth

        Returns:
          Pass if True
          Fail if False

        TAGS: Classic, A2DP
        Priority: 1
        """
        for ad in self.android_devices:
            ad.droid.mediaPlayOpen("file:///sdcard/Music/{}".format(
                self.music_file_to_play))
            ad.droid.mediaPlaySetLooping(True)
            self.log.info("Music is now playing on device {}".format(
                ad.serial))

        sleep_interval = 120
        twelve_hours_in_seconds = 43200
        end_time = time.time() + twelve_hours_in_seconds
        test_result = True
        bluetooth_off_list = []
        device_not_connected_list = []
        while time.time() < end_time:
            for ad in self.android_devices:
                serial = ad.serial
                if (not ad.droid.bluetoothCheckState() and
                        serial not in bluetooth_off_list):
                    self.log.error(
                        "Device {}'s Bluetooth state is off.".format(serial))
                    bluetooth_off_list.append(serial)
                if (ad.droid.bluetoothGetConnectedDevices() == 0 and
                        serial not in device_not_connected_list):
                    self.log.error(
                        "Device {} not connected to any Bluetooth devices.".
                        format(serial))
                    device_not_connected_list.append(serial)
                if len(bluetooth_off_list) == len(self.android_devices):
                    self.log.error(
                        "Bluetooth off on all Android devices. Ending Test")
                    return False
                if len(device_not_connected_list) == len(self.android_devices):
                    self.log.error(
                        "Every Android device has no device connected.")
                    return False
            time.sleep(sleep_interval)

        self._collect_bluetooth_manager_dumpsys_logs(self.android_devices)
        for ad in self.android_devices:
            ad.droid.mediaPlayStopAll()
        if len(device_not_connected_list) > 0 or len(bluetooth_off_list) > 0:
            self.log.info("Devices reported as not connected: {}".format(
                device_not_connected_list))
            self.log.info("Devices reported with Bluetooth state off: {}".
                          format(bluetooth_off_list))
            return False
        return True

    def test_setup_fail_if_devices_not_connected(self):
        """Test for devices connected or not during setup.

        This test is designed to fail if the number of devices having
        connection issues at time of setup is greater than 0. This lets
        the test runner know of the stability of the testbed.

        Steps:
        1. Check lenght of self.device_fails_to_connect_list

        Expected Result:
        No device should be in a disconnected state.

        Returns:
          Pass if True
          Fail if False

        TAGS: None
        Priority: 1
        """
        if len(self.device_fails_to_connect_list) > 0:
            self.log.error("Devices failed to reconnect:\n{}".format(
                self.device_fails_to_connect_list))
            return False
        return True
