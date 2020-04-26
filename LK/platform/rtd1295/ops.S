/*
 * Copyright (c) 2014, Google Inc. All rights reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <asm.h>
#include <arch/ops.h>
#include <arch/defines.h>
#include <arch/asm_macros.h>

.text

/* Turn on I/D Cache and MMU */
/* void rtk_mmu_on(u64 ttbr_addr, u64 tcr_flags, u64 mair) */
FUNCTION(rtk_mmu_on)
	dsb	sy
	mrs	x3, CurrentEL
	lsr	x3, x3, #2
	switch_el x3, 1f, 2f, 3f
1:
	msr	ttbr0_el1, x0
	msr	tcr_el1, x1
	msr	mair_el1, x2
	isb
	mrs	x0, sctlr_el1
	orr	x0, x0, #1
	orr	x0, x0, #(1 << 2)
	msr	sctlr_el1, x0
	b	0f
2:
	msr	ttbr0_el2, x0
	msr	tcr_el2, x1
	msr	mair_el2, x2
	isb
	mrs	x0, sctlr_el2
	orr	x0, x0, #1
	orr	x0, x0, #(1 << 2)
	msr	sctlr_el2, x0
	b	0f
3:
	msr	ttbr0_el3, x0
	msr	tcr_el3, x1
	msr	mair_el3, x2
	isb
	mrs	x0, sctlr_el3
	orr	x0, x0, #1
	orr	x0, x0, #(1 << 2)
	msr	sctlr_el3, x0
0:
	isb
	ret

/* void rtk_cache_coherence_setting(void) */
FUNCTION(rtk_cache_coherence_setting)
	mov	x0, #0x1
	lsl	x0, x0, #6
	msr	S3_1_C15_C2_1, x0 /* set up each core CPUECTLR.SMPEN bit for Cache Coherency */
	isb

	/* setup ACTLR_EL3(2).CPUECTLR_EL1 bit*/
	mov	x0, #0x2
	mrs	x1, ACTLR_EL3
	orr	x1, x1, x0
	msr	ACTLR_EL3, x1
	mrs	x1, ACTLR_EL2
	orr	x1, x1, x0
	msr	ACTLR_EL2, x1
	isb
	ret

#if JTAG_DEBUG
FUNCTION(jtag_on)
	ldr     x10, =__stack
	add     sp, x10, #ARCH_DEFAULT_STACK_SIZE
	stp	x29, x30, [sp,#-16]!

	ldr	x10, =0x9801A90C
	mov	w11, #0x0000
	movk	w11, #0x0010, lsl #16
	str	w11, [x10]
	ldr	x10, =0x9801A910
	mov	w11, #0xa6db
	movk	w11, #0x002a, lsl #16
	str	w11, [x10]

	bl	uart_init_early
	mov	x10, x0
	adr	x11, jtag_str
0:
	ldrb	w0, [x11], #1
	cbz	w0, 1f
	bl	platform_dputc
	b	0b
1:
	mov	x0, x10
	ldp	x29, x30, [sp], #16
	ret

jtag_str: .asciz "\n- JTAG Ready -\n"
        .align 4
#endif
