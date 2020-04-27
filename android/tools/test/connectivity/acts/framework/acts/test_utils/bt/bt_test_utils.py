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

import random
import pprint
import string
from queue import Empty
import threading
import time
from acts import utils

from subprocess import call

from acts.logger import LoggerProxy
from acts.test_utils.bt.BleEnum import AdvertiseSettingsAdvertiseMode
from acts.test_utils.bt.BleEnum import ScanSettingsCallbackType
from acts.test_utils.bt.BleEnum import ScanSettingsMatchMode
from acts.test_utils.bt.BleEnum import ScanSettingsMatchNum
from acts.test_utils.bt.BleEnum import ScanSettingsScanResultType
from acts.test_utils.bt.BleEnum import ScanSettingsScanMode
from acts.test_utils.bt.BleEnum import ScanSettingsReportDelaySeconds
from acts.test_utils.bt.BleEnum import AdvertiseSettingsAdvertiseType
from acts.test_utils.bt.BleEnum import AdvertiseSettingsAdvertiseTxPower
from acts.test_utils.bt.BleEnum import ScanSettingsMatchNum
from acts.test_utils.bt.BleEnum import ScanSettingsScanResultType
from acts.test_utils.bt.BleEnum import ScanSettingsScanMode
from acts.test_utils.bt.BtEnum import BluetoothScanModeType
from acts.test_utils.bt.BtEnum import RfcommUuid
from acts.test_utils.tel.tel_test_utils import toggle_airplane_mode
from acts.test_utils.tel.tel_test_utils import verify_http_connection
from acts.utils import exe_cmd

DEFAULT_TIMEOUT = 15
DEFAULT_RFCOMM_TIMEOUT = 10000
MAGIC_PAN_CONNECT_TIMEOUT = 5
DEFAULT_DISCOVERY_TIMEOUT = 3

log = LoggerProxy()

# Callback strings
scan_result = "BleScan{}onScanResults"
scan_failed = "BleScan{}onScanFailed"
batch_scan_result = "BleScan{}onBatchScanResult"
adv_fail = "BleAdvertise{}onFailure"
adv_succ = "BleAdvertise{}onSuccess"
bluetooth_off = "BluetoothStateChangedOff"
bluetooth_on = "BluetoothStateChangedOn"

# rfcomm test uuids
rfcomm_secure_uuid = "fa87c0d0-afac-11de-8a39-0800200c9a66"
rfcomm_insecure_uuid = "8ce255c0-200a-11e0-ac64-0800200c9a66"

advertisements_to_devices = {}

batch_scan_not_supported_list = ["Nexus 4",
                                 "Nexus 5",
                                 "Nexus 7", ]


class BtTestUtilsError(Exception):
    pass


def scan_and_verify_n_advertisements(scn_ad, max_advertisements):
    """Verify that input number of advertisements can be found from the scanning
    Android device.

    Args:
        scn_ad: The Android device to start LE scanning on.
        max_advertisements: The number of advertisements the scanner expects to
        find.

    Returns:
        True if successful, false if unsuccessful.
    """
    test_result = False
    address_list = []
    filter_list = scn_ad.droid.bleGenFilterList()
    scn_ad.droid.bleBuildScanFilter(filter_list)
    scan_settings = scn_ad.droid.bleBuildScanSetting()
    scan_callback = scn_ad.droid.bleGenScanCallback()
    scn_ad.droid.bleStartBleScan(filter_list, scan_settings, scan_callback)
    start_time = time.time()
    while (start_time + DEFAULT_TIMEOUT) > time.time():
        event = None
        try:
            event = scn_ad.ed.pop_event(
                scan_result.format(scan_callback), DEFAULT_TIMEOUT)
        except Empty as error:
            raise BtTestUtilsError("Failed to find scan event: {}".format(
                error))
        address = event['data']['Result']['deviceInfo']['address']
        if address not in address_list:
            address_list.append(address)
        if len(address_list) == max_advertisements:
            test_result = True
            break
    scn_ad.droid.bleStopBleScan(scan_callback)
    return test_result


