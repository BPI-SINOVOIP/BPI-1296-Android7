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

from enum import Enum
from enum import IntEnum


class BluetoothScanModeType(IntEnum):
    STATE_OFF = -1
    SCAN_MODE_NONE = 0
    SCAN_MODE_CONNECTABLE = 1
    SCAN_MODE_CONNECTABLE_DISCOVERABLE = 3


class BluetoothAdapterState(IntEnum):
    STATE_OFF = 10
    STATE_TURNING_ON = 11
    STATE_ON = 12
    STATE_TURNING_OFF = 13
    STATE_BLE_TURNING_ON = 14
    STATE_BLE_ON = 15
    STATE_BLE_TURNING_OFF = 16

class RfcommUuid(Enum):
    DEFAULT_UUID = "457807c0-4897-11df-9879-0800200c9a66"

class BluetoothProfile(IntEnum):
    # Should be kept in sync with BluetoothProfile.java
    HEADSET = 1
    A2DP = 2
    HEALTH = 3
    INPUT_DEVICE = 4
    PAN = 5
    PBAP = 6
    GATT = 7
    GATT_SERVER = 8
    MAP = 9
    SAP = 10
    A2DP_SINK = 11
    AVRCP_CONTROLLER = 12
    HEADSET_CLIENT = 16
    PBAP_CLIENT = 17

class BluetoothProfileState(IntEnum):
    # Should be kept in sync with BluetoothProfile#STATE_* constants.
    STATE_DISCONNECTED = 0
    STATE_CONNECTING = 1
    STATE_CONNECTED = 2
    STATE_DISCONNECTING = 3

