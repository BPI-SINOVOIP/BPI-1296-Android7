// $Header: /Mux_Demux_SDK60/common/stream.h 1     2/14/05 11:51a Sbramwell $
//
// Copyright (c) 2003 DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.

/*************************************************************************/

#ifndef _STREAM_H
#define _STREAM_H

#include <stdio.h>
#include <string.h>

#ifdef WIN32

#ifdef _PARSERAPI
#include <StdAfx.h>
#endif

#include <windows.h>
#include <vfw.h>

#endif //WIN32

#include "portab.h"
extern "C" {
#include "aviin.h"
#include "aviout.h"
#include "video_info.h"
#include "audio_info.h"
}

#ifdef DRM_ENABLE
#include "DrmSizes.h"
#endif

#ifdef _PARSERAPI
#include "players/parserApi/ParserApiInterface.h"
#endif

#ifdef _MP4WRITER
#include "media/mpeg4FileWriter/mp4Writer/Mp4Writer.h"
#endif

#define MAX_SLEN_CODEC 64
#define STREAM_DIR_IN 1
#define STREAM_DIR_OUT 2

class divx_stream
{
public:
	int frames;
	int frame;
	int width;
	int height;
	int max_bytes;
	double rate;
	char codec[MAX_SLEN_CODEC];
	int direction;


public:

	divx_stream();
	virtual int open_read(char *path) = 0;
	virtual int open_write(char *path) = 0;
	virtual int get_video(unsigned char *data, int *bytes, int *keyframe, int framenum) = 0;
	virtual int put_video(unsigned char *data, int bytes, int keyframe) = 0;
	virtual int get_audio(unsigned char *data, int *bytes, int framenum) = 0;
	virtual int put_audio(unsigned char *data, int bytes, int samples) = 0;
	virtual ~divx_stream() = 0;
	virtual int close() = 0;
	virtual int build_index(int prefixed) { return 0; }
};

class null_stream : public divx_stream
{
public:
	int open_read(char *path);
	int open_write(char *path);
	int close();
	int get_video(unsigned char *data, int *bytes, int *keyframe, int framenum);
	int put_video(unsigned char *data, int bytes, int keyframe);
	int get_audio(unsigned char *data, int *bytes, int framenum) { return 0; }
	int put_audio(unsigned char *data, int bytes, int samples) { return 0; }
	null_stream();
	~null_stream();
};

#define MAX_TRACKS	20

class avi_stream : public divx_stream
{
	audio_info_t audio_info[MAX_TRACKS];
	video_info_t video_info[MAX_TRACKS];
	caption_info_t caption_info[MAX_TRACKS];
	uint8_t *drm_info[MAX_TRACKS];
	int audio_real_track_id[MAX_TRACKS];
	int video_real_track_id[MAX_TRACKS];
	uint8_t *vol_buffer;
	int vol_buffer_len;
	int compressed_flag;
	int audio_tracks, video_tracks;
	int drm_present;
	aviin_t *aviin;
	aviout_t *aviout;

public:
	int open_read(char *pat2h);
	int open_write(char *path);
	int close();
	int get_video(int track, unsigned char *data, int *bytes, int *keyframe, int framenum);
	int put_video(int track, unsigned char *data, int bytes, int keyframe);
	int get_audio(int track, unsigned char *data, int *bytes, int framenum);
	int put_audio(int track, unsigned char *data, int bytes, int samples);

	int get_video(unsigned char *data, int *bytes, int *keyframe, int framenum) { return(get_video(0,data,bytes,keyframe,framenum)); }
	int put_video(unsigned char *data, int bytes, int keyframe) { return(put_video(0,data,bytes,keyframe)); }
	int get_audio(unsigned char *data, int *bytes, int framenum) { return(get_audio(0,data,bytes,framenum)); } 
	int put_audio(unsigned char *data, int bytes, int samples) { return(put_audio(0,data,bytes,samples)); }

	avi_stream();
	~avi_stream();
private:
	void find_max_video_size();
};

class caption_stream
{
	int progress;

public:
	int open_read(char *path);
	int open_write(char *path);
	int close();
	int get_caption(unsigned char *data, int *bytes, int framenum);
	int put_caption(unsigned char *data, int bytes);
	int get_audio(unsigned char *data, int *bytes, int framenum) { return 1; }
	int put_audio(unsigned char *data, int bytes, int samples) { return 1; }
	caption_stream();
	~caption_stream();
};

class raw_stream : public divx_stream
{
protected:
	FILE *rawfile;
public:
	int open_read(char *path);
	int open_write(char *path);
	int close();
	int get_video(unsigned char *data, int *bytes, int *keyframe, int framenum);
	int put_video(unsigned char *data, int bytes, int keyframe);
	int get_audio(unsigned char *data, int *bytes, int framenum) { return 1; }
	int put_audio(unsigned char *data, int bytes, int samples) { return 1; }
	raw_stream();
	~raw_stream();
};

typedef struct _t_frame_index
{
	unsigned long pos;
	unsigned int frame_num;
	unsigned int frame_size;
	bool key_frame;
} t_frame_index;

class m4v_stream : public raw_stream
{
	t_frame_index *frame_index;
	unsigned int length;

public:
	int get_video(unsigned char *data, int *bytes, int *keyframe, int framenum);
	int put_video(unsigned char *data, int bytes, int keyframe);
	int get_audio(unsigned char *data, int *bytes, int framenum) { return 1; }
	int put_audio(unsigned char *data, int bytes, int samples) { return 1; }
	m4v_stream();
	~m4v_stream();

	int build_index(int prefixed);

private:
	int count_frames();
	void set_frame_size(int length);
	void create_frame_index_m4v_pb();
	void create_frame_index_m4v();


};

#endif

