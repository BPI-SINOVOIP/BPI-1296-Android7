#ifndef __NAV_TYPES_H__
#define __NAV_TYPES_H__


#include <io/ifo_types.h> 

#undef ATTRIBUTE_PACKED
#undef PRAGMA_PACK_BEGIN 
#undef PRAGMA_PACK_END

#if defined(__GNUC__)
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
#define ATTRIBUTE_PACKED __attribute__ ((packed))
#define PRAGMA_PACK 0
#endif
#endif

#if !defined(ATTRIBUTE_PACKED)
#define ATTRIBUTE_PACKED
#define PRAGMA_PACK 1
#endif

#if PRAGMA_PACK
#pragma pack(1)
#endif


///////////////////////////////////////////////////////
// PCI and DSI related informations




//
// PCI General Information (VI4-107), including PCI_GI_EX in +RW spec
//
typedef struct {
  uint32_t    nv_pck_lbn;      
  uint16_t    vobu_cat;        
  uint16_t    zero1;           
  uint32_t    vobu_uop_ctl;   
  uint32_t    vobu_s_ptm;     
  uint32_t    vobu_e_ptm;      
  uint32_t    vobu_se_e_ptm;   
  dvd_time_t  c_eltm;        

  // The following is PCI_GI_EX defined by +RW format
  uint8_t     reserved1[4];
  uint8_t     pci_gi_xi;
  uint8_t     chng_fld;
  uint16_t    rt_atr_1;
  uint16_t    rt_atr_2;
  uint32_t    rec_date_1;
  uint32_t    rec_time_1;
  uint32_t    rec_date_2;
  uint32_t    rec_time_2;
  uint8_t     reserved2[6];
} ATTRIBUTE_PACKED pci_gi_t;

//
// Non Seamless Angle Information
//
typedef struct {
  uint32_t    nsml_agl_dsta[9];  
} ATTRIBUTE_PACKED nsml_agli_t;

//
// Highlight General Information  (HLI_GI, VI4-115)
//
typedef struct {
  uint16_t    hli_ss;       
  uint32_t    hli_s_ptm;    
  uint32_t    hli_e_ptm;    
  uint32_t    btn_se_e_ptm; 
  uint16_t    btn_md;       
  uint8_t     btn_ofn;      
  uint8_t     btn_ns;       
  uint8_t     nsl_btn_ns;   
  uint8_t     zero1;        
  uint8_t     fosl_btnn;    
  uint8_t     foac_btnn;    
} ATTRIBUTE_PACKED hl_gi_t;

// 
// Button Color Information Table 
//
typedef struct {
  uint32_t   btn_coli[3][2];  /**< [button color number-1][select:0/action:1] */
} ATTRIBUTE_PACKED btn_colit_t;

// 
// Button Information
//
typedef struct {
  uint8_t    btn_posi[6];   
  uint8_t    ajbtn_posi[4]; 
  vm_cmd_t   cmd;          
} ATTRIBUTE_PACKED btni_t;

//
// Highlight Information 
//
typedef struct {
  hl_gi_t        hl_gi;
  btn_colit_t    btn_colit;
  btni_t         btnit[36];
} ATTRIBUTE_PACKED hli_t;

//
// PCI 
//
typedef struct {
  pci_gi_t    pci_gi;
  nsml_agli_t nsml_agli;
  hli_t       hli;
} ATTRIBUTE_PACKED pci_t;

///////////////////////////////////////////////////////////////
//
// DSI_GI (VI4-134)
//
typedef struct {
  uint32_t    nv_pck_scr;
  uint32_t    nv_pck_lbn;   
  uint32_t    vobu_ea;      
  uint32_t    vobu_1stref_ea;
  uint32_t    vobu_2ndref_ea;
  uint32_t    vobu_3rdref_ea;
  uint16_t    vobu_vob_idn;  
  uint8_t     zero1;         
  uint8_t     vobu_c_idn;    
  dvd_time_t  c_eltm;        
} ATTRIBUTE_PACKED dsi_gi_t;

//
// Seamless Playback Information (SML_PBI VI4-137)
//
typedef struct {
  uint16_t    category;     
  uint32_t    ilvu_ea;      
  uint32_t    ilvu_sa;      
  uint16_t    size;         
  uint32_t    vob_v_s_s_ptm;
  uint32_t    vob_v_e_e_ptm;
  struct {
    uint32_t   vob_a_stp_ptm1;
    uint32_t   vob_a_stp_ptm2;
    uint32_t   vob_a_gap_len1;
    uint32_t   vob_a_gap_len2;      
  } vob_a[8];
} ATTRIBUTE_PACKED sml_pbi_t;

//
// Seamless Angle Infromation for one angle
//
typedef struct {
    uint32_t    dest_addr; 
    uint16_t    dest_size;    
} ATTRIBUTE_PACKED sml_agl_data_t;

//
// Seamless Angle Infromation
//
typedef struct {
  sml_agl_data_t   data[9];
} ATTRIBUTE_PACKED sml_agli_t;

//
// VOBU Search Information  (VOBU_SRI)
//
typedef struct {
  uint32_t    next_video;
  uint32_t    fwdi[19];  
  uint32_t    next_vobu;
  uint32_t    prev_vobu;
  uint32_t    bwdi[19];  
  uint32_t    prev_video;
} ATTRIBUTE_PACKED vobu_sri_t;

//
// Synchronous Information (SYNCI VI4-151)
// 
typedef struct {
  uint16_t    a_synca[8];  
  uint32_t    sp_synca[32];
} ATTRIBUTE_PACKED synci_t;

//
// DSI
//
typedef struct {
  dsi_gi_t      dsi_gi;
  sml_pbi_t     sml_pbi;
  sml_agli_t    sml_agli;
  vobu_sri_t    vobu_sri;
  synci_t       synci;
} ATTRIBUTE_PACKED dsi_t;

#if PRAGMA_PACK
#pragma pack()
#endif

#endif // __NAV_TYPES_H__
