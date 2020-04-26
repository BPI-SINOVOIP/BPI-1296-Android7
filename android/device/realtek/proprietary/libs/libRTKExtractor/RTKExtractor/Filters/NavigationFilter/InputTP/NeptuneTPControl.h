//
// NeptuneTPControl - declaration of control interface to Neptune TP module
//
// Note: 1. currently based on spec "transportneptune.doc", revision 1.17 (9/12/2006)
//

#ifndef __NEPTUNE_TP_CONTROL_H_09072006__
#define __NEPTUNE_TP_CONTROL_H_09072006__

#include <assert.h>

void DumpAllRegisters();
//
// Context
//

typedef struct _tagTP_CONTEXT
{
    unsigned char tp_index;
    unsigned char pid_partition;
    unsigned char sec_partition;
    unsigned char buf_partition;

} TP_CONTEXT;

//
// INFO processing definitions/inline-functions
//

typedef enum _tagTP_INFO_FORMAT
{
    TP_INFO  = 0,
    AF_INFO  = 1,
    SEC_INFO = 2

} TP_INFO_FORMAT;

inline TP_INFO_FORMAT TP_GetInfoFormat(unsigned char *p)
{
    return (TP_INFO_FORMAT)(p[0] >> 6);
}

inline unsigned int TP_GetInfoPointer(unsigned char *p)
{
    unsigned int ret = p[0] & 0x7;
    ret = (ret << 8) + p[1];
    ret = (ret << 8) + p[2];
    ret = (ret << 8) + p[3];
    return ret;
}

#define TP_INFO_TP_SYNC_ERROR         (1 << 0)
#define TP_INFO_TP_TRANSPORT_ERROR    (1 << 1)
#define TP_INFO_TP_AF_LEN_ILLEGAL     (1 << 2)
#define TP_INFO_TP_CC_DISCONTI        (1 << 3)
#define TP_INFO_TP_PSI_SCRAM_ERROR    (1 << 4)
#define TP_INFO_TP_POINTER_ILLEGAL    (1 << 5)
#define TP_INFO_TP_DUPLICATED         (1 << 6)
#define TP_INFO_TP_SEC_INCONTI        (1 << 7)

inline unsigned int TP_GetTPInfo_Error(unsigned char *p)
{
    return p[7];
}

inline unsigned int TP_GetAFInfo_Timestamp(unsigned char *p)
{
    unsigned int ret = p[4];
    ret = (ret << 8) + p[5];
    ret = (ret << 8) + p[6];
    ret = (ret << 8) + p[7];
    return ret;
}

#define TP_INFO_SEC_TAIL_FOUND            (1 << 16)
#define TP_INFO_SEC_TAIL_CRC_ERROR        (1 << 17)
#define TP_INFO_SEC_TAIL_SEC_TOO_SHORT    (1 << 18)
#define TP_INFO_SEC_TAIL_SEC_TRUNCATED    (1 << 19)
#define TP_INFO_SEC_HEAD_FOUND            (1 << 0)
#define TP_INFO_SEC_HEAD_UNWANTED         (1 << 1)
#define TP_INFO_SEC_HEAD_BY_PASS          (1 << 2)

inline unsigned int TP_GetSecInfo_Status(unsigned char *p)
{
    unsigned int ret = p[5];
    ret = (ret << 16) | p[7];
    return ret;
}

inline unsigned int TP_GetSecInfo_MatchedSecIndex(TP_CONTEXT* ctx, unsigned char *p)
{
    unsigned int ret = p[6] & 0x3F;
    if(ctx->tp_index == 1)
        ret -= ctx->sec_partition;

    assert(ret >= 0 && ret < (unsigned)((ctx->tp_index==0)? ctx->sec_partition : 64-ctx->sec_partition));

    return ret;
}

//
// API
//

