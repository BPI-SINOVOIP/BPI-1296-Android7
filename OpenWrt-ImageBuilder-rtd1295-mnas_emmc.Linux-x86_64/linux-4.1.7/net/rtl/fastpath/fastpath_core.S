/*
 * Copyright (C) 2017 Realtek Semiconductor Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

	.file	1 "fastpath_core.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.text
	.align	2
	.globl	rtl_init_bypass_packet
	.set	nomips16
	.ent	rtl_init_bypass_packet
	.type	rtl_init_bypass_packet, @function
rtl_init_bypass_packet:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	sw	$0,64($4)
	j	$31
	sw	$0,68($4)

	.set	macro
	.set	reorder
	.end	rtl_init_bypass_packet
	.size	rtl_init_bypass_packet, .-rtl_init_bypass_packet
	.align	2
	.globl	rtl_check_bypass_packet
	.set	nomips16
	.ent	rtl_check_bypass_packet
	.type	rtl_check_bypass_packet, @function
rtl_check_bypass_packet:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	lw	$2,68($4)
	bne	$2,$0,$L8
	li	$2,1			# 0x1

	lw	$3,64($4)
	lui	$5,%hi(fastpath_packet_bypassed_num)
	addiu	$3,$3,1
	sw	$3,64($4)
	lw	$5,%lo(fastpath_packet_bypassed_num)($5)
	sltu	$3,$5,$3
	beq	$3,$0,$L9
	nop

$L8:
	sw	$2,68($4)
	move	$2,$0
$L9:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	rtl_check_bypass_packet
	.size	rtl_check_bypass_packet, .-rtl_check_bypass_packet
	.align	2
	.set	nomips16
	.ent	cache_timeout
	.type	cache_timeout, @function
cache_timeout:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	lbu	$2,0($4)
	beq	$2,$0,$L15
	nop

	lw	$3,4($4)
	lw	$5,8($4)
	lhu	$2,2($4)
	lbu	$6,1($4)
	xor	$5,$5,$3
	lui	$3,%hi(max_udp_frag_entry)
	xor	$2,$2,$5
	lw	$3,%lo(max_udp_frag_entry)($3)
	srl	$5,$5,16
	xor	$2,$2,$5
	lw	$5,44($4)
	xor	$6,$2,$6
	srl	$2,$2,8
	xor	$2,$6,$2
	addiu	$3,$3,-1
	and	$2,$2,$3
	beq	$5,$0,$L12
	sb	$0,0($4)

	lw	$6,48($4)
	sll	$3,$2,4
	sll	$2,$2,2
	sw	$6,48($5)
	subu	$3,$3,$2
	lui	$5,%hi(udp_cache_table)
$L13:
	lw	$6,48($4)
	lw	$7,44($4)
	lui	$2,%hi(udp_cache_list_free)
	addiu	$2,$2,%lo(udp_cache_list_free)
	sw	$7,0($6)
	lw	$6,%lo(udp_cache_table)($5)
	addiu	$5,$4,52
	lw	$6,0($6)
	addu	$3,$6,$3
	lw	$6,8($3)
	addiu	$6,$6,-1
	sw	$6,8($3)
	sw	$0,52($4)
	lw	$3,4($2)
	sw	$3,56($4)
	lw	$3,4($2)
	sw	$4,0($3)
	lw	$3,8($2)
	sw	$5,4($2)
	addiu	$3,$3,1
	sw	$3,8($2)
$L15:
	j	$31
	nop

$L12:
	lui	$5,%hi(udp_cache_table)
	lw	$6,%lo(udp_cache_table)($5)
	sll	$3,$2,4
	sll	$2,$2,2
	lw	$7,0($6)
	lw	$6,48($4)
	subu	$3,$3,$2
	addu	$2,$7,$3
	j	$L13
	sw	$6,4($2)

	.set	macro
	.set	reorder
	.end	cache_timeout
	.size	cache_timeout, .-cache_timeout
	.align	2
	.set	nomips16
	.ent	negative_cache_timeout
	.type	negative_cache_timeout, @function
negative_cache_timeout:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	lw	$3,4($4)
	lw	$5,8($4)
	lhu	$2,2($4)
	lbu	$6,1($4)
	xor	$5,$5,$3
	lui	$3,%hi(max_udp_frag_entry)
	xor	$2,$2,$5
	lw	$3,%lo(max_udp_frag_entry)($3)
	srl	$5,$5,16
	xor	$2,$2,$5
	lw	$5,40($4)
	xor	$6,$2,$6
	srl	$2,$2,8
	xor	$2,$6,$2
	addiu	$3,$3,-1
	and	$2,$2,$3
	beq	$5,$0,$L17
	sb	$0,0($4)

	lw	$7,44($4)
	sll	$3,$2,4
	lui	$6,%hi(negative_cache_table)
	sw	$7,44($5)
	sll	$2,$2,2
$L18:
	lw	$5,44($4)
	lw	$7,40($4)
	subu	$3,$3,$2
	lui	$2,%hi(negative_cache_list_free)
	sw	$7,0($5)
	lw	$6,%lo(negative_cache_table)($6)
	addiu	$2,$2,%lo(negative_cache_list_free)
	addiu	$5,$4,48
	addu	$3,$6,$3
	lw	$6,8($3)
	addiu	$6,$6,-1
	sw	$6,8($3)
	sw	$0,48($4)
	lw	$3,4($2)
	sw	$3,52($4)
	lw	$3,4($2)
	sw	$4,0($3)
	lw	$3,8($2)
	sw	$5,4($2)
	addiu	$3,$3,1
	j	$31
	sw	$3,8($2)

$L17:
	lui	$6,%hi(negative_cache_table)
	lw	$8,%lo(negative_cache_table)($6)
	sll	$3,$2,4
	lw	$7,44($4)
	sll	$2,$2,2
	subu	$5,$3,$2
	addu	$5,$8,$5
	j	$L18
	sw	$7,4($5)

	.set	macro
	.set	reorder
	.end	negative_cache_timeout
	.size	negative_cache_timeout, .-negative_cache_timeout
	.align	2
	.globl	rtk_addArp
	.set	nomips16
	.ent	rtk_addArp
	.type	rtk_addArp, @function
rtk_addArp:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_addArp
	.size	rtk_addArp, .-rtk_addArp
	.align	2
	.globl	rtk_modifyArp
	.set	nomips16
	.ent	rtk_modifyArp
	.type	rtk_modifyArp, @function
rtk_modifyArp:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_modifyArp
	.size	rtk_modifyArp, .-rtk_modifyArp
	.align	2
	.globl	rtk_delArp
	.set	nomips16
	.ent	rtk_delArp
	.type	rtk_delArp, @function
rtk_delArp:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	j	$31
	li	$2,-4			# 0xfffffffffffffffc

	.set	macro
	.set	reorder
	.end	rtk_delArp
	.size	rtk_delArp, .-rtk_delArp
	.align	2
	.globl	rtk_addRoute
	.set	nomips16
	.ent	rtk_addRoute
	.type	rtk_addRoute, @function
rtk_addRoute:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_addRoute
	.size	rtk_addRoute, .-rtk_addRoute
	.align	2
	.globl	rtk_modifyRoute
	.set	nomips16
	.ent	rtk_modifyRoute
	.type	rtk_modifyRoute, @function
rtk_modifyRoute:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_modifyRoute
	.size	rtk_modifyRoute, .-rtk_modifyRoute
	.align	2
	.globl	rtk_delRoute
	.set	nomips16
	.ent	rtk_delRoute
	.type	rtk_delRoute, @function
rtk_delRoute:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	j	$31
	li	$2,-4			# 0xfffffffffffffffc

	.set	macro
	.set	reorder
	.end	rtk_delRoute
	.size	rtk_delRoute, .-rtk_delRoute
	.align	2
	.globl	rtk_addSession
	.set	nomips16
	.ent	rtk_addSession
	.type	rtk_addSession, @function
rtk_addSession:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_addSession
	.size	rtk_addSession, .-rtk_addSession
	.align	2
	.globl	rtk_delSession
	.set	nomips16
	.ent	rtk_delSession
	.type	rtk_delSession, @function
rtk_delSession:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_delSession
	.size	rtk_delSession, .-rtk_delSession
	.align	2
	.globl	FastPath_Route
	.set	nomips16
	.ent	FastPath_Route
	.type	FastPath_Route, @function
FastPath_Route:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	lui	$2,%hi(route_list_inuse)
	lw	$3,%lo(route_list_inuse)($2)
	beq	$3,$0,$L44
	move	$2,$0

	move	$6,$0
$L41:
	lw	$5,8($3)
	sltu	$7,$5,$6
	bne	$7,$0,$L38
	and	$8,$4,$5

	lw	$7,4($3)
	beq	$8,$7,$L43
	nop

$L38:
	move	$5,$6
$L39:
	lw	$3,44($3)
	bne	$3,$0,$L41
	move	$6,$5

$L44:
	j	$31
	nop

$L43:
	j	$L39
	addiu	$2,$3,16

	.set	macro
	.set	reorder
	.end	FastPath_Route
	.size	FastPath_Route, .-FastPath_Route
	.align	2
	.globl	mark_path_invalid
	.set	nomips16
	.ent	mark_path_invalid
	.type	mark_path_invalid, @function
mark_path_invalid:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	srl	$2,$4,8
	xor	$2,$2,$4
	srl	$3,$6,16
	andi	$5,$5,0xffff
	xor	$2,$2,$6
	xor	$2,$2,$3
	srl	$8,$5,4
	lhu	$3,16($sp)
	xor	$2,$2,$8
	andi	$7,$7,0xffff
	lui	$8,%hi(path_table_list_max)
	lw	$10,%lo(path_table_list_max)($8)
	xor	$2,$2,$7
	lui	$9,%hi(table_path)
	xor	$2,$2,$3
	lw	$8,%lo(table_path)($9)
	srl	$11,$2,12
	xor	$2,$11,$2
	addiu	$10,$10,-1
	and	$2,$2,$10
	lw	$8,0($8)
	sll	$11,$2,4
	sll	$2,$2,2
	subu	$2,$11,$2
	addu	$2,$8,$2
	lw	$2,0($2)
	beq	$2,$0,$L63
	li	$11,255			# 0xff

	j	$L64
	lhu	$8,10($2)

$L47:
	lw	$2,76($2)
	beq	$2,$0,$L63
	nop

	lhu	$8,10($2)
$L64:
	bne	$8,$5,$L47
	nop

	lhu	$8,8($2)
	bne	$8,$7,$L47
	nop

	lw	$8,12($2)
	bne	$8,$4,$L47
	nop

	lw	$8,16($2)
	bne	$8,$6,$L47
	nop

	lbu	$12,0($2)
	bne	$12,$11,$L47
	nop

	lbu	$12,2($2)
	li	$6,1			# 0x1
	beq	$12,$6,$L61
	sb	$0,0($2)

	lhu	$13,32($2)
	lw	$6,28($2)
$L49:
	beq	$6,$0,$L63
	li	$11,1			# 0x1

	beq	$12,$11,$L62
	srl	$11,$4,16

	xor	$11,$4,$11
	xor	$11,$11,$5
	xor	$3,$11,$3
	xor	$3,$3,$6
	srl	$6,$6,8
	xor	$6,$3,$6
	srl	$2,$13,4
	xor	$2,$6,$2
	srl	$3,$2,12
	xor	$2,$3,$2
	and	$10,$2,$10
$L52:
	lw	$2,%lo(table_path)($9)
	sll	$3,$10,4
	sll	$10,$10,2
	lw	$2,0($2)
	subu	$10,$3,$10
	addu	$10,$2,$10
	lw	$2,0($10)
	beq	$2,$0,$L63
	li	$6,255			# 0xff

	j	$L65
	lw	$3,20($2)

$L53:
	lw	$2,76($2)
	beq	$2,$0,$L63
	nop

	lw	$3,20($2)
$L65:
	bne	$8,$3,$L53
	nop

	lhu	$3,24($2)
	bne	$3,$7,$L53
	nop

	lw	$3,28($2)
	bne	$4,$3,$L53
	nop

	lhu	$3,32($2)
	bne	$3,$5,$L53
	nop

	lbu	$3,0($2)
	bne	$3,$6,$L53
	nop

	j	$L53
	sb	$0,0($2)

$L63:
	j	$31
	nop

$L62:
	srl	$2,$8,8
	xor	$2,$8,$2
	xor	$3,$2,$3
	srl	$2,$7,4
	xor	$2,$3,$2
	xor	$2,$2,$6
	xor	$13,$2,$13
	srl	$6,$6,16
	xor	$6,$13,$6
	srl	$2,$6,12
	xor	$6,$2,$6
	j	$L52
	and	$10,$6,$10

$L61:
	lhu	$13,24($2)
	j	$L49
	lw	$6,20($2)

	.set	macro
	.set	reorder
	.end	mark_path_invalid
	.size	mark_path_invalid, .-mark_path_invalid
	.align	2
	.set	nomips16
	.ent	fastpath_napt_single_write
	.type	fastpath_napt_single_write, @function
fastpath_napt_single_write:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	j	$31
	move	$2,$6

	.set	macro
	.set	reorder
	.end	fastpath_napt_single_write
	.size	fastpath_napt_single_write, .-fastpath_napt_single_write
	.align	2
	.set	nomips16
	.ent	fastpath_path_single_write
	.type	fastpath_path_single_write, @function
fastpath_path_single_write:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	j	$31
	move	$2,$6

	.set	macro
	.set	reorder
	.end	fastpath_path_single_write
	.size	fastpath_path_single_write, .-fastpath_path_single_write
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"%s:%s\012\000"
	.align	2
$LC1:
	.ascii	"Realtek FastPath\000"
	.align	2
$LC2:
	.ascii	"v1.03\000"
	.text
	.align	2
	.globl	get_fastpath_module_info
	.set	nomips16
	.ent	get_fastpath_module_info
	.type	get_fastpath_module_info, @function
get_fastpath_module_info:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	beq	$4,$0,$L73
	lui	$5,%hi($LC0)

	lui	$6,%hi($LC1)
	lui	$7,%hi($LC2)
	addiu	$5,$5,%lo($LC0)
	addiu	$6,$6,%lo($LC1)
	addiu	$7,$7,%lo($LC2)
	j	sprintf
	nop

$L73:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	get_fastpath_module_info
	.size	get_fastpath_module_info, .-get_fastpath_module_info
	.section	.rodata.str1.4
	.align	2
$LC3:
	.ascii	"napt num: %d, path num: %d.\012\000"
	.text
	.align	2
	.globl	fastpath_dump_napt_entry_num
	.set	nomips16
	.ent	fastpath_dump_napt_entry_num
	.type	fastpath_dump_napt_entry_num, @function
fastpath_dump_napt_entry_num:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$2,%hi(path_list_inuse)
	lw	$2,%lo(path_list_inuse)($2)
	move	$16,$5
	beq	$2,$0,$L76
	move	$7,$0

$L83:
	lw	$2,84($2)
	bne	$2,$0,$L83
	addiu	$7,$7,1

$L76:
	lui	$2,%hi(napt_list_inuse)
	lw	$2,%lo(napt_list_inuse)($2)
	beq	$2,$0,$L79
	move	$6,$0

$L82:
	lw	$2,44($2)
	bne	$2,$0,$L82
	addiu	$6,$6,1

$L79:
	lui	$5,%hi($LC3)
	addu	$4,$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC3)

	addu	$2,$2,$16
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	fastpath_dump_napt_entry_num
	.size	fastpath_dump_napt_entry_num, .-fastpath_dump_napt_entry_num
	.align	2
	.globl	init_table_path
	.set	nomips16
	.ent	init_table_path
	.type	init_table_path, @function
init_table_path:
	.frame	$sp,48,$31		# vars= 0, regs= 7/0, args= 16, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$21,40($sp)
	sw	$20,36($sp)
	sw	$19,32($sp)
	sw	$18,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$20,%hi(kmalloc_caches)
	addiu	$20,$20,%lo(kmalloc_caches)
	move	$18,$4
	lw	$4,28($20)
	move	$19,$5
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	lui	$17,%hi(table_path)
	move	$21,$2
	beq	$2,$0,$L87
	sw	$2,%lo(table_path)($17)

	lui	$6,%hi(path_list_inuse)
	sll	$2,$18,2
	sll	$4,$18,4
	lui	$3,%hi(path_list_free)
	subu	$4,$4,$2
	addiu	$2,$6,%lo(path_list_inuse)
	addiu	$16,$3,%lo(path_list_free)
	sw	$0,%lo(path_list_free)($3)
	li	$5,32			# 0x20
	lui	$3,%hi(path_table_list_max)
	sw	$2,4($2)
	sw	$0,8($2)
	sw	$0,%lo(path_list_inuse)($6)
	sw	$18,%lo(path_table_list_max)($3)
	sw	$0,8($16)
	jal	__kmalloc
	sw	$16,4($16)

	sw	$2,0($21)
	lw	$2,%lo(table_path)($17)
	lw	$4,0($2)
	beq	$4,$0,$L87
	nop

	blez	$18,$L88
	move	$2,$0

	move	$3,$0
	j	$L89
	move	$5,$17

$L96:
	lw	$4,%lo(table_path)($5)
	lw	$4,0($4)
$L89:
	addu	$4,$4,$2
	sw	$0,8($4)
	lw	$4,%lo(table_path)($17)
	addiu	$3,$3,1
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$0,0($4)
	lw	$4,%lo(table_path)($17)
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$4,4($4)
	bne	$3,$18,$L96
	addiu	$2,$2,12

$L88:
	blez	$19,$L90
	move	$17,$0

	j	$L97
	lw	$4,28($20)

$L91:
	sw	$0,84($2)
	lw	$5,4($16)
	sw	$5,88($2)
	lw	$5,4($16)
	sw	$2,0($5)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$3,$0,$L90
	sw	$2,8($16)

	lw	$4,28($20)
$L97:
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,84
	bne	$2,$0,$L91
	slt	$3,$17,$19

	li	$2,-2			# 0xfffffffffffffffe
$L92:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L90:
	j	$L92
	move	$2,$0

$L87:
	j	$L92
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	init_table_path
	.size	init_table_path, .-init_table_path
	.align	2
	.globl	init_table_napt
	.set	nomips16
	.ent	init_table_napt
	.type	init_table_napt, @function
init_table_napt:
	.frame	$sp,48,$31		# vars= 0, regs= 7/0, args= 16, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$21,40($sp)
	sw	$20,36($sp)
	sw	$19,32($sp)
	sw	$18,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$20,%hi(kmalloc_caches)
	addiu	$20,$20,%lo(kmalloc_caches)
	move	$18,$4
	lw	$4,28($20)
	move	$19,$5
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	lui	$17,%hi(table_napt)
	move	$21,$2
	beq	$2,$0,$L99
	sw	$2,%lo(table_napt)($17)

	lui	$6,%hi(napt_list_inuse)
	sll	$2,$18,2
	sll	$4,$18,4
	lui	$3,%hi(napt_list_free)
	subu	$4,$4,$2
	addiu	$2,$6,%lo(napt_list_inuse)
	addiu	$16,$3,%lo(napt_list_free)
	sw	$0,%lo(napt_list_free)($3)
	li	$5,32			# 0x20
	lui	$3,%hi(napt_table_list_max)
	sw	$2,4($2)
	sw	$0,8($2)
	sw	$0,%lo(napt_list_inuse)($6)
	sw	$18,%lo(napt_table_list_max)($3)
	sw	$0,8($16)
	jal	__kmalloc
	sw	$16,4($16)

	sw	$2,0($21)
	lw	$2,%lo(table_napt)($17)
	lw	$4,0($2)
	beq	$4,$0,$L99
	nop

	blez	$18,$L100
	move	$2,$0

	move	$3,$0
	j	$L101
	move	$5,$17

$L108:
	lw	$4,%lo(table_napt)($5)
	lw	$4,0($4)
$L101:
	addu	$4,$4,$2
	sw	$0,8($4)
	lw	$4,%lo(table_napt)($17)
	addiu	$3,$3,1
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$0,0($4)
	lw	$4,%lo(table_napt)($17)
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$4,4($4)
	bne	$3,$18,$L108
	addiu	$2,$2,12

$L100:
	blez	$19,$L102
	move	$17,$0

	j	$L109
	lw	$4,28($20)

$L103:
	sw	$0,44($2)
	lw	$5,4($16)
	sw	$5,48($2)
	lw	$5,4($16)
	sw	$2,0($5)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$3,$0,$L102
	sw	$2,8($16)

	lw	$4,28($20)
$L109:
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,44
	bne	$2,$0,$L103
	slt	$3,$17,$19

	li	$2,-2			# 0xfffffffffffffffe
$L104:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L102:
	j	$L104
	move	$2,$0

$L99:
	j	$L104
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	init_table_napt
	.size	init_table_napt, .-init_table_napt
	.section	.rodata.str1.4
	.align	2
$LC4:
	.ascii	"fp_napt\000"
	.align	2
$LC5:
	.ascii	"fp_path\000"
	.text
	.align	2
	.globl	remove_fastpath_debug_proc
	.set	nomips16
	.ent	remove_fastpath_debug_proc
	.type	remove_fastpath_debug_proc, @function
remove_fastpath_debug_proc:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(fp_napt)
	lw	$2,%lo(fp_napt)($16)
	beq	$2,$0,$L111
	lui	$4,%hi($LC4)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC4)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(fp_napt)($16)
$L111:
	lui	$16,%hi(fp_path)
	lw	$2,%lo(fp_path)($16)
	beq	$2,$0,$L113
	lui	$4,%hi($LC5)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC5)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(fp_path)($16)
$L113:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	remove_fastpath_debug_proc
	.size	remove_fastpath_debug_proc, .-remove_fastpath_debug_proc
	.align	2
	.globl	init_fastpath_debug_proc
	.set	nomips16
	.ent	init_fastpath_debug_proc
	.type	init_fastpath_debug_proc, @function
init_fastpath_debug_proc:
	.frame	$sp,32,$31		# vars= 0, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	lui	$16,%hi(proc_root)
	lui	$4,%hi($LC4)
	lui	$7,%hi(fastpath_napt_proc_fops)
	addiu	$6,$16,%lo(proc_root)
	addiu	$4,$4,%lo($LC4)
	addiu	$7,$7,%lo(fastpath_napt_proc_fops)
	move	$5,$0
	jal	proc_create_data
	sw	$0,16($sp)

	lui	$4,%hi($LC5)
	lui	$7,%hi(fastpath_path_proc_fops)
	addiu	$6,$16,%lo(proc_root)
	lui	$3,%hi(fp_napt)
	addiu	$4,$4,%lo($LC5)
	addiu	$7,$7,%lo(fastpath_path_proc_fops)
	move	$5,$0
	sw	$2,%lo(fp_napt)($3)
	jal	proc_create_data
	sw	$0,16($sp)

	lw	$31,28($sp)
	lw	$16,24($sp)
	lui	$3,%hi(fp_path)
	sw	$2,%lo(fp_path)($3)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	init_fastpath_debug_proc
	.size	init_fastpath_debug_proc, .-init_fastpath_debug_proc
	.align	2
	.globl	fastpath_path_single_open
	.set	nomips16
	.ent	fastpath_path_single_open
	.type	fastpath_path_single_open, @function
fastpath_path_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	move	$4,$5
	lui	$5,%hi(fastpath_table_path)
	addiu	$5,$5,%lo(fastpath_table_path)
	move	$6,$0
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	fastpath_path_single_open
	.size	fastpath_path_single_open, .-fastpath_path_single_open
	.align	2
	.globl	fastpath_napt_single_open
	.set	nomips16
	.ent	fastpath_napt_single_open
	.type	fastpath_napt_single_open, @function
fastpath_napt_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	move	$4,$5
	lui	$5,%hi(fastpath_table_napt)
	addiu	$5,$5,%lo(fastpath_table_napt)
	move	$6,$0
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	fastpath_napt_single_open
	.size	fastpath_napt_single_open, .-fastpath_napt_single_open
	.section	.rodata.str1.4
	.align	2
$LC6:
	.ascii	"UDP\000"
	.align	2
$LC7:
	.ascii	"TCP\000"
	.align	2
$LC8:
	.ascii	"~Path: [%s] in-S=0x%08X:%-5u in-D=0x%08X:%-5u out-S=0x%0"
	.ascii	"8X:%-5u out-D=0x%08X:%-5u out-ifname=%-5s <%u> {%d}\000"
	.align	2
$LC9:
	.ascii	",[PreMark:%d,PostMark:%d]\000"
	.align	2
$LC10:
	.ascii	",[policy_route_mark:%d]\000"
	.align	2
$LC11:
	.ascii	",[dscp_in:%d,dscp_out:%d]\000"
	.align	2
$LC12:
	.ascii	",[vlanPrio_out:%d]\000"
	.align	2
$LC13:
	.ascii	"\012\000"
	.text
	.align	2
	.set	nomips16
	.ent	fastpath_table_path
	.type	fastpath_table_path, @function
fastpath_table_path:
	.frame	$sp,104,$31		# vars= 8, regs= 10/0, args= 56, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-104
	sw	$31,100($sp)
	sw	$fp,96($sp)
	sw	$23,92($sp)
	sw	$22,88($sp)
	sw	$21,84($sp)
	sw	$20,80($sp)
	sw	$19,76($sp)
	sw	$18,72($sp)
	sw	$17,68($sp)
	sw	$16,64($sp)
	lui	$2,%hi(path_list_inuse)
	lw	$16,%lo(path_list_inuse)($2)
	bne	$16,$0,$L127
	lui	$3,%hi($LC7)

$L121:
	lw	$31,100($sp)
	lw	$fp,96($sp)
	lw	$23,92($sp)
	lw	$22,88($sp)
	lw	$21,84($sp)
	lw	$20,80($sp)
	lw	$19,76($sp)
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,104

$L127:
	lui	$fp,%hi($LC6)
	lui	$23,%hi($LC8)
	lui	$22,%hi($LC9)
	lui	$21,%hi($LC10)
	lui	$20,%hi($LC11)
	lui	$19,%hi($LC12)
	lui	$18,%hi($LC13)
	addiu	$3,$3,%lo($LC7)
	addiu	$fp,$fp,%lo($LC6)
	addiu	$23,$23,%lo($LC8)
	addiu	$22,$22,%lo($LC9)
	addiu	$21,$21,%lo($LC10)
	addiu	$20,$20,%lo($LC11)
	addiu	$19,$19,%lo($LC12)
	addiu	$18,$18,%lo($LC13)
	li	$17,2			# 0x2
	lw	$2,16($16)
$L128:
	lhu	$7,10($16)
	lw	$5,4($16)
	lw	$6,12($16)
	sw	$2,16($sp)
	lhu	$2,8($16)
	xor	$5,$5,$17
	move	$4,$fp
	sw	$2,20($sp)
	lw	$2,20($16)
	movz	$4,$3,$5
	move	$5,$4
	sw	$2,24($sp)
	lhu	$2,24($16)
	move	$4,$23
	sw	$2,28($sp)
	lw	$2,28($16)
	sw	$2,32($sp)
	lhu	$2,32($16)
	sw	$2,36($sp)
	lw	$2,40($16)
	sw	$2,40($sp)
	lbu	$2,2($16)
	sw	$2,44($sp)
	lbu	$2,1($16)
	sw	$3,56($sp)
	jal	printk
	sw	$2,48($sp)

	lw	$6,48($16)
	lw	$5,44($16)
	jal	printk
	move	$4,$22

	lw	$5,60($16)
	jal	printk
	move	$4,$21

	lbu	$6,73($16)
	lbu	$5,72($16)
	jal	printk
	move	$4,$20

	lhu	$5,74($16)
	jal	printk
	move	$4,$19

	jal	printk
	move	$4,$18

	lw	$16,84($16)
	beq	$16,$0,$L121
	lw	$3,56($sp)

	j	$L128
	lw	$2,16($16)

	.set	macro
	.set	reorder
	.end	fastpath_table_path
	.size	fastpath_table_path, .-fastpath_table_path
	.section	.rodata.str1.4
	.align	2
$LC14:
	.ascii	"~Napt: [%s] int=0x%08X:%-5u ext=0x%08X:%-5u rem=0x%08X:%"
	.ascii	"-5u flags=0x%08X \012\000"
	.text
	.align	2
	.set	nomips16
	.ent	fastpath_table_napt
	.type	fastpath_table_napt, @function
fastpath_table_napt:
	.frame	$sp,64,$31		# vars= 0, regs= 6/0, args= 40, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$20,56($sp)
	sw	$19,52($sp)
	sw	$18,48($sp)
	sw	$17,44($sp)
	sw	$16,40($sp)
	lui	$2,%hi(napt_list_inuse)
	lw	$16,%lo(napt_list_inuse)($2)
	bne	$16,$0,$L136
	lui	$20,%hi($LC7)

	lw	$31,60($sp)
$L138:
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,64

$L136:
	lui	$19,%hi($LC6)
	lui	$18,%hi($LC14)
	addiu	$20,$20,%lo($LC7)
	addiu	$19,$19,%lo($LC6)
	addiu	$18,$18,%lo($LC14)
	li	$17,2			# 0x2
	lw	$2,16($16)
$L137:
	lw	$5,4($16)
	lw	$6,8($16)
	lw	$7,12($16)
	sw	$2,16($sp)
	lw	$2,20($16)
	xor	$5,$5,$17
	move	$3,$19
	sw	$2,20($sp)
	lw	$2,24($16)
	movz	$3,$20,$5
	move	$5,$3
	sw	$2,24($sp)
	lw	$2,28($16)
	move	$4,$18
	sw	$2,28($sp)
	lw	$2,32($16)
	jal	printk
	sw	$2,32($sp)

	lw	$16,44($16)
	beq	$16,$0,$L138
	lw	$31,60($sp)

	j	$L137
	lw	$2,16($16)

	.set	macro
	.set	reorder
	.end	fastpath_table_napt
	.size	fastpath_table_napt, .-fastpath_table_napt
	.section	.rodata.str1.4
	.align	2
$LC15:
	.ascii	"lo\000"
	.text
	.align	2
	.globl	enter_fast_path
	.set	nomips16
	.ent	enter_fast_path
	.type	enter_fast_path, @function
enter_fast_path:
	.frame	$sp,96,$31		# vars= 32, regs= 10/0, args= 24, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$fp,88($sp)
	sw	$23,84($sp)
	sw	$22,80($sp)
	sw	$21,76($sp)
	sw	$20,72($sp)
	sw	$19,68($sp)
	sw	$18,64($sp)
	sw	$17,60($sp)
	sw	$16,56($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev_name
	move	$18,$4
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$16,$2
	.set	macro
	.set	reorder

	move	$17,$2
	lbu	$2,8($2)
	sltu	$2,$2,2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L140
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	lbu	$19,9($17)
	.set	noreorder
	.set	nomacro
	beq	$19,$2,$L141
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L141
$L140:
	move	$4,$0
$L142:
	lw	$31,92($sp)
$L282:
	lw	$fp,88($sp)
	lw	$23,84($sp)
	lw	$22,80($sp)
	lw	$21,76($sp)
	lw	$20,72($sp)
	lw	$19,68($sp)
	lw	$18,64($sp)
	lw	$17,60($sp)
	lw	$16,56($sp)
	move	$2,$4
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,96
	.set	macro
	.set	reorder

$L141:
	lw	$21,0($17)
	move	$4,$17
	move	$6,$18
	andi	$21,$21,0xf
	sll	$21,$21,2
	addu	$21,$17,$21
	.set	noreorder
	.set	nomacro
	jal	fast_path_pre_process_check
	move	$5,$21
	.set	macro
	.set	reorder

	move	$4,$2
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L282
	lw	$31,92($sp)
	.set	macro
	.set	reorder

	lui	$2,%hi($LC15)
	addiu	$2,$2,%lo($LC15)
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$3,($16)
1:	lbu	$1,($2)
	addiu	$16,1
	bne	$1,$3,2f
	addiu	$2,1
	bnez	$3,1b
	lbu	$3,($16)
	move	$3,$1
2:	subu	$3,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$3,$0,$L140
	lhu	$2,6($17)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	lw	$23,12($17)
	lw	$3,16($17)
	lhu	$20,0($21)
	lhu	$fp,2($21)
	.set	noreorder
	.set	nomacro
	beq	$19,$4,$L254
	andi	$5,$2,0x3fff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L147
	li	$4,8192			# 0x2000
	.set	macro
	.set	reorder

	xor	$7,$3,$23
$L286:
	move	$22,$0
	move	$9,$0
	move	$8,$0
	li	$11,1			# 0x1
$L148:
	srl	$2,$23,8
	srl	$4,$3,16
	xor	$4,$4,$2
	xor	$4,$4,$19
	lui	$2,%hi(path_table_list_max)
	xor	$4,$4,$fp
	lw	$6,%lo(path_table_list_max)($2)
	xor	$4,$4,$7
	srl	$2,$20,4
	xor	$4,$4,$2
	lui	$2,%hi(table_path)
	lw	$5,%lo(table_path)($2)
	srl	$2,$4,12
	xor	$4,$2,$4
	addiu	$6,$6,-1
	and	$2,$4,$6
	lw	$4,0($5)
	sll	$5,$2,4
	sll	$2,$2,2
	subu	$2,$5,$2
	addu	$2,$4,$2
	lw	$16,0($2)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L163
	li	$12,2			# 0x2
	.set	macro
	.set	reorder

	li	$4,255			# 0xff
	li	$10,1			# 0x1
	li	$6,17			# 0x11
	.set	noreorder
	.set	nomacro
	j	$L236
	li	$5,6			# 0x6
	.set	macro
	.set	reorder

$L164:
	lw	$16,76($16)
	beq	$16,$0,$L163
$L236:
	lhu	$2,10($16)
	bne	$2,$20,$L164
	lhu	$2,8($16)
	bne	$2,$fp,$L164
	lw	$2,12($16)
	bne	$2,$23,$L164
	lw	$2,16($16)
	bne	$2,$3,$L164
	lbu	$2,0($16)
	bne	$2,$4,$L164
	lw	$2,4($16)
	beq	$2,$12,$L255
	bne	$2,$10,$L164
	.set	noreorder
	.set	nomacro
	bne	$19,$6,$L164
	lui	$2,%hi(rtl_check_bypass_packet_hook)
	.set	macro
	.set	reorder

	lw	$2,%lo(rtl_check_bypass_packet_hook)($2)
$L290:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L283
	addiu	$5,$16,60
	.set	macro
	.set	reorder

	sw	$3,48($sp)
	sw	$7,32($sp)
	sw	$8,40($sp)
	sw	$9,36($sp)
	sw	$11,44($sp)
	.set	noreorder
	.set	nomacro
	jalr	$2
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,48($sp)
	lw	$7,32($sp)
	lw	$8,40($sp)
	lw	$9,36($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L140
	lw	$11,44($sp)
	.set	macro
	.set	reorder

	addiu	$5,$16,60
$L283:
	move	$4,$18
	sw	$3,48($sp)
	sw	$7,32($sp)
	sw	$8,40($sp)
	sw	$9,36($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_recheck_fp_policy_route_mark
	sw	$11,44($sp)
	.set	macro
	.set	reorder

	lw	$5,60($16)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_policy_route_mark_set
	addiu	$6,$sp,24
	.set	macro
	.set	reorder

	lw	$5,28($16)
	lw	$6,52($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_route_input
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$3,48($sp)
	lw	$7,32($sp)
	lw	$8,40($sp)
	lw	$9,36($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L256
	lw	$11,44($sp)
	.set	macro
	.set	reorder

	lw	$5,24($sp)
	move	$6,$0
	move	$4,$18
	sw	$3,48($sp)
	sw	$7,32($sp)
	sw	$8,40($sp)
	sw	$9,36($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_policy_route_mark_set
	sw	$11,44($sp)
	.set	macro
	.set	reorder

	lw	$5,28($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_dst_check
	move	$4,$18
	.set	macro
	.set	reorder

	li	$4,-1			# 0xffffffffffffffff
	lw	$3,48($sp)
	lw	$7,32($sp)
	lw	$8,40($sp)
	lw	$9,36($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L174
	lw	$11,44($sp)
	.set	macro
	.set	reorder

	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$19,$2,$L171
	addiu	$10,$21,16
	.set	macro
	.set	reorder

	lhu	$10,6($21)
	addiu	$2,$21,6
	movz	$2,$0,$10
	move	$10,$2
$L171:
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L173
	lui	$4,%hi(udp_cache_list_free)
	.set	macro
	.set	reorder

	lw	$2,%lo(udp_cache_list_free)($4)
	lui	$5,%hi(max_udp_frag_entry)
	lw	$8,%lo(max_udp_frag_entry)($5)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L174
	lhu	$5,4($17)
	.set	macro
	.set	reorder

	lw	$6,52($2)
	li	$9,1			# 0x1
	sh	$20,12($2)
	sh	$fp,14($2)
	sb	$9,0($2)
	sh	$5,2($2)
	sw	$23,4($2)
	sw	$3,8($2)
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L175
	sb	$19,1($2)
	.set	macro
	.set	reorder

	lw	$9,56($2)
	addiu	$4,$4,%lo(udp_cache_list_free)
	sw	$9,56($6)
$L176:
	lw	$6,56($2)
	lw	$12,52($2)
	xor	$9,$7,$5
	srl	$7,$7,16
	sw	$12,0($6)
	lw	$5,8($4)
	lui	$6,%hi(udp_cache_table)
	xor	$7,$9,$7
	addiu	$5,$5,-1
	sw	$5,8($4)
	sw	$0,44($2)
	lw	$5,%lo(udp_cache_table)($6)
	srl	$4,$7,8
	xor	$7,$7,$19
	xor	$7,$7,$4
	addiu	$4,$8,-1
	and	$4,$7,$4
	lw	$7,0($5)
	sll	$5,$4,4
	sll	$4,$4,2
	subu	$5,$5,$4
	addu	$4,$7,$5
	lw	$4,4($4)
	addiu	$8,$2,44
	sw	$4,48($2)
	lw	$7,%lo(udp_cache_table)($6)
	addiu	$4,$2,16
	lw	$7,0($7)
	addu	$7,$7,$5
	lw	$7,4($7)
	sw	$2,0($7)
	lw	$7,%lo(udp_cache_table)($6)
	lw	$7,0($7)
	addu	$7,$7,$5
	sw	$8,4($7)
	lw	$6,%lo(udp_cache_table)($6)
	lw	$6,0($6)
	addu	$5,$6,$5
	lw	$6,8($5)
	addiu	$6,$6,1
	sw	$6,8($5)
	lui	$5,%hi(jiffies)
	lw	$5,%lo(jiffies)($5)
	addiu	$5,$5,1000
	sw	$5,24($2)
	sw	$3,48($sp)
	sw	$10,32($sp)
	.set	noreorder
	.set	nomacro
	jal	add_timer
	sw	$11,44($sp)
	.set	macro
	.set	reorder

	lw	$3,48($sp)
	lw	$10,32($sp)
	lw	$11,44($sp)
$L177:
	move	$4,$18
$L287:
	move	$5,$17
	move	$6,$19
	sw	$3,48($sp)
	sw	$10,32($sp)
	.set	noreorder
	.set	nomacro
	jal	fast_path_before_nat_check
	sw	$11,44($sp)
	.set	macro
	.set	reorder

	move	$4,$2
	li	$2,1			# 0x1
	lw	$3,48($sp)
	lw	$10,32($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L142
	lw	$11,44($sp)
	.set	macro
	.set	reorder

	li	$2,6			# 0x6
	beq	$19,$2,$L257
$L180:
	lbu	$2,1($16)
	sltu	$4,$2,13
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L181
	lui	$4,%hi($L187)
	.set	macro
	.set	reorder

	sll	$2,$2,2
	addiu	$4,$4,%lo($L187)
	addu	$2,$4,$2
	lw	$2,0($2)
	j	$2
	.rdata
	.align	2
	.align	2
$L187:
	.word	$L182
	.word	$L183
	.word	$L184
	.word	$L184
	.word	$L185
	.word	$L181
	.word	$L181
	.word	$L181
	.word	$L186
	.word	$L181
	.word	$L181
	.word	$L181
	.word	$L186
	.text
$L147:
$L280:
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L258
	move	$9,$0
	.set	macro
	.set	reorder

	andi	$4,$2,0x2000
$L285:
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L284
	move	$9,$0
	.set	macro
	.set	reorder

	andi	$2,$2,0x1fff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L149
	move	$8,$0
	.set	macro
	.set	reorder

	li	$9,1			# 0x1
$L149:
	lhu	$6,4($17)
	xor	$7,$3,$23
	lui	$2,%hi(max_udp_frag_entry)
	lbu	$10,9($17)
	lw	$5,%lo(max_udp_frag_entry)($2)
	xor	$4,$6,$7
	srl	$2,$7,16
	xor	$2,$4,$2
	xor	$4,$2,$10
	srl	$2,$2,8
	xor	$4,$4,$2
	addiu	$5,$5,-1
	lui	$2,%hi(negative_cache_table)
	and	$4,$4,$5
	lw	$5,%lo(negative_cache_table)($2)
	sll	$12,$4,4
	sll	$11,$4,2
	subu	$4,$12,$11
	addu	$4,$5,$4
	lw	$4,0($4)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L151
	li	$13,3			# 0x3
	.set	macro
	.set	reorder

	j	$L238
$L152:
	lw	$4,40($4)
	beq	$4,$0,$L151
$L238:
	lhu	$5,2($4)
	bne	$5,$6,$L152
	lw	$5,4($4)
	bne	$23,$5,$L152
	lw	$5,8($4)
	bne	$3,$5,$L152
	lbu	$5,1($4)
	bne	$5,$10,$L152
	lbu	$5,0($4)
	bne	$5,$13,$L152
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L142
	move	$4,$0
	.set	macro
	.set	reorder

$L151:
	lui	$4,%hi(udp_cache_table)
	lw	$5,%lo(udp_cache_table)($4)
	subu	$4,$12,$11
	lw	$5,0($5)
	addu	$4,$5,$4
	lw	$22,0($4)
	.set	noreorder
	.set	nomacro
	beq	$22,$0,$L155
	li	$13,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L289
	lhu	$4,2($22)
	.set	macro
	.set	reorder

$L156:
	lw	$22,44($22)
	beq	$22,$0,$L155
	lhu	$4,2($22)
$L289:
	bne	$4,$6,$L156
	lw	$5,4($22)
	bne	$23,$5,$L156
	lw	$4,8($22)
	bne	$3,$4,$L156
	lbu	$4,1($22)
	bne	$4,$10,$L156
	lbu	$4,0($22)
	bne	$4,$13,$L156
	beq	$22,$0,$L155
	lhu	$20,12($22)
	lhu	$fp,14($22)
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L148
	move	$11,$0
	.set	macro
	.set	reorder

$L243:
	li	$8,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L148
	li	$11,1			# 0x1
	.set	macro
	.set	reorder

$L254:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L145
	li	$4,8192			# 0x2000
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$4,$L285
	andi	$4,$2,0x2000
	.set	macro
	.set	reorder

$L145:
	lw	$4,12($21)
	andi	$6,$4,0x100
	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L259
	move	$7,$fp
	.set	macro
	.set	reorder

	andi	$4,$4,0x600
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L142
	move	$4,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L286
	xor	$7,$3,$23
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L280
	li	$4,8192			# 0x2000
	.set	macro
	.set	reorder

$L163:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_inDev
	move	$4,$18
	.set	macro
	.set	reorder

	move	$4,$17
	move	$5,$21
	.set	noreorder
	.set	nomacro
	jal	fast_path_post_process_return_check
	move	$6,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L142
	move	$4,$2
	.set	macro
	.set	reorder

$L284:
	.set	noreorder
	.set	nomacro
	j	$L149
	move	$8,$0
	.set	macro
	.set	reorder

$L258:
	.set	noreorder
	.set	nomacro
	j	$L149
	li	$8,1			# 0x1
	.set	macro
	.set	reorder

$L155:
	.set	noreorder
	.set	nomacro
	bne	$8,$0,$L243
	move	$22,$0
	.set	macro
	.set	reorder

	lui	$4,%hi(negative_cache_list_free)
	lw	$5,%lo(negative_cache_list_free)($4)
	beq	$5,$0,$L140
	lw	$7,48($5)
	sw	$3,8($5)
	li	$3,3			# 0x3
	sh	$6,2($5)
	sw	$23,4($5)
	sb	$10,1($5)
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L161
	sb	$3,0($5)
	.set	macro
	.set	reorder

	lw	$6,52($5)
	addiu	$3,$4,%lo(negative_cache_list_free)
	sw	$6,52($7)
$L162:
	lw	$4,52($5)
	lw	$7,48($5)
	subu	$11,$12,$11
	addiu	$6,$5,40
	sw	$7,0($4)
	lw	$7,8($3)
	addiu	$4,$5,12
	addiu	$7,$7,-1
	sw	$7,8($3)
	sw	$0,40($5)
	lw	$3,%lo(negative_cache_table)($2)
	addu	$3,$3,$11
	lw	$3,4($3)
	sw	$3,44($5)
	lw	$3,%lo(negative_cache_table)($2)
	addu	$3,$3,$11
	lw	$3,4($3)
	sw	$5,0($3)
	lw	$3,%lo(negative_cache_table)($2)
	addu	$3,$3,$11
	sw	$6,4($3)
	lw	$2,%lo(negative_cache_table)($2)
	addu	$11,$2,$11
	lw	$2,8($11)
	addiu	$2,$2,1
	sw	$2,8($11)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,1000
	.set	noreorder
	.set	nomacro
	jal	add_timer
	sw	$2,20($5)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L142
	move	$4,$0
	.set	macro
	.set	reorder

$L259:
	move	$4,$23
	li	$2,6			# 0x6
	move	$5,$20
	move	$6,$3
	.set	noreorder
	.set	nomacro
	jal	mark_path_invalid
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L142
	move	$4,$0
	.set	macro
	.set	reorder

$L255:
	.set	noreorder
	.set	nomacro
	bne	$19,$5,$L164
	lui	$2,%hi(rtl_check_bypass_packet_hook)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L290
	lw	$2,%lo(rtl_check_bypass_packet_hook)($2)
	.set	macro
	.set	reorder

$L161:
	lw	$6,52($5)
	addiu	$3,$4,%lo(negative_cache_list_free)
	.set	noreorder
	.set	nomacro
	j	$L162
	sw	$6,4($3)
	.set	macro
	.set	reorder

$L186:
	lw	$2,28($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	srl	$4,$2,16
	subu	$4,$0,$4
	andi	$2,$2,0xffff
	subu	$2,$4,$2
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $3
	rotr	$4, $4, 16

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	addu	$2,$2,$5
	andi	$4,$4,0xffff
	addu	$4,$2,$4
	lhu	$2,10($17)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L261
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$2,10($17)
$L210:
	.set	noreorder
	.set	nomacro
	beq	$11,$0,$L247
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L262
	lw	$2,28($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	srl	$6,$2,16
	subu	$6,$0,$6
	andi	$2,$2,0xffff
	subu	$6,$6,$2
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3
	rotr	$3, $3, 16

 # 0 "" 2
 #NO_APP
	lhu	$4,2($21)
	srl	$5,$3,16
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	lhu	$2,0($10)
	andi	$4,$4,0xffff
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$5,$6,$5
	addu	$2,$2,$4
$L249:
	andi	$3,$3,0xffff
	lhu	$4,32($16)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$3,$5,$3
	subu	$2,$2,$4
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L263
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$2,0($10)
$L213:
	lhu	$2,32($16)
	sh	$2,2($21)
$L247:
	lw	$2,28($16)
	sw	$2,16($17)
$L182:
	lbu	$2,8($17)
	lbu	$4,9($17)
	lhu	$5,10($17)
	addiu	$3,$2,-1
	andi	$3,$3,0x00ff
	sll	$2,$2,8
	sll	$6,$3,8
	or	$6,$6,$4
	or	$4,$2,$4
	subu	$4,$4,$6
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $5

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L264
	sb	$3,8($17)
	.set	macro
	.set	reorder

	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$2,10($17)
$L230:
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ip_summed
	move	$4,$18
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dev
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$6,48($16)
	lw	$5,44($16)
	.set	noreorder
	.set	nomacro
	jal	fastpath_set_qos_mark
	move	$4,$18
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_set_iph_dscp
	move	$5,$17
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_remark_vlanpri
	move	$5,$18
	.set	macro
	.set	reorder

	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	move	$4,$17
	move	$5,$21
	sw	$2,36($16)
	.set	noreorder
	.set	nomacro
	jal	fast_path_post_process_xmit_check
	move	$6,$18
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L142
	li	$4,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	ip_finish_output3
	move	$4,$18
	.set	macro
	.set	reorder

	lui	$2,%hi(statistic_fp)
	lw	$3,%lo(statistic_fp)($2)
	li	$4,1			# 0x1
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	j	$L142
	sw	$3,%lo(statistic_fp)($2)
	.set	macro
	.set	reorder

$L261:
	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L210
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L264:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L230
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L174:
	.set	noreorder
	.set	nomacro
	jal	rtl_dst_release_ipv4
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L142
	move	$4,$0
	.set	macro
	.set	reorder

$L256:
	lw	$5,24($sp)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_policy_route_mark_set
	move	$6,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L142
	move	$4,$0
	.set	macro
	.set	reorder

$L263:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L213
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L262:
	beq	$10,$0,$L213
	lhu	$2,0($10)
	beq	$2,$0,$L213
	lw	$4,28($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4
	rotr	$4, $4, 16

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	subu	$5,$0,$5
	andi	$4,$4,0xffff
	subu	$4,$5,$4
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3
	rotr	$3, $3, 16

 # 0 "" 2
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	srl	$5,$3,16
	andi	$2,$2,0xffff
	lhu	$6,2($21)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$6, $6

 # 0 "" 2
 #NO_APP
	andi	$6,$6,0xffff
	addu	$5,$4,$5
	.set	noreorder
	.set	nomacro
	j	$L249
	addu	$2,$2,$6
	.set	macro
	.set	reorder

$L185:
	lw	$2,28($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	srl	$4,$2,16
	subu	$4,$0,$4
	andi	$2,$2,0xffff
	subu	$2,$4,$2
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $3
	rotr	$4, $4, 16

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	addu	$2,$2,$5
	andi	$4,$4,0xffff
	addu	$4,$2,$4
	lhu	$2,10($17)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L265
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$2,10($17)
$L204:
	.set	noreorder
	.set	nomacro
	beq	$11,$0,$L247
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L266
	lw	$2,28($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	srl	$4,$2,16
	subu	$4,$0,$4
	andi	$2,$2,0xffff
	subu	$2,$4,$2
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3
	rotr	$3, $3, 16

 # 0 "" 2
 #NO_APP
	srl	$4,$3,16
	addu	$2,$2,$4
	andi	$3,$3,0xffff
	addu	$3,$2,$3
	lhu	$2,0($10)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L244
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

$L288:
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L247
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L183:
	lw	$2,20($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	srl	$3,$2,16
	subu	$3,$0,$3
	andi	$2,$2,0xffff
	subu	$2,$3,$2
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$23, $23
	rotr	$23, $23, 16

 # 0 "" 2
 #NO_APP
	srl	$3,$23,16
	addu	$2,$2,$3
	andi	$23,$23,0xffff
	addu	$23,$2,$23
	lhu	$2,10($17)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L267
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$2,10($17)
$L189:
	.set	noreorder
	.set	nomacro
	beq	$11,$0,$L197
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L268
	lw	$2,20($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	srl	$3,$2,16
	subu	$3,$0,$3
	andi	$2,$2,0xffff
	subu	$2,$3,$2
	lw	$3,12($17)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3
	rotr	$3, $3, 16

 # 0 "" 2
 #NO_APP
	srl	$4,$3,16
	addu	$2,$2,$4
	andi	$3,$3,0xffff
	addu	$3,$2,$3
	lhu	$2,0($10)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L269
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$2,0($10)
$L194:
	lw	$2,12($21)
	andi	$2,$2,0x1000
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L197
	li	$2,10240			# 0x2800
	.set	macro
	.set	reorder

	lhu	$3,2($17)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L197
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,10240			# 0x2800
	.set	macro
	.set	reorder

$L197:
	lw	$2,20($16)
	.set	noreorder
	.set	nomacro
	j	$L182
	sw	$2,12($17)
	.set	macro
	.set	reorder

$L267:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L189
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L269:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L194
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L268:
	beq	$10,$0,$L197
	lhu	$3,0($10)
	beq	$3,$0,$L197
	lw	$2,20($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	srl	$4,$2,16
	subu	$4,$0,$4
	andi	$2,$2,0xffff
	subu	$2,$4,$2
	lw	$4,12($17)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4
	rotr	$4, $4, 16

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	addu	$2,$2,$5
	andi	$4,$4,0xffff
	addu	$2,$2,$4
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3

 # 0 "" 2
 #NO_APP
	andi	$3,$3,0xffff
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L270
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L197
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L184:
	lw	$2,20($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	srl	$3,$2,16
	subu	$3,$0,$3
	andi	$2,$2,0xffff
	subu	$2,$3,$2
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$23, $23
	rotr	$23, $23, 16

 # 0 "" 2
 #NO_APP
	srl	$3,$23,16
	addu	$2,$2,$3
	andi	$23,$23,0xffff
	addu	$23,$2,$23
	lhu	$2,10($17)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L271
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$2,10($17)
$L196:
	.set	noreorder
	.set	nomacro
	beq	$11,$0,$L197
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L272
	lw	$2,20($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	srl	$6,$2,16
	subu	$6,$0,$6
	andi	$2,$2,0xffff
	subu	$6,$6,$2
	lw	$3,12($17)
	lhu	$4,0($21)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3
	rotr	$3, $3, 16

 # 0 "" 2
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	srl	$5,$3,16
	andi	$4,$4,0xffff
	lhu	$2,0($10)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$2,$4
	addu	$5,$6,$5
	andi	$3,$3,0xffff
	lhu	$4,24($16)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$3,$5,$3
	subu	$2,$2,$4
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L273
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$2,0($10)
$L202:
	lw	$2,12($21)
	andi	$2,$2,0x1000
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L199
	li	$2,10240			# 0x2800
	.set	macro
	.set	reorder

	lhu	$3,2($17)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L199
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,10240			# 0x2800
	.set	macro
	.set	reorder

$L199:
	lhu	$2,24($16)
	.set	noreorder
	.set	nomacro
	j	$L197
	sh	$2,0($21)
	.set	macro
	.set	reorder

$L273:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L202
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L272:
	beq	$10,$0,$L199
	lhu	$2,0($10)
	beq	$2,$0,$L199
	lw	$3,20($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3
	rotr	$3, $3, 16

 # 0 "" 2
 #NO_APP
	srl	$6,$3,16
	subu	$6,$0,$6
	andi	$3,$3,0xffff
	subu	$6,$6,$3
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	lw	$3,12($17)
	andi	$2,$2,0xffff
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3
	rotr	$3, $3, 16

 # 0 "" 2
 #NO_APP
	lhu	$4,0($21)
	srl	$5,$3,16
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$2,$2,$4
	addu	$5,$6,$5
	andi	$3,$3,0xffff
	lhu	$4,24($16)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$3,$5,$3
	subu	$2,$2,$4
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L274
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L199
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L271:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L196
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L274:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L199
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L181:
	lw	$2,20($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	srl	$4,$2,16
	subu	$4,$0,$4
	andi	$2,$2,0xffff
	subu	$2,$4,$2
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $23
	rotr	$4, $4, 16

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	addu	$2,$2,$5
	andi	$4,$4,0xffff
	addu	$4,$2,$4
	lhu	$2,10($17)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$4,$2
	bltz	$2,$L275
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$4,$4,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	sh	$4,10($17)
$L217:
	lw	$2,28($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	srl	$5,$2,16
	subu	$5,$0,$5
	andi	$2,$2,0xffff
	subu	$2,$5,$2
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $3
	rotr	$5, $5, 16

 # 0 "" 2
 #NO_APP
	srl	$6,$5,16
	addu	$2,$2,$6
	andi	$5,$5,0xffff
	addu	$2,$2,$5
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bltz	$2,$L276
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$2,10($17)
$L219:
	.set	noreorder
	.set	nomacro
	beq	$11,$0,$L220
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L277
	lw	$2,20($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	srl	$6,$2,16
	subu	$6,$0,$6
	andi	$2,$2,0xffff
	subu	$6,$6,$2
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$23, $23
	rotr	$23, $23, 16

 # 0 "" 2
 #NO_APP
	lhu	$4,0($21)
	srl	$5,$23,16
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	lhu	$2,0($10)
	andi	$4,$4,0xffff
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$5,$6,$5
	addu	$2,$2,$4
$L253:
	andi	$23,$23,0xffff
	lhu	$4,24($16)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$23,$5,$23
	subu	$2,$2,$4
	addu	$2,$2,$23
	.set	noreorder
	.set	nomacro
	bltz	$2,$L278
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	sh	$2,0($10)
$L227:
	lw	$4,28($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4
	rotr	$4, $4, 16

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	subu	$5,$0,$5
	andi	$4,$4,0xffff
	subu	$4,$5,$4
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3
	rotr	$3, $3, 16

 # 0 "" 2
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	srl	$5,$3,16
	andi	$2,$2,0xffff
	lhu	$6,2($21)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$6, $6

 # 0 "" 2
 #NO_APP
	andi	$6,$6,0xffff
	addu	$5,$4,$5
	addu	$2,$2,$6
	andi	$3,$3,0xffff
	lhu	$4,32($16)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$3,$5,$3
	subu	$2,$2,$4
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L279
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$2,0($10)
$L222:
	lhu	$2,24($16)
	sh	$2,0($21)
	lhu	$2,32($16)
	sh	$2,2($21)
$L220:
	lw	$2,20($16)
	.set	noreorder
	.set	nomacro
	j	$L247
	sw	$2,12($17)
	.set	macro
	.set	reorder

$L276:
	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L219
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L275:
	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$4,$4,$2
	nor	$4,$0,$4
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	.set	noreorder
	.set	nomacro
	j	$L217
	sh	$4,10($17)
	.set	macro
	.set	reorder

$L175:
	lw	$6,56($2)
	addiu	$4,$4,%lo(udp_cache_list_free)
	.set	noreorder
	.set	nomacro
	j	$L176
	sw	$6,4($4)
	.set	macro
	.set	reorder

$L173:
	.set	noreorder
	.set	nomacro
	beq	$9,$0,$L287
	move	$4,$18
	.set	macro
	.set	reorder

	addiu	$4,$22,16
	sw	$3,48($sp)
	sw	$10,32($sp)
	.set	noreorder
	.set	nomacro
	jal	del_timer
	sw	$11,44($sp)
	.set	macro
	.set	reorder

	lw	$2,4($22)
	lw	$4,8($22)
	lhu	$5,2($22)
	lbu	$6,1($22)
	xor	$4,$4,$2
	lui	$2,%hi(max_udp_frag_entry)
	xor	$5,$5,$4
	lw	$2,%lo(max_udp_frag_entry)($2)
	srl	$4,$4,16
	xor	$5,$5,$4
	lw	$4,44($22)
	xor	$6,$5,$6
	srl	$5,$5,8
	sb	$0,0($22)
	xor	$5,$6,$5
	addiu	$2,$2,-1
	and	$2,$5,$2
	lw	$3,48($sp)
	lw	$10,32($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L178
	lw	$11,44($sp)
	.set	macro
	.set	reorder

	lw	$6,48($22)
	sll	$5,$2,4
	sll	$2,$2,2
	sw	$6,48($4)
	subu	$5,$5,$2
	lui	$4,%hi(udp_cache_table)
$L179:
	lw	$6,48($22)
	lw	$7,44($22)
	lui	$2,%hi(udp_cache_list_free)
	addiu	$2,$2,%lo(udp_cache_list_free)
	sw	$7,0($6)
	lw	$4,%lo(udp_cache_table)($4)
	addiu	$6,$22,52
	lw	$4,0($4)
	addu	$5,$4,$5
	lw	$4,8($5)
	addiu	$4,$4,-1
	sw	$4,8($5)
	sw	$0,52($22)
	lw	$4,4($2)
	sw	$4,56($22)
	lw	$4,4($2)
	sw	$22,0($4)
	lw	$4,8($2)
	sw	$6,4($2)
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	j	$L177
	sw	$4,8($2)
	.set	macro
	.set	reorder

$L266:
	beq	$10,$0,$L247
	lhu	$4,0($10)
	beq	$4,$0,$L247
	lw	$2,28($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	srl	$5,$2,16
	subu	$5,$0,$5
	andi	$2,$2,0xffff
	subu	$2,$5,$2
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3
	rotr	$3, $3, 16

 # 0 "" 2
 #NO_APP
	srl	$5,$3,16
	addu	$2,$2,$5
	andi	$3,$3,0xffff
	addu	$2,$2,$3
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bgez	$2,$L288
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

$L244:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L247
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L270:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L197
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L265:
	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L204
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L178:
	lui	$4,%hi(udp_cache_table)
	lw	$6,%lo(udp_cache_table)($4)
	sll	$7,$2,4
	sll	$5,$2,2
	lw	$2,0($6)
	lw	$6,48($22)
	subu	$5,$7,$5
	addu	$2,$2,$5
	.set	noreorder
	.set	nomacro
	j	$L179
	sw	$6,4($2)
	.set	macro
	.set	reorder

$L257:
	lw	$2,12($21)
	andi	$2,$2,0x1000
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L180
	li	$2,40			# 0x28
	.set	macro
	.set	reorder

	lhu	$4,2($17)
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L180
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,40			# 0x28
	.set	macro
	.set	reorder

	lw	$11,44($sp)
	lw	$10,32($sp)
	.set	noreorder
	.set	nomacro
	j	$L180
	lw	$3,48($sp)
	.set	macro
	.set	reorder

$L279:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L222
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L278:
	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	.set	noreorder
	.set	nomacro
	j	$L227
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L277:
	beq	$10,$0,$L222
	lhu	$2,0($10)
	beq	$2,$0,$L222
	lw	$4,20($16)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4
	rotr	$4, $4, 16

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	subu	$5,$0,$5
	andi	$4,$4,0xffff
	subu	$4,$5,$4
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$23, $23
	rotr	$23, $23, 16

 # 0 "" 2
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	srl	$5,$23,16
	andi	$2,$2,0xffff
	lhu	$6,0($21)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$6, $6

 # 0 "" 2
 #NO_APP
	andi	$6,$6,0xffff
	addu	$5,$4,$5
	.set	noreorder
	.set	nomacro
	j	$L253
	addu	$2,$2,$6
	.set	macro
	.set	reorder

	.end	enter_fast_path
	.size	enter_fast_path, .-enter_fast_path
	.align	2
	.globl	rtl_br_fdb_time_update
	.set	nomips16
	.ent	rtl_br_fdb_time_update
	.type	rtl_br_fdb_time_update, @function
rtl_br_fdb_time_update:
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
	move	$17,$4
	move	$16,$5
	move	$4,$0
	move	$5,$6
	jal	rtl_get_hw_fdb_age
	li	$6,2			# 0x2

	li	$3,300			# 0x12c
	beq	$2,$3,$L294
	li	$3,450			# 0x1c2

	beq	$2,$3,$L295
	li	$3,150			# 0x96

	beq	$2,$3,$L293
	lui	$18,%hi(jiffies)

$L292:
	lw	$18,%lo(jiffies)($18)
	jal	rtl_get_fdb_aging
	move	$4,$16

	move	$16,$2
	jal	rtl_hold_time
	move	$4,$17

	subu	$16,$18,$16
	subu	$2,$16,$2
	slt	$2,$2,0
	li	$3,-1			# 0xffffffffffffffff
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	movn	$3,$0,$2
	move	$2,$3
	j	$31
	addiu	$sp,$sp,32

$L293:
	lw	$5,%lo(jiffies)($18)
	move	$4,$16
	jal	rtl_set_fdb_aging
	addiu	$5,$5,-30000

	j	$L292
	nop

$L295:
	lui	$18,%hi(jiffies)
	lw	$5,%lo(jiffies)($18)
	jal	rtl_set_fdb_aging
	move	$4,$16

	j	$L292
	nop

$L294:
	lui	$18,%hi(jiffies)
	lw	$5,%lo(jiffies)($18)
	move	$4,$16
	jal	rtl_set_fdb_aging
	addiu	$5,$5,-15000

	j	$L292
	nop

	.set	macro
	.set	reorder
	.end	rtl_br_fdb_time_update
	.size	rtl_br_fdb_time_update, .-rtl_br_fdb_time_update
	.align	2
	.globl	rtk_idleNaptConnection
	.set	nomips16
	.ent	rtk_idleNaptConnection
	.type	rtk_idleNaptConnection, @function
rtk_idleNaptConnection:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lw	$8,4($4)
	lw	$6,20($4)
	lw	$9,24($4)
	lw	$12,8($4)
	xor	$2,$6,$8
	srl	$7,$8,8
	lw	$3,0($4)
	xor	$2,$2,$9
	xor	$2,$2,$7
	srl	$7,$6,16
	xor	$2,$2,$7
	srl	$10,$12,4
	xori	$13,$3,0x2
	li	$11,6			# 0x6
	xor	$2,$2,$10
	li	$7,17			# 0x11
	lui	$10,%hi(path_table_list_max)
	movz	$7,$11,$13
	lw	$11,%lo(path_table_list_max)($10)
	lui	$10,%hi(table_path)
	xor	$2,$2,$7
	lw	$10,%lo(table_path)($10)
	srl	$13,$2,12
	xor	$2,$13,$2
	addiu	$11,$11,-1
	and	$2,$2,$11
	lw	$10,0($10)
	sll	$13,$2,4
	sll	$2,$2,2
	subu	$2,$13,$2
	addu	$2,$10,$2
	lw	$2,0($2)
	lui	$13,%hi(jiffies)
	lw	$24,%lo(jiffies)($13)
	beq	$2,$0,$L314
	li	$14,16711680			# 0xff0000

	li	$13,65536			# 0x10000
	lw	$25,16($4)
	lw	$15,12($4)
	ori	$14,$14,0xff
	j	$L306
	ori	$13,$13,0xff

$L304:
	lw	$2,76($2)
	beq	$2,$0,$L315
	srl	$4,$6,8

$L306:
	lw	$4,4($2)
	bne	$3,$4,$L304
	nop

	lw	$4,0($2)
	and	$4,$4,$14
	bne	$4,$13,$L304
	nop

	lw	$4,12($2)
	bne	$8,$4,$L304
	nop

	lhu	$4,10($2)
	bne	$12,$4,$L304
	nop

	lw	$4,20($2)
	bne	$4,$15,$L304
	nop

	lhu	$4,24($2)
	bne	$4,$25,$L304
	nop

	lw	$4,16($2)
	bne	$6,$4,$L304
	nop

	lhu	$4,8($2)
	bne	$9,$4,$L304
	subu	$4,$24,$5

	lw	$13,36($2)
	subu	$4,$4,$13
	bgez	$4,$L315
	srl	$4,$6,8

	lw	$4,52($2)
	jal	rtl_set_ct_timeout_expires
	addu	$5,$5,$13

	j	$L305
	li	$2,-1			# 0xffffffffffffffff

$L314:
	lw	$25,16($4)
	lw	$15,12($4)
	srl	$4,$6,8
$L315:
	srl	$2,$9,4
	xor	$2,$4,$2
	xor	$2,$2,$6
	xor	$2,$2,$7
	xor	$2,$2,$25
	srl	$4,$15,16
	xor	$2,$2,$15
	xor	$2,$2,$4
	srl	$4,$2,12
	xor	$2,$4,$2
	and	$11,$2,$11
	sll	$2,$11,4
	sll	$11,$11,2
	subu	$11,$2,$11
	addu	$10,$10,$11
	lw	$4,0($10)
	beq	$4,$0,$L307
	li	$10,16711680			# 0xff0000

	li	$7,131072			# 0x20000
	ori	$10,$10,0xff
	j	$L311
	ori	$7,$7,0xff

$L308:
	lw	$4,76($4)
	beq	$4,$0,$L305
	move	$2,$0

$L311:
	lw	$2,4($4)
	bne	$3,$2,$L308
	nop

	lw	$2,0($4)
	and	$2,$2,$10
	bne	$2,$7,$L308
	nop

	lw	$2,16($4)
	bne	$15,$2,$L308
	nop

	lhu	$2,8($4)
	bne	$25,$2,$L308
	nop

	lw	$2,20($4)
	bne	$6,$2,$L308
	nop

	lhu	$2,24($4)
	bne	$9,$2,$L308
	nop

	lw	$2,28($4)
	bne	$8,$2,$L308
	nop

	lhu	$2,32($4)
	bne	$12,$2,$L308
	nop

	lw	$2,36($4)
	subu	$24,$24,$5
	subu	$24,$24,$2
	bgez	$24,$L307
	nop

	lw	$4,52($4)
	jal	rtl_set_ct_timeout_expires
	addu	$5,$5,$2

	j	$L305
	li	$2,-1			# 0xffffffffffffffff

$L307:
	move	$2,$0
$L305:
	lw	$31,20($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	rtk_idleNaptConnection
	.size	rtk_idleNaptConnection, .-rtk_idleNaptConnection
	.align	2
	.globl	rtl_fpTimer_update
	.set	nomips16
	.ent	rtl_fpTimer_update
	.type	rtl_fpTimer_update, @function
rtl_fpTimer_update:
	.frame	$sp,72,$31		# vars= 32, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$20,64($sp)
	sw	$19,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	sw	$16,48($sp)
	lui	$2,%hi(jiffies)
	move	$5,$0
	lw	$17,%lo(jiffies)($2)
	jal	rtl_get_ct_protonum
	move	$16,$4

	li	$3,17			# 0x11
	beq	$2,$3,$L327
	nop

	move	$4,$16
	jal	rtl_get_ct_protonum
	move	$5,$0

	li	$3,6			# 0x6
	beq	$2,$3,$L328
	nop

$L320:
	li	$2,-1			# 0xffffffffffffffff
$L323:
	lw	$31,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	j	$31
	addiu	$sp,$sp,72

$L327:
	jal	rtl_get_ct_udp_status
	move	$4,$16

	andi	$2,$2,0x2
	beq	$2,$0,$L329
	li	$4,1			# 0x1

$L325:
	move	$5,$16
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1

	move	$19,$2
	li	$20,1			# 0x1
$L319:
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	move	$4,$16

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	move	$18,$2

	beq	$18,$2,$L330
	move	$5,$0

	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	move	$4,$16

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	move	$18,$2

	bne	$18,$2,$L320
	move	$4,$16

	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$20,16($sp)

	move	$4,$16
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_port_by_dir
	sw	$2,20($sp)

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	sw	$2,24($sp)

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_get_ct_port_by_dir
	sw	$2,28($sp)

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$2,32($sp)

	move	$4,$16
	j	$L326
	li	$5,1			# 0x1

$L329:
	j	$L325
	move	$4,$0

$L330:
	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$20,16($sp)

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_port_by_dir
	sw	$2,20($sp)

	move	$4,$16
	move	$5,$0
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	sw	$2,24($sp)

	move	$4,$16
	move	$5,$0
	li	$6,1			# 0x1
	jal	rtl_get_ct_port_by_dir
	sw	$2,28($sp)

	move	$4,$16
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$2,32($sp)

	move	$4,$16
	move	$5,$0
$L326:
	move	$6,$0
	jal	rtl_get_ct_port_by_dir
	sw	$2,36($sp)

	addiu	$4,$sp,16
	move	$5,$19
	jal	rtk_idleNaptConnection
	sw	$2,40($sp)

	beq	$2,$0,$L320
	nop

	addu	$5,$19,$17
	jal	rtl_check_for_acc
	move	$4,$16

	j	$L323
	move	$2,$0

$L328:
	jal	rtl_get_ct_tcp_state
	move	$4,$16

	sltu	$2,$2,6
	beq	$2,$0,$L323
	li	$2,-1			# 0xffffffffffffffff

	jal	rtl_tcp_get_timeouts
	move	$4,$16

	move	$19,$2
	j	$L319
	li	$20,2			# 0x2

	.set	macro
	.set	reorder
	.end	rtl_fpTimer_update
	.size	rtl_fpTimer_update, .-rtl_fpTimer_update
	.align	2
	.globl	rtk_delNaptConnection
	.set	nomips16
	.ent	rtk_delNaptConnection
	.type	rtk_delNaptConnection, @function
rtk_delNaptConnection:
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lw	$3,0($4)
	li	$2,6			# 0x6
	li	$18,17			# 0x11
	xori	$3,$3,0x2
	move	$16,$4
	movz	$18,$2,$3
 #APP
 # 39 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push
	.set	reorder
	.set	noat
	di	$17
	andi	$17, 1
	sll $0, $0, 3
	.set	pop

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	jal	filter_delconnect
	lw	$4,20($4)
	.set	macro
	.set	reorder

	lw	$5,4($16)
	lw	$6,8($16)
	lw	$7,20($16)
	lw	$10,24($16)
	xor	$3,$6,$5
	xor	$3,$3,$7
	srl	$2,$5,16
	lw	$4,0($16)
	xor	$3,$3,$10
	xor	$3,$3,$2
	srl	$2,$7,16
	xor	$3,$3,$2
	lui	$2,%hi(napt_table_list_max)
	lw	$11,%lo(napt_table_list_max)($2)
	lui	$9,%hi(table_napt)
	andi	$2,$4,0xffff
	xor	$3,$3,$2
	lw	$8,%lo(table_napt)($9)
	srl	$2,$3,12
	xor	$3,$2,$3
	addiu	$11,$11,-1
	and	$2,$3,$11
	lw	$3,0($8)
	sll	$8,$2,4
	sll	$2,$2,2
	subu	$8,$8,$2
	addu	$2,$3,$8
	lw	$2,0($2)
	bne	$2,$0,$L361
	j	$L334
$L335:
	lw	$2,36($2)
	beq	$2,$0,$L334
$L361:
	lw	$3,4($2)
	bne	$4,$3,$L335
	lw	$3,8($2)
	bne	$5,$3,$L335
	lw	$3,12($2)
	bne	$6,$3,$L335
	lw	$11,16($2)
	lw	$3,12($16)
	bne	$11,$3,$L335
	lw	$11,20($2)
	lw	$3,16($16)
	bne	$11,$3,$L335
	lw	$3,24($2)
	bne	$7,$3,$L335
	lw	$3,28($2)
	bne	$10,$3,$L335
	lw	$3,36($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L336
	sb	$0,0($2)
	.set	macro
	.set	reorder

	lw	$4,40($2)
	sw	$4,40($3)
$L337:
	lw	$3,40($2)
	lw	$4,36($2)
	sw	$4,0($3)
	lw	$3,%lo(table_napt)($9)
	lw	$3,0($3)
	addu	$8,$3,$8
	lw	$3,8($8)
	addiu	$3,$3,-1
	sw	$3,8($8)
	lw	$3,44($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L338
	lui	$4,%hi(napt_list_inuse)
	.set	macro
	.set	reorder

	lw	$5,48($2)
	addiu	$4,$4,%lo(napt_list_inuse)
	sw	$5,48($3)
$L339:
	lw	$5,48($2)
	lw	$6,44($2)
	lui	$3,%hi(napt_list_free)
	addiu	$3,$3,%lo(napt_list_free)
	sw	$6,0($5)
	lw	$6,8($4)
	addiu	$5,$2,44
	lui	$12,%hi(path_table_list_max)
	addiu	$6,$6,-1
	sw	$6,8($4)
	sw	$0,44($2)
	lw	$6,4($3)
	lui	$4,%hi(table_path)
	li	$14,1			# 0x1
	sw	$6,48($2)
	lw	$6,4($3)
	sw	$2,0($6)
	lw	$2,8($3)
	sw	$5,4($3)
	lw	$8,%lo(path_table_list_max)($12)
	addiu	$2,$2,1
	sw	$2,8($3)
	lw	$11,4($16)
	lw	$3,20($16)
	lw	$5,24($16)
	lw	$13,8($16)
	xor	$6,$3,$11
	srl	$2,$11,8
	xor	$6,$6,$5
	xor	$6,$6,$2
	srl	$2,$3,16
	xor	$6,$6,$2
	srl	$2,$13,4
	xor	$2,$6,$2
	xor	$2,$2,$18
	lw	$6,%lo(table_path)($4)
	srl	$7,$2,12
	xor	$2,$7,$2
	addiu	$8,$8,-1
	and	$2,$2,$8
	lw	$9,0($6)
	sll	$10,$2,4
	sll	$2,$2,2
	subu	$10,$10,$2
	addu	$2,$9,$10
	lw	$2,0($2)
	lw	$6,12($16)
	lw	$7,16($16)
$L340:
	beq	$2,$0,$L346
	lw	$24,4($2)
	lw	$15,0($16)
	beq	$24,$15,$L363
$L341:
	.set	noreorder
	.set	nomacro
	j	$L340
	lw	$2,76($2)
	.set	macro
	.set	reorder

$L334:
 #APP
 # 58 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push
	.set	noreorder
	.set	noat
	beqz	$17, 1f
	di
	ei
1:
	sll $0, $0, 3
	.set	pop

 # 0 "" 2
 #NO_APP
	li	$2,-4			# 0xfffffffffffffffc
$L357:
	lw	$31,28($sp)
$L365:
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L363:
	lbu	$15,2($2)
	bne	$15,$14,$L341
	lw	$15,12($2)
	bne	$11,$15,$L341
	lhu	$15,10($2)
	bne	$13,$15,$L341
	lw	$15,20($2)
	bne	$15,$6,$L341
	lhu	$15,24($2)
	bne	$15,$7,$L341
	lw	$15,16($2)
	bne	$3,$15,$L341
	lhu	$15,8($2)
	bne	$5,$15,$L341
	lw	$3,76($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L342
	sb	$0,0($2)
	.set	macro
	.set	reorder

	lw	$5,80($2)
	sw	$5,80($3)
$L343:
	lw	$3,80($2)
	lw	$5,76($2)
	sw	$5,0($3)
	lw	$3,%lo(table_path)($4)
	lw	$3,0($3)
	addu	$10,$3,$10
	lw	$3,8($10)
	addiu	$3,$3,-1
	sw	$3,8($10)
	lw	$5,84($2)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L344
	lui	$3,%hi(path_list_inuse)
	.set	macro
	.set	reorder

	lw	$6,88($2)
	addiu	$3,$3,%lo(path_list_inuse)
	sw	$6,88($5)
$L345:
	lw	$6,88($2)
	lw	$7,84($2)
	lui	$5,%hi(path_list_free)
	addiu	$5,$5,%lo(path_list_free)
	sw	$7,0($6)
	lw	$7,8($3)
	addiu	$6,$2,84
	addiu	$7,$7,-1
	sw	$7,8($3)
	sw	$0,84($2)
	lw	$3,4($5)
	sw	$3,88($2)
	lw	$3,4($5)
	sw	$2,0($3)
	lw	$3,8($5)
	lw	$2,%lo(table_path)($4)
	sw	$6,4($5)
	addiu	$3,$3,1
	sw	$3,8($5)
	lw	$8,%lo(path_table_list_max)($12)
	lw	$9,0($2)
	lw	$3,20($16)
	lw	$5,24($16)
	lw	$6,12($16)
	lw	$7,16($16)
	addiu	$8,$8,-1
$L346:
	xor	$10,$3,$6
	xor	$10,$10,$18
	srl	$2,$3,8
	xor	$10,$10,$7
	xor	$10,$10,$2
	srl	$2,$6,16
	xor	$10,$10,$2
	srl	$2,$5,4
	xor	$2,$10,$2
	srl	$10,$2,12
	xor	$2,$10,$2
	and	$8,$2,$8
	sll	$2,$8,4
	sll	$8,$8,2
	subu	$8,$2,$8
	addu	$9,$9,$8
	lw	$2,0($9)
	li	$9,2			# 0x2
$L348:
	beq	$2,$0,$L354
	lw	$11,4($2)
	lw	$10,0($16)
	beq	$11,$10,$L364
$L349:
	.set	noreorder
	.set	nomacro
	j	$L348
	lw	$2,76($2)
	.set	macro
	.set	reorder

$L364:
	lbu	$10,2($2)
	bne	$10,$9,$L349
	lw	$10,16($2)
	bne	$10,$6,$L349
	lhu	$10,8($2)
	bne	$10,$7,$L349
	lw	$10,20($2)
	bne	$10,$3,$L349
	lhu	$10,24($2)
	bne	$10,$5,$L349
	lw	$11,28($2)
	lw	$10,4($16)
	bne	$11,$10,$L349
	lhu	$11,32($2)
	lw	$10,8($16)
	bne	$11,$10,$L349
	lw	$3,76($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L350
	sb	$0,0($2)
	.set	macro
	.set	reorder

	lw	$5,80($2)
	sw	$5,80($3)
$L351:
	lw	$3,80($2)
	lw	$5,76($2)
	sw	$5,0($3)
	lw	$3,%lo(table_path)($4)
	lw	$3,0($3)
	addu	$8,$3,$8
	lw	$3,8($8)
	addiu	$3,$3,-1
	sw	$3,8($8)
	lw	$4,84($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L352
	lui	$3,%hi(path_list_inuse)
	.set	macro
	.set	reorder

	lw	$5,88($2)
	addiu	$3,$3,%lo(path_list_inuse)
	sw	$5,88($4)
$L353:
	lw	$5,88($2)
	lw	$6,84($2)
	lui	$4,%hi(path_list_free)
	addiu	$4,$4,%lo(path_list_free)
	sw	$6,0($5)
	lw	$6,8($3)
	addiu	$5,$2,84
	addiu	$6,$6,-1
	sw	$6,8($3)
	sw	$0,84($2)
	lw	$3,4($4)
	sw	$3,88($2)
	lw	$3,4($4)
	sw	$2,0($3)
	lw	$2,8($4)
	sw	$5,4($4)
	addiu	$2,$2,1
	sw	$2,8($4)
$L354:
	andi	$2,$17,0x1
	bne	$2,$0,$L356
 #APP
 # 58 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push
	.set	noreorder
	.set	noat
	beqz	$17, 1f
	di
	ei
1:
	sll $0, $0, 3
	.set	pop

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L365
	lw	$31,28($sp)
	.set	macro
	.set	reorder

$L352:
	lw	$4,88($2)
	addiu	$3,$3,%lo(path_list_inuse)
	.set	noreorder
	.set	nomacro
	j	$L353
	sw	$4,4($3)
	.set	macro
	.set	reorder

$L350:
	lw	$3,%lo(table_path)($4)
	lw	$5,80($2)
	lw	$3,0($3)
	addu	$3,$3,$8
	.set	noreorder
	.set	nomacro
	j	$L351
	sw	$5,4($3)
	.set	macro
	.set	reorder

$L356:
 #APP
 # 58 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push
	.set	noreorder
	.set	noat
	beqz	$17, 1f
	di
	ei
1:
	sll $0, $0, 3
	.set	pop

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L357
	move	$2,$0
	.set	macro
	.set	reorder

$L344:
	lw	$5,88($2)
	addiu	$3,$3,%lo(path_list_inuse)
	.set	noreorder
	.set	nomacro
	j	$L345
	sw	$5,4($3)
	.set	macro
	.set	reorder

$L342:
	lw	$3,%lo(table_path)($4)
	lw	$5,80($2)
	lw	$3,0($3)
	addu	$3,$3,$10
	.set	noreorder
	.set	nomacro
	j	$L343
	sw	$5,4($3)
	.set	macro
	.set	reorder

$L338:
	lw	$3,48($2)
	addiu	$4,$4,%lo(napt_list_inuse)
	.set	noreorder
	.set	nomacro
	j	$L339
	sw	$3,4($4)
	.set	macro
	.set	reorder

$L336:
	lw	$3,%lo(table_napt)($9)
	lw	$4,40($2)
	lw	$3,0($3)
	addu	$3,$3,$8
	.set	noreorder
	.set	nomacro
	j	$L337
	sw	$4,4($3)
	.set	macro
	.set	reorder

	.end	rtk_delNaptConnection
	.size	rtk_delNaptConnection, .-rtk_delNaptConnection
	.align	2
	.globl	rtk_addNaptConnection
	.set	nomips16
	.ent	rtk_addNaptConnection
	.type	rtk_addNaptConnection, @function
rtk_addNaptConnection:
	.frame	$sp,176,$31		# vars= 120, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-176
	sw	$31,172($sp)
	sw	$fp,168($sp)
	sw	$23,164($sp)
	sw	$22,160($sp)
	sw	$21,156($sp)
	sw	$20,152($sp)
	sw	$19,148($sp)
	sw	$18,144($sp)
	sw	$17,140($sp)
	sw	$16,136($sp)
	move	$17,$5
	move	$16,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L437
	move	$19,$6
	.set	macro
	.set	reorder

$L367:
	li	$2,-1			# 0xffffffffffffffff
$L373:
	lw	$31,172($sp)
	lw	$fp,168($sp)
	lw	$23,164($sp)
	lw	$22,160($sp)
	lw	$21,156($sp)
	lw	$20,152($sp)
	lw	$19,148($sp)
	lw	$18,144($sp)
	lw	$17,140($sp)
	lw	$16,136($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,176
	.set	macro
	.set	reorder

$L437:
	move	$4,$5
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	sw	$7,128($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L367
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	lw	$3,0($16)
	lw	$4,20($16)
	xori	$3,$3,0x2
	li	$23,17			# 0x11
	.set	noreorder
	.set	nomacro
	jal	filter_addconnect
	movz	$23,$2,$3
	.set	macro
	.set	reorder

 #APP
 # 39 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push
	.set	reorder
	.set	noat
	di	$2
	andi	$2, 1
	sll $0, $0, 3
	.set	pop

 # 0 "" 2
 #NO_APP
	sw	$2,76($sp)
	lw	$5,4($16)
	lw	$9,8($16)
	lw	$10,20($16)
	lw	$11,24($16)
	xor	$3,$9,$5
	xor	$3,$3,$10
	srl	$2,$5,16
	lw	$4,0($16)
	xor	$3,$3,$11
	xor	$3,$3,$2
	srl	$2,$10,16
	xor	$3,$3,$2
	lui	$2,%hi(napt_table_list_max)
	lw	$6,%lo(napt_table_list_max)($2)
	lui	$8,%hi(table_napt)
	andi	$2,$4,0xffff
	xor	$3,$3,$2
	lw	$12,%lo(table_napt)($8)
	srl	$2,$3,12
	xor	$3,$2,$3
	addiu	$6,$6,-1
	and	$2,$3,$6
	lw	$3,0($12)
	sll	$6,$2,4
	sll	$2,$2,2
	subu	$6,$6,$2
	addu	$2,$3,$6
	lw	$2,0($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L433
	lw	$7,128($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L454
	lui	$3,%hi(napt_list_free)
	.set	macro
	.set	reorder

$L371:
	lw	$2,36($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L370
	lui	$3,%hi(napt_list_free)
	.set	macro
	.set	reorder

$L433:
	lw	$3,4($2)
	bne	$4,$3,$L371
	lw	$3,8($2)
	bne	$5,$3,$L371
	lw	$3,12($2)
	bne	$9,$3,$L371
	lw	$12,16($2)
	lw	$3,12($16)
	bne	$12,$3,$L371
	lw	$12,20($2)
	lw	$3,16($16)
	bne	$12,$3,$L371
	lw	$3,24($2)
	bne	$10,$3,$L371
	lw	$3,28($2)
	bne	$11,$3,$L371
	move	$4,$19
	move	$5,$23
	.set	noreorder
	.set	nomacro
	jal	rtl_conntrack_drop_check_hook
	move	$6,$19
	.set	macro
	.set	reorder

	lw	$3,76($sp)
	andi	$2,$3,0x1
	bne	$2,$0,$L372
	move	$2,$3
 #APP
 # 58 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push
	.set	noreorder
	.set	noat
	beqz	$2, 1f
	di
	ei
1:
	sll $0, $0, 3
	.set	pop

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L373
	move	$2,$0
	.set	macro
	.set	reorder

$L370:
$L454:
	lw	$2,%lo(napt_list_free)($3)
	beq	$2,$0,$L375
	sw	$4,4($2)
	lw	$5,4($16)
	lw	$4,44($2)
	sw	$5,8($2)
	lw	$5,8($16)
	sw	$5,12($2)
	lw	$5,12($16)
	sw	$5,16($2)
	lw	$5,16($16)
	sw	$5,20($2)
	lw	$5,20($16)
	sw	$5,24($2)
	lw	$5,24($16)
	sw	$7,32($2)
	li	$7,-1			# 0xffffffffffffffff
	sb	$7,0($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L376
	sw	$5,28($2)
	.set	macro
	.set	reorder

	lw	$5,48($2)
	addiu	$3,$3,%lo(napt_list_free)
	sw	$5,48($4)
$L377:
	lw	$5,48($2)
	lw	$7,44($2)
	lui	$4,%hi(napt_list_inuse)
	addiu	$4,$4,%lo(napt_list_inuse)
	sw	$7,0($5)
	lw	$9,8($3)
	addiu	$7,$2,44
	addiu	$5,$2,36
	addiu	$9,$9,-1
	sw	$9,8($3)
	sw	$0,44($2)
	lw	$3,4($4)
	sw	$3,48($2)
	lw	$3,4($4)
	sw	$2,0($3)
	lw	$3,8($4)
	sw	$7,4($4)
	addiu	$3,$3,1
	sw	$3,8($4)
	sw	$0,36($2)
	lw	$3,%lo(table_napt)($8)
	lw	$3,0($3)
	addu	$3,$3,$6
	lw	$3,4($3)
	sw	$3,40($2)
	lw	$3,%lo(table_napt)($8)
	lw	$3,0($3)
	addu	$3,$3,$6
	lw	$3,4($3)
	sw	$2,0($3)
	lw	$2,%lo(table_napt)($8)
	lw	$2,0($2)
	addu	$2,$2,$6
	sw	$5,4($2)
	lw	$2,%lo(table_napt)($8)
	lw	$2,0($2)
	addu	$6,$2,$6
	lw	$2,8($6)
	addiu	$2,$2,1
	sw	$2,8($6)
	li	$2,33			# 0x21
	sb	$2,17($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl865x_getLanDev
	sb	$0,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl865x_getWanDev
	sw	$2,60($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_protocol
	sw	$2,56($sp)
	.set	macro
	.set	reorder

	move	$4,$17
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	sw	$2,124($sp)
	.set	macro
	.set	reorder

	lw	$21,0($2)
	move	$4,$17
	move	$20,$2
	andi	$21,$21,0xf
	sll	$21,$21,2
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	addu	$21,$2,$21
	.set	macro
	.set	reorder

	addiu	$5,$2,6
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$sp,20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	move	$5,$2
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$sp,28
	.set	macro
	.set	reorder

	lbu	$4,1($20)
	sw	$4,88($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_orig_dscp
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,88($sp)
	sll	$2,$2,2
	move	$4,$17
	andi	$3,$5,0x3
	or	$2,$2,$3
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_remark_vlanpri
	sb	$2,1($20)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ori_vlanpri
	sw	$2,96($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_phy_port
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	sw	$2,92($sp)
	.set	macro
	.set	reorder

	sw	$2,100($sp)
	lw	$6,12($20)
	move	$4,$17
	sw	$6,116($sp)
	lhu	$2,0($21)
	sw	$2,108($sp)
	lw	$3,16($20)
	sw	$3,112($sp)
	lhu	$5,2($21)
	.set	noreorder
	.set	nomacro
	jal	rtl_store_skb_dst
	sw	$5,104($sp)
	.set	macro
	.set	reorder

	lui	$6,%hi(path_table_list_max)
	lw	$2,%lo(path_table_list_max)($6)
	lw	$fp,4($16)
	lui	$3,%hi(path_list_free)
	addiu	$2,$2,-1
	sw	$2,84($sp)
	lw	$4,8($16)
	lw	$18,%lo(path_list_free)($3)
	sw	$4,80($sp)
	lw	$5,20($16)
	sw	$5,64($sp)
	lw	$6,24($16)
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L405
	sw	$6,72($sp)
	.set	macro
	.set	reorder

	lw	$2,0($16)
	sw	$2,4($18)
	lw	$8,4($16)
	sw	$8,12($18)
	lw	$2,8($16)
	sh	$2,10($18)
	lw	$2,20($16)
	sw	$2,16($18)
	lw	$2,24($16)
	sh	$2,8($18)
	lw	$9,12($16)
	sw	$9,20($18)
	lw	$2,16($16)
	sh	$2,24($18)
	lw	$7,20($16)
	sw	$7,28($18)
	lw	$2,24($16)
	sh	$2,32($18)
	lui	$2,%hi(route_list_inuse)
	lw	$3,%lo(route_list_inuse)($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L382
	move	$10,$0
	.set	macro
	.set	reorder

	move	$4,$0
$L383:
	lw	$2,8($3)
	sltu	$5,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L380
	and	$6,$2,$7
	.set	macro
	.set	reorder

	lw	$5,4($3)
	beq	$6,$5,$L438
$L380:
	move	$2,$4
$L381:
	lw	$3,44($3)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L383
	move	$4,$2
	.set	macro
	.set	reorder

$L382:
	li	$2,1			# 0x1
	sb	$2,2($18)
	li	$2,-1			# 0xffffffffffffffff
	sw	$10,40($18)
	sb	$2,0($18)
	sb	$0,1($18)
	sw	$19,52($18)
	lui	$3,%hi(rtl_init_bypass_packet_hook)
	lw	$2,%lo(rtl_init_bypass_packet_hook)($3)
	beq	$2,$0,$L384
	.set	noreorder
	.set	nomacro
	jalr	$2
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$8,12($18)
	lw	$9,20($18)
$L384:
	beq	$9,$8,$L385
	lbu	$2,1($18)
	ori	$2,$2,0x1
	sb	$2,1($18)
$L385:
	lhu	$3,10($18)
	lhu	$2,24($18)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L457
	li	$2,255			# 0xff
	.set	macro
	.set	reorder

	lbu	$2,1($18)
	ori	$2,$2,0x2
	sb	$2,1($18)
	li	$2,255			# 0xff
$L457:
	sh	$2,74($18)
	sw	$0,44($18)
	sw	$0,48($18)
	sw	$0,60($18)
	sb	$0,72($18)
	sb	$0,73($18)
	lw	$2,4($16)
	move	$5,$0
	addiu	$4,$sp,36
	sw	$2,12($20)
	lw	$2,8($16)
	li	$6,14			# 0xe
	sh	$2,0($21)
	lw	$2,20($16)
	sw	$2,16($20)
	lw	$2,24($16)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($21)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$4,60($sp)
	beq	$4,$0,$L387
	lw	$4,4($16)
	lw	$5,60($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_arp_req_get_ha
	addiu	$6,$sp,36
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L387
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$22,$2
	li	$2,1			# 0x1
	beq	$22,$2,$L439
$L388:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	addiu	$4,$2,6
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,36
	.set	macro
	.set	reorder

	lw	$5,60($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dmac
	move	$4,$17
	.set	macro
	.set	reorder

	addiu	$5,$sp,36
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_src_phy_port
	move	$6,$0
	.set	macro
	.set	reorder

	lw	$5,124($sp)
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L387
	lui	$2,%hi(nf_hooks+128)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+128)($2)
	addiu	$2,$2,%lo(nf_hooks+128)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L389
	lw	$6,60($sp)
	.set	macro
	.set	reorder

	lw	$7,56($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	move	$5,$0
	.set	macro
	.set	reorder

$L389:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$0,$L440
	lw	$2,44($18)
	sw	$2,44($18)
$L456:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	sw	$2,60($18)
	lui	$2,%hi(nf_hooks+160)
	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L392
	lw	$6,60($sp)
	.set	macro
	.set	reorder

	lw	$7,56($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	li	$5,4			# 0x4
	.set	macro
	.set	reorder

$L392:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$0,$L441
	lw	$2,48($18)
	sw	$2,48($18)
$L387:
	lw	$2,12($16)
	li	$6,14			# 0xe
	move	$5,$0
	sw	$2,12($20)
	lw	$2,16($16)
	addiu	$4,$sp,36
	sh	$2,0($21)
	lw	$2,20($16)
	sw	$2,16($20)
	lw	$2,24($16)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($21)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$6,56($sp)
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L395
	move	$5,$6
	.set	macro
	.set	reorder

	lw	$4,20($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_arp_req_get_ha
	addiu	$6,$sp,36
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L458
	move	$4,$18
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$22,$2
	li	$2,1			# 0x1
	beq	$22,$2,$L442
$L396:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$2
	addiu	$5,$sp,36
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lw	$5,56($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_smac
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$3,124($sp)
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L395
	lui	$2,%hi(nf_hooks+160)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L397
	lw	$6,60($sp)
	.set	macro
	.set	reorder

	lw	$7,56($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	li	$5,4			# 0x4
	.set	macro
	.set	reorder

$L397:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$0,$L443
	lw	$2,48($18)
	sw	$2,48($18)
$L395:
	move	$4,$18
$L458:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_entry_dscp_out
	move	$5,$17
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_entry_vlanpri
	move	$5,$17
	.set	macro
	.set	reorder

	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$23,$2,$L444
	li	$4,1			# 0x1
	.set	macro
	.set	reorder

	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	sw	$2,56($18)
$L401:
	sw	$19,52($18)
	lui	$4,%hi(jiffies)
	lw	$3,%lo(jiffies)($4)
	lw	$2,84($18)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L402
	sw	$3,36($18)
	.set	macro
	.set	reorder

	lw	$3,88($18)
	lui	$4,%hi(path_list_free)
	addiu	$4,$4,%lo(path_list_free)
	sw	$4,68($sp)
	sw	$3,88($2)
$L403:
	lw	$2,88($18)
	lw	$3,84($18)
	lui	$22,%hi(path_list_inuse)
	addiu	$22,$22,%lo(path_list_inuse)
	sw	$3,0($2)
	lw	$6,68($sp)
	lw	$4,64($sp)
	lw	$5,72($sp)
	lw	$2,8($6)
	xor	$3,$4,$fp
	xor	$3,$3,$5
	addiu	$2,$2,-1
	sw	$2,8($6)
	sw	$0,84($18)
	lw	$2,4($22)
	srl	$fp,$fp,8
	xor	$3,$3,$fp
	sw	$2,88($18)
	lw	$4,4($22)
	lw	$6,64($sp)
	lui	$fp,%hi(table_path)
	sw	$18,0($4)
	lw	$4,8($22)
	lw	$5,80($sp)
	srl	$2,$6,16
	xor	$3,$3,$2
	addiu	$4,$4,1
	srl	$2,$5,4
	addiu	$5,$18,84
	sw	$5,4($22)
	sw	$4,8($22)
	xor	$2,$3,$2
	sw	$0,76($18)
	lw	$6,84($sp)
	lw	$3,%lo(table_path)($fp)
	xor	$2,$2,$23
	srl	$4,$2,12
	xor	$4,$4,$2
	lw	$3,0($3)
	and	$4,$4,$6
	sll	$5,$4,4
	sll	$2,$4,2
	subu	$2,$5,$2
	addu	$3,$3,$2
	lw	$4,4($3)
	addiu	$3,$18,76
	sw	$4,80($18)
	lw	$4,%lo(table_path)($fp)
	lw	$4,0($4)
	addu	$4,$4,$2
	lw	$4,4($4)
	sw	$18,0($4)
	lw	$4,%lo(table_path)($fp)
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$3,4($4)
	lw	$3,%lo(table_path)($fp)
	lui	$4,%hi(path_list_free)
	lw	$3,0($3)
	addu	$2,$3,$2
	lw	$3,8($2)
	addiu	$3,$3,1
	sw	$3,8($2)
	lui	$3,%hi(path_table_list_max)
	lw	$2,%lo(path_table_list_max)($3)
	lw	$5,20($16)
	lw	$18,%lo(path_list_free)($4)
	addiu	$2,$2,-1
	sw	$5,72($sp)
	sw	$2,84($sp)
	lw	$6,24($16)
	sw	$6,120($sp)
	lw	$2,12($16)
	sw	$2,64($sp)
	lw	$3,16($16)
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L405
	sw	$3,80($sp)
	.set	macro
	.set	reorder

	lw	$2,0($16)
	lui	$4,%hi(route_list_inuse)
	sw	$2,4($18)
	lw	$2,20($16)
	sw	$2,12($18)
	lw	$2,24($16)
	sh	$2,10($18)
	lw	$7,12($16)
	sw	$7,16($18)
	lw	$2,16($16)
	sh	$2,8($18)
	lw	$2,20($16)
	sw	$2,20($18)
	lw	$2,24($16)
	sh	$2,24($18)
	lw	$6,4($16)
	sw	$6,28($18)
	lw	$2,8($16)
	sh	$2,32($18)
	lw	$3,%lo(route_list_inuse)($4)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L409
	move	$8,$0
	.set	macro
	.set	reorder

	move	$4,$0
$L410:
	lw	$2,8($3)
	sltu	$5,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L407
	and	$9,$2,$6
	.set	macro
	.set	reorder

	lw	$5,4($3)
	beq	$9,$5,$L445
$L407:
	move	$2,$4
$L408:
	lw	$3,44($3)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L410
	move	$4,$2
	.set	macro
	.set	reorder

$L409:
	li	$2,2			# 0x2
	sb	$2,2($18)
	li	$2,-1			# 0xffffffffffffffff
	sw	$8,40($18)
	sb	$2,0($18)
	sb	$0,1($18)
	sw	$19,52($18)
	lui	$5,%hi(rtl_init_bypass_packet_hook)
	lw	$2,%lo(rtl_init_bypass_packet_hook)($5)
	beq	$2,$0,$L411
	.set	noreorder
	.set	nomacro
	jalr	$2
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$7,16($18)
	lw	$6,28($18)
$L411:
	beq	$7,$6,$L412
	lbu	$2,1($18)
	ori	$2,$2,0x4
	sb	$2,1($18)
$L412:
	lhu	$3,8($18)
	lhu	$2,32($18)
	beq	$3,$2,$L413
	lbu	$2,1($18)
	ori	$2,$2,0x8
	sb	$2,1($18)
$L413:
	lbu	$3,1($20)
	move	$4,$17
	andi	$3,$3,0x3
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_orig_dscp
	sw	$3,128($sp)
	.set	macro
	.set	reorder

	lw	$3,128($sp)
	sll	$2,$2,2
	li	$6,14			# 0xe
	or	$3,$3,$2
	li	$2,255			# 0xff
	sb	$3,1($20)
	sh	$2,74($18)
	sw	$0,44($18)
	sw	$0,48($18)
	sw	$0,60($18)
	sb	$0,72($18)
	sb	$0,73($18)
	lw	$2,20($16)
	move	$5,$0
	addiu	$4,$sp,36
	sw	$2,12($20)
	lw	$2,24($16)
	sh	$2,0($21)
	lw	$2,12($16)
	sw	$2,16($20)
	lw	$2,16($16)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($21)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ori_vlanpri
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$6,56($sp)
	beq	$6,$0,$L415
	lw	$4,20($16)
	move	$5,$6
	.set	noreorder
	.set	nomacro
	jal	rtl_arp_req_get_ha
	addiu	$6,$sp,36
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L446
	move	$4,$17
	.set	macro
	.set	reorder

$L415:
	lw	$2,20($16)
	move	$5,$0
	addiu	$4,$sp,36
	sw	$2,12($20)
	lw	$2,24($16)
	li	$6,14			# 0xe
	sh	$2,0($21)
	lw	$2,4($16)
	sw	$2,16($20)
	lw	$2,8($16)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($21)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$4,60($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L421
	lw	$5,60($sp)
	.set	macro
	.set	reorder

	lw	$4,4($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_arp_req_get_ha
	addiu	$6,$sp,36
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L447
	move	$4,$17
	.set	macro
	.set	reorder

$L421:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	sw	$2,48($18)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_entry_dscp_in
	move	$5,$17
	.set	macro
	.set	reorder

	li	$2,6			# 0x6
	beq	$23,$2,$L448
	li	$4,1			# 0x1
	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	sw	$2,56($18)
$L424:
	sw	$19,52($18)
	lui	$6,%hi(jiffies)
	lw	$3,%lo(jiffies)($6)
	lw	$2,84($18)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L425
	sw	$3,36($18)
	.set	macro
	.set	reorder

	lw	$3,88($18)
	sw	$3,88($2)
$L426:
	lw	$2,88($18)
	lw	$3,84($18)
	lw	$6,64($sp)
	lw	$5,80($sp)
	sw	$3,0($2)
	lw	$2,68($sp)
	xor	$4,$5,$6
	srl	$5,$6,16
	lw	$3,8($2)
	xor	$2,$4,$5
	lw	$4,68($sp)
	addiu	$3,$3,-1
	xor	$2,$2,$23
	sw	$3,8($4)
	sw	$0,84($18)
	lw	$3,4($22)
	lw	$5,72($sp)
	sw	$3,88($18)
	lw	$6,4($22)
	xor	$2,$5,$2
	srl	$7,$5,8
	sw	$18,0($6)
	lw	$4,8($22)
	lw	$6,120($sp)
	addiu	$3,$18,84
	addiu	$4,$4,1
	sw	$3,4($22)
	sw	$4,8($22)
	srl	$3,$6,4
	sw	$0,76($18)
	xor	$2,$2,$7
	lw	$4,%lo(table_path)($fp)
	lw	$5,84($sp)
	xor	$2,$2,$3
	srl	$3,$2,12
	xor	$2,$3,$2
	lw	$4,0($4)
	and	$3,$5,$2
	sll	$2,$3,4
	sll	$3,$3,2
	subu	$2,$2,$3
	addu	$3,$4,$2
	lw	$3,4($3)
	addiu	$4,$18,76
	sw	$3,80($18)
	lw	$3,%lo(table_path)($fp)
	lw	$3,0($3)
	addu	$3,$3,$2
	lw	$3,4($3)
	sw	$18,0($3)
	lw	$3,%lo(table_path)($fp)
	lw	$3,0($3)
	addu	$3,$3,$2
	sw	$4,4($3)
	lw	$3,%lo(table_path)($fp)
	lw	$3,0($3)
	addu	$2,$3,$2
	lw	$3,8($2)
	addiu	$3,$3,1
	sw	$3,8($2)
$L405:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	addiu	$4,$2,6
	addiu	$5,$sp,20
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$2
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,28
	.set	macro
	.set	reorder

	lw	$5,100($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$6,116($sp)
	sw	$6,12($20)
	lw	$2,108($sp)
	sh	$2,0($21)
	lw	$3,112($sp)
	sw	$3,16($20)
	lw	$4,104($sp)
	sh	$4,2($21)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dst
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,92($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_phy_port
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,88($sp)
	move	$4,$17
	sb	$5,1($20)
	.set	noreorder
	.set	nomacro
	jal	rtl_restore_skb_vlanpri
	lw	$5,96($sp)
	.set	macro
	.set	reorder

	lw	$6,60($sp)
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L459
	lw	$2,56($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_dev_put
	move	$4,$6
	.set	macro
	.set	reorder

	lw	$2,56($sp)
$L459:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L460
	lw	$3,76($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_dev_put
	move	$4,$2
	.set	macro
	.set	reorder

	lw	$3,76($sp)
$L460:
	andi	$2,$3,0x1
	beq	$2,$0,$L449
	lw	$2,76($sp)
 #APP
 # 58 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push
	.set	noreorder
	.set	noat
	beqz	$2, 1f
	di
	ei
1:
	sll $0, $0, 3
	.set	pop

 # 0 "" 2
 #NO_APP
	move	$4,$19
$L455:
	move	$5,$23
	.set	noreorder
	.set	nomacro
	jal	rtl_conntrack_drop_check_hook
	move	$6,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L373
	move	$2,$0
	.set	macro
	.set	reorder

$L438:
	.set	noreorder
	.set	nomacro
	j	$L381
	addiu	$10,$3,16
	.set	macro
	.set	reorder

$L445:
	.set	noreorder
	.set	nomacro
	j	$L408
	addiu	$8,$3,16
	.set	macro
	.set	reorder

$L449:
	move	$2,$3
 #APP
 # 58 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push
	.set	noreorder
	.set	noat
	beqz	$2, 1f
	di
	ei
1:
	sll $0, $0, 3
	.set	pop

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L455
	move	$4,$19
	.set	macro
	.set	reorder

$L444:
	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_get_timeouts
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L401
	sw	$2,56($18)
	.set	macro
	.set	reorder

$L448:
	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_get_timeouts
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L424
	sw	$2,56($18)
	.set	macro
	.set	reorder

$L375:
	lw	$4,76($sp)
	andi	$2,$4,0x1
	bne	$2,$0,$L431
	move	$2,$4
 #APP
 # 58 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push
	.set	noreorder
	.set	noat
	beqz	$2, 1f
	di
	ei
1:
	sll $0, $0, 3
	.set	pop

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L373
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L446:
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L450
$L417:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	addiu	$4,$2,6
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,36
	.set	macro
	.set	reorder

	lw	$5,56($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dmac
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,92($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_phy_port
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$17
	addiu	$5,$sp,36
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_src_phy_port
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	lw	$3,124($sp)
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L451
	lui	$2,%hi(nf_hooks+128)
	.set	macro
	.set	reorder

$L418:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$0,$L452
	lw	$2,44($18)
	.set	noreorder
	.set	nomacro
	j	$L415
	sw	$2,44($18)
	.set	macro
	.set	reorder

$L447:
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$16,$2
	li	$2,1			# 0x1
	beq	$16,$2,$L453
$L422:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$2
	addiu	$5,$sp,36
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lw	$5,60($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_smac
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,124($sp)
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L421
	lui	$2,%hi(nf_hooks+160)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L421
	lw	$6,56($sp)
	.set	macro
	.set	reorder

	lw	$7,60($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	li	$5,4			# 0x4
	.set	macro
	.set	reorder

	j	$L421
$L376:
	lw	$4,48($2)
	addiu	$3,$3,%lo(napt_list_free)
	.set	noreorder
	.set	nomacro
	j	$L377
	sw	$4,4($3)
	.set	macro
	.set	reorder

$L402:
	lw	$2,88($18)
	lui	$5,%hi(path_list_free)
	addiu	$5,$5,%lo(path_list_free)
	sw	$5,68($sp)
	.set	noreorder
	.set	nomacro
	j	$L403
	sw	$2,4($5)
	.set	macro
	.set	reorder

$L425:
	lw	$2,88($18)
	lw	$3,68($sp)
	.set	noreorder
	.set	nomacro
	j	$L426
	sw	$2,4($3)
	.set	macro
	.set	reorder

$L431:
	lw	$2,76($sp)
 #APP
 # 58 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push
	.set	noreorder
	.set	noat
	beqz	$2, 1f
	di
	ei
1:
	sll $0, $0, 3
	.set	pop

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L373
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L452:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L415
	sw	$2,44($18)
	.set	macro
	.set	reorder

$L451:
	lw	$3,%lo(nf_hooks+128)($2)
	addiu	$2,$2,%lo(nf_hooks+128)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L418
	lw	$6,56($sp)
	.set	macro
	.set	reorder

	lw	$7,60($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	move	$5,$0
	.set	macro
	.set	reorder

	j	$L418
$L450:
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	sw	$3,128($sp)
	.set	macro
	.set	reorder

	lw	$3,128($sp)
	bne	$2,$3,$L417
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,13($2)
	lbu	$4,12($2)
	sll	$2,$3,8
	or	$4,$2,$4
	li	$2,8			# 0x8
	beq	$4,$2,$L417
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	j	$L417
$L439:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$22,$L388
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,13($2)
	lbu	$4,12($2)
	sll	$2,$3,8
	or	$4,$2,$4
	li	$2,8			# 0x8
	beq	$4,$2,$L388
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	j	$L388
$L442:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$22,$L396
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,13($2)
	lbu	$4,12($2)
	sll	$2,$3,8
	or	$4,$2,$4
	li	$2,8			# 0x8
	beq	$4,$2,$L396
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	j	$L396
$L453:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$16,$L422
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,13($2)
	lbu	$4,12($2)
	sll	$2,$3,8
	or	$4,$2,$4
	li	$2,8			# 0x8
	beq	$4,$2,$L422
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	j	$L422
$L441:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L387
	sw	$2,48($18)
	.set	macro
	.set	reorder

$L443:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L395
	sw	$2,48($18)
	.set	macro
	.set	reorder

$L440:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L456
	sw	$2,44($18)
	.set	macro
	.set	reorder

$L372:
	lw	$2,76($sp)
 #APP
 # 58 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push
	.set	noreorder
	.set	noat
	beqz	$2, 1f
	di
	ei
1:
	sll $0, $0, 3
	.set	pop

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L373
	move	$2,$0
	.set	macro
	.set	reorder

	.end	rtk_addNaptConnection
	.size	rtk_addNaptConnection, .-rtk_addNaptConnection
	.align	2
	.globl	negative_fragCache_init
	.set	nomips16
	.ent	negative_fragCache_init
	.type	negative_fragCache_init, @function
negative_fragCache_init:
	.frame	$sp,48,$31		# vars= 8, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$20,40($sp)
	sw	$19,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	lui	$18,%hi(kmalloc_caches)
	addiu	$18,$18,%lo(kmalloc_caches)
	lw	$4,40($18)
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	lui	$3,%hi(negative_cache_table)
	beq	$2,$0,$L462
	sw	$2,%lo(negative_cache_table)($3)

	lui	$4,%hi(negative_cache_list_free)
	addiu	$16,$4,%lo(negative_cache_list_free)
	sw	$0,%lo(negative_cache_list_free)($4)
	sw	$0,8($16)
	sw	$16,4($16)
	move	$4,$0
	move	$8,$3
	j	$L464
	li	$7,64			# 0x40

$L469:
	lw	$2,%lo(negative_cache_table)($8)
$L464:
	sll	$5,$4,2
	sll	$6,$4,4
	subu	$5,$6,$5
	addu	$2,$2,$5
	sw	$0,8($2)
	lw	$2,%lo(negative_cache_table)($3)
	addiu	$4,$4,1
	addu	$2,$2,$5
	sw	$0,0($2)
	lw	$2,%lo(negative_cache_table)($3)
	addu	$5,$2,$5
	bne	$4,$7,$L469
	sw	$5,4($5)

	lui	$20,%hi(negative_cache_timeout)
	addiu	$20,$20,%lo(negative_cache_timeout)
	move	$17,$0
	j	$L465
	li	$19,64			# 0x40

$L471:
	jal	init_timer_key
	sw	$2,16($sp)

	lw	$2,16($sp)
	sw	$2,32($2)
	sw	$20,28($2)
	sw	$0,48($2)
	lw	$4,4($16)
	addiu	$3,$2,48
	sw	$4,52($2)
	lw	$4,4($16)
	sw	$2,0($4)
	lw	$2,8($16)
	sw	$3,4($16)
	addiu	$2,$2,1
	beq	$17,$19,$L470
	sw	$2,8($16)

$L465:
	lw	$4,28($18)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,12
	move	$5,$0
	move	$6,$0
	bne	$2,$0,$L471
	move	$7,$0

$L462:
	lw	$31,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,48

$L470:
	lw	$31,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	li	$2,1			# 0x1
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	negative_fragCache_init
	.size	negative_fragCache_init, .-negative_fragCache_init
	.align	2
	.globl	udp_fragCache_init
	.set	nomips16
	.ent	udp_fragCache_init
	.type	udp_fragCache_init, @function
udp_fragCache_init:
	.frame	$sp,48,$31		# vars= 8, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$20,40($sp)
	sw	$19,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	lui	$19,%hi(kmalloc_caches)
	addiu	$19,$19,%lo(kmalloc_caches)
	move	$18,$4
	lw	$4,28($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	lui	$17,%hi(udp_cache_table)

	move	$20,$2
	beq	$2,$0,$L473
	sw	$2,%lo(udp_cache_table)($17)

	lui	$2,%hi(udp_cache_list_free)
	sll	$3,$18,2
	sll	$4,$18,4
	addiu	$16,$2,%lo(udp_cache_list_free)
	subu	$4,$4,$3
	sw	$0,%lo(udp_cache_list_free)($2)
	li	$5,32			# 0x20
	lui	$2,%hi(max_udp_frag_entry)
	sw	$18,%lo(max_udp_frag_entry)($2)
	sw	$0,8($16)
	jal	__kmalloc
	sw	$16,4($16)

	sw	$2,0($20)
	lw	$3,%lo(udp_cache_table)($17)
	lw	$3,0($3)
	beq	$3,$0,$L475
	li	$2,-1			# 0xffffffffffffffff

	blez	$18,$L476
	nop

	move	$2,$0
	move	$4,$0
	j	$L478
	move	$5,$17

$L482:
	lw	$3,%lo(udp_cache_table)($5)
	lw	$3,0($3)
$L478:
	addu	$3,$3,$2
	sw	$0,8($3)
	lw	$3,%lo(udp_cache_table)($17)
	addiu	$4,$4,1
	lw	$3,0($3)
	addu	$3,$3,$2
	sw	$0,0($3)
	lw	$3,%lo(udp_cache_table)($17)
	lw	$3,0($3)
	addu	$3,$3,$2
	sw	$3,4($3)
	bne	$4,$18,$L482
	addiu	$2,$2,12

	lui	$20,%hi(cache_timeout)
	addiu	$20,$20,%lo(cache_timeout)
	j	$L479
	move	$17,$0

$L483:
	jal	init_timer_key
	sw	$2,16($sp)

	lw	$2,16($sp)
	slt	$3,$17,$18
	sw	$2,36($2)
	sw	$20,32($2)
	sw	$0,52($2)
	lw	$5,4($16)
	addiu	$4,$2,52
	sw	$5,56($2)
	lw	$5,4($16)
	sw	$2,0($5)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$3,$0,$L476
	sw	$2,8($16)

$L479:
	lw	$4,28($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,16
	move	$5,$0
	move	$6,$0
	bne	$2,$0,$L483
	move	$7,$0

$L473:
	move	$2,$0
$L475:
	lw	$31,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,48

$L476:
	j	$L475
	li	$2,1			# 0x1

	.set	macro
	.set	reorder
	.end	udp_fragCache_init
	.size	udp_fragCache_init, .-udp_fragCache_init
	.globl	fastpath_packet_bypassed_num
	.data
	.align	2
	.type	fastpath_packet_bypassed_num, @object
	.size	fastpath_packet_bypassed_num, 4
fastpath_packet_bypassed_num:
	.word	10
	.globl	rtl_init_bypass_packet_hook
	.section	.bss,"aw",@nobits
	.align	2
	.type	rtl_init_bypass_packet_hook, @object
	.size	rtl_init_bypass_packet_hook, 4
rtl_init_bypass_packet_hook:
	.space	4
	.globl	rtl_check_bypass_packet_hook
	.align	2
	.type	rtl_check_bypass_packet_hook, @object
	.size	rtl_check_bypass_packet_hook, 4
rtl_check_bypass_packet_hook:
	.space	4
	.globl	fastpath_napt_proc_fops
	.data
	.align	2
	.type	fastpath_napt_proc_fops, @object
	.size	fastpath_napt_proc_fops, 108
fastpath_napt_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	fastpath_napt_single_write
	.space	28
	.word	fastpath_napt_single_open
	.space	4
	.word	single_release
	.space	52
	.globl	fastpath_path_proc_fops
	.align	2
	.type	fastpath_path_proc_fops, @object
	.size	fastpath_path_proc_fops, 108
fastpath_path_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	fastpath_path_single_write
	.space	28
	.word	fastpath_path_single_open
	.space	4
	.word	single_release
	.space	52
	.local	napt_table_list_max
	.comm	napt_table_list_max,4,4
	.local	path_table_list_max
	.comm	path_table_list_max,4,4
	.local	max_udp_frag_entry
	.comm	max_udp_frag_entry,4,4
	.globl	route_list_inuse
	.section	.bss
	.align	2
	.type	route_list_inuse, @object
	.size	route_list_inuse, 12
route_list_inuse:
	.space	12
	.globl	route_list_free
	.align	2
	.type	route_list_free, @object
	.size	route_list_free, 12
route_list_free:
	.space	12
	.globl	napt_list_inuse
	.align	2
	.type	napt_list_inuse, @object
	.size	napt_list_inuse, 12
napt_list_inuse:
	.space	12
	.globl	napt_list_free
	.align	2
	.type	napt_list_free, @object
	.size	napt_list_free, 12
napt_list_free:
	.space	12
	.globl	table_napt
	.align	2
	.type	table_napt, @object
	.size	table_napt, 4
table_napt:
	.space	4
	.globl	path_list_inuse
	.align	2
	.type	path_list_inuse, @object
	.size	path_list_inuse, 12
path_list_inuse:
	.space	12
	.globl	path_list_free
	.align	2
	.type	path_list_free, @object
	.size	path_list_free, 12
path_list_free:
	.space	12
	.globl	table_path
	.align	2
	.type	table_path, @object
	.size	table_path, 4
table_path:
	.space	4
	.globl	fp_arp
	.align	2
	.type	fp_arp, @object
	.size	fp_arp, 4
fp_arp:
	.space	4
	.globl	fp_path
	.align	2
	.type	fp_path, @object
	.size	fp_path, 4
fp_path:
	.space	4
	.globl	fp_route
	.align	2
	.type	fp_route, @object
	.size	fp_route, 4
fp_route:
	.space	4
	.globl	fp_napt
	.align	2
	.type	fp_napt, @object
	.size	fp_napt, 4
fp_napt:
	.space	4
	.globl	udp_cache_list_free
	.align	2
	.type	udp_cache_list_free, @object
	.size	udp_cache_list_free, 12
udp_cache_list_free:
	.space	12
	.globl	udp_cache_table
	.align	2
	.type	udp_cache_table, @object
	.size	udp_cache_table, 4
udp_cache_table:
	.space	4
	.globl	negative_cache_list_free
	.align	2
	.type	negative_cache_list_free, @object
	.size	negative_cache_list_free, 12
negative_cache_list_free:
	.space	12
	.globl	negative_cache_table
	.align	2
	.type	negative_cache_table, @object
	.size	negative_cache_table, 4
negative_cache_table:
	.space	4
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
