/**
/////////////////////////////////////////////////////////////////////////////
//	@file CDemuxDV.h
//
//	@brief Implementation for DV demux plug-in 
//
//	@author Rey     7/7/2005
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////
**/
#ifndef __DEMUX_DV_H__
#define __DEMUX_DV_H__

#include <Filters/NavigationFilter/NavPlugins.h>
#include "vaux.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __NAV_USE_DLL_PLUGIN__
    HRESULT FUNCNAME_OPEN_DEMUX_PLUGIN
#else
    HRESULT openDemuxPluginDV
#endif
    (
        ENUM_MEDIA_TYPE type,
        int             channelIndex,
        INPUTPLUGIN*    pInputPlugin,
        NAVDEMUXCONFIG* pDemuxConfig,
        DEMUXPLUGIN*    pDemuxPlugin
    );

#ifdef __cplusplus
}
#endif
//============= DV structure ==================//
typedef enum system_e { 
  e_dv_system_none = 0,
  e_dv_system_525_60,    // NTSC
  e_dv_system_625_50,    // PAL
} dv_system_t;

typedef enum std_e { 
  e_dv_std_none = 0,
  e_dv_std_smpte_314m,    
  e_dv_std_iec_61834,    
} dv_std_t;

typedef struct bitstream_s {
  uint32_t current_word;
  uint32_t next_word;
  uint16_t bits_left;
  uint16_t next_bits;

  uint8_t *buf;
  uint32_t buflen;
  int32_t  bufoffset;

  uint32_t (*bitstream_next_buffer) (uint8_t **,void *);
  void *priv;

  int32_t bitsread;
} bitstream_t;

typedef struct {
  int8_t sct;      // Section type (header,subcode,aux,audio,video)
  int8_t dif;      // DIF sequence number (0-12)
  int fsc;   // First (0)/Second channel (1)
  uint8_t dbn;      // DIF block number (0-134)
} dv_id_t;

typedef struct {
  bool 	dsf;   // DIF sequence flag: 525/60 (0) or 625,50 (1)
  int8_t apt;
  bool 	tf1;
  int8_t ap1;
  bool 	tf2;
  int8_t ap2;
  bool 	tf3;
  int8_t ap3;
} dv_header_t;

typedef enum sample_e { 
  e_dv_sample_none = 0,
  e_dv_sample_411,
  e_dv_sample_420,
  e_dv_sample_422,
} dv_sample_t;

typedef struct {
  dv_aaux_as_t      aaux_as, /* low-level audio format info direct from the stream */
                    aaux_as1;
  dv_aaux_asc_t     aaux_asc,
                    aaux_asc1;
  int              samples_this_frame;
  int              raw_samples_this_frame [2];
  int              quantization;
  int              max_samples;
  int              frequency;
  int              num_channels;
  int              raw_num_channels;
  int              emphasis;
  int              arg_audio_emphasis;
  int              arg_audio_frequency;
  int              arg_audio_quantization;

  int              new_recording_on_next_frame;
  int              new_recording_current_time_stamp [4],
                   arg_mixing_level;

  /* -------------------------------------------------------------------------
   * audio error correction handling
   */
  int              correction_method,
                   block_failure,
                   sample_failure,
                   real_samples_this_frame,
                   fail_samples_this_frame;
  /* -------------------------------------------------------------------------
   * memorize deemphasis values
   */
  short            lastin [4];
  double           lastout [4];
} dv_audio_t;

typedef struct {
  unsigned int              quality;
  int               arg_block_quality; // default 3
  int               arg_monochrome;

} dv_video_t;

typedef struct dv_decoder_s {
  unsigned int       quality;
  dv_system_t        system;
  dv_std_t           std;
  dv_sample_t        sampling;
  int                  num_dif_seqs; // DIF sequences per frame
  int                  num_super_macro;
  int                  height, width;
  int             		frame_size;
  dv_header_t        header;
  dv_audio_t        *audio;
  dv_video_t        *video;
  int                arg_video_system;
  int                add_ntsc_setup;
  int                clamp_luma;
  int                clamp_chroma;
  int                prev_frame_decoded;
  /* -------------------------------------------------------------------------
   * per dif sequence! there are 45 vaux data packs 1 byte header 4 byte data.
   */
  uint8_t             vaux_next;
  uint8_t             vaux_pack [256];
  uint8_t             vaux_data [45][4];

  uint8_t             ssyb_next;
  uint8_t             ssyb_pack [256];
  uint8_t             ssyb_data [12][4];
  bitstream_t        *bs;

} dv_decoder_t;
typedef struct dv_subcode_s{
	uint8_t             ssyb_next;
	uint8_t             ssyb_prev_pack;
	uint8_t             ssyb_pack [256];
	uint8_t             ssyb_data [12][4];
}dv_subcode_t;

typedef struct dv_vaux_s{
	uint8_t             vaux_next;
	uint8_t             vaux_pack [256];
	uint8_t             vaux_data [45][4];
}dv_vaux_t;

typedef struct dv_aaux_s{
	uint8_t             aaux_next;
	uint8_t             aaux_pack [256];
	uint8_t             aaux_data [9][4];
}dv_aaux_t;

#endif /*__DEMUX_DV_PROGRAM_H__*/
