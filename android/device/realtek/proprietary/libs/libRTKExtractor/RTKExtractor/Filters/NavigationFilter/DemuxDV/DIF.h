
#ifndef __DIF_H__
#define __DIF_H__

#define DIF_BLK_SIZE                    80
#define DIF_HEADER_ID_SIZE              3
#define VAUX_PACKET_SIZE                5

#define NTSC_DIF_COUNT                  10 
#define PAL_DIF_COUNT                   12 

#define DIF_BLOCK_COUNT                 150
#define DIF_HEADER_COUNT                1
#define DIF_SUBCODE_COUNT               2
#define DIF_VAUX_COUNT                  3
#define DIF_AUDIO_COUNT                 9
#define DIF_VIDEO_COUNT                 135

#define DIF_AUDIO_SEGMENT_COUNT         9
#define DIF_AUDIO_SEGMENT_DIF_COUNT     1

#define DIF_VIDEO_SEGMENT_COUNT         9
#define DIF_VIDEO_SEGMENT_DIF_COUNT     15

#define SCT_HEADER                      0x00
#define SCT_SUBCODE                     0x01
#define SCT_VAUX                        0x02
#define SCT_AUDIO                       0x03
#define SCT_VIDEO                       0x04

//--- DIF Code
typedef struct {
    unsigned char Seq  : 4;
    unsigned char RSV0 : 1;
    unsigned char SCT  : 3;
    unsigned char RSV1 : 3;
    unsigned char zero : 1;
    unsigned char Dseq : 4;
    unsigned char DBN;
}DIF_HEADER;

typedef struct {
    DIF_HEADER        header;
    unsigned char     data[DIF_BLK_SIZE-DIF_HEADER_ID_SIZE]; 
}DIF;

//---------------------------------------------
// Sub Code
typedef struct{
    unsigned char subcode_id[2];
    unsigned char reserved;
    unsigned char subcode_data[5];    
}SUBCODE_PACK;

//---------------------------------------------
// VAUX

#define VAUX_VS_ID              0x60
#define VAUX_VSC_ID             0x61
#define VAUX_RECDATE_ID         0x62
#define VAUX_RECTIME_ID         0x63


typedef struct{
    unsigned char ID;
    
    unsigned char TV_CHANNEL    : 4;
    unsigned char TV_CHANNEL_T  : 4;
                                
    unsigned char TV_CHANNEL_H  : 4;
    unsigned char CLF           : 2;
    unsigned char EN            : 1;
    unsigned char BW            : 1;
                                
    unsigned char STYPE         : 5;
    unsigned char FS_50_60      : 1;
    unsigned char SC            : 2;
    
    unsigned char TUNER_CATEGORY;
}VAUX_VS_PACK;


typedef struct{
    unsigned char ID;
    
    unsigned char SS            : 2;
    unsigned char CMP           : 2;
    unsigned char ISR           : 2;
    unsigned char CGMS          : 2;
                                
    unsigned char DISP          : 4;
    unsigned char one           : 1;
    unsigned char REC_MODE      : 2;
    unsigned char one_2         : 1;
    unsigned char REC_ST        : 1;
                                
    unsigned char BCSYS         : 2;
    unsigned char SC            : 1;
    unsigned char ST            : 1;
    unsigned char IL            : 1;
    unsigned char FC            : 1;
    unsigned char FS            : 1;
    unsigned char FF            : 1;
    
    unsigned char GEMRE_CATEGORY: 7;
    unsigned char one_3         : 1;
}VAUX_VSC_PACK;


typedef struct {
    unsigned char ID;
    
    unsigned char TIME_ZONE     : 4;    
    unsigned char TIME_ZONE_T   : 2;
    unsigned char TM            : 1;
    unsigned char DS            : 1;
    
    unsigned char DAY           : 4;
    unsigned char DAY_T         : 2;
    unsigned char ones          : 2;
                                
    unsigned char MONTH         : 4;
    unsigned char MONTH_T       : 1;
    unsigned char WEEK          : 3;
    
    unsigned char YEAR          : 4;
    unsigned char YEAR_T        : 4;
}VAUX_RECDATE_PACK;


typedef struct {
    unsigned char ID;
    
    unsigned char FRAME         : 4;    
    unsigned char FRAME_T       : 2;
    unsigned char ones          : 2;    
    
    unsigned char SECOND        : 4;
    unsigned char SECOND_T      : 3;
    unsigned char one_2         : 1;
                                
    unsigned char MINUTE        : 4;
    unsigned char MINUTE_T      : 3;
    unsigned char one_1         : 1;
    
    unsigned char HOUR          : 4;
    unsigned char HOUR_T        : 2;
    unsigned char one_3         : 2;                                
}VAUX_RECTIME_PACK;

union VAUX_PACK {    
    unsigned char       PC[VAUX_PACKET_SIZE];   
    VAUX_VS_PACK        VS;                     
    VAUX_VSC_PACK       VSC;
    VAUX_RECDATE_PACK   RECDATE;
    VAUX_RECTIME_PACK   RECTIME;
};

#endif // __DIF_H__
