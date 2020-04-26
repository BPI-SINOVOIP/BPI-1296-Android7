#ifndef _SYSTEMCOMMAND_H_
#define _SYSTEMCOMMAND_H_

#define SIDE_BY_SIDE_3DMODE                  1
#define TOP_AND_BOTTOM_3DMODE                2

bool system_init(void);
bool system_analogop(bool op); // turn analog on or off
bool system_getTVSupport3D(unsigned short* p3D_TV_Info);
bool system_set3DDisplay(bool on, int mode, int fps); // set 3d to on or off
void system_close(void); // close IPC channel
#endif
