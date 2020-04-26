#ifndef _COMMANDLISTENER_H__
#define _COMMANDLISTENER_H__

#include <string>
#include <list>
#include <vector>

#include <sysutils/FrameworkListener.h>
#include <sysutils/FrameworkCommand.h>
#include "ResponseCode.h"

using namespace std;

class CommandListener : public FrameworkListener {
public:
    CommandListener();
    virtual ~CommandListener();

private:
    static void dumpArgs(int argc, char **argv, int argObscure);

    class GpioCmd : public FrameworkCommand {
    public:
        GpioCmd();
        virtual ~GpioCmd() {}
        int runCommand(SocketClient *c, int argc, char **argv);
    };

};

#endif // _COMMANDLISTENER_H__
