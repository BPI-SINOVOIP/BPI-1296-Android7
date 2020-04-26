#ifndef _SETTINGS_CLIENT_H
#define _SETTINGS_CLIENT_H

#include <string>
#include <list>

class settingsclient {
public:
    settingsclient(const char *socket_name);
    settingsclient();
    virtual ~settingsclient();

    bool isReady();
    int sendRequest(int num, char **parameters);
    std::list<std::string>& getResponseData();

    static int prepareCmd(char ***cmd, int count, ...);
    static void freeCmd(int count, char **cmd);

private:
    int init(const char *socket_name, int &sock);
    int do_cmd(int sock, int argc, char **argv);
    int do_monitor(int sock, bool stop_after_cmd);

private:
    int mSock;
    std::list<std::string> mResponseData;
    char mSocketName[256];
};

#endif //  _SETTINGS_CLIENT_H
