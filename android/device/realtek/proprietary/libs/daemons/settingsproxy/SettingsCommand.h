#ifndef _SETTINGS_COMMAND_H
#define _SETTINGS_COMMAND_H

#include <sysutils/FrameworkCommand.h>

class SettingsCommand : public FrameworkCommand {
public:
    SettingsCommand(const char *cmd);
    virtual ~SettingsCommand() {}
};

#endif // _SETTINGS_COMMAND_H
