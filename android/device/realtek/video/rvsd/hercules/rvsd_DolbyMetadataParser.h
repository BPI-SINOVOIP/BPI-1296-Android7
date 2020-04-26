#include "rvsd_maincore.h"
#include "rvsd_VDecVPU.h"
#include "rvsd_dbg.h"
#include "rpu_ext_config.h"
#include "rpu_api_common.h"
#include "rpu_api_decoder.h"
#include "dv_md_parser.h"

#undef LOG_TAG
#define LOG_TAG "DolbyMetadataParser"

typedef struct ParserOutputContext
{
    ParserOutputContext()
    {
        comp_out = NULL;
        dm_out = NULL;
        memset(&rpu_data_out_buffer, 0, sizeof(rpu_data_t));
        memset(&main_cfg, 0, sizeof(rpu_ext_config_fixpt_main_t));
        p_dm_byte_sequence = NULL;
        p_parser_handle = NULL;
    }

    ~ParserOutputContext()
    {
    }

    osal_file_t comp_out;
    osal_file_t dm_out;
    rpu_data_t rpu_data_out_buffer;
    rpu_ext_config_fixpt_main_t main_cfg;
    char *p_dm_byte_sequence;
    dv_md_parser_handle_t *p_parser_handle;
} ParserOutputContext;


class DolbyMetadataParser
{
public:
    DolbyMetadataParser(const unsigned char *const p_buffer,
                                const unsigned long size);
    ~DolbyMetadataParser();

    unsigned long PushData(const unsigned long size, const bool is_eos);
    void* DoTask();
    bool IsLastDataDone() const;

private:
    void Allocate();
    void Release();

    bool AllocateParserEngine();
    bool ReleaseParserEngine();

    void InitOutputContext();
    void UninitOutputContext();

    static void OutputCallback(void *ctx_arg,
                                    dv_md_parser_evt_t md_parser_evt,
                                    timestamp_t *p_pts,
                                    rpu_data_header_t *hdr,
                                    vdr_rpu_data_payload_t *rpu_payload,
                                    int rpu_payload_sz,
                                    vdr_dm_data_payload_t *dm_payload,
                                    int dm_payload_sz,
                                    int len);
    static void ConvertRpuToConfig(rpu_data_header_t *rh,
                                           vdr_rpu_data_payload_t *rpu_payload,
                                           rpu_ext_config_fixpt_main_t *comp_cfg);
    static int ConvertDmToConfig(vdr_dm_data_payload_t *dm_rpu,
                                          char* dm_byte_sequence,
                                          int *length);
    bool IsWriteRpuBufferSucceed(dv_md_parser_rc_t process_ret) const;

    RVSD_RINGBUF m_input;

    bool m_is_eos;
    unsigned long m_eos_position;

    unsigned char *mp_work_buffer;

    dv_md_parser_handle_t m_parser_handle;
    dv_md_parser_conf_t m_parser_config;

    ParserOutputContext m_output_context;

    pthread_mutex_t m_mutex;
};

