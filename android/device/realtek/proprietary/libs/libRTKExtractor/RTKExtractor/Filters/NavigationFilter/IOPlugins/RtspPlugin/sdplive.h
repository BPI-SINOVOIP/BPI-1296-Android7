#ifndef __SDPLIVE_H__
#define __SDPLIVE_H__

#include "common.h"
#include <string.h>


#define MAXLONGFIELDLEN 1024
#define MLFLENS "1023"

#define MAXSHORTFIELDLEN 96
#define MSFLENS "95"

const char *get_sdplive_range (const fsdp_description_t * fsdpDes);

const fsdp_media_description_t *get_sdplive_media (const fsdp_description_t *
						fsdpDes, unsigned int index);

fsdp_media_t get_sdplive_media_type (const fsdp_media_description_t * fsdpDes);

int sdplive_parse (const char *description, fsdp_description_t * fsdpDes);

int
parse_rtpmap (fsdp_rtpmap_t *** rtpmap, unsigned int *counter,
		   const char *value);



fsdp_transport_protocol_t
get_sdplive_media_transport_protocol (const fsdp_media_description_t * fsdpDes);

unsigned int get_sdplive_media_port (const fsdp_media_description_t * fsdpDes);

fsdp_network_type_t
get_sdplive_media_network_type (const fsdp_media_description_t * fsdpDes);

fsdp_address_type_t
get_sdplive_media_address_type (const fsdp_media_description_t * fsdpDes);




fsdp_address_type_t
get_sdplive_global_conn_address_type (const fsdp_description_t * fsdpDes);


fsdp_network_type_t
get_sdplive_global_conn_network_type (const fsdp_description_t * fsdpDes);

char *get_sdplive_media_control (const fsdp_media_description_t * mdsc,
			      unsigned int index);


int
parse_c_line (const char **p, fsdp_network_type_t * ntype,
	      fsdp_address_type_t * atype,
	      fsdp_connection_address_t * address);

const char *get_sdplive_media_address (const fsdp_media_description_t * fsdpDes);

const char *get_sdplive_global_conn_address (const fsdp_description_t * fsdpDes);

int
parse_b_line (const char **p, fsdp_bw_modifier_t ** bw_modifiers,
	      unsigned int *bw_modifiers_count);


int
parse_k_line (const char **p, fsdp_encryption_method_t * method,
	      char **content);


int
sdplive_time_to_uint (const char *time, unsigned long int *seconds);





#endif