def setup_n_advertisements(adv_ad, num_advertisements):
    """Setup input number of advertisements on input Android device.

    Args:
        adv_ad: The Android device to start LE advertisements on.
        num_advertisements: The number of advertisements to start.

    Returns:
        advertise_callback_list: List of advertisement callback ids.
    """
    adv_ad.droid.bleSetAdvertiseSettingsAdvertiseMode(
        AdvertiseSettingsAdvertiseMode.ADVERTISE_MODE_LOW_LATENCY.value)
    advertise_data = adv_ad.droid.bleBuildAdvertiseData()
    advertise_settings = adv_ad.droid.bleBuildAdvertiseSettings()
    advertise_callback_list = []
    for i in range(num_advertisements):
        advertise_callback = adv_ad.droid.bleGenBleAdvertiseCallback()
        advertise_callback_list.append(advertise_callback)
        adv_ad.droid.bleStartBleAdvertising(advertise_callback, advertise_data,
                                            advertise_settings)
        try:
            adv_ad.ed.pop_event(
                adv_succ.format(advertise_callback), DEFAULT_TIMEOUT)
            log.info("Advertisement {} started.".format(i + 1))
        except Empty as error:
            log.error("Advertisement {} failed to start.".format(i + 1))
            raise BtTestUtilsError("Test failed with Empty error: {}".format(
                error))
    return advertise_callback_list


def teardown_n_advertisements(adv_ad, num_advertisements,
                              advertise_callback_list):
    """Stop input number of advertisements on input Android device.

    Args:
        adv_ad: The Android device to stop LE advertisements on.
        num_advertisements: The number of advertisements to stop.
        advertise_callback_list: The list of advertisement callbacks to stop.

    Returns:
        True if successful, false if unsuccessful.
    """
    for n in range(num_advertisements):
        adv_ad.droid.bleStopBleAdvertising(advertise_callback_list[n])
    return True


def generate_ble_scan_objects(droid):
    """Generate generic LE scan objects.

    Args:
        droid: The droid object to generate LE scan objects from.

    Returns:
        filter_list: The generated scan filter list id.
        scan_settings: The generated scan settings id.
        scan_callback: The generated scan callback id.
    """
    filter_list = droid.bleGenFilterList()
    scan_settings = droid.bleBuildScanSetting()
    scan_callback = droid.bleGenScanCallback()
    return filter_list, scan_settings, scan_callback


def generate_ble_advertise_objects(droid):
    """Generate generic LE advertise objects.

    Args:
        droid: The droid object to generate advertise LE objects from.

    Returns:
        advertise_callback: The generated advertise callback id.
        advertise_data: The generated advertise data id.
        advertise_settings: The generated advertise settings id.
    """
    advertise_callback = droid.bleGenBleAdvertiseCallback()
    advertise_data = droid.bleBuildAdvertiseData()
    advertise_settings = droid.bleBuildAdvertiseSettings()
    return advertise_callback, advertise_data, advertise_settings


def setup_multiple_devices_for_bt_test(android_devices):
    """A common setup routine for Bluetooth on input Android device list.

    Things this function sets up:
    1. Resets Bluetooth
    2. Set Bluetooth local name to random string of size 4
    3. Disable BLE background scanning.
    4. Enable Bluetooth snoop logging.

    Args:
        android_devices: Android device list to setup Bluetooth on.

    Returns:
        True if successful, false if unsuccessful.
    """
    log.info("Setting up Android Devices")
    # TODO: Temp fix for an selinux error.
    for ad in android_devices:
        ad.adb.shell("setenforce 0")
    threads = []
    try:
        for a in android_devices:
            thread = threading.Thread(target=reset_bluetooth, args=([[a]]))
            threads.append(thread)
            thread.start()
        for t in threads:
            t.join()

        for a in android_devices:
            d = a.droid
            setup_result = d.bluetoothSetLocalName(generate_id_by_size(4))
            if not setup_result:
                log.error("Failed to set device name.")
                return setup_result
            d.bluetoothDisableBLE()
            bonded_devices = d.bluetoothGetBondedDevices()
            for b in bonded_devices:
                d.bluetoothUnbond(b['address'])
        for a in android_devices:
            setup_result = a.droid.bluetoothConfigHciSnoopLog(True)
            if not setup_result:
                log.error("Failed to enable Bluetooth Hci Snoop Logging.")
                return setup_result
    except Exception as err:
        log.error("Something went wrong in multi device setup: {}".format(err))
        return False
    return setup_result


