#ifndef __MCP_AUX_H__
#define __MCP_AUX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

void mcp_dump_data(unsigned char* data, unsigned int len);

#define mcp_dump_data_with_text(data, len, fmt, args...)    do {printf(fmt, ## args); mcp_dump_data(data, len);} while(0)

#ifdef __cplusplus
}
#endif

#endif 
