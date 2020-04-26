#ifndef __FLAC_H
#define FLAC_PARSE_SEEKTBL_FOUND      1
#define FLAC_PARSE_SEEKTBL_NOT_FOUND  2
#define FLAC_PARSE_OK         3
#define FLAC_PARSE_FAIL       4

#define FLAC_METADATA_TYPE_STREAMINFO  0
#define FLAC_METADATA_TYPE_SEEKTABLE   3

#define FLAC__STREAM_METADATA_SEEKPOINT_PLACEHOLDER -1

/* Size(in bytes) of one seek point */
#define FLAC_SEEKPOINT_SIZE 18

typedef struct 
{
  unsigned long long sample_number;
  unsigned long long stream_offset;
  unsigned long frame_samples;   
}FLAC_SEEKPOINT;

typedef struct 
{
  unsigned long num_points;
  FLAC_SEEKPOINT *points;    
}FLAC_SEEKTBL;

typedef struct 
{
  unsigned long nch;
  unsigned long bps;  
  unsigned long sample_rate;  
  unsigned long max_blocksize;
  unsigned long min_blocksize;  
  unsigned long min_frmsize;
  unsigned long max_frmsize;

  unsigned long long total_samples;
  
  FLAC_SEEKTBL seektbl;

}FLACINFO;
#endif