def bluetooth_enabled_check(ad):
    """Checks if the Bluetooth state is enabled, if not it will attempt to
    enable it.

    Args:
        ad: The Android device list to enable Bluetooth on.

    Returns:
        True if successful, false if unsuccessful.
    """
    if not ad.droid.bluetoothCheckState():
        ad.droid.bluetoothToggleState(True)
        expected_bluetooth_on_event_name = bluetooth_on
        try:
            ad.ed.pop_event(expected_bluetooth_on_event_name, DEFAULT_TIMEOUT)
        except Empty:
            log.info("Failed to toggle Bluetooth on (no broadcast received).")
            # Try one more time to poke at the actual state.
            if ad.droid.bluetoothCheckState():
                log.info(".. actual state is ON")
                return True
            log.error(".. actual state is OFF")
            return False
    return True


def reset_bluetooth(android_devices):
    """Resets Bluetooth state of input Android device list.

    Args:
        android_devices: The Android device list to reset Bluetooth state on.

    Returns:
        True if successful, false if unsuccessful.
    """
    for a in android_devices:
        droid, ed = a.droid, a.ed
        log.info("Reset state of bluetooth on device: {}".format(
            droid.getBuildSerial()))
        if droid.bluetoothCheckState() is True:
            droid.bluetoothToggleState(False)
            expected_bluetooth_off_event_name = bluetooth_off
            try:
                ed.pop_event(expected_bluetooth_off_event_name,
                             DEFAULT_TIMEOUT)
            except Exception:
                log.error("Failed to toggle Bluetooth off.")
                return False
        # temp sleep for b/17723234
        time.sleep(3)
        if not bluetooth_enabled_check(a):
            return False
    return True


def determine_max_advertisements(android_device):
    """Determines programatically how many advertisements the Android device
    supports.

    Args:
        android_device: The Android device to determine max advertisements of.

    Returns:
        The maximum advertisement count.
    """
    log.info("Determining number of maximum concurrent advertisements...")
    advertisement_count = 0
    bt_enabled = False
    if not android_device.droid.bluetoothCheckState():
        android_device.droid.bluetoothToggleState(True)
    try:
        android_device.ed.pop_event(expected_bluetooth_on_event_name,
                                    DEFAULT_TIMEOUT)
    except Exception:
        log.info("Failed to toggle Bluetooth on (no broadcast received).")
        # Try one more time to poke at the actual state.
        if android_device.droid.bluetoothCheckState() is True:
            log.info(".. actual state is ON")
        else:
            log.error(
                "Failed to turn Bluetooth on. Setting default advertisements to 1")
            advertisement_count = -1
            return advertisement_count
    advertise_callback_list = []
    advertise_data = android_device.droid.bleBuildAdvertiseData()
    advertise_settings = android_device.droid.bleBuildAdvertiseSettings()
    while (True):
        advertise_callback = android_device.droid.bleGenBleAdvertiseCallback()
        advertise_callback_list.append(advertise_callback)

        android_device.droid.bleStartBleAdvertising(
            advertise_callback, advertise_data, advertise_settings)
        try:
            android_device.ed.pop_event(
                adv_succ.format(advertise_callback), DEFAULT_TIMEOUT)
            log.info("Advertisement {} started.".format(advertisement_count +
                                                        1))
            advertisement_count += 1
        except Exception as err:
            log.info(
                "Advertisement failed to start. Reached max advertisements at {}".
                format(advertisement_count))
            break
    try:
        for adv in advertise_callback_list:
            android_device.droid.bleStopBleAdvertising(adv)
    except Exception:
        log.error("Failed to stop advertisingment, resetting Bluetooth.")
        reset_bluetooth([android_device])
    return advertisement_count


def get_advanced_droid_list(android_devices):
    """Add max_advertisement and batch_scan_supported attributes to input
    Android devices

    This will programatically determine maximum LE advertisements of each
    input Android device.

    Args:
        android_devices: The Android devices to setup.

    Returns:
        List of Android devices with new attribtues.
    """
    droid_list = []
    for a in android_devices:
        d, e = a.droid, a.ed
        model = d.getBuildModel()
        max_advertisements = 1
        batch_scan_supported = True
        if model in advertisements_to_devices.keys():
            max_advertisements = advertisements_to_devices[model]
        else:
            max_advertisements = determine_max_advertisements(a)
            max_tries = 3
            #Retry to calculate max advertisements
            while max_advertisements == -1 and max_tries > 0:
                log.info("Attempts left to determine max advertisements: {}".
                         format(max_tries))
                max_advertisements = determine_max_advertisements(a)
                max_tries -= 1
            advertisements_to_devices[model] = max_advertisements
        if model in batch_scan_not_supported_list:
            batch_scan_supported = False
        role = {
            'droid': d,
            'ed': e,
            'max_advertisements': max_advertisements,
            'batch_scan_supported': batch_scan_supported
        }
        droid_list.append(role)
    return droid_list


