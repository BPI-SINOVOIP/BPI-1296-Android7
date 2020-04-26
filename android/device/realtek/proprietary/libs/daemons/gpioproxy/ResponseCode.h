/*
 * These values should be consistent with the definitions in RtkGPIOService.java.
*/

#ifndef _RESPONSECODE_H
#define _RESPONSECODE_H

class ResponseCode {
public:
    // 100 series - Requested action was initiated; expect another reply
    // before proceeding with a new command;
    static const int ActionInitiated = 100;

    // 200 series - Requested action has been successfully completed
    static const int CommandOkay    = 200;
    static const int GpioResult     = 210;

    // 400 series - The command was accepted but the requested action
    // did not take place.
    static const int OperationFailed    = 400;

    // 500 series - The command was not accepted and the requested
    // action did not take place.
    static const int CommandSyntaxError = 500;
    static const int CommandParameterError = 501;
    static const int CommandNoPermission = 502;

    // 600 series - Unsolicited broadcasts.
    static const int GpioTrigger = 650;  // used to notify higer (java) layer gpio trigger.
    
    static int convertFromErrno();
};
#endif // _RESPONSECODE_H
