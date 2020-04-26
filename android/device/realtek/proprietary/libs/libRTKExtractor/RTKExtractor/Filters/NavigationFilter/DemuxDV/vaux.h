#define LittleEndian_BitField
//for Audio
typedef struct {
#ifdef LittleEndian_BitField
    uint8_t af_size :6;
    uint8_t         :1;
    uint8_t lf      :1;
#else
    uint8_t lf      :1;   // Samples per frame: * 32 kHz: 1053-1080   * 44.1: 1452-1489   * 48: 1580-1620 
    uint8_t         :1;   // Should be 1
    uint8_t af_size :6;   // Locked mode flag (1 = unlocked)
#endif
} dv_aaux_as_pc1_t;

typedef struct {
#ifdef LittleEndian_BitField
    uint8_t audio_mode:4;
    uint8_t pa        :1;
    uint8_t chn       :2;
    uint8_t sm        :1;
#else
    uint8_t sm        :1; // stereo mode: 0 = Multi-stereo, 1 = Lumped
    uint8_t chn       :2; // number of audio channels per block: 0 = 1 channel, 1 = 2 channels, others reserved
    uint8_t pa        :1; // pair bit: 0 = one pair of channels, 1 = independent channel (for sm = 1, pa shall be 1)
    uint8_t audio_mode:4; // See 8.1...
#endif
} dv_aaux_as_pc2_t;

typedef struct {
#ifdef LittleEndian_BitField
    uint8_t stype     :5;
    uint8_t system    :1;
    uint8_t mk        :1;
    uint8_t           :1;
#else
    uint8_t           :1;
    uint8_t ml        :1; // Multi-languag flag
    uint8_t system    :1; // 0 = 60 fields, 1 = 50 field
    uint8_t stype     :5; // 0x0 = SD (525/625), 0x2 = HD (1125,1250), others reserved
#endif
} dv_aaux_as_pc3_t;

typedef struct {
#ifdef LittleEndian_BitField
    uint8_t qu      :3;
    uint8_t smp     :3;
    uint8_t tc      :1;
    uint8_t ef      :1;
#else
    uint8_t ef      :1; // emphasis: 0=on, 1=off
    uint8_t tc      :1; // time constant of emphasis: 1=50/15us, 0=reserved
    uint8_t smp     :3; // sampling frequency: 0=48kHz, 1=44,1 kHz, 2=32 kHz
    uint8_t qu      :3; // quantization: 0=16bits linear, 1=12bits non-linear, 2=20bits linear, others reserved
#endif
} dv_aaux_as_pc4_t;

typedef struct {               // AAUX source pack (AS)
    uint8_t pc0;                     // value is 0x50;
    dv_aaux_as_pc1_t pc1;
    dv_aaux_as_pc2_t pc2;
    dv_aaux_as_pc3_t pc3;
    dv_aaux_as_pc4_t pc4;
} dv_aaux_as_t;

// From 61834-4 (section 8.2), and SMPE 314M (section 4.6.2.3.2)
typedef struct {
#ifdef LittleEndian_BitField
    uint8_t ss      :2;
    uint8_t cmp     :2;
    uint8_t isr     :2;
    uint8_t cgms    :2;
#else
              
    uint8_t cgms    :2; /* Copy generation management system:  0=unrestricted, 1=not used, 2=one generation only, 3=no copy */
    uint8_t isr     :2; /* 0=analog input, 1=digital input, 2=reserved, 3=no information */
    uint8_t cmp     :2; /* number of times compression: 0=once, 1=twice, 2=three or more, 3=no information */
    uint8_t ss      :2; /* 61834 says "Source and recorded situation...", SMPTE says EFC (emphasis audio channel flag)
                           0=emphasis off, 1=emphasis on, others reserved.  EFC shall be set for each audio block. */
#endif
} dv_aaux_asc_pc1_t;

typedef struct {
#ifdef LittleEndian_BitField
    uint8_t insert_ch :3;
    uint8_t rec_mode  :3;
    uint8_t rec_end   :1;
    uint8_t rec_st    :1;
#else
    uint8_t rec_st    :1;/* recording start point: 0=yes,1=no */
    uint8_t rec_end   :1; /* recording end point: same as starting... */
    uint8_t rec_mode  :3; /* recording mode: 1=original, others=various dubs... (see 68134-4) */
    uint8_t insert_ch :3; /* see 61834-4... */
#endif
} dv_aaux_asc_pc2_t;

typedef struct {
#ifdef LittleEndian_BitField
    uint8_t speed     :7;
    uint8_t drf       :1;
#else
    uint8_t drf       :1; /* direction: 1=forward, 0=reverse */
    uint8_t speed     :7; /* speed: see tables in 314M and 61834-4 (they differ), except 0xff = invalid/unkown */
#endif
} dv_aaux_asc_pc3_t;

typedef struct {
#ifdef LittleEndian_BitField
    uint8_t genre_category: 7;
    uint8_t               : 1;
#else
    uint8_t               : 1;
    uint8_t genre_category: 7;
#endif
} dv_aaux_asc_pc4_t;

typedef struct {                    // AAUX source control pack (ASC)
    uint8_t pc0;                        // value is 0x51;
    dv_aaux_asc_pc1_t pc1;
    dv_aaux_asc_pc2_t pc2;
    dv_aaux_asc_pc3_t pc3;
    dv_aaux_asc_pc4_t pc4;
} dv_aaux_asc_t;

