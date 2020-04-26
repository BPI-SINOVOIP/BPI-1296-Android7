#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <debug.h>
#include <target/debugconfig.h>
#include <arch/arch_ops.h>
#include <lib/console.h>
#include <platform.h>
#include <platform/io.h>
#include <platform/cpu.h>

#define BASE_ADDRESS 0x30000000

unsigned int touch_isr; 
unsigned int pic_done; 
void diag_pass(void);
void diag_fail(void);

void ve_isr(void);	

static void ve_test_main(void)
{
    int rlt;
	lk_bigtime_t ve_start, ve_simulation;
	arch_disable_ints();
	/*The setting for enabling VE.*/
	(*((volatile unsigned int*) 0x98000000)=0xffffffff);
	(*((volatile unsigned int*) 0x9800000c)=0xffffffff);
    (*((volatile unsigned int*) 0x98043000)=0x00000002);
    touch_isr = 0;
#ifdef SCPU_ARM
    SET_INT(INTERRUPT_SCPU_VE1, ve_isr);
#else
    //*((volatile unsigned int*) SW_INT_RET_ADDR) = (void*) ve_isr;
#endif
	ve_start = current_time_hires();
	printf("Start Time: %d\n", (int)ve_start);
//    $display("[RTK_C] DELAY_C(66600); aclk");
//    DELAY_C(66600);
    //in vtSystemInit
    //in vtHostRegReset
    (*((volatile unsigned int*) 0x98040100)=0x00000000);//`WRITE_APB(16'h0100, 32'h00000000);
    (*((volatile unsigned int*) 0x98040104)=0x00000000);//`WRITE_APB(16'h0104, 32'h00000000);
    (*((volatile unsigned int*) 0x98040108)=0x00000000);//`WRITE_APB(16'h0108, 32'h00000000);
    (*((volatile unsigned int*) 0x9804010c)=0x00000000);//`WRITE_APB(16'h010c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040110)=0x00000000);//`WRITE_APB(16'h0110, 32'h00000000);
    (*((volatile unsigned int*) 0x98040114)=0x00000000);//`WRITE_APB(16'h0114, 32'h00000000);
    (*((volatile unsigned int*) 0x98040118)=0x00000000);//`WRITE_APB(16'h0118, 32'h00000000);
    (*((volatile unsigned int*) 0x9804011c)=0x00000000);//`WRITE_APB(16'h011c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040120)=0x00000000);//`WRITE_APB(16'h0120, 32'h00000000);
    (*((volatile unsigned int*) 0x98040124)=0x00000000);//`WRITE_APB(16'h0124, 32'h00000000);
    (*((volatile unsigned int*) 0x98040128)=0x00000000);//`WRITE_APB(16'h0128, 32'h00000000);
    (*((volatile unsigned int*) 0x9804012c)=0x00000000);//`WRITE_APB(16'h012c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040130)=0x00000000);//`WRITE_APB(16'h0130, 32'h00000000);
    (*((volatile unsigned int*) 0x98040134)=0x00000000);//`WRITE_APB(16'h0134, 32'h00000000);
    (*((volatile unsigned int*) 0x98040138)=0x00000000);//`WRITE_APB(16'h0138, 32'h00000000);
    (*((volatile unsigned int*) 0x9804013c)=0x00000000);//`WRITE_APB(16'h013c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040140)=0x00000000);//`WRITE_APB(16'h0140, 32'h00000000);
    (*((volatile unsigned int*) 0x98040144)=0x00000000);//`WRITE_APB(16'h0144, 32'h00000000);
    (*((volatile unsigned int*) 0x98040148)=0x00000000);//`WRITE_APB(16'h0148, 32'h00000000);
    (*((volatile unsigned int*) 0x9804014c)=0x00000000);//`WRITE_APB(16'h014c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040150)=0x00000000);//`WRITE_APB(16'h0150, 32'h00000000);
    (*((volatile unsigned int*) 0x98040154)=0x00000000);//`WRITE_APB(16'h0154, 32'h00000000);
    (*((volatile unsigned int*) 0x98040158)=0x00000000);//`WRITE_APB(16'h0158, 32'h00000000);
    (*((volatile unsigned int*) 0x9804015c)=0x00000000);//`WRITE_APB(16'h015c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040160)=0x00000000);//`WRITE_APB(16'h0160, 32'h00000000);
    (*((volatile unsigned int*) 0x98040164)=0x00000000);//`WRITE_APB(16'h0164, 32'h00000000);
    (*((volatile unsigned int*) 0x98040168)=0x00000000);//`WRITE_APB(16'h0168, 32'h00000000);
    (*((volatile unsigned int*) 0x9804016c)=0x00000000);//`WRITE_APB(16'h016c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040170)=0x00000000);//`WRITE_APB(16'h0170, 32'h00000000);
    (*((volatile unsigned int*) 0x98040174)=0x00000000);//`WRITE_APB(16'h0174, 32'h00000000);
    (*((volatile unsigned int*) 0x98040178)=0x00000000);//`WRITE_APB(16'h0178, 32'h00000000);
    (*((volatile unsigned int*) 0x9804017c)=0x00000000);//`WRITE_APB(16'h017c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040180)=0x00000000);//`WRITE_APB(16'h0180, 32'h00000000);
    (*((volatile unsigned int*) 0x98040184)=0x00000000);//`WRITE_APB(16'h0184, 32'h00000000);
    (*((volatile unsigned int*) 0x98040188)=0x00000000);//`WRITE_APB(16'h0188, 32'h00000000);
    (*((volatile unsigned int*) 0x9804018c)=0x00000000);//`WRITE_APB(16'h018c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040190)=0x00000000);//`WRITE_APB(16'h0190, 32'h00000000);
    (*((volatile unsigned int*) 0x98040194)=0x00000000);//`WRITE_APB(16'h0194, 32'h00000000);
    (*((volatile unsigned int*) 0x98040198)=0x00000000);//`WRITE_APB(16'h0198, 32'h00000000);
    (*((volatile unsigned int*) 0x9804019c)=0x00000000);//`WRITE_APB(16'h019c, 32'h00000000);
    (*((volatile unsigned int*) 0x980401a0)=0x00000000);//`WRITE_APB(16'h01a0, 32'h00000000);
    (*((volatile unsigned int*) 0x980401a4)=0x00000000);//`WRITE_APB(16'h01a4, 32'h00000000);
    (*((volatile unsigned int*) 0x980401a8)=0x00000000);//`WRITE_APB(16'h01a8, 32'h00000000);
    (*((volatile unsigned int*) 0x980401ac)=0x00000000);//`WRITE_APB(16'h01ac, 32'h00000000);
    (*((volatile unsigned int*) 0x980401b0)=0x00000000);//`WRITE_APB(16'h01b0, 32'h00000000);
    (*((volatile unsigned int*) 0x980401b4)=0x00000000);//`WRITE_APB(16'h01b4, 32'h00000000);
    (*((volatile unsigned int*) 0x980401b8)=0x00000000);//`WRITE_APB(16'h01b8, 32'h00000000);
    (*((volatile unsigned int*) 0x980401bc)=0x00000000);//`WRITE_APB(16'h01bc, 32'h00000000);
    (*((volatile unsigned int*) 0x980401c0)=0x00000000);//`WRITE_APB(16'h01c0, 32'h00000000);
    (*((volatile unsigned int*) 0x980401c4)=0x00000000);//`WRITE_APB(16'h01c4, 32'h00000000);
    (*((volatile unsigned int*) 0x980401c8)=0x00000000);//`WRITE_APB(16'h01c8, 32'h00000000);
    (*((volatile unsigned int*) 0x980401cc)=0x00000000);//`WRITE_APB(16'h01cc, 32'h00000000);
    (*((volatile unsigned int*) 0x980401d0)=0x00000000);//`WRITE_APB(16'h01d0, 32'h00000000);
    (*((volatile unsigned int*) 0x980401d4)=0x00000000);//`WRITE_APB(16'h01d4, 32'h00000000);
    (*((volatile unsigned int*) 0x980401d8)=0x00000000);//`WRITE_APB(16'h01d8, 32'h00000000);
    (*((volatile unsigned int*) 0x980401dc)=0x00000000);//`WRITE_APB(16'h01dc, 32'h00000000);
    (*((volatile unsigned int*) 0x980401e0)=0x00000000);//`WRITE_APB(16'h01e0, 32'h00000000);
    (*((volatile unsigned int*) 0x980401e4)=0x00000000);//`WRITE_APB(16'h01e4, 32'h00000000);
    (*((volatile unsigned int*) 0x980401e8)=0x00000000);//`WRITE_APB(16'h01e8, 32'h00000000);
    (*((volatile unsigned int*) 0x980401ec)=0x00000000);//`WRITE_APB(16'h01ec, 32'h00000000);
    (*((volatile unsigned int*) 0x980401f0)=0x00000000);//`WRITE_APB(16'h01f0, 32'h00000000);
    (*((volatile unsigned int*) 0x980401f4)=0x00000000);//`WRITE_APB(16'h01f4, 32'h00000000);
    (*((volatile unsigned int*) 0x980401f8)=0x00000000);//`WRITE_APB(16'h01f8, 32'h00000000);
    (*((volatile unsigned int*) 0x980401fc)=0x00000000);//`WRITE_APB(16'h01fc, 32'h00000000);
    (*((volatile unsigned int*) 0x98040150)=0x00000000);//`WRITE_APB(16'h0150, 32'h00000000);
    //in vtBootLoader
    (*((volatile unsigned int*) 0x98040000)=0x00000000);//`WRITE_APB(16'h0000, 32'h00000000);
    (*((volatile unsigned int*) 0x98040004)=0x0000e40e);//`WRITE_APB(16'h0004, 32'h0000e40e);
    (*((volatile unsigned int*) 0x98040004)=0x00010020);//`WRITE_APB(16'h0004, 32'h00010020);
    (*((volatile unsigned int*) 0x98040004)=0x0002e470);//`WRITE_APB(16'h0004, 32'h0002e470);
    (*((volatile unsigned int*) 0x98040004)=0x0003e190);//`WRITE_APB(16'h0004, 32'h0003e190);
    (*((volatile unsigned int*) 0x98040004)=0x0004e470);//`WRITE_APB(16'h0004, 32'h0004e470);
    (*((volatile unsigned int*) 0x98040004)=0x0005e190);//`WRITE_APB(16'h0004, 32'h0005e190);
    (*((volatile unsigned int*) 0x98040004)=0x0006e470);//`WRITE_APB(16'h0004, 32'h0006e470);
    (*((volatile unsigned int*) 0x98040004)=0x0007e190);//`WRITE_APB(16'h0004, 32'h0007e190);
    (*((volatile unsigned int*) 0x98040004)=0x0008e470);//`WRITE_APB(16'h0004, 32'h0008e470);
    (*((volatile unsigned int*) 0x98040004)=0x0009e190);//`WRITE_APB(16'h0004, 32'h0009e190);
    (*((volatile unsigned int*) 0x98040004)=0x000ae40e);//`WRITE_APB(16'h0004, 32'h000ae40e);
    (*((volatile unsigned int*) 0x98040004)=0x000b037a);//`WRITE_APB(16'h0004, 32'h000b037a);
    (*((volatile unsigned int*) 0x98040004)=0x000ce470);//`WRITE_APB(16'h0004, 32'h000ce470);
    (*((volatile unsigned int*) 0x98040004)=0x000de190);//`WRITE_APB(16'h0004, 32'h000de190);
    (*((volatile unsigned int*) 0x98040004)=0x000ee470);//`WRITE_APB(16'h0004, 32'h000ee470);
    (*((volatile unsigned int*) 0x98040004)=0x000fe190);//`WRITE_APB(16'h0004, 32'h000fe190);
    (*((volatile unsigned int*) 0x98040004)=0x0010e470);//`WRITE_APB(16'h0004, 32'h0010e470);
    (*((volatile unsigned int*) 0x98040004)=0x0011e190);//`WRITE_APB(16'h0004, 32'h0011e190);
    (*((volatile unsigned int*) 0x98040004)=0x0012e470);//`WRITE_APB(16'h0004, 32'h0012e470);
    (*((volatile unsigned int*) 0x98040004)=0x0013e190);//`WRITE_APB(16'h0004, 32'h0013e190);
    (*((volatile unsigned int*) 0x98040004)=0x0014e40e);//`WRITE_APB(16'h0004, 32'h0014e40e);
    (*((volatile unsigned int*) 0x98040004)=0x00150394);//`WRITE_APB(16'h0004, 32'h00150394);
    (*((volatile unsigned int*) 0x98040004)=0x0016e470);//`WRITE_APB(16'h0004, 32'h0016e470);
    (*((volatile unsigned int*) 0x98040004)=0x0017e190);//`WRITE_APB(16'h0004, 32'h0017e190);
    (*((volatile unsigned int*) 0x98040004)=0x0018e470);//`WRITE_APB(16'h0004, 32'h0018e470);
    (*((volatile unsigned int*) 0x98040004)=0x0019e190);//`WRITE_APB(16'h0004, 32'h0019e190);
    (*((volatile unsigned int*) 0x98040004)=0x001ae40e);//`WRITE_APB(16'h0004, 32'h001ae40e);
    (*((volatile unsigned int*) 0x98040004)=0x001b0078);//`WRITE_APB(16'h0004, 32'h001b0078);
    (*((volatile unsigned int*) 0x98040004)=0x001ce470);//`WRITE_APB(16'h0004, 32'h001ce470);
    (*((volatile unsigned int*) 0x98040004)=0x001de190);//`WRITE_APB(16'h0004, 32'h001de190);
    (*((volatile unsigned int*) 0x98040004)=0x001ee470);//`WRITE_APB(16'h0004, 32'h001ee470);
    (*((volatile unsigned int*) 0x98040004)=0x001fe190);//`WRITE_APB(16'h0004, 32'h001fe190);
    (*((volatile unsigned int*) 0x98040004)=0x0020e168);//`WRITE_APB(16'h0004, 32'h0020e168);
    (*((volatile unsigned int*) 0x98040004)=0x0021d027);//`WRITE_APB(16'h0004, 32'h0021d027);
    (*((volatile unsigned int*) 0x98040004)=0x00220001);//`WRITE_APB(16'h0004, 32'h00220001);
    (*((volatile unsigned int*) 0x98040004)=0x0023a202);//`WRITE_APB(16'h0004, 32'h0023a202);
    (*((volatile unsigned int*) 0x98040004)=0x0024e0c2);//`WRITE_APB(16'h0004, 32'h0024e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00250008);//`WRITE_APB(16'h0004, 32'h00250008);
    (*((volatile unsigned int*) 0x98040004)=0x0026e0c2);//`WRITE_APB(16'h0004, 32'h0026e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00270058);//`WRITE_APB(16'h0004, 32'h00270058);
    (*((volatile unsigned int*) 0x98040004)=0x0028a200);//`WRITE_APB(16'h0004, 32'h0028a200);
    (*((volatile unsigned int*) 0x98040004)=0x0029e0c2);//`WRITE_APB(16'h0004, 32'h0029e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x002a005a);//`WRITE_APB(16'h0004, 32'h002a005a);
    (*((volatile unsigned int*) 0x98040004)=0x002be004);//`WRITE_APB(16'h0004, 32'h002be004);
    (*((volatile unsigned int*) 0x98040004)=0x002c0080);//`WRITE_APB(16'h0004, 32'h002c0080);
    (*((volatile unsigned int*) 0x98040004)=0x002de0c2);//`WRITE_APB(16'h0004, 32'h002de0c2);
    (*((volatile unsigned int*) 0x98040004)=0x002e0178);//`WRITE_APB(16'h0004, 32'h002e0178);
    (*((volatile unsigned int*) 0x98040004)=0x002fa2fe);//`WRITE_APB(16'h0004, 32'h002fa2fe);
    (*((volatile unsigned int*) 0x98040004)=0x00303cee);//`WRITE_APB(16'h0004, 32'h00303cee);
    (*((volatile unsigned int*) 0x98040004)=0x00313cef);//`WRITE_APB(16'h0004, 32'h00313cef);
    (*((volatile unsigned int*) 0x98040004)=0x00323cec);//`WRITE_APB(16'h0004, 32'h00323cec);
    (*((volatile unsigned int*) 0x98040004)=0x0033e0c0);//`WRITE_APB(16'h0004, 32'h0033e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x0034004a);//`WRITE_APB(16'h0004, 32'h0034004a);
    (*((volatile unsigned int*) 0x98040004)=0x0035f09a);//`WRITE_APB(16'h0004, 32'h0035f09a);
    (*((volatile unsigned int*) 0x98040004)=0x0036e41e);//`WRITE_APB(16'h0004, 32'h0036e41e);
    (*((volatile unsigned int*) 0x98040004)=0x00370170);//`WRITE_APB(16'h0004, 32'h00370170);
    (*((volatile unsigned int*) 0x98040004)=0x0038f04e);//`WRITE_APB(16'h0004, 32'h0038f04e);
    (*((volatile unsigned int*) 0x98040004)=0x003928ee);//`WRITE_APB(16'h0004, 32'h003928ee);
    (*((volatile unsigned int*) 0x98040004)=0x003ae412);//`WRITE_APB(16'h0004, 32'h003ae412);
    (*((volatile unsigned int*) 0x98040004)=0x003b0117);//`WRITE_APB(16'h0004, 32'h003b0117);
    (*((volatile unsigned int*) 0x98040004)=0x003ce0c0);//`WRITE_APB(16'h0004, 32'h003ce0c0);
    (*((volatile unsigned int*) 0x98040004)=0x003d0059);//`WRITE_APB(16'h0004, 32'h003d0059);
    (*((volatile unsigned int*) 0x98040004)=0x003e3ced);//`WRITE_APB(16'h0004, 32'h003e3ced);
    (*((volatile unsigned int*) 0x98040004)=0x003fa200);//`WRITE_APB(16'h0004, 32'h003fa200);
    (*((volatile unsigned int*) 0x98040004)=0x0040e0c2);//`WRITE_APB(16'h0004, 32'h0040e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00410059);//`WRITE_APB(16'h0004, 32'h00410059);
    (*((volatile unsigned int*) 0x98040004)=0x0042e0c2);//`WRITE_APB(16'h0004, 32'h0042e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00430058);//`WRITE_APB(16'h0004, 32'h00430058);
    (*((volatile unsigned int*) 0x98040004)=0x0044e0c2);//`WRITE_APB(16'h0004, 32'h0044e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00450008);//`WRITE_APB(16'h0004, 32'h00450008);
    (*((volatile unsigned int*) 0x98040004)=0x0046e0c0);//`WRITE_APB(16'h0004, 32'h0046e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x0047005c);//`WRITE_APB(16'h0004, 32'h0047005c);
    (*((volatile unsigned int*) 0x98040004)=0x0048a203);//`WRITE_APB(16'h0004, 32'h0048a203);
    (*((volatile unsigned int*) 0x98040004)=0x00495aed);//`WRITE_APB(16'h0004, 32'h00495aed);
    (*((volatile unsigned int*) 0x98040004)=0x004ae052);//`WRITE_APB(16'h0004, 32'h004ae052);
    (*((volatile unsigned int*) 0x98040004)=0x004bb615);//`WRITE_APB(16'h0004, 32'h004bb615);
    (*((volatile unsigned int*) 0x98040004)=0x004ce0c3);//`WRITE_APB(16'h0004, 32'h004ce0c3);
    (*((volatile unsigned int*) 0x98040004)=0x004d005d);//`WRITE_APB(16'h0004, 32'h004d005d);
    (*((volatile unsigned int*) 0x98040004)=0x004ee01a);//`WRITE_APB(16'h0004, 32'h004ee01a);
    (*((volatile unsigned int*) 0x98040004)=0x004fce00);//`WRITE_APB(16'h0004, 32'h004fce00);
    (*((volatile unsigned int*) 0x98040004)=0x0050e41e);//`WRITE_APB(16'h0004, 32'h0050e41e);
    (*((volatile unsigned int*) 0x98040004)=0x00510096);//`WRITE_APB(16'h0004, 32'h00510096);
    (*((volatile unsigned int*) 0x98040004)=0x0052e41e);//`WRITE_APB(16'h0004, 32'h0052e41e);
    (*((volatile unsigned int*) 0x98040004)=0x005300bf);//`WRITE_APB(16'h0004, 32'h005300bf);
    (*((volatile unsigned int*) 0x98040004)=0x0054e41e);//`WRITE_APB(16'h0004, 32'h0054e41e);
    (*((volatile unsigned int*) 0x98040004)=0x00550102);//`WRITE_APB(16'h0004, 32'h00550102);
    (*((volatile unsigned int*) 0x98040004)=0x0056d071);//`WRITE_APB(16'h0004, 32'h0056d071);
    (*((volatile unsigned int*) 0x98040004)=0x0057242a);//`WRITE_APB(16'h0004, 32'h0057242a);
    (*((volatile unsigned int*) 0x98040004)=0x0058e181);//`WRITE_APB(16'h0004, 32'h0058e181);
    (*((volatile unsigned int*) 0x98040004)=0x0059e41e);//`WRITE_APB(16'h0004, 32'h0059e41e);
    (*((volatile unsigned int*) 0x98040004)=0x005a013e);//`WRITE_APB(16'h0004, 32'h005a013e);
    (*((volatile unsigned int*) 0x98040004)=0x005be09e);//`WRITE_APB(16'h0004, 32'h005be09e);
    (*((volatile unsigned int*) 0x98040004)=0x005ca202);//`WRITE_APB(16'h0004, 32'h005ca202);
    (*((volatile unsigned int*) 0x98040004)=0x005d9f07);//`WRITE_APB(16'h0004, 32'h005d9f07);
    (*((volatile unsigned int*) 0x98040004)=0x005ee0c0);//`WRITE_APB(16'h0004, 32'h005ee0c0);
    (*((volatile unsigned int*) 0x98040004)=0x005f0048);//`WRITE_APB(16'h0004, 32'h005f0048);
    (*((volatile unsigned int*) 0x98040004)=0x0060e0c2);//`WRITE_APB(16'h0004, 32'h0060e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00610047);//`WRITE_APB(16'h0004, 32'h00610047);
    (*((volatile unsigned int*) 0x98040004)=0x0062e0c0);//`WRITE_APB(16'h0004, 32'h0062e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x00630059);//`WRITE_APB(16'h0004, 32'h00630059);
    (*((volatile unsigned int*) 0x98040004)=0x0064ae02);//`WRITE_APB(16'h0004, 32'h0064ae02);
    (*((volatile unsigned int*) 0x98040004)=0x0065e000);//`WRITE_APB(16'h0004, 32'h0065e000);
    (*((volatile unsigned int*) 0x98040004)=0x0066037a);//`WRITE_APB(16'h0004, 32'h0066037a);
    (*((volatile unsigned int*) 0x98040004)=0x0067e16a);//`WRITE_APB(16'h0004, 32'h0067e16a);
    (*((volatile unsigned int*) 0x98040004)=0x0068c000);//`WRITE_APB(16'h0004, 32'h0068c000);
    (*((volatile unsigned int*) 0x98040004)=0x0069e67c);//`WRITE_APB(16'h0004, 32'h0069e67c);
    (*((volatile unsigned int*) 0x98040004)=0x006ae0c0);//`WRITE_APB(16'h0004, 32'h006ae0c0);
    (*((volatile unsigned int*) 0x98040004)=0x006b005a);//`WRITE_APB(16'h0004, 32'h006b005a);
    (*((volatile unsigned int*) 0x98040004)=0x006ce008);//`WRITE_APB(16'h0004, 32'h006ce008);
    (*((volatile unsigned int*) 0x98040004)=0x006dffff);//`WRITE_APB(16'h0004, 32'h006dffff);
    (*((volatile unsigned int*) 0x98040004)=0x006e3cee);//`WRITE_APB(16'h0004, 32'h006e3cee);
    (*((volatile unsigned int*) 0x98040004)=0x006fe0c0);//`WRITE_APB(16'h0004, 32'h006fe0c0);
    (*((volatile unsigned int*) 0x98040004)=0x0070005b);//`WRITE_APB(16'h0004, 32'h0070005b);
    (*((volatile unsigned int*) 0x98040004)=0x0071e0c1);//`WRITE_APB(16'h0004, 32'h0071e0c1);
    (*((volatile unsigned int*) 0x98040004)=0x0072005e);//`WRITE_APB(16'h0004, 32'h0072005e);
    (*((volatile unsigned int*) 0x98040004)=0x0073ae11);//`WRITE_APB(16'h0004, 32'h0073ae11);
    (*((volatile unsigned int*) 0x98040004)=0x0074e056);//`WRITE_APB(16'h0004, 32'h0074e056);
    (*((volatile unsigned int*) 0x98040004)=0x00753cef);//`WRITE_APB(16'h0004, 32'h00753cef);
    (*((volatile unsigned int*) 0x98040004)=0x0076e40e);//`WRITE_APB(16'h0004, 32'h0076e40e);
    (*((volatile unsigned int*) 0x98040004)=0x00770039);//`WRITE_APB(16'h0004, 32'h00770039);
    (*((volatile unsigned int*) 0x98040004)=0x0078e0c0);//`WRITE_APB(16'h0004, 32'h0078e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x0079042b);//`WRITE_APB(16'h0004, 32'h0079042b);
    (*((volatile unsigned int*) 0x98040004)=0x007ae42a);//`WRITE_APB(16'h0004, 32'h007ae42a);
    (*((volatile unsigned int*) 0x98040004)=0x007be41e);//`WRITE_APB(16'h0004, 32'h007be41e);
    (*((volatile unsigned int*) 0x98040004)=0x007c013e);//`WRITE_APB(16'h0004, 32'h007c013e);
    (*((volatile unsigned int*) 0x98040004)=0x007de09e);//`WRITE_APB(16'h0004, 32'h007de09e);
    (*((volatile unsigned int*) 0x98040004)=0x007ea202);//`WRITE_APB(16'h0004, 32'h007ea202);
    (*((volatile unsigned int*) 0x98040004)=0x007fae3e);//`WRITE_APB(16'h0004, 32'h007fae3e);
    (*((volatile unsigned int*) 0x98040004)=0x00809f07);//`WRITE_APB(16'h0004, 32'h00809f07);
    (*((volatile unsigned int*) 0x98040004)=0x0081e0c0);//`WRITE_APB(16'h0004, 32'h0081e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x0082005c);//`WRITE_APB(16'h0004, 32'h0082005c);
    (*((volatile unsigned int*) 0x98040004)=0x0083e0c1);//`WRITE_APB(16'h0004, 32'h0083e0c1);
    (*((volatile unsigned int*) 0x98040004)=0x00840059);//`WRITE_APB(16'h0004, 32'h00840059);
    (*((volatile unsigned int*) 0x98040004)=0x00853eed);//`WRITE_APB(16'h0004, 32'h00853eed);
    (*((volatile unsigned int*) 0x98040004)=0x0086a203);//`WRITE_APB(16'h0004, 32'h0086a203);
    (*((volatile unsigned int*) 0x98040004)=0x00875aed);//`WRITE_APB(16'h0004, 32'h00875aed);
    (*((volatile unsigned int*) 0x98040004)=0x0088e052);//`WRITE_APB(16'h0004, 32'h0088e052);
    (*((volatile unsigned int*) 0x98040004)=0x0089b615);//`WRITE_APB(16'h0004, 32'h0089b615);
    (*((volatile unsigned int*) 0x98040004)=0x008ae0c3);//`WRITE_APB(16'h0004, 32'h008ae0c3);
    (*((volatile unsigned int*) 0x98040004)=0x008b005d);//`WRITE_APB(16'h0004, 32'h008b005d);
    (*((volatile unsigned int*) 0x98040004)=0x008ce01a);//`WRITE_APB(16'h0004, 32'h008ce01a);
    (*((volatile unsigned int*) 0x98040004)=0x008dce00);//`WRITE_APB(16'h0004, 32'h008dce00);
    (*((volatile unsigned int*) 0x98040004)=0x008ea200);//`WRITE_APB(16'h0004, 32'h008ea200);
    (*((volatile unsigned int*) 0x98040004)=0x008fe0c2);//`WRITE_APB(16'h0004, 32'h008fe0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00900059);//`WRITE_APB(16'h0004, 32'h00900059);
    (*((volatile unsigned int*) 0x98040004)=0x0091e0c2);//`WRITE_APB(16'h0004, 32'h0091e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00920058);//`WRITE_APB(16'h0004, 32'h00920058);
    (*((volatile unsigned int*) 0x98040004)=0x0093e0c2);//`WRITE_APB(16'h0004, 32'h0093e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00940008);//`WRITE_APB(16'h0004, 32'h00940008);
    (*((volatile unsigned int*) 0x98040004)=0x0095f00e);//`WRITE_APB(16'h0004, 32'h0095f00e);
    (*((volatile unsigned int*) 0x98040004)=0x0096e004);//`WRITE_APB(16'h0004, 32'h0096e004);
    (*((volatile unsigned int*) 0x98040004)=0x00970080);//`WRITE_APB(16'h0004, 32'h00970080);
    (*((volatile unsigned int*) 0x98040004)=0x0098e0c2);//`WRITE_APB(16'h0004, 32'h0098e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00990178);//`WRITE_APB(16'h0004, 32'h00990178);
    (*((volatile unsigned int*) 0x98040004)=0x009ae0c0);//`WRITE_APB(16'h0004, 32'h009ae0c0);
    (*((volatile unsigned int*) 0x98040004)=0x009b0059);//`WRITE_APB(16'h0004, 32'h009b0059);
    (*((volatile unsigned int*) 0x98040004)=0x009cf7ea);//`WRITE_APB(16'h0004, 32'h009cf7ea);
    (*((volatile unsigned int*) 0x98040004)=0x009da11e);//`WRITE_APB(16'h0004, 32'h009da11e);
    (*((volatile unsigned int*) 0x98040004)=0x009ef198);//`WRITE_APB(16'h0004, 32'h009ef198);
    (*((volatile unsigned int*) 0x98040004)=0x009fa202);//`WRITE_APB(16'h0004, 32'h009fa202);
    (*((volatile unsigned int*) 0x98040004)=0x00a0e0c2);//`WRITE_APB(16'h0004, 32'h00a0e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00a10058);//`WRITE_APB(16'h0004, 32'h00a10058);
    (*((volatile unsigned int*) 0x98040004)=0x00a2e004);//`WRITE_APB(16'h0004, 32'h00a2e004);
    (*((volatile unsigned int*) 0x98040004)=0x00a3e428);//`WRITE_APB(16'h0004, 32'h00a3e428);
    (*((volatile unsigned int*) 0x98040004)=0x00a4ae20);//`WRITE_APB(16'h0004, 32'h00a4ae20);
    (*((volatile unsigned int*) 0x98040004)=0x00a5e005);//`WRITE_APB(16'h0004, 32'h00a5e005);
    (*((volatile unsigned int*) 0x98040004)=0x00a63001);//`WRITE_APB(16'h0004, 32'h00a63001);
    (*((volatile unsigned int*) 0x98040004)=0x00a7e056);//`WRITE_APB(16'h0004, 32'h00a7e056);
    (*((volatile unsigned int*) 0x98040004)=0x00a8e0c2);//`WRITE_APB(16'h0004, 32'h00a8e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00a90070);//`WRITE_APB(16'h0004, 32'h00a90070);
    (*((volatile unsigned int*) 0x98040004)=0x00aae004);//`WRITE_APB(16'h0004, 32'h00aae004);
    (*((volatile unsigned int*) 0x98040004)=0x00ab0000);//`WRITE_APB(16'h0004, 32'h00ab0000);
    (*((volatile unsigned int*) 0x98040004)=0x00acae20);//`WRITE_APB(16'h0004, 32'h00acae20);
    (*((volatile unsigned int*) 0x98040004)=0x00ade00a);//`WRITE_APB(16'h0004, 32'h00ade00a);
    (*((volatile unsigned int*) 0x98040004)=0x00ae0000);//`WRITE_APB(16'h0004, 32'h00ae0000);
    (*((volatile unsigned int*) 0x98040004)=0x00afe0c2);//`WRITE_APB(16'h0004, 32'h00afe0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00b00071);//`WRITE_APB(16'h0004, 32'h00b00071);
    (*((volatile unsigned int*) 0x98040004)=0x00b1a200);//`WRITE_APB(16'h0004, 32'h00b1a200);
    (*((volatile unsigned int*) 0x98040004)=0x00b2e0c2);//`WRITE_APB(16'h0004, 32'h00b2e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00b30059);//`WRITE_APB(16'h0004, 32'h00b30059);
    (*((volatile unsigned int*) 0x98040004)=0x00b4e0c2);//`WRITE_APB(16'h0004, 32'h00b4e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00b50058);//`WRITE_APB(16'h0004, 32'h00b50058);
    (*((volatile unsigned int*) 0x98040004)=0x00b6f64e);//`WRITE_APB(16'h0004, 32'h00b6f64e);
    (*((volatile unsigned int*) 0x98040004)=0x00b7a202);//`WRITE_APB(16'h0004, 32'h00b7a202);
    (*((volatile unsigned int*) 0x98040004)=0x00b8e0c2);//`WRITE_APB(16'h0004, 32'h00b8e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00b90008);//`WRITE_APB(16'h0004, 32'h00b90008);
    (*((volatile unsigned int*) 0x98040004)=0x00bae0c2);//`WRITE_APB(16'h0004, 32'h00bae0c2);
    (*((volatile unsigned int*) 0x98040004)=0x00bb0058);//`WRITE_APB(16'h0004, 32'h00bb0058);
    (*((volatile unsigned int*) 0x98040004)=0x00bcd027);//`WRITE_APB(16'h0004, 32'h00bcd027);
    (*((volatile unsigned int*) 0x98040004)=0x00bd0001);//`WRITE_APB(16'h0004, 32'h00bd0001);
    (*((volatile unsigned int*) 0x98040004)=0x00bee42e);//`WRITE_APB(16'h0004, 32'h00bee42e);
    (*((volatile unsigned int*) 0x98040004)=0x00bfe0c1);//`WRITE_APB(16'h0004, 32'h00bfe0c1);
    (*((volatile unsigned int*) 0x98040004)=0x00c0005b);//`WRITE_APB(16'h0004, 32'h00c0005b);
    (*((volatile unsigned int*) 0x98040004)=0x00c1f19b);//`WRITE_APB(16'h0004, 32'h00c1f19b);
    (*((volatile unsigned int*) 0x98040004)=0x00c2a23e);//`WRITE_APB(16'h0004, 32'h00c2a23e);
    (*((volatile unsigned int*) 0x98040004)=0x00c3a103);//`WRITE_APB(16'h0004, 32'h00c3a103);
    (*((volatile unsigned int*) 0x98040004)=0x00c4f31b);//`WRITE_APB(16'h0004, 32'h00c4f31b);
    (*((volatile unsigned int*) 0x98040004)=0x00c5a24e);//`WRITE_APB(16'h0004, 32'h00c5a24e);
    (*((volatile unsigned int*) 0x98040004)=0x00c6a103);//`WRITE_APB(16'h0004, 32'h00c6a103);
    (*((volatile unsigned int*) 0x98040004)=0x00c7f2eb);//`WRITE_APB(16'h0004, 32'h00c7f2eb);
    (*((volatile unsigned int*) 0x98040004)=0x00c8a103);//`WRITE_APB(16'h0004, 32'h00c8a103);
    (*((volatile unsigned int*) 0x98040004)=0x00c9f17b);//`WRITE_APB(16'h0004, 32'h00c9f17b);
    (*((volatile unsigned int*) 0x98040004)=0x00cae004);//`WRITE_APB(16'h0004, 32'h00cae004);
    (*((volatile unsigned int*) 0x98040004)=0x00cb003b);//`WRITE_APB(16'h0004, 32'h00cb003b);
    (*((volatile unsigned int*) 0x98040004)=0x00cca103);//`WRITE_APB(16'h0004, 32'h00cca103);
    (*((volatile unsigned int*) 0x98040004)=0x00cdf28b);//`WRITE_APB(16'h0004, 32'h00cdf28b);
    (*((volatile unsigned int*) 0x98040004)=0x00cee004);//`WRITE_APB(16'h0004, 32'h00cee004);
    (*((volatile unsigned int*) 0x98040004)=0x00cf0040);//`WRITE_APB(16'h0004, 32'h00cf0040);
    (*((volatile unsigned int*) 0x98040004)=0x00d0a103);//`WRITE_APB(16'h0004, 32'h00d0a103);
    (*((volatile unsigned int*) 0x98040004)=0x00d1f24b);//`WRITE_APB(16'h0004, 32'h00d1f24b);
    (*((volatile unsigned int*) 0x98040004)=0x00d2a105);//`WRITE_APB(16'h0004, 32'h00d2a105);
    (*((volatile unsigned int*) 0x98040004)=0x00d3f13b);//`WRITE_APB(16'h0004, 32'h00d3f13b);
    (*((volatile unsigned int*) 0x98040004)=0x00d4a103);//`WRITE_APB(16'h0004, 32'h00d4a103);
    (*((volatile unsigned int*) 0x98040004)=0x00d5f19b);//`WRITE_APB(16'h0004, 32'h00d5f19b);
    (*((volatile unsigned int*) 0x98040004)=0x00d6e004);//`WRITE_APB(16'h0004, 32'h00d6e004);
    (*((volatile unsigned int*) 0x98040004)=0x00d70062);//`WRITE_APB(16'h0004, 32'h00d70062);
    (*((volatile unsigned int*) 0x98040004)=0x00d8a107);//`WRITE_APB(16'h0004, 32'h00d8a107);
    (*((volatile unsigned int*) 0x98040004)=0x00d9f1cb);//`WRITE_APB(16'h0004, 32'h00d9f1cb);
    (*((volatile unsigned int*) 0x98040004)=0x00daa200);//`WRITE_APB(16'h0004, 32'h00daa200);
    (*((volatile unsigned int*) 0x98040004)=0x00dbe0c1);//`WRITE_APB(16'h0004, 32'h00dbe0c1);
    (*((volatile unsigned int*) 0x98040004)=0x00dc005e);//`WRITE_APB(16'h0004, 32'h00dc005e);
    (*((volatile unsigned int*) 0x98040004)=0x00ddf18b);//`WRITE_APB(16'h0004, 32'h00ddf18b);
    (*((volatile unsigned int*) 0x98040004)=0x00dea22a);//`WRITE_APB(16'h0004, 32'h00dea22a);
    (*((volatile unsigned int*) 0x98040004)=0x00dff16e);//`WRITE_APB(16'h0004, 32'h00dff16e);
    (*((volatile unsigned int*) 0x98040004)=0x00e0a25a);//`WRITE_APB(16'h0004, 32'h00e0a25a);
    (*((volatile unsigned int*) 0x98040004)=0x00e1e0c1);//`WRITE_APB(16'h0004, 32'h00e1e0c1);
    (*((volatile unsigned int*) 0x98040004)=0x00e2005e);//`WRITE_APB(16'h0004, 32'h00e2005e);
    (*((volatile unsigned int*) 0x98040004)=0x00e3f12b);//`WRITE_APB(16'h0004, 32'h00e3f12b);
    (*((volatile unsigned int*) 0x98040004)=0x00e4a010);//`WRITE_APB(16'h0004, 32'h00e4a010);
    (*((volatile unsigned int*) 0x98040004)=0x00e5f10e);//`WRITE_APB(16'h0004, 32'h00e5f10e);
    (*((volatile unsigned int*) 0x98040004)=0x00e6e004);//`WRITE_APB(16'h0004, 32'h00e6e004);
    (*((volatile unsigned int*) 0x98040004)=0x00e70046);//`WRITE_APB(16'h0004, 32'h00e70046);
    (*((volatile unsigned int*) 0x98040004)=0x00e8e0c1);//`WRITE_APB(16'h0004, 32'h00e8e0c1);
    (*((volatile unsigned int*) 0x98040004)=0x00e9005e);//`WRITE_APB(16'h0004, 32'h00e9005e);
    (*((volatile unsigned int*) 0x98040004)=0x00eaf0bb);//`WRITE_APB(16'h0004, 32'h00eaf0bb);
    (*((volatile unsigned int*) 0x98040004)=0x00ebe004);//`WRITE_APB(16'h0004, 32'h00ebe004);
    (*((volatile unsigned int*) 0x98040004)=0x00ec004e);//`WRITE_APB(16'h0004, 32'h00ec004e);
    (*((volatile unsigned int*) 0x98040004)=0x00edf08e);//`WRITE_APB(16'h0004, 32'h00edf08e);
    (*((volatile unsigned int*) 0x98040004)=0x00eee004);//`WRITE_APB(16'h0004, 32'h00eee004);
    (*((volatile unsigned int*) 0x98040004)=0x00ef0058);//`WRITE_APB(16'h0004, 32'h00ef0058);
    (*((volatile unsigned int*) 0x98040004)=0x00f0e0c1);//`WRITE_APB(16'h0004, 32'h00f0e0c1);
    (*((volatile unsigned int*) 0x98040004)=0x00f1005e);//`WRITE_APB(16'h0004, 32'h00f1005e);
    (*((volatile unsigned int*) 0x98040004)=0x00f2f03b);//`WRITE_APB(16'h0004, 32'h00f2f03b);
    (*((volatile unsigned int*) 0x98040004)=0x00f3e004);//`WRITE_APB(16'h0004, 32'h00f3e004);
    (*((volatile unsigned int*) 0x98040004)=0x00f40068);//`WRITE_APB(16'h0004, 32'h00f40068);
    (*((volatile unsigned int*) 0x98040004)=0x00f5ae16);//`WRITE_APB(16'h0004, 32'h00f5ae16);
    (*((volatile unsigned int*) 0x98040004)=0x00f6e0c1);//`WRITE_APB(16'h0004, 32'h00f6e0c1);
    (*((volatile unsigned int*) 0x98040004)=0x00f70040);//`WRITE_APB(16'h0004, 32'h00f70040);
    (*((volatile unsigned int*) 0x98040004)=0x00f8e041);//`WRITE_APB(16'h0004, 32'h00f8e041);
    (*((volatile unsigned int*) 0x98040004)=0x00f9ce21);//`WRITE_APB(16'h0004, 32'h00f9ce21);
    (*((volatile unsigned int*) 0x98040004)=0x00fad111);//`WRITE_APB(16'h0004, 32'h00fad111);
    (*((volatile unsigned int*) 0x98040004)=0x00fb0000);//`WRITE_APB(16'h0004, 32'h00fb0000);
    (*((volatile unsigned int*) 0x98040004)=0x00fcd112);//`WRITE_APB(16'h0004, 32'h00fcd112);
    (*((volatile unsigned int*) 0x98040004)=0x00fd2800);//`WRITE_APB(16'h0004, 32'h00fd2800);
    (*((volatile unsigned int*) 0x98040004)=0x00fed113);//`WRITE_APB(16'h0004, 32'h00fed113);
    (*((volatile unsigned int*) 0x98040004)=0x00ff000b);//`WRITE_APB(16'h0004, 32'h00ff000b);
    (*((volatile unsigned int*) 0x98040004)=0x0100e1e1);//`WRITE_APB(16'h0004, 32'h0100e1e1);
    (*((volatile unsigned int*) 0x98040004)=0x0101e42e);//`WRITE_APB(16'h0004, 32'h0101e42e);
    (*((volatile unsigned int*) 0x98040004)=0x0102e41e);//`WRITE_APB(16'h0004, 32'h0102e41e);
    (*((volatile unsigned int*) 0x98040004)=0x01030252);//`WRITE_APB(16'h0004, 32'h01030252);
    (*((volatile unsigned int*) 0x98040004)=0x0104e0c0);//`WRITE_APB(16'h0004, 32'h0104e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x01050059);//`WRITE_APB(16'h0004, 32'h01050059);
    (*((volatile unsigned int*) 0x98040004)=0x0106a102);//`WRITE_APB(16'h0004, 32'h0106a102);
    (*((volatile unsigned int*) 0x98040004)=0x0107e42a);//`WRITE_APB(16'h0004, 32'h0107e42a);
    (*((volatile unsigned int*) 0x98040004)=0x0108e0c0);//`WRITE_APB(16'h0004, 32'h0108e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x0109005b);//`WRITE_APB(16'h0004, 32'h0109005b);
    (*((volatile unsigned int*) 0x98040004)=0x010aa810);//`WRITE_APB(16'h0004, 32'h010aa810);
    (*((volatile unsigned int*) 0x98040004)=0x010bf098);//`WRITE_APB(16'h0004, 32'h010bf098);
    (*((volatile unsigned int*) 0x98040004)=0x010ce0c0);//`WRITE_APB(16'h0004, 32'h010ce0c0);
    (*((volatile unsigned int*) 0x98040004)=0x010d005b);//`WRITE_APB(16'h0004, 32'h010d005b);
    (*((volatile unsigned int*) 0x98040004)=0x010ea10c);//`WRITE_APB(16'h0004, 32'h010ea10c);
    (*((volatile unsigned int*) 0x98040004)=0x010fe42a);//`WRITE_APB(16'h0004, 32'h010fe42a);
    (*((volatile unsigned int*) 0x98040004)=0x0110a206);//`WRITE_APB(16'h0004, 32'h0110a206);
    (*((volatile unsigned int*) 0x98040004)=0x0111e41e);//`WRITE_APB(16'h0004, 32'h0111e41e);
    (*((volatile unsigned int*) 0x98040004)=0x0112012d);//`WRITE_APB(16'h0004, 32'h0112012d);
    (*((volatile unsigned int*) 0x98040004)=0x0113e42e);//`WRITE_APB(16'h0004, 32'h0113e42e);
    (*((volatile unsigned int*) 0x98040004)=0x0114e004);//`WRITE_APB(16'h0004, 32'h0114e004);
    (*((volatile unsigned int*) 0x98040004)=0x0115039a);//`WRITE_APB(16'h0004, 32'h0115039a);
    (*((volatile unsigned int*) 0x98040004)=0x0116e67c);//`WRITE_APB(16'h0004, 32'h0116e67c);
    (*((volatile unsigned int*) 0x98040004)=0x0117e0c0);//`WRITE_APB(16'h0004, 32'h0117e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x0118005b);//`WRITE_APB(16'h0004, 32'h0118005b);
    (*((volatile unsigned int*) 0x98040004)=0x0119a810);//`WRITE_APB(16'h0004, 32'h0119a810);
    (*((volatile unsigned int*) 0x98040004)=0x011af058);//`WRITE_APB(16'h0004, 32'h011af058);
    (*((volatile unsigned int*) 0x98040004)=0x011ba204);//`WRITE_APB(16'h0004, 32'h011ba204);
    (*((volatile unsigned int*) 0x98040004)=0x011ce41e);//`WRITE_APB(16'h0004, 32'h011ce41e);
    (*((volatile unsigned int*) 0x98040004)=0x011d012d);//`WRITE_APB(16'h0004, 32'h011d012d);
    (*((volatile unsigned int*) 0x98040004)=0x011ee42e);//`WRITE_APB(16'h0004, 32'h011ee42e);
    (*((volatile unsigned int*) 0x98040004)=0x011fe004);//`WRITE_APB(16'h0004, 32'h011fe004);
    (*((volatile unsigned int*) 0x98040004)=0x0120039c);//`WRITE_APB(16'h0004, 32'h0120039c);
    (*((volatile unsigned int*) 0x98040004)=0x0121e67c);//`WRITE_APB(16'h0004, 32'h0121e67c);
    (*((volatile unsigned int*) 0x98040004)=0x0122a200);//`WRITE_APB(16'h0004, 32'h0122a200);
    (*((volatile unsigned int*) 0x98040004)=0x0123c401);//`WRITE_APB(16'h0004, 32'h0123c401);
    (*((volatile unsigned int*) 0x98040004)=0x0124e188);//`WRITE_APB(16'h0004, 32'h0124e188);
    (*((volatile unsigned int*) 0x98040004)=0x012500eb);//`WRITE_APB(16'h0004, 32'h012500eb);
    (*((volatile unsigned int*) 0x98040004)=0x01263d11);//`WRITE_APB(16'h0004, 32'h01263d11);
    (*((volatile unsigned int*) 0x98040004)=0x0127e161);//`WRITE_APB(16'h0004, 32'h0127e161);
    (*((volatile unsigned int*) 0x98040004)=0x012800f6);//`WRITE_APB(16'h0004, 32'h012800f6);
    (*((volatile unsigned int*) 0x98040004)=0x0129e188);//`WRITE_APB(16'h0004, 32'h0129e188);
    (*((volatile unsigned int*) 0x98040004)=0x012a0b09);//`WRITE_APB(16'h0004, 32'h012a0b09);
    (*((volatile unsigned int*) 0x98040004)=0x012b3d11);//`WRITE_APB(16'h0004, 32'h012b3d11);
    (*((volatile unsigned int*) 0x98040004)=0x012ce42e);//`WRITE_APB(16'h0004, 32'h012ce42e);
    (*((volatile unsigned int*) 0x98040004)=0x012d3cec);//`WRITE_APB(16'h0004, 32'h012d3cec);
    (*((volatile unsigned int*) 0x98040004)=0x012ee0c0);//`WRITE_APB(16'h0004, 32'h012ee0c0);
    (*((volatile unsigned int*) 0x98040004)=0x012f0041);//`WRITE_APB(16'h0004, 32'h012f0041);
    (*((volatile unsigned int*) 0x98040004)=0x0130e005);//`WRITE_APB(16'h0004, 32'h0130e005);
    (*((volatile unsigned int*) 0x98040004)=0x01310000);//`WRITE_APB(16'h0004, 32'h01310000);
    (*((volatile unsigned int*) 0x98040004)=0x0132ae11);//`WRITE_APB(16'h0004, 32'h0132ae11);
    (*((volatile unsigned int*) 0x98040004)=0x0133e042);//`WRITE_APB(16'h0004, 32'h0133e042);
    (*((volatile unsigned int*) 0x98040004)=0x0134ce20);//`WRITE_APB(16'h0004, 32'h0134ce20);
    (*((volatile unsigned int*) 0x98040004)=0x0135d111);//`WRITE_APB(16'h0004, 32'h0135d111);
    (*((volatile unsigned int*) 0x98040004)=0x01360000);//`WRITE_APB(16'h0004, 32'h01360000);
    (*((volatile unsigned int*) 0x98040004)=0x0137d112);//`WRITE_APB(16'h0004, 32'h0137d112);
    (*((volatile unsigned int*) 0x98040004)=0x01380c00);//`WRITE_APB(16'h0004, 32'h01380c00);
    (*((volatile unsigned int*) 0x98040004)=0x013988ec);//`WRITE_APB(16'h0004, 32'h013988ec);
    (*((volatile unsigned int*) 0x98040004)=0x013a0113);//`WRITE_APB(16'h0004, 32'h013a0113);
    (*((volatile unsigned int*) 0x98040004)=0x013be41e);//`WRITE_APB(16'h0004, 32'h013be41e);
    (*((volatile unsigned int*) 0x98040004)=0x013c01ee);//`WRITE_APB(16'h0004, 32'h013c01ee);
    (*((volatile unsigned int*) 0x98040004)=0x013de42e);//`WRITE_APB(16'h0004, 32'h013de42e);
    (*((volatile unsigned int*) 0x98040004)=0x013ea200);//`WRITE_APB(16'h0004, 32'h013ea200);
    (*((volatile unsigned int*) 0x98040004)=0x013fe0c1);//`WRITE_APB(16'h0004, 32'h013fe0c1);
    (*((volatile unsigned int*) 0x98040004)=0x01400059);//`WRITE_APB(16'h0004, 32'h01400059);
    (*((volatile unsigned int*) 0x98040004)=0x0141a107);//`WRITE_APB(16'h0004, 32'h0141a107);
    (*((volatile unsigned int*) 0x98040004)=0x0142b6d6);//`WRITE_APB(16'h0004, 32'h0142b6d6);
    (*((volatile unsigned int*) 0x98040004)=0x0143a107);//`WRITE_APB(16'h0004, 32'h0143a107);
    (*((volatile unsigned int*) 0x98040004)=0x0144b636);//`WRITE_APB(16'h0004, 32'h0144b636);
    (*((volatile unsigned int*) 0x98040004)=0x0145e000);//`WRITE_APB(16'h0004, 32'h0145e000);
    (*((volatile unsigned int*) 0x98040004)=0x01460070);//`WRITE_APB(16'h0004, 32'h01460070);
    (*((volatile unsigned int*) 0x98040004)=0x0147e42e);//`WRITE_APB(16'h0004, 32'h0147e42e);
    (*((volatile unsigned int*) 0x98040004)=0x0148e0c0);//`WRITE_APB(16'h0004, 32'h0148e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x0149006b);//`WRITE_APB(16'h0004, 32'h0149006b);
    (*((volatile unsigned int*) 0x98040004)=0x014ae167);//`WRITE_APB(16'h0004, 32'h014ae167);
    (*((volatile unsigned int*) 0x98040004)=0x014b01a0);//`WRITE_APB(16'h0004, 32'h014b01a0);
    (*((volatile unsigned int*) 0x98040004)=0x014c3d07);//`WRITE_APB(16'h0004, 32'h014c3d07);
    (*((volatile unsigned int*) 0x98040004)=0x014de0c0);//`WRITE_APB(16'h0004, 32'h014de0c0);
    (*((volatile unsigned int*) 0x98040004)=0x014e0414);//`WRITE_APB(16'h0004, 32'h014e0414);
    (*((volatile unsigned int*) 0x98040004)=0x014fe428);//`WRITE_APB(16'h0004, 32'h014fe428);
    (*((volatile unsigned int*) 0x98040004)=0x0150e0c1);//`WRITE_APB(16'h0004, 32'h0150e0c1);
    (*((volatile unsigned int*) 0x98040004)=0x01510044);//`WRITE_APB(16'h0004, 32'h01510044);
    (*((volatile unsigned int*) 0x98040004)=0x0152a809);//`WRITE_APB(16'h0004, 32'h0152a809);
    (*((volatile unsigned int*) 0x98040004)=0x0153ae33);//`WRITE_APB(16'h0004, 32'h0153ae33);
    (*((volatile unsigned int*) 0x98040004)=0x0154e0c0);//`WRITE_APB(16'h0004, 32'h0154e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x0155006a);//`WRITE_APB(16'h0004, 32'h0155006a);
    (*((volatile unsigned int*) 0x98040004)=0x0156e167);//`WRITE_APB(16'h0004, 32'h0156e167);
    (*((volatile unsigned int*) 0x98040004)=0x015701a0);//`WRITE_APB(16'h0004, 32'h015701a0);
    (*((volatile unsigned int*) 0x98040004)=0x01583517);//`WRITE_APB(16'h0004, 32'h01583517);
    (*((volatile unsigned int*) 0x98040004)=0x01593d17);//`WRITE_APB(16'h0004, 32'h01593d17);
    (*((volatile unsigned int*) 0x98040004)=0x015ae0c0);//`WRITE_APB(16'h0004, 32'h015ae0c0);
    (*((volatile unsigned int*) 0x98040004)=0x015b006b);//`WRITE_APB(16'h0004, 32'h015b006b);
    (*((volatile unsigned int*) 0x98040004)=0x015ce056);//`WRITE_APB(16'h0004, 32'h015ce056);
    (*((volatile unsigned int*) 0x98040004)=0x015d3517);//`WRITE_APB(16'h0004, 32'h015d3517);
    (*((volatile unsigned int*) 0x98040004)=0x015e3d07);//`WRITE_APB(16'h0004, 32'h015e3d07);
    (*((volatile unsigned int*) 0x98040004)=0x015fe42e);//`WRITE_APB(16'h0004, 32'h015fe42e);
    (*((volatile unsigned int*) 0x98040004)=0x0160e167);//`WRITE_APB(16'h0004, 32'h0160e167);
    (*((volatile unsigned int*) 0x98040004)=0x016101a0);//`WRITE_APB(16'h0004, 32'h016101a0);
    (*((volatile unsigned int*) 0x98040004)=0x01622907);//`WRITE_APB(16'h0004, 32'h01622907);
    (*((volatile unsigned int*) 0x98040004)=0x0163e0c2);//`WRITE_APB(16'h0004, 32'h0163e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01640151);//`WRITE_APB(16'h0004, 32'h01640151);
    (*((volatile unsigned int*) 0x98040004)=0x0165a204);//`WRITE_APB(16'h0004, 32'h0165a204);
    (*((volatile unsigned int*) 0x98040004)=0x0166e0c2);//`WRITE_APB(16'h0004, 32'h0166e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01670150);//`WRITE_APB(16'h0004, 32'h01670150);
    (*((volatile unsigned int*) 0x98040004)=0x0168e0c0);//`WRITE_APB(16'h0004, 32'h0168e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x01690150);//`WRITE_APB(16'h0004, 32'h01690150);
    (*((volatile unsigned int*) 0x98040004)=0x016aa804);//`WRITE_APB(16'h0004, 32'h016aa804);
    (*((volatile unsigned int*) 0x98040004)=0x016bf7da);//`WRITE_APB(16'h0004, 32'h016bf7da);
    (*((volatile unsigned int*) 0x98040004)=0x016ca208);//`WRITE_APB(16'h0004, 32'h016ca208);
    (*((volatile unsigned int*) 0x98040004)=0x016de0c2);//`WRITE_APB(16'h0004, 32'h016de0c2);
    (*((volatile unsigned int*) 0x98040004)=0x016e0150);//`WRITE_APB(16'h0004, 32'h016e0150);
    (*((volatile unsigned int*) 0x98040004)=0x016fe42e);//`WRITE_APB(16'h0004, 32'h016fe42e);
    (*((volatile unsigned int*) 0x98040004)=0x0170e0c0);//`WRITE_APB(16'h0004, 32'h0170e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x01710059);//`WRITE_APB(16'h0004, 32'h01710059);
    (*((volatile unsigned int*) 0x98040004)=0x0172a102);//`WRITE_APB(16'h0004, 32'h0172a102);
    (*((volatile unsigned int*) 0x98040004)=0x0173e41a);//`WRITE_APB(16'h0004, 32'h0173e41a);
    (*((volatile unsigned int*) 0x98040004)=0x01740182);//`WRITE_APB(16'h0004, 32'h01740182);
    (*((volatile unsigned int*) 0x98040004)=0x0175e0c0);//`WRITE_APB(16'h0004, 32'h0175e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x01760059);//`WRITE_APB(16'h0004, 32'h01760059);
    (*((volatile unsigned int*) 0x98040004)=0x0177a106);//`WRITE_APB(16'h0004, 32'h0177a106);
    (*((volatile unsigned int*) 0x98040004)=0x0178e41a);//`WRITE_APB(16'h0004, 32'h0178e41a);
    (*((volatile unsigned int*) 0x98040004)=0x0179018c);//`WRITE_APB(16'h0004, 32'h0179018c);
    (*((volatile unsigned int*) 0x98040004)=0x017ae0c0);//`WRITE_APB(16'h0004, 32'h017ae0c0);
    (*((volatile unsigned int*) 0x98040004)=0x017b0047);//`WRITE_APB(16'h0004, 32'h017b0047);
    (*((volatile unsigned int*) 0x98040004)=0x017ce0c2);//`WRITE_APB(16'h0004, 32'h017ce0c2);
    (*((volatile unsigned int*) 0x98040004)=0x017d0048);//`WRITE_APB(16'h0004, 32'h017d0048);
    (*((volatile unsigned int*) 0x98040004)=0x017ea200);//`WRITE_APB(16'h0004, 32'h017ea200);
    (*((volatile unsigned int*) 0x98040004)=0x017fe0c2);//`WRITE_APB(16'h0004, 32'h017fe0c2);
    (*((volatile unsigned int*) 0x98040004)=0x0180004a);//`WRITE_APB(16'h0004, 32'h0180004a);
    (*((volatile unsigned int*) 0x98040004)=0x0181e42e);//`WRITE_APB(16'h0004, 32'h0181e42e);
    (*((volatile unsigned int*) 0x98040004)=0x0182a23e);//`WRITE_APB(16'h0004, 32'h0182a23e);
    (*((volatile unsigned int*) 0x98040004)=0x01833cf0);//`WRITE_APB(16'h0004, 32'h01833cf0);
    (*((volatile unsigned int*) 0x98040004)=0x0184a202);//`WRITE_APB(16'h0004, 32'h0184a202);
    (*((volatile unsigned int*) 0x98040004)=0x018558f0);//`WRITE_APB(16'h0004, 32'h018558f0);
    (*((volatile unsigned int*) 0x98040004)=0x0186e0c2);//`WRITE_APB(16'h0004, 32'h0186e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01870078);//`WRITE_APB(16'h0004, 32'h01870078);
    (*((volatile unsigned int*) 0x98040004)=0x0188a220);//`WRITE_APB(16'h0004, 32'h0188a220);
    (*((volatile unsigned int*) 0x98040004)=0x0189e0c2);//`WRITE_APB(16'h0004, 32'h0189e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x018a0070);//`WRITE_APB(16'h0004, 32'h018a0070);
    (*((volatile unsigned int*) 0x98040004)=0x018be42e);//`WRITE_APB(16'h0004, 32'h018be42e);
    (*((volatile unsigned int*) 0x98040004)=0x018ca220);//`WRITE_APB(16'h0004, 32'h018ca220);
    (*((volatile unsigned int*) 0x98040004)=0x018de0c2);//`WRITE_APB(16'h0004, 32'h018de0c2);
    (*((volatile unsigned int*) 0x98040004)=0x018e0076);//`WRITE_APB(16'h0004, 32'h018e0076);
    (*((volatile unsigned int*) 0x98040004)=0x018fa200);//`WRITE_APB(16'h0004, 32'h018fa200);
    (*((volatile unsigned int*) 0x98040004)=0x0190e0c2);//`WRITE_APB(16'h0004, 32'h0190e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01910072);//`WRITE_APB(16'h0004, 32'h01910072);
    (*((volatile unsigned int*) 0x98040004)=0x0192a2fc);//`WRITE_APB(16'h0004, 32'h0192a2fc);
    (*((volatile unsigned int*) 0x98040004)=0x0193e0c2);//`WRITE_APB(16'h0004, 32'h0193e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01940077);//`WRITE_APB(16'h0004, 32'h01940077);
    (*((volatile unsigned int*) 0x98040004)=0x0195a2fa);//`WRITE_APB(16'h0004, 32'h0195a2fa);
    (*((volatile unsigned int*) 0x98040004)=0x0196e0c2);//`WRITE_APB(16'h0004, 32'h0196e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01970071);//`WRITE_APB(16'h0004, 32'h01970071);
    (*((volatile unsigned int*) 0x98040004)=0x0198e42e);//`WRITE_APB(16'h0004, 32'h0198e42e);
    (*((volatile unsigned int*) 0x98040004)=0x0199d022);//`WRITE_APB(16'h0004, 32'h0199d022);
    (*((volatile unsigned int*) 0x98040004)=0x019a270f);//`WRITE_APB(16'h0004, 32'h019a270f);
    (*((volatile unsigned int*) 0x98040004)=0x019be184);//`WRITE_APB(16'h0004, 32'h019be184);
    (*((volatile unsigned int*) 0x98040004)=0x019c01a6);//`WRITE_APB(16'h0004, 32'h019c01a6);
    (*((volatile unsigned int*) 0x98040004)=0x019de0c0);//`WRITE_APB(16'h0004, 32'h019de0c0);
    (*((volatile unsigned int*) 0x98040004)=0x019e004a);//`WRITE_APB(16'h0004, 32'h019e004a);
    (*((volatile unsigned int*) 0x98040004)=0x019ff048);//`WRITE_APB(16'h0004, 32'h019ff048);
    (*((volatile unsigned int*) 0x98040004)=0x01a0e0c0);//`WRITE_APB(16'h0004, 32'h01a0e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x01a1041f);//`WRITE_APB(16'h0004, 32'h01a1041f);
    (*((volatile unsigned int*) 0x98040004)=0x01a2f7e8);//`WRITE_APB(16'h0004, 32'h01a2f7e8);
    (*((volatile unsigned int*) 0x98040004)=0x01a3e0c0);//`WRITE_APB(16'h0004, 32'h01a3e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x01a4043d);//`WRITE_APB(16'h0004, 32'h01a4043d);
    (*((volatile unsigned int*) 0x98040004)=0x01a5a1ee);//`WRITE_APB(16'h0004, 32'h01a5a1ee);
    (*((volatile unsigned int*) 0x98040004)=0x01a6f7d8);//`WRITE_APB(16'h0004, 32'h01a6f7d8);
    (*((volatile unsigned int*) 0x98040004)=0x01a7a200);//`WRITE_APB(16'h0004, 32'h01a7a200);
    (*((volatile unsigned int*) 0x98040004)=0x01a8e0c2);//`WRITE_APB(16'h0004, 32'h01a8e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01a9041c);//`WRITE_APB(16'h0004, 32'h01a9041c);
    (*((volatile unsigned int*) 0x98040004)=0x01aae42e);//`WRITE_APB(16'h0004, 32'h01aae42e);
    (*((volatile unsigned int*) 0x98040004)=0x01abe41e);//`WRITE_APB(16'h0004, 32'h01abe41e);
    (*((volatile unsigned int*) 0x98040004)=0x01ac0199);//`WRITE_APB(16'h0004, 32'h01ac0199);
    (*((volatile unsigned int*) 0x98040004)=0x01ade004);//`WRITE_APB(16'h0004, 32'h01ade004);
    (*((volatile unsigned int*) 0x98040004)=0x01ae0011);//`WRITE_APB(16'h0004, 32'h01ae0011);
    (*((volatile unsigned int*) 0x98040004)=0x01afe0c2);//`WRITE_APB(16'h0004, 32'h01afe0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01b0043c);//`WRITE_APB(16'h0004, 32'h01b0043c);
    (*((volatile unsigned int*) 0x98040004)=0x01b1e42e);//`WRITE_APB(16'h0004, 32'h01b1e42e);
    (*((volatile unsigned int*) 0x98040004)=0x01b2e41e);//`WRITE_APB(16'h0004, 32'h01b2e41e);
    (*((volatile unsigned int*) 0x98040004)=0x01b301ab);//`WRITE_APB(16'h0004, 32'h01b301ab);
    (*((volatile unsigned int*) 0x98040004)=0x01b4e0c0);//`WRITE_APB(16'h0004, 32'h01b4e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x01b5043d);//`WRITE_APB(16'h0004, 32'h01b5043d);
    (*((volatile unsigned int*) 0x98040004)=0x01b6a1ee);//`WRITE_APB(16'h0004, 32'h01b6a1ee);
    (*((volatile unsigned int*) 0x98040004)=0x01b7f7d8);//`WRITE_APB(16'h0004, 32'h01b7f7d8);
    (*((volatile unsigned int*) 0x98040004)=0x01b8e004);//`WRITE_APB(16'h0004, 32'h01b8e004);
    (*((volatile unsigned int*) 0x98040004)=0x01b901e0);//`WRITE_APB(16'h0004, 32'h01b901e0);
    (*((volatile unsigned int*) 0x98040004)=0x01bae0c2);//`WRITE_APB(16'h0004, 32'h01bae0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01bb0009);//`WRITE_APB(16'h0004, 32'h01bb0009);
    (*((volatile unsigned int*) 0x98040004)=0x01bca200);//`WRITE_APB(16'h0004, 32'h01bca200);
    (*((volatile unsigned int*) 0x98040004)=0x01bde0c2);//`WRITE_APB(16'h0004, 32'h01bde0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01be0009);//`WRITE_APB(16'h0004, 32'h01be0009);
    (*((volatile unsigned int*) 0x98040004)=0x01bfe0c0);//`WRITE_APB(16'h0004, 32'h01bfe0c0);
    (*((volatile unsigned int*) 0x98040004)=0x01c0000d);//`WRITE_APB(16'h0004, 32'h01c0000d);
    (*((volatile unsigned int*) 0x98040004)=0x01c1f7e8);//`WRITE_APB(16'h0004, 32'h01c1f7e8);
    (*((volatile unsigned int*) 0x98040004)=0x01c2a200);//`WRITE_APB(16'h0004, 32'h01c2a200);
    (*((volatile unsigned int*) 0x98040004)=0x01c3e0c2);//`WRITE_APB(16'h0004, 32'h01c3e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01c4043c);//`WRITE_APB(16'h0004, 32'h01c4043c);
    (*((volatile unsigned int*) 0x98040004)=0x01c5e42e);//`WRITE_APB(16'h0004, 32'h01c5e42e);
    (*((volatile unsigned int*) 0x98040004)=0x01c6e0c0);//`WRITE_APB(16'h0004, 32'h01c6e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x01c70045);//`WRITE_APB(16'h0004, 32'h01c70045);
    (*((volatile unsigned int*) 0x98040004)=0x01c8af04);//`WRITE_APB(16'h0004, 32'h01c8af04);
    (*((volatile unsigned int*) 0x98040004)=0x01c9a80e);//`WRITE_APB(16'h0004, 32'h01c9a80e);
    (*((volatile unsigned int*) 0x98040004)=0x01caa104);//`WRITE_APB(16'h0004, 32'h01caa104);
    (*((volatile unsigned int*) 0x98040004)=0x01cbe428);//`WRITE_APB(16'h0004, 32'h01cbe428);
    (*((volatile unsigned int*) 0x98040004)=0x01cce161);//`WRITE_APB(16'h0004, 32'h01cce161);
    (*((volatile unsigned int*) 0x98040004)=0x01cd0144);//`WRITE_APB(16'h0004, 32'h01cd0144);
    (*((volatile unsigned int*) 0x98040004)=0x01ce28a9);//`WRITE_APB(16'h0004, 32'h01ce28a9);
    (*((volatile unsigned int*) 0x98040004)=0x01cfe094);//`WRITE_APB(16'h0004, 32'h01cfe094);
    (*((volatile unsigned int*) 0x98040004)=0x01d0e412);//`WRITE_APB(16'h0004, 32'h01d0e412);
    (*((volatile unsigned int*) 0x98040004)=0x01d11b20);//`WRITE_APB(16'h0004, 32'h01d11b20);
    (*((volatile unsigned int*) 0x98040004)=0x01d228a9);//`WRITE_APB(16'h0004, 32'h01d228a9);
    (*((volatile unsigned int*) 0x98040004)=0x01d33c29);//`WRITE_APB(16'h0004, 32'h01d33c29);
    (*((volatile unsigned int*) 0x98040004)=0x01d4e412);//`WRITE_APB(16'h0004, 32'h01d4e412);
    (*((volatile unsigned int*) 0x98040004)=0x01d51b3d);//`WRITE_APB(16'h0004, 32'h01d51b3d);
    (*((volatile unsigned int*) 0x98040004)=0x01d6e41e);//`WRITE_APB(16'h0004, 32'h01d6e41e);
    (*((volatile unsigned int*) 0x98040004)=0x01d70754);//`WRITE_APB(16'h0004, 32'h01d70754);
    (*((volatile unsigned int*) 0x98040004)=0x01d8a202);//`WRITE_APB(16'h0004, 32'h01d8a202);
    (*((volatile unsigned int*) 0x98040004)=0x01d9e0c2);//`WRITE_APB(16'h0004, 32'h01d9e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01da004a);//`WRITE_APB(16'h0004, 32'h01da004a);
    (*((volatile unsigned int*) 0x98040004)=0x01dbe0c0);//`WRITE_APB(16'h0004, 32'h01dbe0c0);
    (*((volatile unsigned int*) 0x98040004)=0x01dc0111);//`WRITE_APB(16'h0004, 32'h01dc0111);
    (*((volatile unsigned int*) 0x98040004)=0x01ddf7e8);//`WRITE_APB(16'h0004, 32'h01ddf7e8);
    (*((volatile unsigned int*) 0x98040004)=0x01def90e);//`WRITE_APB(16'h0004, 32'h01def90e);
    (*((volatile unsigned int*) 0x98040004)=0x01dfca48);//`WRITE_APB(16'h0004, 32'h01dfca48);
    (*((volatile unsigned int*) 0x98040004)=0x01e0a802);//`WRITE_APB(16'h0004, 32'h01e0a802);
    (*((volatile unsigned int*) 0x98040004)=0x01e1f7e8);//`WRITE_APB(16'h0004, 32'h01e1f7e8);
    (*((volatile unsigned int*) 0x98040004)=0x01e2e004);//`WRITE_APB(16'h0004, 32'h01e2e004);
    (*((volatile unsigned int*) 0x98040004)=0x01e30078);//`WRITE_APB(16'h0004, 32'h01e30078);
    (*((volatile unsigned int*) 0x98040004)=0x01e4e0c2);//`WRITE_APB(16'h0004, 32'h01e4e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01e50009);//`WRITE_APB(16'h0004, 32'h01e50009);
    (*((volatile unsigned int*) 0x98040004)=0x01e6a200);//`WRITE_APB(16'h0004, 32'h01e6a200);
    (*((volatile unsigned int*) 0x98040004)=0x01e7e0c2);//`WRITE_APB(16'h0004, 32'h01e7e0c2);
    (*((volatile unsigned int*) 0x98040004)=0x01e80009);//`WRITE_APB(16'h0004, 32'h01e80009);
    (*((volatile unsigned int*) 0x98040004)=0x01e9e0c0);//`WRITE_APB(16'h0004, 32'h01e9e0c0);
    (*((volatile unsigned int*) 0x98040004)=0x01ea000d);//`WRITE_APB(16'h0004, 32'h01ea000d);
    (*((volatile unsigned int*) 0x98040004)=0x01ebf7e8);//`WRITE_APB(16'h0004, 32'h01ebf7e8);
    (*((volatile unsigned int*) 0x98040004)=0x01eca200);//`WRITE_APB(16'h0004, 32'h01eca200);
    (*((volatile unsigned int*) 0x98040004)=0x01ede42e);//`WRITE_APB(16'h0004, 32'h01ede42e);
    (*((volatile unsigned int*) 0x98040004)=0x01eeca28);//`WRITE_APB(16'h0004, 32'h01eeca28);
    (*((volatile unsigned int*) 0x98040004)=0x01eff7f8);//`WRITE_APB(16'h0004, 32'h01eff7f8);
    (*((volatile unsigned int*) 0x98040004)=0x01f0e42e);//`WRITE_APB(16'h0004, 32'h01f0e42e);
    (*((volatile unsigned int*) 0x98040004)=0x01f1c001);//`WRITE_APB(16'h0004, 32'h01f1c001);
    (*((volatile unsigned int*) 0x98040004)=0x01f23443);//`WRITE_APB(16'h0004, 32'h01f23443);
    (*((volatile unsigned int*) 0x98040004)=0x01f33c42);//`WRITE_APB(16'h0004, 32'h01f33c42);
    (*((volatile unsigned int*) 0x98040004)=0x01f4c000);//`WRITE_APB(16'h0004, 32'h01f4c000);
    (*((volatile unsigned int*) 0x98040004)=0x01f5e42e);//`WRITE_APB(16'h0004, 32'h01f5e42e);
    (*((volatile unsigned int*) 0x98040004)=0x01f6c001);//`WRITE_APB(16'h0004, 32'h01f6c001);
    (*((volatile unsigned int*) 0x98040004)=0x01f72c43);//`WRITE_APB(16'h0004, 32'h01f72c43);
    (*((volatile unsigned int*) 0x98040004)=0x01f82e42);//`WRITE_APB(16'h0004, 32'h01f82e42);
    (*((volatile unsigned int*) 0x98040004)=0x01f9c000);//`WRITE_APB(16'h0004, 32'h01f9c000);
    (*((volatile unsigned int*) 0x98040004)=0x01fae42a);//`WRITE_APB(16'h0004, 32'h01fae42a);
    (*((volatile unsigned int*) 0x98040004)=0x01fb1c1e);//`WRITE_APB(16'h0004, 32'h01fb1c1e);
    (*((volatile unsigned int*) 0x98040004)=0x01fcf0b4);//`WRITE_APB(16'h0004, 32'h01fcf0b4);
    (*((volatile unsigned int*) 0x98040004)=0x01fde04a);//`WRITE_APB(16'h0004, 32'h01fde04a);
    (*((volatile unsigned int*) 0x98040004)=0x01feaf10);//`WRITE_APB(16'h0004, 32'h01feaf10);
    (*((volatile unsigned int*) 0x98040004)=0x01ff1857);//`WRITE_APB(16'h0004, 32'h01ff1857);
    //in vtSetBpuCodeWorkBuf
    (*((volatile unsigned int*) 0x98040108)=(0x00342000+BASE_ADDRESS));//`WRITE_APB(16'h0108, 32'h00342000);
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    (*((volatile unsigned int*) 0x98040100)=(0x00700000+BASE_ADDRESS));//`WRITE_APB(16'h0100, 32'h00700000);
    (*((volatile unsigned int*) 0x98040170)=0x00000000);//`WRITE_APB(16'h0170, 32'h00000000);

/* ----- FuChun test start -----*/
    (*((volatile unsigned int*) 0x98040160)=0x00000001);//`WRITE_APB(16'h0170, 32'h00000000);
/* ----- FuChun test end   -----*/

    (*((volatile unsigned int*) 0x98040000)=0x00000001);//`WRITE_APB(16'h0000, 32'h00000001);

/* ----- FuChun test start -----*/
    rlt = 1;
    while(rlt){
        rlt = (*((volatile unsigned int*) 0x98040160));
    }
/* ----- FuChun test end   -----*/

    //in vtInitSeqParam
    //in vtSetStreamFrameConfig
    (*((volatile unsigned int*) 0x9804010c)=0x00000040);//`WRITE_APB(16'h010c, 32'h00000040);
    (*((volatile unsigned int*) 0x98040114)=0x00000000);//`WRITE_APB(16'h0114, 32'h00000000);
    (*((volatile unsigned int*) 0x98040110)=0x00008000);//`WRITE_APB(16'h0110, 32'h00008000);
    (*((volatile unsigned int*) 0x98040140)=0x00000d0d);//`WRITE_APB(16'h0140, 32'h00000d0d);
    (*((volatile unsigned int*) 0x98040118)=(0x00400000+BASE_ADDRESS));//`WRITE_APB(16'h0118, 32'h00400000);
    (*((volatile unsigned int*) 0x98040120)=(0x01000000+BASE_ADDRESS));//`WRITE_APB(16'h0120, 32'h01000000);
    (*((volatile unsigned int*) 0x98040124)=(0x01000000+BASE_ADDRESS));//`WRITE_APB(16'h0124, 32'h01000000);
    (*((volatile unsigned int*) 0x98040114)=0x00000000);//`WRITE_APB(16'h0114, 32'h00000000);
    //in vtInputFileOpenLoad
    //in vtInitLoadBitStrm
    (*((volatile unsigned int*) 0x98040120)=(0x01000000+BASE_ADDRESS));//`WRITE_APB(16'h0120, 32'h01000000);
    (*((volatile unsigned int*) 0x98040124)=(0x01000000+BASE_ADDRESS));//`WRITE_APB(16'h0124, 32'h01000000);
    //in vtFillBitstrmBuf
    rlt = (*((volatile unsigned int*) 0x98040120));//`READ_APB(rlt,16'h0120)=01000000;
    rlt = (*((volatile unsigned int*) 0x98040124));//`READ_APB(rlt,16'h0124)=01000000;
    (*((volatile unsigned int*) 0x98040124)=(0x01080000+BASE_ADDRESS));//`WRITE_APB(16'h0124, 32'h01080000);
//    $display("[RTK_C] DELAY_C(128); cclk");
    //DELAY_C(128);
    //in vtWaitCoda980Idle
    //$display("[RTK_CHK] POLLING check @ 10610");
    asm ("POLL_10610_0:");
    rlt = 1;
    while(rlt){
       rlt = (*((volatile unsigned int*) 0x98040160));//`READ_APB(rlt,16'h0160);
    }
//    $display("[RTK_C] DELAY_C(128); cclk");
    //DELAY_C(128);
    //in vtWaitCoda980Idle
    //$display("[RTK_CHK] POLLING check @ 10610");
    asm ("POLL_10610_1:");
    rlt = 1;
    while(rlt){
       rlt = (*((volatile unsigned int*) 0x98040160));//`READ_APB(rlt,16'h0160);
    }
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //in vtDecodeSeqInit
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    (*((volatile unsigned int*) 0x98040180)=(0x01000000+BASE_ADDRESS));//`WRITE_APB(16'h0180, 32'h01000000);
    (*((volatile unsigned int*) 0x98040184)=0x00002000);//`WRITE_APB(16'h0184, 32'h00002000);
    (*((volatile unsigned int*) 0x98040188)=0x00000002);//`WRITE_APB(16'h0188, 32'h00000002);
    (*((volatile unsigned int*) 0x9804019c)=0x00000001);//`WRITE_APB(16'h019c, 32'h00000001);
    (*((volatile unsigned int*) 0x980401a0)=0x00000200);//`WRITE_APB(16'h01a0, 32'h00000200);
    //in vtCommand2Coda980
    (*((volatile unsigned int*) 0x98040160)=0x00000001);//`WRITE_APB(16'h0160, 32'h00000001);
    (*((volatile unsigned int*) 0x98040168)=0x00000000);//`WRITE_APB(16'h0168, 32'h00000000);
    (*((volatile unsigned int*) 0x9804016c)=0x00000000);//`WRITE_APB(16'h016c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040178)=0x00000000);//`WRITE_APB(16'h0178, 32'h00000000);
    (*((volatile unsigned int*) 0x98040164)=0x00000001);//`WRITE_APB(16'h0164, 32'h00000001);
    //in vtWaitCoda980Idle
    //$display("[RTK_CHK] POLLING check @ 10610");
    asm ("POLL_10610_2:");
    rlt = 1;
    while(rlt){
       rlt = (*((volatile unsigned int*) 0x98040160));//`READ_APB(rlt,16'h0160);
    }
    rlt = (*((volatile unsigned int*) 0x980401c0));//`READ_APB(rlt,16'h01c0)=00000001;
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //in vtSetFrameBuffers
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    //in vtGetFrameSize
    rlt = (*((volatile unsigned int*) 0x980401c4));//`READ_APB(rlt,16'h01c4)=07800440;
    rlt = (*((volatile unsigned int*) 0x980401cc));//`READ_APB(rlt,16'h01cc)=00000006;
    //in vtInitDpb
    (*((volatile unsigned int*) (0x00342004+BASE_ADDRESS))=(0x02000000+BASE_ADDRESS));//0x00342000=0x02000000
    (*((volatile unsigned int*) (0x00342000+BASE_ADDRESS))=(0x021fe000+BASE_ADDRESS));//0x00342004=0x021fe000
    (*((volatile unsigned int*) (0x0034200c+BASE_ADDRESS))=(0x0227d800+BASE_ADDRESS));//0x00342008=0x0227d800
    (*((volatile unsigned int*) (0x00342184+BASE_ADDRESS))=(0x031ee000+BASE_ADDRESS));//0x00342180=0x031ee000
    (*((volatile unsigned int*) (0x00342008+BASE_ADDRESS))=(0x022fd000+BASE_ADDRESS));//0x0034200c=0x022fd000
    (*((volatile unsigned int*) (0x00342014+BASE_ADDRESS))=(0x024fb000+BASE_ADDRESS));//0x00342010=0x024fb000
    (*((volatile unsigned int*) (0x00342010+BASE_ADDRESS))=(0x0257a800+BASE_ADDRESS));//0x00342014=0x0257a800
    (*((volatile unsigned int*) (0x00342180+BASE_ADDRESS))=(0x03287000+BASE_ADDRESS));//0x00342184=0x03287000
    (*((volatile unsigned int*) (0x0034201c+BASE_ADDRESS))=(0x025fa000+BASE_ADDRESS));//0x00342018=0x025fa000
    (*((volatile unsigned int*) (0x00342018+BASE_ADDRESS))=(0x027f8000+BASE_ADDRESS));//0x0034201c=0x027f8000
    (*((volatile unsigned int*) (0x00342024+BASE_ADDRESS))=(0x02877800+BASE_ADDRESS));//0x00342020=0x02877800
    (*((volatile unsigned int*) (0x0034218c+BASE_ADDRESS))=(0x03320000+BASE_ADDRESS));//0x00342188=0x03320000
    (*((volatile unsigned int*) (0x00342020+BASE_ADDRESS))=(0x028f7000+BASE_ADDRESS));//0x00342024=0x028f7000
    (*((volatile unsigned int*) (0x0034202c+BASE_ADDRESS))=(0x02af5000+BASE_ADDRESS));//0x00342028=0x02af5000
    (*((volatile unsigned int*) (0x00342028+BASE_ADDRESS))=(0x02b74800+BASE_ADDRESS));//0x0034202c=0x02b74800
    (*((volatile unsigned int*) (0x00342188+BASE_ADDRESS))=(0x033b9000+BASE_ADDRESS));//0x0034218c=0x033b9000
    (*((volatile unsigned int*) (0x00342034+BASE_ADDRESS))=(0x02bf4000+BASE_ADDRESS));//0x00342030=0x02bf4000
    (*((volatile unsigned int*) (0x00342030+BASE_ADDRESS))=(0x02df2000+BASE_ADDRESS));//0x00342034=0x02df2000
    (*((volatile unsigned int*) (0x0034203c+BASE_ADDRESS))=(0x02e71800+BASE_ADDRESS));//0x00342038=0x02e71800
    (*((volatile unsigned int*) (0x00342194+BASE_ADDRESS))=(0x03452000+BASE_ADDRESS));//0x00342190=0x03452000
    (*((volatile unsigned int*) (0x00342038+BASE_ADDRESS))=(0x02ef1000+BASE_ADDRESS));//0x0034203c=0x02ef1000
    (*((volatile unsigned int*) (0x00342044+BASE_ADDRESS))=(0x030ef000+BASE_ADDRESS));//0x00342040=0x030ef000
    (*((volatile unsigned int*) (0x00342040+BASE_ADDRESS))=(0x0316e800+BASE_ADDRESS));//0x00342044=0x0316e800
    (*((volatile unsigned int*) (0x00342190+BASE_ADDRESS))=(0x034eb000+BASE_ADDRESS));//0x00342194=0x034eb000
    (*((volatile unsigned int*) (0x00342204+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342200=0x00000000
    (*((volatile unsigned int*) (0x00342200+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342204=0x00000000
    (*((volatile unsigned int*) (0x0034220c+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342208=0x00000000
    (*((volatile unsigned int*) (0x00342208+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x0034220c=0x00000000
    (*((volatile unsigned int*) (0x00342214+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342210=0x00000000
    (*((volatile unsigned int*) (0x00342210+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342214=0x00000000
    (*((volatile unsigned int*) (0x0034221c+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342218=0x00000000
    (*((volatile unsigned int*) (0x00342218+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x0034221c=0x00000000
    (*((volatile unsigned int*) (0x00342224+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342220=0x00000000
    (*((volatile unsigned int*) (0x00342220+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342224=0x00000000
    (*((volatile unsigned int*) (0x0034222c+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342228=0x00000000
    (*((volatile unsigned int*) (0x00342228+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x0034222c=0x00000000
    (*((volatile unsigned int*) (0x00342234+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342230=0x00000000
    (*((volatile unsigned int*) (0x00342230+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342234=0x00000000
    (*((volatile unsigned int*) (0x0034223c+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342238=0x00000000
    (*((volatile unsigned int*) (0x00342238+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x0034223c=0x00000000
    (*((volatile unsigned int*) (0x00342244+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342240=0x00000000
    (*((volatile unsigned int*) (0x00342240+BASE_ADDRESS))=(0x00000000+BASE_ADDRESS));//0x00342244=0x00000000
    (*((volatile unsigned int*) 0x98040180)=0x00000006);//`WRITE_APB(16'h0180, 32'h00000006);
    (*((volatile unsigned int*) 0x98040184)=0x00000780);//`WRITE_APB(16'h0184, 32'h00000780);
    (*((volatile unsigned int*) 0x98040190)=0xf8010000);//`WRITE_APB(16'h0190, 32'hf8010000);
    (*((volatile unsigned int*) 0x98040194)=0xf8019000);//`WRITE_APB(16'h0194, 32'hf8019000);
    (*((volatile unsigned int*) 0x98040198)=0xf8000000);//`WRITE_APB(16'h0198, 32'hf8000000);
    (*((volatile unsigned int*) 0x9804019c)=0xf8008000);//`WRITE_APB(16'h019c, 32'hf8008000);
    (*((volatile unsigned int*) 0x980401a0)=0xf801f000);//`WRITE_APB(16'h01a0, 32'hf801f000);
    (*((volatile unsigned int*) 0x980401a4)=0xf8024000);//`WRITE_APB(16'h01a4, 32'hf8024000);
    (*((volatile unsigned int*) 0x980401ac)=0x000007e0);//`WRITE_APB(16'h01ac, 32'h000007e0);
    (*((volatile unsigned int*) 0x98040188)=0x00480000);//`WRITE_APB(16'h0188, 32'h00480000);
    (*((volatile unsigned int*) 0x9804018c)=0x00000400);//`WRITE_APB(16'h018c, 32'h00000400);
    (*((volatile unsigned int*) 0x980401b8)=0x00000000);//`WRITE_APB(16'h01b8, 32'h00000000);
    (*((volatile unsigned int*) 0x980401bc)=0xffffffff);//`WRITE_APB(16'h01bc, 32'hffffffff);
    //in vtCommand2Coda980
    (*((volatile unsigned int*) 0x98040160)=0x00000001);//`WRITE_APB(16'h0160, 32'h00000001);
    (*((volatile unsigned int*) 0x98040168)=0x00000000);//`WRITE_APB(16'h0168, 32'h00000000);
    (*((volatile unsigned int*) 0x9804016c)=0x00000000);//`WRITE_APB(16'h016c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040178)=0x00000000);//`WRITE_APB(16'h0178, 32'h00000000);
    (*((volatile unsigned int*) 0x98040164)=0x00000004);//`WRITE_APB(16'h0164, 32'h00000004);
    //in vtWaitCoda980Idle
    //$display("[RTK_CHK] POLLING check @ 10610");
    asm ("POLL_10610_3:");
    rlt = 1;
    while(rlt){
       rlt = (*((volatile unsigned int*) 0x98040160));//`READ_APB(rlt,16'h0160);
    }
//    $display("[RTK_C] DELAY_C(16); cclk");
    //DELAY_C(16);
    //in vtSetFrameMapReg
    (*((volatile unsigned int*) 0x98041800)=0x00000040);//`WRITE_APB(16'h1800, 32'h00000040);
    (*((volatile unsigned int*) 0x98041804)=0x00000040);//`WRITE_APB(16'h1804, 32'h00000040);
    (*((volatile unsigned int*) 0x98041808)=0x00000040);//`WRITE_APB(16'h1808, 32'h00000040);
    (*((volatile unsigned int*) 0x9804180c)=0x00000040);//`WRITE_APB(16'h180c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041810)=0x00000040);//`WRITE_APB(16'h1810, 32'h00000040);
    (*((volatile unsigned int*) 0x98041814)=0x00000040);//`WRITE_APB(16'h1814, 32'h00000040);
    (*((volatile unsigned int*) 0x98041818)=0x00000040);//`WRITE_APB(16'h1818, 32'h00000040);
    (*((volatile unsigned int*) 0x9804181c)=0x00000040);//`WRITE_APB(16'h181c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041820)=0x00000040);//`WRITE_APB(16'h1820, 32'h00000040);
    (*((volatile unsigned int*) 0x98041824)=0x00000040);//`WRITE_APB(16'h1824, 32'h00000040);
    (*((volatile unsigned int*) 0x98041828)=0x00000040);//`WRITE_APB(16'h1828, 32'h00000040);
    (*((volatile unsigned int*) 0x9804182c)=0x00000040);//`WRITE_APB(16'h182c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041830)=0x00000040);//`WRITE_APB(16'h1830, 32'h00000040);
    (*((volatile unsigned int*) 0x98041834)=0x00000040);//`WRITE_APB(16'h1834, 32'h00000040);
    (*((volatile unsigned int*) 0x98041838)=0x00000040);//`WRITE_APB(16'h1838, 32'h00000040);
    (*((volatile unsigned int*) 0x9804183c)=0x00000040);//`WRITE_APB(16'h183c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041840)=0x00000040);//`WRITE_APB(16'h1840, 32'h00000040);
    (*((volatile unsigned int*) 0x98041844)=0x00000040);//`WRITE_APB(16'h1844, 32'h00000040);
    (*((volatile unsigned int*) 0x98041848)=0x00000040);//`WRITE_APB(16'h1848, 32'h00000040);
    (*((volatile unsigned int*) 0x9804184c)=0x00000040);//`WRITE_APB(16'h184c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041850)=0x00000040);//`WRITE_APB(16'h1850, 32'h00000040);
    (*((volatile unsigned int*) 0x98041854)=0x00000040);//`WRITE_APB(16'h1854, 32'h00000040);
    (*((volatile unsigned int*) 0x98041858)=0x00000040);//`WRITE_APB(16'h1858, 32'h00000040);
    (*((volatile unsigned int*) 0x9804185c)=0x00000040);//`WRITE_APB(16'h185c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041860)=0x00000040);//`WRITE_APB(16'h1860, 32'h00000040);
    (*((volatile unsigned int*) 0x98041864)=0x00000040);//`WRITE_APB(16'h1864, 32'h00000040);
    (*((volatile unsigned int*) 0x98041868)=0x00000040);//`WRITE_APB(16'h1868, 32'h00000040);
    (*((volatile unsigned int*) 0x9804186c)=0x00000040);//`WRITE_APB(16'h186c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041870)=0x00000040);//`WRITE_APB(16'h1870, 32'h00000040);
    (*((volatile unsigned int*) 0x98041874)=0x00000040);//`WRITE_APB(16'h1874, 32'h00000040);
    (*((volatile unsigned int*) 0x98041878)=0x00000040);//`WRITE_APB(16'h1878, 32'h00000040);
    (*((volatile unsigned int*) 0x9804187c)=0x00000040);//`WRITE_APB(16'h187c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041880)=0x00000040);//`WRITE_APB(16'h1880, 32'h00000040);
    (*((volatile unsigned int*) 0x98041884)=0x00000040);//`WRITE_APB(16'h1884, 32'h00000040);
    (*((volatile unsigned int*) 0x98041888)=0x00000040);//`WRITE_APB(16'h1888, 32'h00000040);
    (*((volatile unsigned int*) 0x9804188c)=0x00000040);//`WRITE_APB(16'h188c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041890)=0x00000040);//`WRITE_APB(16'h1890, 32'h00000040);
    (*((volatile unsigned int*) 0x98041894)=0x00000040);//`WRITE_APB(16'h1894, 32'h00000040);
    (*((volatile unsigned int*) 0x98041898)=0x00000040);//`WRITE_APB(16'h1898, 32'h00000040);
    (*((volatile unsigned int*) 0x9804189c)=0x00000040);//`WRITE_APB(16'h189c, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a0)=0x00000040);//`WRITE_APB(16'h18a0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a4)=0x00000040);//`WRITE_APB(16'h18a4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a8)=0x00000040);//`WRITE_APB(16'h18a8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ac)=0x00000040);//`WRITE_APB(16'h18ac, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b0)=0x00000040);//`WRITE_APB(16'h18b0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b4)=0x00000040);//`WRITE_APB(16'h18b4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b8)=0x00000040);//`WRITE_APB(16'h18b8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418bc)=0x00000040);//`WRITE_APB(16'h18bc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c0)=0x00000040);//`WRITE_APB(16'h18c0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c4)=0x00000040);//`WRITE_APB(16'h18c4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c8)=0x00000040);//`WRITE_APB(16'h18c8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418cc)=0x00000040);//`WRITE_APB(16'h18cc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d0)=0x00000040);//`WRITE_APB(16'h18d0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d4)=0x00000040);//`WRITE_APB(16'h18d4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d8)=0x00000040);//`WRITE_APB(16'h18d8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418dc)=0x00000040);//`WRITE_APB(16'h18dc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e0)=0x00000040);//`WRITE_APB(16'h18e0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e4)=0x00000040);//`WRITE_APB(16'h18e4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e8)=0x00000040);//`WRITE_APB(16'h18e8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ec)=0x00000040);//`WRITE_APB(16'h18ec, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f0)=0x00000040);//`WRITE_APB(16'h18f0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f4)=0x00000040);//`WRITE_APB(16'h18f4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f8)=0x00000040);//`WRITE_APB(16'h18f8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418fc)=0x00000040);//`WRITE_APB(16'h18fc, 32'h00000040);
    (*((volatile unsigned int*) 0x98041900)=0x00000000);//`WRITE_APB(16'h1900, 32'h00000000);
    rlt = (*((volatile unsigned int*) 0x98040120));//`READ_APB(rlt,16'h0120)=0100003b;
    rlt = (*((volatile unsigned int*) 0x98040124));//`READ_APB(rlt,16'h0124)=01080000;
    rlt = (*((volatile unsigned int*) 0x98040114));//`READ_APB(rlt,16'h0114)=00000000;
    //$display("[RTK_CHK] POLLING check @ 1402");
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //in vtPicInit
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    //in vtGenAxiRcRandPattern
    (*((volatile unsigned int*) 0x98040120)=(0x0100003b+BASE_ADDRESS));//`WRITE_APB(16'h0120, 32'h0100003b);
    (*((volatile unsigned int*) 0x98040124)=(0x01080000+BASE_ADDRESS));//`WRITE_APB(16'h0124, 32'h01080000);
    (*((volatile unsigned int*) 0x98040150)=0x00000000);//`WRITE_APB(16'h0150, 32'h00000000);
    //(*((volatile unsigned int*) 0x98040114)=0x00000000);//`WRITE_APB(16'h0114, 32'h00000000); //FuChun mark
    (*((volatile unsigned int*) 0x98040114)=0x00000004);//`WRITE_APB(16'h0114, 32'h00000000); //FuChun add
    //in vtSetFrameMapReg
    (*((volatile unsigned int*) 0x98041800)=0x00000040);//`WRITE_APB(16'h1800, 32'h00000040);
    (*((volatile unsigned int*) 0x98041804)=0x00000040);//`WRITE_APB(16'h1804, 32'h00000040);
    (*((volatile unsigned int*) 0x98041808)=0x00000040);//`WRITE_APB(16'h1808, 32'h00000040);
    (*((volatile unsigned int*) 0x9804180c)=0x00000040);//`WRITE_APB(16'h180c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041810)=0x00000040);//`WRITE_APB(16'h1810, 32'h00000040);
    (*((volatile unsigned int*) 0x98041814)=0x00000040);//`WRITE_APB(16'h1814, 32'h00000040);
    (*((volatile unsigned int*) 0x98041818)=0x00000040);//`WRITE_APB(16'h1818, 32'h00000040);
    (*((volatile unsigned int*) 0x9804181c)=0x00000040);//`WRITE_APB(16'h181c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041820)=0x00000040);//`WRITE_APB(16'h1820, 32'h00000040);
    (*((volatile unsigned int*) 0x98041824)=0x00000040);//`WRITE_APB(16'h1824, 32'h00000040);
    (*((volatile unsigned int*) 0x98041828)=0x00000040);//`WRITE_APB(16'h1828, 32'h00000040);
    (*((volatile unsigned int*) 0x9804182c)=0x00000040);//`WRITE_APB(16'h182c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041830)=0x00000040);//`WRITE_APB(16'h1830, 32'h00000040);
    (*((volatile unsigned int*) 0x98041834)=0x00000040);//`WRITE_APB(16'h1834, 32'h00000040);
    (*((volatile unsigned int*) 0x98041838)=0x00000040);//`WRITE_APB(16'h1838, 32'h00000040);
    (*((volatile unsigned int*) 0x9804183c)=0x00000040);//`WRITE_APB(16'h183c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041840)=0x00000040);//`WRITE_APB(16'h1840, 32'h00000040);
    (*((volatile unsigned int*) 0x98041844)=0x00000040);//`WRITE_APB(16'h1844, 32'h00000040);
    (*((volatile unsigned int*) 0x98041848)=0x00000040);//`WRITE_APB(16'h1848, 32'h00000040);
    (*((volatile unsigned int*) 0x9804184c)=0x00000040);//`WRITE_APB(16'h184c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041850)=0x00000040);//`WRITE_APB(16'h1850, 32'h00000040);
    (*((volatile unsigned int*) 0x98041854)=0x00000040);//`WRITE_APB(16'h1854, 32'h00000040);
    (*((volatile unsigned int*) 0x98041858)=0x00000040);//`WRITE_APB(16'h1858, 32'h00000040);
    (*((volatile unsigned int*) 0x9804185c)=0x00000040);//`WRITE_APB(16'h185c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041860)=0x00000040);//`WRITE_APB(16'h1860, 32'h00000040);
    (*((volatile unsigned int*) 0x98041864)=0x00000040);//`WRITE_APB(16'h1864, 32'h00000040);
    (*((volatile unsigned int*) 0x98041868)=0x00000040);//`WRITE_APB(16'h1868, 32'h00000040);
    (*((volatile unsigned int*) 0x9804186c)=0x00000040);//`WRITE_APB(16'h186c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041870)=0x00000040);//`WRITE_APB(16'h1870, 32'h00000040);
    (*((volatile unsigned int*) 0x98041874)=0x00000040);//`WRITE_APB(16'h1874, 32'h00000040);
    (*((volatile unsigned int*) 0x98041878)=0x00000040);//`WRITE_APB(16'h1878, 32'h00000040);
    (*((volatile unsigned int*) 0x9804187c)=0x00000040);//`WRITE_APB(16'h187c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041880)=0x00000040);//`WRITE_APB(16'h1880, 32'h00000040);
    (*((volatile unsigned int*) 0x98041884)=0x00000040);//`WRITE_APB(16'h1884, 32'h00000040);
    (*((volatile unsigned int*) 0x98041888)=0x00000040);//`WRITE_APB(16'h1888, 32'h00000040);
    (*((volatile unsigned int*) 0x9804188c)=0x00000040);//`WRITE_APB(16'h188c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041890)=0x00000040);//`WRITE_APB(16'h1890, 32'h00000040);
    (*((volatile unsigned int*) 0x98041894)=0x00000040);//`WRITE_APB(16'h1894, 32'h00000040);
    (*((volatile unsigned int*) 0x98041898)=0x00000040);//`WRITE_APB(16'h1898, 32'h00000040);
    (*((volatile unsigned int*) 0x9804189c)=0x00000040);//`WRITE_APB(16'h189c, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a0)=0x00000040);//`WRITE_APB(16'h18a0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a4)=0x00000040);//`WRITE_APB(16'h18a4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a8)=0x00000040);//`WRITE_APB(16'h18a8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ac)=0x00000040);//`WRITE_APB(16'h18ac, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b0)=0x00000040);//`WRITE_APB(16'h18b0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b4)=0x00000040);//`WRITE_APB(16'h18b4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b8)=0x00000040);//`WRITE_APB(16'h18b8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418bc)=0x00000040);//`WRITE_APB(16'h18bc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c0)=0x00000040);//`WRITE_APB(16'h18c0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c4)=0x00000040);//`WRITE_APB(16'h18c4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c8)=0x00000040);//`WRITE_APB(16'h18c8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418cc)=0x00000040);//`WRITE_APB(16'h18cc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d0)=0x00000040);//`WRITE_APB(16'h18d0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d4)=0x00000040);//`WRITE_APB(16'h18d4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d8)=0x00000040);//`WRITE_APB(16'h18d8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418dc)=0x00000040);//`WRITE_APB(16'h18dc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e0)=0x00000040);//`WRITE_APB(16'h18e0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e4)=0x00000040);//`WRITE_APB(16'h18e4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e8)=0x00000040);//`WRITE_APB(16'h18e8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ec)=0x00000040);//`WRITE_APB(16'h18ec, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f0)=0x00000040);//`WRITE_APB(16'h18f0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f4)=0x00000040);//`WRITE_APB(16'h18f4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f8)=0x00000040);//`WRITE_APB(16'h18f8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418fc)=0x00000040);//`WRITE_APB(16'h18fc, 32'h00000040);
    (*((volatile unsigned int*) 0x98041900)=0x00000000);//`WRITE_APB(16'h1900, 32'h00000000);
    //in vtDecodePicture
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    (*((volatile unsigned int*) 0x98040194)=0x00000000);//`WRITE_APB(16'h0194, 32'h00000000);
    (*((volatile unsigned int*) 0x98040198)=0x00000000);//`WRITE_APB(16'h0198, 32'h00000000);
    (*((volatile unsigned int*) 0x98040180)=0x00000000);//`WRITE_APB(16'h0180, 32'h00000000);
    (*((volatile unsigned int*) 0x98040170)=0x00004008);//`WRITE_APB(16'h0170, 32'h00004008);
    //in vtCommand2Coda980
    (*((volatile unsigned int*) 0x98040168)=0x00000000);//`WRITE_APB(16'h0168, 32'h00000000);
    (*((volatile unsigned int*) 0x9804016c)=0x00000000);//`WRITE_APB(16'h016c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040178)=0x00000000);//`WRITE_APB(16'h0178, 32'h00000000);
    (*((volatile unsigned int*) 0x98040164)=0x00000003);//`WRITE_APB(16'h0164, 32'h00000003);
    //$display("[RTK_CHK] POLLING check @ 8562");
    asm ("POLL_8562_4:");
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    pic_done = 0;
    while(!pic_done){
		ve_isr();
        //while( !((volatile int)touch_isr)){}
        //touch_isr = 0;
    }
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    rlt = (*((volatile unsigned int*) 0x980401c8));//`READ_APB(rlt,16'h01c8)=00000000;
    rlt = (*((volatile unsigned int*) 0x980401c4));//`READ_APB(rlt,16'h01c4)=fffffffd;
    rlt = (*((volatile unsigned int*) 0x980401dc));//`READ_APB(rlt,16'h01dc)=00000000;
    rlt = (*((volatile unsigned int*) 0x980401cc));//`READ_APB(rlt,16'h01cc)=38240081;
    rlt = (*((volatile unsigned int*) 0x980401d0));//`READ_APB(rlt,16'h01d0)=00000000;
    //in vtPicEnd
    rlt = (*((volatile unsigned int*) 0x98040120));//`READ_APB(rlt,16'h0120)=01020aba;
    rlt = (*((volatile unsigned int*) 0x98040124));//`READ_APB(rlt,16'h0124)=01080000;
    rlt = (*((volatile unsigned int*) 0x98040150));//`READ_APB(rlt,16'h0150)=00000001;
    rlt = (*((volatile unsigned int*) 0x98040114));//`READ_APB(rlt,16'h0114)=00000000;
    rlt = (*((volatile unsigned int*) 0x9804014c));//`READ_APB(rlt,16'h014c)=00400d4e;
	printf("0x9804014c, rlt = 0x%08x\n",rlt);
//    $display("[RTK_C] DELAY_C(4); cclk");
    //DELAY_C(4);
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //$display("[RTK_CHK] end POLLING check @ 1402");
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //in vtPicInit
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    //in vtGenAxiRcRandPattern
    (*((volatile unsigned int*) 0x98040120)=(0x01020aba+BASE_ADDRESS));//`WRITE_APB(16'h0120, 32'h01020aba);
    (*((volatile unsigned int*) 0x98040124)=(0x01080000+BASE_ADDRESS));//`WRITE_APB(16'h0124, 32'h01080000);
    (*((volatile unsigned int*) 0x98040150)=0x00000001);//`WRITE_APB(16'h0150, 32'h00000001);
    //(*((volatile unsigned int*) 0x98040114)=0x00000000);//`WRITE_APB(16'h0114, 32'h00000000); //FuChun mark
    //in vtSetFrameMapReg
    (*((volatile unsigned int*) 0x98041800)=0x00000040);//`WRITE_APB(16'h1800, 32'h00000040);
    (*((volatile unsigned int*) 0x98041804)=0x00000040);//`WRITE_APB(16'h1804, 32'h00000040);
    (*((volatile unsigned int*) 0x98041808)=0x00000040);//`WRITE_APB(16'h1808, 32'h00000040);
    (*((volatile unsigned int*) 0x9804180c)=0x00000040);//`WRITE_APB(16'h180c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041810)=0x00000040);//`WRITE_APB(16'h1810, 32'h00000040);
    (*((volatile unsigned int*) 0x98041814)=0x00000040);//`WRITE_APB(16'h1814, 32'h00000040);
    (*((volatile unsigned int*) 0x98041818)=0x00000040);//`WRITE_APB(16'h1818, 32'h00000040);
    (*((volatile unsigned int*) 0x9804181c)=0x00000040);//`WRITE_APB(16'h181c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041820)=0x00000040);//`WRITE_APB(16'h1820, 32'h00000040);
    (*((volatile unsigned int*) 0x98041824)=0x00000040);//`WRITE_APB(16'h1824, 32'h00000040);
    (*((volatile unsigned int*) 0x98041828)=0x00000040);//`WRITE_APB(16'h1828, 32'h00000040);
    (*((volatile unsigned int*) 0x9804182c)=0x00000040);//`WRITE_APB(16'h182c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041830)=0x00000040);//`WRITE_APB(16'h1830, 32'h00000040);
    (*((volatile unsigned int*) 0x98041834)=0x00000040);//`WRITE_APB(16'h1834, 32'h00000040);
    (*((volatile unsigned int*) 0x98041838)=0x00000040);//`WRITE_APB(16'h1838, 32'h00000040);
    (*((volatile unsigned int*) 0x9804183c)=0x00000040);//`WRITE_APB(16'h183c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041840)=0x00000040);//`WRITE_APB(16'h1840, 32'h00000040);
    (*((volatile unsigned int*) 0x98041844)=0x00000040);//`WRITE_APB(16'h1844, 32'h00000040);
    (*((volatile unsigned int*) 0x98041848)=0x00000040);//`WRITE_APB(16'h1848, 32'h00000040);
    (*((volatile unsigned int*) 0x9804184c)=0x00000040);//`WRITE_APB(16'h184c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041850)=0x00000040);//`WRITE_APB(16'h1850, 32'h00000040);
    (*((volatile unsigned int*) 0x98041854)=0x00000040);//`WRITE_APB(16'h1854, 32'h00000040);
    (*((volatile unsigned int*) 0x98041858)=0x00000040);//`WRITE_APB(16'h1858, 32'h00000040);
    (*((volatile unsigned int*) 0x9804185c)=0x00000040);//`WRITE_APB(16'h185c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041860)=0x00000040);//`WRITE_APB(16'h1860, 32'h00000040);
    (*((volatile unsigned int*) 0x98041864)=0x00000040);//`WRITE_APB(16'h1864, 32'h00000040);
    (*((volatile unsigned int*) 0x98041868)=0x00000040);//`WRITE_APB(16'h1868, 32'h00000040);
    (*((volatile unsigned int*) 0x9804186c)=0x00000040);//`WRITE_APB(16'h186c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041870)=0x00000040);//`WRITE_APB(16'h1870, 32'h00000040);
    (*((volatile unsigned int*) 0x98041874)=0x00000040);//`WRITE_APB(16'h1874, 32'h00000040);
    (*((volatile unsigned int*) 0x98041878)=0x00000040);//`WRITE_APB(16'h1878, 32'h00000040);
    (*((volatile unsigned int*) 0x9804187c)=0x00000040);//`WRITE_APB(16'h187c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041880)=0x00000040);//`WRITE_APB(16'h1880, 32'h00000040);
    (*((volatile unsigned int*) 0x98041884)=0x00000040);//`WRITE_APB(16'h1884, 32'h00000040);
    (*((volatile unsigned int*) 0x98041888)=0x00000040);//`WRITE_APB(16'h1888, 32'h00000040);
    (*((volatile unsigned int*) 0x9804188c)=0x00000040);//`WRITE_APB(16'h188c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041890)=0x00000040);//`WRITE_APB(16'h1890, 32'h00000040);
    (*((volatile unsigned int*) 0x98041894)=0x00000040);//`WRITE_APB(16'h1894, 32'h00000040);
    (*((volatile unsigned int*) 0x98041898)=0x00000040);//`WRITE_APB(16'h1898, 32'h00000040);
    (*((volatile unsigned int*) 0x9804189c)=0x00000040);//`WRITE_APB(16'h189c, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a0)=0x00000040);//`WRITE_APB(16'h18a0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a4)=0x00000040);//`WRITE_APB(16'h18a4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a8)=0x00000040);//`WRITE_APB(16'h18a8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ac)=0x00000040);//`WRITE_APB(16'h18ac, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b0)=0x00000040);//`WRITE_APB(16'h18b0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b4)=0x00000040);//`WRITE_APB(16'h18b4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b8)=0x00000040);//`WRITE_APB(16'h18b8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418bc)=0x00000040);//`WRITE_APB(16'h18bc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c0)=0x00000040);//`WRITE_APB(16'h18c0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c4)=0x00000040);//`WRITE_APB(16'h18c4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c8)=0x00000040);//`WRITE_APB(16'h18c8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418cc)=0x00000040);//`WRITE_APB(16'h18cc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d0)=0x00000040);//`WRITE_APB(16'h18d0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d4)=0x00000040);//`WRITE_APB(16'h18d4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d8)=0x00000040);//`WRITE_APB(16'h18d8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418dc)=0x00000040);//`WRITE_APB(16'h18dc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e0)=0x00000040);//`WRITE_APB(16'h18e0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e4)=0x00000040);//`WRITE_APB(16'h18e4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e8)=0x00000040);//`WRITE_APB(16'h18e8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ec)=0x00000040);//`WRITE_APB(16'h18ec, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f0)=0x00000040);//`WRITE_APB(16'h18f0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f4)=0x00000040);//`WRITE_APB(16'h18f4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f8)=0x00000040);//`WRITE_APB(16'h18f8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418fc)=0x00000040);//`WRITE_APB(16'h18fc, 32'h00000040);
    (*((volatile unsigned int*) 0x98041900)=0x00000000);//`WRITE_APB(16'h1900, 32'h00000000);
    //in vtDecodePicture
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    (*((volatile unsigned int*) 0x98040194)=0x00000000);//`WRITE_APB(16'h0194, 32'h00000000);
    (*((volatile unsigned int*) 0x98040198)=0x00000000);//`WRITE_APB(16'h0198, 32'h00000000);
    (*((volatile unsigned int*) 0x98040180)=0x00000000);//`WRITE_APB(16'h0180, 32'h00000000);
    (*((volatile unsigned int*) 0x98040170)=0x00004008);//`WRITE_APB(16'h0170, 32'h00004008);
    //in vtCommand2Coda980
    (*((volatile unsigned int*) 0x98040168)=0x00000000);//`WRITE_APB(16'h0168, 32'h00000000);
    (*((volatile unsigned int*) 0x9804016c)=0x00000000);//`WRITE_APB(16'h016c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040178)=0x00000000);//`WRITE_APB(16'h0178, 32'h00000000);
    (*((volatile unsigned int*) 0x98040164)=0x00000003);//`WRITE_APB(16'h0164, 32'h00000003);
    //$display("[RTK_CHK] POLLING check @ 8562");
    asm ("POLL_8562_5:");
    pic_done = 0;
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    while(!pic_done){
		ve_isr();
        //while( !((volatile int)touch_isr)){}
        //touch_isr = 0;
    }
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    rlt = (*((volatile unsigned int*) 0x980401c8));//`READ_APB(rlt,16'h01c8)=00000000;
    rlt = (*((volatile unsigned int*) 0x980401c4));//`READ_APB(rlt,16'h01c4)=fffffffd;
    rlt = (*((volatile unsigned int*) 0x980401dc));//`READ_APB(rlt,16'h01dc)=00000001;
    rlt = (*((volatile unsigned int*) 0x980401cc));//`READ_APB(rlt,16'h01cc)=38240089;
    rlt = (*((volatile unsigned int*) 0x980401d0));//`READ_APB(rlt,16'h01d0)=00000000;
    //in vtPicEnd
    rlt = (*((volatile unsigned int*) 0x98040120));//`READ_APB(rlt,16'h0120)=010333f8;
    rlt = (*((volatile unsigned int*) 0x98040124));//`READ_APB(rlt,16'h0124)=01080000;
    rlt = (*((volatile unsigned int*) 0x98040150));//`READ_APB(rlt,16'h0150)=00000003;
    rlt = (*((volatile unsigned int*) 0x98040114));//`READ_APB(rlt,16'h0114)=00000000;
    rlt = (*((volatile unsigned int*) 0x9804014c));//`READ_APB(rlt,16'h014c)=00447701;
	printf("0x9804014c, rlt = 0x%08x\n",rlt);
//    $display("[RTK_C] DELAY_C(4); cclk");
    //DELAY_C(4);
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //$display("[RTK_CHK] end POLLING check @ 1402");
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //in vtPicInit
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    //in vtGenAxiRcRandPattern
    (*((volatile unsigned int*) 0x98040120)=(0x010333f8+BASE_ADDRESS));//`WRITE_APB(16'h0120, 32'h010333f8);
    (*((volatile unsigned int*) 0x98040124)=(0x01080000+BASE_ADDRESS));//`WRITE_APB(16'h0124, 32'h01080000);
    (*((volatile unsigned int*) 0x98040150)=0x00000003);//`WRITE_APB(16'h0150, 32'h00000003);
    //(*((volatile unsigned int*) 0x98040114)=0x00000000);//`WRITE_APB(16'h0114, 32'h00000000); //FuChun mark
    //in vtSetFrameMapReg
    (*((volatile unsigned int*) 0x98041800)=0x00000040);//`WRITE_APB(16'h1800, 32'h00000040);
    (*((volatile unsigned int*) 0x98041804)=0x00000040);//`WRITE_APB(16'h1804, 32'h00000040);
    (*((volatile unsigned int*) 0x98041808)=0x00000040);//`WRITE_APB(16'h1808, 32'h00000040);
    (*((volatile unsigned int*) 0x9804180c)=0x00000040);//`WRITE_APB(16'h180c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041810)=0x00000040);//`WRITE_APB(16'h1810, 32'h00000040);
    (*((volatile unsigned int*) 0x98041814)=0x00000040);//`WRITE_APB(16'h1814, 32'h00000040);
    (*((volatile unsigned int*) 0x98041818)=0x00000040);//`WRITE_APB(16'h1818, 32'h00000040);
    (*((volatile unsigned int*) 0x9804181c)=0x00000040);//`WRITE_APB(16'h181c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041820)=0x00000040);//`WRITE_APB(16'h1820, 32'h00000040);
    (*((volatile unsigned int*) 0x98041824)=0x00000040);//`WRITE_APB(16'h1824, 32'h00000040);
    (*((volatile unsigned int*) 0x98041828)=0x00000040);//`WRITE_APB(16'h1828, 32'h00000040);
    (*((volatile unsigned int*) 0x9804182c)=0x00000040);//`WRITE_APB(16'h182c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041830)=0x00000040);//`WRITE_APB(16'h1830, 32'h00000040);
    (*((volatile unsigned int*) 0x98041834)=0x00000040);//`WRITE_APB(16'h1834, 32'h00000040);
    (*((volatile unsigned int*) 0x98041838)=0x00000040);//`WRITE_APB(16'h1838, 32'h00000040);
    (*((volatile unsigned int*) 0x9804183c)=0x00000040);//`WRITE_APB(16'h183c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041840)=0x00000040);//`WRITE_APB(16'h1840, 32'h00000040);
    (*((volatile unsigned int*) 0x98041844)=0x00000040);//`WRITE_APB(16'h1844, 32'h00000040);
    (*((volatile unsigned int*) 0x98041848)=0x00000040);//`WRITE_APB(16'h1848, 32'h00000040);
    (*((volatile unsigned int*) 0x9804184c)=0x00000040);//`WRITE_APB(16'h184c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041850)=0x00000040);//`WRITE_APB(16'h1850, 32'h00000040);
    (*((volatile unsigned int*) 0x98041854)=0x00000040);//`WRITE_APB(16'h1854, 32'h00000040);
    (*((volatile unsigned int*) 0x98041858)=0x00000040);//`WRITE_APB(16'h1858, 32'h00000040);
    (*((volatile unsigned int*) 0x9804185c)=0x00000040);//`WRITE_APB(16'h185c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041860)=0x00000040);//`WRITE_APB(16'h1860, 32'h00000040);
    (*((volatile unsigned int*) 0x98041864)=0x00000040);//`WRITE_APB(16'h1864, 32'h00000040);
    (*((volatile unsigned int*) 0x98041868)=0x00000040);//`WRITE_APB(16'h1868, 32'h00000040);
    (*((volatile unsigned int*) 0x9804186c)=0x00000040);//`WRITE_APB(16'h186c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041870)=0x00000040);//`WRITE_APB(16'h1870, 32'h00000040);
    (*((volatile unsigned int*) 0x98041874)=0x00000040);//`WRITE_APB(16'h1874, 32'h00000040);
    (*((volatile unsigned int*) 0x98041878)=0x00000040);//`WRITE_APB(16'h1878, 32'h00000040);
    (*((volatile unsigned int*) 0x9804187c)=0x00000040);//`WRITE_APB(16'h187c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041880)=0x00000040);//`WRITE_APB(16'h1880, 32'h00000040);
    (*((volatile unsigned int*) 0x98041884)=0x00000040);//`WRITE_APB(16'h1884, 32'h00000040);
    (*((volatile unsigned int*) 0x98041888)=0x00000040);//`WRITE_APB(16'h1888, 32'h00000040);
    (*((volatile unsigned int*) 0x9804188c)=0x00000040);//`WRITE_APB(16'h188c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041890)=0x00000040);//`WRITE_APB(16'h1890, 32'h00000040);
    (*((volatile unsigned int*) 0x98041894)=0x00000040);//`WRITE_APB(16'h1894, 32'h00000040);
    (*((volatile unsigned int*) 0x98041898)=0x00000040);//`WRITE_APB(16'h1898, 32'h00000040);
    (*((volatile unsigned int*) 0x9804189c)=0x00000040);//`WRITE_APB(16'h189c, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a0)=0x00000040);//`WRITE_APB(16'h18a0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a4)=0x00000040);//`WRITE_APB(16'h18a4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a8)=0x00000040);//`WRITE_APB(16'h18a8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ac)=0x00000040);//`WRITE_APB(16'h18ac, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b0)=0x00000040);//`WRITE_APB(16'h18b0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b4)=0x00000040);//`WRITE_APB(16'h18b4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b8)=0x00000040);//`WRITE_APB(16'h18b8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418bc)=0x00000040);//`WRITE_APB(16'h18bc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c0)=0x00000040);//`WRITE_APB(16'h18c0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c4)=0x00000040);//`WRITE_APB(16'h18c4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c8)=0x00000040);//`WRITE_APB(16'h18c8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418cc)=0x00000040);//`WRITE_APB(16'h18cc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d0)=0x00000040);//`WRITE_APB(16'h18d0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d4)=0x00000040);//`WRITE_APB(16'h18d4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d8)=0x00000040);//`WRITE_APB(16'h18d8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418dc)=0x00000040);//`WRITE_APB(16'h18dc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e0)=0x00000040);//`WRITE_APB(16'h18e0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e4)=0x00000040);//`WRITE_APB(16'h18e4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e8)=0x00000040);//`WRITE_APB(16'h18e8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ec)=0x00000040);//`WRITE_APB(16'h18ec, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f0)=0x00000040);//`WRITE_APB(16'h18f0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f4)=0x00000040);//`WRITE_APB(16'h18f4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f8)=0x00000040);//`WRITE_APB(16'h18f8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418fc)=0x00000040);//`WRITE_APB(16'h18fc, 32'h00000040);
    (*((volatile unsigned int*) 0x98041900)=0x00000000);//`WRITE_APB(16'h1900, 32'h00000000);
    //in vtDecodePicture
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    (*((volatile unsigned int*) 0x98040194)=0x00000000);//`WRITE_APB(16'h0194, 32'h00000000);
    (*((volatile unsigned int*) 0x98040198)=0x00000000);//`WRITE_APB(16'h0198, 32'h00000000);
    (*((volatile unsigned int*) 0x98040180)=0x00000000);//`WRITE_APB(16'h0180, 32'h00000000);
    (*((volatile unsigned int*) 0x98040170)=0x00004008);//`WRITE_APB(16'h0170, 32'h00004008);
    //in vtCommand2Coda980
    (*((volatile unsigned int*) 0x98040168)=0x00000000);//`WRITE_APB(16'h0168, 32'h00000000);
    (*((volatile unsigned int*) 0x9804016c)=0x00000000);//`WRITE_APB(16'h016c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040178)=0x00000000);//`WRITE_APB(16'h0178, 32'h00000000);
    (*((volatile unsigned int*) 0x98040164)=0x00000003);//`WRITE_APB(16'h0164, 32'h00000003);
    //$display("[RTK_CHK] POLLING check @ 8562");
    asm ("POLL_8562_6:");
    pic_done = 0;
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    while(!pic_done){
		ve_isr();
        //while( !((volatile int)touch_isr)){}
        //touch_isr = 0;
    }
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    rlt = (*((volatile unsigned int*) 0x980401c8));//`READ_APB(rlt,16'h01c8)=00000000;
    rlt = (*((volatile unsigned int*) 0x980401c4));//`READ_APB(rlt,16'h01c4)=fffffffd;
    rlt = (*((volatile unsigned int*) 0x980401dc));//`READ_APB(rlt,16'h01dc)=00000002;
    rlt = (*((volatile unsigned int*) 0x980401cc));//`READ_APB(rlt,16'h01cc)=38240089;
    rlt = (*((volatile unsigned int*) 0x980401d0));//`READ_APB(rlt,16'h01d0)=00000000;
    //in vtPicEnd
    rlt = (*((volatile unsigned int*) 0x98040120));//`READ_APB(rlt,16'h0120)=010458d9;
    rlt = (*((volatile unsigned int*) 0x98040124));//`READ_APB(rlt,16'h0124)=01080000;
    rlt = (*((volatile unsigned int*) 0x98040150));//`READ_APB(rlt,16'h0150)=00000007;
    rlt = (*((volatile unsigned int*) 0x98040114));//`READ_APB(rlt,16'h0114)=00000000;
    rlt = (*((volatile unsigned int*) 0x9804014c));//`READ_APB(rlt,16'h014c)=00457426;
	printf("0x9804014c, rlt = 0x%08x\n",rlt);
//    $display("[RTK_C] DELAY_C(4); cclk");
    //DELAY_C(4);
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //$display("[RTK_CHK] end POLLING check @ 1402");
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //in vtPicInit
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    //in vtGenAxiRcRandPattern
    (*((volatile unsigned int*) 0x98040120)=(0x010458d9+BASE_ADDRESS));//`WRITE_APB(16'h0120, 32'h010458d9);
    (*((volatile unsigned int*) 0x98040124)=(0x01080000+BASE_ADDRESS));//`WRITE_APB(16'h0124, 32'h01080000);
    (*((volatile unsigned int*) 0x98040150)=0x00000007);//`WRITE_APB(16'h0150, 32'h00000007);
    //(*((volatile unsigned int*) 0x98040114)=0x00000000);//`WRITE_APB(16'h0114, 32'h00000000); //FuChun mark
    //in vtSetFrameMapReg
    (*((volatile unsigned int*) 0x98041800)=0x00000040);//`WRITE_APB(16'h1800, 32'h00000040);
    (*((volatile unsigned int*) 0x98041804)=0x00000040);//`WRITE_APB(16'h1804, 32'h00000040);
    (*((volatile unsigned int*) 0x98041808)=0x00000040);//`WRITE_APB(16'h1808, 32'h00000040);
    (*((volatile unsigned int*) 0x9804180c)=0x00000040);//`WRITE_APB(16'h180c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041810)=0x00000040);//`WRITE_APB(16'h1810, 32'h00000040);
    (*((volatile unsigned int*) 0x98041814)=0x00000040);//`WRITE_APB(16'h1814, 32'h00000040);
    (*((volatile unsigned int*) 0x98041818)=0x00000040);//`WRITE_APB(16'h1818, 32'h00000040);
    (*((volatile unsigned int*) 0x9804181c)=0x00000040);//`WRITE_APB(16'h181c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041820)=0x00000040);//`WRITE_APB(16'h1820, 32'h00000040);
    (*((volatile unsigned int*) 0x98041824)=0x00000040);//`WRITE_APB(16'h1824, 32'h00000040);
    (*((volatile unsigned int*) 0x98041828)=0x00000040);//`WRITE_APB(16'h1828, 32'h00000040);
    (*((volatile unsigned int*) 0x9804182c)=0x00000040);//`WRITE_APB(16'h182c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041830)=0x00000040);//`WRITE_APB(16'h1830, 32'h00000040);
    (*((volatile unsigned int*) 0x98041834)=0x00000040);//`WRITE_APB(16'h1834, 32'h00000040);
    (*((volatile unsigned int*) 0x98041838)=0x00000040);//`WRITE_APB(16'h1838, 32'h00000040);
    (*((volatile unsigned int*) 0x9804183c)=0x00000040);//`WRITE_APB(16'h183c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041840)=0x00000040);//`WRITE_APB(16'h1840, 32'h00000040);
    (*((volatile unsigned int*) 0x98041844)=0x00000040);//`WRITE_APB(16'h1844, 32'h00000040);
    (*((volatile unsigned int*) 0x98041848)=0x00000040);//`WRITE_APB(16'h1848, 32'h00000040);
    (*((volatile unsigned int*) 0x9804184c)=0x00000040);//`WRITE_APB(16'h184c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041850)=0x00000040);//`WRITE_APB(16'h1850, 32'h00000040);
    (*((volatile unsigned int*) 0x98041854)=0x00000040);//`WRITE_APB(16'h1854, 32'h00000040);
    (*((volatile unsigned int*) 0x98041858)=0x00000040);//`WRITE_APB(16'h1858, 32'h00000040);
    (*((volatile unsigned int*) 0x9804185c)=0x00000040);//`WRITE_APB(16'h185c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041860)=0x00000040);//`WRITE_APB(16'h1860, 32'h00000040);
    (*((volatile unsigned int*) 0x98041864)=0x00000040);//`WRITE_APB(16'h1864, 32'h00000040);
    (*((volatile unsigned int*) 0x98041868)=0x00000040);//`WRITE_APB(16'h1868, 32'h00000040);
    (*((volatile unsigned int*) 0x9804186c)=0x00000040);//`WRITE_APB(16'h186c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041870)=0x00000040);//`WRITE_APB(16'h1870, 32'h00000040);
    (*((volatile unsigned int*) 0x98041874)=0x00000040);//`WRITE_APB(16'h1874, 32'h00000040);
    (*((volatile unsigned int*) 0x98041878)=0x00000040);//`WRITE_APB(16'h1878, 32'h00000040);
    (*((volatile unsigned int*) 0x9804187c)=0x00000040);//`WRITE_APB(16'h187c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041880)=0x00000040);//`WRITE_APB(16'h1880, 32'h00000040);
    (*((volatile unsigned int*) 0x98041884)=0x00000040);//`WRITE_APB(16'h1884, 32'h00000040);
    (*((volatile unsigned int*) 0x98041888)=0x00000040);//`WRITE_APB(16'h1888, 32'h00000040);
    (*((volatile unsigned int*) 0x9804188c)=0x00000040);//`WRITE_APB(16'h188c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041890)=0x00000040);//`WRITE_APB(16'h1890, 32'h00000040);
    (*((volatile unsigned int*) 0x98041894)=0x00000040);//`WRITE_APB(16'h1894, 32'h00000040);
    (*((volatile unsigned int*) 0x98041898)=0x00000040);//`WRITE_APB(16'h1898, 32'h00000040);
    (*((volatile unsigned int*) 0x9804189c)=0x00000040);//`WRITE_APB(16'h189c, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a0)=0x00000040);//`WRITE_APB(16'h18a0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a4)=0x00000040);//`WRITE_APB(16'h18a4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a8)=0x00000040);//`WRITE_APB(16'h18a8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ac)=0x00000040);//`WRITE_APB(16'h18ac, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b0)=0x00000040);//`WRITE_APB(16'h18b0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b4)=0x00000040);//`WRITE_APB(16'h18b4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b8)=0x00000040);//`WRITE_APB(16'h18b8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418bc)=0x00000040);//`WRITE_APB(16'h18bc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c0)=0x00000040);//`WRITE_APB(16'h18c0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c4)=0x00000040);//`WRITE_APB(16'h18c4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c8)=0x00000040);//`WRITE_APB(16'h18c8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418cc)=0x00000040);//`WRITE_APB(16'h18cc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d0)=0x00000040);//`WRITE_APB(16'h18d0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d4)=0x00000040);//`WRITE_APB(16'h18d4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d8)=0x00000040);//`WRITE_APB(16'h18d8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418dc)=0x00000040);//`WRITE_APB(16'h18dc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e0)=0x00000040);//`WRITE_APB(16'h18e0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e4)=0x00000040);//`WRITE_APB(16'h18e4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e8)=0x00000040);//`WRITE_APB(16'h18e8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ec)=0x00000040);//`WRITE_APB(16'h18ec, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f0)=0x00000040);//`WRITE_APB(16'h18f0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f4)=0x00000040);//`WRITE_APB(16'h18f4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f8)=0x00000040);//`WRITE_APB(16'h18f8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418fc)=0x00000040);//`WRITE_APB(16'h18fc, 32'h00000040);
    (*((volatile unsigned int*) 0x98041900)=0x00000000);//`WRITE_APB(16'h1900, 32'h00000000);
    //in vtDecodePicture
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    (*((volatile unsigned int*) 0x98040194)=0x00000000);//`WRITE_APB(16'h0194, 32'h00000000);
    (*((volatile unsigned int*) 0x98040198)=0x00000000);//`WRITE_APB(16'h0198, 32'h00000000);
    (*((volatile unsigned int*) 0x98040180)=0x00000000);//`WRITE_APB(16'h0180, 32'h00000000);
    (*((volatile unsigned int*) 0x98040170)=0x00004008);//`WRITE_APB(16'h0170, 32'h00004008);
    //in vtCommand2Coda980
    (*((volatile unsigned int*) 0x98040168)=0x00000000);//`WRITE_APB(16'h0168, 32'h00000000);
    (*((volatile unsigned int*) 0x9804016c)=0x00000000);//`WRITE_APB(16'h016c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040178)=0x00000000);//`WRITE_APB(16'h0178, 32'h00000000);
    (*((volatile unsigned int*) 0x98040164)=0x00000003);//`WRITE_APB(16'h0164, 32'h00000003);
    //$display("[RTK_CHK] POLLING check @ 8562");
    asm ("POLL_8562_7:");
    pic_done = 0;
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    while(!pic_done){
		ve_isr();
        //while( !((volatile int)touch_isr)){}
        //touch_isr = 0;
    }
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    rlt = (*((volatile unsigned int*) 0x980401c8));//`READ_APB(rlt,16'h01c8)=00000000;
    rlt = (*((volatile unsigned int*) 0x980401c4));//`READ_APB(rlt,16'h01c4)=fffffffd;
    rlt = (*((volatile unsigned int*) 0x980401dc));//`READ_APB(rlt,16'h01dc)=00000003;
    rlt = (*((volatile unsigned int*) 0x980401cc));//`READ_APB(rlt,16'h01cc)=38240089;
    rlt = (*((volatile unsigned int*) 0x980401d0));//`READ_APB(rlt,16'h01d0)=00000000;
    //in vtPicEnd
    rlt = (*((volatile unsigned int*) 0x98040120));//`READ_APB(rlt,16'h0120)=0105c893;
    rlt = (*((volatile unsigned int*) 0x98040124));//`READ_APB(rlt,16'h0124)=01080000;
    rlt = (*((volatile unsigned int*) 0x98040150));//`READ_APB(rlt,16'h0150)=0000000f;
    rlt = (*((volatile unsigned int*) 0x98040114));//`READ_APB(rlt,16'h0114)=00000000;
    rlt = (*((volatile unsigned int*) 0x9804014c));//`READ_APB(rlt,16'h014c)=00471694;
	printf("0x9804014c, rlt = 0x%08x\n",rlt);
//    $display("[RTK_C] DELAY_C(4); cclk");
    //DELAY_C(4);
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //$display("[RTK_CHK] end POLLING check @ 1402");
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //in vtPicInit
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    //in vtGenAxiRcRandPattern
    (*((volatile unsigned int*) 0x98040120)=(0x0105c893+BASE_ADDRESS));//`WRITE_APB(16'h0120, 32'h0105c893);
    (*((volatile unsigned int*) 0x98040124)=(0x01080000+BASE_ADDRESS));//`WRITE_APB(16'h0124, 32'h01080000);
    (*((volatile unsigned int*) 0x98040150)=0x0000000f);//`WRITE_APB(16'h0150, 32'h0000000f);
    //(*((volatile unsigned int*) 0x98040114)=0x00000000);//`WRITE_APB(16'h0114, 32'h00000000); //FuChun mark
    //in vtSetFrameMapReg
    (*((volatile unsigned int*) 0x98041800)=0x00000040);//`WRITE_APB(16'h1800, 32'h00000040);
    (*((volatile unsigned int*) 0x98041804)=0x00000040);//`WRITE_APB(16'h1804, 32'h00000040);
    (*((volatile unsigned int*) 0x98041808)=0x00000040);//`WRITE_APB(16'h1808, 32'h00000040);
    (*((volatile unsigned int*) 0x9804180c)=0x00000040);//`WRITE_APB(16'h180c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041810)=0x00000040);//`WRITE_APB(16'h1810, 32'h00000040);
    (*((volatile unsigned int*) 0x98041814)=0x00000040);//`WRITE_APB(16'h1814, 32'h00000040);
    (*((volatile unsigned int*) 0x98041818)=0x00000040);//`WRITE_APB(16'h1818, 32'h00000040);
    (*((volatile unsigned int*) 0x9804181c)=0x00000040);//`WRITE_APB(16'h181c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041820)=0x00000040);//`WRITE_APB(16'h1820, 32'h00000040);
    (*((volatile unsigned int*) 0x98041824)=0x00000040);//`WRITE_APB(16'h1824, 32'h00000040);
    (*((volatile unsigned int*) 0x98041828)=0x00000040);//`WRITE_APB(16'h1828, 32'h00000040);
    (*((volatile unsigned int*) 0x9804182c)=0x00000040);//`WRITE_APB(16'h182c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041830)=0x00000040);//`WRITE_APB(16'h1830, 32'h00000040);
    (*((volatile unsigned int*) 0x98041834)=0x00000040);//`WRITE_APB(16'h1834, 32'h00000040);
    (*((volatile unsigned int*) 0x98041838)=0x00000040);//`WRITE_APB(16'h1838, 32'h00000040);
    (*((volatile unsigned int*) 0x9804183c)=0x00000040);//`WRITE_APB(16'h183c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041840)=0x00000040);//`WRITE_APB(16'h1840, 32'h00000040);
    (*((volatile unsigned int*) 0x98041844)=0x00000040);//`WRITE_APB(16'h1844, 32'h00000040);
    (*((volatile unsigned int*) 0x98041848)=0x00000040);//`WRITE_APB(16'h1848, 32'h00000040);
    (*((volatile unsigned int*) 0x9804184c)=0x00000040);//`WRITE_APB(16'h184c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041850)=0x00000040);//`WRITE_APB(16'h1850, 32'h00000040);
    (*((volatile unsigned int*) 0x98041854)=0x00000040);//`WRITE_APB(16'h1854, 32'h00000040);
    (*((volatile unsigned int*) 0x98041858)=0x00000040);//`WRITE_APB(16'h1858, 32'h00000040);
    (*((volatile unsigned int*) 0x9804185c)=0x00000040);//`WRITE_APB(16'h185c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041860)=0x00000040);//`WRITE_APB(16'h1860, 32'h00000040);
    (*((volatile unsigned int*) 0x98041864)=0x00000040);//`WRITE_APB(16'h1864, 32'h00000040);
    (*((volatile unsigned int*) 0x98041868)=0x00000040);//`WRITE_APB(16'h1868, 32'h00000040);
    (*((volatile unsigned int*) 0x9804186c)=0x00000040);//`WRITE_APB(16'h186c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041870)=0x00000040);//`WRITE_APB(16'h1870, 32'h00000040);
    (*((volatile unsigned int*) 0x98041874)=0x00000040);//`WRITE_APB(16'h1874, 32'h00000040);
    (*((volatile unsigned int*) 0x98041878)=0x00000040);//`WRITE_APB(16'h1878, 32'h00000040);
    (*((volatile unsigned int*) 0x9804187c)=0x00000040);//`WRITE_APB(16'h187c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041880)=0x00000040);//`WRITE_APB(16'h1880, 32'h00000040);
    (*((volatile unsigned int*) 0x98041884)=0x00000040);//`WRITE_APB(16'h1884, 32'h00000040);
    (*((volatile unsigned int*) 0x98041888)=0x00000040);//`WRITE_APB(16'h1888, 32'h00000040);
    (*((volatile unsigned int*) 0x9804188c)=0x00000040);//`WRITE_APB(16'h188c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041890)=0x00000040);//`WRITE_APB(16'h1890, 32'h00000040);
    (*((volatile unsigned int*) 0x98041894)=0x00000040);//`WRITE_APB(16'h1894, 32'h00000040);
    (*((volatile unsigned int*) 0x98041898)=0x00000040);//`WRITE_APB(16'h1898, 32'h00000040);
    (*((volatile unsigned int*) 0x9804189c)=0x00000040);//`WRITE_APB(16'h189c, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a0)=0x00000040);//`WRITE_APB(16'h18a0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a4)=0x00000040);//`WRITE_APB(16'h18a4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a8)=0x00000040);//`WRITE_APB(16'h18a8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ac)=0x00000040);//`WRITE_APB(16'h18ac, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b0)=0x00000040);//`WRITE_APB(16'h18b0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b4)=0x00000040);//`WRITE_APB(16'h18b4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b8)=0x00000040);//`WRITE_APB(16'h18b8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418bc)=0x00000040);//`WRITE_APB(16'h18bc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c0)=0x00000040);//`WRITE_APB(16'h18c0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c4)=0x00000040);//`WRITE_APB(16'h18c4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c8)=0x00000040);//`WRITE_APB(16'h18c8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418cc)=0x00000040);//`WRITE_APB(16'h18cc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d0)=0x00000040);//`WRITE_APB(16'h18d0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d4)=0x00000040);//`WRITE_APB(16'h18d4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d8)=0x00000040);//`WRITE_APB(16'h18d8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418dc)=0x00000040);//`WRITE_APB(16'h18dc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e0)=0x00000040);//`WRITE_APB(16'h18e0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e4)=0x00000040);//`WRITE_APB(16'h18e4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e8)=0x00000040);//`WRITE_APB(16'h18e8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ec)=0x00000040);//`WRITE_APB(16'h18ec, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f0)=0x00000040);//`WRITE_APB(16'h18f0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f4)=0x00000040);//`WRITE_APB(16'h18f4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f8)=0x00000040);//`WRITE_APB(16'h18f8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418fc)=0x00000040);//`WRITE_APB(16'h18fc, 32'h00000040);
    (*((volatile unsigned int*) 0x98041900)=0x00000000);//`WRITE_APB(16'h1900, 32'h00000000);
    //in vtDecodePicture
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    (*((volatile unsigned int*) 0x98040194)=0x00000000);//`WRITE_APB(16'h0194, 32'h00000000);
    (*((volatile unsigned int*) 0x98040198)=0x00000000);//`WRITE_APB(16'h0198, 32'h00000000);
    (*((volatile unsigned int*) 0x98040180)=0x00000000);//`WRITE_APB(16'h0180, 32'h00000000);
    (*((volatile unsigned int*) 0x98040170)=0x00004008);//`WRITE_APB(16'h0170, 32'h00004008);
    //in vtCommand2Coda980
    (*((volatile unsigned int*) 0x98040168)=0x00000000);//`WRITE_APB(16'h0168, 32'h00000000);
    (*((volatile unsigned int*) 0x9804016c)=0x00000000);//`WRITE_APB(16'h016c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040178)=0x00000000);//`WRITE_APB(16'h0178, 32'h00000000);
    (*((volatile unsigned int*) 0x98040164)=0x00000003);//`WRITE_APB(16'h0164, 32'h00000003);
    //$display("[RTK_CHK] POLLING check @ 8562");
    asm ("POLL_8562_8:");
    pic_done = 0;
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    while(!pic_done){
		ve_isr();
        //while( !((volatile int)touch_isr)){}
        //touch_isr = 0;
    }
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    rlt = (*((volatile unsigned int*) 0x980401c8));//`READ_APB(rlt,16'h01c8)=00000000;
    rlt = (*((volatile unsigned int*) 0x980401c4));//`READ_APB(rlt,16'h01c4)=00000000;
    rlt = (*((volatile unsigned int*) 0x980401dc));//`READ_APB(rlt,16'h01dc)=00000004;
    rlt = (*((volatile unsigned int*) 0x980401cc));//`READ_APB(rlt,16'h01cc)=38240089;
    rlt = (*((volatile unsigned int*) 0x980401d0));//`READ_APB(rlt,16'h01d0)=00000000;
    //in vtDumpPicture
    //in vIO_DumpDpb
    //in vtPicEnd
    rlt = (*((volatile unsigned int*) 0x98040120));//`READ_APB(rlt,16'h0120)=01072d1a;
    rlt = (*((volatile unsigned int*) 0x98040124));//`READ_APB(rlt,16'h0124)=01080000;
    rlt = (*((volatile unsigned int*) 0x98040150));//`READ_APB(rlt,16'h0150)=0000001f;
    rlt = (*((volatile unsigned int*) 0x98040114));//`READ_APB(rlt,16'h0114)=00000000;
    rlt = (*((volatile unsigned int*) 0x9804014c));//`READ_APB(rlt,16'h014c)=004768ad;
	printf("0x9804014c, rlt = 0x%08x\n",rlt);
//    $display("[RTK_C] DELAY_C(4); cclk");
    //DELAY_C(4);
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //$display("[RTK_CHK] end POLLING check @ 1402");
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //in vtPicInit
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    //in vtGenAxiRcRandPattern
    (*((volatile unsigned int*) 0x98040120)=(0x01072d1a+BASE_ADDRESS));//`WRITE_APB(16'h0120, 32'h01072d1a);
    (*((volatile unsigned int*) 0x98040124)=(0x01080000+BASE_ADDRESS));//`WRITE_APB(16'h0124, 32'h01080000);
    (*((volatile unsigned int*) 0x98040150)=0x0000001f);//`WRITE_APB(16'h0150, 32'h0000001f);
    //(*((volatile unsigned int*) 0x98040114)=0x00000000);//`WRITE_APB(16'h0114, 32'h00000000); //FuChun mark
    //in vtSetFrameMapReg
    (*((volatile unsigned int*) 0x98041800)=0x00000040);//`WRITE_APB(16'h1800, 32'h00000040);
    (*((volatile unsigned int*) 0x98041804)=0x00000040);//`WRITE_APB(16'h1804, 32'h00000040);
    (*((volatile unsigned int*) 0x98041808)=0x00000040);//`WRITE_APB(16'h1808, 32'h00000040);
    (*((volatile unsigned int*) 0x9804180c)=0x00000040);//`WRITE_APB(16'h180c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041810)=0x00000040);//`WRITE_APB(16'h1810, 32'h00000040);
    (*((volatile unsigned int*) 0x98041814)=0x00000040);//`WRITE_APB(16'h1814, 32'h00000040);
    (*((volatile unsigned int*) 0x98041818)=0x00000040);//`WRITE_APB(16'h1818, 32'h00000040);
    (*((volatile unsigned int*) 0x9804181c)=0x00000040);//`WRITE_APB(16'h181c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041820)=0x00000040);//`WRITE_APB(16'h1820, 32'h00000040);
    (*((volatile unsigned int*) 0x98041824)=0x00000040);//`WRITE_APB(16'h1824, 32'h00000040);
    (*((volatile unsigned int*) 0x98041828)=0x00000040);//`WRITE_APB(16'h1828, 32'h00000040);
    (*((volatile unsigned int*) 0x9804182c)=0x00000040);//`WRITE_APB(16'h182c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041830)=0x00000040);//`WRITE_APB(16'h1830, 32'h00000040);
    (*((volatile unsigned int*) 0x98041834)=0x00000040);//`WRITE_APB(16'h1834, 32'h00000040);
    (*((volatile unsigned int*) 0x98041838)=0x00000040);//`WRITE_APB(16'h1838, 32'h00000040);
    (*((volatile unsigned int*) 0x9804183c)=0x00000040);//`WRITE_APB(16'h183c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041840)=0x00000040);//`WRITE_APB(16'h1840, 32'h00000040);
    (*((volatile unsigned int*) 0x98041844)=0x00000040);//`WRITE_APB(16'h1844, 32'h00000040);
    (*((volatile unsigned int*) 0x98041848)=0x00000040);//`WRITE_APB(16'h1848, 32'h00000040);
    (*((volatile unsigned int*) 0x9804184c)=0x00000040);//`WRITE_APB(16'h184c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041850)=0x00000040);//`WRITE_APB(16'h1850, 32'h00000040);
    (*((volatile unsigned int*) 0x98041854)=0x00000040);//`WRITE_APB(16'h1854, 32'h00000040);
    (*((volatile unsigned int*) 0x98041858)=0x00000040);//`WRITE_APB(16'h1858, 32'h00000040);
    (*((volatile unsigned int*) 0x9804185c)=0x00000040);//`WRITE_APB(16'h185c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041860)=0x00000040);//`WRITE_APB(16'h1860, 32'h00000040);
    (*((volatile unsigned int*) 0x98041864)=0x00000040);//`WRITE_APB(16'h1864, 32'h00000040);
    (*((volatile unsigned int*) 0x98041868)=0x00000040);//`WRITE_APB(16'h1868, 32'h00000040);
    (*((volatile unsigned int*) 0x9804186c)=0x00000040);//`WRITE_APB(16'h186c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041870)=0x00000040);//`WRITE_APB(16'h1870, 32'h00000040);
    (*((volatile unsigned int*) 0x98041874)=0x00000040);//`WRITE_APB(16'h1874, 32'h00000040);
    (*((volatile unsigned int*) 0x98041878)=0x00000040);//`WRITE_APB(16'h1878, 32'h00000040);
    (*((volatile unsigned int*) 0x9804187c)=0x00000040);//`WRITE_APB(16'h187c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041880)=0x00000040);//`WRITE_APB(16'h1880, 32'h00000040);
    (*((volatile unsigned int*) 0x98041884)=0x00000040);//`WRITE_APB(16'h1884, 32'h00000040);
    (*((volatile unsigned int*) 0x98041888)=0x00000040);//`WRITE_APB(16'h1888, 32'h00000040);
    (*((volatile unsigned int*) 0x9804188c)=0x00000040);//`WRITE_APB(16'h188c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041890)=0x00000040);//`WRITE_APB(16'h1890, 32'h00000040);
    (*((volatile unsigned int*) 0x98041894)=0x00000040);//`WRITE_APB(16'h1894, 32'h00000040);
    (*((volatile unsigned int*) 0x98041898)=0x00000040);//`WRITE_APB(16'h1898, 32'h00000040);
    (*((volatile unsigned int*) 0x9804189c)=0x00000040);//`WRITE_APB(16'h189c, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a0)=0x00000040);//`WRITE_APB(16'h18a0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a4)=0x00000040);//`WRITE_APB(16'h18a4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a8)=0x00000040);//`WRITE_APB(16'h18a8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ac)=0x00000040);//`WRITE_APB(16'h18ac, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b0)=0x00000040);//`WRITE_APB(16'h18b0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b4)=0x00000040);//`WRITE_APB(16'h18b4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b8)=0x00000040);//`WRITE_APB(16'h18b8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418bc)=0x00000040);//`WRITE_APB(16'h18bc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c0)=0x00000040);//`WRITE_APB(16'h18c0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c4)=0x00000040);//`WRITE_APB(16'h18c4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c8)=0x00000040);//`WRITE_APB(16'h18c8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418cc)=0x00000040);//`WRITE_APB(16'h18cc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d0)=0x00000040);//`WRITE_APB(16'h18d0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d4)=0x00000040);//`WRITE_APB(16'h18d4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d8)=0x00000040);//`WRITE_APB(16'h18d8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418dc)=0x00000040);//`WRITE_APB(16'h18dc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e0)=0x00000040);//`WRITE_APB(16'h18e0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e4)=0x00000040);//`WRITE_APB(16'h18e4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e8)=0x00000040);//`WRITE_APB(16'h18e8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ec)=0x00000040);//`WRITE_APB(16'h18ec, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f0)=0x00000040);//`WRITE_APB(16'h18f0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f4)=0x00000040);//`WRITE_APB(16'h18f4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f8)=0x00000040);//`WRITE_APB(16'h18f8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418fc)=0x00000040);//`WRITE_APB(16'h18fc, 32'h00000040);
    (*((volatile unsigned int*) 0x98041900)=0x00000000);//`WRITE_APB(16'h1900, 32'h00000000);
    //in vtDecodePicture
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    (*((volatile unsigned int*) 0x98040194)=0x00000000);//`WRITE_APB(16'h0194, 32'h00000000);
    (*((volatile unsigned int*) 0x98040198)=0x00000000);//`WRITE_APB(16'h0198, 32'h00000000);
    (*((volatile unsigned int*) 0x98040180)=0x00000000);//`WRITE_APB(16'h0180, 32'h00000000);
    (*((volatile unsigned int*) 0x98040170)=0x00004008);//`WRITE_APB(16'h0170, 32'h00004008);
    //in vtCommand2Coda980
    (*((volatile unsigned int*) 0x98040168)=0x00000000);//`WRITE_APB(16'h0168, 32'h00000000);
    (*((volatile unsigned int*) 0x9804016c)=0x00000000);//`WRITE_APB(16'h016c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040178)=0x00000000);//`WRITE_APB(16'h0178, 32'h00000000);
    (*((volatile unsigned int*) 0x98040164)=0x00000003);//`WRITE_APB(16'h0164, 32'h00000003);
    //$display("[RTK_CHK] POLLING check @ 8562");
    asm ("POLL_8562_9:");
    pic_done = 0;
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    while(!pic_done){
		ve_isr();
        //while( !((volatile int)touch_isr)){}
        //touch_isr = 0;
    }
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    rlt = (*((volatile unsigned int*) 0x980401c8));//`READ_APB(rlt,16'h01c8)=00000000;
    rlt = (*((volatile unsigned int*) 0x980401c4));//`READ_APB(rlt,16'h01c4)=00000001;
    rlt = (*((volatile unsigned int*) 0x980401dc));//`READ_APB(rlt,16'h01dc)=00000005;
    rlt = (*((volatile unsigned int*) 0x980401cc));//`READ_APB(rlt,16'h01cc)=38240089;
    rlt = (*((volatile unsigned int*) 0x980401d0));//`READ_APB(rlt,16'h01d0)=00000000;
    //in vtDumpPicture
    //in vIO_DumpDpb
    rlt = (*((volatile unsigned int*) 0x98040150));//`READ_APB(rlt,16'h0150)=0000003f;
    (*((volatile unsigned int*) 0x98040150)=0x0000003e);//`WRITE_APB(16'h0150, 32'h0000003e);
    //in vtPicEnd
    rlt = (*((volatile unsigned int*) 0x98040120));//`READ_APB(rlt,16'h0120)=01089963;
    rlt = (*((volatile unsigned int*) 0x98040124));//`READ_APB(rlt,16'h0124)=01100000;
    rlt = (*((volatile unsigned int*) 0x98040150));//`READ_APB(rlt,16'h0150)=0000003e;
    rlt = (*((volatile unsigned int*) 0x98040114));//`READ_APB(rlt,16'h0114)=00000000;
    rlt = (*((volatile unsigned int*) 0x9804014c));//`READ_APB(rlt,16'h014c)=0046147a;
	printf("0x9804014c, rlt = 0x%08x\n",rlt);
//    $display("[RTK_C] DELAY_C(4); cclk");
    //DELAY_C(4);
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //$display("[RTK_CHK] end POLLING check @ 1402");
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //in vtPicInit
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    //in vtGenAxiRcRandPattern
    (*((volatile unsigned int*) 0x98040120)=(0x01089963+BASE_ADDRESS));//`WRITE_APB(16'h0120, 32'h01089963);
    (*((volatile unsigned int*) 0x98040124)=(0x01100000+BASE_ADDRESS));//`WRITE_APB(16'h0124, 32'h01100000);
    (*((volatile unsigned int*) 0x98040150)=0x0000003e);//`WRITE_APB(16'h0150, 32'h0000003e);
    //(*((volatile unsigned int*) 0x98040114)=0x00000000);//`WRITE_APB(16'h0114, 32'h00000000); //FuChun mark
    //in vtSetFrameMapReg
    (*((volatile unsigned int*) 0x98041800)=0x00000040);//`WRITE_APB(16'h1800, 32'h00000040);
    (*((volatile unsigned int*) 0x98041804)=0x00000040);//`WRITE_APB(16'h1804, 32'h00000040);
    (*((volatile unsigned int*) 0x98041808)=0x00000040);//`WRITE_APB(16'h1808, 32'h00000040);
    (*((volatile unsigned int*) 0x9804180c)=0x00000040);//`WRITE_APB(16'h180c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041810)=0x00000040);//`WRITE_APB(16'h1810, 32'h00000040);
    (*((volatile unsigned int*) 0x98041814)=0x00000040);//`WRITE_APB(16'h1814, 32'h00000040);
    (*((volatile unsigned int*) 0x98041818)=0x00000040);//`WRITE_APB(16'h1818, 32'h00000040);
    (*((volatile unsigned int*) 0x9804181c)=0x00000040);//`WRITE_APB(16'h181c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041820)=0x00000040);//`WRITE_APB(16'h1820, 32'h00000040);
    (*((volatile unsigned int*) 0x98041824)=0x00000040);//`WRITE_APB(16'h1824, 32'h00000040);
    (*((volatile unsigned int*) 0x98041828)=0x00000040);//`WRITE_APB(16'h1828, 32'h00000040);
    (*((volatile unsigned int*) 0x9804182c)=0x00000040);//`WRITE_APB(16'h182c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041830)=0x00000040);//`WRITE_APB(16'h1830, 32'h00000040);
    (*((volatile unsigned int*) 0x98041834)=0x00000040);//`WRITE_APB(16'h1834, 32'h00000040);
    (*((volatile unsigned int*) 0x98041838)=0x00000040);//`WRITE_APB(16'h1838, 32'h00000040);
    (*((volatile unsigned int*) 0x9804183c)=0x00000040);//`WRITE_APB(16'h183c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041840)=0x00000040);//`WRITE_APB(16'h1840, 32'h00000040);
    (*((volatile unsigned int*) 0x98041844)=0x00000040);//`WRITE_APB(16'h1844, 32'h00000040);
    (*((volatile unsigned int*) 0x98041848)=0x00000040);//`WRITE_APB(16'h1848, 32'h00000040);
    (*((volatile unsigned int*) 0x9804184c)=0x00000040);//`WRITE_APB(16'h184c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041850)=0x00000040);//`WRITE_APB(16'h1850, 32'h00000040);
    (*((volatile unsigned int*) 0x98041854)=0x00000040);//`WRITE_APB(16'h1854, 32'h00000040);
    (*((volatile unsigned int*) 0x98041858)=0x00000040);//`WRITE_APB(16'h1858, 32'h00000040);
    (*((volatile unsigned int*) 0x9804185c)=0x00000040);//`WRITE_APB(16'h185c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041860)=0x00000040);//`WRITE_APB(16'h1860, 32'h00000040);
    (*((volatile unsigned int*) 0x98041864)=0x00000040);//`WRITE_APB(16'h1864, 32'h00000040);
    (*((volatile unsigned int*) 0x98041868)=0x00000040);//`WRITE_APB(16'h1868, 32'h00000040);
    (*((volatile unsigned int*) 0x9804186c)=0x00000040);//`WRITE_APB(16'h186c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041870)=0x00000040);//`WRITE_APB(16'h1870, 32'h00000040);
    (*((volatile unsigned int*) 0x98041874)=0x00000040);//`WRITE_APB(16'h1874, 32'h00000040);
    (*((volatile unsigned int*) 0x98041878)=0x00000040);//`WRITE_APB(16'h1878, 32'h00000040);
    (*((volatile unsigned int*) 0x9804187c)=0x00000040);//`WRITE_APB(16'h187c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041880)=0x00000040);//`WRITE_APB(16'h1880, 32'h00000040);
    (*((volatile unsigned int*) 0x98041884)=0x00000040);//`WRITE_APB(16'h1884, 32'h00000040);
    (*((volatile unsigned int*) 0x98041888)=0x00000040);//`WRITE_APB(16'h1888, 32'h00000040);
    (*((volatile unsigned int*) 0x9804188c)=0x00000040);//`WRITE_APB(16'h188c, 32'h00000040);
    (*((volatile unsigned int*) 0x98041890)=0x00000040);//`WRITE_APB(16'h1890, 32'h00000040);
    (*((volatile unsigned int*) 0x98041894)=0x00000040);//`WRITE_APB(16'h1894, 32'h00000040);
    (*((volatile unsigned int*) 0x98041898)=0x00000040);//`WRITE_APB(16'h1898, 32'h00000040);
    (*((volatile unsigned int*) 0x9804189c)=0x00000040);//`WRITE_APB(16'h189c, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a0)=0x00000040);//`WRITE_APB(16'h18a0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a4)=0x00000040);//`WRITE_APB(16'h18a4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418a8)=0x00000040);//`WRITE_APB(16'h18a8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ac)=0x00000040);//`WRITE_APB(16'h18ac, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b0)=0x00000040);//`WRITE_APB(16'h18b0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b4)=0x00000040);//`WRITE_APB(16'h18b4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418b8)=0x00000040);//`WRITE_APB(16'h18b8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418bc)=0x00000040);//`WRITE_APB(16'h18bc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c0)=0x00000040);//`WRITE_APB(16'h18c0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c4)=0x00000040);//`WRITE_APB(16'h18c4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418c8)=0x00000040);//`WRITE_APB(16'h18c8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418cc)=0x00000040);//`WRITE_APB(16'h18cc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d0)=0x00000040);//`WRITE_APB(16'h18d0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d4)=0x00000040);//`WRITE_APB(16'h18d4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418d8)=0x00000040);//`WRITE_APB(16'h18d8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418dc)=0x00000040);//`WRITE_APB(16'h18dc, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e0)=0x00000040);//`WRITE_APB(16'h18e0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e4)=0x00000040);//`WRITE_APB(16'h18e4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418e8)=0x00000040);//`WRITE_APB(16'h18e8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418ec)=0x00000040);//`WRITE_APB(16'h18ec, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f0)=0x00000040);//`WRITE_APB(16'h18f0, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f4)=0x00000040);//`WRITE_APB(16'h18f4, 32'h00000040);
    (*((volatile unsigned int*) 0x980418f8)=0x00000040);//`WRITE_APB(16'h18f8, 32'h00000040);
    (*((volatile unsigned int*) 0x980418fc)=0x00000040);//`WRITE_APB(16'h18fc, 32'h00000040);
    (*((volatile unsigned int*) 0x98041900)=0x00000000);//`WRITE_APB(16'h1900, 32'h00000000);
    //in vtDecodePicture
    (*((volatile unsigned int*) 0x98040104)=(0x00342800+BASE_ADDRESS));//`WRITE_APB(16'h0104, 32'h00342800);
    (*((volatile unsigned int*) 0x98040194)=0x00000000);//`WRITE_APB(16'h0194, 32'h00000000);
    (*((volatile unsigned int*) 0x98040198)=0x00000000);//`WRITE_APB(16'h0198, 32'h00000000);
    (*((volatile unsigned int*) 0x98040180)=0x00000000);//`WRITE_APB(16'h0180, 32'h00000000);
    (*((volatile unsigned int*) 0x98040170)=0x00004008);//`WRITE_APB(16'h0170, 32'h00004008);
    //in vtCommand2Coda980
    (*((volatile unsigned int*) 0x98040168)=0x00000000);//`WRITE_APB(16'h0168, 32'h00000000);
    (*((volatile unsigned int*) 0x9804016c)=0x00000000);//`WRITE_APB(16'h016c, 32'h00000000);
    (*((volatile unsigned int*) 0x98040178)=0x00000000);//`WRITE_APB(16'h0178, 32'h00000000);
    (*((volatile unsigned int*) 0x98040164)=0x00000003);//`WRITE_APB(16'h0164, 32'h00000003);
    //$display("[RTK_CHK] POLLING check @ 8562");
    asm ("POLL_8562_10:");
    pic_done = 0;
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    while(!pic_done){
		ve_isr();
        //while( !((volatile int)touch_isr)){}
        //touch_isr = 0;
    }
	ve_simulation = current_time_hires();
	printf("%s %d Time: %d us\n",__func__, __LINE__, (int)(ve_simulation - ve_start));
    rlt = (*((volatile unsigned int*) 0x980401c8));//`READ_APB(rlt,16'h01c8)=00000000;
    rlt = (*((volatile unsigned int*) 0x980401c4));//`READ_APB(rlt,16'h01c4)=00000002;
    rlt = (*((volatile unsigned int*) 0x980401dc));//`READ_APB(rlt,16'h01dc)=00000000;
    rlt = (*((volatile unsigned int*) 0x980401cc));//`READ_APB(rlt,16'h01cc)=38240089;
    rlt = (*((volatile unsigned int*) 0x980401d0));//`READ_APB(rlt,16'h01d0)=00000000;
    //in vtDumpPicture
    //in vIO_DumpDpb
    rlt = (*((volatile unsigned int*) 0x98040150));//`READ_APB(rlt,16'h0150)=0000003f;
    (*((volatile unsigned int*) 0x98040150)=0x0000003d);//`WRITE_APB(16'h0150, 32'h0000003d);
    //in vtPicEnd
    rlt = (*((volatile unsigned int*) 0x98040120));//`READ_APB(rlt,16'h0120)=0109fa76;
    rlt = (*((volatile unsigned int*) 0x98040124));//`READ_APB(rlt,16'h0124)=01100000;
    rlt = (*((volatile unsigned int*) 0x98040150));//`READ_APB(rlt,16'h0150)=0000003d;
    rlt = (*((volatile unsigned int*) 0x98040114));//`READ_APB(rlt,16'h0114)=00000000;
    rlt = (*((volatile unsigned int*) 0x9804014c));//`READ_APB(rlt,16'h014c)=0045af2f;
	printf("0x9804014c, rlt = 0x%08x\n",rlt);
//    $display("[RTK_C] DELAY_C(4); cclk");
    //DELAY_C(4);
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    //$display("[RTK_CHK] end POLLING check @ 1402");
//    $display("[RTK_C] DELAY_C(32); cclk");
    //DELAY_C(32);
    diag_pass();
}
void diag_pass(void) {
	printf("PASS\n");
}

void diag_fail(void) {
	printf("FAIL\n");
}
void ve_isr(void) {
    int rlt;
    rlt = (*((volatile unsigned int*) 0x98040174));//`READ_APB(rlt,16'h0174);
    if(rlt != 0){
        (*((volatile unsigned int*) 0x98040174)=0x00000000);//`WRITE_APB(16'h174,0);
    }
    if(rlt&8){
        pic_done = 1;
    }
    (*((volatile unsigned int*) 0x9804000c)=0x00000001);//`WRITE_APB(16'h00c,1);
    touch_isr = 1;
}

int ve_test_cmd(int argc, const cmd_args *argv)
{
	ve_test_main();
	return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("ve_test", "block device GPT command", (console_cmd)&ve_test_cmd)
STATIC_COMMAND_END(ve_test_cmd);
