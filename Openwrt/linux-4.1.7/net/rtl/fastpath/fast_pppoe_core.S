/*
 * Copyright (C) 2017 Realtek Semiconductor Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

	.file	1 "fast_pppoe_core.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.text
	.align	2
	.set	nomips16
	.ent	fastpath_pppoe_single_write
	.type	fastpath_pppoe_single_write, @function
fastpath_pppoe_single_write:
	.frame	$sp,16,$31		# vars= 8, regs= 1/0, args= 0, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-16
	sw	$31,12($sp)
	sltu	$3,$6,2
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L6
	move	$2,$6
	.set	macro
	.set	reorder

$L2:
	li	$2,-14			# 0xfffffffffffffff2
$L3:
	lw	$31,12($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,16
	.set	macro
	.set	reorder

$L6:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L2
	addiu	$3,$5,1
	.set	macro
	.set	reorder

	or	$4,$3,$5
	lw	$3,24($28)
	and	$4,$4,$3
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L2
	move	$4,$sp
	.set	macro
	.set	reorder

	li	$6,1			# 0x1
 #APP
 # 69 "net/rtl/fastpath/fast_pppoe_core.c" 1
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L2
	lb	$4,0($sp)
	.set	macro
	.set	reorder

	lui	$3,%hi(fast_pppoe_fw)
	addiu	$4,$4,-48
	.set	noreorder
	.set	nomacro
	j	$L3
	sw	$4,%lo(fast_pppoe_fw)($3)
	.set	macro
	.set	reorder

	.end	fastpath_pppoe_single_write
	.size	fastpath_pppoe_single_write, .-fastpath_pppoe_single_write
	.align	2
	.set	nomips16
	.ent	find_pppoe_info
	.type	find_pppoe_info, @function
find_pppoe_info:
	.frame	$sp,64,$31		# vars= 8, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$fp,56($sp)
	sw	$23,52($sp)
	sw	$22,48($sp)
	sw	$21,44($sp)
	sw	$20,40($sp)
	sw	$19,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	lw	$20,80($sp)
	lw	$19,84($sp)
	lw	$18,88($sp)
	lui	$22,%hi(fast_pppoe_info+54)
	lui	$23,%hi(fast_pppoe_info)
	move	$16,$4
	move	$17,$5
	sw	$7,76($sp)
	andi	$fp,$6,0xffff
	addiu	$22,$22,%lo(fast_pppoe_info+54)
	addiu	$23,$23,%lo(fast_pppoe_info)
	move	$21,$0
$L16:
	lhu	$2,0($22)
	beq	$2,$0,$L8
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L9
	sll	$2,$21,2
	.set	macro
	.set	reorder

	sll	$3,$21,4
	subu	$3,$3,$2
	subu	$3,$3,$21
	sll	$3,$3,3
	addiu	$4,$3,16
	addu	$4,$23,$4
	move	$5,$16
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$2,($4)
1:	lbu	$1,($5)
	addiu	$4,1
	bne	$1,$2,2f
	addiu	$5,1
	bnez	$2,1b
	lbu	$2,($4)
	move	$2,$1
2:	subu	$2,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L23
	addu	$2,$23,$3
	.set	macro
	.set	reorder

$L9:
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L11
	sll	$2,$21,2
	.set	macro
	.set	reorder

	sll	$3,$21,4
	subu	$3,$3,$2
	subu	$3,$3,$21
	sll	$3,$3,3
	addu	$3,$23,$3
	move	$4,$3
	move	$5,$17
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$2,($4)
1:	lbu	$1,($5)
	addiu	$4,1
	bne	$1,$2,2f
	addiu	$5,1
	bnez	$2,1b
	lbu	$2,($4)
	move	$2,$1
2:	subu	$2,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L22
	lw	$31,60($sp)
	.set	macro
	.set	reorder

$L11:
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L12
	sll	$2,$21,2
	.set	macro
	.set	reorder

	sll	$3,$21,4
	subu	$3,$3,$2
	subu	$3,$3,$21
	sll	$3,$3,3
	addu	$4,$23,$3
	addiu	$4,$4,38
	move	$5,$18
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcmp
	sw	$3,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L20
	lw	$3,16($sp)
	.set	macro
	.set	reorder

$L12:
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L13
	sll	$2,$21,2
	.set	macro
	.set	reorder

	sll	$3,$21,4
	subu	$3,$3,$2
	subu	$3,$3,$21
	sll	$3,$3,3
	addiu	$4,$3,32
	addu	$4,$23,$4
	move	$5,$19
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcmp
	sw	$3,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L20
	lw	$3,16($sp)
	.set	macro
	.set	reorder

$L13:
	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L24
	lw	$2,76($sp)
	.set	macro
	.set	reorder

	lw	$2,-6($22)
	.set	noreorder
	.set	nomacro
	beq	$2,$20,$L21
	lw	$2,76($sp)
	.set	macro
	.set	reorder

$L24:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L15
	lw	$3,76($sp)
	.set	macro
	.set	reorder

	lw	$2,-10($22)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L25
	sll	$3,$21,4
	.set	macro
	.set	reorder

$L15:
	beq	$fp,$0,$L8
	lhu	$2,-2($22)
	.set	noreorder
	.set	nomacro
	beq	$2,$fp,$L25
	sll	$3,$21,4
	.set	macro
	.set	reorder

$L8:
	addiu	$21,$21,1
	li	$2,16			# 0x10
	.set	noreorder
	.set	nomacro
	bne	$21,$2,$L16
	addiu	$22,$22,88
	.set	macro
	.set	reorder

	lw	$31,60($sp)
	lw	$fp,56($sp)
	lw	$23,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L20:
	addu	$2,$23,$3
$L23:
	lw	$31,60($sp)
	lw	$fp,56($sp)
	lw	$23,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L21:
	sll	$3,$21,4
$L25:
	sll	$2,$21,2
	subu	$2,$3,$2
	subu	$21,$2,$21
	lui	$2,%hi(fast_pppoe_info)
	sll	$21,$21,3
	addiu	$2,$2,%lo(fast_pppoe_info)
	addu	$2,$2,$21
	lw	$31,60($sp)
	lw	$fp,56($sp)
	lw	$23,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L22:
	lw	$fp,56($sp)
	lw	$23,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	move	$2,$3
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

	.end	find_pppoe_info
	.size	find_pppoe_info, .-find_pppoe_info
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"ppp\000"
	.text
	.align	2
	.globl	fast_pppoe_xmit
	.set	nomips16
	.ent	fast_pppoe_xmit
	.type	fast_pppoe_xmit, @function
fast_pppoe_xmit:
	.frame	$sp,56,$31		# vars= 0, regs= 5/0, args= 32, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$19,48($sp)
	sw	$18,44($sp)
	sw	$17,40($sp)
	sw	$16,36($sp)
	lui	$2,%hi(fast_pppoe_fw)
	lw	$2,%lo(fast_pppoe_fw)($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L30
	move	$16,$4
	.set	macro
	.set	reorder

$L27:
	move	$2,$0
$L28:
	lw	$31,52($sp)
	lw	$19,48($sp)
	lw	$18,44($sp)
	lw	$17,40($sp)
	lw	$16,36($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,56
	.set	macro
	.set	reorder

$L30:
	jal	rtl_get_skb_dev
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L28
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$3,%hi($LC0)
	addiu	$3,$3,%lo($LC0)
	li	$4,3			# 0x3
 #APP
 # 108 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$5,($2)
	beqz	$4,2f
	lbu	$1,($3)
	subu	$4,1
	bne	$1,$5,3f
	addiu	$2,1
	bnez	$5,1b
	addiu	$3,1
2:
	move	$5,$1
3:	subu	$5,$1
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L28
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_headroom
	move	$4,$16
	.set	macro
	.set	reorder

	sltu	$2,$2,22
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L28
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_cloned
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L28
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_shared
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L28
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	move	$4,$16
	.set	macro
	.set	reorder

	move	$4,$2
	move	$5,$0
	move	$6,$0
	move	$7,$0
	sw	$0,16($sp)
	sw	$0,20($sp)
	.set	noreorder
	.set	nomacro
	jal	find_pppoe_info
	sw	$0,24($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L27
	move	$17,$2
	.set	macro
	.set	reorder

	lb	$2,16($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L28
	move	$2,$0
	.set	macro
	.set	reorder

	lb	$2,0($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L28
	move	$2,$0
	.set	macro
	.set	reorder

	lhu	$2,52($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L28
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$4,$16
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	move	$19,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L27
	move	$18,$2
	.set	macro
	.set	reorder

	li	$5,2			# 0x2
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_push
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$16
	.set	macro
	.set	reorder

	li	$3,8448			# 0x2100
	sh	$3,0($2)
	li	$5,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_push
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$16
	.set	macro
	.set	reorder

	lbu	$3,0($2)
	li	$4,1			# 0x1
	sb	$0,1($2)
	ins	$3,$4,4,4
	ins	$3,$4,0,4
	sb	$3,0($2)
	lhu	$4,52($17)
	addiu	$3,$19,2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	andi	$3,$3,0xffff
	srl	$6,$3,8
	srl	$5,$4,8
	sb	$6,5($2)
	sb	$3,4($2)
	sb	$5,3($2)
	sb	$4,2($2)
	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dev
	move	$4,$16
	.set	macro
	.set	reorder

	li	$5,2			# 0x2
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_push
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$16
	.set	macro
	.set	reorder

	li	$3,25736			# 0x6488
	sh	$3,0($2)
	li	$5,12			# 0xc
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_push
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$16
	.set	macro
	.set	reorder

	move	$4,$2
	addiu	$5,$17,38
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$5,396($18)
	addiu	$4,$2,6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	ip_finish_output4
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L28
	sw	$3,60($17)
	.set	macro
	.set	reorder

	.end	fast_pppoe_xmit
	.size	fast_pppoe_xmit, .-fast_pppoe_xmit
	.align	2
	.globl	get_pppoe_last_rx_tx
	.set	nomips16
	.ent	get_pppoe_last_rx_tx
	.type	get_pppoe_last_rx_tx, @function
get_pppoe_last_rx_tx:
	.frame	$sp,40,$31		# vars= 0, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	lw	$2,56($sp)
	andi	$6,$6,0xffff
	sw	$2,16($sp)
	lw	$2,60($sp)
	sw	$2,20($sp)
	lw	$2,64($sp)
	jal	find_pppoe_info
	sw	$2,24($sp)

	beq	$2,$0,$L32
	lw	$3,68($sp)

	lw	$4,56($2)
	sw	$4,0($3)
	lw	$3,60($2)
	lw	$2,72($sp)
	sw	$3,0($2)
$L32:
	lw	$31,36($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	get_pppoe_last_rx_tx
	.size	get_pppoe_last_rx_tx, .-get_pppoe_last_rx_tx
	.align	2
	.globl	check_and_restore_pppoe_hdr
	.set	nomips16
	.ent	check_and_restore_pppoe_hdr
	.type	check_and_restore_pppoe_hdr, @function
check_and_restore_pppoe_hdr:
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lui	$2,%hi(fast_pppoe_fw)
	lw	$2,%lo(fast_pppoe_fw)($2)
	bne	$2,$0,$L37
	move	$16,$4

	lw	$31,28($sp)
$L38:
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

$L37:
	jal	rtl_get_skb_pppoe_flag
	nop

	beq	$2,$0,$L38
	lw	$31,28($sp)

	li	$5,8			# 0x8
	jal	rtl_skb_push
	move	$4,$16

	jal	rtl_get_skb_data
	move	$4,$16

	move	$5,$2
	jal	rtl_set_skb_network_header
	move	$4,$16

	jal	rtl_get_skb_data
	move	$4,$16

	move	$5,$2
	jal	rtl_set_skb_transport_header
	move	$4,$16

	jal	rtl_get_skb_dev
	move	$4,$16

	lw	$3,128($2)
	move	$4,$16
	addiu	$3,$3,-1
	jal	rtl_get_skb_dev
	sw	$3,128($2)

	lw	$18,136($2)
	move	$4,$16
	jal	rtl_get_skb_len
	move	$17,$2

	subu	$2,$18,$2
	move	$4,$16
	jal	rtl_get_skb_rx_dev
	sw	$2,136($17)

	move	$5,$2
	jal	rtl_set_skb_dev
	move	$4,$16

	move	$4,$16
	jal	rtl_set_skb_protocol
	li	$5,25736			# 0x6488

	move	$4,$16
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	move	$5,$0
	j	rtl_set_skb_pppoe_flag
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	check_and_restore_pppoe_hdr
	.size	check_and_restore_pppoe_hdr, .-check_and_restore_pppoe_hdr
	.align	2
	.globl	check_and_pull_pppoe_hdr
	.set	nomips16
	.ent	check_and_pull_pppoe_hdr
	.type	check_and_pull_pppoe_hdr, @function
check_and_pull_pppoe_hdr:
	.frame	$sp,56,$31		# vars= 0, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$fp,48($sp)
	sw	$23,44($sp)
	sw	$22,40($sp)
	sw	$21,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lui	$2,%hi(fast_pppoe_fw)
	lw	$2,%lo(fast_pppoe_fw)($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L49
	move	$18,$4
	.set	macro
	.set	reorder

$L40:
	move	$2,$0
$L45:
	lw	$31,52($sp)
	lw	$fp,48($sp)
	lw	$23,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,56
	.set	macro
	.set	reorder

$L49:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_pppoe_flag
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L40
	move	$19,$2
	.set	macro
	.set	reorder

	lhu	$3,12($2)
	li	$2,25736			# 0x6488
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L45
	move	$2,$0
	.set	macro
	.set	reorder

	lhu	$2,20($19)
	li	$3,8448			# 0x2100
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L41
	li	$3,22272			# 0x5700
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L45
	move	$2,$0
	.set	macro
	.set	reorder

$L41:
	lui	$16,%hi(fast_pppoe_info+54)
	lui	$23,%hi(fast_pppoe_info)
	lhu	$21,16($19)
	addiu	$16,$16,%lo(fast_pppoe_info+54)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$21, $21

 # 0 "" 2
 #NO_APP
	addiu	$23,$23,%lo(fast_pppoe_info)
	andi	$21,$21,0xffff
	addiu	$fp,$19,6
	move	$17,$0
	.set	noreorder
	.set	nomacro
	j	$L44
	li	$20,16			# 0x10
	.set	macro
	.set	reorder

$L42:
	addiu	$17,$17,1
$L50:
	.set	noreorder
	.set	nomacro
	beq	$17,$20,$L40
	addiu	$16,$16,88
	.set	macro
	.set	reorder

$L44:
	lhu	$2,0($16)
	beq	$2,$0,$L42
	lhu	$2,-2($16)
	.set	noreorder
	.set	nomacro
	bne	$2,$21,$L42
	sll	$2,$17,2
	.set	macro
	.set	reorder

	sll	$22,$17,4
	subu	$22,$22,$2
	subu	$22,$22,$17
	sll	$22,$22,3
	addu	$4,$23,$22
	addiu	$4,$4,38
	move	$5,$fp
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L50
	addiu	$17,$17,1
	.set	macro
	.set	reorder

	addiu	$17,$17,-1
	addu	$22,$23,$22
	.set	noreorder
	.set	nomacro
	beq	$22,$0,$L45
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$22,16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L40
	move	$16,$2
	.set	macro
	.set	reorder

	addiu	$19,$19,22
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_network_header
	move	$5,$19
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_transport_header
	move	$5,$19
	.set	macro
	.set	reorder

	li	$5,8			# 0x8
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_pull
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	move	$4,$18
	.set	macro
	.set	reorder

	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_rx_dev
	move	$4,$18
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_protocol
	li	$5,8			# 0x8
	.set	macro
	.set	reorder

	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dev
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$3,128($2)
	move	$4,$18
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	sw	$3,128($2)
	.set	macro
	.set	reorder

	lw	$17,136($2)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$16,$2
	.set	macro
	.set	reorder

	addu	$2,$2,$17
	sw	$2,136($16)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_pppoe_flag
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L45
	sw	$3,56($22)
	.set	macro
	.set	reorder

	.end	check_and_pull_pppoe_hdr
	.size	check_and_pull_pppoe_hdr, .-check_and_pull_pppoe_hdr
	.align	2
	.globl	set_pppoe_info
	.set	nomips16
	.ent	set_pppoe_info
	.type	set_pppoe_info, @function
set_pppoe_info:
	.frame	$sp,72,$31		# vars= 0, regs= 9/0, args= 32, gp= 0
	.mask	0x80ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$23,64($sp)
	sw	$22,60($sp)
	sw	$21,56($sp)
	sw	$20,52($sp)
	sw	$19,48($sp)
	sw	$18,44($sp)
	sw	$17,40($sp)
	sw	$16,36($sp)
	lw	$18,88($sp)
	lw	$16,92($sp)
	lw	$17,96($sp)
	move	$19,$5
	andi	$22,$6,0xffff
	move	$5,$0
	move	$6,$0
	sw	$18,16($sp)
	sw	$16,20($sp)
	sw	$17,24($sp)
	move	$20,$4
	.set	noreorder
	.set	nomacro
	jal	find_pppoe_info
	move	$21,$7
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L66
	move	$23,$2
	.set	macro
	.set	reorder

	beq	$22,$0,$L56
	sh	$22,52($23)
$L56:
	beq	$21,$0,$L57
	sw	$21,44($23)
$L57:
	bne	$18,$0,$L67
$L58:
	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L59
	addiu	$2,$23,16
	.set	macro
	.set	reorder

 #APP
 # 27 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($20)
	addiu	$20,1
	sb	$1,($2)
	bnez	$1,1b
	addiu	$2,1
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
$L59:
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L60
	move	$2,$23
	.set	macro
	.set	reorder

 #APP
 # 27 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($19)
	addiu	$19,1
	sb	$1,($2)
	bnez	$1,1b
	addiu	$2,1
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
$L60:
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L61
	move	$5,$17
	.set	macro
	.set	reorder

	addiu	$4,$23,38
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

$L61:
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L62
	move	$5,$16
	.set	macro
	.set	reorder

	addiu	$4,$23,32
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

$L62:
	li	$2,1			# 0x1
	sh	$2,54($23)
	lw	$31,68($sp)
$L70:
	lw	$23,64($sp)
	lw	$22,60($sp)
	lw	$21,56($sp)
	lw	$20,52($sp)
	lw	$19,48($sp)
	lw	$18,44($sp)
	lw	$17,40($sp)
	lw	$16,36($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,72
	.set	macro
	.set	reorder

$L67:
	.set	noreorder
	.set	nomacro
	j	$L58
	sw	$18,48($23)
	.set	macro
	.set	reorder

$L66:
	lui	$2,%hi(fast_pppoe_info+54)
	addiu	$2,$2,%lo(fast_pppoe_info+54)
	move	$3,$0
	li	$5,16			# 0x10
$L55:
	lhu	$4,0($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L68
	addiu	$2,$2,88
	.set	macro
	.set	reorder

	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	bne	$3,$5,$L55
	lui	$23,%hi(fast_pppoe_info)
	.set	macro
	.set	reorder

	addiu	$4,$23,%lo(fast_pppoe_info)
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,1408			# 0x580
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$22,$0,$L56
	addiu	$23,$23,%lo(fast_pppoe_info)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L56
	sh	$22,52($23)
	.set	macro
	.set	reorder

$L68:
	sll	$2,$3,2
	sll	$23,$3,4
	subu	$23,$23,$2
	subu	$3,$23,$3
	lui	$23,%hi(fast_pppoe_info)
	sll	$3,$3,3
	addiu	$23,$23,%lo(fast_pppoe_info)
	addu	$23,$23,$3
	.set	noreorder
	.set	nomacro
	beq	$23,$0,$L70
	lw	$31,68($sp)
	.set	macro
	.set	reorder

	beq	$22,$0,$L56
	.set	noreorder
	.set	nomacro
	j	$L56
	sh	$22,52($23)
	.set	macro
	.set	reorder

	.end	set_pppoe_info
	.size	set_pppoe_info, .-set_pppoe_info
	.align	2
	.globl	clear_pppoe_info
	.set	nomips16
	.ent	clear_pppoe_info
	.type	clear_pppoe_info, @function
clear_pppoe_info:
	.frame	$sp,56,$31		# vars= 0, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$fp,48($sp)
	sw	$23,44($sp)
	sw	$22,40($sp)
	sw	$21,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lui	$18,%hi(fast_pppoe_info)
	lw	$23,76($sp)
	lw	$22,80($sp)
	addiu	$18,$18,%lo(fast_pppoe_info)
	move	$21,$4
	move	$20,$5
	andi	$fp,$6,0xffff
	move	$17,$18
	move	$16,$0
	li	$19,16			# 0x10
$L77:
	lhu	$2,54($17)
	beq	$2,$0,$L72
	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L73
	sll	$3,$16,4
	.set	macro
	.set	reorder

	sll	$2,$16,2
	subu	$2,$3,$2
	subu	$2,$2,$16
	sll	$2,$2,3
	addiu	$2,$2,16
	addu	$2,$18,$2
	move	$3,$21
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$4,2f
	addiu	$3,1
	bnez	$4,1b
	lbu	$4,($2)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L80
	move	$4,$17
	.set	macro
	.set	reorder

$L73:
	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L74
	sll	$3,$16,4
	.set	macro
	.set	reorder

	sll	$2,$16,2
	subu	$2,$3,$2
	subu	$2,$2,$16
	sll	$2,$2,3
	addu	$2,$18,$2
	move	$3,$20
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$4,2f
	addiu	$3,1
	bnez	$4,1b
	lbu	$4,($2)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L81
	move	$4,$17
	.set	macro
	.set	reorder

$L74:
	.set	noreorder
	.set	nomacro
	beq	$22,$0,$L75
	sll	$2,$16,2
	.set	macro
	.set	reorder

	sll	$4,$16,4
	subu	$4,$4,$2
	subu	$4,$4,$16
	sll	$4,$4,3
	addu	$4,$18,$4
	addiu	$4,$4,38
	move	$5,$22
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L82
	move	$4,$17
	.set	macro
	.set	reorder

$L75:
	.set	noreorder
	.set	nomacro
	beq	$23,$0,$L76
	sll	$2,$16,2
	.set	macro
	.set	reorder

	sll	$4,$16,4
	subu	$4,$4,$2
	subu	$4,$4,$16
	sll	$4,$4,3
	addiu	$4,$4,32
	addu	$4,$18,$4
	move	$5,$23
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L83
	move	$4,$17
	.set	macro
	.set	reorder

$L76:
	lhu	$2,52($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$fp,$L86
	move	$4,$17
	.set	macro
	.set	reorder

$L72:
	addiu	$16,$16,1
$L85:
	.set	noreorder
	.set	nomacro
	bne	$16,$19,$L77
	addiu	$17,$17,88
	.set	macro
	.set	reorder

	lw	$31,52($sp)
	lw	$fp,48($sp)
	lw	$23,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,56
	.set	macro
	.set	reorder

$L80:
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,88			# 0x58
	.set	macro
	.set	reorder

	j	$L73
$L81:
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,88			# 0x58
	.set	macro
	.set	reorder

	j	$L74
$L82:
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,88			# 0x58
	.set	macro
	.set	reorder

	j	$L75
$L83:
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,88			# 0x58
	.set	macro
	.set	reorder

	lhu	$2,52($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$fp,$L72
	move	$4,$17
	.set	macro
	.set	reorder

$L86:
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,88			# 0x58
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L85
	addiu	$16,$16,1
	.set	macro
	.set	reorder

	.end	clear_pppoe_info
	.size	clear_pppoe_info, .-clear_pppoe_info
	.section	.rodata.str1.4
	.align	2
$LC1:
	.ascii	"fast_pppoe\000"
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	fast_pppoe_exit
	.set	nomips16
	.ent	fast_pppoe_exit
	.type	fast_pppoe_exit, @function
fast_pppoe_exit:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(fast_pppoe_proc)
	lw	$2,%lo(fast_pppoe_proc)($16)
	beq	$2,$0,$L88
	lui	$4,%hi($LC1)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC1)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(fast_pppoe_proc)($16)
$L88:
	lw	$31,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	fast_pppoe_exit
	.size	fast_pppoe_exit, .-fast_pppoe_exit
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	fast_pppoe_init
	.set	nomips16
	.ent	fast_pppoe_init
	.type	fast_pppoe_init, @function
fast_pppoe_init:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$4,%hi($LC1)
	lui	$6,%hi(proc_root)
	lui	$7,%hi(fastpath_pppoe_proc_fops)
	addiu	$7,$7,%lo(fastpath_pppoe_proc_fops)
	addiu	$4,$4,%lo($LC1)
	addiu	$6,$6,%lo(proc_root)
	move	$5,$0
	jal	proc_create_data
	sw	$0,16($sp)

	lui	$4,%hi(fast_pppoe_info)
	lui	$3,%hi(fast_pppoe_proc)
	addiu	$4,$4,%lo(fast_pppoe_info)
	move	$5,$0
	li	$6,1408			# 0x580
	jal	memset
	sw	$2,%lo(fast_pppoe_proc)($3)

	lw	$31,28($sp)
	li	$3,1			# 0x1
	lui	$2,%hi(fast_pppoe_fw)
	sw	$3,%lo(fast_pppoe_fw)($2)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	fast_pppoe_init
	.size	fast_pppoe_init, .-fast_pppoe_init
	.text
	.align	2
	.globl	fastpath_pppoe_single_open
	.set	nomips16
	.ent	fastpath_pppoe_single_open
	.type	fastpath_pppoe_single_open, @function
fastpath_pppoe_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	move	$4,$5
	lui	$5,%hi(fast_pppoe_read_proc)
	addiu	$5,$5,%lo(fast_pppoe_read_proc)
	move	$6,$0
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	fastpath_pppoe_single_open
	.size	fastpath_pppoe_single_open, .-fastpath_pppoe_single_open
	.section	.rodata.str1.4
	.align	2
$LC2:
	.ascii	"fast pppoe enable:%c\012\000"
	.align	2
$LC3:
	.ascii	"[%d] null,null,0\012\000"
	.align	2
$LC4:
	.ascii	"[%d] %s,%s,%d,0x%x:%x:%x:%x:%x:%x\012\000"
	.text
	.align	2
	.set	nomips16
	.ent	fast_pppoe_read_proc
	.type	fast_pppoe_read_proc, @function
fast_pppoe_read_proc:
	.frame	$sp,80,$31		# vars= 0, regs= 8/0, args= 48, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$22,72($sp)
	sw	$21,68($sp)
	sw	$20,64($sp)
	sw	$19,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	sw	$16,48($sp)
	lui	$2,%hi(fast_pppoe_fw)
	lw	$6,%lo(fast_pppoe_fw)($2)
	lui	$5,%hi($LC2)
	addiu	$5,$5,%lo($LC2)
	addiu	$6,$6,48
	lui	$17,%hi(fast_pppoe_info+54)
	lui	$18,%hi(fast_pppoe_info)
	lui	$22,%hi($LC4)
	lui	$21,%hi($LC3)
	move	$19,$4
	jal	seq_printf
	addiu	$17,$17,%lo(fast_pppoe_info+54)

	addiu	$18,$18,%lo(fast_pppoe_info)
	addiu	$22,$22,%lo($LC4)
	addiu	$21,$21,%lo($LC3)
	move	$16,$0
	j	$L97
	li	$20,16			# 0x10

$L101:
	jal	seq_printf
	addiu	$16,$16,1

	beq	$16,$20,$L100
	addiu	$17,$17,88

$L97:
	sll	$2,$16,2
$L102:
	sll	$3,$16,4
	subu	$3,$3,$2
	subu	$3,$3,$16
	lhu	$2,0($17)
	sll	$3,$3,3
	addiu	$7,$3,16
	addu	$7,$18,$7
	addu	$3,$18,$3
	move	$4,$19
	move	$5,$21
	beq	$2,$0,$L101
	move	$6,$16

	lhu	$13,-2($17)
	lbu	$12,-16($17)
	lbu	$11,-15($17)
	lbu	$10,-14($17)
	lbu	$9,-13($17)
	lbu	$8,-12($17)
	lbu	$2,-11($17)
	move	$6,$16
	move	$4,$19
	move	$5,$22
	addiu	$16,$16,1
	sw	$3,16($sp)
	sw	$13,20($sp)
	sw	$12,24($sp)
	sw	$11,28($sp)
	sw	$10,32($sp)
	sw	$9,36($sp)
	sw	$8,40($sp)
	sw	$2,44($sp)
	jal	seq_printf
	addiu	$17,$17,88

	bne	$16,$20,$L102
	sll	$2,$16,2

$L100:
	lw	$31,76($sp)
	lw	$22,72($sp)
	lw	$21,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,80

	.set	macro
	.set	reorder
	.end	fast_pppoe_read_proc
	.size	fast_pppoe_read_proc, .-fast_pppoe_read_proc
	.globl	fast_pppoe_fw
	.section	.bss,"aw",@nobits
	.align	2
	.type	fast_pppoe_fw, @object
	.size	fast_pppoe_fw, 4
fast_pppoe_fw:
	.space	4
	.globl	fast_pppoe_proc
	.align	2
	.type	fast_pppoe_proc, @object
	.size	fast_pppoe_proc, 4
fast_pppoe_proc:
	.space	4
	.globl	fastpath_pppoe_proc_fops
	.data
	.align	2
	.type	fastpath_pppoe_proc_fops, @object
	.size	fastpath_pppoe_proc_fops, 108
fastpath_pppoe_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	fastpath_pppoe_single_write
	.space	28
	.word	fastpath_pppoe_single_open
	.space	4
	.word	single_release
	.space	52
	.globl	fast_pppoe_info
	.section	.bss
	.align	2
	.type	fast_pppoe_info, @object
	.size	fast_pppoe_info, 1408
fast_pppoe_info:
	.space	1408
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
