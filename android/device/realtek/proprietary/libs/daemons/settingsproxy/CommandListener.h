#ifndef _COMMANDLISTENER_H__
#define _COMMANDLISTENER_H__

#include <string>
#include <list>
#include <vector>

#include <sysutils/FrameworkListener.h>
#include "SettingsCommand.h"

using namespace std;

class CommandListener : public FrameworkListener {
public:
    CommandListener();
    virtual ~CommandListener();

private:
    static void dumpArgs(int argc, char **argv, int argObscure);

    class DbCmd : public SettingsCommand {
    public:
        DbCmd();
        virtual ~DbCmd() {}
        int runCommand(SocketClient *c, int argc, char **argv);
        int handleWriteCmd(SocketClient *c, vector<string> cmd);
        int handleReadCmd(SocketClient *c, vector<string> cmd);
    };

    class PropertyCmd : public SettingsCommand {
    public:
        PropertyCmd();
        virtual ~PropertyCmd() {}
        int runCommand(SocketClient *c, int argc, char **argv);
    };

    class WifiCmd : public SettingsCommand {
    public:
        WifiCmd();
        virtual ~WifiCmd() {}
        int runCommand(SocketClient *c, int argc, char **argv);
    };
};

#endif // _COMMANDLISTENER_H__
