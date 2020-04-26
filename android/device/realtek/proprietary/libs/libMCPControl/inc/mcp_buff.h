#ifndef __MCP_BUFF_H__
#define __MCP_BUFF_H__


#ifdef __cplusplus
extern "C" {
#endif

#define MCP_BUFF_HEAD_RESERVE            128
#define MCP_BUFF_TAIL_RESERVE            128
#define MCP_BUFF_RESERVE_AREA            (MCP_BUFF_HEAD_RESERVE + MCP_BUFF_TAIL_RESERVE)

typedef struct 
{    
    unsigned char*   head;
    unsigned char*   data;
    unsigned char*   tail;
    unsigned char*   end;
    unsigned long    len;
}mcp_buff;


mcp_buff*      alloc_mcpb    (unsigned int len);
void           free_mcpb     (mcp_buff* mcpb);
void           sync_mcpb     (mcp_buff* mcpb);

void           mcpb_reserve  (mcp_buff* mcpb, unsigned int len);
unsigned char* mcpb_put      (mcp_buff* mcpb, unsigned int len);
void           mcpb_trim     (mcp_buff* mcpb, unsigned int len);
unsigned char* mcpb_push     (mcp_buff* mcpb, unsigned int len);
unsigned char* mcpb_pull     (mcp_buff* mcpb, unsigned int len);
void           mcpb_purge    (mcp_buff* mcpb);
int            mcpb_headroom (mcp_buff* mcpb);
int            mcpb_tailroom (mcp_buff* mcpb);

void           mcpb_dump_data(mcp_buff* mcpb);
void           mcpb_dump_tail(mcp_buff* mcpb);

#define dump_mcpb_data(mcpb)    mcp_dump_data_with_text(mcpb->data, mcpb->len, "****************** mcpb (%p) ************************\n", mcpb)
#define dump_mcpb_tail(mcpb)    mcp_dump_data_with_text(mcpb->tail, mcpb_tailroom(mcpb), "****************** mcpb (%p) ************************\n", mcpb)

#ifdef __cplusplus
}
#endif

#endif  // __MCP_BUFF_H__
