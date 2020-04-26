/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
/*
      File fieldpriv.h.   Version 20 September 2002

      Declarations for field routines which the application program
      doesn't need to know about.
*/

#ifndef __FIELDPRIV_H
#define __FIELDPRIV_H 1
#include "bigpriv.h"
#include "field.h"


/* For C++ */
#ifdef __cplusplus
extern "C" {
#endif


//   The following routines are for internal use only

extern DWORDREG DRM_API GF2_mod_mul_temps(DWORDREGC, PBIGCTX_ARG);

typedef struct {
        int      sign;      //  +1 or -1
        digit_tc *mul1;
        digit_tc *mul2;
} Kdot_operand_t;           // Operand for Kdot

typedef const Kdot_operand_t Kdot_operand_tc;

extern BOOL DRM_API Kadder_2(FIELD_INPUT, FIELD_INPUT, FIELD_OUTPUT,
                            DWORDREGC, field_desc_tc*, PBIGCTX_ARG);

extern BOOL DRM_API Kdot    (Kdot_operand_tc*, DWORDREGC, FIELD_OUTPUT,
                            field_desc_tc*, MP_MODIFIED, PBIGCTX_ARG);

extern BOOL DRM_API K2_expon(FIELD_INPUT, MP_INPUT,
                            DWORDREGC, FIELD_OUTPUT,
                            field_desc_tc*, PBIGCTX_ARG);

extern BOOL DRM_API Kequaler_default (FIELD_INPUT, FIELD_INPUT,
                                     DWORDREGC, field_desc_tc*, PBIGCTX_ARG);

extern BOOL DRM_API Kexponentiator1_default(FIELD_INPUT, MP_INPUT, DWORDREGC,
                                    FIELD_OUTPUT, field_desc_tc*, PBIGCTX_ARG);

extern BOOL DRM_API Kfdesc_initialize(field_desc_t*, DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API Kimmediater_2(sdigit_tc*, FIELD_OUTPUT,
                                 DWORDREGC, field_desc_tc*, PBIGCTX_ARG);
extern BOOL DRM_API Kiszeroer_default(FIELD_INPUT, DWORDREGC,
                                     field_desc_tc*, PBIGCTX_ARG);

extern BOOL DRM_API Kmulpower2er_2
                          (FIELD_INPUT, const int, FIELD_OUTPUT,
                           DWORDREGC, field_desc_tc*, PBIGCTX_ARG);

extern BOOL DRM_API KPgcd_helper(FIELD_INPUT   f1,
                         FIELD_INPUT   f2,
                         DWORDREGC     lngf1,
                         DWORDREGC     lngf2,
                         FIELD_OUTPUT  gcd,
                         DWORDREG     *lnggcd,     // OUT
                         FIELD_OUTPUT  f1mulby,
                         FIELD_OUTPUT  f2mulby,
                         field_desc_tc *fdesc,
                         MP_MODIFIED   temps,
                         PBIGCTX_ARG);

extern BOOL DRM_API KPgcd_helper_space
        (DWORDREGC    lngf1,
         DWORDREGC    lngf2,
         const BOOL   need_f1mulby,
         const BOOL   need_f2mulby,
         field_desc_tc *fdesc,
         DWORDREG      *pneed,     // OUT
         PBIGCTX_ARG);


extern BOOL DRM_API Krandomizer_2(FIELD_OUTPUT, DWORDREGC,
                                 field_desc_tc*, PBIGCTX_ARG);

extern BOOL DRM_API Ksizer_2(MP_OUTPUT, field_desc_tc*, PBIGCTX_ARG);

extern BOOL DRM_API Kzeroizer_default(FIELD_OUTPUT, DWORDREGC,
                                     field_desc_tc*, PBIGCTX_ARG);


#ifdef __cplusplus
}
#endif

#endif /* __FIELDPRIV_H */