def generate_id_by_size(
        size,
        chars=(
            string.ascii_lowercase + string.ascii_uppercase + string.digits)):
    """Generate random ascii characters of input size and input char types

    Args:
        size: Input size of string.
        chars: (Optional) Chars to use in generating a random string.

    Returns:
        String of random input chars at the input size.
    """
    return ''.join(random.choice(chars) for _ in range(size))


def cleanup_scanners_and_advertisers(scn_android_device, scn_callback_list,
                                     adv_android_device, adv_callback_list):
    """Try to gracefully stop all scanning and advertising instances.

    Args:
        scn_android_device: The Android device that is actively scanning.
        scn_callback_list: The scan callback id list that needs to be stopped.
        adv_android_device: The Android device that is actively advertising.
        adv_callback_list: The advertise callback id list that needs to be
            stopped.
    """
    scan_droid, scan_ed = scn_android_device.droid, scn_android_device.ed
    adv_droid = adv_android_device.droid
    try:
        for scan_callback in scn_callback_list:
            scan_droid.bleStopBleScan(scan_callback)
    except Exception as err:
        log.debug("Failed to stop LE scan... reseting Bluetooth. Error {}".
                  format(err))
        reset_bluetooth([scn_android_device])
    try:
        for adv_callback in adv_callback_list:
            adv_droid.bleStopBleAdvertising(adv_callback)
    except Exception as err:
        log.debug(
            "Failed to stop LE advertisement... reseting Bluetooth. Error {}".
            format(err))
        reset_bluetooth([adv_android_device])


def get_mac_address_of_generic_advertisement(scan_ad, adv_ad):
    """Start generic advertisement and get it's mac address by LE scanning.

    Args:
        scan_ad: The Android device to use as the scanner.
        adv_ad: The Android device to use as the advertiser.

    Returns:
        mac_address: The mac address of the advertisement.
        advertise_callback: The advertise callback id of the active
            advertisement.
    """
    adv_ad.droid.bleSetAdvertiseDataIncludeDeviceName(True)
    adv_ad.droid.bleSetAdvertiseSettingsAdvertiseMode(
        AdvertiseSettingsAdvertiseMode.ADVERTISE_MODE_LOW_LATENCY.value)
    adv_ad.droid.bleSetAdvertiseSettingsIsConnectable(True)
    adv_ad.droid.bleSetAdvertiseSettingsTxPowerLevel(
        AdvertiseSettingsAdvertiseTxPower.ADVERTISE_TX_POWER_HIGH.value)
    advertise_callback, advertise_data, advertise_settings = (
        generate_ble_advertise_objects(adv_ad.droid))
    adv_ad.droid.bleStartBleAdvertising(advertise_callback, advertise_data,
                                        advertise_settings)
    try:
        adv_ad.ed.pop_event(
            adv_succ.format(advertise_callback),
            DEFAULT_TIMEOUT)
    except Empty as err:
        raise BtTestUtilsError(
            "Advertiser did not start successfully {}".format(err))
    filter_list = scan_ad.droid.bleGenFilterList()
    scan_settings = scan_ad.droid.bleBuildScanSetting()
    scan_callback = scan_ad.droid.bleGenScanCallback()
    scan_ad.droid.bleSetScanFilterDeviceName(
        adv_ad.droid.bluetoothGetLocalName())
    scan_ad.droid.bleBuildScanFilter(filter_list)
    scan_ad.droid.bleStartBleScan(filter_list, scan_settings, scan_callback)
    try:
        event = scan_ad.ed.pop_event(
            "BleScan{}onScanResults".format(scan_callback), DEFAULT_TIMEOUT)
    except Empty as err:
        raise BtTestUtilsError("Scanner did not find advertisement {}".format(
            err))
    mac_address = event['data']['Result']['deviceInfo']['address']
    scan_ad.droid.bleStopBleScan(scan_callback)
    return mac_address, advertise_callback


def disable_bluetooth(droid):
    """Disable Bluetooth on input Droid object.

    Args:
        droid: The droid object to disable Bluetooth on.

    Returns:
        True if successful, false if unsuccessful.
    """
    if droid.bluetoothCheckState() is True:
        droid.bluetoothToggleState(False)
        if droid.bluetoothCheckState() is True:
            return False
    return True


