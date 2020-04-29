/*
 * Copyright (C) 2017 Realtek Semiconductor Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

	.file	1 "fast_l2tp_core.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.text
	.align	2
	.globl	get_fast_l2tp_lastxmit
	.set	nomips16
	.ent	get_fast_l2tp_lastxmit
	.type	get_fast_l2tp_lastxmit, @function
get_fast_l2tp_lastxmit:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	lui	$2,%hi(l2tpInfo)
	addiu	$2,$2,%lo(l2tpInfo)
	lhu	$4,38($2)
	lw	$3,8($2)
	move	$2,$0
	xori	$4,$4,0x1
	j	$31
	movz	$2,$3,$4

	.set	macro
	.set	reorder
	.end	get_fast_l2tp_lastxmit
	.size	get_fast_l2tp_lastxmit, .-get_fast_l2tp_lastxmit
	.align	2
	.set	nomips16
	.ent	fastpath_l2tp_single_write
	.type	fastpath_l2tp_single_write, @function
fastpath_l2tp_single_write:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	sltu	$2,$6,2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L12
	move	$16,$6
	.set	macro
	.set	reorder

$L6:
	li	$2,-14			# 0xfffffffffffffff2
$L9:
	lw	$31,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L12:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L9
	li	$2,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	addiu	$2,$5,1
	or	$3,$2,$5
	lw	$2,24($28)
	and	$3,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L6
	addiu	$4,$sp,16
	.set	macro
	.set	reorder

	li	$6,1			# 0x1
 #APP
 # 720 "net/rtl/fastpath/fast_l2tp_core.c" 1
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
	bne	$6,$0,$L6
	lb	$2,16($sp)
	.set	macro
	.set	reorder

	lui	$3,%hi(fast_l2tp_fw)
	addiu	$2,$2,-48
	sw	$2,%lo(fast_l2tp_fw)($3)
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L13
	lui	$3,%hi(l2tp_tx_id_hook)
	.set	macro
	.set	reorder

	lui	$4,%hi(l2tpInfo)
	addiu	$2,$4,%lo(l2tpInfo)
	sw	$0,%lo(l2tp_tx_id_hook)($3)
	sh	$0,20($2)
$L8:
	addiu	$4,$4,%lo(l2tpInfo)
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,40			# 0x28
	.set	macro
	.set	reorder

	lui	$2,%hi(state)
	sw	$0,%lo(state)($2)
	.set	noreorder
	.set	nomacro
	j	$L9
	move	$2,$16
	.set	macro
	.set	reorder

$L13:
	lui	$3,%hi(l2tp_tx_id)
	addiu	$3,$3,%lo(l2tp_tx_id)
	lui	$2,%hi(l2tp_tx_id_hook)
	sw	$3,%lo(l2tp_tx_id_hook)($2)
	.set	noreorder
	.set	nomacro
	j	$L8
	lui	$4,%hi(l2tpInfo)
	.set	macro
	.set	reorder

	.end	fastpath_l2tp_single_write
	.size	fastpath_l2tp_single_write, .-fastpath_l2tp_single_write
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"fast_l2tp\000"
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	fast_l2tp_exit
	.set	nomips16
	.ent	fast_l2tp_exit
	.type	fast_l2tp_exit, @function
fast_l2tp_exit:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(res1)
	lw	$2,%lo(res1)($16)
	beq	$2,$0,$L16
	lui	$4,%hi($LC0)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC0)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(res1)($16)
$L16:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	fast_l2tp_exit
	.size	fast_l2tp_exit, .-fast_l2tp_exit
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	fast_l2tp_init
	.set	nomips16
	.ent	fast_l2tp_init
	.type	fast_l2tp_init, @function
fast_l2tp_init:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$4,%hi($LC0)
	lui	$6,%hi(proc_root)
	lui	$7,%hi(fastpath_l2tp_proc_fops)
	addiu	$7,$7,%lo(fastpath_l2tp_proc_fops)
	addiu	$4,$4,%lo($LC0)
	addiu	$6,$6,%lo(proc_root)
	move	$5,$0
	jal	proc_create_data
	sw	$0,16($sp)

	lui	$3,%hi(res1)
	sw	$2,%lo(res1)($3)
	lui	$2,%hi(l2tp_tx_id_hook)
	sw	$0,%lo(l2tp_tx_id_hook)($2)
	lui	$4,%hi(l2tpInfo)
	lui	$2,%hi(fast_l2tp_fw)
	addiu	$4,$4,%lo(l2tpInfo)
	move	$5,$0
	li	$6,40			# 0x28
	sw	$0,%lo(fast_l2tp_fw)($2)
	lui	$2,%hi(state)
	jal	memset
	sw	$0,%lo(state)($2)

	lw	$31,28($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	fast_l2tp_init
	.size	fast_l2tp_init, .-fast_l2tp_init
	.text
	.align	2
	.globl	fastpath_l2tp_single_open
	.set	nomips16
	.ent	fastpath_l2tp_single_open
	.type	fastpath_l2tp_single_open, @function
fastpath_l2tp_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	move	$4,$5
	lui	$5,%hi(l2tp_read_proc)
	addiu	$5,$5,%lo(l2tp_read_proc)
	move	$6,$0
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	fastpath_l2tp_single_open
	.size	fastpath_l2tp_single_open, .-fastpath_l2tp_single_open
	.section	.rodata.str1.4
	.align	2
$LC1:
	.ascii	"%c\012\000"
	.text
	.align	2
	.set	nomips16
	.ent	l2tp_read_proc
	.type	l2tp_read_proc, @function
l2tp_read_proc:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(fast_l2tp_fw)
	lw	$6,%lo(fast_l2tp_fw)($2)
	lui	$5,%hi($LC1)
	addiu	$5,$5,%lo($LC1)
	jal	seq_printf
	addiu	$6,$6,48

	lw	$31,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	l2tp_read_proc
	.size	l2tp_read_proc, .-l2tp_read_proc
	.align	2
	.globl	check_for_fast_l2tp_to_wan
	.set	nomips16
	.ent	check_for_fast_l2tp_to_wan
	.type	check_for_fast_l2tp_to_wan, @function
check_for_fast_l2tp_to_wan:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	jal	rtl_ip_hdr
	move	$16,$4

	jal	rtl_ip_hdr
	move	$4,$16

	lw	$3,12($2)
	li	$2,16711680			# 0xff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	li	$3,-4259840			# 0xffffffffffbf0000
	ori	$3,$3,0xbff6
	lw	$31,20($sp)
	lw	$16,16($sp)
	addu	$2,$2,$3
	sltu	$2,$0,$2
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	check_for_fast_l2tp_to_wan
	.size	check_for_fast_l2tp_to_wan, .-check_for_fast_l2tp_to_wan
	.align	2
	.globl	fast_l2tp_to_wan
	.set	nomips16
	.ent	fast_l2tp_to_wan
	.type	fast_l2tp_to_wan, @function
fast_l2tp_to_wan:
	.frame	$sp,96,$31		# vars= 48, regs= 7/0, args= 16, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$21,88($sp)
	sw	$20,84($sp)
	sw	$19,80($sp)
	sw	$18,76($sp)
	sw	$17,72($sp)
	sw	$16,68($sp)
	lui	$2,%hi(fast_l2tp_fw)
	lw	$2,%lo(fast_l2tp_fw)($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L26
	move	$20,$4
	.set	macro
	.set	reorder

	lui	$17,%hi(l2tpInfo)
	addiu	$16,$17,%lo(l2tpInfo)
	lhu	$2,20($16)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L33
	move	$2,$0
	.set	macro
	.set	reorder

	lhu	$2,22($16)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L26
	lw	$2,%lo(l2tpInfo)($17)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L26
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	lhu	$3,38($16)
	beq	$3,$2,$L36
$L26:
	move	$2,$0
$L33:
	lw	$31,92($sp)
	lw	$21,88($sp)
	lw	$20,84($sp)
	lw	$19,80($sp)
	lw	$18,76($sp)
	lw	$17,72($sp)
	lw	$16,68($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,96
	.set	macro
	.set	reorder

$L36:
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$18,$20
	.set	macro
	.set	reorder

	move	$4,$20
	lhu	$19,2($2)
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_headroom
	move	$21,$2
	.set	macro
	.set	reorder

	sltu	$2,$2,52
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$19, $19

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L27
	andi	$19,$19,0xffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_cloned
	move	$4,$20
	.set	macro
	.set	reorder

	beq	$2,$0,$L37
$L27:
	move	$4,$20
$L39:
	.set	noreorder
	.set	nomacro
	jal	skb_realloc_headroom
	li	$5,52			# 0x34
	.set	macro
	.set	reorder

	beq	$2,$0,$L26
	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	consume_skb
	sw	$2,56($sp)
	.set	macro
	.set	reorder

	lw	$2,56($sp)
	move	$18,$2
	li	$5,52			# 0x34
$L41:
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	skb_push
	lbu	$20,1($21)
	.set	macro
	.set	reorder

	lui	$5,%hi(l2tpInfo+24)
	move	$4,$2
	addiu	$5,$5,%lo(l2tpInfo+24)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,14			# 0xe
	.set	macro
	.set	reorder

	lw	$3,36($sp)
	li	$2,4			# 0x4
	lw	$7,12($16)
	lw	$6,16($16)
	ins	$3,$2,4,4
	li	$2,5			# 0x5
	ins	$3,$2,0,4
	move	$5,$0
	li	$2,64			# 0x40
	sw	$3,36($sp)
	move	$4,$18
	li	$3,17			# 0x11
	sb	$3,45($sp)
	sw	$7,52($sp)
	sw	$6,48($sp)
	sb	$2,44($sp)
	sh	$2,42($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ip_summed
	sb	$20,37($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$4,$18
	.set	macro
	.set	reorder

	addiu	$2,$2,-14
	addiu	$5,$sp,36
	sh	$0,40($sp)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$0,46($sp)
	sh	$2,38($sp)
	lw	$6,36($sp)
	lw	$7,4($5)
	lw	$3,8($5)
	lw	$4,48($sp)
	addu	$2,$7,$6
	sltu	$7,$2,$7
	addu	$2,$2,$3
	addu	$7,$2,$7
	sltu	$3,$7,$3
	addu	$7,$7,$4
	addu	$3,$7,$3
	andi	$6,$6,0xf
	sltu	$2,$3,$4
	sll	$6,$6,2
	addu	$2,$2,$3
	addu	$6,$5,$6
	addiu	$3,$sp,52
$L29:
	lw	$4,0($3)
	addiu	$3,$3,4
	addu	$2,$2,$4
	sltu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$6,$3,$L29
	addu	$2,$4,$2
	.set	macro
	.set	reorder

	move	$4,$18
 #APP
 # 78 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat
	sll	$1, $2, 16
	addu	$2, $1
	sltu	$1, $2, $1
	srl	$2, $2, 16
	addu	$2, $1
	xori	$2, 0xffff
	.set	pop
 # 0 "" 2
 #NO_APP
	sw	$5,56($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$2,46($sp)
	.set	macro
	.set	reorder

	lw	$5,56($sp)
	addiu	$4,$2,14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,20			# 0x14
	.set	macro
	.set	reorder

	lhu	$3,22($16)
	lhu	$5,20($16)
	li	$2,-23290			# 0xffffffffffffa506
	sh	$3,28($sp)
	li	$3,1023			# 0x3ff
	li	$6,512			# 0x200
	move	$4,$18
	sh	$3,30($sp)
	li	$3,8448			# 0x2100
	sh	$3,32($sp)
	sh	$6,24($sp)
	sh	$5,26($sp)
	sh	$2,18($sp)
	sh	$2,16($sp)
	addiu	$19,$19,18
	sh	$0,22($sp)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$19, $19

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$19,20($sp)
	.set	macro
	.set	reorder

	addiu	$4,$2,34
	li	$6,18			# 0x12
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	lw	$5,%lo(l2tpInfo)($17)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dev
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	timeoutCheck_skipp_pkt
	addiu	$4,$2,52
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L30
	lui	$2,%hi(jiffies)
	.set	macro
	.set	reorder

	lw	$2,%lo(jiffies)($2)
	sw	$2,8($16)
$L30:
	lw	$4,4($16)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L40
	lui	$2,%hi(gQosEnabled)
	.set	macro
	.set	reorder

	jal	rtl_get_ppp_dev_priv
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L40
	lui	$2,%hi(gQosEnabled)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_ppp_dev_priv
	lw	$4,4($16)
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$16,$2
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_inc_ppp_stats
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(gQosEnabled)
$L40:
	lw	$2,%lo(gQosEnabled)($2)
	bne	$2,$0,$L38
	.set	noreorder
	.set	nomacro
	jal	rtl_call_skb_ndo_start_xmit
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L33
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L38:
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L33
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L37:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_shared
	move	$4,$20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L41
	li	$5,52			# 0x34
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L39
	move	$4,$20
	.set	macro
	.set	reorder

	.end	fast_l2tp_to_wan
	.size	fast_l2tp_to_wan, .-fast_l2tp_to_wan
	.align	2
	.globl	set_l2tp_device
	.set	nomips16
	.ent	set_l2tp_device
	.type	set_l2tp_device, @function
set_l2tp_device:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(fast_l2tp_fw)
	lw	$2,%lo(fast_l2tp_fw)($2)
	beq	$2,$0,$L45
	lw	$31,20($sp)

	jal	rtl_get_dev_by_name
	nop

	lui	$3,%hi(l2tpInfo+4)
	sw	$2,%lo(l2tpInfo+4)($3)
	lw	$31,20($sp)
$L45:
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	set_l2tp_device
	.size	set_l2tp_device, .-set_l2tp_device
	.section	.rodata.str1.4
	.align	2
$LC2:
	.ascii	"ppp0\000"
	.align	2
$LC3:
	.ascii	"eth1\000"
	.align	2
$LC4:
	.ascii	"wlan\000"
	.text
	.align	2
	.globl	l2tp_tx_id
	.set	nomips16
	.ent	l2tp_tx_id
	.type	l2tp_tx_id, @function
l2tp_tx_id:
	.frame	$sp,40,$31		# vars= 0, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$16,$4
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$17,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L66
	li	$4,42246			# 0xa506
	.set	macro
	.set	reorder

	lw	$18,0($2)
	andi	$18,$18,0xf
	sll	$18,$18,2
	addu	$18,$2,$18
	lhu	$3,0($18)
	beq	$3,$4,$L69
$L66:
	lw	$31,36($sp)
$L76:
$L78:
	lw	$20,32($sp)
$L77:
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,40
	.set	macro
	.set	reorder

$L69:
	lhu	$4,2($18)
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L76
	lw	$31,36($sp)
	.set	macro
	.set	reorder

	lhu	$3,8($18)
	andi	$4,$3,0x80
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L70
	addiu	$5,$18,8
	.set	macro
	.set	reorder

	lui	$20,%hi(l2tpInfo)
	addiu	$19,$20,%lo(l2tpInfo)
	lw	$4,12($19)
	addiu	$2,$18,10
	andi	$3,$3,0x40
	addiu	$18,$18,12
	movz	$18,$2,$3
	.set	noreorder
	.set	nomacro
	jal	filter_checkConnect
	addiu	$17,$18,6
	.set	macro
	.set	reorder

	beq	$2,$0,$L56
	.set	noreorder
	.set	nomacro
	jal	filter_addconnect
	lw	$4,12($19)
	.set	macro
	.set	reorder

$L56:
	lhu	$3,0($17)
	li	$2,8640			# 0x21c0
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L76
	lw	$31,36($sp)
	.set	macro
	.set	reorder

	lbu	$2,2($17)
	li	$3,5			# 0x5
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L71
	addiu	$16,$17,2
	.set	macro
	.set	reorder

$L57:
	li	$3,6			# 0x6
	beq	$2,$3,$L72
$L58:
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L76
	lw	$31,36($sp)
	.set	macro
	.set	reorder

	lhu	$5,2($16)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $5

 # 0 "" 2
 #NO_APP
	andi	$5,$5,0xffff
	slt	$2,$5,5
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L76
	li	$4,5			# 0x5
	.set	macro
	.set	reorder

	lbu	$6,4($16)
	lbu	$3,5($16)
	li	$2,4			# 0x4
	.set	noreorder
	.set	nomacro
	beq	$6,$4,$L60
	li	$7,6			# 0x6
	.set	macro
	.set	reorder

	li	$6,5			# 0x5
$L67:
	addu	$2,$2,$3
	slt	$4,$2,$5
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L66
	addu	$3,$16,$2
	.set	macro
	.set	reorder

	lbu	$4,0($3)
	.set	noreorder
	.set	nomacro
	bne	$4,$6,$L67
	lbu	$3,1($3)
	.set	macro
	.set	reorder

	addiu	$7,$2,2
$L60:
	addu	$7,$16,$7
	lw	$3,0($7)
	lui	$2,%hi(l2tp_ppp_imagic)
	.set	noreorder
	.set	nomacro
	j	$L66
	sw	$3,%lo(l2tp_ppp_imagic)($2)
	.set	macro
	.set	reorder

$L70:
	andi	$3,$3,0x40
	bne	$3,$0,$L49
	lhu	$5,2($5)
	addiu	$18,$18,10
$L50:
	lhu	$3,14($18)
	li	$4,1024			# 0x400
	.set	noreorder
	.set	nomacro
	beq	$3,$4,$L73
	li	$4,2816			# 0xb00
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$4,$L74
	li	$4,3072			# 0xc00
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$3,$4,$L76
	lw	$31,36($sp)
	.set	macro
	.set	reorder

	lhu	$3,2($18)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L78
	addiu	$4,$18,2
	.set	macro
	.set	reorder

	lui	$16,%hi(state)
	lw	$3,%lo(state)($16)
	sltu	$3,$3,2
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L77
	lw	$20,32($sp)
	.set	macro
	.set	reorder

	lhu	$3,0($18)
	lui	$17,%hi(l2tpInfo)
	addiu	$17,$17,%lo(l2tpInfo)
	sh	$3,20($17)
	lhu	$3,0($4)
	sh	$3,22($17)
	lw	$3,12($2)
	sw	$3,16($17)
	lw	$2,16($2)
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	filter_addconnect
	sw	$2,12($17)
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	lui	$4,%hi($LC2)
	addiu	$4,$4,%lo($LC2)
	li	$5,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	rtl865x_setNetifType
	sh	$2,38($17)
	.set	macro
	.set	reorder

	li	$2,2			# 0x2
	.set	noreorder
	.set	nomacro
	j	$L66
	sw	$2,%lo(state)($16)
	.set	macro
	.set	reorder

$L49:
	lhu	$5,4($5)
	.set	noreorder
	.set	nomacro
	j	$L50
	addiu	$18,$18,12
	.set	macro
	.set	reorder

$L72:
	lhu	$3,20($19)
	lhu	$2,0($18)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L76
	lw	$31,36($sp)
	.set	macro
	.set	reorder

	lhu	$2,2($18)
	lhu	$3,22($19)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L76
	move	$4,$19
	.set	macro
	.set	reorder

	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,40			# 0x28
	.set	macro
	.set	reorder

	lui	$2,%hi(state)
	sw	$0,%lo(state)($2)
	.set	noreorder
	.set	nomacro
	j	$L58
	lbu	$2,2($17)
	.set	macro
	.set	reorder

$L74:
	lui	$18,%hi(state)
	lw	$2,%lo(state)($18)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L76
	lw	$31,36($sp)
	.set	macro
	.set	reorder

	lui	$19,%hi(rtl865x_curOpMode)
	lw	$2,%lo(rtl865x_curOpMode)($19)
	beq	$2,$0,$L75
$L53:
	li	$3,2			# 0x2
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L76
	lw	$31,36($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev_name
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$5,%hi($LC4)
	move	$4,$2
	addiu	$5,$5,%lo($LC4)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L76
	lw	$31,36($sp)
	.set	macro
	.set	reorder

$L54:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$4,%hi(l2tpInfo+24)
	lui	$3,%hi(l2tpInfo)
	addiu	$4,$4,%lo(l2tpInfo+24)
	addiu	$5,$17,-8
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,%lo(l2tpInfo)($3)
	.set	macro
	.set	reorder

	lui	$4,%hi(l2tpInfo+30)
	addiu	$4,$4,%lo(l2tpInfo+30)
	addiu	$5,$17,-14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lui	$4,%hi(l2tpInfo+36)
	addiu	$4,$4,%lo(l2tpInfo+36)
	addiu	$5,$17,-2
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,2			# 0x2
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L66
	sw	$2,%lo(state)($18)
	.set	macro
	.set	reorder

$L73:
	lui	$4,%hi(l2tpInfo)
	addiu	$4,$4,%lo(l2tpInfo)
	lhu	$2,20($4)
	.set	noreorder
	.set	nomacro
	bne	$2,$5,$L76
	lw	$31,36($sp)
	.set	macro
	.set	reorder

	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,40			# 0x28
	.set	macro
	.set	reorder

	lui	$4,%hi($LC2)
	addiu	$4,$4,%lo($LC2)
	.set	noreorder
	.set	nomacro
	jal	rtl865x_setNetifType
	li	$5,2			# 0x2
	.set	macro
	.set	reorder

	lui	$2,%hi(state)
	.set	noreorder
	.set	nomacro
	j	$L66
	sw	$0,%lo(state)($2)
	.set	macro
	.set	reorder

$L71:
	addiu	$4,$20,%lo(l2tpInfo)
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,40			# 0x28
	.set	macro
	.set	reorder

	lui	$2,%hi(state)
	sw	$0,%lo(state)($2)
	.set	noreorder
	.set	nomacro
	j	$L57
	lbu	$2,2($17)
	.set	macro
	.set	reorder

$L75:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev_name
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$5,%hi($LC3)
	move	$4,$2
	addiu	$5,$5,%lo($LC3)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	beq	$2,$0,$L54
	.set	noreorder
	.set	nomacro
	j	$L53
	lw	$2,%lo(rtl865x_curOpMode)($19)
	.set	macro
	.set	reorder

	.end	l2tp_tx_id
	.size	l2tp_tx_id, .-l2tp_tx_id
	.align	2
	.globl	fast_l2tp_rx
	.set	nomips16
	.ent	fast_l2tp_rx
	.type	fast_l2tp_rx, @function
fast_l2tp_rx:
	.frame	$sp,40,$31		# vars= 0, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	jal	rtl_ip_hdr
	move	$16,$4

	move	$4,$16
	lw	$19,0($2)
	jal	rtl_get_skb_data
	move	$17,$2

	move	$4,$16
	jal	rtl_get_skb_len
	move	$18,$2

	sltu	$2,$2,40
	bne	$2,$0,$L80
	andi	$19,$19,0xf

	sll	$19,$19,2
	addu	$19,$17,$19
	lhu	$2,0($19)
	li	$3,42246			# 0xa506
	beq	$2,$3,$L90
	nop

$L80:
	move	$3,$0
$L86:
	lw	$31,36($sp)
$L94:
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,40

$L90:
	lhu	$3,2($19)
	bne	$3,$2,$L80
	lui	$20,%hi(l2tpInfo)

	lw	$2,%lo(l2tpInfo)($20)
	beq	$2,$0,$L91
	nop

	lbu	$2,28($18)
$L93:
	li	$3,2			# 0x2
	move	$4,$18
	andi	$19,$2,0x40
	movz	$3,$0,$19
	andi	$2,$2,0x2
	addu	$19,$3,$2
	move	$5,$19
	jal	fast_l2tp_tx_lcp_echo_reply
	move	$6,$16

	li	$4,1			# 0x1
	beq	$2,$4,$L86
	li	$3,1			# 0x1

	addiu	$20,$20,%lo(l2tpInfo)
	lhu	$2,20($20)
	beq	$2,$0,$L80
	nop

	lhu	$2,22($20)
	beq	$2,$0,$L80
	addu	$18,$18,$19

	lbu	$2,36($18)
	bne	$2,$0,$L80
	li	$2,33			# 0x21

	lbu	$3,37($18)
	bne	$3,$2,$L86
	move	$3,$0

	lui	$2,%hi(l2tpInfo)
	lw	$2,%lo(l2tpInfo)($2)
	beq	$2,$0,$L94
	lw	$31,36($sp)

	lw	$2,16($17)
	lw	$3,16($20)
	bne	$3,$2,$L94
	move	$3,$0

	lw	$5,4($20)
	beq	$5,$0,$L92
	lui	$4,%hi($LC2)

$L87:
	lbu	$2,47($18)
	li	$3,6			# 0x6
	beq	$2,$3,$L88
	li	$3,17			# 0x11

	beq	$2,$3,$L88
	li	$3,1			# 0x1

	beq	$2,$3,$L88
	li	$3,47			# 0x2f

	bne	$2,$3,$L86
	move	$3,$0

$L88:
	jal	rtl_set_skb_dev
	move	$4,$16

	addiu	$5,$19,38
	jal	skb_pull
	move	$4,$16

	jal	rtl_skb_reset_network_header
	move	$4,$16

	jal	rtl_skb_reset_transport_header
	move	$4,$16

	lw	$4,4($20)
	beq	$4,$0,$L86
	move	$3,$0

	jal	rtl_get_ppp_dev_priv
	nop

	beq	$2,$0,$L80
	nop

	jal	rtl_get_ppp_dev_priv
	lw	$4,4($20)

	move	$4,$16
	jal	rtl_get_skb_len
	move	$17,$2

	move	$6,$2
	move	$4,$17
	jal	rtl_inc_ppp_stats
	move	$5,$0

	j	$L86
	move	$3,$0

$L91:
	jal	l2tp_tx_id
	move	$4,$16

	j	$L93
	lbu	$2,28($18)

$L92:
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC2)

	beq	$2,$0,$L80
	sw	$2,4($20)

	j	$L87
	move	$5,$2

	.set	macro
	.set	reorder
	.end	fast_l2tp_rx
	.size	fast_l2tp_rx, .-fast_l2tp_rx
	.align	2
	.globl	event_ppp_dev_down
	.set	nomips16
	.ent	event_ppp_dev_down
	.type	event_ppp_dev_down, @function
event_ppp_dev_down:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$16,%hi(l2tpInfo)
	addiu	$16,$16,%lo(l2tpInfo)
	lhu	$2,38($16)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L97
	move	$17,$4
	.set	macro
	.set	reorder

	lw	$4,4($16)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L99
	lw	$31,28($sp)
	.set	macro
	.set	reorder

	jal	rtl_get_ppp_dev_name
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$3,($2)
1:	lbu	$1,($17)
	addiu	$2,1
	bne	$1,$3,2f
	addiu	$17,1
	bnez	$3,1b
	lbu	$3,($2)
	move	$3,$1
2:	subu	$3,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L98
	move	$4,$16
	.set	macro
	.set	reorder

$L97:
	lw	$31,28($sp)
$L99:
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L98:
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,40			# 0x28
	.set	macro
	.set	reorder

	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	lui	$2,%hi(state)
	sw	$0,%lo(state)($2)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

	.end	event_ppp_dev_down
	.size	event_ppp_dev_down, .-event_ppp_dev_down
	.align	2
	.globl	is_l2tp_device
	.set	nomips16
	.ent	is_l2tp_device
	.type	is_l2tp_device, @function
is_l2tp_device:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$2,%hi(l2tpInfo+4)
	lw	$3,%lo(l2tpInfo+4)($2)
	move	$16,$4
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L102
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_ppp_dev_name
	move	$4,$3
	.set	macro
	.set	reorder

 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$3,($2)
1:	lbu	$1,($16)
	addiu	$2,1
	bne	$1,$3,2f
	addiu	$16,1
	bnez	$3,1b
	lbu	$3,($2)
	move	$3,$1
2:	subu	$3,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	sltu	$2,$3,1
$L102:
	lw	$31,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	is_l2tp_device
	.size	is_l2tp_device, .-is_l2tp_device
	.globl	l2tpInfo
	.section	.bss,"aw",@nobits
	.align	2
	.type	l2tpInfo, @object
	.size	l2tpInfo, 40
l2tpInfo:
	.space	40
	.globl	l2tp_ppp_imagic
	.align	2
	.type	l2tp_ppp_imagic, @object
	.size	l2tp_ppp_imagic, 4
l2tp_ppp_imagic:
	.space	4
	.globl	fastpath_l2tp_proc_fops
	.data
	.align	2
	.type	fastpath_l2tp_proc_fops, @object
	.size	fastpath_l2tp_proc_fops, 108
fastpath_l2tp_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	fastpath_l2tp_single_write
	.space	28
	.word	fastpath_l2tp_single_open
	.space	4
	.word	single_release
	.space	52
	.local	res1
	.comm	res1,4,4
	.globl	l2tp_tx_id_hook
	.section	.bss
	.align	2
	.type	l2tp_tx_id_hook, @object
	.size	l2tp_tx_id_hook, 4
l2tp_tx_id_hook:
	.space	4
	.globl	fast_l2tp_fw
	.align	2
	.type	fast_l2tp_fw, @object
	.size	fast_l2tp_fw, 4
fast_l2tp_fw:
	.space	4
	.globl	state
	.align	2
	.type	state, @object
	.size	state, 4
state:
	.space	4
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
