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
Test the HFP profile for basic calling functionality.
"""

import time

from acts.test_utils.bt.BluetoothBaseTest import BluetoothBaseTest
from acts.test_utils.bt import BtEnum
from acts.test_utils.bt import bt_test_utils
from acts.test_utils.car import car_telecom_utils
from acts.test_utils.tel import tel_defines

BLUETOOTH_PKG_NAME = "com.android.bluetooth"
CALL_TYPE_OUTGOING = "CALL_TYPE_OUTGOING"
CALL_TYPE_INCOMING = "CALL_TYPE_INCOMING"
default_timeout = 20

class BtCarHfpTest(BluetoothBaseTest):
    def setup_class(self):
        self.hf = self.android_devices[0]
        self.ag = self.android_devices[1]
        self.re = self.android_devices[2]
        self.ag_phone_number = "tel:{}".format(
            self.ag.droid.telephonyGetLine1Number())
        self.re_phone_number = "tel:{}".format(
            self.re.droid.telephonyGetLine1Number())
        self.log.info("ag tel: {} re tel: {}".format(
            self.ag_phone_number, self.re_phone_number))

        # Setup includes pairing and connecting the devices.
        bt_test_utils.setup_multiple_devices_for_bt_test([self.hf, self.ag])
        bt_test_utils.reset_bluetooth([self.hf, self.ag])

        # Pair and connect the devices.
        if not bt_test_utils.pair_pri_to_sec(self.hf.droid, self.ag.droid):
            self.log.error("Failed to pair")
            return False

        # Connect the devices now, try twice.
        attempts = 2
        connected = False
        while attempts > 0 and not connected:
            connected = bt_test_utils.connect_pri_to_sec(
                self.log, self.hf, self.ag.droid,
                set([BtEnum.BluetoothProfile.HEADSET_CLIENT.value]))
            self.log.info("Connected {}".format(connected))
            attempts -= 1
        return connected

    def setup_test(self):
        # Reset the devices.
        for d in self.android_devices:
            d.ed.clear_all_events()

    def on_fail(self, test_name, begin_time):
        self.log.debug("Test {} failed.".format(test_name))

    @BluetoothBaseTest.bt_test_wrap
    def test_default_calling_account(self):
        """
        Tests if the default calling account is coming from the
        bluetooth pacakge.

        Precondition:
        1. Devices are connected.

        Steps:
        1. Check if the default calling account is via Bluetooth package.

        Returns:
          Pass if True
          Fail if False

        Priority: 0
        """
        selected_acc = \
            self.hf.droid.telecomGetUserSelectedOutgoingPhoneAccount()
        if not selected_acc:
            self.log.info("No default account found.")
            return False

        # Check if the default account is from the Bluetooth package. This is a
        # light weight check.
        try:
            acc_component_id = selected_acc['ComponentName']
        except KeyError:
            self.log.info(
                "No component name for account {}".format(selected_acc))
            return False
        if not acc_component_id.startswith(BLUETOOTH_PKG_NAME):
            self.log.info(
                "Component name does not start with pkg name {}".format(
                    selected_acc))
            return False

    @BluetoothBaseTest.bt_test_wrap
    def test_outgoing_call_hf(self):
        """
        Tests if we can make a phone call from HF role and disconnect from HF
        role.

        Precondition:
        1. Devices are connected.

        Steps:
        1. Make a call from HF role.
        2. Wait for the HF, AG to be dialing and RE to see the call ringing.
        3. Hangup the call on HF role.
        4. Wait for all devices to hangup the call.

        Returns:
          Pass if True
          Fail if False

        Priority: 0
        """
        return self.dial_a_hangup_b(self.hf, self.hf)


    @BluetoothBaseTest.bt_test_wrap
    def test_outgoing_call_ag(self):
        """
        Tests if we can make a phone call from AG role and disconnect from AG
        role.

        Precondition:
        1. Devices are connected.

        Steps:
        1. Make a call from AG role.
        2. Wait for the HF, AG to be in dialing and RE to see the call ringing.
        3. Hangup the call on AG role.
        4. Wait for all devices to hangup the call.

        Returns:
          Pass if True
          Fail if False

        Priority: 0
        """
        return self.dial_a_hangup_b(self.ag, self.ag)

    @BluetoothBaseTest.bt_test_wrap
    def test_outgoing_dial_ag_hangup_hf(self):
        """
        Tests if we can make a phone call from AG role and disconnect from HF
        role.

        Precondition:
        1. Devices are connected.

        Steps:
        1. Make a call from AG role.
        2. Wait for the HF, AG to show dialing and RE to see the call ringing.
        3. Hangup the call on HF role.
        4. Wait for all devices to hangup the call.

        Returns:
          Pass if True
          Fail if False

        Priority: 0
        """
        return self.dial_a_hangup_b(self.ag, self.hf)

    @BluetoothBaseTest.bt_test_wrap
    def test_outgoing_dial_hf_hangup_ag(self):
        """
        Tests if we can make a phone call from HF role and disconnect from AG
        role.

        Precondition:
        1. Devices are connected.

        Steps:
        1. Make a call from HF role.
        2. Wait for the HF, AG to show dialing and RE to see the call ringing.
        3. Hangup the call on AG role.
        4. Wait for all devices to hangup the call.

        Returns:
          Pass if True
          Fail if False

        Priority: 0
        """
        return self.dial_a_hangup_b(self.hf, self.ag)

    @BluetoothBaseTest.bt_test_wrap
    def test_incoming_dial_re_hangup_re(self):
        """
        Tests if we can make a phone call from remote and disconnect from
        remote.

        Precondition:
        1. Devices are connected.

        Steps:
        1. Make a call from RE role.
        2. Wait for the HF, AG to show ringing and RE to see the call dialing.
        3. Hangup the call on RE role.
        4. Wait for all devices to hangup the call.

        Returns:
          Pass if True
          Fail if False

        Priority: 0
        """
        return self.dial_a_hangup_b(self.re, self.re, self.ag_phone_number)

    def dial_a_hangup_b(self, a, b, ph=""):
        """
        a, b and c can be either of AG, HF or Remote.
        1. Make a call from 'a' on a fixed number.
        2. Wait for the call to get connected (check on both 'a' and 'b')
           Check that 'c' is in ringing state.
        3. Hangup the call on 'b'.
        4. Wait for call to get completely disconnected
        (check on both 'a' and 'b')
        It is assumed that scenarios will not go into voice mail.
        """
        if ph == "": ph = self.re_phone_number

        # Determine if this is outgoing or incoming call.
        call_type = None
        if a == self.ag or a == self.hf:
            call_type = CALL_TYPE_OUTGOING
            if b != self.ag and b != self.hf:
                self.log.info("outgoing call should terminate at AG or HF")
                return False
        elif a == self.re:
            call_type = CALL_TYPE_INCOMING
            if b != self.re:
                self.log.info("Incoming call should terminate at Re")
                return False

        self.log.info("Call type is {}".format(call_type))

        # make a call on 'a'
        if not car_telecom_utils.dial_number(self.log, a, ph):
            return False

        # Check that everyone is in dialing/ringing state.
        ret = True
        if call_type == CALL_TYPE_OUTGOING:
            ret &= car_telecom_utils.wait_for_dialing(self.log, self.hf)
            ret &= car_telecom_utils.wait_for_dialing(self.log, self.ag)
            ret &= car_telecom_utils.wait_for_ringing(self.log, self.re)
        else:
            ret &= car_telecom_utils.wait_for_ringing(self.log, self.hf)
            ret &= car_telecom_utils.wait_for_ringing(self.log, self.ag)
            ret &= car_telecom_utils.wait_for_dialing(self.log, self.re)
        if not ret:
            return False

        # Check if we have any calls with dialing or active state on 'b'.
        # We assume we never disconnect from 'ringing' state since it will lead
        # to voicemail.
        call_state_dialing_or_active = \
            [tel_defines.CALL_STATE_CONNECTING,
             tel_defines.CALL_STATE_DIALING,
             tel_defines.CALL_STATE_ACTIVE]

        calls_in_dialing_or_active = car_telecom_utils.get_calls_in_states(
            self.log, b, call_state_dialing_or_active)

        # Make sure there is only one!
        if len(calls_in_dialing_or_active) != 1:
            self.log.info("Call State in dialing or active failed {}".format(
                calls_in_dialing_or_active))
            return False

        # Hangup the *only* call on 'b'
        if not car_telecom_utils.hangup_call(
            self.log, b, calls_in_dialing_or_active[0]):
            return False

        # Make sure everyone got out of in call state.
        for d in self.android_devices:
            ret &= car_telecom_utils.wait_for_not_in_call(self.log, d)
        return ret