//for Video
typedef struct {
#ifdef LittleEndian_BitField
    uint8_t units   :4; //units of TV chnnels
    uint8_t tens    :4; //tens of TV chnnels
#else
    uint8_t tens    :4; //tens of TV chnnels
    uint8_t units   :4; //units of TV chnnels
#endif
} dv_vaux_vs_pc1_t;

typedef struct {
#ifdef LittleEndian_BitField
    uint8_t hd      :4; //hundredss of TV chnnels
    uint8_t CLF     :2; //colour frames identification code
    uint8_t en      :1; //colour framse enable flag  :  0 CLF is valid
    uint8_t bw      :1; // black and white  0: black/ 1: colour
#else
    uint8_t bw      :1; // black and white  0: black/ 1: colour
    uint8_t en      :1; //colour framse enable flag  :  0 CLF is valid
    uint8_t CLF     :2; //colour frames identification code
    uint8_t hd      :4; //hundredss of TV chnnels
#endif
} dv_vaux_vs_pc2_t;

typedef struct {
#ifdef LittleEndian_BitField
    uint8_t stype   :5; //colour frames identification code
    uint8_t s50_60  :1; //colour framse enable flag  :  0 CLF is valid
    uint8_t src_code:2; // black and white  0: black/ 1: colour
#else
    uint8_t src_code:2; // black and white  0: black/ 1: colour
    uint8_t s50_60  :1; //colour framse enable flag  :  0 CLF is valid
    uint8_t stype   :5; //colour frames identification code
#endif
} dv_vaux_vs_pc3_t;

typedef struct {                // VAUX source pack (VS)
    uint8_t pc0;                  // value is 0x60;
    dv_vaux_vs_pc1_t pc1;
    dv_vaux_vs_pc2_t pc2;
    dv_vaux_vs_pc3_t pc3;
    uint8_t pc4;                  // Tuner Category
} dv_vaux_vs_t;

typedef struct {
#ifdef LittleEndian_BitField
    uint8_t ss      :2;
    uint8_t cmp     :2;
    uint8_t isr     :2;
    uint8_t cgms    :2;
#else
              
    uint8_t cgms    :2; /* Copy generation management system:  0=unrestricted, 1=not used, 2=one generation only, 3=no copy */
    uint8_t isr     :2; /* 0=analog input, 1=digital input, 2=reserved, 3=no information */
    uint8_t cmp     :2; /* number of times compression: 0=once, 1=twice, 2=three or more, 3=no information */
    uint8_t ss      :2; /* 61834 says "Source and recorded situation...", SMPTE says EFC (emphasis audio channel flag)
                       0=emphasis off, 1=emphasis on, others reserved.  EFC shall be set for each audio block. */
#endif
} dv_vaux_vsc_pc1_t;

typedef struct {
#ifdef LittleEndian_BitField
    uint8_t disp    :3;// display select mode , 4:3 full format, 16:9 letter box, 16:9 full format.....
    uint8_t         :1;
    uint8_t rec_mode:2; /* 0: original, 1:reserved, 2: insert, 3: invalid recording  */
    uint8_t         :1;
    uint8_t rec_st  :1; /*recording start point*/
#else
    uint8_t rec_st  :1; /*recording start point*/
    uint8_t         :1;
    uint8_t rec_mode:2; /* 0: original, 1:reserved, 2: insert, 3: invalid recording  */
    uint8_t         :1;
    uint8_t disp    :3;// display select mode , 4:3 full format, 16:9 letter box, 16:9 full format.....
#endif
} dv_vaux_vsc_pc2_t;

typedef struct {
#ifdef LittleEndian_BitField
    uint8_t bcsys   :2;// broadcast system
    uint8_t sc      :1; // still camera picture
    uint8_t st      :1; // still field picture flag
    uint8_t IL      :1; //Interlace flag : 0 non-interlaced , 1: interlaced or  unrecognized
    uint8_t fc      :1; //frame change flag
    uint8_t fs      :1;// first/second flag
    uint8_t ff      :1; //frame/field flag: 
#else
    uint8_t ff      :1; //frame/field flag: 
    uint8_t fs      :1;// first/second flag
    uint8_t fc      :1; //frame change flag
    uint8_t IL      :1; //Interlace flag : 0 non-interlaced , 1: interlaced or  unrecognized
    uint8_t st      :1; // still field picture flag
    uint8_t sc      :1; // still camera picture
    uint8_t bcsys   :2;// broadcast system
#endif
}dv_vaux_vsc_pc3_t;


typedef struct {
#ifdef LittleEndian_BitField
    uint8_t genre_category:7;
    uint8_t               :1;
#else
    uint8_t               :1;
    uint8_t genre_category:7;
#endif
} dv_vaux_vsc_pc4_t;

typedef struct {                  // VAUX source control  pack (VSC)
    uint8_t pc0;                  // value is 0x61;
    dv_vaux_vsc_pc1_t pc1;
    dv_vaux_vsc_pc2_t pc2;
    dv_vaux_vsc_pc3_t pc3;
    dv_vaux_vsc_pc4_t pc4;    
} dv_vaux_vsc_t;

typedef struct {
    dv_vaux_vs_t vs;
    dv_vaux_vsc_t vsc;
} dv_vaux_c;

typedef struct {
    dv_aaux_as_t as;
    dv_aaux_asc_t asc;
} dv_aaux_c;