def set_bt_scan_mode(ad, scan_mode_value):
    """Set Android device's Bluetooth scan mode.

    Args:
        ad: The Android device to set the scan mode on.
        scan_mode_value: The value to set the scan mode to.

    Returns:
        True if successful, false if unsuccessful.
    """
    droid, ed = ad.droid, ad.ed
    if scan_mode_value == BluetoothScanModeType.STATE_OFF.value:
        disable_bluetooth(droid)
        scan_mode = droid.bluetoothGetScanMode()
        reset_bluetooth([ad])
        if scan_mode != scan_mode_value:
            return False
    elif scan_mode_value == BluetoothScanModeType.SCAN_MODE_NONE.value:
        droid.bluetoothMakeUndiscoverable()
        scan_mode = droid.bluetoothGetScanMode()
        if scan_mode != scan_mode_value:
            return False
    elif scan_mode_value == BluetoothScanModeType.SCAN_MODE_CONNECTABLE.value:
        droid.bluetoothMakeUndiscoverable()
        droid.bluetoothMakeConnectable()
        scan_mode = droid.bluetoothGetScanMode()
        if scan_mode != scan_mode_value:
            return False
    elif (scan_mode_value ==
          BluetoothScanModeType.SCAN_MODE_CONNECTABLE_DISCOVERABLE.value):
        droid.bluetoothMakeDiscoverable()
        scan_mode = droid.bluetoothGetScanMode()
        if scan_mode != scan_mode_value:
            return False
    else:
        # invalid scan mode
        return False
    return True


def set_device_name(droid, name):
    """Set and check Bluetooth local name on input droid object.

    Args:
        droid: Droid object to set local name on.
        name: the Bluetooth local name to set.

    Returns:
        True if successful, false if unsuccessful.
    """
    droid.bluetoothSetLocalName(name)
    time.sleep(2)
    droid_name = droid.bluetoothGetLocalName()
    if droid_name != name:
        return False
    return True


def check_device_supported_profiles(droid):
    """Checks for Android device supported profiles.

    Args:
        droid: The droid object to query.

    Returns:
        A dictionary of supported profiles.
    """
    profile_dict = {}
    profile_dict['hid'] = droid.bluetoothHidIsReady()
    profile_dict['hsp'] = droid.bluetoothHspIsReady()
    profile_dict['a2dp'] = droid.bluetoothA2dpIsReady()
    profile_dict['avrcp'] = droid.bluetoothAvrcpIsReady()
    return profile_dict


def pair_pri_to_sec(pri_droid, sec_droid):
    """Pairs pri droid to sec droid.

    Args:
        pri_droid: Droid initiating pairing.
        sec_droid: Droid accepting pairing.

    Returns:
        True if pairing is successful, false if uncsuccsessful.
    """
    # Enable discovery on sec_droid so that pri_droid can find it.
    # The timeout here is based on how much time it would take for two devices
    # to pair with each other once pri_droid starts seeing devices.
    log.info(
        "Bonding device {} to {}".format(pri_droid.bluetoothGetLocalAddress(),
                                         sec_droid.bluetoothGetLocalAddress()))
    sec_droid.bluetoothMakeDiscoverable(DEFAULT_TIMEOUT)
    target_address = sec_droid.bluetoothGetLocalAddress()
    log.debug("Starting paring helper on each device")
    pri_droid.bluetoothStartPairingHelper()
    sec_droid.bluetoothStartPairingHelper()
    log.info("Primary device starting discovery and executing bond")
    result = pri_droid.bluetoothDiscoverAndBond(target_address)
    # Loop until we have bonded successfully or timeout.
    end_time = time.time() + DEFAULT_TIMEOUT
    log.info("Verifying devices are bonded")
    while time.time() < end_time:
        bonded_devices = pri_droid.bluetoothGetBondedDevices()
        bonded = False
        for d in bonded_devices:
            if d['address'] == target_address:
                log.info("Successfully bonded to device")
                return True
    # Timed out trying to bond.
    log.info("Failed to bond devices.")
    return False


