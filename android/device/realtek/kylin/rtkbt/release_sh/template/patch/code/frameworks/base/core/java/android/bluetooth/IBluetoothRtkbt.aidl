package android.bluetooth;

interface IBluetoothRtkbt {
    int GetFeature(int id);
    int GenericCommand(int id, int command, in byte[] data, int len);
}
