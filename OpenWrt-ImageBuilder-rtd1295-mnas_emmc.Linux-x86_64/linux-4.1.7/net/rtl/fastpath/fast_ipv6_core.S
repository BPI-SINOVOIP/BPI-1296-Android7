/*
 * Copyright (C) 2017 Realtek Semiconductor Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

	.file	1 "fast_ipv6_core.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.text
	.align	2
	.globl	rtk_flushV6Connection
	.set	nomips16
	.ent	rtk_flushV6Connection
	.type	rtk_flushV6Connection, @function
rtk_flushV6Connection:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	lui	$2,%hi(v6_path_table_list_max)
	lw	$11,%lo(v6_path_table_list_max)($2)
	beq	$11,$0,$L18
	lui	$5,%hi(v6_path_list_inuse)

	lui	$3,%hi(v6_path_list_free)
	addiu	$5,$5,%lo(v6_path_list_inuse)
	addiu	$3,$3,%lo(v6_path_list_free)
	move	$8,$0
	move	$10,$0
	lui	$9,%hi(v6_table_path)
$L9:
	lw	$2,%lo(v6_table_path)($9)
	lw	$2,0($2)
	addu	$2,$2,$8
	lw	$2,0($2)
	bne	$2,$0,$L8
	nop

	j	$L17
	addiu	$10,$10,1

$L15:
	lw	$6,56($2)
	sw	$6,56($4)
	lw	$4,56($2)
	lw	$6,52($2)
	sw	$6,0($4)
	lw	$4,%lo(v6_table_path)($9)
	lw	$4,0($4)
	addu	$4,$4,$8
	lw	$6,8($4)
	addiu	$6,$6,-1
	sw	$6,8($4)
	lw	$4,60($2)
	beq	$4,$0,$L6
	nop

$L16:
	lw	$6,64($2)
	sw	$6,64($4)
$L7:
	lw	$4,64($2)
	lw	$7,60($2)
	addiu	$6,$2,60
	sw	$7,0($4)
	lw	$4,8($5)
	addiu	$4,$4,-1
	sw	$4,8($5)
	sw	$0,60($2)
	lw	$4,4($3)
	sw	$4,64($2)
	lw	$4,4($3)
	sw	$2,0($4)
	lw	$4,8($3)
	sw	$6,4($3)
	addiu	$4,$4,1
	sw	$4,8($3)
	lw	$2,52($2)
	beq	$2,$0,$L3
	nop

$L8:
	lw	$4,52($2)
	bne	$4,$0,$L15
	nop

	lw	$4,%lo(v6_table_path)($9)
	lw	$6,56($2)
	lw	$4,0($4)
	addu	$4,$4,$8
	sw	$6,4($4)
	lw	$4,56($2)
	lw	$6,52($2)
	sw	$6,0($4)
	lw	$4,%lo(v6_table_path)($9)
	lw	$4,0($4)
	addu	$4,$4,$8
	lw	$6,8($4)
	addiu	$6,$6,-1
	sw	$6,8($4)
	lw	$4,60($2)
	bne	$4,$0,$L16
	nop

$L6:
	lw	$4,64($2)
	j	$L7
	sw	$4,4($5)

$L3:
	addiu	$10,$10,1
$L17:
	sltu	$2,$10,$11
	bne	$2,$0,$L9
	addiu	$8,$8,12

$L18:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	rtk_flushV6Connection
	.size	rtk_flushV6Connection, .-rtk_flushV6Connection
	.align	2
	.set	nomips16
	.ent	V6_cache_timeout
	.type	V6_cache_timeout, @function
V6_cache_timeout:
	.frame	$sp,32,$31		# vars= 32, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	lbu	$2,48($4)
	beq	$2,$0,$L23
	addiu	$sp,$sp,-32

	lw	$10,4($4)
	lw	$9,8($4)
	lw	$8,12($4)
	lw	$7,16($4)
	lhu	$11,36($4)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$7,28($sp)
	lw	$6,20($4)
	xor	$12,$9,$10
	lw	$5,24($4)
	xor	$12,$12,$8
	lw	$3,28($4)
	xor	$12,$12,$7
	lw	$2,32($4)
	xor	$12,$12,$6
	xor	$12,$12,$5
	xor	$12,$12,$3
	xor	$12,$12,$2
	srl	$10,$10,8
	xor	$10,$12,$10
	srl	$9,$9,8
	xor	$9,$10,$9
	srl	$8,$8,8
	xor	$8,$9,$8
	srl	$7,$7,8
	srl	$12,$6,16
	xor	$7,$8,$7
	xor	$7,$7,$12
	lw	$10,0($4)
	srl	$12,$5,16
	sw	$3,8($sp)
	srl	$9,$3,16
	xor	$7,$7,$12
	lui	$3,%hi(V6_max_udp_frag_entry)
	sw	$6,0($sp)
	sw	$2,12($sp)
	sw	$5,4($sp)
	lw	$3,%lo(V6_max_udp_frag_entry)($3)
	xor	$7,$7,$9
	srl	$2,$2,16
	xor	$2,$7,$2
	xor	$6,$11,$10
	lw	$5,80($4)
	xor	$6,$6,$2
	srl	$2,$2,16
	xor	$2,$6,$2
	addiu	$3,$3,-1
	and	$2,$2,$3
	beq	$5,$0,$L21
	sb	$0,48($4)

	lw	$6,84($4)
	sll	$3,$2,4
	sll	$2,$2,2
	sw	$6,84($5)
	subu	$3,$3,$2
	lui	$5,%hi(V6_udp_cache_table)
$L22:
	lw	$6,84($4)
	lw	$7,80($4)
	lui	$2,%hi(V6_udp_cache_list_free)
	addiu	$2,$2,%lo(V6_udp_cache_list_free)
	sw	$7,0($6)
	lw	$6,%lo(V6_udp_cache_table)($5)
	addiu	$5,$4,88
	lw	$6,0($6)
	addu	$3,$6,$3
	lw	$6,8($3)
	addiu	$6,$6,-1
	sw	$6,8($3)
	sw	$0,88($4)
	lw	$3,4($2)
	sw	$3,92($4)
	lw	$3,4($2)
	sw	$4,0($3)
	lw	$3,8($2)
	sw	$5,4($2)
	addiu	$3,$3,1
	sw	$3,8($2)
$L23:
	addiu	$sp,$sp,32
	j	$31
	nop

$L21:
	lui	$5,%hi(V6_udp_cache_table)
	lw	$6,%lo(V6_udp_cache_table)($5)
	sll	$3,$2,4
	sll	$2,$2,2
	lw	$7,0($6)
	lw	$6,84($4)
	subu	$3,$3,$2
	addu	$2,$7,$3
	j	$L22
	sw	$6,4($2)

	.set	macro
	.set	reorder
	.end	V6_cache_timeout
	.size	V6_cache_timeout, .-V6_cache_timeout
	.align	2
	.set	nomips16
	.ent	ipv6_fast_forward_single_write
	.type	ipv6_fast_forward_single_write, @function
ipv6_fast_forward_single_write:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	sltu	$2,$6,2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L29
	move	$16,$6
	.set	macro
	.set	reorder

$L25:
	li	$2,-14			# 0xfffffffffffffff2
$L27:
	lw	$31,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L29:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L27
	li	$2,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	addiu	$2,$5,1
	or	$3,$2,$5
	lw	$2,24($28)
	and	$3,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L25
	addiu	$4,$sp,16
	.set	macro
	.set	reorder

	li	$6,1			# 0x1
 #APP
 # 1169 "net/rtl/fastpath/fast_ipv6_core.c" 1
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
	bne	$6,$0,$L25
	lb	$2,16($sp)
	.set	macro
	.set	reorder

	lui	$3,%hi(fast_ipv6_fw)
	addiu	$2,$2,-48
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L30
	sw	$2,%lo(fast_ipv6_fw)($3)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L27
	move	$2,$16
	.set	macro
	.set	reorder

$L30:
	jal	rtk_flushV6Connection
	.set	noreorder
	.set	nomacro
	j	$L27
	move	$2,$16
	.set	macro
	.set	reorder

	.end	ipv6_fast_forward_single_write
	.size	ipv6_fast_forward_single_write, .-ipv6_fast_forward_single_write
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"ipv6_fast_forward\000"
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	ipv6_fast_forward_exit
	.set	nomips16
	.ent	ipv6_fast_forward_exit
	.type	ipv6_fast_forward_exit, @function
ipv6_fast_forward_exit:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(ipv6_res)
	lw	$2,%lo(ipv6_res)($2)
	beq	$2,$0,$L32
	lui	$4,%hi($LC0)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC0)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

$L32:
	lw	$31,20($sp)
	lui	$2,%hi(fast_ipv6_fw)
	sw	$0,%lo(fast_ipv6_fw)($2)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	ipv6_fast_forward_exit
	.size	ipv6_fast_forward_exit, .-ipv6_fast_forward_exit
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	ipv6_fast_forward_init
	.set	nomips16
	.ent	ipv6_fast_forward_init
	.type	ipv6_fast_forward_init, @function
ipv6_fast_forward_init:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$4,%hi($LC0)
	lui	$6,%hi(proc_root)
	lui	$7,%hi(ipv6_fast_forward_proc_fops)
	addiu	$4,$4,%lo($LC0)
	addiu	$6,$6,%lo(proc_root)
	addiu	$7,$7,%lo(ipv6_fast_forward_proc_fops)
	move	$5,$0
	jal	proc_create_data
	sw	$0,16($sp)

	lw	$31,28($sp)
	lui	$3,%hi(ipv6_res)
	sw	$2,%lo(ipv6_res)($3)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	ipv6_fast_forward_init
	.size	ipv6_fast_forward_init, .-ipv6_fast_forward_init
	.text
	.align	2
	.globl	ipv6_fast_forward_single_open
	.set	nomips16
	.ent	ipv6_fast_forward_single_open
	.type	ipv6_fast_forward_single_open, @function
ipv6_fast_forward_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	move	$4,$5
	lui	$5,%hi(ipv6_forward_read_proc)
	addiu	$5,$5,%lo(ipv6_forward_read_proc)
	move	$6,$0
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	ipv6_fast_forward_single_open
	.size	ipv6_fast_forward_single_open, .-ipv6_fast_forward_single_open
	.section	.rodata.str1.4
	.align	2
$LC1:
	.ascii	"%c\012\000"
	.text
	.align	2
	.set	nomips16
	.ent	ipv6_forward_read_proc
	.type	ipv6_forward_read_proc, @function
ipv6_forward_read_proc:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(fast_ipv6_fw)
	lw	$6,%lo(fast_ipv6_fw)($2)
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
	.end	ipv6_forward_read_proc
	.size	ipv6_forward_read_proc, .-ipv6_forward_read_proc
	.align	2
	.globl	init_V6_table_path
	.set	nomips16
	.ent	init_V6_table_path
	.type	init_V6_table_path, @function
init_V6_table_path:
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
	li	$5,32800			# 0x8020

	lui	$17,%hi(v6_table_path)
	move	$21,$2
	beq	$2,$0,$L41
	sw	$2,%lo(v6_table_path)($17)

	lui	$6,%hi(v6_path_list_inuse)
	sll	$2,$18,2
	sll	$4,$18,4
	lui	$3,%hi(v6_path_list_free)
	subu	$4,$4,$2
	addiu	$2,$6,%lo(v6_path_list_inuse)
	addiu	$16,$3,%lo(v6_path_list_free)
	sw	$0,%lo(v6_path_list_free)($3)
	li	$5,32			# 0x20
	lui	$3,%hi(v6_path_table_list_max)
	sw	$2,4($2)
	sw	$0,8($2)
	sw	$0,%lo(v6_path_list_inuse)($6)
	sw	$18,%lo(v6_path_table_list_max)($3)
	sw	$0,8($16)
	jal	__kmalloc
	sw	$16,4($16)

	sw	$2,0($21)
	lw	$2,%lo(v6_table_path)($17)
	lw	$4,0($2)
	beq	$4,$0,$L41
	nop

	blez	$18,$L42
	move	$2,$0

	move	$3,$0
	j	$L43
	move	$5,$17

$L50:
	lw	$4,%lo(v6_table_path)($5)
	lw	$4,0($4)
$L43:
	addu	$4,$4,$2
	sw	$0,8($4)
	lw	$4,%lo(v6_table_path)($17)
	addiu	$3,$3,1
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$0,0($4)
	lw	$4,%lo(v6_table_path)($17)
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$4,4($4)
	bne	$3,$18,$L50
	addiu	$2,$2,12

$L42:
	blez	$19,$L44
	move	$17,$0

	j	$L51
	lw	$4,28($20)

$L45:
	sw	$0,60($2)
	lw	$5,4($16)
	sw	$5,64($2)
	lw	$5,4($16)
	sw	$2,0($5)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$3,$0,$L44
	sw	$2,8($16)

	lw	$4,28($20)
$L51:
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,60
	bne	$2,$0,$L45
	slt	$3,$17,$19

	li	$2,-2			# 0xfffffffffffffffe
$L46:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L44:
	j	$L46
	move	$2,$0

$L41:
	j	$L46
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	init_V6_table_path
	.size	init_V6_table_path, .-init_V6_table_path
	.align	2
	.globl	rtk_idleV6Connection
	.set	nomips16
	.ent	rtk_idleV6Connection
	.type	rtk_idleV6Connection, @function
rtk_idleV6Connection:
	.frame	$sp,112,$31		# vars= 64, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$22,104($sp)
	sw	$21,100($sp)
	sw	$20,96($sp)
	sw	$19,92($sp)
	sw	$18,88($sp)
	sw	$17,84($sp)
	sw	$16,80($sp)
	lw	$8,0($4)
	lw	$3,40($4)
	lw	$7,4($4)
	lw	$6,8($4)
	move	$17,$4
	xor	$13,$8,$3
	lw	$4,12($4)
	lw	$12,16($17)
	xor	$13,$13,$7
	lw	$11,20($17)
	xor	$13,$13,$6
	lw	$10,24($17)
	xor	$13,$13,$4
	lw	$9,28($17)
	xor	$13,$13,$12
	xor	$13,$13,$11
	xor	$13,$13,$10
	srl	$2,$8,8
	xor	$13,$13,$9
	xor	$13,$13,$2
	srl	$2,$7,8
	xor	$13,$13,$2
	srl	$2,$6,8
	xor	$13,$13,$2
	srl	$2,$4,8
	xor	$13,$13,$2
	srl	$2,$12,16
	xor	$13,$13,$2
	srl	$14,$11,16
	lw	$2,36($17)
	xor	$13,$13,$14
	srl	$14,$10,16
	lhu	$15,32($17)
	xor	$13,$13,$14
	srl	$14,$9,16
	xor	$13,$13,$14
	lui	$18,%hi(v6_path_table_list_max)
	srl	$14,$2,4
	xor	$13,$13,$14
	lui	$19,%hi(v6_table_path)
	lw	$14,%lo(v6_path_table_list_max)($18)
	xor	$13,$13,$15
	lw	$25,%lo(v6_table_path)($19)
	srl	$24,$13,12
	xor	$24,$24,$13
	sw	$8,16($sp)
	sw	$7,20($sp)
	sw	$6,24($sp)
	sw	$4,28($sp)
	sw	$12,32($sp)
	sw	$11,36($sp)
	sw	$10,40($sp)
	sw	$9,44($sp)
	addiu	$14,$14,-1
	and	$24,$24,$14
	lw	$13,0($25)
	sll	$25,$24,4
	sll	$24,$24,2
	subu	$24,$25,$24
	addu	$24,$13,$24
	lw	$16,0($24)
	move	$21,$5
	lui	$5,%hi(jiffies)
	lw	$22,%lo(jiffies)($5)
	move	$24,$3
	beq	$16,$0,$L53
	move	$5,$2

	j	$L57
	addiu	$20,$17,16

$L54:
	lw	$16,52($16)
	beq	$16,$0,$L71
	lw	$4,%lo(v6_table_path)($19)

$L57:
	lw	$4,36($16)
	bne	$4,$2,$L54
	nop

	lw	$4,40($16)
	bne	$4,$3,$L54
	move	$4,$16

	move	$5,$17
	jal	memcmp
	li	$6,16			# 0x10

	beq	$2,$0,$L66
	addiu	$4,$16,16

$L69:
	lw	$16,52($16)
	lw	$2,36($17)
	bne	$16,$0,$L57
	lw	$3,40($17)

	lw	$4,%lo(v6_table_path)($19)
$L71:
	lw	$14,%lo(v6_path_table_list_max)($18)
	lhu	$15,32($17)
	lw	$13,0($4)
	lw	$8,0($17)
	lw	$7,4($17)
	lw	$6,8($17)
	lw	$4,12($17)
	lw	$12,16($17)
	lw	$11,20($17)
	lw	$10,24($17)
	lw	$9,28($17)
	move	$24,$3
	move	$5,$2
	addiu	$14,$14,-1
$L53:
	xor	$2,$11,$12
	xor	$2,$2,$10
	xor	$2,$2,$9
	xor	$2,$2,$8
	xor	$2,$2,$7
	xor	$2,$2,$6
	srl	$3,$12,8
	xor	$2,$2,$4
	xor	$2,$2,$3
	srl	$3,$11,8
	xor	$2,$2,$3
	srl	$3,$10,8
	xor	$2,$2,$3
	srl	$3,$9,8
	xor	$2,$2,$3
	srl	$3,$8,16
	xor	$2,$2,$3
	srl	$3,$7,16
	xor	$2,$2,$3
	srl	$3,$6,16
	xor	$2,$2,$3
	srl	$3,$4,16
	xor	$2,$2,$3
	xor	$2,$2,$5
	xor	$15,$2,$15
	srl	$24,$24,4
	xor	$15,$15,$24
	srl	$2,$15,12
	xor	$15,$2,$15
	and	$14,$15,$14
	sll	$2,$14,4
	sll	$14,$14,2
	subu	$14,$2,$14
	addu	$13,$13,$14
	sw	$12,48($sp)
	sw	$11,52($sp)
	sw	$10,56($sp)
	sw	$9,60($sp)
	sw	$8,64($sp)
	sw	$7,68($sp)
	sw	$6,72($sp)
	sw	$4,76($sp)
	lw	$16,0($13)
	beq	$16,$0,$L58
	addiu	$18,$17,16

	j	$L72
	lw	$2,36($16)

$L59:
	lw	$16,52($16)
	beq	$16,$0,$L61
	move	$2,$0

	lw	$5,36($17)
	lw	$2,36($16)
$L72:
	bne	$2,$5,$L59
	nop

	lw	$3,40($16)
	lw	$2,40($17)
	bne	$3,$2,$L59
	move	$4,$16

	move	$5,$17
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L59
	addiu	$4,$16,16

	move	$5,$18
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L59
	nop

	lw	$3,32($16)
	lw	$2,32($17)
	bne	$3,$2,$L59
	nop

	lw	$2,44($16)
	subu	$21,$22,$21
	subu	$21,$21,$2
	bltz	$21,$L61
	li	$2,-1			# 0xffffffffffffffff

$L58:
	move	$2,$0
$L61:
	lw	$31,108($sp)
	lw	$22,104($sp)
	lw	$21,100($sp)
	lw	$20,96($sp)
	lw	$19,92($sp)
	lw	$18,88($sp)
	lw	$17,84($sp)
	lw	$16,80($sp)
	j	$31
	addiu	$sp,$sp,112

$L66:
	move	$5,$20
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L69
	nop

	lw	$15,32($16)
	lw	$2,32($17)
	bne	$15,$2,$L69
	subu	$3,$22,$21

	lw	$2,44($16)
	subu	$2,$3,$2
	bltz	$2,$L55
	lw	$14,%lo(v6_path_table_list_max)($18)

	lw	$2,%lo(v6_table_path)($19)
	andi	$15,$15,0xffff
	lw	$13,0($2)
	addiu	$14,$14,-1
	lw	$24,40($17)
	lw	$5,36($17)
	lw	$8,0($17)
	lw	$7,4($17)
	lw	$6,8($17)
	lw	$4,12($17)
	lw	$12,16($17)
	lw	$11,20($17)
	lw	$10,24($17)
	j	$L53
	lw	$9,28($17)

$L55:
	j	$L61
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	rtk_idleV6Connection
	.size	rtk_idleV6Connection, .-rtk_idleV6Connection
	.align	2
	.globl	rtk_delV6Connection
	.set	nomips16
	.ent	rtk_delV6Connection
	.type	rtk_delV6Connection, @function
rtk_delV6Connection:
	.frame	$sp,80,$31		# vars= 32, regs= 8/0, args= 16, gp= 0
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
	lw	$10,0($4)
	lw	$11,40($4)
	lw	$9,4($4)
	lw	$8,8($4)
	lw	$7,12($4)
	xor	$11,$10,$11
	lw	$6,16($4)
	xor	$11,$11,$9
	lw	$5,20($4)
	xor	$11,$11,$8
	move	$17,$4
	xor	$11,$11,$7
	lw	$4,24($4)
	lw	$3,28($17)
	xor	$11,$11,$6
	xor	$11,$11,$5
	xor	$11,$11,$4
	xor	$11,$11,$3
	srl	$2,$10,8
	xor	$11,$11,$2
	srl	$2,$9,8
	xor	$11,$11,$2
	srl	$2,$8,8
	xor	$11,$11,$2
	srl	$2,$7,8
	xor	$11,$11,$2
	srl	$2,$6,16
	xor	$11,$11,$2
	srl	$12,$5,16
	lw	$2,36($17)
	xor	$11,$11,$12
	srl	$12,$4,16
	srl	$13,$3,16
	xor	$11,$11,$12
	xor	$11,$11,$13
	lhu	$12,32($17)
	srl	$13,$2,4
	xor	$11,$11,$13
	lui	$13,%hi(v6_path_table_list_max)
	lw	$13,%lo(v6_path_table_list_max)($13)
	lui	$19,%hi(v6_table_path)
	xor	$11,$11,$12
	lw	$12,%lo(v6_table_path)($19)
	srl	$18,$11,12
	sw	$4,40($sp)
	sw	$3,44($sp)
	xor	$11,$18,$11
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$7,28($sp)
	sw	$6,32($sp)
	sw	$5,36($sp)
	addiu	$4,$13,-1
	and	$3,$11,$4
	lw	$4,0($12)
	sll	$18,$3,4
	sll	$3,$3,2
	subu	$18,$18,$3
	addu	$3,$4,$18
	lw	$16,0($3)
	beq	$16,$0,$L82
	lui	$21,%hi(v6_path_list_free)

	lui	$22,%hi(v6_path_list_inuse)
	addiu	$21,$21,%lo(v6_path_list_free)
	addiu	$22,$22,%lo(v6_path_list_inuse)
	j	$L80
	addiu	$20,$17,16

$L75:
	lw	$16,52($16)
	beq	$16,$0,$L83
	lw	$31,76($sp)

	lw	$2,36($17)
$L80:
	lw	$3,36($16)
	bne	$3,$2,$L75
	nop

	lw	$3,40($16)
	lw	$2,40($17)
	bne	$3,$2,$L75
	move	$4,$16

	move	$5,$17
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L75
	addiu	$4,$16,16

	move	$5,$20
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L75
	nop

	lw	$3,32($16)
	lw	$2,32($17)
	bne	$3,$2,$L75
	nop

	lw	$2,52($16)
	beq	$2,$0,$L76
	nop

	lw	$3,56($16)
	sw	$3,56($2)
$L77:
	lw	$2,56($16)
	lw	$3,52($16)
	sw	$3,0($2)
	lw	$2,%lo(v6_table_path)($19)
	lw	$2,0($2)
	addu	$2,$2,$18
	lw	$3,8($2)
	addiu	$3,$3,-1
	sw	$3,8($2)
	lw	$3,60($16)
	beq	$3,$0,$L78
	move	$2,$22

	lw	$4,64($16)
	sw	$4,64($3)
$L79:
	lw	$4,64($16)
	lw	$5,60($16)
	addiu	$3,$16,60
	sw	$5,0($4)
	lw	$4,8($2)
	addiu	$4,$4,-1
	sw	$4,8($2)
	sw	$0,60($16)
	lw	$2,4($21)
	sw	$2,64($16)
	lw	$2,4($21)
	sw	$16,0($2)
	lw	$2,8($21)
	sw	$3,4($21)
	addiu	$2,$2,1
	j	$L75
	sw	$2,8($21)

$L82:
	lw	$31,76($sp)
$L83:
	lw	$22,72($sp)
	lw	$21,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	j	$31
	addiu	$sp,$sp,80

$L76:
	lw	$2,%lo(v6_table_path)($19)
	lw	$3,56($16)
	lw	$2,0($2)
	addu	$2,$2,$18
	j	$L77
	sw	$3,4($2)

$L78:
	lw	$3,64($16)
	j	$L79
	sw	$3,4($22)

	.set	macro
	.set	reorder
	.end	rtk_delV6Connection
	.size	rtk_delV6Connection, .-rtk_delV6Connection
	.align	2
	.globl	ipv6_fast_enter
	.set	nomips16
	.ent	ipv6_fast_enter
	.type	ipv6_fast_enter, @function
ipv6_fast_enter:
	.frame	$sp,296,$31		# vars= 240, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-296
	sw	$31,292($sp)
	sw	$fp,288($sp)
	sw	$23,284($sp)
	sw	$22,280($sp)
	sw	$21,276($sp)
	sw	$20,272($sp)
	sw	$19,268($sp)
	sw	$18,264($sp)
	sw	$17,260($sp)
	sw	$16,256($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header
	move	$17,$4
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipv6_hdr
	move	$16,$2
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_pppoe_flag
	move	$18,$2
	.set	macro
	.set	reorder

	bne	$2,$0,$L85
	lbu	$3,13($16)
	lbu	$2,12($16)
	sll	$3,$3,8
	or	$3,$3,$2
	li	$2,56710			# 0xdd86
	beq	$3,$2,$L85
$L86:
	move	$2,$0
$L100:
	lw	$31,292($sp)
$L121:
	lw	$fp,288($sp)
	lw	$23,284($sp)
	lw	$22,280($sp)
	lw	$21,276($sp)
	lw	$20,272($sp)
	lw	$19,268($sp)
	lw	$18,264($sp)
	lw	$17,260($sp)
	lw	$16,256($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,296
	.set	macro
	.set	reorder

$L85:
	.set	noreorder
	.set	nomacro
	jal	__ipv6_addr_type
	addiu	$4,$18,8
	.set	macro
	.set	reorder

	andi	$2,$2,0x20
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L100
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	__ipv6_addr_type
	addiu	$4,$18,24
	.set	macro
	.set	reorder

	andi	$2,$2,0x22
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L86
	li	$3,6			# 0x6
	.set	macro
	.set	reorder

	lbu	$2,6($18)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L87
	addiu	$4,$18,40
	.set	macro
	.set	reorder

	li	$3,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L88
	li	$3,44			# 0x2c
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L100
	move	$2,$0
	.set	macro
	.set	reorder

$L88:
	lw	$6,8($18)
	lw	$5,12($18)
	lw	$3,16($18)
	lw	$2,20($18)
	sw	$6,16($sp)
	sw	$5,20($sp)
	sw	$3,24($sp)
	sw	$2,28($sp)
	lw	$2,36($18)
	lw	$6,24($18)
	lw	$5,28($18)
	lw	$3,32($18)
	sw	$2,44($sp)
	sw	$6,32($sp)
	sw	$5,36($sp)
	sw	$3,40($sp)
	lbu	$22,6($18)
	li	$2,44			# 0x2c
	lhu	$19,40($18)
	.set	noreorder
	.set	nomacro
	beq	$22,$2,$L89
	lhu	$20,2($4)
	.set	macro
	.set	reorder

	andi	$23,$22,0xffff
	move	$21,$0
	sw	$0,240($sp)
	sw	$0,244($sp)
	sw	$0,248($sp)
$L90:
	lw	$9,16($sp)
	lw	$8,20($sp)
	lw	$7,24($sp)
	lw	$6,28($sp)
	lw	$5,32($sp)
	xor	$10,$8,$9
	lw	$4,36($sp)
	xor	$10,$10,$7
	lw	$3,40($sp)
	xor	$10,$10,$6
	lw	$2,44($sp)
	xor	$10,$10,$5
	xor	$10,$10,$4
	xor	$10,$10,$3
	xor	$10,$10,$2
	srl	$11,$9,8
	xor	$10,$10,$11
	srl	$11,$8,8
	xor	$10,$10,$11
	srl	$11,$7,8
	xor	$10,$10,$11
	srl	$11,$6,8
	xor	$10,$10,$11
	srl	$11,$5,16
	xor	$10,$10,$11
	srl	$11,$4,16
	xor	$10,$10,$11
	srl	$11,$3,16
	xor	$10,$10,$11
	srl	$11,$2,16
	xor	$10,$10,$11
	xor	$10,$10,$20
	lui	$11,%hi(v6_path_table_list_max)
	lw	$12,%lo(v6_path_table_list_max)($11)
	xor	$10,$10,$23
	srl	$11,$19,4
	xor	$10,$10,$11
	lui	$11,%hi(v6_table_path)
	lw	$11,%lo(v6_table_path)($11)
	srl	$13,$10,12
	sw	$4,132($sp)
	sw	$3,136($sp)
	sw	$2,140($sp)
	sw	$9,112($sp)
	sw	$8,116($sp)
	sw	$7,120($sp)
	sw	$6,124($sp)
	sw	$5,128($sp)
	addiu	$3,$12,-1
	xor	$10,$13,$10
	and	$2,$10,$3
	lw	$3,0($11)
	sll	$4,$2,4
	sll	$2,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$16,0($2)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L86
	addiu	$fp,$sp,16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L120
	lw	$2,36($16)
	.set	macro
	.set	reorder

$L98:
	lw	$16,52($16)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L100
	move	$2,$0
	.set	macro
	.set	reorder

	lw	$2,36($16)
$L120:
	bne	$2,$19,$L98
	lw	$2,40($16)
	.set	noreorder
	.set	nomacro
	bne	$2,$20,$L98
	move	$4,$16
	.set	macro
	.set	reorder

	move	$5,$fp
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L98
	addiu	$4,$16,16
	.set	macro
	.set	reorder

	addiu	$5,$sp,32
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	bne	$2,$0,$L98
	lw	$2,32($16)
	bne	$2,$22,$L98
	.set	noreorder
	.set	nomacro
	jal	rtl_ip6_route_input
	move	$4,$17
	.set	macro
	.set	reorder

	li	$fp,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$fp,$L100
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_ipv6_dst
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L100
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_ipv6_dst_check
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$fp,$L103
	.set	noreorder
	.set	nomacro
	jal	rtl_check_dst_input
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$fp,$L103
	lw	$2,248($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L102
	lw	$2,244($sp)
	.set	macro
	.set	reorder

	lw	$13,240($sp)
	lui	$3,%hi(V6_udp_cache_list_free)
	lw	$11,16($sp)
	lw	$10,20($sp)
	lw	$9,24($sp)
	lw	$8,28($sp)
	lw	$7,32($sp)
	lw	$6,36($sp)
	lw	$5,40($sp)
	lw	$4,44($sp)
	lw	$2,%lo(V6_udp_cache_list_free)($3)
	lw	$12,4($13)
	lui	$13,%hi(V6_max_udp_frag_entry)
	lw	$14,%lo(V6_max_udp_frag_entry)($13)
	sw	$11,144($sp)
	sw	$10,148($sp)
	sw	$9,152($sp)
	sw	$8,156($sp)
	sw	$7,160($sp)
	sw	$6,164($sp)
	sw	$5,168($sp)
	sw	$4,172($sp)
	sw	$11,192($sp)
	sw	$10,196($sp)
	sw	$9,200($sp)
	sw	$8,204($sp)
	sw	$7,176($sp)
	sw	$6,180($sp)
	sw	$5,184($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L103
	sw	$4,188($sp)
	.set	macro
	.set	reorder

	sw	$12,0($2)
	lw	$24,148($sp)
	lw	$15,152($sp)
	lw	$13,156($sp)
	lw	$25,144($sp)
	sw	$24,8($2)
	sw	$13,16($2)
	sw	$25,4($2)
	sw	$15,12($2)
	lw	$15,172($sp)
	lw	$25,164($sp)
	lw	$24,168($sp)
	lw	$21,160($sp)
	lw	$13,88($2)
	andi	$19,$19,0xffff
	andi	$20,$20,0xffff
	sw	$15,32($2)
	li	$15,1			# 0x1
	sw	$21,20($2)
	sw	$25,24($2)
	sw	$24,28($2)
	sw	$22,36($2)
	sw	$19,40($2)
	sw	$20,44($2)
	.set	noreorder
	.set	nomacro
	beq	$13,$0,$L104
	sb	$15,48($2)
	.set	macro
	.set	reorder

	lw	$15,92($2)
	addiu	$3,$3,%lo(V6_udp_cache_list_free)
	sw	$15,92($13)
$L105:
	xor	$24,$10,$11
	xor	$24,$24,$9
	xor	$24,$24,$8
	xor	$24,$24,$7
	xor	$24,$24,$6
	xor	$24,$24,$5
	lw	$13,92($2)
	lw	$15,88($2)
	xor	$24,$24,$4
	srl	$11,$11,8
	xor	$11,$24,$11
	srl	$10,$10,8
	xor	$10,$11,$10
	srl	$9,$9,8
	sw	$15,0($13)
	xor	$9,$10,$9
	srl	$8,$8,8
	xor	$8,$9,$8
	srl	$7,$7,16
	lw	$9,8($3)
	xor	$7,$8,$7
	srl	$6,$6,16
	xor	$6,$7,$6
	srl	$5,$5,16
	xor	$5,$6,$5
	srl	$4,$4,16
	addiu	$6,$9,-1
	sw	$6,8($3)
	xor	$4,$5,$4
	sw	$0,80($2)
	lui	$5,%hi(V6_udp_cache_table)
	lw	$3,%lo(V6_udp_cache_table)($5)
	xor	$12,$4,$12
	srl	$4,$4,16
	xor	$4,$12,$4
	xor	$23,$4,$23
	addiu	$4,$14,-1
	and	$23,$23,$4
	lw	$4,0($3)
	sll	$3,$23,4
	sll	$23,$23,2
	subu	$3,$3,$23
	addu	$4,$4,$3
	lw	$4,4($4)
	addiu	$7,$2,80
	sw	$4,84($2)
	lw	$6,%lo(V6_udp_cache_table)($5)
	addiu	$4,$2,52
	lw	$6,0($6)
	addu	$6,$6,$3
	lw	$6,4($6)
	sw	$2,0($6)
	lw	$6,%lo(V6_udp_cache_table)($5)
	lw	$6,0($6)
	addu	$6,$6,$3
	sw	$7,4($6)
	lw	$5,%lo(V6_udp_cache_table)($5)
	lw	$5,0($5)
	addu	$3,$5,$3
	lw	$5,8($3)
	addiu	$5,$5,1
	sw	$5,8($3)
	lui	$3,%hi(jiffies)
	lw	$3,%lo(jiffies)($3)
	addiu	$3,$3,1000
	.set	noreorder
	.set	nomacro
	jal	add_timer
	sw	$3,60($2)
	.set	macro
	.set	reorder

$L106:
	move	$4,$17
$L119:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dev
	move	$5,$0
	.set	macro
	.set	reorder

	lbu	$2,7($18)
	sltu	$3,$2,2
	bne	$3,$0,$L109
	addiu	$2,$2,-1
	sb	$2,7($18)
	lw	$5,48($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ip6_skb_dst_mtu
	move	$18,$2
	.set	macro
	.set	reorder

	sltu	$2,$2,$18
	beq	$2,$0,$L110
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_is_gso
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$0,$L109
$L110:
	.set	noreorder
	.set	nomacro
	jal	rtl_dst_allfrag
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$0,$L118
$L109:
	.set	noreorder
	.set	nomacro
	jal	kfree_skb
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L100
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L87:
	lw	$2,12($4)
	andi	$2,$2,0x700
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L88
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L121
	lw	$31,292($sp)
	.set	macro
	.set	reorder

$L89:
	lbu	$3,40($18)
	li	$2,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L91
	sw	$4,240($sp)
	.set	macro
	.set	reorder

	li	$23,44			# 0x2c
	move	$21,$0
	sw	$0,244($sp)
	.set	noreorder
	.set	nomacro
	j	$L90
	sw	$0,248($sp)
	.set	macro
	.set	reorder

$L91:
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$20, $20

 # 0 "" 2
 #NO_APP
	andi	$20,$20,0xffff
	andi	$3,$20,0xfff9
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L92
	andi	$2,$20,0xfff8
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L94
	sw	$0,244($sp)
	.set	macro
	.set	reorder

	andi	$20,$20,0x1
	xori	$20,$20,0x1
	sw	$20,244($sp)
$L94:
	lw	$10,16($sp)
	lw	$9,20($sp)
	lw	$8,24($sp)
	lw	$7,28($sp)
	lw	$6,32($sp)
	xor	$13,$9,$10
	lw	$5,36($sp)
	xor	$13,$13,$8
	lw	$3,40($sp)
	xor	$13,$13,$7
	lw	$2,44($sp)
	xor	$13,$13,$6
	xor	$13,$13,$5
	xor	$13,$13,$3
	xor	$13,$13,$2
	srl	$11,$10,8
	xor	$13,$13,$11
	srl	$11,$9,8
	xor	$13,$13,$11
	srl	$11,$8,8
	xor	$13,$13,$11
	srl	$11,$7,8
	xor	$13,$13,$11
	srl	$11,$6,16
	lw	$16,4($4)
	xor	$13,$13,$11
	srl	$4,$5,16
	xor	$13,$13,$4
	srl	$4,$3,16
	xor	$13,$13,$4
	lui	$4,%hi(V6_max_udp_frag_entry)
	srl	$11,$2,16
	lw	$12,%lo(V6_max_udp_frag_entry)($4)
	lui	$4,%hi(V6_udp_cache_table)
	xor	$11,$13,$11
	lw	$4,%lo(V6_udp_cache_table)($4)
	xori	$13,$16,0x11
	xor	$13,$13,$11
	srl	$11,$11,16
	sw	$2,76($sp)
	sw	$2,92($sp)
	sw	$10,48($sp)
	sw	$9,52($sp)
	sw	$8,56($sp)
	sw	$7,60($sp)
	sw	$6,64($sp)
	sw	$5,68($sp)
	sw	$3,72($sp)
	sw	$10,96($sp)
	sw	$9,100($sp)
	sw	$8,104($sp)
	sw	$7,108($sp)
	sw	$6,80($sp)
	sw	$5,84($sp)
	sw	$3,88($sp)
	xor	$11,$13,$11
	addiu	$2,$12,-1
	and	$2,$11,$2
	lw	$3,0($4)
	sll	$4,$2,4
	sll	$2,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$21,0($2)
	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L86
	addiu	$19,$sp,48
	.set	macro
	.set	reorder

	addiu	$22,$sp,64
	.set	noreorder
	.set	nomacro
	j	$L114
	li	$20,17			# 0x11
	.set	macro
	.set	reorder

$L95:
	lw	$21,80($21)
	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L100
	move	$2,$0
	.set	macro
	.set	reorder

$L114:
	lw	$2,0($21)
	.set	noreorder
	.set	nomacro
	bne	$16,$2,$L95
	addiu	$4,$21,4
	.set	macro
	.set	reorder

	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L95
	addiu	$4,$21,20
	.set	macro
	.set	reorder

	move	$5,$22
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	bne	$2,$0,$L95
	lw	$2,36($21)
	bne	$2,$20,$L95
	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L86
	li	$23,17			# 0x11
	.set	macro
	.set	reorder

	lw	$19,40($21)
	lw	$20,44($21)
	sw	$0,248($sp)
	.set	noreorder
	.set	nomacro
	j	$L90
	li	$22,17			# 0x11
	.set	macro
	.set	reorder

$L92:
	lw	$2,240($sp)
	li	$13,1			# 0x1
	lhu	$20,50($18)
	lhu	$19,8($2)
	li	$23,17			# 0x11
	move	$21,$0
	sw	$0,244($sp)
	sw	$13,248($sp)
	.set	noreorder
	.set	nomacro
	j	$L90
	li	$22,17			# 0x11
	.set	macro
	.set	reorder

$L104:
	lw	$13,92($2)
	addiu	$3,$3,%lo(V6_udp_cache_list_free)
	.set	noreorder
	.set	nomacro
	j	$L105
	sw	$13,4($3)
	.set	macro
	.set	reorder

$L118:
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	ip_finish_output6
	sw	$2,44($16)
	.set	macro
	.set	reorder

	lui	$3,%hi(statistic_ipv6_fp)
	lw	$4,%lo(statistic_ipv6_fp)($3)
	li	$2,1			# 0x1
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	j	$L100
	sw	$4,%lo(statistic_ipv6_fp)($3)
	.set	macro
	.set	reorder

$L102:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L119
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$21,$0,$L119
	.set	noreorder
	.set	nomacro
	jal	del_timer
	addiu	$4,$21,52
	.set	macro
	.set	reorder

	lw	$9,4($21)
	lw	$8,8($21)
	lw	$7,12($21)
	lw	$6,16($21)
	lw	$10,0($21)
	lhu	$11,36($21)
	sw	$9,224($sp)
	sw	$8,228($sp)
	sw	$7,232($sp)
	sw	$6,236($sp)
	lw	$5,20($21)
	xor	$12,$8,$9
	lw	$4,24($21)
	xor	$12,$12,$7
	lw	$2,28($21)
	xor	$12,$12,$6
	lw	$3,32($21)
	xor	$12,$12,$5
	xor	$12,$12,$4
	xor	$12,$12,$2
	xor	$12,$12,$3
	srl	$9,$9,8
	xor	$9,$12,$9
	srl	$8,$8,8
	xor	$8,$9,$8
	srl	$7,$7,8
	xor	$7,$8,$7
	srl	$6,$6,8
	xor	$6,$7,$6
	srl	$7,$5,16
	xor	$7,$6,$7
	srl	$6,$4,16
	sw	$2,216($sp)
	xor	$7,$7,$6
	srl	$6,$2,16
	lui	$2,%hi(V6_max_udp_frag_entry)
	sw	$5,208($sp)
	sw	$3,220($sp)
	sw	$4,212($sp)
	lw	$2,%lo(V6_max_udp_frag_entry)($2)
	xor	$6,$7,$6
	srl	$3,$3,16
	xor	$3,$6,$3
	xor	$5,$11,$10
	lw	$4,80($21)
	xor	$5,$5,$3
	srl	$3,$3,16
	xor	$3,$5,$3
	addiu	$2,$2,-1
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L107
	sb	$0,48($21)
	.set	macro
	.set	reorder

	lw	$5,84($21)
	sll	$3,$2,4
	sll	$2,$2,2
	sw	$5,84($4)
	subu	$3,$3,$2
	lui	$4,%hi(V6_udp_cache_table)
$L108:
	lw	$5,84($21)
	lw	$6,80($21)
	lui	$2,%hi(V6_udp_cache_list_free)
	addiu	$2,$2,%lo(V6_udp_cache_list_free)
	sw	$6,0($5)
	lw	$5,%lo(V6_udp_cache_table)($4)
	addiu	$4,$21,88
	lw	$5,0($5)
	addu	$3,$5,$3
	lw	$5,8($3)
	addiu	$5,$5,-1
	sw	$5,8($3)
	sw	$0,88($21)
	lw	$3,4($2)
	sw	$3,92($21)
	lw	$3,4($2)
	sw	$21,0($3)
	lw	$3,8($2)
	sw	$4,4($2)
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	j	$L106
	sw	$3,8($2)
	.set	macro
	.set	reorder

$L103:
	.set	noreorder
	.set	nomacro
	jal	rtl_dst_release
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L100
	move	$2,$0
	.set	macro
	.set	reorder

$L107:
	lui	$4,%hi(V6_udp_cache_table)
	lw	$5,%lo(V6_udp_cache_table)($4)
	sll	$6,$2,4
	sll	$3,$2,2
	lw	$2,0($5)
	lw	$5,84($21)
	subu	$3,$6,$3
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	j	$L108
	sw	$5,4($2)
	.set	macro
	.set	reorder

	.end	ipv6_fast_enter
	.size	ipv6_fast_enter, .-ipv6_fast_enter
	.align	2
	.globl	V6_udp_fragCache_init
	.set	nomips16
	.ent	V6_udp_fragCache_init
	.type	V6_udp_fragCache_init, @function
V6_udp_fragCache_init:
	.frame	$sp,56,$31		# vars= 8, regs= 7/0, args= 16, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$21,48($sp)
	sw	$20,44($sp)
	sw	$19,40($sp)
	sw	$18,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	lui	$19,%hi(kmalloc_caches)
	addiu	$19,$19,%lo(kmalloc_caches)
	move	$20,$4
	lw	$4,28($19)
	li	$5,32800			# 0x8020
	jal	kmem_cache_alloc
	lui	$17,%hi(V6_udp_cache_table)

	move	$21,$2
	beq	$2,$0,$L123
	sw	$2,%lo(V6_udp_cache_table)($17)

	lui	$2,%hi(V6_udp_cache_list_free)
	sll	$3,$20,2
	sll	$4,$20,4
	subu	$4,$4,$3
	addiu	$16,$2,%lo(V6_udp_cache_list_free)
	lui	$18,%hi(V6_max_udp_frag_entry)
	li	$5,32			# 0x20
	sw	$0,%lo(V6_udp_cache_list_free)($2)
	sw	$0,8($16)
	sw	$16,4($16)
	jal	__kmalloc
	sw	$20,%lo(V6_max_udp_frag_entry)($18)

	sw	$2,0($21)
	lw	$3,%lo(V6_udp_cache_table)($17)
	lw	$4,0($3)
	beq	$4,$0,$L125
	li	$2,-1			# 0xffffffffffffffff

	blez	$20,$L126
	move	$2,$0

	move	$3,$0
	j	$L127
	move	$5,$17

$L132:
	lw	$4,%lo(V6_udp_cache_table)($5)
	lw	$4,0($4)
$L127:
	addu	$4,$4,$2
	sw	$0,8($4)
	lw	$4,%lo(V6_udp_cache_table)($17)
	addiu	$3,$3,1
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$0,0($4)
	lw	$4,%lo(V6_udp_cache_table)($17)
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$4,4($4)
	bne	$3,$20,$L132
	addiu	$2,$2,12

$L126:
	lw	$2,%lo(V6_max_udp_frag_entry)($18)
	blez	$2,$L128
	lui	$20,%hi(V6_cache_timeout)

	addiu	$20,$20,%lo(V6_cache_timeout)
	j	$L129
	move	$17,$0

$L133:
	jal	init_timer_key
	sw	$2,16($sp)

	lw	$2,16($sp)
	sw	$2,72($2)
	sw	$20,68($2)
	sw	$0,88($2)
	lw	$3,4($16)
	addiu	$4,$2,88
	sw	$3,92($2)
	lw	$3,4($16)
	sw	$2,0($3)
	lw	$3,8($16)
	lw	$2,%lo(V6_max_udp_frag_entry)($18)
	sw	$4,4($16)
	addiu	$3,$3,1
	slt	$2,$17,$2
	beq	$2,$0,$L128
	sw	$3,8($16)

$L129:
	lw	$4,28($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,52
	move	$5,$0
	move	$6,$0
	bne	$2,$0,$L133
	move	$7,$0

$L123:
	move	$2,$0
$L125:
	lw	$31,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,56

$L128:
	j	$L125
	li	$2,1			# 0x1

	.set	macro
	.set	reorder
	.end	V6_udp_fragCache_init
	.size	V6_udp_fragCache_init, .-V6_udp_fragCache_init
	.align	2
	.globl	rtl_V6_Cache_Timer_update
	.set	nomips16
	.ent	rtl_V6_Cache_Timer_update
	.type	rtl_V6_Cache_Timer_update, @function
rtl_V6_Cache_Timer_update:
	.frame	$sp,88,$31		# vars= 48, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$19,80($sp)
	sw	$18,76($sp)
	sw	$17,72($sp)
	sw	$16,68($sp)
	lui	$2,%hi(jiffies)
	move	$5,$0
	lw	$17,%lo(jiffies)($2)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_protonum
	move	$16,$4
	.set	macro
	.set	reorder

	li	$3,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L142
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_protonum
	move	$5,$0
	.set	macro
	.set	reorder

	li	$3,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L143
	lw	$31,84($sp)
	.set	macro
	.set	reorder

$L145:
	lw	$19,80($sp)
	lw	$18,76($sp)
	lw	$17,72($sp)
	lw	$16,68($sp)
	li	$2,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,88
	.set	macro
	.set	reorder

$L142:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_udp_status
	move	$4,$16
	.set	macro
	.set	reorder

	andi	$2,$2,0x2
	bne	$2,$0,$L144
	move	$4,$0
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	move	$19,$2
$L137:
	addiu	$18,$sp,16
	li	$6,44			# 0x2c
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	move	$4,$18
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_protonum_ipv6
	move	$5,$0
	.set	macro
	.set	reorder

	move	$5,$0
	move	$6,$0
	move	$7,$18
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_v6_ip_by_dir
	sw	$2,48($sp)
	.set	macro
	.set	reorder

	addiu	$7,$sp,32
	move	$4,$16
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_v6_ip_by_dir
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_port_by_dir
	move	$6,$0
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$0
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	li	$6,1			# 0x1
	andi	$2,$2,0xffff
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_port_by_dir
	sw	$2,52($sp)
	.set	macro
	.set	reorder

	move	$4,$18
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	move	$5,$19
	andi	$2,$2,0xffff
	.set	noreorder
	.set	nomacro
	jal	rtk_idleV6Connection
	sw	$2,56($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L145
	lw	$31,84($sp)
	.set	macro
	.set	reorder

	addu	$5,$19,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_check_for_acc
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$31,84($sp)
	lw	$19,80($sp)
	lw	$18,76($sp)
	lw	$17,72($sp)
	lw	$16,68($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,88
	.set	macro
	.set	reorder

$L144:
	li	$4,1			# 0x1
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L137
	move	$19,$2
	.set	macro
	.set	reorder

$L143:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_tcp_state
	move	$4,$16
	.set	macro
	.set	reorder

	sltu	$2,$2,6
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L145
	lw	$31,84($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_get_timeouts
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L137
	move	$19,$2
	.set	macro
	.set	reorder

	.end	rtl_V6_Cache_Timer_update
	.size	rtl_V6_Cache_Timer_update, .-rtl_V6_Cache_Timer_update
	.align	2
	.globl	rtk_addV6Connection
	.set	nomips16
	.ent	rtk_addV6Connection
	.type	rtk_addV6Connection, @function
rtk_addV6Connection:
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
	move	$19,$5
	move	$16,$4
	.set	noreorder
	.set	nomacro
	jal	rtl_store_skb_ipv6_dst
	move	$4,$5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	move	$4,$19
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_ip6_route_input
	sw	$2,120($sp)
	.set	macro
	.set	reorder

	li	$3,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L175
	lw	$31,172($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_ipv6_dst
	move	$4,$19
	.set	macro
	.set	reorder

	beq	$2,$0,$L171
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_ipv6_dst_dev
	move	$4,$19
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_dst_release
	sw	$2,132($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ipv6_dst
	move	$4,$19
	.set	macro
	.set	reorder

	lw	$2,132($sp)
	sw	$2,128($sp)
$L149:
	lw	$10,0($16)
	lw	$11,40($16)
	lw	$9,4($16)
	lw	$8,8($16)
	lw	$7,12($16)
	xor	$11,$10,$11
	lw	$6,16($16)
	xor	$11,$11,$9
	lw	$5,20($16)
	xor	$11,$11,$8
	lw	$4,24($16)
	xor	$11,$11,$7
	lw	$3,28($16)
	xor	$11,$11,$6
	xor	$11,$11,$5
	xor	$11,$11,$4
	xor	$11,$11,$3
	srl	$2,$10,8
	xor	$11,$11,$2
	srl	$2,$9,8
	xor	$11,$11,$2
	srl	$2,$8,8
	xor	$11,$11,$2
	srl	$2,$7,8
	xor	$11,$11,$2
	srl	$2,$6,16
	xor	$11,$11,$2
	srl	$12,$5,16
	lw	$2,36($16)
	xor	$11,$11,$12
	srl	$12,$4,16
	srl	$13,$3,16
	xor	$11,$11,$12
	lhu	$12,32($16)
	xor	$11,$11,$13
	lui	$23,%hi(v6_path_table_list_max)
	srl	$13,$2,4
	xor	$11,$11,$13
	lui	$20,%hi(v6_table_path)
	lw	$13,%lo(v6_path_table_list_max)($23)
	xor	$11,$11,$12
	lw	$12,%lo(v6_table_path)($20)
	srl	$14,$11,12
	sw	$4,72($sp)
	sw	$3,76($sp)
	sw	$10,48($sp)
	sw	$9,52($sp)
	sw	$8,56($sp)
	sw	$7,60($sp)
	sw	$6,64($sp)
	sw	$5,68($sp)
	addiu	$4,$13,-1
	xor	$11,$14,$11
	and	$3,$11,$4
	lw	$4,0($12)
	sll	$21,$3,4
	sll	$3,$3,2
	subu	$21,$21,$3
	addu	$3,$4,$21
	lw	$17,0($3)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L150
	addiu	$18,$16,16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L178
	lw	$3,36($17)
	.set	macro
	.set	reorder

$L151:
	lw	$17,52($17)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L176
	lui	$22,%hi(v6_path_list_free)
	.set	macro
	.set	reorder

	lw	$2,36($16)
	lw	$3,36($17)
$L178:
	bne	$3,$2,$L151
	lw	$3,40($17)
	lw	$2,40($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L151
	move	$4,$17
	.set	macro
	.set	reorder

	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L151
	addiu	$4,$17,16
	.set	macro
	.set	reorder

	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	bne	$2,$0,$L151
	lw	$12,32($17)
	lw	$2,32($16)
	bne	$12,$2,$L151
	.set	noreorder
	.set	nomacro
	j	$L174
	lw	$10,16($16)
	.set	macro
	.set	reorder

$L150:
	lui	$22,%hi(v6_path_list_free)
$L176:
	lw	$18,%lo(v6_path_list_free)($22)
	beq	$18,$0,$L170
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_protocol
	move	$4,$19
	.set	macro
	.set	reorder

	move	$4,$19
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$3,$2,0xffff
	.set	noreorder
	.set	nomacro
	jal	rtl_ipv6_hdr
	sw	$3,132($sp)
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$17,$2
	.set	macro
	.set	reorder

	sw	$2,124($sp)
	lw	$6,8($17)
	lw	$5,12($17)
	lw	$4,16($17)
	lw	$2,20($17)
	sw	$6,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$2,28($sp)
	lw	$6,24($17)
	lw	$5,28($17)
	lw	$4,32($17)
	lw	$2,36($17)
	lhu	$7,40($17)
	addiu	$fp,$17,40
	sw	$6,32($sp)
	sw	$5,36($sp)
	sw	$4,40($sp)
	sw	$7,116($sp)
	sw	$2,44($sp)
	lhu	$2,2($fp)
	move	$4,$19
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	sw	$2,112($sp)
	.set	macro
	.set	reorder

	lw	$6,0($16)
	lw	$5,4($16)
	lw	$4,8($16)
	lw	$2,12($16)
	sw	$6,8($17)
	sw	$5,12($17)
	sw	$4,16($17)
	sw	$2,20($17)
	lw	$2,36($16)
	sh	$2,40($17)
	lw	$2,28($16)
	lw	$6,16($16)
	lw	$5,20($16)
	lw	$4,24($16)
	sw	$6,24($17)
	sw	$5,28($17)
	sw	$4,32($17)
	sw	$2,36($17)
	lw	$2,40($16)
	sh	$2,2($fp)
	lw	$3,132($sp)
	li	$2,34525			# 0x86dd
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L172
	lui	$2,%hi(nf_hooks+640)
	.set	macro
	.set	reorder

$L155:
	lw	$5,124($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$19
	.set	macro
	.set	reorder

	lw	$4,20($sp)
	lw	$3,24($sp)
	lw	$2,28($sp)
	lw	$5,16($sp)
	sw	$4,12($17)
	sw	$3,16($17)
	sw	$5,8($17)
	sw	$2,20($17)
	lw	$7,116($sp)
	sh	$7,40($17)
	lw	$4,36($sp)
	lw	$2,40($sp)
	lw	$3,44($sp)
	lw	$5,32($sp)
	sw	$4,28($17)
	sw	$3,36($17)
	sw	$5,24($17)
	sw	$2,32($17)
	lw	$2,112($sp)
	sh	$2,2($fp)
	lw	$5,0($16)
	lw	$4,4($16)
	lw	$3,8($16)
	lw	$2,12($16)
	sw	$5,0($18)
	sw	$4,4($18)
	sw	$3,8($18)
	sw	$2,12($18)
	lw	$3,24($16)
	lw	$2,28($16)
	lw	$5,16($16)
	lw	$4,20($16)
	sw	$5,16($18)
	sw	$4,20($18)
	sw	$2,28($18)
	sw	$3,24($18)
	lw	$3,32($16)
	lw	$2,60($18)
	sw	$3,32($18)
	lw	$3,36($16)
	sw	$3,36($18)
	lw	$3,40($16)
	sw	$3,40($18)
	lui	$3,%hi(jiffies)
	lw	$3,%lo(jiffies)($3)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L158
	sw	$3,44($18)
	.set	macro
	.set	reorder

	lw	$3,64($18)
	lui	$22,%hi(v6_path_list_free)
	addiu	$22,$22,%lo(v6_path_list_free)
	sw	$3,64($2)
$L159:
	lw	$3,64($18)
	lw	$4,60($18)
	lui	$2,%hi(v6_path_list_inuse)
	addiu	$2,$2,%lo(v6_path_list_inuse)
	sw	$4,0($3)
	lw	$4,8($22)
	addiu	$5,$18,60
	addiu	$3,$18,52
	addiu	$4,$4,-1
	sw	$4,8($22)
	sw	$0,60($18)
	lw	$4,4($2)
	sw	$4,64($18)
	lw	$4,4($2)
	sw	$18,0($4)
	lw	$4,8($2)
	sw	$5,4($2)
	addiu	$4,$4,1
	sw	$4,8($2)
	sw	$0,52($18)
	lw	$2,%lo(v6_table_path)($20)
	lw	$2,0($2)
	addu	$2,$2,$21
	lw	$2,4($2)
	sw	$2,56($18)
	lw	$2,%lo(v6_table_path)($20)
	lw	$2,0($2)
	addu	$2,$2,$21
	lw	$2,4($2)
	sw	$18,0($2)
	lw	$2,%lo(v6_table_path)($20)
	lw	$2,0($2)
	addu	$2,$2,$21
	sw	$3,4($2)
	lw	$2,%lo(v6_table_path)($20)
	lw	$2,0($2)
	addu	$21,$2,$21
	lw	$2,8($21)
	addiu	$2,$2,1
	sw	$2,8($21)
$L170:
	lw	$12,32($16)
$L152 = .
	lw	$10,16($16)
$L174:
	lw	$11,36($16)
	lw	$9,20($16)
	lw	$8,24($16)
	lw	$7,28($16)
	xor	$11,$10,$11
	lw	$6,0($16)
	xor	$11,$11,$9
	lw	$5,4($16)
	xor	$11,$11,$8
	lw	$4,8($16)
	xor	$11,$11,$7
	lw	$3,12($16)
	xor	$11,$11,$6
	xor	$11,$11,$5
	xor	$11,$11,$4
	xor	$11,$11,$3
	srl	$2,$10,8
	xor	$11,$11,$2
	srl	$2,$9,8
	xor	$11,$11,$2
	srl	$2,$8,8
	xor	$11,$11,$2
	srl	$2,$7,8
	xor	$11,$11,$2
	srl	$2,$6,16
	xor	$11,$11,$2
	srl	$13,$5,16
	lw	$2,40($16)
	xor	$11,$11,$13
	srl	$13,$4,16
	xor	$11,$11,$13
	srl	$13,$3,16
	xor	$11,$11,$13
	srl	$13,$2,4
	xor	$11,$11,$13
	andi	$12,$12,0xffff
	lw	$13,%lo(v6_path_table_list_max)($23)
	xor	$11,$11,$12
	lw	$12,%lo(v6_table_path)($20)
	srl	$14,$11,12
	sw	$4,104($sp)
	sw	$3,108($sp)
	sw	$10,80($sp)
	sw	$9,84($sp)
	sw	$8,88($sp)
	sw	$7,92($sp)
	sw	$6,96($sp)
	sw	$5,100($sp)
	addiu	$4,$13,-1
	xor	$11,$14,$11
	and	$3,$11,$4
	lw	$4,0($12)
	sll	$21,$3,4
	sll	$3,$3,2
	subu	$21,$21,$3
	addu	$3,$4,$21
	lw	$17,0($3)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L160
	addiu	$18,$16,16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L179
	lw	$3,36($17)
	.set	macro
	.set	reorder

$L161:
	lw	$17,52($17)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L177
	lui	$22,%hi(v6_path_list_free)
	.set	macro
	.set	reorder

	lw	$2,40($16)
	lw	$3,36($17)
$L179:
	bne	$3,$2,$L161
	lw	$3,40($17)
	lw	$2,36($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L161
	move	$4,$17
	.set	macro
	.set	reorder

	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L161
	addiu	$4,$17,16
	.set	macro
	.set	reorder

	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	bne	$2,$0,$L161
	lw	$3,32($17)
	lw	$2,32($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L161
	lw	$31,172($sp)
	.set	macro
	.set	reorder

$L175:
	lw	$fp,168($sp)
	lw	$23,164($sp)
	lw	$22,160($sp)
	lw	$21,156($sp)
	lw	$20,152($sp)
	lw	$19,148($sp)
	lw	$18,144($sp)
	lw	$17,140($sp)
	lw	$16,136($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,176
	.set	macro
	.set	reorder

$L160:
	lui	$22,%hi(v6_path_list_free)
$L177:
	lw	$18,%lo(v6_path_list_free)($22)
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L175
	lw	$31,172($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_protocol
	move	$4,$19
	.set	macro
	.set	reorder

	move	$4,$19
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$3,$2,0xffff
	.set	noreorder
	.set	nomacro
	jal	rtl_ipv6_hdr
	sw	$3,132($sp)
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$17,$2
	.set	macro
	.set	reorder

	sw	$2,116($sp)
	lw	$6,8($17)
	lw	$5,12($17)
	lw	$4,16($17)
	lw	$2,20($17)
	sw	$6,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$2,28($sp)
	lw	$6,24($17)
	lw	$5,28($17)
	lw	$4,32($17)
	lw	$2,36($17)
	lhu	$7,40($17)
	addiu	$23,$17,40
	sw	$6,32($sp)
	sw	$5,36($sp)
	sw	$4,40($sp)
	sw	$2,44($sp)
	sw	$7,112($sp)
	move	$4,$19
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	lhu	$fp,2($23)
	.set	macro
	.set	reorder

	lw	$6,16($16)
	lw	$5,20($16)
	lw	$4,24($16)
	lw	$2,28($16)
	sw	$6,8($17)
	sw	$5,12($17)
	sw	$4,16($17)
	sw	$2,20($17)
	lw	$2,40($16)
	sh	$2,40($17)
	lw	$2,12($16)
	lw	$6,0($16)
	lw	$5,4($16)
	lw	$4,8($16)
	sw	$6,24($17)
	sw	$5,28($17)
	sw	$4,32($17)
	sw	$2,36($17)
	lw	$2,36($16)
	sh	$2,2($23)
	lw	$3,132($sp)
	li	$2,34525			# 0x86dd
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L173
	lui	$2,%hi(nf_hooks+640)
	.set	macro
	.set	reorder

$L163:
	lw	$5,116($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$19
	.set	macro
	.set	reorder

	lw	$4,20($sp)
	lw	$3,24($sp)
	lw	$2,28($sp)
	lw	$5,16($sp)
	sw	$4,12($17)
	sw	$3,16($17)
	sw	$5,8($17)
	sw	$2,20($17)
	lw	$2,112($sp)
	sh	$2,40($17)
	lw	$4,36($sp)
	lw	$2,40($sp)
	lw	$3,44($sp)
	lw	$5,32($sp)
	sw	$4,28($17)
	sw	$3,36($17)
	sw	$5,24($17)
	sw	$2,32($17)
	sh	$fp,2($23)
	lw	$5,16($16)
	lw	$4,20($16)
	lw	$3,24($16)
	lw	$2,28($16)
	sw	$5,0($18)
	sw	$4,4($18)
	sw	$3,8($18)
	sw	$2,12($18)
	lw	$2,12($16)
	lw	$3,8($16)
	lw	$5,0($16)
	lw	$4,4($16)
	sw	$5,16($18)
	sw	$4,20($18)
	sw	$3,24($18)
	sw	$2,28($18)
	lw	$3,32($16)
	lw	$2,60($18)
	sw	$3,32($18)
	lw	$3,40($16)
	sw	$3,36($18)
	lw	$3,36($16)
	sw	$3,40($18)
	lui	$3,%hi(jiffies)
	lw	$3,%lo(jiffies)($3)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L166
	sw	$3,44($18)
	.set	macro
	.set	reorder

	lw	$3,64($18)
	lui	$22,%hi(v6_path_list_free)
	addiu	$22,$22,%lo(v6_path_list_free)
	sw	$3,64($2)
$L167:
	lw	$2,64($18)
	lw	$4,60($18)
	lui	$3,%hi(v6_path_list_inuse)
	addiu	$3,$3,%lo(v6_path_list_inuse)
	sw	$4,0($2)
	lw	$2,8($22)
	addiu	$6,$18,60
	addiu	$4,$18,52
	addiu	$2,$2,-1
	sw	$2,8($22)
	sw	$0,60($18)
	lw	$5,4($3)
	li	$2,1			# 0x1
	sw	$5,64($18)
	lw	$5,4($3)
	sw	$18,0($5)
	lw	$5,8($3)
	sw	$6,4($3)
	addiu	$5,$5,1
	sw	$5,8($3)
	sw	$0,52($18)
	lw	$3,%lo(v6_table_path)($20)
	lw	$3,0($3)
	addu	$3,$3,$21
	lw	$3,4($3)
	sw	$3,56($18)
	lw	$3,%lo(v6_table_path)($20)
	lw	$3,0($3)
	addu	$3,$3,$21
	lw	$3,4($3)
	sw	$18,0($3)
	lw	$3,%lo(v6_table_path)($20)
	lw	$3,0($3)
	addu	$3,$3,$21
	sw	$4,4($3)
	lw	$3,%lo(v6_table_path)($20)
	lw	$3,0($3)
	addu	$21,$3,$21
	lw	$3,8($21)
	addiu	$3,$3,1
	sw	$3,8($21)
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

$L173:
	lw	$3,%lo(nf_hooks+640)($2)
	addiu	$2,$2,%lo(nf_hooks+640)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L164
	lw	$6,128($sp)
	.set	macro
	.set	reorder

	move	$4,$19
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt6_do_table
	move	$7,$0
	.set	macro
	.set	reorder

$L164:
	lui	$2,%hi(nf_hooks+672)
	lw	$3,%lo(nf_hooks+672)($2)
	addiu	$2,$2,%lo(nf_hooks+672)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L165
	lw	$7,120($sp)
	.set	macro
	.set	reorder

	move	$4,$19
	li	$5,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt6_do_table
	move	$6,$0
	.set	macro
	.set	reorder

$L165:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L163
	sw	$2,48($18)
	.set	macro
	.set	reorder

$L172:
	lw	$3,%lo(nf_hooks+640)($2)
	addiu	$2,$2,%lo(nf_hooks+640)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L156
	lw	$6,120($sp)
	.set	macro
	.set	reorder

	move	$4,$19
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt6_do_table
	move	$7,$0
	.set	macro
	.set	reorder

$L156:
	lui	$2,%hi(nf_hooks+672)
	lw	$3,%lo(nf_hooks+672)($2)
	addiu	$2,$2,%lo(nf_hooks+672)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L157
	lw	$7,128($sp)
	.set	macro
	.set	reorder

	move	$4,$19
	li	$5,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt6_do_table
	move	$6,$0
	.set	macro
	.set	reorder

$L157:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L155
	sw	$2,48($18)
	.set	macro
	.set	reorder

$L171:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ipv6_dst
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L149
	sw	$0,128($sp)
	.set	macro
	.set	reorder

$L166:
	lw	$2,64($18)
	addiu	$22,$22,%lo(v6_path_list_free)
	.set	noreorder
	.set	nomacro
	j	$L167
	sw	$2,4($22)
	.set	macro
	.set	reorder

$L158:
	lw	$2,64($18)
	addiu	$22,$22,%lo(v6_path_list_free)
	.set	noreorder
	.set	nomacro
	j	$L159
	sw	$2,4($22)
	.set	macro
	.set	reorder

	.end	rtk_addV6Connection
	.size	rtk_addV6Connection, .-rtk_addV6Connection
	.globl	fast_ipv6_fw
	.section	.bss,"aw",@nobits
	.align	2
	.type	fast_ipv6_fw, @object
	.size	fast_ipv6_fw, 4
fast_ipv6_fw:
	.space	4
	.globl	ipv6_fast_forward_proc_fops
	.data
	.align	2
	.type	ipv6_fast_forward_proc_fops, @object
	.size	ipv6_fast_forward_proc_fops, 108
ipv6_fast_forward_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	ipv6_fast_forward_single_write
	.space	28
	.word	ipv6_fast_forward_single_open
	.space	4
	.word	single_release
	.space	52
	.local	ipv6_res
	.comm	ipv6_res,4,4
	.local	v6_path_table_list_max
	.comm	v6_path_table_list_max,4,4
	.local	V6_max_udp_frag_entry
	.comm	V6_max_udp_frag_entry,4,4
	.globl	v6_path_list_inuse
	.section	.bss
	.align	2
	.type	v6_path_list_inuse, @object
	.size	v6_path_list_inuse, 12
v6_path_list_inuse:
	.space	12
	.globl	v6_path_list_free
	.align	2
	.type	v6_path_list_free, @object
	.size	v6_path_list_free, 12
v6_path_list_free:
	.space	12
	.globl	v6_table_path
	.align	2
	.type	v6_table_path, @object
	.size	v6_table_path, 4
v6_table_path:
	.space	4
	.globl	V6_udp_cache_list_free
	.align	2
	.type	V6_udp_cache_list_free, @object
	.size	V6_udp_cache_list_free, 12
V6_udp_cache_list_free:
	.space	12
	.globl	V6_udp_cache_table
	.align	2
	.type	V6_udp_cache_table, @object
	.size	V6_udp_cache_table, 4
V6_udp_cache_table:
	.space	4
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