def connect_pri_to_sec(log, pri_droid, sec_droid, profiles_set):
    """Connects pri droid to secondary droid.

    Args:
        pri_droid: Droid initiating connection.
        sec_droid: Droid accepting connection.
        profiles_set: Set of profiles to be connected.

    Returns:
        True of connection is successful, false if unsuccessful.
    """
    # Check if we support all profiles.
    supported_profiles = [i.value for i in BluetoothProfile]
    for profile in profiles_set:
        if profile not in supported_profiles:
            log.info("Profile {} is not supported list {}".format(
                profile, supported_profiles))
            return False

    # First check that devices are bonded.
    paired = False
    for paired_device in pri_droid.droid.bluetoothGetBondedDevices():
        if paired_device['address'] == \
            sec_droid.bluetoothGetLocalAddress():
            paired = True
            break

    if not paired:
        log.info("{} not paired to {}".format(pri_droid.droid.getBuildSerial(),
                                              sec_droid.getBuildSerial()))
        return False

    # Now try to connect them, the following call will try to initiate all
    # connections.
    pri_droid.droid.bluetoothConnectBonded(sec_droid.bluetoothGetLocalAddress(
    ))

    profile_connected = set()
    log.info("Profiles to be connected {}".format(profiles_set))
    while not profile_connected.issuperset(profiles_set):
        try:
            profile_event = pri_droid.ed.pop_event(
                bluetooth_profile_connection_state_changed, DEFAULT_TIMEOUT)
            log.info("Got event {}".format(profile_event))
        except Exception:
            log.error("Did not get {} profiles left {}".format(
                bluetooth_profile_connection_state_changed, profile_connected))
            return False

        profile = profile_event['data']['profile']
        state = profile_event['data']['state']
        device_addr = profile_event['data']['addr']

        if state == BluetoothProfileState.STATE_CONNECTED.value and \
            device_addr == sec_droid.bluetoothGetLocalAddress():
            profile_connected.add(profile)
        log.info("Profiles connected until now {}".format(profile_connected))
    # Failure happens inside the while loop. If we came here then we already
    # connected.
    return True


def take_btsnoop_logs(android_devices, testcase, testname):
    """Pull btsnoop logs from an input list of android devices.

    Args:
        android_devices: the list of Android devices to pull btsnoop logs from.
        testcase: Name of the test calss that triggered this snoop log.
        testname: Name of the test case that triggered this bug report.
    """
    for a in android_devices:
        take_btsnoop_log(a, testcase, testname)


def take_btsnoop_log(ad, testcase, testname):
    """Grabs the btsnoop_hci log on a device and stores it in the log directory
    of the test class.

    If you want grab the btsnoop_hci log, call this function with android_device
    objects in on_fail. Bug report takes a relative long time to take, so use
    this cautiously.

    Args:
        ad: The android_device instance to take bugreport on.
        testcase: Name of the test calss that triggered this snoop log.
        testname: Name of the test case that triggered this bug report.
    """
    testname = "".join(x for x in testname if x.isalnum())
    serial = ad.droid.getBuildSerial()
    device_model = ad.droid.getBuildModel()
    device_model = device_model.replace(" ", "")
    out_name = ','.join((testname, device_model, serial))
    snoop_path = ad.log_path + "/BluetoothSnoopLogs"
    utils.create_dir(snoop_path)
    cmd = ''.join(("adb -s ", serial, " pull /sdcard/btsnoop_hci.log ",
                   snoop_path + '/' + out_name, ".btsnoop_hci.log"))
    testcase.log.info("Test failed, grabbing the bt_snoop logs on {} {}."
                      .format(device_model, serial))
    exe_cmd(cmd)


def kill_bluetooth_process(ad):
    """Kill Bluetooth process on Android device.

    Args:
        ad: Android device to kill BT process on.
    """
    log.info("Killing Bluetooth process.")
    pid = ad.adb.shell(
        "ps | grep com.android.bluetooth | awk '{print $2}'").decode('ascii')
    call(["adb -s " + ad.serial + " shell kill " + pid], shell=True)


