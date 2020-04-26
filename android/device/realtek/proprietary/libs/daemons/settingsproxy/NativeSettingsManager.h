#ifndef _NATIVE_SETTINGS_MANAGER_H_
#define _NATIVE_SETTINGS_MANAGER_H_
#include <string>
#include <list>

#include <utils/StrongPointer.h>
#include <binder/IBinder.h>

using namespace std;
using namespace android;

class NativeSettingsManager {

public:

    NativeSettingsManager();
    virtual ~NativeSettingsManager();

    string getString(string table, string name);
    bool putString(string table, string name, string value);
    int getInt(string table, string name);
    bool putInt(string table, string name, int value);
    long getLong(string table, string name);
    bool putLong(string table, string name, long value);
    float getFloat(string table, string name);
    bool putFloat(string table, string name, float value);

    string getProperty(string prop, string def);
    bool setProperty(string prop, string value);

    string wifiCmd(list<string> cmd);

private:
    sp<IBinder> getSettingsServiceBinder();
    status_t writeInt32ToField(const char *table, const char *fieldName, const int32_t value);
    int readInt32ToField(const char *table, const char *fieldName);
    status_t writeStringToField(const char* table, const char *fieldName, const char* value);
    const char* readStringToField(const char* table, const char *fieldName);

    sp<IBinder> mSettingsServiceBinder;
};

#endif // _NATIVE_SETTINGS_MANAGER_H_
