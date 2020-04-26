#ifndef __C_DVD_VIDEO_CMD_HANDLER_H__
#define __C_DVD_VIDEO_CMD_HANDLER_H__

#include "common.h"

class CDVDVideoVM;

// dvd-video navigation command parser and handler

class CDVDVideoCmdHandler
{
public:

    CDVDVideoCmdHandler(CDVDVideoVM* pVM) : m_pVM(pVM) {}

    // to execute dvd-video commands
    void ExecuteCmds(vm_cmd_t* pCmds, int totalLines, int startLine,
                     DVDVideoState* pState, DVDVideoBranchCmd* pBranchCmd, DVDVideoCmdArea area);

    // direct commands for changing STNs
    inline bool SetAudioStream(DVDVideoState* pState, int newSTN)
    {
        m_pState = pState;
        return SetASTN(newSTN);
    }

    inline HRESULT SetSpicStream(DVDVideoState* pState, int newSTN)
    {
        m_pState = pState;
        return SetSPSTN(newSTN);
    }

    inline HRESULT SetAngle(DVDVideoState* pState, int newSTN)
    {
        m_pState = pState;
        return SetAGLN(newSTN);
    }

protected:

    DVDVideoState*     m_pState;
    DVDVideoBranchCmd* m_pBranchCmd;
    DVDVideoCmdArea    m_area;
    CDVDVideoVM*       m_pVM;

    // to start parsing and dispatching one dvd-video command
    // return positive value for "GOTO", '-1' for "BREAK", or '0' to move on
    int Execute(uint64_t cmd);

    // dispatch for major command groups
    // return positive value for "GOTO", '-1' for "BREAK", or '0' to move on
    int ExecuteGroupGoto               (uint64_t cmd);
    int ExecuteGroupLink               (uint64_t cmd);
    int ExecuteGroupJump               (uint64_t cmd);
    int ExecuteGroupSetSystem          (uint64_t cmd);
    int ExecuteGroupSet                (uint64_t cmd);
    int ExecuteGroupSetCompareLinkSub  (uint64_t cmd);
    int ExecuteGroupCompare_SetLinkSub (uint64_t cmd);
    int ExecuteGroupCompareSet_LinkSub (uint64_t cmd);

    // major commands
    bool Compare   (int code, int IFlag, int C1, int C2);
    void Set       (int code, int IFlag, int S1, int S2);
    void SetSystem (int code, int IFlag, int OP);
    int  Goto      (int code, int OP);
    int  Link      (int code, int OP);
    int  Jump      (int code, int OP);
    int  LinkSub   (int OP);

    // set system commands
    void SetSTN      (int IFlag, int OP);
    void SetNvTimer  (int IFlag, int OP);
    void SetGprmMode (int IFlag, int OP);
    void SetAMixMode (int IFlag, int OP);
    void SetHL_BTNN  (int IFlag, int OP);
    bool SetASTN     (int newSTN); // sub-function of SetSTN
    bool SetSPSTN    (int newSTN); // sub-function of SetSTN
    bool SetAGLN     (int newSTN); // sub-function of SetSTN

    // helper functions
    inline int GetCmdID1   (uint64_t cmd) { return GetBits(cmd, 61, 63); }
    inline int GetCmdID2   (uint64_t cmd) { return GetBits(cmd, 60, 60); }
    inline int GetSetField (uint64_t cmd) { return GetBits(cmd, 56, 59); }
    inline int GetSetIFlag (uint64_t cmd) { return GetBits(cmd, 60, 60); }
    inline int GetCmpField (uint64_t cmd) { return GetBits(cmd, 52, 54); }
    inline int GetCmpIFlag (uint64_t cmd) { return GetBits(cmd, 55, 55); }
    inline int GetBchField (uint64_t cmd) { return GetBits(cmd, 48, 51); }

    bool GetCmpSetParameters(int IFlag, int P1, int P2,
                             uint16_t** data1, uint16_t** data2, uint16_t* tmp,
                             bool* counter_mode1, bool* counter_mode2);
};

#endif //__C_DVD_VIDEO_CMD_HANDLER_H__