#ifdef __cplusplus
extern "C" {
#endif

// ********* init/destroy ********* //

void TP_Init(TP_CONTEXT* ctx,
             char tp_index,       // transport interface index (0 or 1)
             char pid_partition,  // the 1st PID table entry for 2nd interface, 0~63 (default: 32)
             char sec_partition,  // the 1st section table entry for 2nd interface, 0~63
             char buf_partition); // the 1st ring-buffer table entry for 2nd interface, 0~63

void TP_Destroy(TP_CONTEXT* ctx);

// ********* per interface general settings ********* //

void TP_SetMemToMemMode      (TP_CONTEXT* ctx, bool bEnable); // true: memory-to-memory TP;  false: data from external interface
void TP_SetTsDescramble      (TP_CONTEXT* ctx, bool bEnable); // true: enable transport packet descrambling;  false: disable
void TP_SetExtractTimestamp  (TP_CONTEXT* ctx, bool bEnable); // true: extract timestamp from 1394 packet;  false: use internal clock
void TP_SetTimestampFreq     (TP_CONTEXT* ctx, char freq);    // 0: 27MHz;  1: 90KHz  (used only when ExtractTimestamp is false)
void TP_SetChangeTscToZero   (TP_CONTEXT* ctx, bool bEnable); // true: change transport-scrambling-control to '00' after descrambling
void TP_SetPIDFilter         (TP_CONTEXT* ctx, bool bEnable); // true: enable PID filtering;  false: disable
void TP_SetResetMachine      (TP_CONTEXT* ctx, bool bEnable); // true: reset state machine;  false: normal operating state

void TP_SetDiscardDuplicated (TP_CONTEXT* ctx, bool bEnable); // true: discard duplicated packet;  false: keep
void TP_SetDiscardNull       (TP_CONTEXT* ctx, bool bEnable); // true: discard null packet;  false: keep
void TP_SetDiscardTsError    (TP_CONTEXT* ctx, bool bEnable); // true: discard transport error packet;  false: keep
void TP_SetDiscardSyncError  (TP_CONTEXT* ctx, bool bEnable); // true: discard sync byte error packet;  false: keep

void TP_SetCRC32InitValue    (TP_CONTEXT* ctx, unsigned int initValue);

void TP_SetTsPacketCounter     (TP_CONTEXT* ctx, unsigned int count);
void TP_SetTsPacketDropCounter (TP_CONTEXT* ctx, unsigned int count);
void TP_SetTsPacketErrCounter  (TP_CONTEXT* ctx, unsigned int count);

unsigned int TP_GetTsPacketCounter     (TP_CONTEXT* ctx);
unsigned int TP_GetTsPacketDropCounter (TP_CONTEXT* ctx);
unsigned int TP_GetTsPacketErrCounter  (TP_CONTEXT* ctx);

void TP_ConfigFramer(TP_CONTEXT* ctx, bool bEnable,     // true: enable framer;  false: disable
                                      char sync_byte,   // should use default value 0x47
                                      char dropno,      // should use 1~16 (default: 1)
                                      char lockno,      // should use 1~16 (default: 3)
                                      char packet_size, // 0: 188;  1: 192;  2: 204 bytes
                                      char data_order,  // 0: big-endian;  1: little-endian
                                      char forcedrop,   // 1: force dropping all frames;  0: disable
                                      char syncmode,    // 0~16: packetize by sync signal, first byte at byte n (0~16)
                                                        // 31:   packetize by sync byte, HW search for the first byte
                                      char serial,      // 0: parallel input;  1: serial input
                                      char datapin,     // 0: serial input in big-endian;  1: little-endian
                                      char err_pol,     // 0: active high;  1: active low
                                      char sync_pol,    // 0: active high;  1: active low
                                      char val_pol,     // 0: active high;  1: active low
                                      char clk_pol);    // 0: data latching on rising edge;  1: falling edge

void TP_SetInterruptState_Overflow (TP_CONTEXT* ctx, bool bSet); // true: set;  false: reset
void TP_SetInterruptState_Drop     (TP_CONTEXT* ctx, bool bSet); // true: set;  false: reset
void TP_SetInterruptState_Sync     (TP_CONTEXT* ctx, bool bSet); // true: set;  false: reset
bool TP_GetInterruptState_Overflow (TP_CONTEXT* ctx);
bool TP_GetInterruptState_Drop     (TP_CONTEXT* ctx);
bool TP_GetInterruptState_Sync     (TP_CONTEXT* ctx);
void TP_EnableInterrupt_Overflow   (TP_CONTEXT* ctx, bool bEnable); // true: enable;  false: disable
void TP_EnableInterrupt_Drop       (TP_CONTEXT* ctx, bool bEnable); // true: enable;  false: disable
void TP_EnableInterrupt_Sync       (TP_CONTEXT* ctx, bool bEnable); // true: enable;  false: disable

// ********* DES settings ********* //

void TP_ConfigDES(TP_CONTEXT* ctx, char key_num_for_mode11,   // use key 0 or 1 when transport_scrambling_control == 11
                                   char key_num_for_mode10,   // use key 0 or 1 when transport_scrambling_control == 10
                                   char key_num_for_mode01,   // use key 0 or 1 when transport_scrambling_control == 01
                                   char des_mode,             // 0: CBC mode;  1: ECB mode
                                   char bit_strength,         // 0: 56 bits single DES;  1: 168 bits triple DES
                                   unsigned int key0_A_msb,   // 24 bits MSB, key 0 part A
                                   unsigned int key0_A_lsb,   // 32 bits LSB, key 0 part A
                                   unsigned int key0_B_msb,   // 24 bits MSB, key 0 part B
                                   unsigned int key0_B_lsb,   // 32 bits LSB, key 0 part B
                                   unsigned int key0_C_msb,   // 24 bits MSB, key 0 part C
                                   unsigned int key0_C_lsb,   // 32 bits LSB, key 0 part C
                                   unsigned int key0_IV_msb,  // 32 bits MSB, key 0 initialization vector
                                   unsigned int key0_IV_lsb,  // 32 bits LSB, key 0 initialization vector
                                   unsigned int key1_A_msb,   // 24 bits MSB, key 1 part A
                                   unsigned int key1_A_lsb,   // 32 bits LSB, key 1 part A
                                   unsigned int key1_B_msb,   // 24 bits MSB, key 1 part B
                                   unsigned int key1_B_lsb,   // 32 bits LSB, key 1 part B
                                   unsigned int key1_C_msb,   // 24 bits MSB, key 1 part C
                                   unsigned int key1_C_lsb,   // 32 bits LSB, key 1 part C
                                   unsigned int key1_IV_msb,  // 32 bits MSB, key 1 initialization vector
                                   unsigned int key1_IV_lsb); // 32 bits LSB, key 1 initialization vector

// ********* PID/section filters ********* //

void TP_AddPIDFilter(TP_CONTEXT* ctx, char pid_index,   // PID table index relative to pid_partition base (0 ~ pid_partition_size-1)
                                      unsigned int PID, // 13 bits PID
                                      char buf_index,   // ring buffer index relative to buf_partition base (0 ~ buf_partition_size-1)
                                      char ifo_index,   // info ring buffer index relative to buf_partition base (0 ~ buf_partition_size-1)
                                      char sec_index,   // section table index relative to sec_partition base (0 ~ sec_partition_size-1)
                                                        // negative (say, -1) means section filters not used
                                      bool bCheckCC,    // true: check continuity counter;  false: ignore
                                      bool bUseTPInfo,  // true: enable TP info;  false: disable
                                      bool bUseSecInfo, // true: enable section info;  false: disable
                                      bool bTrackAF);   // true: enable AF tracking;  false: disable

void TP_RemovePIDFilter(TP_CONTEXT* ctx, char pid_index); // PID table index relative to pid_partition base (0 ~ pid_partition_size-1)

void TP_SetSecFilter(TP_CONTEXT* ctx, char sec_index,                // section table index relative to sec_partition base (0 ~ sec_partition_size-1)
                                      unsigned int filter_value_msb, // match target (byte 0 3 4 5, MSB is byte 0) of section header
                                      unsigned int filter_value_lsb, // match target (byte 6 7 8 9, MSB is byte 6) of section header
                                      unsigned int mask_value_msb,   // match mask (byte 0 3 4 5, MSB is byte 0) of section header
                                      unsigned int mask_value_lsb,   // match mask (byte 6 7 8 9, MSB is byte 6) of section header
                                      unsigned char sp_filter,       // match target (2 bits, bit 1: section_syntax_ind, bit 0: private_ind)
                                      unsigned char sp_mask,         // match mask (2 bits, bit 1: section_syntax_ind, bit 0: private_ind)
                                      char next_sec_index,           // section table index relative to sec_partition base (0 ~ sec_partition_size-1)
                                                                     // negative (say, -1) means it's the last section filter (no next section)
                                      bool bCheckCRC,                // true: enable CRC32 check;  false: disable
                                      bool bMatchWillPass,           // true: match will pass;  false: match will drop
                                      bool bDefaultPass);            // true: default pass;  false: default drop (for last section filter only)

// ********* ring buffer control ********* //

void TP_SetRingBuffer(TP_CONTEXT* ctx, char buf_index,           // ring buffer index relative to buf_partition base (0 ~ buf_partition_size-1)
                                       unsigned int ring_base,   // 27 bits address, 8-byte aligned. Address of the first accessable byte.
                                       unsigned int ring_limit); // 27 bits address, 8-byte aligned. Address of the last accessable byte + 1.

void TP_GetRingBufferStatus(TP_CONTEXT* ctx, char buf_index,     // ring buffer index relative to buf_partition base (0 ~ buf_partition_size-1)
                                             unsigned int* pRP,  // to return 27 bits read pointer
                                             unsigned int* pWP); // to return 27 bits write pointer

void TP_UpdateRingBufferRP(TP_CONTEXT* ctx, char buf_index,   // ring buffer index relative to buf_partition base (0 ~ buf_partition_size-1)
                                            unsigned int RP); // update 27 bits read pointer

void TP_SetRingFullThreshold  (TP_CONTEXT* ctx, unsigned int occupancy); // 24 bits, number of bytes in buffer (to notify buffer full)
void TP_SetRingAvailThreshold (TP_CONTEXT* ctx, unsigned int occupancy); // 24 bits, number of bytes in buffer (to notify data available)

void TP_SetInterruptState_RingFull  (TP_CONTEXT* ctx, char buf_index, // ring buffer index relative to buf_partition base (0 ~ buf_partition_size-1)
                                                      bool bSet);     // true: set;  false: reset
void TP_SetInterruptState_RingAvail (TP_CONTEXT* ctx, char buf_index, // ring buffer index relative to buf_partition base (0 ~ buf_partition_size-1)
                                                      bool bSet);     // true: set;  false: reset

bool TP_GetInterruptState_RingFull  (TP_CONTEXT* ctx, char buf_index); // ring buffer index relative to buf_partition base (0 ~ buf_partition_size-1)
bool TP_GetInterruptState_RingAvail (TP_CONTEXT* ctx, char buf_index); // ring buffer index relative to buf_partition base (0 ~ buf_partition_size-1)

void TP_EnableInterrupt_RingFull  (TP_CONTEXT* ctx, char buf_index, // ring buffer index relative to buf_partition base (0 ~ buf_partition_size-1)
                                                    bool bEnable);  // true: enable;  false: disable
void TP_EnableInterrupt_RingAvail (TP_CONTEXT* ctx, char buf_index, // ring buffer index relative to buf_partition base (0 ~ buf_partition_size-1)
                                                    bool bEnable);  // true: enable;  false: disable

// ********* M2M control *********

void TP_SetM2MRingBuffer(TP_CONTEXT* ctx, unsigned int ring_base,   // 27 bits address, 8-byte aligned. Address of the first accessable byte.
                                          unsigned int ring_limit); // 27 bits address, 8-byte aligned. Address of the last accessable byte + 1.

void TP_SetM2MGo(TP_CONTEXT* ctx);
void TP_GetM2MRingBufferStatus(TP_CONTEXT* ctx, unsigned int* pRP,  // to return 27 bits read pointer
                                                unsigned int* pWP); // to return 27 bits write pointer

void TP_UpdateM2MRingBufferWP(TP_CONTEXT* ctx, unsigned int WP); // update 27 bits write pointer

void TP_StartStopM2MTransfer(TP_CONTEXT* ctx, bool bStart); // true: start;  false: stop

void TP_SetInterruptState_M2M_Empty (TP_CONTEXT* ctx, bool bSet);    // true: set;  false: reset
bool TP_GetInterruptState_M2M_Empty (TP_CONTEXT* ctx);
void TP_EnableInterrupt_M2M_Empty   (TP_CONTEXT* ctx, bool bEnable); // true: enable;  false: disable

// ********* Debug *********

void TP_DebugSetDebugMode      (TP_CONTEXT* ctx, char mode); // user defined (0~15)
void TP_DebugPrintAllRegisters (TP_CONTEXT* ctx);

#ifdef __cplusplus
}
#endif

#endif
