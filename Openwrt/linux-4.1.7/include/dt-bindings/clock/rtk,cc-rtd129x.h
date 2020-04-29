#ifndef __DT_BINDINGS_REALTEK_CC_RTD129X_H
#define __DT_BINDINGS_REALTEK_CC_RTD129X_H

#define CC_PLL_SCPU          0
#define CC_CLK_SCPU          1
#define CC_CLK_SCPU_SET      2

#define CC_PLL_BUS           3
#define CC_PLL_BUS_DIV2      4
#define CC_CLK_SYS           5
#define CC_CLK_SYS_SB2       6

#define CC_PLL_BUS_H         7
#define CC_CLK_SYSH          8

#define CC_PLL_DDSA          9
#define CC_PLL_DDSB         10

#define CC_PLL_GPU	    11
#define CC_CLK_GPU          12

#define CC_PLL_VE1          13
#define CC_PLL_VE2          14
#define CC_CLK_VE1          15
#define CC_CLK_VE2          16
#define CC_CLK_VE3          17

#define CC_PLL_VODMA        18
#define CC_PLL_ACPU         19
#define CC_CLK_ACPU         20
#define CC_CLK_MAX          21

#define CGC_CRT_CLK_EN_1          0
#define CGC_CRT_CLK_EN_2          1
#define CGC_ISO_CLK_EN            2
#define CGC_CRT_CLK_EN_LSADC      3

#endif /* __DT_BINDINGS_REALTEK_CC_RTD129X_H */
