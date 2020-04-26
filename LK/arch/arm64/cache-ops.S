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

.macro cache_range_op, cache op
    add     x2, x0, x1                  // calculate the end address
    bic     x3, x0, #(CACHE_LINE-1)     // align the start with a cache line
.Lcache_range_op_loop\@:
    \cache  \op, x3
    add     x3, x3, #CACHE_LINE
    cmp     x3, x2
    blo     .Lcache_range_op_loop\@
    dsb     sy
.endm

    /* void arch_flush_cache_range(addr_t start, size_t len); */
FUNCTION(arch_clean_cache_range)
    cache_range_op dc cvac         // clean cache to PoC by MVA
    ret

    /* void arch_flush_invalidate_cache_range(addr_t start, size_t len); */
FUNCTION(arch_clean_invalidate_cache_range)
    cache_range_op dc civac        // clean & invalidate dcache to PoC by MVA
    ret

    /* void arch_invalidate_cache_range(addr_t start, size_t len); */
FUNCTION(arch_invalidate_cache_range)
    cache_range_op dc ivac         // invalidate dcache to PoC by MVA
    ret

    /* void arch_sync_cache_range(addr_t start, size_t len); */
FUNCTION(arch_sync_cache_range)
    cache_range_op dc cvau         // clean dcache to PoU by MVA
    cache_range_op ic ivau         // invalidate icache to PoU by MVA
    ret

	/* void arch_invalidate_icache_all(void) */
FUNCTION(arch_invalidate_icache_all)
	ic	ialluis
	isb	sy
	ret

	/* void arch_disable_cache(uint flags) */
FUNCTION(arch_disable_cache)
	dsb	sy
	mrs	x4, CurrentEL
	lsr	x4, x4, #2

	/* Check I-CACHE first */
	ands	x1, x0, #ICACHE
	beq	dcache_disable // I-CACHE flag not set, jump to D-CACHE

	switch_el x4, 1f, 2f, 3f
1:	mrs	x2, sctlr_el1
	bic	x2, x2, #(1<<12)
	msr	sctlr_el1, x2
	b	dcache_disable
2:	mrs	x2, sctlr_el2
	bic	x2, x2, #(1<<12)
	msr	sctlr_el2, x2
	b	dcache_disable
3:	mrs	x2, sctlr_el3
	bic	x2, x2, #(1<<12)
	msr	sctlr_el3, x2

dcache_disable:  /* Check D-CACHE */
	isb
	ands	x1, x0, #DCACHE
	beq	finish

	switch_el x4, 1f, 2f, 3f
1:	mrs	x2, sctlr_el1
	bic	x2, x2, #(1<<2)
	msr	sctlr_el1, x2
	b	finish
2:	mrs	x2, sctlr_el2
	bic	x2, x2, #(1<<2)
	msr	sctlr_el2, x2
	b	finish
3:	mrs	x2, sctlr_el3
	bic	x2, x2, #(1<<2)
	msr	sctlr_el3, x2
finish:
	isb
	ret

	/* void arch_disable_mmu(void) */
FUNCTION(arch_disable_mmu)
	mrs	x4, CurrentEL
	lsr	x4, x4, #2
	switch_el x4, 1f, 2f, 3f
1:	mrs	x2, sctlr_el1
	bic	x2, x2, #1
	msr	sctlr_el1, x2
	b	0f
2:	mrs	x2, sctlr_el2
	bic	x2, x2, #1
	msr	sctlr_el2, x2
	b	0f
3:	mrs	x2, sctlr_el3
	bic	x2, x2, #1
	msr	sctlr_el3, x2
0:
	isb
	ret
