//-----------------------------------------------------------------------------
// HDMVDecoder_TSParserUtils.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __HDMVDECODER_TSPARSERUTILS_H__
#define __HDMVDECODER_TSPARSERUTILS_H__


//___________________________________________________________________________________________________________________________________________________
//
// BDROM TS Packet and Payload (PES) Parsing functions
//

namespace BDROM_TSParserUtils
{

	//-----------------------------------------------------------------------------------
	/// @name BDROM MPEG-2 TS Packet Parsing functions - BDROM TS Header Parsing
	/// @brief input pointer should point to the first byte of a BDROM TS packet
	//@{
	//
	inline UInt32	bdts__copy_permission_indicator(UInt8 * pBDTSPkt)	{ return (pBDTSPkt[0] & 0xC0) >> 6;	}
	inline UInt32	bdts__arrival_time_stamp(UInt8 * pBDTSPkt)			{ return ((pBDTSPkt[0] & 0x3F) << 24) | (pBDTSPkt[1] << 16) | (pBDTSPkt[2] << 8) | pBDTSPkt[3];	}
	//@}

	//-----------------------------------------------------------------------------------
	/// @name BDROM MPEG-2 TS Packet Parsing functions - BDROM TS Header Parsing
	/// @brief input pointer should point to the first byte of a TS packet
	//@{
	//
	inline bool		ts__transport_error(UInt8 * pTSPkt)					{ return (pTSPkt[1] & 0x80) == 0x80;							}
	inline bool		ts__payload_unit_start(UInt8 * pTSPkt)				{ return (pTSPkt[1] & 0x40) == 0x40;							}
	inline bool		ts__has_transport_error(UInt8 * pTSPkt)				{ return (pTSPkt[1] & 0x80) == 0x80;							}
	inline UInt16	ts__PID(UInt8 * pTSPkt)								{ return ((pTSPkt[1] & 0x1f) << 8) | pTSPkt[2];					}
	inline void		ts__set_PID(UInt8 * pTSPkt, UInt16 pid)				{ pTSPkt[1] = (UInt8)((pTSPkt[1] & 0xE0) | ((pid >> 8) & 0x1F));
																		  pTSPkt[2] = (UInt8)(pid & 0xFF);
																		}
    inline UInt8    ts__adaptation_field_control(UInt8 * pTSPkt)		{ return (pTSPkt[3] & 0x30) >> 4;								}
	inline bool		ts__has_adaptation_field(UInt8 * pTSPkt)			{ return (pTSPkt[3] & 0x20) == 0x20;							}
	inline bool		ts__has_payload(UInt8 * pTSPkt)						{ return (pTSPkt[3] & 0x10) == 0x10;							}
    inline UInt8	ts__continuity_counter(UInt8 * pTSPkt)				{ return pTSPkt[3] & 0x0F;										}
    inline UInt8    ts__adaptation_field_length(UInt8 * pTSPkt)			{ return pTSPkt[4];												}
    inline UInt8    ts__sizeof_adaptation_field(UInt8 * pTSPkt)			{ return 1 + pTSPkt[4];											}
	//@}

	//-----------------------------------------------------------------------------------
	/// @name BDROM MPEG-2 TS Packet Parsing functions - BDROM TS Header Parsing
	/// @brief input pointer should point to the first byte of a PES header
	//@{
	//
	inline UInt8	pes__PES_header_data_length(UInt8 * pPESPkt)		{ return pPESPkt[8];											}
	inline UInt16	pes__sizeof_PES_header(UInt8 * pPESPkt)				{ return 9 + pPESPkt[8];										}
	inline UInt16	pes__PES_packet_length(UInt8 * pPESPkt)				{ return (pPESPkt[4] << 8) | pPESPkt[5];						}
	inline UInt32	pes__sizeof_PES_packet(UInt8 * pPESPkt)				{ return 6 + ((pPESPkt[4] << 8) | pPESPkt[5]);					}
	inline bool		pes__has_PTS(UInt8 * pPESPkt)						{ return (pPESPkt[7] & 0x80) == 0x80;							}
	inline bool		pes__has_DTS(UInt8 * pPESPkt)						{ return (pPESPkt[7] & 0x40) == 0x40;							}
	inline UInt64	pes__PTS(UInt8 * pPESPkt)							{ return ((pPESPkt[9]  & 0x0E) << 29) |									// bits 32..30
																				 ( pPESPkt[10]         << 22) | ((pPESPkt[11] & 0xFE) << 14) |	// bits 29..15
																				 ( pPESPkt[12]         <<  7) | ( pPESPkt[13]         >>  1);	// bits 14..0
																		}
	inline UInt64	pes__DTS(UInt8 * pPESPkt)							{ return ((pPESPkt[14] & 0x0E) << 29) |									// bits 32..30
																				 ( pPESPkt[15]         << 22) | ((pPESPkt[16] & 0xFE) << 14) |	// bits 29..15
																				 ( pPESPkt[17]         <<  7) | ( pPESPkt[18]         >>  1);	// bits 14..0
																		}
	//@}

	//-----------------------------------------------------------------------------------
	/// @name BDROM MPEG-2 TS Packet Parsing functions - BDROM HDMV LPCM PES Payload Parsing of HDMV_LPCM_audio_data_header
	/// @brief input pointer should point to the first byte of a PES Payload
	//@{
	//
	inline UInt16	pesLPCM__audio_data_payload_size(UInt8 * pPESPayld)	{ return (pPESPayld[0] << 8) | pPESPayld[1];					}
	inline UInt8	pesLPCM__channel_assignment(UInt8 * pPESPayld)		{ return ((pPESPayld[2] & 0xF0) >> 4);							}
	inline UInt8	pesLPCM__sampling_frequency(UInt8 * pPESPayld)		{ return ( pPESPayld[2] & 0x0F);								}
	inline UInt8	pesLPCM__bits_per_sample(UInt8 * pPESPayld)			{ return ((pPESPayld[3] & 0xC0) >> 6);							}
	inline UInt8	pesLPCM__start_flag(UInt8 * pPESPayld)				{ return ((pPESPayld[3] & 0x20) >> 5);							}
	//@}
} // namespace BDROM

#endif // __HDMVDECODER_TSPARSERUTILS_H__
