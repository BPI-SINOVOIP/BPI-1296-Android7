#ifndef _WIFIDATA_H_
#define _WIFIDATA_H_

#include <vector>
#include <string>

class WIFIDATA {
private:
    std::vector<std::string> mData;

public:
    WIFIDATA();
    ~WIFIDATA();

    int getCount();
    void setCount(const int count);
    bool add(const char *item);
    const char* get(const int index);
};

#endif // _WIFIDATA_H_
