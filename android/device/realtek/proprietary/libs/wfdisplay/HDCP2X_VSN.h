/*
 * HDCP2X_VSN.h
 *
 *  Created on: 2013/6/10
 *      Author: bruce_huang
 */

#ifndef HDCP2X_VSN_H_
#define HDCP2X_VSN_H_

// Default HDCP version is 2.0
#define	HDCP_2X_VSN	22

#ifdef HDCP_2X_VSN_2_0
#ifdef HDCP_2X_VSN
#	undef HDCP_2X_VSN
#endif
#	define	HDCP_2X_VSN	20
#endif

#ifdef HDCP_2X_VSN_2_1
#ifdef HDCP_2X_VSN
#	undef HDCP_2X_VSN
#endif
#	define	HDCP_2X_VSN	21
#endif

#ifdef HDCP_2X_VSN_2_2
#ifdef HDCP_2X_VSN
#	undef HDCP_2X_VSN
#endif
#	define	HDCP_2X_VSN	22
#endif

#endif /* HDCP2X_VSN_H_ */
