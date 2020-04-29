/*
 * Copyright (c) 2017 Realtek Semiconductor Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#ifndef CWMP_UM_H
#define CWMP_UM_H
#include <linux/ioctl.h>

enum cwmp_um_cmd {
    eCreateFileNode,
    eCreateDirNode,
    eDeleteFileNode,
    eDeleteDirNode,
    eWriteValue,
    eReadValue,
    eMaxUmCmd,
};

struct cwmp_um_config {
    enum cwmp_um_cmd cmd;
    char * path;
    char * value;
};

#define CWMP_IOC_MAGIC  't'
#define CWMP_IOC_SENDCMD  _IOWR(CWMP_IOC_MAGIC, 1, struct cwmp_um_config)
//#define CWMP_IOC_SENDCMD  _IO(CWMP_IOC_MAGIC, 1)

#endif /* End of CWMP_UM_H */
