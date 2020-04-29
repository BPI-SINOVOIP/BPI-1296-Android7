/*
 * Copyright (C) 2017 Realtek Semiconductor Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

	.file	1 "fast_pptp_core.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.text
	.align	2
	.globl	get_fastpptp_lastxmit
	.set	nomips16
	.ent	get_fastpptp_lastxmit
	.type	get_fastpptp_lastxmit, @function
get_fastpptp_lastxmit:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	lui	$2,%hi(fast_pptp_fw)
	lw	$2,%lo(fast_pptp_fw)($2)
	beq	$2,$0,$L2
	nop

	lui	$2,%hi(pptpAccInfo)
	addiu	$3,$2,%lo(pptpAccInfo)
	lw	$4,4($3)
	li	$3,1			# 0x1
	beq	$4,$3,$L6
	nop

$L2:
	j	$31
	move	$2,$0

$L6:
	j	$31
	lw	$2,%lo(pptpAccInfo)($2)

	.set	macro
	.set	reorder
	.end	get_fastpptp_lastxmit
	.size	get_fastpptp_lastxmit, .-get_fastpptp_lastxmit
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"fast_pptp\000"
	.align	2
$LC1:
	.ascii	"pptp_conn_ck\000"
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	fast_pptp_exit
	.set	nomips16
	.ent	fast_pptp_exit
	.type	fast_pptp_exit, @function
fast_pptp_exit:
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
	beq	$2,$0,$L8
	lui	$4,%hi($LC0)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC0)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(res1)($16)
$L8:
	lui	$16,%hi(res_check_pptp)
	lw	$2,%lo(res_check_pptp)($16)
	beq	$2,$0,$L10
	lui	$4,%hi($LC1)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC1)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(res_check_pptp)($16)
$L10:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	fast_pptp_exit
	.size	fast_pptp_exit, .-fast_pptp_exit
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	fast_pptp_init
	.set	nomips16
	.ent	fast_pptp_init
	.type	fast_pptp_init, @function
fast_pptp_init:
	.frame	$sp,32,$31		# vars= 0, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	lui	$16,%hi(proc_root)
	lui	$4,%hi($LC0)
	lui	$7,%hi(fastpath_pptp_proc_fops)
	addiu	$6,$16,%lo(proc_root)
	addiu	$4,$4,%lo($LC0)
	addiu	$7,$7,%lo(fastpath_pptp_proc_fops)
	move	$5,$0
	jal	proc_create_data
	sw	$0,16($sp)

	lui	$4,%hi($LC1)
	lui	$7,%hi(pptp_conn_proc_fops)
	addiu	$6,$16,%lo(proc_root)
	addiu	$7,$7,%lo(pptp_conn_proc_fops)
	lui	$3,%hi(res1)
	addiu	$4,$4,%lo($LC1)
	move	$5,$0
	sw	$2,%lo(res1)($3)
	jal	proc_create_data
	sw	$0,16($sp)

	lui	$4,%hi(pptpAccInfo)
	lui	$3,%hi(res_check_pptp)
	addiu	$4,$4,%lo(pptpAccInfo)
	move	$5,$0
	li	$6,64			# 0x40
	jal	memset
	sw	$2,%lo(res_check_pptp)($3)

	lw	$31,28($sp)
	lw	$16,24($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	fast_pptp_init
	.size	fast_pptp_init, .-fast_pptp_init
	.text
	.align	2
	.globl	pptp_conn_single_open
	.set	nomips16
	.ent	pptp_conn_single_open
	.type	pptp_conn_single_open, @function
pptp_conn_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	move	$4,$5
	lui	$5,%hi(pptpconn_read_proc)
	addiu	$5,$5,%lo(pptpconn_read_proc)
	move	$6,$0
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	pptp_conn_single_open
	.size	pptp_conn_single_open, .-pptp_conn_single_open
	.align	2
	.globl	fastpath_pptp_single_open
	.set	nomips16
	.ent	fastpath_pptp_single_open
	.type	fastpath_pptp_single_open, @function
fastpath_pptp_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	move	$4,$5
	lui	$5,%hi(ppfw_read_proc)
	addiu	$5,$5,%lo(ppfw_read_proc)
	move	$6,$0
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	fastpath_pptp_single_open
	.size	fastpath_pptp_single_open, .-fastpath_pptp_single_open
	.section	.rodata.str1.4
	.align	2
$LC2:
	.ascii	"%d\012\000"
	.text
	.align	2
	.globl	pptpconn_read_proc
	.set	nomips16
	.ent	pptpconn_read_proc
	.type	pptpconn_read_proc, @function
pptpconn_read_proc:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(pptp_conn_check)
	lw	$6,%lo(pptp_conn_check)($2)
	lui	$5,%hi($LC2)
	jal	seq_printf
	addiu	$5,$5,%lo($LC2)

	lw	$31,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	pptpconn_read_proc
	.size	pptpconn_read_proc, .-pptpconn_read_proc
	.align	2
	.globl	ppfw_read_proc
	.set	nomips16
	.ent	ppfw_read_proc
	.type	ppfw_read_proc, @function
ppfw_read_proc:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(fast_pptp_fw)
	lw	$6,%lo(fast_pptp_fw)($2)
	lui	$5,%hi($LC2)
	jal	seq_printf
	addiu	$5,$5,%lo($LC2)

	lw	$31,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	ppfw_read_proc
	.size	ppfw_read_proc, .-ppfw_read_proc
	.align	2
	.globl	pptpconn_write_proc
	.set	nomips16
	.ent	pptpconn_write_proc
	.type	pptpconn_write_proc, @function
pptpconn_write_proc:
	.frame	$sp,208,$31		# vars= 200, regs= 1/0, args= 0, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-208
	sw	$31,204($sp)
	sltu	$4,$6,2
	bne	$4,$0,$L22
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L22
	or	$2,$5,$6
	.set	macro
	.set	reorder

	addu	$4,$5,$6
	or	$2,$2,$4
	lw	$4,24($28)
	and	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L28
	lw	$31,204($sp)
	.set	macro
	.set	reorder

	move	$4,$sp
 #APP
 # 928 "net/rtl/fastpath/fast_pptp_core.c" 1
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
	bne	$6,$0,$L22
	lb	$2,0($sp)
	.set	macro
	.set	reorder

	addiu	$4,$2,-48
	andi	$4,$4,0x00ff
	sltu	$4,$4,10
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L24
	move	$3,$0
	.set	macro
	.set	reorder

	move	$4,$sp
$L25:
	sll	$5,$3,3
	sll	$3,$3,1
	addu	$3,$3,$5
	addiu	$4,$4,1
	addu	$3,$3,$2
	lb	$2,0($4)
	addiu	$5,$2,-48
	andi	$5,$5,0x00ff
	sltu	$5,$5,10
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L25
	addiu	$3,$3,-48
	.set	macro
	.set	reorder

$L24:
	lui	$2,%hi(pptp_conn_check)
	sw	$3,%lo(pptp_conn_check)($2)
$L22:
	lw	$31,204($sp)
$L28:
	li	$2,-14			# 0xfffffffffffffff2
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,208
	.set	macro
	.set	reorder

	.end	pptpconn_write_proc
	.size	pptpconn_write_proc, .-pptpconn_write_proc
	.align	2
	.set	nomips16
	.ent	pptp_conn_single_write
	.type	pptp_conn_single_write, @function
pptp_conn_single_write:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	j	pptpconn_write_proc
	nop

	.set	macro
	.set	reorder
	.end	pptp_conn_single_write
	.size	pptp_conn_single_write, .-pptp_conn_single_write
	.align	2
	.globl	ppfw_write_proc
	.set	nomips16
	.ent	ppfw_write_proc
	.type	ppfw_write_proc, @function
ppfw_write_proc:
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	sltu	$2,$6,2
	move	$17,$6
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L32
	move	$18,$5
	.set	macro
	.set	reorder

	addiu	$4,$6,32
	.set	noreorder
	.set	nomacro
	jal	__kmalloc
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L32
	move	$16,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L33
	addu	$2,$18,$17
	.set	macro
	.set	reorder

	or	$3,$18,$17
	or	$3,$3,$2
	lw	$2,24($28)
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L33
	move	$5,$18
	.set	macro
	.set	reorder

	move	$4,$16
	move	$6,$17
 #APP
 # 954 "net/rtl/fastpath/fast_pptp_core.c" 1
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	bne	$6,$0,$L33
	lb	$3,0($16)
	addiu	$4,$3,-48
	andi	$4,$4,0x00ff
	sltu	$4,$4,10
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L35
	move	$4,$16
	.set	macro
	.set	reorder

$L36:
	sll	$5,$2,3
	sll	$2,$2,1
	addu	$2,$2,$5
	addiu	$4,$4,1
	addu	$2,$2,$3
	lb	$3,0($4)
	addiu	$5,$3,-48
	andi	$5,$5,0x00ff
	sltu	$5,$5,10
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L36
	addiu	$2,$2,-48
	.set	macro
	.set	reorder

$L35:
	lui	$4,%hi(pptpAccInfo)
	lui	$18,%hi(fast_pptp_fw)
	addiu	$4,$4,%lo(pptpAccInfo)
	move	$5,$0
	li	$6,64			# 0x40
	.set	noreorder
	.set	nomacro
	jal	memset
	sw	$2,%lo(fast_pptp_fw)($18)
	.set	macro
	.set	reorder

	lw	$2,%lo(fast_pptp_fw)($18)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L42
	lui	$3,%hi(sync_tx_pptp_gre_seqno)
	.set	macro
	.set	reorder

	lui	$2,%hi(sync_tx_pptp_gre_seqno_hook)
	sw	$0,%lo(sync_tx_pptp_gre_seqno_hook)($2)
$L38:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$16
	.set	macro
	.set	reorder

$L39:
	move	$2,$17
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L42:
	addiu	$3,$3,%lo(sync_tx_pptp_gre_seqno)
	lui	$2,%hi(sync_tx_pptp_gre_seqno_hook)
	.set	noreorder
	.set	nomacro
	j	$L38
	sw	$3,%lo(sync_tx_pptp_gre_seqno_hook)($2)
	.set	macro
	.set	reorder

$L32:
	.set	noreorder
	.set	nomacro
	j	$L39
	li	$17,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

$L33:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L39
	li	$17,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	.end	ppfw_write_proc
	.size	ppfw_write_proc, .-ppfw_write_proc
	.align	2
	.set	nomips16
	.ent	fastpath_pptp_single_write
	.type	fastpath_pptp_single_write, @function
fastpath_pptp_single_write:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	j	ppfw_write_proc
	nop

	.set	macro
	.set	reorder
	.end	fastpath_pptp_single_write
	.size	fastpath_pptp_single_write, .-fastpath_pptp_single_write
	.align	2
	.globl	sync_tx_pptp_gre_seqno
	.set	nomips16
	.ent	sync_tx_pptp_gre_seqno
	.type	sync_tx_pptp_gre_seqno, @function
sync_tx_pptp_gre_seqno:
	.frame	$sp,48,$31		# vars= 8, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$19,40($sp)
	sw	$18,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$19,$4
	.set	macro
	.set	reorder

	move	$17,$2
	lui	$2,%hi(fast_pptp_fw)
	lw	$2,%lo(fast_pptp_fw)($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L54
	addiu	$18,$17,14
	.set	macro
	.set	reorder

 #APP
 # 39 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push
	.set	reorder
	.set	noat
	di	$16
	andi	$16, 1
	sll $0, $0, 3
	.set	pop

 # 0 "" 2
 #NO_APP
	lbu	$3,9($18)
	li	$2,47			# 0x2f
	beq	$3,$2,$L56
$L47:
 #APP
 # 58 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push
	.set	noreorder
	.set	noat
	beqz	$16, 1f
	di
	ei
1:
	sll $0, $0, 3
	.set	pop

 # 0 "" 2
 #NO_APP
$L54:
	lw	$31,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,48
	.set	macro
	.set	reorder

$L56:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$4,$19
	.set	macro
	.set	reorder

	sltu	$2,$2,35
	bne	$2,$0,$L47
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$19
	.set	macro
	.set	reorder

	lw	$3,14($17)
	li	$4,1			# 0x1
	andi	$3,$3,0xf
	sll	$3,$3,2
	addiu	$3,$3,14
	addu	$2,$2,$3
	lbu	$3,2($2)
	sb	$3,16($sp)
	lbu	$3,3($2)
	sb	$3,17($sp)
	lbu	$3,1($2)
	andi	$5,$3,0x7
	.set	noreorder
	.set	nomacro
	bne	$5,$4,$L47
	lhu	$5,16($sp)
	.set	macro
	.set	reorder

	li	$4,34827			# 0x880b
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $5

 # 0 "" 2
 #NO_APP
	andi	$5,$5,0xffff
	bne	$5,$4,$L47
	lbu	$5,0($2)
	andi	$5,$5,0x10
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L49
	move	$4,$0
	.set	macro
	.set	reorder

	lui	$4,%hi(pptpAccInfo)
	addiu	$4,$4,%lo(pptpAccInfo)
	lhu	$6,4($18)
	lhu	$5,46($4)
	beq	$6,$5,$L50
	lhu	$6,6($2)
	lhu	$5,18($4)
	beq	$6,$5,$L57
$L50:
	li	$4,1			# 0x1
$L49:
	seb	$3,$3
	.set	noreorder
	.set	nomacro
	bltz	$3,$L58
	lui	$3,%hi(pptpAccInfo)
	.set	macro
	.set	reorder

$L51:
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L47
	lui	$4,%hi(pptpAccInfo)
	.set	macro
	.set	reorder

	addiu	$4,$4,%lo(pptpAccInfo)
	lhu	$3,44($4)
	addiu	$2,$18,16
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,44($4)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3

 # 0 "" 2
 #NO_APP
	sh	$0,10($18)
	sh	$3,4($18)
	lw	$5,14($17)
	lw	$7,4($18)
	lw	$4,8($18)
	lw	$6,12($18)
	addu	$3,$7,$5
	sltu	$7,$3,$7
	addu	$3,$3,$4
	addu	$7,$3,$7
	sltu	$4,$7,$4
	addu	$7,$7,$6
	addu	$4,$7,$4
	andi	$5,$5,0xf
	sll	$5,$5,2
	sltu	$3,$4,$6
	addu	$3,$3,$4
	addu	$5,$18,$5
$L52:
	lw	$4,0($2)
	addiu	$2,$2,4
	addu	$3,$3,$4
	sltu	$4,$3,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L52
	addu	$3,$4,$3
	.set	macro
	.set	reorder

 #APP
 # 78 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat
	sll	$1, $3, 16
	addu	$3, $1
	sltu	$1, $3, $1
	srl	$3, $3, 16
	addu	$3, $1
	xori	$3, 0xffff
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L47
	sh	$3,10($18)
	.set	macro
	.set	reorder

$L58:
	addiu	$3,$3,%lo(pptpAccInfo)
	lhu	$6,6($2)
	lhu	$5,18($3)
	bne	$6,$5,$L47
	lw	$6,32($3)
	lw	$5,16($18)
	bne	$6,$5,$L47
	lw	$3,40($3)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3
	rotr	$3, $3, 16

 # 0 "" 2
 #NO_APP
	sw	$3,20($sp)
	sb	$3,12($2)
	addiu	$3,$sp,20
	lbu	$5,1($3)
	addiu	$2,$2,12
	sb	$5,1($2)
	lbu	$5,2($3)
	sb	$5,2($2)
	lbu	$3,3($3)
	.set	noreorder
	.set	nomacro
	j	$L51
	sb	$3,3($2)
	.set	macro
	.set	reorder

$L57:
	lw	$6,32($4)
	lw	$5,16($18)
	.set	noreorder
	.set	nomacro
	bne	$6,$5,$L50
	lui	$7,%hi(pptpAccInfo+48)
	.set	macro
	.set	reorder

	lw	$6,36($4)
	lw	$3,8($2)
	addiu	$5,$7,%lo(pptpAccInfo+48)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3
	rotr	$3, $3, 16

 # 0 "" 2
 #NO_APP
	addiu	$10,$6,1
	srl	$11,$3,24
	srl	$9,$3,8
	srl	$8,$3,16
	sb	$11,3($5)
	sw	$10,36($4)
	sb	$3,%lo(pptpAccInfo+48)($7)
	sb	$9,1($5)
	sb	$8,2($5)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $6
	rotr	$3, $3, 16

 # 0 "" 2
 #NO_APP
	sw	$3,20($sp)
	sb	$3,8($2)
	lbu	$5,21($sp)
	addiu	$3,$2,8
	move	$4,$0
	sb	$5,1($3)
	lbu	$5,22($sp)
	sb	$5,2($3)
	lbu	$5,23($sp)
	sb	$5,3($3)
	.set	noreorder
	.set	nomacro
	j	$L49
	lbu	$3,1($2)
	.set	macro
	.set	reorder

	.end	sync_tx_pptp_gre_seqno
	.size	sync_tx_pptp_gre_seqno, .-sync_tx_pptp_gre_seqno
	.align	2
	.globl	fast_pptp_sync_rx_seq
	.set	nomips16
	.ent	fast_pptp_sync_rx_seq
	.type	fast_pptp_sync_rx_seq, @function
fast_pptp_sync_rx_seq:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	jal	rtl_ip_hdr
	addiu	$3,$2,20
	lbu	$4,1($3)
	li	$5,1			# 0x1
	andi	$6,$4,0x7
	.set	noreorder
	.set	nomacro
	beq	$6,$5,$L63
	li	$5,34827			# 0x880b
	.set	macro
	.set	reorder

$L62:
	lw	$31,20($sp)
$L64:
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,24
	.set	macro
	.set	reorder

$L63:
	lhu	$6,2($3)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$6, $6

 # 0 "" 2
 #NO_APP
	andi	$6,$6,0xffff
	.set	noreorder
	.set	nomacro
	bne	$6,$5,$L64
	lw	$31,20($sp)
	.set	macro
	.set	reorder

	lbu	$2,20($2)
	andi	$2,$2,0x10
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L65
	seb	$4,$4
	.set	macro
	.set	reorder

	lui	$2,%hi(pptpAccInfo)
	addiu	$2,$2,%lo(pptpAccInfo)
	lw	$4,52($2)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $4
	rotr	$5, $5, 16

 # 0 "" 2
 #NO_APP
	addiu	$4,$4,1
	sw	$4,52($2)
	lbu	$4,1($3)
	sw	$5,8($3)
	seb	$4,$4
$L65:
	.set	noreorder
	.set	nomacro
	bgez	$4,$L64
	lw	$31,20($sp)
	.set	macro
	.set	reorder

	lui	$2,%hi(pptpAccInfo+48)
	lw	$2,%lo(pptpAccInfo+48)($2)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L62
	sw	$2,12($3)
	.set	macro
	.set	reorder

	.end	fast_pptp_sync_rx_seq
	.size	fast_pptp_sync_rx_seq, .-fast_pptp_sync_rx_seq
	.align	2
	.globl	is_pptp_device
	.set	nomips16
	.ent	is_pptp_device
	.type	is_pptp_device, @function
is_pptp_device:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$2,%hi(pptpAccInfo+56)
	lw	$3,%lo(pptpAccInfo+56)($2)
	move	$16,$4
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L68
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
$L68:
	lw	$31,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	is_pptp_device
	.size	is_pptp_device, .-is_pptp_device
	.section	.rodata.str1.4
	.align	2
$LC3:
	.ascii	"ppp0\000"
	.align	2
$LC4:
	.ascii	"RTL\000"
	.align	2
$LC5:
	.ascii	"pptpInfo.ppp0_dev == NULL\012\000"
	.align	2
$LC6:
	.ascii	"pptpInfo.ppp0_dev->priv == NULL\012\000"
	.text
	.align	2
	.globl	fast_pptp_to_wan
	.set	nomips16
	.ent	fast_pptp_to_wan
	.type	fast_pptp_to_wan, @function
fast_pptp_to_wan:
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
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$17,$4
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	lbu	$16,12($2)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	lw	$18,12($2)
	.set	macro
	.set	reorder

	lw	$3,12($2)
	li	$2,10			# 0xa
	.set	noreorder
	.set	nomacro
	beq	$16,$2,$L85
	andi	$18,$18,0xff00
	.set	macro
	.set	reorder

$L71:
	lui	$18,%hi(pptpAccInfo)
$L91:
	addiu	$16,$18,%lo(pptpAccInfo)
$L92:
	lw	$4,56($16)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L76
	move	$2,$0
	.set	macro
	.set	reorder

	jal	rtl_get_ppp_dev_name
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L76
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_ppp_dev_name
	lw	$4,56($16)
	.set	macro
	.set	reorder

	lui	$3,%hi($LC3)
	addiu	$3,$3,%lo($LC3)
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
	bne	$4,$0,$L76
	move	$2,$0
	.set	macro
	.set	reorder

	lw	$2,4($16)
	beq	$2,$0,$L73
	lw	$4,56($16)
	beq	$4,$0,$L74
	jal	rtl_get_ppp_dev_priv
	beq	$2,$0,$L73
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ppp_dev_priv
	lw	$4,56($16)
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_vj_check
	move	$19,$2
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L76
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_xmit_lock
	lbu	$20,1($2)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_ppp_xmit_pending
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L75
	lui	$5,%hi($LC4)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_xmit_unlock
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L76
	move	$2,$0
	.set	macro
	.set	reorder

$L73:
	lw	$4,56($16)
	beq	$4,$0,$L74
	jal	rtl_get_ppp_dev_priv
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L86
	lui	$4,%hi($LC6)
	.set	macro
	.set	reorder

	move	$2,$0
$L76:
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

$L85:
	li	$2,16384			# 0x4000
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L71
	li	$2,16711680			# 0xff0000
	.set	macro
	.set	reorder

	and	$3,$3,$2
	li	$2,4194304			# 0x400000
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L91
	lui	$18,%hi(pptpAccInfo)
	.set	macro
	.set	reorder

	lui	$2,%hi(pptp_conn_check)
	lw	$3,%lo(pptp_conn_check)($2)
	li	$2,3			# 0x3
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L92
	addiu	$16,$18,%lo(pptpAccInfo)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L76
	move	$2,$0
	.set	macro
	.set	reorder

$L75:
	addiu	$5,$5,%lo($LC4)
	li	$6,3			# 0x3
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_cb
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,56($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_start_xmit
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_ppp_xmit_pending
	move	$4,$19
	.set	macro
	.set	reorder

	beq	$2,$0,$L87
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ppp_xmit_pending
	move	$4,$19
	.set	macro
	.set	reorder

	move	$5,$0
	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_set_ppp_xmit_pending
	move	$21,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_xmit_unlock
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_headroom
	move	$4,$21
	.set	macro
	.set	reorder

	sltu	$2,$2,50
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L78
	move	$17,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_cloned
	move	$4,$21
	.set	macro
	.set	reorder

	beq	$2,$0,$L88
$L78:
	move	$4,$21
$L90:
	.set	noreorder
	.set	nomacro
	jal	skb_realloc_headroom
	li	$5,50			# 0x32
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L89
	move	$17,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	consume_skb
	move	$4,$21
	.set	macro
	.set	reorder

	li	$5,50			# 0x32
$L93:
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$4,$17
	.set	macro
	.set	reorder

	lui	$5,%hi(pptpAccInfo+24)
	move	$4,$2
	addiu	$5,$5,%lo(pptpAccInfo+24)
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$19,$2
	.set	macro
	.set	reorder

	lui	$5,%hi(pptpAccInfo+12)
	addiu	$5,$5,%lo(pptpAccInfo+12)
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$19,6
	.set	macro
	.set	reorder

	li	$2,8			# 0x8
	sb	$2,12($19)
	sb	$0,13($19)
	lw	$3,32($sp)
	li	$2,4			# 0x4
	lw	$7,32($16)
	lw	$6,20($16)
	ins	$3,$2,4,4
	li	$2,5			# 0x5
	ins	$3,$2,0,4
	move	$5,$0
	li	$2,64			# 0x40
	sw	$3,32($sp)
	move	$4,$17
	li	$3,47			# 0x2f
	sb	$3,41($sp)
	sw	$7,48($sp)
	sw	$6,44($sp)
	sb	$2,40($sp)
	sh	$2,38($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ip_summed
	sb	$20,33($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$4,$17
	.set	macro
	.set	reorder

	lhu	$3,44($16)
	addiu	$5,$sp,32
	sh	$0,42($sp)
	addiu	$3,$3,1
	andi	$4,$3,0xffff
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $4

 # 0 "" 2
 #NO_APP
	andi	$3,$3,0xffff
	sh	$3,36($sp)
	addiu	$2,$2,-14
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$2,34($sp)
	lw	$7,32($sp)
	lw	$9,4($5)
	lw	$6,8($5)
	lw	$8,44($sp)
	addu	$2,$9,$7
	sltu	$9,$2,$9
	addu	$2,$2,$6
	addu	$9,$2,$9
	sltu	$6,$9,$6
	addu	$9,$9,$8
	addu	$6,$9,$6
	andi	$7,$7,0xf
	sll	$7,$7,2
	sltu	$2,$6,$8
	sh	$4,44($16)
	addu	$2,$2,$6
	addu	$7,$5,$7
	addiu	$4,$sp,48
$L81:
	lw	$6,0($4)
	addiu	$4,$4,4
	addu	$2,$2,$6
	sltu	$6,$2,$6
	.set	noreorder
	.set	nomacro
	bne	$7,$4,$L81
	addu	$2,$6,$2
	.set	macro
	.set	reorder

	sh	$3,46($16)
	move	$4,$17
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
	sh	$2,42($sp)
	.set	macro
	.set	reorder

	lw	$5,56($sp)
	li	$6,20			# 0x14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$2,14
	.set	macro
	.set	reorder

	li	$2,48			# 0x30
	sb	$2,16($sp)
	li	$2,-127			# 0xffffffffffffff81
	move	$4,$17
	sb	$2,17($sp)
	li	$2,2952			# 0xb88
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$2,18($sp)
	.set	macro
	.set	reorder

	lw	$3,36($16)
	lhu	$6,18($16)
	move	$4,$17
	addiu	$5,$3,1
	sw	$5,36($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3
	rotr	$3, $3, 16

 # 0 "" 2
 #NO_APP
	sh	$6,22($sp)
	sw	$3,24($sp)
	addiu	$2,$2,-50
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$2,20($sp)
	lw	$2,40($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sw	$2,28($sp)
	.set	macro
	.set	reorder

	addiu	$4,$2,34
	li	$6,16			# 0x10
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	lw	$5,8($16)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dev
	sw	$2,%lo(pptpAccInfo)($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L76
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L86:
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC6)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L76
	move	$2,$0
	.set	macro
	.set	reorder

$L88:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_shared
	move	$4,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L93
	li	$5,50			# 0x32
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L90
	move	$4,$21
	.set	macro
	.set	reorder

$L87:
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_xmit_unlock
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L76
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L89:
	.set	noreorder
	.set	nomacro
	jal	consume_skb
	move	$4,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L76
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L74:
	lui	$4,%hi($LC5)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC5)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L76
	move	$2,$0
	.set	macro
	.set	reorder

	.end	fast_pptp_to_wan
	.size	fast_pptp_to_wan, .-fast_pptp_to_wan
	.align	2
	.globl	fast_pptp_filter
	.set	nomips16
	.ent	fast_pptp_filter
	.type	fast_pptp_filter, @function
fast_pptp_filter:
	.frame	$sp,40,$31		# vars= 0, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$19,32($sp)
	sw	$18,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$16,$4
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$19,$2
	.set	macro
	.set	reorder

	sltu	$2,$2,56
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L105
	lw	$31,36($sp)
	.set	macro
	.set	reorder

	lw	$2,0($19)
	lbu	$3,9($19)
	andi	$4,$2,0xf
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L103
	li	$2,47878			# 0xbb06
	.set	macro
	.set	reorder

$L102:
	lw	$31,36($sp)
$L105:
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,40
	.set	macro
	.set	reorder

$L103:
	sll	$4,$4,2
	addu	$4,$19,$4
	lhu	$3,0($4)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L105
	lw	$31,36($sp)
	.set	macro
	.set	reorder

	lw	$2,12($4)
	ext	$2,$2,4,4
	sll	$2,$2,2
	addu	$4,$4,$2
	lhu	$3,2($4)
	li	$2,1			# 0x1
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $3

 # 0 "" 2
 #NO_APP
	andi	$5,$5,0xffff
	.set	noreorder
	.set	nomacro
	beq	$5,$2,$L104
	addiu	$6,$4,8
	.set	macro
	.set	reorder

$L97:
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	andi	$3,$3,0xffff
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L102
	li	$2,13			# 0xd
	.set	macro
	.set	reorder

	lhu	$3,8($4)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3

 # 0 "" 2
 #NO_APP
	andi	$3,$3,0xffff
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L102
	lui	$17,%hi(pptpAccInfo)
	.set	macro
	.set	reorder

	addiu	$17,$17,%lo(pptpAccInfo)
	lw	$2,36($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L106
	lui	$2,%hi(pptp_tcp_finished)
	.set	macro
	.set	reorder

	lw	$2,20($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L106
	lui	$2,%hi(pptp_tcp_finished)
	.set	macro
	.set	reorder

	lw	$2,32($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L106
	lui	$2,%hi(pptp_tcp_finished)
	.set	macro
	.set	reorder

	lhu	$2,4($6)
	lhu	$3,18($17)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L105
	lw	$31,36($sp)
	.set	macro
	.set	reorder

	lui	$2,%hi(pptp_tcp_finished)
$L106:
	sw	$0,%lo(pptp_tcp_finished)($2)
	.set	noreorder
	.set	nomacro
	j	$L102
	sw	$0,4($17)
	.set	macro
	.set	reorder

$L104:
	lhu	$7,8($4)
	li	$2,8			# 0x8
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$7, $7

 # 0 "" 2
 #NO_APP
	andi	$7,$7,0xffff
	.set	noreorder
	.set	nomacro
	bne	$7,$2,$L97
	addiu	$2,$4,12
	.set	macro
	.set	reorder

	lbu	$7,4($2)
	.set	noreorder
	.set	nomacro
	bne	$7,$5,$L97
	lui	$17,%hi(pptpAccInfo)
	.set	macro
	.set	reorder

	addiu	$17,$17,%lo(pptpAccInfo)
	lw	$3,36($17)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L107
	li	$18,1			# 0x1
	.set	macro
	.set	reorder

	lw	$3,20($17)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L108
	lui	$3,%hi(pptp_tcp_finished)
	.set	macro
	.set	reorder

	lw	$3,32($17)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L108
	lui	$3,%hi(pptp_tcp_finished)
	.set	macro
	.set	reorder

	lhu	$5,18($17)
	lhu	$3,12($4)
	.set	noreorder
	.set	nomacro
	bne	$5,$3,$L105
	lw	$31,36($sp)
	.set	macro
	.set	reorder

	lhu	$5,30($17)
	lhu	$3,2($2)
	bne	$5,$3,$L105
$L107:
	lui	$3,%hi(pptp_tcp_finished)
$L108:
	sw	$18,%lo(pptp_tcp_finished)($3)
	lhu	$3,12($4)
	move	$4,$16
	sh	$3,18($17)
	lhu	$2,2($2)
	sh	$2,30($17)
	lw	$2,16($19)
	sw	$2,20($17)
	lw	$2,12($19)
	sw	$18,36($17)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	sw	$2,32($17)
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sw	$2,8($17)
	.set	macro
	.set	reorder

	lui	$4,%hi(pptpAccInfo+24)
	addiu	$5,$2,-8
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,%lo(pptpAccInfo+24)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$4,%hi(pptpAccInfo+12)
	addiu	$4,$4,%lo(pptpAccInfo+12)
	addiu	$5,$2,-14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L102
	sw	$18,4($17)
	.set	macro
	.set	reorder

	.end	fast_pptp_filter
	.size	fast_pptp_filter, .-fast_pptp_filter
	.align	2
	.globl	Check_GRE_rx_net_device
	.set	nomips16
	.ent	Check_GRE_rx_net_device
	.type	Check_GRE_rx_net_device, @function
Check_GRE_rx_net_device:
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
	lui	$18,%hi(pptpAccInfo)
	jal	rtl_ip_hdr
	move	$17,$4

	move	$16,$2
	move	$4,$17
	jal	rtl_get_skb_data
	addiu	$18,$18,%lo(pptpAccInfo)

	lw	$20,0($16)
	lw	$16,8($18)
	move	$19,$2
	beq	$16,$0,$L110
	andi	$20,$20,0xf

	jal	rtl_get_skb_dev
	move	$4,$17

	beq	$16,$2,$L113
	sll	$20,$20,2

$L110:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,40

$L113:
	addu	$19,$19,$20
	lhu	$3,6($19)
	lhu	$2,30($18)
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	xor	$2,$3,$2
	sltu	$2,$2,1
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	Check_GRE_rx_net_device
	.size	Check_GRE_rx_net_device, .-Check_GRE_rx_net_device
	.align	2
	.globl	fast_pptp_to_lan
	.set	nomips16
	.ent	fast_pptp_to_lan
	.type	fast_pptp_to_lan, @function
fast_pptp_to_lan:
	.frame	$sp,56,$31		# vars= 8, regs= 7/0, args= 16, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$21,48($sp)
	sw	$20,44($sp)
	sw	$19,40($sp)
	sw	$18,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	lw	$16,0($4)
	lui	$17,%hi(pptpAccInfo)
	move	$18,$4
	addiu	$17,$17,%lo(pptpAccInfo)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,8($17)
	beq	$3,$2,$L135
$L115:
	move	$2,$0
$L127:
	lw	$31,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,56
	.set	macro
	.set	reorder

$L135:
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$16
	.set	macro
	.set	reorder

	lbu	$3,9($2)
	li	$2,47			# 0x2f
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L127
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$4,$16
	.set	macro
	.set	reorder

	sltu	$2,$2,20
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L127
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$16
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,0($19)
	li	$4,1			# 0x1
	andi	$3,$3,0xf
	sll	$3,$3,2
	addu	$2,$2,$3
	lbu	$3,1($2)
	andi	$5,$3,0x7
	bne	$5,$4,$L115
	lhu	$5,2($2)
	li	$4,34827			# 0x880b
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $5

 # 0 "" 2
 #NO_APP
	andi	$5,$5,0xffff
	bne	$5,$4,$L115
	lhu	$5,6($2)
	lhu	$4,30($17)
	bne	$5,$4,$L115
	lbu	$4,0($2)
	andi	$4,$4,0x10
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L117
	li	$20,8			# 0x8
	.set	macro
	.set	reorder

	lw	$3,8($2)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3
	rotr	$3, $3, 16

 # 0 "" 2
 #NO_APP
	sw	$3,40($17)
	lbu	$3,1($2)
	li	$20,12			# 0xc
$L117:
	lhu	$4,4($2)
	srl	$3,$3,7
	addiu	$5,$20,4
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L115
	movn	$20,$5,$3
	.set	macro
	.set	reorder

	addu	$2,$2,$20
	lbu	$4,0($2)
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L119
	li	$21,1			# 0x1
	.set	macro
	.set	reorder

	lbu	$4,1($2)
$L120:
	li	$2,33			# 0x21
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L123
	li	$2,253			# 0xfd
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L127
	move	$2,$0
	.set	macro
	.set	reorder

	lw	$19,0($19)
	lw	$4,56($17)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L127
	andi	$19,$19,0xf
	.set	macro
	.set	reorder

	jal	rtl_get_ppp_dev_priv
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L115
	sll	$5,$19,2
	.set	macro
	.set	reorder

	addiu	$5,$5,-1
	addu	$5,$5,$20
	addu	$5,$21,$5
	.set	noreorder
	.set	nomacro
	jal	skb_pull
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$16
	.set	macro
	.set	reorder

	li	$3,-3			# 0xfffffffffffffffd
	sb	$3,1($2)
	sb	$0,0($2)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ppp_dev_priv
	lw	$4,56($17)
	.set	macro
	.set	reorder

	move	$19,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_recv_lock
	move	$4,$2
	.set	macro
	.set	reorder

	move	$5,$16
	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_receive_nonmp_frame
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	move	$16,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_recv_unlock
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L127
	li	$2,-2			# 0xfffffffffffffffe
	.set	macro
	.set	reorder

	sw	$16,0($18)
	li	$19,1			# 0x1
$L125:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$16
	.set	macro
	.set	reorder

	andi	$18,$2,0x3
	bne	$18,$0,$L136
$L128:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_reset_network_header
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_reset_transport_header
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$5,56($17)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L127
	move	$2,$0
	.set	macro
	.set	reorder

	beq	$19,$0,$L137
$L129:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dev
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L127
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L119:
	li	$3,255			# 0xff
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L120
	move	$21,$0
	.set	macro
	.set	reorder

	lbu	$4,1($2)
	li	$3,3			# 0x3
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L115
	li	$21,2			# 0x2
	.set	macro
	.set	reorder

	lbu	$4,2($2)
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L120
	addiu	$2,$2,2
	.set	macro
	.set	reorder

	lbu	$4,1($2)
	.set	noreorder
	.set	nomacro
	j	$L120
	li	$21,3			# 0x3
	.set	macro
	.set	reorder

$L123:
	lw	$5,0($19)
	move	$4,$16
	move	$19,$0
	andi	$5,$5,0xf
	sll	$5,$5,2
	addiu	$5,$5,1
	addu	$5,$5,$20
	.set	noreorder
	.set	nomacro
	jal	skb_pull
	addu	$5,$5,$21
	.set	macro
	.set	reorder

	j	$L125
$L137:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ppp_dev_priv
	move	$4,$5
	.set	macro
	.set	reorder

	beq	$2,$0,$L134
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ppp_dev_priv
	lw	$4,56($17)
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$18,$2
	.set	macro
	.set	reorder

	addiu	$6,$2,-2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_inc_ppp_stats
	move	$5,$0
	.set	macro
	.set	reorder

$L134:
	.set	noreorder
	.set	nomacro
	j	$L129
	lw	$5,56($17)
	.set	macro
	.set	reorder

$L136:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_headroom
	move	$4,$16
	.set	macro
	.set	reorder

	sltu	$2,$2,$18
	bne	$2,$0,$L128
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$16
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$20,$2
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$5,16($sp)
	subu	$4,$20,$18
	.set	noreorder
	.set	nomacro
	jal	memmove
	move	$6,$2
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_data
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_tail
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	j	$L128
	.end	fast_pptp_to_lan
	.size	fast_pptp_to_lan, .-fast_pptp_to_lan
	.align	2
	.globl	set_pptp_device
	.set	nomips16
	.ent	set_pptp_device
	.type	set_pptp_device, @function
set_pptp_device:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(fast_pptp_fw)
	lw	$2,%lo(fast_pptp_fw)($2)
	beq	$2,$0,$L141
	lw	$31,20($sp)

	jal	rtl_get_dev_by_name
	nop

	lui	$3,%hi(pptpAccInfo+56)
	sw	$2,%lo(pptpAccInfo+56)($3)
	lw	$31,20($sp)
$L141:
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	set_pptp_device
	.size	set_pptp_device, .-set_pptp_device
	.globl	pptp_tcp_finished
	.section	.bss,"aw",@nobits
	.align	2
	.type	pptp_tcp_finished, @object
	.size	pptp_tcp_finished, 4
pptp_tcp_finished:
	.space	4
	.globl	fast_pptp_fw
	.align	2
	.type	fast_pptp_fw, @object
	.size	fast_pptp_fw, 4
fast_pptp_fw:
	.space	4
	.globl	fastpath_pptp_proc_fops
	.data
	.align	2
	.type	fastpath_pptp_proc_fops, @object
	.size	fastpath_pptp_proc_fops, 108
fastpath_pptp_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	fastpath_pptp_single_write
	.space	28
	.word	fastpath_pptp_single_open
	.space	4
	.word	single_release
	.space	52
	.globl	pptp_conn_proc_fops
	.align	2
	.type	pptp_conn_proc_fops, @object
	.size	pptp_conn_proc_fops, 108
pptp_conn_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	pptp_conn_single_write
	.space	28
	.word	pptp_conn_single_open
	.space	4
	.word	single_release
	.space	52
	.local	res1
	.comm	res1,4,4
	.local	res_check_pptp
	.comm	res_check_pptp,4,4
	.local	pptp_conn_check
	.comm	pptp_conn_check,4,4
	.local	pptpAccInfo
	.comm	pptpAccInfo,64,4
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
