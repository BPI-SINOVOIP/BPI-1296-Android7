/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/*
** This file implements the ability to send packets on the wire to a WMDRMNET
** transmitter (server).
**
** The .c file itself isn't shipped as a part of the reference port because
** the wire protocol doesn't yet have a reference port.
**
** The .h file ships to define the wire abstraction.
*/

#ifndef _WIRE_H_
#define _WIRE_H_

#ifdef  __cplusplus
extern "C" {
#endif


/*****************************************************************************
** Function:    WireOpen
**
** Synopsis:    Opens a connection to the wire layer.
**
** Arguments:   [f_ppWireHandle] : Returns a handle to the wire interface
**
** Returns:     DRM_SUCCESS - on success
**              or other return code from a function called within
**
** Notes:       The returned handle must be closed using WireClose().
**
******************************************************************************/
DRM_RESULT DRM_CALL WireOpen(
    __out DRM_VOID **f_ppWireHandle
);



/*****************************************************************************
** Function:    WireSendReceiveMessage
**
** Synopsis:    Sends a WmdrmNet message on the wire to the transmitter and returns a response message.
**
** Arguments:   [f_pWireHandle]            : The handle returned from WireOpen
**              [f_pbSentMessage]          : The message to send
**              [f_cbSentMessageSize]      : The size (in bytes) of the message to send
**              [f_ppbReceivedMessage]     : The response message from the transmitter
**              [f_pcbReceivedMessageSize] : The size (in bytes) of the response message from the transmitter
**
** Returns:     DRM_SUCCESS - on success
**              or other return code from a function called within
**
** Notes:       None.
**
******************************************************************************/
DRM_RESULT DRM_CALL WireSendReceiveMessage(
    __in DRM_VOID                                           *f_pWireHandle,
    __in_bcount(f_cbSentMessageSize) DRM_BYTE               *f_pbSentMessage,
    __in const DRM_DWORD                                     f_cbSentMessageSize,
    __deref_out_bcount(*f_pcbReceivedMessageSize) DRM_BYTE **f_ppbReceivedMessage,
    __out DRM_DWORD                                         *f_pcbReceivedMessageSize
);



/*****************************************************************************
** Function:    WireSendReceiveMessageTo
**
** Synopsis:    Sends a WmdrmNet message on the wire to the transmitter using the specified address 
**              and returns a response message.
**
** Arguments:   [f_pWireHandle]            : The handle returned from WireOpen
**              [f_pbAddress]              : The address to send the message to
**              [f_cbAddress]              : The size (in bytes) of the address
**              [f_pbSentMessage]          : The message to send
**              [f_cbSentMessageSize]      : The size (in bytes) of the message to send
**              [f_ppbReceivedMessage]     : The response message from the transmitter
**              [f_pcbReceivedMessageSize] : The size (in bytes) of the response message from the transmitter
**
** Returns:     DRM_SUCCESS - on success
**              or other return code from a function called within
**
** Notes:       None.
**
******************************************************************************/
DRM_RESULT DRM_CALL WireSendReceiveMessageTo(
    __in DRM_VOID                                           *f_pWireHandle,
    __in_bcount(f_cbAddress) DRM_BYTE                       *f_pbAddress,
    __in const DRM_DWORD                                     f_cbAddress,
    __in_bcount(f_cbSentMessageSize) DRM_BYTE               *f_pbSentMessage,
    __in const DRM_DWORD                                     f_cbSentMessageSize,
    __deref_out_bcount(*f_pcbReceivedMessageSize) DRM_BYTE **f_ppbReceivedMessage,
    __out DRM_DWORD                                         *f_pcbReceivedMessageSize
);



/*****************************************************************************
** Function:    WireSendReceiveMessageTo
**
** Synopsis:    Closes the connection to the wire layer
**
** Arguments:   [f_pWireHandle] : The handle returned from WireOpen
**
** Returns:     DRM_SUCCESS - on success
**              or other return code from a function called within
**
** Notes:       None.
**
******************************************************************************/
DRM_RESULT DRM_CALL WireClose(
    __in DRM_VOID *f_pWireHandle
);

#ifdef  __cplusplus
}
#endif

#endif /*_WIRE_H_ */
