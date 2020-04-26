/*
 * Copyright (c) 2015 Realtek Semiconductor Corp.
 *
 * MdOp.h
 *
 *  Created on: June 25, 2015
 *      Author: calvinkuo
 */

#ifndef MDOP_H_
#define MDOP_H_

#ifdef __cplusplus
extern "C" {
#endif

unsigned int MdInit();
void MdUninit(unsigned int handler);
int MdMemcpy(unsigned int dstPAddr, unsigned int srcPAddr, int size, unsigned int handler);

int MdLibExecCmd(int byCommandQueue);
int MdLibWaitCmdDone(int byCommandQueue);


//int MdLibCopy(int byCommandQueue, void *pSeDestinationProcInfo, void *pSeSourceProcInfo);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* MDOP_H_ */
