/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <media/stagefright/MediaDefs.h>

namespace android {

const char *MEDIA_MIMETYPE_IMAGE_JPEG = "image/jpeg";

const char *MEDIA_MIMETYPE_VIDEO_VP8 = "video/x-vnd.on2.vp8";
const char *MEDIA_MIMETYPE_VIDEO_VP9 = "video/x-vnd.on2.vp9";
const char *MEDIA_MIMETYPE_VIDEO_AVC = "video/avc";
const char *MEDIA_MIMETYPE_VIDEO_HEVC = "video/hevc";
const char *MEDIA_MIMETYPE_VIDEO_MPEG4 = "video/mp4v-es";
const char *MEDIA_MIMETYPE_VIDEO_H263 = "video/3gpp";
const char *MEDIA_MIMETYPE_VIDEO_MPEG2 = "video/mpeg2";
const char *MEDIA_MIMETYPE_VIDEO_RAW = "video/raw";
const char *MEDIA_MIMETYPE_VIDEO_DOLBY_VISION = "video/dolby-vision";
const char *MEDIA_MIMETYPE_VIDEO_VC1 = "video/vc1";
const char *MEDIA_MIMETYPE_VIDEO_RV = "video/vnd.rn-realvideo";
const char *MEDIA_MIMETYPE_VIDEO_DIVX3 = "video/divx3";
const char *MEDIA_MIMETYPE_VIDEO_VP6 = "video/vp6";
const char *MEDIA_MIMETYPE_VIDEO_AVS = "video/avs-video";
const char *MEDIA_MIMETYPE_VIDEO_MJPEG = "video/x-motion-jpeg";
const char *MEDIA_MIMETYPE_VIDEO_WVC1 = "video/wvc1";
const char *MEDIA_MIMETYPE_VIDEO_WMV3 = "video/wmv3";
const char *MEDIA_MIMETYPE_VIDEO_WMV = "video/x-ms-wmv";
const char *MEDIA_MIMETYPE_VIDEO_MP43 = "video/mp43";
const char *MEDIA_MIMETYPE_VIDEO_FLV = "video/x-flv";       // sorenson spark
const char *MEDIA_MIMETYPE_VIDEO_RV30 = "video/rv30";
const char *MEDIA_MIMETYPE_VIDEO_RV40 = "video/rv40";

const char *MEDIA_MIMETYPE_AUDIO_AMR_NB = "audio/3gpp";
const char *MEDIA_MIMETYPE_AUDIO_AMR_WB = "audio/amr-wb";
const char *MEDIA_MIMETYPE_AUDIO_MPEG = "audio/mpeg";
const char *MEDIA_MIMETYPE_AUDIO_MPEG_LAYER_I = "audio/mpeg-L1";
const char *MEDIA_MIMETYPE_AUDIO_MPEG_LAYER_II = "audio/mpeg-L2";
const char *MEDIA_MIMETYPE_AUDIO_MIDI = "audio/midi";
const char *MEDIA_MIMETYPE_AUDIO_AAC = "audio/mp4a-latm";
const char *MEDIA_MIMETYPE_AUDIO_QCELP = "audio/qcelp";
const char *MEDIA_MIMETYPE_AUDIO_VORBIS = "audio/vorbis";
const char *MEDIA_MIMETYPE_AUDIO_OPUS = "audio/opus";
const char *MEDIA_MIMETYPE_AUDIO_G711_ALAW = "audio/g711-alaw";
const char *MEDIA_MIMETYPE_AUDIO_G711_MLAW = "audio/g711-mlaw";
const char *MEDIA_MIMETYPE_AUDIO_RAW = "audio/raw";
const char *MEDIA_MIMETYPE_AUDIO_FLAC = "audio/flac";
const char *MEDIA_MIMETYPE_AUDIO_AAC_ADTS = "audio/aac-adts";
const char *MEDIA_MIMETYPE_AUDIO_AAC_LATM = "audio/aac-latm";
const char *MEDIA_MIMETYPE_AUDIO_AC3 = "audio/ac3";
const char *MEDIA_MIMETYPE_AUDIO_EAC3 = "audio/eac3";
const char *MEDIA_MIMETYPE_AUDIO_DDP = "audio/eac3";
const char *MEDIA_MIMETYPE_AUDIO_DTS = "audio/dts";
const char *MEDIA_MIMETYPE_AUDIO_DTS_HD = "audio/dts-hd";
const char *MEDIA_MIMETYPE_AUDIO_WMA = "audio/wma";
const char *MEDIA_MIMETYPE_AUDIO_WMAPRO = "audio/wmapro";
const char *MEDIA_MIMETYPE_AUDIO_RA_COOK = "audio/ra-cook";
const char *MEDIA_MIMETYPE_AUDIO_RA_LSD = "audio/ra-lsd";
const char *MEDIA_MIMETYPE_AUDIO_RA_RA1 = "audio/ra-ra1";
const char *MEDIA_MIMETYPE_AUDIO_DV = "audio/dv";
const char *MEDIA_MIMETYPE_AUDIO_MLP = "audio/mlp";
const char *MEDIA_MIMETYPE_AUDIO_SILK = "audio/silk";
const char *MEDIA_MIMETYPE_AUDIO_G729 = "audio/g729";
const char *MEDIA_MIMETYPE_AUDIO_APE = "audio/ape";
const char *MEDIA_MIMETYPE_AUDIO_ADPCM = "audio/adpcm";
const char *MEDIA_MIMETYPE_AUDIO_ALAC = "audio/alac";
const char *MEDIA_MIMETYPE_AUDIO_MSGSM = "audio/gsm";

const char *MEDIA_MIMETYPE_CONTAINER_MPEG4 = "video/mp4";
const char *MEDIA_MIMETYPE_CONTAINER_WAV = "audio/x-wav";
const char *MEDIA_MIMETYPE_CONTAINER_OGG = "application/ogg";
const char *MEDIA_MIMETYPE_CONTAINER_MATROSKA = "video/x-matroska";
const char *MEDIA_MIMETYPE_CONTAINER_MPEG2TS = "video/mp2ts";
const char *MEDIA_MIMETYPE_CONTAINER_AVI = "video/avi";
const char *MEDIA_MIMETYPE_CONTAINER_MPEG2PS = "video/mp2p";

const char *MEDIA_MIMETYPE_CONTAINER_WVM = "video/wvm";
const char *MEDIA_MIMETYPE_CONTAINER_RTK = "video/rtk";

const char *MEDIA_MIMETYPE_TEXT_3GPP = "text/3gpp-tt";
const char *MEDIA_MIMETYPE_TEXT_SUBRIP = "application/x-subrip";
const char *MEDIA_MIMETYPE_TEXT_VTT = "text/vtt";
const char *MEDIA_MIMETYPE_TEXT_CEA_608 = "text/cea-608";
const char *MEDIA_MIMETYPE_TEXT_CEA_708 = "text/cea-708";
const char *MEDIA_MIMETYPE_DATA_TIMED_ID3 = "application/x-id3v4";

#ifdef RTK_AUDIO_EXTRACTOR_EN
const char *MEDIA_MIMETYPE_AUDIO_ALAC_CAFF = "audio/alac_caff";
#endif

#ifdef RTK_PLATFORM
const char *MEDIA_MIMETYPE_CONTAINER_WMV = "video/x-ms-asf";
const char *MEDIA_MIMETYPE_CONTAINER_AIFF = "audio/x-aiff";
const char *MEDIA_MIMETYPE_AUDIO_PCM = "audio/x-pcm";
const char *MEDIA_MIMETYPE_CONTAINER_FLV = "video/x-flv";
const char *MEDIA_MIMETYPE_AUDIO_DSD = "audio/dsd-dsf";
const char *MEDIA_MIMETYPE_AUDIO_AAC_MAIN = "audio/aac-main";
const char *MEDIA_MIMETYPE_CONTAINER_HEIF = "image/heif";
#endif

}  // namespace android