def orchestrate_rfcomm_connection(client_ad,
                                  server_ad,
                                  accept_timeout_ms=DEFAULT_RFCOMM_TIMEOUT):
    """Sets up the RFCOMM connection between two Android devices.

    Args:
        client_ad: the Android device performing the connection.
        server_ad: the Android device accepting the connection.
    Returns:
        True if connection was successful, false if unsuccessful.
    """
    server_ad.droid.bluetoothStartPairingHelper()
    client_ad.droid.bluetoothStartPairingHelper()
    server_ad.droid.bluetoothRfcommBeginAcceptThread(
        RfcommUuid.DEFAULT_UUID.value, accept_timeout_ms)
    client_ad.droid.bluetoothRfcommBeginConnectThread(
        server_ad.droid.bluetoothGetLocalAddress())
    end_time = time.time() + DEFAULT_TIMEOUT
    result = False
    test_result = True
    while time.time() < end_time:
        if len(client_ad.droid.bluetoothRfcommActiveConnections()) > 0:
            test_result = True
            log.info("RFCOMM Client Connection Active")
            break
        else:
            test_result = False
        time.sleep(1)
    if not test_result:
        log.error("Failed to establish an RFCOMM connection")
        return False
    return True


def write_read_verify_data(client_ad, server_ad, msg, binary=False):
    """Verify that the client wrote data to the server Android device correctly.

    Args:
        client_ad: the Android device to perform the write.
        server_ad: the Android device to read the data written.
        msg: the message to write.
        binary: if the msg arg is binary or not.

    Returns:
        True if the data written matches the data read, false if not.
    """
    log.info("Write message.")
    try:
        if binary:
            client_ad.droid.bluetoothRfcommWriteBinary(msg)
        else:
            client_ad.droid.bluetoothRfcommWrite(msg)
    except Exception as err:
        log.error("Failed to write data: {}".format(err))
        return False
    log.info("Read message.")
    try:
        if binary:
            read_msg = server_ad.droid.bluetoothRfcommReadBinary().rstrip(
                "\r\n")
        else:
            read_msg = server_ad.droid.bluetoothRfcommRead()
    except Exception as err:
        log.error("Failed to read data: {}".format(err))
        return False
    log.info("Verify message.")
    if msg != read_msg:
        log.error("Mismatch! Read: {}, Expected: {}".format(read_msg, msg))
        return False
    return True


def clear_bonded_devices(ad):
    """Clear bonded devices from the input Android device.

    Args:
        ad: the Android device performing the connection.
    Returns:
        True if clearing bonded devices was successful, false if unsuccessful.
    """
    bonded_device_list = ad.droid.bluetoothGetBondedDevices()
    for device in bonded_device_list:
        device_address = device['address']
        if not ad.droid.bluetoothUnbond(device_address):
            log.error("Failed to unbond {}".format(device_address))
            return False
        log.info("Successfully unbonded {}".format(device_address))
    return True


def verify_server_and_client_connected(client_ad, server_ad):
    """Verify that input server and client Android devices are connected.

    This code is under the assumption that there will only be
    a single connection.

    Args:
        client_ad: the Android device to check number of active connections.
        server_ad: the Android device to check number of active connections.

    Returns:
        True both server and client have at least 1 active connection,
        false if unsuccessful.
    """
    test_result = True
    if len(server_ad.droid.bluetoothRfcommActiveConnections()) == 0:
        log.error("No rfcomm connections found on server.")
        test_result = False
    if len(client_ad.droid.bluetoothRfcommActiveConnections()) == 0:
        log.error("No rfcomm connections found on client.")
        test_result = False
    return test_result


def orchestrate_and_verify_pan_connection(pan_dut, panu_dut):
    """Setups up a PAN conenction between two android devices.

    Args:
        pan_dut: the Android device providing tethering services
        panu_dut: the Android device using the internet connection from the
            pan_dut
    Returns:
        True if PAN connection and verification is successful,
        false if unsuccessful.
    """
    if not toggle_airplane_mode(log, panu_dut, True):
        log.error("Failed to toggle airplane mode on")
        return False
    if not bluetooth_enabled_check(panu_dut):
        return False
    pan_dut.droid.bluetoothPanSetBluetoothTethering(True)
    if not (pair_pri_to_sec(pan_dut.droid, panu_dut.droid)):
        return False
    if not pan_dut.droid.bluetoothPanIsTetheringOn():
        log.error("Failed to enable Bluetooth tethering.")
        return False
    # Magic sleep needed to give the stack time in between bonding and
    # connecting the PAN profile.
    time.sleep(MAGIC_PAN_CONNECT_TIMEOUT)
    panu_dut.droid.bluetoothConnectBonded(
        pan_dut.droid.bluetoothGetLocalAddress())
    if not verify_http_connection(log, panu_dut):
        log.error("Can't verify http connection on PANU device.")
        if not verify_http_connection(log, pan_dut):
            log.info("Can't verify http connection on PAN service device")
        return False
    return True
