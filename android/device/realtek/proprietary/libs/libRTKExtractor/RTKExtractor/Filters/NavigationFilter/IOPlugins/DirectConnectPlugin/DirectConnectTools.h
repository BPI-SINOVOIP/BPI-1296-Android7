///////////////////////////////////////////////////////////////////////////////
// Tools for DirectConnect.
//
// Author: zackchiu@realtek.com
//
// Created: 3/14/2011
////////////////////////////////////////////////////////////////////////////////

#ifndef __DIRECT_CONNECT_TOOLS__
#define __DIRECT_CONNECT_TOOLS__

bool ParseDirectConnectURL(const char* pURL, 
                           char* pMark = 0, 
                           unsigned int* pSeqNum = 0, 
                           unsigned long long* pSize = 0,
                           unsigned int* pKernelId = 0,
                           unsigned int* pIndex = 0,
                           unsigned int* pSubType = 0);

void PostCommandUserDefine(char chMark, 
                           int iSequenceNumber, 
                           unsigned long long iSize, 
                           unsigned int iKernelId, 
                           unsigned int iIndex = 0,
                           unsigned int iSubType = 0);

#endif
