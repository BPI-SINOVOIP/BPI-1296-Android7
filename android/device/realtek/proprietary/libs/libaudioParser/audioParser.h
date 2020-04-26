#ifdef  __cplusplus
extern "C" {
#endif

#define USE_DTSM8_PARSER

typedef enum
{
    ENUM_RTK_DTS_NONE = 0,
    ENUM_RTK_DTS_CORE_ONLY,
    ENUM_RTK_DTSHD,
    ENUM_RTK_DTS_CORELESS,
}RTK_DTS_BS_TYPE;

typedef struct
{
    int samplerate;
    int ch_num;
    RTK_DTS_BS_TYPE bs_type;
}RTK_DTS_PARSER_INFO;

void *rtk_dts_parser_init();
int rtk_dts_parser_process(void *parser, char *pBuf, int nBufLen, RTK_DTS_PARSER_INFO *);
void rtk_dts_parser_uninit(void *parser);
int check_audio_bitstream(char *p, unsigned int size, unsigned int *sample_rate);

#ifdef __cplusplus
}
#endif
