#ifndef BD_MPLS_SUB_PATH_H
#define BD_MPLS_SUB_PATH_H

typedef struct
{
    char            clip_id[6];
    uint8_t         connection_condition;
    uint8_t         stc_id;
    uint32_t        in_time;
    uint32_t        out_time;
    uint16_t		sync_pi;		//sync PlayItem id
    uint32_t		sync_pts;		//sync start PTS of PlayItem
    bool			is_multi_clips;

    // Extrapolated items
    uint32_t        abs_start;
    uint32_t        abs_end;
} bdt_spi_t;

typedef struct
{
    uint8_t 		type;			//SubPath type
    bool			is_repeat;		//is repeat SubPath
    uint8_t 		spi_count;		//number of SubPlayItems
    bdt_spi_t*		spi;			//sub play items
} bdt_sub_path_t;

typedef struct
{
	uint8_t	 type;
	uint16_t pid;
	uint8_t  subpath_id;
	uint8_t  subclip_id;
} bdt_stream_entry_t;


typedef struct
{
	uint8_t		offset_seq_id;
	bool		dialog_region_offset_valid;
	bool		is_ss;
	bool		is_top_as;
	bool		is_bottom_as;
	struct {
		bdt_stream_entry_t left;
		bdt_stream_entry_t right;
		uint8_t offset_seq_id;
	} ss_pg;
	
	struct {
		bdt_stream_entry_t top;
		uint8_t offset_seq_id;
	} top_as_pg;
	
	struct {
		bdt_stream_entry_t bottom;
		uint8_t offset_seq_id;
	} bottom_as_pg;
} bdt_pg_stream_ss;


typedef struct
{
	int  num_pg;
	bdt_pg_stream_ss* pg;
	
} bdt_stn_ss_t;

bool parse_subpath(FILE *stream, bool bExt, bdt_sub_path_t *sub_path);
bool parse_extension_data(FILE *stream, uint32_t ext_pos, void *data);

#endif //BD_MPLS_SUB_PATH_H
