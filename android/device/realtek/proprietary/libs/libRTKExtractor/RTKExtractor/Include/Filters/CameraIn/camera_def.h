#ifndef __CAMERA_DEF_H__
#define __CAMERA_DEF_H__


enum {
    CAMERA_NONE = 0,        
    CAMERA_PSEUDO,    
    CAMERA_UVC,    
    CAMERA_MAX
};

typedef enum {
    CAMERA_VFMT_UNKNOWN = 0,                
    CAMERA_VFMT_RGB332,
    CAMERA_VFMT_RGB444,
    CAMERA_VFMT_RGB555,    
    CAMERA_VFMT_RGB565,
    CAMERA_VFMT_RGB555X,
    CAMERA_VFMT_RGB565X,        
    CAMERA_VFMT_RGB24,
    CAMERA_VFMT_RGB32,    
    CAMERA_VFMT_GREY,        
    CAMERA_VFMT_YUV410,            
    CAMERA_VFMT_YUV420,
    CAMERA_VFMT_YVU410,            
    CAMERA_VFMT_YVU420,
    CAMERA_VFMT_YUYV,
    CAMERA_VFMT_YYUV,
    CAMERA_VFMT_UYVY,    
    CAMERA_VFMT_YUV411P,
    CAMERA_VFMT_YUV422P,    
    CAMERA_VFMT_YUV41P,                    
    /* compressed formats */
    CAMERA_VFMT_MJPG,    
    CAMERA_VFMT_JPG,
    CAMERA_VFMT_DV,
    CAMERA_VFMT_MPEG,      
    CAMERA_VFMT_H264,      
    CAMERA_VFMT_MAX,      
}CAMERA_VFMT;


typedef struct {
    int             count;     
    CAMERA_VFMT     fmt[CAMERA_VFMT_MAX];       
}CAMERA_VFMT_CAP;


typedef struct {
    CAMERA_VFMT     fmt;
    int             width;
    int             height;    
    float           fps;        
}CAMERA_VIDEO_FORMAT;


/***************************************************************
 * Fame Size
 ***************************************************************/

typedef struct {    
    int             width;
    int             height;
}CAMERA_FS_REGION;


typedef struct {    
    int             min_width;
    int             max_width;
    int             step_width;    
    int             min_height;
    int             max_height;
    int             step_height;
}CAMERA_FS_RANGE;


typedef struct 
{
    unsigned char       type;    

    #define CAMERA_FS_CAP_TYPE_RANGE         0x01
    #define CAMERA_FS_CAP_TYPE_LIST          0x02

    union 
    {
        // type =  CAMERA_FS_CAP_TYPE_RANGE  
        CAMERA_FS_RANGE  range;
        
        // type =  CAMERA_FS_CAP_TYPE_LIST  
        struct                      
        {
            #define MAX_FS_CAP_ENTRY    64            
            
            unsigned char       count;
            
            CAMERA_FS_REGION    size[MAX_FS_CAP_ENTRY];
            
        }size_list;
    };    
    
}CAMERA_FS_CAP;



/***************************************************************
 * Fame Rate
 ***************************************************************/
 
typedef struct {    
    float             min_fps;
    float             max_fps;
    float             step_fps;
}CAMERA_FR_RANGE;
 
typedef struct 
{
    unsigned char       type;    

    #define CAMERA_FR_CAP_TYPE_RANGE         0x01
    #define CAMERA_FR_CAP_TYPE_LIST          0x02

    union 
    {
        // type =  CAMERA_FR_CAP_TYPE_RANGE  
        CAMERA_FR_RANGE     range;
        
        // type =  CAMERA_FR_CAP_TYPE_LIST  
        struct                      
        {
            #define MAX_FR_CAP_ENTRY    64                        
            unsigned char   count;            
            float           fps[MAX_FR_CAP_ENTRY];            
        }rate;
    };
}CAMERA_FR_CAP;


/***************************************************************
 * Fame Info
 ***************************************************************/

typedef struct 
{    
    unsigned long   flags;    
    long long       pts;  
    unsigned long   phy_addr;
    
    #define CAMERA_FRAME_FLAG_KEYFRAME      0x0008      // this frame is I frame
    #define CAMERA_FRAME_FLAG_PFRAME        0x0004      // this frame is P frame
    #define CAMERA_FRAME_FLAG_BFRAME        0x0002      // this frame is B frame
    #define CAMERA_FRAME_FLAG_PHYADDR       0x1000      // phy_addr field is available
}CAMERA_FRAME_INFO;


/***************************************************************
 * MISC
 ***************************************************************/

#define CAMERA_CTRL_OK           (0)
#define CAMERA_CTRL_FAIL        (-1)
#define CAMERA_CTRL_ENODEV      (-2)


/***************************************************************
 * DEBUG
 ***************************************************************/
 
#define CAMERA_WARNING(fmt, args...)       printf("[CAMERA] WARNING, " fmt, ## args)
//#define CAMERA_DBG(fmt, args...)         printf("[CAMERA] DBG, " fmt, ## args)
#define CAMERA_DBG(fmt, args...)   
#define CAMERA_INFO(fmt, args...)          printf("[CAMERA] INFO, " fmt, ## args)


#endif  //__CAMERA_DEF_H__
