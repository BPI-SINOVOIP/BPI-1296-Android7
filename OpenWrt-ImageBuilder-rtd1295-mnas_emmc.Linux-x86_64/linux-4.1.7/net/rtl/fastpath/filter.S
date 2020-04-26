/*
 * Copyright (C) 2017 Realtek Semiconductor Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

	.file	1 "filter.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.text
	.align	2
	.globl	str2hexnum
	.set	nomips16
	.ent	str2hexnum
	.type	str2hexnum, @function
str2hexnum:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	lbu	$3,0($4)
	beq	$3,$0,$L13
	move	$2,$0

$L9:
	addiu	$6,$3,-97
	addiu	$5,$3,-48
	sltu	$7,$6,6
	sltu	$6,$5,10
	addiu	$4,$4,1
	bne	$6,$0,$L4
	sll	$2,$2,4

	addiu	$5,$3,-65
	sltu	$6,$5,6
	bne	$7,$0,$L4
	addiu	$5,$3,-87

	beq	$6,$0,$L12
	addiu	$5,$3,-55

$L4:
	lbu	$3,0($4)
	bne	$3,$0,$L9
	addu	$2,$5,$2

$L13:
	j	$31
	nop

$L12:
	j	$31
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	str2hexnum
	.size	str2hexnum, .-str2hexnum
	.align	2
	.globl	scan_table
	.set	nomips16
	.ent	scan_table
	.type	scan_table, @function
scan_table:
	.frame	$sp,56,$31		# vars= 0, regs= 9/0, args= 16, gp= 0
	.mask	0x80ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$23,48($sp)
	sw	$22,44($sp)
	sw	$21,40($sp)
	sw	$20,36($sp)
	sw	$19,32($sp)
	sw	$18,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$18,%hi(url_filter_flag)
	move	$16,$6
	move	$21,$4
	move	$20,$5
	move	$19,$7
	bne	$6,$0,$L34
	sw	$0,%lo(url_filter_flag)($18)

$L15:
	li	$2,-1			# 0xffffffffffffffff
$L23:
	lw	$31,52($sp)
$L39:
	lw	$23,48($sp)
	lw	$22,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,56

$L34:
	lw	$2,28($6)
	beq	$2,$0,$L23
	li	$2,-1			# 0xffffffffffffffff

	lw	$2,24($6)
	sltu	$2,$2,2
	bne	$2,$0,$L16
	addiu	$22,$6,8

	lw	$17,8($6)
	beq	$22,$17,$L37
	lui	$2,%hi(url_filter_mode)

$L31:
	lw	$2,16($17)
	andi	$3,$2,0x1
	bne	$3,$0,$L17
	move	$23,$17

	lw	$3,20($17)
	beq	$20,$3,$L38
	andi	$3,$2,0x200

$L18:
	lw	$17,0($17)
	bne	$22,$17,$L31
	nop

$L27 = .
	beq	$23,$0,$L16
	lw	$2,%lo(url_filter_flag)($18)

	bne	$2,$0,$L20
	nop

	lw	$3,16($23)
	andi	$2,$3,0x1
	ext	$4,$3,3,1
	movz	$4,$0,$2
	j	$L23
	move	$2,$4

$L17:
	andi	$3,$2,0x200
$L38:
	beq	$3,$0,$L19
	nop

	andi	$2,$2,0x2
	beq	$2,$0,$L18
	lw	$6,0($19)

	beq	$6,$0,$L15
	move	$4,$21

	lw	$5,20($17)
	jal	scan_table
	move	$7,$6

	beq	$2,$0,$L18
	nop

$L20:
	j	$L23
	li	$2,1			# 0x1

$L19:
	andi	$3,$2,0x100
	beq	$3,$0,$L35
	move	$4,$21

$L21:
	andi	$3,$2,0x2
	beq	$3,$0,$L36
	andi	$3,$2,0x1

	lw	$6,0($19)
	beq	$6,$0,$L15
	move	$4,$21

	lw	$5,20($17)
	jal	scan_table
	move	$7,$6

	beq	$2,$0,$L18
	nop

	lw	$3,16($17)
	andi	$2,$3,0x1
	beq	$2,$0,$L24
	nop

	lui	$2,%hi(url_filter_mode)
	lw	$4,%lo(url_filter_mode)($2)
	bne	$4,$0,$L23
	ext	$2,$3,3,1

	xori	$2,$3,0x8
	j	$L23
	ext	$2,$2,3,1

$L35:
	lw	$2,28($16)
	jalr	$2
	addiu	$5,$17,28

	beq	$2,$0,$L18
	nop

	j	$L21
	lw	$2,16($17)

$L24:
	lui	$2,%hi(url_filter_mode)
	lw	$2,%lo(url_filter_mode)($2)
	j	$L23
	sltu	$2,$2,1

$L16:
	lui	$2,%hi(url_filter_mode)
$L37:
	lw	$2,%lo(url_filter_mode)($2)
	beq	$2,$0,$L15
	li	$2,1			# 0x1

	j	$L39
	lw	$31,52($sp)

$L36:
	beq	$3,$0,$L20
	nop

	xori	$2,$2,0x8
	j	$L23
	ext	$2,$2,3,1

	.set	macro
	.set	reorder
	.end	scan_table
	.size	scan_table, .-scan_table
	.align	2
	.globl	strtoint
	.set	nomips16
	.ent	strtoint
	.type	strtoint, @function
strtoint:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	lb	$3,0($4)
	beq	$3,$0,$L41
	addiu	$5,$3,-48

	sltu	$5,$5,10
	bne	$5,$0,$L42
	move	$2,$0

	j	$L46
	nop

$L44:
	beq	$7,$0,$L46
	nop

$L42:
	addiu	$4,$4,1
	lb	$5,0($4)
	sll	$6,$2,3
	sll	$2,$2,1
	addu	$2,$2,$6
	addiu	$7,$5,-48
	addiu	$6,$3,-48
	sltu	$7,$7,10
	move	$3,$5
	bne	$5,$0,$L44
	addu	$2,$6,$2

	j	$31
	nop

$L41:
	move	$2,$0
$L46:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	strtoint
	.size	strtoint, .-strtoint
	.align	2
	.globl	clear_attack_address
	.set	nomips16
	.ent	clear_attack_address
	.type	clear_attack_address, @function
clear_attack_address:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	lui	$7,%hi(attack_saddr)
	lui	$6,%hi(attack_daddr)
	addiu	$7,$7,%lo(attack_saddr)
	addiu	$6,$6,%lo(attack_daddr)
	move	$2,$0
	li	$5,108			# 0x6c
$L49:
	addu	$4,$7,$2
	addu	$3,$6,$2
	addiu	$2,$2,4
	sw	$0,0($4)
	bne	$2,$5,$L49
	sw	$0,0($3)

	j	$31
	nop

	.set	macro
	.set	reorder
	.end	clear_attack_address
	.size	clear_attack_address, .-clear_attack_address
	.align	2
	.globl	filter_addconnect
	.set	nomips16
	.ent	filter_addconnect
	.type	filter_addconnect, @function
filter_addconnect:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	andi	$3,$4,0xff
	lui	$2,%hi(ConnectedIp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(ConnectedIp)
	addu	$2,$3,$2
	j	$31
	sw	$4,0($2)

	.set	macro
	.set	reorder
	.end	filter_addconnect
	.size	filter_addconnect, .-filter_addconnect
	.align	2
	.globl	filter_delconnect
	.set	nomips16
	.ent	filter_delconnect
	.type	filter_delconnect, @function
filter_delconnect:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	andi	$4,$4,0x00ff
	lui	$2,%hi(ConnectedIp)
	sll	$4,$4,2
	addiu	$2,$2,%lo(ConnectedIp)
	addu	$4,$4,$2
	j	$31
	sw	$0,0($4)

	.set	macro
	.set	reorder
	.end	filter_delconnect
	.size	filter_delconnect, .-filter_delconnect
	.align	2
	.globl	filter_checkConnect
	.set	nomips16
	.ent	filter_checkConnect
	.type	filter_checkConnect, @function
filter_checkConnect:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	andi	$3,$4,0xff
	lui	$2,%hi(ConnectedIp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(ConnectedIp)
	addu	$2,$3,$2
	lw	$3,0($2)
	beq	$3,$0,$L60
	li	$2,1			# 0x1

	xor	$2,$4,$3
	sltu	$2,$0,$2
$L60:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	filter_checkConnect
	.size	filter_checkConnect, .-filter_checkConnect
	.align	2
	.globl	register_customRspHook
	.set	nomips16
	.ent	register_customRspHook
	.type	register_customRspHook, @function
register_customRspHook:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	beq	$4,$0,$L62
	lui	$2,%hi(fast_path_cusRsp401_func)

	sw	$4,%lo(fast_path_cusRsp401_func)($2)
$L63:
	beq	$5,$0,$L64
	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)

	sw	$5,%lo(fast_path_cusRspTCPFinAck_func)($2)
$L65:
	beq	$6,$0,$L66
	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)

	j	$31
	sw	$6,%lo(fast_path_cusRspTCPEndAck_func)($2)

$L66:
	lui	$3,%hi(GenerateTCPENDACK)
	addiu	$3,$3,%lo(GenerateTCPENDACK)
	j	$31
	sw	$3,%lo(fast_path_cusRspTCPEndAck_func)($2)

$L64:
	lui	$3,%hi(GenerateTCPFINACK)
	addiu	$3,$3,%lo(GenerateTCPFINACK)
	j	$L65
	sw	$3,%lo(fast_path_cusRspTCPFinAck_func)($2)

$L62:
	lui	$3,%hi(GenerateHTTP401)
	addiu	$3,$3,%lo(GenerateHTTP401)
	j	$L63
	sw	$3,%lo(fast_path_cusRsp401_func)($2)

	.set	macro
	.set	reorder
	.end	register_customRspHook
	.size	register_customRspHook, .-register_customRspHook
	.align	2
	.globl	unregister_customRspHook
	.set	nomips16
	.ent	unregister_customRspHook
	.type	unregister_customRspHook, @function
unregister_customRspHook:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	lui	$2,%hi(fast_path_cusRsp401_func)
	sw	$0,%lo(fast_path_cusRsp401_func)($2)
	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)
	sw	$0,%lo(fast_path_cusRspTCPFinAck_func)($2)
	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)
	j	$31
	sw	$0,%lo(fast_path_cusRspTCPEndAck_func)($2)

	.set	macro
	.set	reorder
	.end	unregister_customRspHook
	.size	unregister_customRspHook, .-unregister_customRspHook
	.align	2
	.set	nomips16
	.ent	dos_single_open
	.type	dos_single_open, @function
dos_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	move	$4,$5
	lui	$5,%hi(dos_read_proc)
	addiu	$5,$5,%lo(dos_read_proc)
	move	$6,$0
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	dos_single_open
	.size	dos_single_open, .-dos_single_open
	.align	2
	.globl	filter_table_single_open
	.set	nomips16
	.ent	filter_table_single_open
	.type	filter_table_single_open, @function
filter_table_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	move	$4,$5
	lui	$5,%hi(filter_table_read_proc)
	addiu	$5,$5,%lo(filter_table_read_proc)
	move	$6,$0
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	filter_table_single_open
	.size	filter_table_single_open, .-filter_table_single_open
	.align	2
	.globl	filter_table_head_init
	.set	nomips16
	.ent	filter_table_head_init
	.type	filter_table_head_init, @function
filter_table_head_init:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(table_list_head)
	addiu	$4,$16,%lo(table_list_head)
	move	$5,$0
	jal	memset
	li	$6,32			# 0x20

	lui	$3,%hi(table_list_head+8)
	addiu	$2,$16,%lo(table_list_head)
	addiu	$3,$3,%lo(table_list_head+8)
	li	$4,1			# 0x1
	sw	$4,24($2)
	sw	$3,12($2)
	sw	$2,4($2)
	sw	$3,8($2)
	sw	$0,16($2)
	sw	$0,20($2)
	sw	$0,28($2)
	sw	$2,%lo(table_list_head)($16)
	lw	$31,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_table_head_init
	.size	filter_table_head_init, .-filter_table_head_init
	.align	2
	.globl	check_esc_end
	.set	nomips16
	.ent	check_esc_end
	.type	check_esc_end, @function
check_esc_end:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	seb	$17,$5
	jal	strlen
	move	$16,$4

	addiu	$3,$2,-1
	bltz	$3,$L78
	addu	$4,$16,$3

	lb	$5,0($4)
	bne	$5,$17,$L78
	addu	$16,$16,$2

	j	$L79
	move	$4,$0

$L81:
	lb	$2,-2($16)
	bne	$2,$5,$L84
	addiu	$16,$16,-1

$L79:
	addiu	$3,$3,-1
	bgez	$3,$L81
	addiu	$4,$4,1

$L84:
	srl	$3,$4,31
	addu	$2,$4,$3
	andi	$2,$2,0x1
	subu	$2,$2,$3
$L80:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

$L78:
	j	$L80
	move	$2,$0

	.set	macro
	.set	reorder
	.end	check_esc_end
	.size	check_esc_end, .-check_esc_end
	.align	2
	.globl	unregister_customRspStr
	.set	nomips16
	.ent	unregister_customRspStr
	.type	unregister_customRspStr, @function
unregister_customRspStr:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(block_message)
	lw	$4,%lo(block_message)($16)
	beq	$4,$0,$L88
	lw	$31,20($sp)

	jal	kfree
	nop

	sw	$0,%lo(block_message)($16)
	lw	$31,20($sp)
$L88:
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	unregister_customRspStr
	.size	unregister_customRspStr, .-unregister_customRspStr
	.align	2
	.globl	filter_table_regist
	.set	nomips16
	.ent	filter_table_regist
	.type	filter_table_regist, @function
filter_table_regist:
	.frame	$sp,40,$31		# vars= 0, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$19,32($sp)
	sw	$18,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$17,%hi(table_list_head)
	addiu	$16,$17,%lo(table_list_head)
	lw	$2,24($16)
	move	$19,$4
	bne	$2,$0,$L90
	move	$18,$5

	jal	filter_table_head_init
	nop

$L90:
	lui	$2,%hi(kmalloc_caches+28)
	lw	$4,%lo(kmalloc_caches+28)($2)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	move	$3,$2
	li	$4,1			# 0x1
	addiu	$2,$2,8
	sw	$2,12($3)
	sw	$3,0($3)
	sw	$3,4($3)
	sw	$2,8($3)
	sw	$19,16($3)
	sw	$4,24($3)
	sw	$18,28($3)
	sw	$0,20($3)
	lw	$4,%lo(table_list_head)($17)
	move	$2,$0
	sw	$3,4($4)
	sw	$4,0($3)
	sw	$16,4($3)
	lw	$4,24($16)
	sw	$3,%lo(table_list_head)($17)
	lw	$31,36($sp)
	addiu	$3,$4,1
	sw	$3,24($16)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	filter_table_regist
	.size	filter_table_regist, .-filter_table_regist
	.align	2
	.set	nomips16
	.ent	rtl_schedule_filter_fastpath
	.type	rtl_schedule_filter_fastpath, @function
rtl_schedule_filter_fastpath:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	addiu	$4,$sp,16
	jal	do_gettimeofday
	sw	$5,24($sp)

	lw	$2,16($sp)
	li	$3,-1037172736			# 0xffffffffc22e0000
	ori	$3,$3,0x4507
	mult	$2,$3
	sra	$4,$2,31
	li	$7,-1840709632			# 0xffffffff92490000
	ori	$7,$7,0x2493
	lw	$5,24($sp)
	lw	$6,8($5)
	mfhi	$3
	addu	$3,$3,$2
	sra	$3,$3,16
	subu	$3,$3,$4
	addiu	$3,$3,4
	mult	$3,$7
	sra	$8,$3,31
	mfhi	$7
	addu	$7,$7,$3
	sra	$7,$7,2
	subu	$7,$7,$8
	sll	$8,$7,3
	subu	$7,$8,$7
	subu	$3,$3,$7
	li	$7,1			# 0x1
	sll	$3,$7,$3
	ori	$3,$3,0x80
	and	$6,$3,$6
	bne	$6,$0,$L98
	li	$3,-1851654144			# 0xffffffff91a20000

$L93:
	move	$2,$0
$L96:
	lw	$31,36($sp)
	j	$31
	addiu	$sp,$sp,40

$L98:
	ori	$3,$3,0xb3c5
	mult	$2,$3
	li	$6,-2004353024			# 0xffffffff88880000
	ori	$6,$6,0x8889
	lw	$7,0($5)
	mfhi	$3
	addu	$3,$3,$2
	sra	$3,$3,11
	mult	$2,$6
	subu	$3,$3,$4
	sra	$8,$3,31
	mfhi	$9
	addu	$2,$9,$2
	li	$9,715784192			# 0x2aaa0000
	ori	$9,$9,0xaaab
	mult	$3,$9
	sra	$2,$2,5
	subu	$2,$2,$4
	sra	$4,$2,31
	mfhi	$9
	sra	$9,$9,2
	subu	$8,$9,$8
	mult	$2,$6
	sll	$9,$8,5
	sll	$8,$8,3
	subu	$8,$9,$8
	subu	$3,$3,$8
	sll	$8,$3,6
	sll	$3,$3,2
	subu	$3,$8,$3
	mfhi	$6
	addu	$6,$6,$2
	sra	$6,$6,5
	subu	$4,$6,$4
	sll	$6,$4,6
	sll	$4,$4,2
	subu	$4,$6,$4
	subu	$2,$2,$4
	bne	$7,$0,$L94
	addu	$3,$3,$2

	lw	$4,4($5)
	beq	$4,$0,$L96
	li	$2,1			# 0x1

	sltu	$2,$4,$3
	xori	$2,$2,0x1
$L99:
	lw	$31,36($sp)
	j	$31
	addiu	$sp,$sp,40

$L94:
	sltu	$7,$3,$7
	bne	$7,$0,$L93
	nop

	lw	$4,4($5)
	sltu	$2,$4,$3
	j	$L99
	xori	$2,$2,0x1

	.set	macro
	.set	reorder
	.end	rtl_schedule_filter_fastpath
	.size	rtl_schedule_filter_fastpath, .-rtl_schedule_filter_fastpath
	.align	2
	.set	nomips16
	.ent	rtl_ip_range_filter_fastpath
	.type	rtl_ip_range_filter_fastpath, @function
rtl_ip_range_filter_fastpath:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	jal	rtl_ip_hdr
	sw	$5,16($sp)

	lw	$5,16($sp)
	lw	$3,12($2)
	lw	$4,0($5)
	sltu	$4,$3,$4
	bne	$4,$0,$L102
	move	$2,$0

	lw	$2,4($5)
	sltu	$2,$2,$3
	xori	$2,$2,0x1
$L102:
	lw	$31,28($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	rtl_ip_range_filter_fastpath
	.size	rtl_ip_range_filter_fastpath, .-rtl_ip_range_filter_fastpath
	.align	2
	.globl	filter_out
	.set	nomips16
	.ent	filter_out
	.type	filter_out, @function
filter_out:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	jal	rtl_ip_hdr
	nop

	lw	$4,0($2)
	lbu	$3,9($2)
	andi	$5,$4,0xf
	li	$4,1			# 0x1
	beq	$3,$4,$L107
	lw	$31,20($sp)

$L108:
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

$L107:
	sll	$5,$5,2
	addu	$2,$2,$5
	lbu	$4,0($2)
	li	$2,8			# 0x8
	bne	$4,$2,$L108
	nop

	lui	$2,%hi(jiffies)
	lw	$4,%lo(jiffies)($2)
	lw	$31,20($sp)
	lui	$2,%hi(icmp_echo_request_time)
	sw	$4,%lo(icmp_echo_request_time)($2)
	lui	$2,%hi(icmp_echo_request)
	sw	$3,%lo(icmp_echo_request)($2)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_out
	.size	filter_out, .-filter_out
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"filter_table\000"
	.text
	.align	2
	.globl	filter_exit_fastpath
	.set	nomips16
	.ent	filter_exit_fastpath
	.type	filter_exit_fastpath, @function
filter_exit_fastpath:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(res12)
	lw	$2,%lo(res12)($16)
	beq	$2,$0,$L110
	lui	$4,%hi($LC0)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC0)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(res12)($16)
$L110:
	lw	$31,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_exit_fastpath
	.size	filter_exit_fastpath, .-filter_exit_fastpath
	.section	.rodata.str1.4
	.align	2
$LC1:
	.ascii	"enable_dos\000"
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	filter_exit
	.set	nomips16
	.ent	filter_exit
	.type	filter_exit, @function
filter_exit:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(res3)
	lw	$5,%lo(res3)($16)
	beq	$5,$0,$L113
	nop

	lui	$4,%hi($LC1)
	jal	remove_proc_entry
	addiu	$4,$4,%lo($LC1)

	sw	$0,%lo(res3)($16)
$L113:
	lui	$4,%hi(dos_timer)
	jal	del_timer
	addiu	$4,$4,%lo(dos_timer)

	lw	$31,20($sp)
	lw	$16,16($sp)
	j	filter_exit_fastpath
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_exit
	.size	filter_exit, .-filter_exit
	.text
	.align	2
	.globl	filter_table_init
	.set	nomips16
	.ent	filter_table_init
	.type	filter_table_init, @function
filter_table_init:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	jal	filter_table_head_init
	nop

	lui	$4,%hi($LC0)
	lui	$6,%hi(proc_root)
	lui	$7,%hi(filter_table_proc_fops)
	addiu	$4,$4,%lo($LC0)
	addiu	$6,$6,%lo(proc_root)
	addiu	$7,$7,%lo(filter_table_proc_fops)
	move	$5,$0
	jal	proc_create_data
	sw	$0,16($sp)

	lw	$31,28($sp)
	lui	$3,%hi(res12)
	sw	$2,%lo(res12)($3)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	filter_table_init
	.size	filter_table_init, .-filter_table_init
	.align	2
	.globl	filter_init_fastpath
	.set	nomips16
	.ent	filter_init_fastpath
	.type	filter_init_fastpath, @function
filter_init_fastpath:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	li	$3,1			# 0x1
	lui	$2,%hi(filter_rule_num)
	jal	filter_table_init
	sw	$3,%lo(filter_rule_num)($2)

	lw	$31,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_init_fastpath
	.size	filter_init_fastpath, .-filter_init_fastpath
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	filter_init
	.set	nomips16
	.ent	filter_init
	.type	filter_init, @function
filter_init:
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 24, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	lui	$2,%hi(item)
	sw	$0,%lo(item)($2)
	lui	$4,%hi($LC1)
	lui	$2,%hi(none_dos_drop_pkt_cnt)
	lui	$6,%hi(proc_root)
	lui	$7,%hi(dos_proc_fops)
	addiu	$4,$4,%lo($LC1)
	addiu	$6,$6,%lo(proc_root)
	addiu	$7,$7,%lo(dos_proc_fops)
	move	$5,$0
	lui	$16,%hi(dos_timer)
	sw	$0,%lo(none_dos_drop_pkt_cnt)($2)
	lui	$2,%hi(url_count)
	sw	$0,16($sp)
	jal	proc_create_data
	sb	$0,%lo(url_count)($2)

	move	$7,$0
	move	$6,$0
	addiu	$4,$16,%lo(dos_timer)
	lui	$3,%hi(res3)
	move	$5,$0
	jal	init_timer_key
	sw	$2,%lo(res3)($3)

	lui	$3,%hi(jiffies)
	lw	$2,%lo(jiffies)($3)
	lw	$5,%lo(jiffies)($3)
	addiu	$16,$16,%lo(dos_timer)
	addiu	$2,$2,100
	sw	$2,8($16)
	lui	$2,%hi(dos_timer_fn)
	lui	$17,%hi(_tcpDosScanBitmap)
	move	$4,$16
	addiu	$5,$5,100
	addiu	$2,$2,%lo(dos_timer_fn)
	sw	$2,16($16)
	jal	mod_timer
	sw	$0,20($16)

	addiu	$4,$17,%lo(_tcpDosScanBitmap)
	move	$5,$0
	jal	memset
	li	$6,64			# 0x40

	addiu	$3,$17,%lo(_tcpDosScanBitmap)
	li	$2,1			# 0x1
	sb	$2,3($3)
	sb	$2,63($3)
	sb	$2,58($3)
	sb	$2,41($3)
	sb	$2,40($3)
	sb	$2,33($3)
	sb	$2,32($3)
	sb	$2,9($3)
	sb	$2,8($3)
	jal	filter_init_fastpath
	sb	$2,%lo(_tcpDosScanBitmap)($17)

	jal	igmp_delete_init_netlink
	nop

	lw	$31,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	filter_init
	.size	filter_init, .-filter_init
	.section	.rodata.str1.4
	.align	2
$LC2:
	.ascii	"DoS: Whole System SYN Flood Attack\012\000"
	.align	2
$LC3:
	.ascii	"DoS: Whole System FIN Flood Attack\012\000"
	.align	2
$LC4:
	.ascii	"DoS: Whole System UDP Flood Attack\012\000"
	.align	2
$LC5:
	.ascii	"DoS: Whole System ICMP Flood Attack\012\000"
	.align	2
$LC6:
	.ascii	"DoS: Per-source SYN Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\012\000"
	.align	2
$LC7:
	.ascii	"DoS: Per-source FIN Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\012\000"
	.align	2
$LC8:
	.ascii	"DoS: Per-source UDP Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\012\000"
	.align	2
$LC9:
	.ascii	"DoS: Per-source ICMP Flood Attack source=%u.%u.%u.%u des"
	.ascii	"tination=%u.%u.%u.%u\012\000"
	.align	2
$LC10:
	.ascii	"DoS: Port Scan Attack source=%u.%u.%u.%u destination=%u."
	.ascii	"%u.%u.%u\012\000"
	.align	2
$LC11:
	.ascii	"DoS: Tcp Scan Attack source=%u.%u.%u.%u destination=%u.%"
	.ascii	"u.%u.%u\012\000"
	.align	2
$LC12:
	.ascii	"DoS: Tcp SYN With Data Attack source=%u.%u.%u.%u destina"
	.ascii	"tion=%u.%u.%u.%u\012\000"
	.align	2
$LC13:
	.ascii	"DoS: IP Land Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\012\000"
	.align	2
$LC14:
	.ascii	"DoS: UdpEchoChargen Attack source=%u.%u.%u.%u destinatio"
	.ascii	"n=%u.%u.%u.%u\012\000"
	.align	2
$LC15:
	.ascii	"DoS: UdpBomb Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\012\000"
	.align	2
$LC16:
	.ascii	"DoS: PingOfDeath Attack source=%u.%u.%u.%u destination=%"
	.ascii	"u.%u.%u.%u\012\000"
	.align	2
$LC17:
	.ascii	"DoS: IcmpSmurf Attack source=%u.%u.%u.%u destination=%u."
	.ascii	"%u.%u.%u\012\000"
	.align	2
$LC18:
	.ascii	"DoS: IpSpoof Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\012\000"
	.align	2
$LC19:
	.ascii	"DoS: TearDrop Attack source=%u.%u.%u.%u destination=%u.%"
	.ascii	"u.%u.%u\012\000"
	.text
	.align	2
	.set	nomips16
	.ent	dos_timer_fn
	.type	dos_timer_fn, @function
dos_timer_fn:
	.frame	$sp,72,$31		# vars= 0, regs= 7/0, args= 40, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$21,64($sp)
	sw	$20,60($sp)
	sw	$19,56($sp)
	sw	$18,52($sp)
	sw	$17,48($sp)
	sw	$16,44($sp)
	lui	$17,%hi(item)
	lw	$15,%lo(item)($17)
	bne	$15,$0,$L180
	lui	$19,%hi(block)

	lui	$3,%hi(jiffies)
$L122:
	lui	$2,%hi(none_dos_drop_pkt_cnt)
$L231:
	lw	$4,%lo(none_dos_drop_pkt_cnt)($2)
	sltu	$5,$4,3001
	bne	$5,$0,$L168
	lui	$5,%hi(item_backup)

	lw	$4,%lo(item)($17)
	bne	$4,$0,$L191
	nop

$L169:
	lw	$5,%lo(jiffies)($3)
	lw	$31,68($sp)
	lw	$21,64($sp)
	lw	$20,60($sp)
	lw	$19,56($sp)
	lw	$18,52($sp)
	lw	$17,48($sp)
	lw	$16,44($sp)
	lui	$4,%hi(dos_timer)
	addiu	$4,$4,%lo(dos_timer)
	addiu	$5,$5,100
	sw	$0,%lo(none_dos_drop_pkt_cnt)($2)
	j	mod_timer
	addiu	$sp,$sp,72

$L168:
	lw	$6,%lo(item_backup)($5)
	beq	$6,$0,$L170
	nop

	sw	$6,%lo(item)($17)
	sw	$0,%lo(item_backup)($5)
$L170:
	bne	$4,$0,$L169
	nop

	lw	$5,%lo(jiffies)($3)
	lw	$31,68($sp)
	lw	$21,64($sp)
	lw	$20,60($sp)
	lw	$19,56($sp)
	lw	$18,52($sp)
	lw	$17,48($sp)
	lw	$16,44($sp)
	lui	$4,%hi(dos_timer)
	addiu	$4,$4,%lo(dos_timer)
	addiu	$5,$5,100
	j	mod_timer
	addiu	$sp,$sp,72

$L180:
	lw	$3,%lo(block)($19)
	li	$2,1			# 0x1
	beq	$3,$2,$L181
	lui	$21,%hi(block_count)

$L123:
	andi	$2,$15,0x1e
	bne	$2,$0,$L124
	lui	$20,%hi(dos_pkt)

	lui	$18,%hi(LogFlag)
	lw	$16,%lo(LogFlag)($18)
	addiu	$20,$20,%lo(dos_pkt)
$L125:
	andi	$2,$16,0x2
	bne	$2,$0,$L212
	lui	$4,%hi($LC2)

	andi	$2,$16,0x4
$L233:
	bne	$2,$0,$L213
	lui	$4,%hi($LC3)

	andi	$2,$16,0x8
$L234:
	bne	$2,$0,$L214
	lui	$4,%hi($LC4)

	andi	$2,$16,0x10
$L235:
	bne	$2,$0,$L215
	lui	$4,%hi($LC5)

	andi	$2,$16,0x20
$L236:
	bne	$2,$0,$L216
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x40
$L237:
	bne	$2,$0,$L217
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x80
$L238:
	bne	$2,$0,$L218
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x100
$L239:
	bne	$2,$0,$L219
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x200
$L240:
	bne	$2,$0,$L220
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x8000
$L241:
	bne	$2,$0,$L221
	lui	$2,%hi(attack_saddr)

	ext	$2,$16,16,1
$L242:
	bne	$2,$0,$L222
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x800
$L243:
	bne	$2,$0,$L223
	lui	$2,%hi(attack_saddr)

	ext	$2,$16,18,1
$L244:
	bne	$2,$0,$L224
	lui	$2,%hi(attack_saddr)

	ext	$2,$16,17,1
$L245:
	bne	$2,$0,$L225
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x4000
$L246:
	bne	$2,$0,$L226
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x400
$L247:
	bne	$2,$0,$L227
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x1000
$L248:
	bne	$2,$0,$L228
	lui	$2,%hi(attack_saddr)

$L163:
	andi	$16,$16,0x2000
$L211:
	beq	$16,$0,$L229
	lui	$2,%hi(dos_pkt+40960)

	lui	$2,%hi(attack_saddr)
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,41($2)
	lbu	$7,42($2)
	lbu	$10,43($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+40)
	lui	$2,%hi(attack_daddr+40)
	lbu	$8,41($3)
	lbu	$9,%lo(attack_daddr+40)($2)
	lbu	$5,%lo(attack_saddr+40)($4)
	lbu	$2,43($3)
	lbu	$3,42($3)
	lui	$4,%hi($LC19)
	addiu	$4,$4,%lo($LC19)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	lui	$2,%hi(dos_pkt+40960)
$L229:
	sw	$0,%lo(LogFlag)($18)
	addiu	$2,$2,%lo(dos_pkt+40960)
$L165:
	sb	$0,0($20)
	sw	$0,28($20)
	sw	$0,4($20)
	sw	$0,8($20)
	sw	$0,12($20)
	sw	$0,16($20)
	sw	$0,24($20)
	addiu	$20,$20,40
	bne	$20,$2,$L165
	lw	$3,%lo(block_count)($21)

	lui	$2,%hi(block_time)
	lw	$2,%lo(block_time)($2)
	slt	$2,$3,$2
	bne	$2,$0,$L230
	lui	$2,%hi(icmp_echo_request_time)

	lui	$4,%hi(block_count)
	lui	$2,%hi(attack_saddr)
	lui	$3,%hi(attack_daddr)
	sw	$0,%lo(block_count)($4)
	sw	$0,%lo(block)($19)
	addiu	$2,$2,%lo(attack_saddr)
	addiu	$3,$3,%lo(attack_daddr)
	move	$4,$0
	li	$7,108			# 0x6c
$L167:
	addu	$6,$2,$4
	addu	$5,$3,$4
	addiu	$4,$4,4
	sw	$0,0($6)
	bne	$4,$7,$L167
	sw	$0,0($5)

	lui	$2,%hi(icmp_echo_request_time)
$L230:
	lw	$2,%lo(icmp_echo_request_time)($2)
	lui	$3,%hi(jiffies)
	lw	$4,%lo(jiffies)($3)
	addiu	$2,$2,100
	subu	$2,$2,$4
	bgez	$2,$L231
	lui	$2,%hi(none_dos_drop_pkt_cnt)

	lui	$2,%hi(icmp_echo_request)
	j	$L122
	sw	$0,%lo(icmp_echo_request)($2)

$L191:
	sw	$4,%lo(item_backup)($5)
	lui	$4,%hi(item)
	j	$L169
	sw	$0,%lo(item)($4)

$L124:
	addiu	$20,$20,%lo(dos_pkt)
	move	$2,$20
	lb	$3,0($2)
	lui	$5,%hi(dos_pkt+40920)
	li	$4,1			# 0x1
	addiu	$5,$5,%lo(dos_pkt+40920)
	move	$6,$0
	move	$7,$0
	move	$8,$0
	move	$9,$0
	andi	$13,$15,0x2
	andi	$12,$15,0x4
	andi	$11,$15,0x8
	beq	$3,$4,$L209
	andi	$10,$15,0x10

$L126:
	beq	$2,$5,$L130
	nop

$L210:
	addiu	$2,$2,40
	lb	$3,0($2)
	bne	$3,$4,$L126
	nop

$L209:
	beq	$13,$0,$L127
	nop

	lw	$3,4($2)
	slt	$14,$0,$3
	addu	$3,$9,$3
	movn	$9,$3,$14
$L127:
	beq	$12,$0,$L128
	nop

	lw	$3,8($2)
	slt	$14,$0,$3
	addu	$3,$8,$3
	movn	$8,$3,$14
$L128:
	beq	$11,$0,$L129
	nop

	lw	$3,12($2)
	slt	$14,$0,$3
	addu	$3,$7,$3
	movn	$7,$3,$14
$L129:
	beq	$10,$0,$L126
	nop

	lw	$3,16($2)
	slt	$14,$0,$3
	addu	$3,$6,$3
	bne	$2,$5,$L210
	movn	$6,$3,$14

$L130:
	lui	$2,%hi(whole_syn_threshold)
	lw	$2,%lo(whole_syn_threshold)($2)
	slt	$9,$2,$9
	beq	$9,$0,$L232
	lui	$18,%hi(LogFlag)

	andi	$3,$15,0x2
	beq	$3,$0,$L133
	lw	$16,%lo(LogFlag)($18)

	blez	$2,$L232
	ori	$16,$16,0x2

	j	$L133
	sw	$16,%lo(LogFlag)($18)

$L232:
	lw	$16,%lo(LogFlag)($18)
$L133:
	lui	$2,%hi(whole_fin_threshold)
	lw	$2,%lo(whole_fin_threshold)($2)
	slt	$8,$2,$8
	beq	$8,$0,$L185
	andi	$3,$15,0x4

	beq	$3,$0,$L185
	nop

	blez	$2,$L185
	ori	$16,$16,0x4

	j	$L137
	sw	$16,%lo(LogFlag)($18)

$L185:
	lw	$16,%lo(LogFlag)($18)
$L137:
	lui	$2,%hi(whole_udp_threshold)
	lw	$2,%lo(whole_udp_threshold)($2)
	slt	$7,$2,$7
	beq	$7,$0,$L187
	andi	$3,$15,0x8

	beq	$3,$0,$L187
	nop

	blez	$2,$L187
	ori	$16,$16,0x8

	j	$L141
	sw	$16,%lo(LogFlag)($18)

$L187:
	lw	$16,%lo(LogFlag)($18)
$L141:
	lui	$2,%hi(whole_icmp_threshold)
	lw	$2,%lo(whole_icmp_threshold)($2)
	slt	$6,$2,$6
	beq	$6,$0,$L189
	andi	$15,$15,0x10

	beq	$15,$0,$L189
	nop

	blez	$2,$L189
	ori	$16,$16,0x10

	j	$L125
	sw	$16,%lo(LogFlag)($18)

$L189:
	lw	$16,%lo(LogFlag)($18)
	andi	$2,$16,0x2
	beq	$2,$0,$L233
	andi	$2,$16,0x4

	lui	$4,%hi($LC2)
$L212:
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC2)

	andi	$2,$16,0x4
	beq	$2,$0,$L234
	andi	$2,$16,0x8

	lui	$4,%hi($LC3)
$L213:
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC3)

	andi	$2,$16,0x8
	beq	$2,$0,$L235
	andi	$2,$16,0x10

	lui	$4,%hi($LC4)
$L214:
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC4)

	andi	$2,$16,0x10
	beq	$2,$0,$L236
	andi	$2,$16,0x20

	lui	$4,%hi($LC5)
$L215:
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC5)

	andi	$2,$16,0x20
	beq	$2,$0,$L237
	andi	$2,$16,0x40

	lui	$2,%hi(attack_saddr)
$L216:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,5($2)
	lbu	$7,6($2)
	lbu	$10,7($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+4)
	lui	$2,%hi(attack_daddr+4)
	lbu	$8,5($3)
	lbu	$9,%lo(attack_daddr+4)($2)
	lbu	$5,%lo(attack_saddr+4)($4)
	lbu	$2,7($3)
	lbu	$3,6($3)
	lui	$4,%hi($LC6)
	addiu	$4,$4,%lo($LC6)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x40
	beq	$2,$0,$L238
	andi	$2,$16,0x80

	lui	$2,%hi(attack_saddr)
$L217:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,9($2)
	lbu	$7,10($2)
	lbu	$10,11($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+8)
	lui	$2,%hi(attack_daddr+8)
	lbu	$8,9($3)
	lbu	$9,%lo(attack_daddr+8)($2)
	lbu	$5,%lo(attack_saddr+8)($4)
	lbu	$2,11($3)
	lbu	$3,10($3)
	lui	$4,%hi($LC7)
	addiu	$4,$4,%lo($LC7)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x80
	beq	$2,$0,$L239
	andi	$2,$16,0x100

	lui	$2,%hi(attack_saddr)
$L218:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,13($2)
	lbu	$7,14($2)
	lbu	$10,15($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+12)
	lui	$2,%hi(attack_daddr+12)
	lbu	$8,13($3)
	lbu	$9,%lo(attack_daddr+12)($2)
	lbu	$5,%lo(attack_saddr+12)($4)
	lbu	$2,15($3)
	lbu	$3,14($3)
	lui	$4,%hi($LC8)
	addiu	$4,$4,%lo($LC8)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x100
	beq	$2,$0,$L240
	andi	$2,$16,0x200

	lui	$2,%hi(attack_saddr)
$L219:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,17($2)
	lbu	$7,18($2)
	lbu	$10,19($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+16)
	lui	$2,%hi(attack_daddr+16)
	lbu	$8,17($3)
	lbu	$9,%lo(attack_daddr+16)($2)
	lbu	$5,%lo(attack_saddr+16)($4)
	lbu	$2,19($3)
	lbu	$3,18($3)
	lui	$4,%hi($LC9)
	addiu	$4,$4,%lo($LC9)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x200
	beq	$2,$0,$L241
	andi	$2,$16,0x8000

	lui	$2,%hi(attack_saddr)
$L220:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,25($2)
	lbu	$7,26($2)
	lbu	$10,27($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+24)
	lui	$2,%hi(attack_daddr+24)
	lbu	$8,25($3)
	lbu	$9,%lo(attack_daddr+24)($2)
	lbu	$5,%lo(attack_saddr+24)($4)
	lbu	$2,27($3)
	lbu	$3,26($3)
	lui	$4,%hi($LC10)
	addiu	$4,$4,%lo($LC10)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x8000
	beq	$2,$0,$L242
	ext	$2,$16,16,1

	lui	$2,%hi(attack_saddr)
$L221:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,49($2)
	lbu	$7,50($2)
	lbu	$10,51($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+48)
	lui	$2,%hi(attack_daddr+48)
	lbu	$8,49($3)
	lbu	$9,%lo(attack_daddr+48)($2)
	lbu	$5,%lo(attack_saddr+48)($4)
	lbu	$2,51($3)
	lbu	$3,50($3)
	lui	$4,%hi($LC11)
	addiu	$4,$4,%lo($LC11)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	ext	$2,$16,16,1
	beq	$2,$0,$L243
	andi	$2,$16,0x800

	lui	$2,%hi(attack_saddr)
$L222:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,53($2)
	lbu	$7,54($2)
	lbu	$10,55($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+52)
	lui	$2,%hi(attack_daddr+52)
	lbu	$8,53($3)
	lbu	$9,%lo(attack_daddr+52)($2)
	lbu	$5,%lo(attack_saddr+52)($4)
	lbu	$2,55($3)
	lbu	$3,54($3)
	lui	$4,%hi($LC12)
	addiu	$4,$4,%lo($LC12)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x800
	beq	$2,$0,$L244
	ext	$2,$16,18,1

	lui	$2,%hi(attack_saddr)
$L223:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,33($2)
	lbu	$7,34($2)
	lbu	$10,35($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+32)
	lui	$2,%hi(attack_daddr+32)
	lbu	$8,33($3)
	lbu	$9,%lo(attack_daddr+32)($2)
	lbu	$5,%lo(attack_saddr+32)($4)
	lbu	$2,35($3)
	lbu	$3,34($3)
	lui	$4,%hi($LC13)
	addiu	$4,$4,%lo($LC13)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	ext	$2,$16,18,1
	beq	$2,$0,$L245
	ext	$2,$16,17,1

	lui	$2,%hi(attack_saddr)
$L224:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,61($2)
	lbu	$7,62($2)
	lbu	$10,63($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+60)
	lui	$2,%hi(attack_daddr+60)
	lbu	$8,61($3)
	lbu	$9,%lo(attack_daddr+60)($2)
	lbu	$5,%lo(attack_saddr+60)($4)
	lbu	$2,63($3)
	lbu	$3,62($3)
	lui	$4,%hi($LC14)
	addiu	$4,$4,%lo($LC14)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	ext	$2,$16,17,1
	beq	$2,$0,$L246
	andi	$2,$16,0x4000

	lui	$2,%hi(attack_saddr)
$L225:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,57($2)
	lbu	$7,58($2)
	lbu	$10,59($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+56)
	lui	$2,%hi(attack_daddr+56)
	lbu	$8,57($3)
	lbu	$9,%lo(attack_daddr+56)($2)
	lbu	$5,%lo(attack_saddr+56)($4)
	lbu	$2,59($3)
	lbu	$3,58($3)
	lui	$4,%hi($LC15)
	addiu	$4,$4,%lo($LC15)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x4000
	beq	$2,$0,$L247
	andi	$2,$16,0x400

	lui	$2,%hi(attack_saddr)
$L226:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,45($2)
	lbu	$7,46($2)
	lbu	$10,47($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+44)
	lui	$2,%hi(attack_daddr+44)
	lbu	$8,45($3)
	lbu	$9,%lo(attack_daddr+44)($2)
	lbu	$5,%lo(attack_saddr+44)($4)
	lbu	$2,47($3)
	lbu	$3,46($3)
	lui	$4,%hi($LC16)
	addiu	$4,$4,%lo($LC16)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x400
	beq	$2,$0,$L248
	andi	$2,$16,0x1000

	lui	$2,%hi(attack_saddr)
$L227:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,29($2)
	lbu	$7,30($2)
	lbu	$10,31($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+28)
	lui	$2,%hi(attack_daddr+28)
	lbu	$8,29($3)
	lbu	$9,%lo(attack_daddr+28)($2)
	lbu	$5,%lo(attack_saddr+28)($4)
	lbu	$2,31($3)
	lbu	$3,30($3)
	lui	$4,%hi($LC17)
	addiu	$4,$4,%lo($LC17)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x1000
	beq	$2,$0,$L163
	lui	$2,%hi(attack_saddr)

$L228:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,37($2)
	lbu	$7,38($2)
	lbu	$10,39($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+36)
	lui	$2,%hi(attack_daddr+36)
	lbu	$8,37($3)
	lbu	$9,%lo(attack_daddr+36)($2)
	lbu	$5,%lo(attack_saddr+36)($4)
	lbu	$2,39($3)
	lbu	$3,38($3)
	lui	$4,%hi($LC18)
	addiu	$4,$4,%lo($LC18)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	j	$L211
	andi	$16,$16,0x2000

$L181:
	lw	$2,%lo(block_count)($21)
	addiu	$2,$2,1
	j	$L123
	sw	$2,%lo(block_count)($21)

	.set	macro
	.set	reorder
	.end	dos_timer_fn
	.size	dos_timer_fn, .-dos_timer_fn
	.section	.rodata.str1.4
	.align	2
$LC20:
	.ascii	"br0\000"
	.align	2
$LC21:
	.ascii	"ppp0\000"
	.align	2
$LC22:
	.ascii	"eth1\000"
	.align	2
$LC23:
	.ascii	"wlan0\000"
	.text
	.align	2
	.globl	need_filter
	.set	nomips16
	.ent	need_filter
	.type	need_filter, @function
need_filter:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	lui	$16,%hi($LC20)
	.set	macro
	.set	reorder

	addiu	$4,$16,%lo($LC20)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	move	$17,$2
	.set	macro
	.set	reorder

	beq	$2,$0,$L250
	lw	$2,372($2)
	beq	$2,$0,$L250
	lw	$4,12($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L250
	addiu	$16,$16,%lo($LC20)
	.set	macro
	.set	reorder

	addiu	$3,$4,43
	move	$2,$16
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$3,1
	bnez	$5,1b
	lbu	$5,($2)
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	bne	$5,$0,$L268
	.set	noreorder
	.set	nomacro
	j	$L272
	lw	$3,28($4)
	.set	macro
	.set	reorder

$L253:
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$3,1
	bnez	$5,1b
	lbu	$5,($2)
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$5,$0,$L251
$L268:
	lw	$4,8($4)
	move	$2,$16
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L253
	addiu	$3,$4,43
	.set	macro
	.set	reorder

$L250:
	lui	$16,%hi($LC21)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$16,%lo($LC21)
	.set	macro
	.set	reorder

	beq	$2,$0,$L255
	lw	$2,372($2)
	beq	$2,$0,$L256
	lw	$4,12($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L256
	addiu	$16,$16,%lo($LC21)
	.set	macro
	.set	reorder

	addiu	$3,$4,43
	move	$2,$16
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$3,1
	bnez	$5,1b
	lbu	$5,($2)
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	bne	$5,$0,$L269
	.set	noreorder
	.set	nomacro
	j	$L273
	lw	$3,16($17)
	.set	macro
	.set	reorder

$L259:
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$3,1
	bnez	$5,1b
	lbu	$5,($2)
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$5,$0,$L257
$L269:
	lw	$4,8($4)
	move	$2,$16
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L259
	addiu	$3,$4,43
	.set	macro
	.set	reorder

$L256:
	li	$2,1			# 0x1
$L254:
	lw	$31,28($sp)
$L274:
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L251:
	lw	$3,28($4)
$L272:
	lw	$4,16($17)
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L250
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L274
	lw	$31,28($sp)
	.set	macro
	.set	reorder

$L257:
	lw	$3,16($17)
$L273:
	lw	$2,24($4)
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	xor	$2,$3,$2
	sltu	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L255:
	lui	$16,%hi($LC22)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$16,%lo($LC22)
	.set	macro
	.set	reorder

	beq	$2,$0,$L260
	lw	$2,372($2)
	beq	$2,$0,$L256
	lw	$4,12($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L256
	addiu	$16,$16,%lo($LC22)
	.set	macro
	.set	reorder

	addiu	$3,$4,43
	move	$2,$16
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$3,1
	bnez	$5,1b
	lbu	$5,($2)
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	bne	$5,$0,$L270
	.set	noreorder
	.set	nomacro
	j	$L275
	lw	$3,16($17)
	.set	macro
	.set	reorder

$L263:
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$3,1
	bnez	$5,1b
	lbu	$5,($2)
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$5,$0,$L261
$L270:
	lw	$4,8($4)
	move	$2,$16
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L263
	addiu	$3,$4,43
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L254
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L261:
	lw	$3,16($17)
$L275:
	lw	$2,28($4)
	xor	$2,$3,$2
	.set	noreorder
	.set	nomacro
	j	$L254
	sltu	$2,$0,$2
	.set	macro
	.set	reorder

$L260:
	lui	$16,%hi($LC23)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$16,%lo($LC23)
	.set	macro
	.set	reorder

	beq	$2,$0,$L256
	lw	$2,372($2)
	beq	$2,$0,$L256
	lw	$3,12($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L256
	addiu	$16,$16,%lo($LC23)
	.set	macro
	.set	reorder

	addiu	$4,$3,43
	move	$2,$16
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($4)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$4,1
	bnez	$5,1b
	lbu	$5,($2)
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	bne	$5,$0,$L271
	.set	noreorder
	.set	nomacro
	j	$L276
	lw	$4,16($17)
	.set	macro
	.set	reorder

$L266:
 #APP
 # 77 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($4)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$4,1
	bnez	$5,1b
	lbu	$5,($2)
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$5,$0,$L264
$L271:
	lw	$3,8($3)
	move	$2,$16
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L266
	addiu	$4,$3,43
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L254
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L264:
	lw	$4,16($17)
$L276:
	lw	$2,28($3)
	xor	$2,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L254
	sltu	$2,$0,$2
	.set	macro
	.set	reorder

	.end	need_filter
	.size	need_filter, .-need_filter
	.section	.rodata.str1.4
	.align	2
$LC24:
	.ascii	"dos_filter: alloc skb fail!\012\000"
	.text
	.align	2
	.set	nomips16
	.ent	GenerateTCPENDACK
	.type	GenerateTCPENDACK, @function
GenerateTCPENDACK:
	.frame	$sp,112,$31		# vars= 56, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$fp,104($sp)
	sw	$23,100($sp)
	sw	$22,96($sp)
	sw	$21,92($sp)
	sw	$20,88($sp)
	sw	$19,84($sp)
	sw	$18,80($sp)
	sw	$17,76($sp)
	sw	$16,72($sp)
	move	$18,$4
	lui	$4,%hi($LC20)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC20)
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$19,$2
	.set	macro
	.set	reorder

	lw	$17,0($2)
	move	$16,$2
	lui	$2,%hi(endStateClientPort)
	andi	$17,$17,0xf
	sll	$17,$17,2
	addu	$17,$16,$17
	lhu	$2,%lo(endStateClientPort)($2)
	lhu	$3,0($17)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L305
	lui	$2,%hi(endStateClientSeq)
	.set	macro
	.set	reorder

$L278:
	lw	$31,108($sp)
$L312:
	lw	$fp,104($sp)
	lw	$23,100($sp)
	lw	$22,96($sp)
	lw	$21,92($sp)
	lw	$20,88($sp)
	lw	$19,84($sp)
	lw	$18,80($sp)
	lw	$17,76($sp)
	lw	$16,72($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,112
	.set	macro
	.set	reorder

$L305:
	lw	$2,%lo(endStateClientSeq)($2)
	lw	$3,4($17)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L312
	lw	$31,108($sp)
	.set	macro
	.set	reorder

	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	addiu	$4,$16,12
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L278
	move	$19,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L306
	move	$4,$18
	.set	macro
	.set	reorder

	li	$5,96			# 0x60
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L307
	move	$20,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	lw	$5,0($2)
	move	$4,$20
	andi	$5,$5,0xf
	addiu	$5,$5,5
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sll	$5,$5,2
	.set	macro
	.set	reorder

	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$19
	.set	macro
	.set	reorder

	move	$21,$2
	addiu	$23,$2,4
$L303:
	lw	$22,0($23)
	andi	$2,$22,0x1
	bne	$2,$0,$L308
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$20
	.set	macro
	.set	reorder

	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	move	$fp,$2
	.set	macro
	.set	reorder

	move	$5,$2
	addiu	$4,$fp,-16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lw	$2,4($21)
	bne	$22,$2,$L303
	.set	noreorder
	.set	nomacro
	jal	rtl_get_len_from_hh
	move	$4,$21
	.set	macro
	.set	reorder

	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$4,$20
	.set	macro
	.set	reorder

 #APP
 # 185 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	addiu	$2,$19,20
$L287:
 #APP
 # 207 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, 0($2)	# atomic_sub_return
	subu	$4, $3, 1
	sc	$4, 0($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L287
 #APP
 # 228 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L309
	move	$4,$20
$L313:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$5,$0
	.set	macro
	.set	reorder

	lhu	$3,2($17)
	lw	$2,56($sp)
	li	$4,5			# 0x5
	sh	$3,44($sp)
	lhu	$3,0($17)
	ins	$2,$4,4,4
	addiu	$19,$sp,44
	sh	$3,46($sp)
	lw	$5,4($17)
	lw	$6,8($17)
	sw	$2,56($sp)
	sb	$0,13($19)
	lw	$2,56($sp)
	lui	$22,%hi(url_cache)
	addiu	$21,$22,%lo(url_cache)
	lhu	$3,12($21)
	ins	$2,$0,10,1
	ori	$2,$2,0x1000
	addiu	$5,$5,1
	ins	$2,$0,8,1
	move	$4,$20
	sw	$2,56($sp)
	sw	$6,48($sp)
	sh	$3,58($sp)
	sw	$5,52($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$0,62($sp)
	.set	macro
	.set	reorder

	lw	$3,12($17)
	addiu	$23,$2,-34
	ext	$3,$3,4,4
	sll	$3,$3,2
	sltu	$2,$3,21
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L289
	li	$17,20			# 0x14
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L293
	li	$6,8			# 0x8
	.set	macro
	.set	reorder

$L290:
	.set	noreorder
	.set	nomacro
	beq	$2,$6,$L310
	lbu	$4,1($5)
	.set	macro
	.set	reorder

	addu	$17,$17,$4
$L291:
	sltu	$2,$17,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L315
	move	$5,$23
	.set	macro
	.set	reorder

$L293:
	addu	$2,$19,$17
	lbu	$2,0($2)
	addu	$5,$19,$17
	sltu	$4,$2,2
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L290
	addiu	$7,$17,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L291
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L306:
 #APP
 # 185 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	addiu	$2,$19,20
$L281:
 #APP
 # 207 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, 0($2)	# atomic_sub_return
	subu	$4, $3, 1
	sc	$4, 0($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L281
 #APP
 # 228 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	bne	$3,$2,$L278
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L312
	lw	$31,108($sp)
	.set	macro
	.set	reorder

$L310:
	addu	$7,$19,$7
	addiu	$fp,$sp,16
	move	$5,$7
	move	$4,$fp
	li	$6,4			# 0x4
	addiu	$17,$17,6
	sw	$7,68($sp)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$17,$19,$17
	.set	macro
	.set	reorder

	addiu	$3,$sp,20
	move	$4,$3
	move	$5,$17
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$3,64($sp)
	.set	macro
	.set	reorder

	lw	$7,68($sp)
	lw	$3,64($sp)
	li	$6,4			# 0x4
	move	$4,$7
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$5,$3
	.set	macro
	.set	reorder

	move	$4,$17
	move	$5,$fp
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

$L289:
	move	$5,$23
$L315:
	sh	$0,60($sp)
	move	$6,$0
	move	$4,$19
	andi	$23,$23,0xffff
	lw	$fp,16($16)
	lw	$17,12($16)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	addiu	$23,$23,6
	.set	macro
	.set	reorder

	move	$4,$20
	sll	$23,$23,8
 #APP
 # 136 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
		.set	push		# csum_tcpudp_nofold
	.set	noat
	addu	$fp, $17
	sltu	$1, $fp, $17
	addu	$fp, $1
	addu	$fp, $23
	sltu	$1, $fp, $23
	addu	$fp, $1
	addu	$fp, $2
	sltu	$1, $fp, $2
	addu	$fp, $1
	.set	pop
 # 0 "" 2
 # 78 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat
	sll	$1, $fp, 16
	addu	$fp, $1
	sltu	$1, $fp, $1
	srl	$fp, $fp, 16
	addu	$fp, $1
	xori	$fp, 0xffff
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$fp,60($sp)
	.set	macro
	.set	reorder

	addiu	$4,$2,34
	li	$6,20			# 0x14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$5,$19
	.set	macro
	.set	reorder

	lw	$2,24($sp)
	li	$3,4			# 0x4
	sh	$0,30($sp)
	ins	$2,$3,4,4
	li	$3,5			# 0x5
	ins	$2,$3,0,4
	sw	$2,24($sp)
	li	$2,6			# 0x6
	sb	$2,33($sp)
	sb	$0,25($sp)
	lw	$2,12($16)
	lbu	$3,14($21)
	move	$4,$18
	sw	$2,40($sp)
	lw	$2,16($16)
	move	$5,$0
	sb	$3,32($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ip_summed
	sw	$2,36($sp)
	.set	macro
	.set	reorder

	lhu	$2,%lo(url_cache)($22)
	li	$3,10240			# 0x2800
	sh	$3,26($sp)
	addiu	$2,$2,1
	sh	$2,28($sp)
	addiu	$5,$sp,24
	sh	$0,34($sp)
	lw	$6,24($sp)
	lw	$7,4($5)
	lw	$3,8($5)
	lw	$4,36($sp)
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
	addiu	$3,$sp,40
$L294:
	lw	$4,0($3)
	addiu	$3,$3,4
	addu	$2,$2,$4
	sltu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$6,$3,$L294
	addu	$2,$4,$2
	.set	macro
	.set	reorder

	move	$4,$20
	sw	$5,64($sp)
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
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$2,34($sp)
	.set	macro
	.set	reorder

	lw	$5,64($sp)
	addiu	$4,$2,14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,20			# 0x14
	.set	macro
	.set	reorder

	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sb	$0,16($21)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L312
	lw	$31,108($sp)
	.set	macro
	.set	reorder

$L307:
 #APP
 # 185 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	addiu	$2,$19,20
$L283:
 #APP
 # 207 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$4, 0($2)	# atomic_sub_return
	subu	$3, $4, 1
	sc	$3, 0($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	beq	$3,$0,$L283
 #APP
 # 228 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$4,$2,$L311
	lui	$4,%hi($LC24)
$L314:
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC24)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L312
	lw	$31,108($sp)
	.set	macro
	.set	reorder

$L309:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L313
	move	$4,$20
	.set	macro
	.set	reorder

$L311:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L314
	lui	$4,%hi($LC24)
	.set	macro
	.set	reorder

$L308:
	j	$L303
	.end	GenerateTCPENDACK
	.size	GenerateTCPENDACK, .-GenerateTCPENDACK
	.align	2
	.set	nomips16
	.ent	GenerateTCPFINACK
	.type	GenerateTCPFINACK, @function
GenerateTCPFINACK:
	.frame	$sp,112,$31		# vars= 56, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$fp,104($sp)
	sw	$23,100($sp)
	sw	$22,96($sp)
	sw	$21,92($sp)
	sw	$20,88($sp)
	sw	$19,84($sp)
	sw	$18,80($sp)
	sw	$17,76($sp)
	sw	$16,72($sp)
	move	$18,$4
	lui	$4,%hi($LC20)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC20)
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$19,$2
	.set	macro
	.set	reorder

	lw	$17,0($2)
	move	$16,$2
	lui	$2,%hi(finStateClientPort)
	andi	$17,$17,0xf
	sll	$17,$17,2
	addu	$17,$16,$17
	lhu	$2,%lo(finStateClientPort)($2)
	lhu	$3,0($17)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L344
	lui	$2,%hi(finStateClientSeq)
	.set	macro
	.set	reorder

$L317:
	lw	$31,108($sp)
$L351:
	lw	$fp,104($sp)
	lw	$23,100($sp)
	lw	$22,96($sp)
	lw	$21,92($sp)
	lw	$20,88($sp)
	lw	$19,84($sp)
	lw	$18,80($sp)
	lw	$17,76($sp)
	lw	$16,72($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,112
	.set	macro
	.set	reorder

$L344:
	lw	$2,%lo(finStateClientSeq)($2)
	lw	$3,4($17)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L351
	lw	$31,108($sp)
	.set	macro
	.set	reorder

	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	addiu	$4,$16,12
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L317
	move	$19,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L345
	move	$4,$18
	.set	macro
	.set	reorder

	li	$5,96			# 0x60
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L346
	move	$20,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	lw	$5,0($2)
	move	$4,$20
	andi	$5,$5,0xf
	addiu	$5,$5,5
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sll	$5,$5,2
	.set	macro
	.set	reorder

	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$19
	.set	macro
	.set	reorder

	move	$21,$2
	addiu	$23,$2,4
$L342:
	lw	$22,0($23)
	andi	$2,$22,0x1
	bne	$2,$0,$L347
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$20
	.set	macro
	.set	reorder

	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	move	$fp,$2
	.set	macro
	.set	reorder

	move	$5,$2
	addiu	$4,$fp,-16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lw	$2,4($21)
	bne	$22,$2,$L342
	.set	noreorder
	.set	nomacro
	jal	rtl_get_len_from_hh
	move	$4,$21
	.set	macro
	.set	reorder

	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$4,$20
	.set	macro
	.set	reorder

 #APP
 # 185 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	addiu	$2,$19,20
$L326:
 #APP
 # 207 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, 0($2)	# atomic_sub_return
	subu	$4, $3, 1
	sc	$4, 0($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L326
 #APP
 # 228 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L348
	move	$4,$20
$L352:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$5,$0
	.set	macro
	.set	reorder

	lhu	$3,2($17)
	lw	$2,56($sp)
	li	$4,5			# 0x5
	sh	$3,44($sp)
	lhu	$3,0($17)
	ins	$2,$4,4,4
	addiu	$19,$sp,44
	sh	$3,46($sp)
	lw	$5,4($17)
	lw	$6,8($17)
	sw	$2,56($sp)
	sb	$0,13($19)
	lw	$2,56($sp)
	lui	$22,%hi(url_cache)
	addiu	$21,$22,%lo(url_cache)
	lhu	$3,12($21)
	ins	$2,$0,10,1
	li	$4,1			# 0x1
	ori	$2,$2,0x1000
	ins	$2,$4,8,1
	move	$4,$20
	sw	$2,56($sp)
	sw	$6,48($sp)
	sh	$3,58($sp)
	sw	$5,52($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$0,62($sp)
	.set	macro
	.set	reorder

	lw	$3,12($17)
	addiu	$23,$2,-34
	ext	$3,$3,4,4
	sll	$3,$3,2
	sltu	$2,$3,21
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L328
	li	$17,20			# 0x14
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L332
	li	$6,8			# 0x8
	.set	macro
	.set	reorder

$L329:
	.set	noreorder
	.set	nomacro
	beq	$2,$6,$L349
	lbu	$4,1($5)
	.set	macro
	.set	reorder

	addu	$17,$17,$4
$L330:
	sltu	$2,$17,$3
	beq	$2,$0,$L328
$L332:
	addu	$2,$19,$17
	lbu	$2,0($2)
	addu	$5,$19,$17
	sltu	$4,$2,2
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L329
	addiu	$7,$17,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L330
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L345:
 #APP
 # 185 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	addiu	$2,$19,20
$L320:
 #APP
 # 207 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, 0($2)	# atomic_sub_return
	subu	$4, $3, 1
	sc	$4, 0($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L320
 #APP
 # 228 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	bne	$3,$2,$L317
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L351
	lw	$31,108($sp)
	.set	macro
	.set	reorder

$L349:
	addu	$7,$19,$7
	addiu	$fp,$sp,16
	move	$5,$7
	move	$4,$fp
	li	$6,4			# 0x4
	addiu	$17,$17,6
	sw	$7,68($sp)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$17,$19,$17
	.set	macro
	.set	reorder

	addiu	$3,$sp,20
	move	$4,$3
	move	$5,$17
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$3,64($sp)
	.set	macro
	.set	reorder

	lw	$7,68($sp)
	lw	$3,64($sp)
	li	$6,4			# 0x4
	move	$4,$7
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$5,$3
	.set	macro
	.set	reorder

	move	$4,$17
	move	$5,$fp
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

$L328:
	lhu	$3,46($sp)
	lui	$2,%hi(endStateClientPort)
	move	$5,$23
	sh	$3,%lo(endStateClientPort)($2)
	lw	$3,52($sp)
	lui	$2,%hi(endStateClientSeq)
	sh	$0,60($sp)
	sw	$3,%lo(endStateClientSeq)($2)
	move	$6,$0
	move	$4,$19
	andi	$23,$23,0xffff
	lw	$fp,16($16)
	lw	$17,12($16)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	addiu	$23,$23,6
	.set	macro
	.set	reorder

	move	$4,$20
	sll	$23,$23,8
 #APP
 # 136 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
		.set	push		# csum_tcpudp_nofold
	.set	noat
	addu	$fp, $17
	sltu	$1, $fp, $17
	addu	$fp, $1
	addu	$fp, $23
	sltu	$1, $fp, $23
	addu	$fp, $1
	addu	$fp, $2
	sltu	$1, $fp, $2
	addu	$fp, $1
	.set	pop
 # 0 "" 2
 # 78 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat
	sll	$1, $fp, 16
	addu	$fp, $1
	sltu	$1, $fp, $1
	srl	$fp, $fp, 16
	addu	$fp, $1
	xori	$fp, 0xffff
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$fp,60($sp)
	.set	macro
	.set	reorder

	addiu	$4,$2,34
	li	$6,20			# 0x14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$5,$19
	.set	macro
	.set	reorder

	lw	$2,24($sp)
	li	$3,4			# 0x4
	sh	$0,30($sp)
	ins	$2,$3,4,4
	li	$3,5			# 0x5
	ins	$2,$3,0,4
	sw	$2,24($sp)
	li	$2,6			# 0x6
	sb	$2,33($sp)
	sb	$0,25($sp)
	lw	$2,12($16)
	lbu	$3,14($21)
	move	$4,$18
	sw	$2,40($sp)
	lw	$2,16($16)
	move	$5,$0
	sb	$3,32($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ip_summed
	sw	$2,36($sp)
	.set	macro
	.set	reorder

	lhu	$2,%lo(url_cache)($22)
	li	$3,10240			# 0x2800
	sh	$3,26($sp)
	addiu	$2,$2,1
	sh	$2,28($sp)
	addiu	$5,$sp,24
	sh	$0,34($sp)
	lw	$6,24($sp)
	lw	$7,4($5)
	lw	$3,8($5)
	lw	$4,36($sp)
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
	addiu	$3,$sp,40
$L333:
	lw	$4,0($3)
	addiu	$3,$3,4
	addu	$2,$2,$4
	sltu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$6,$3,$L333
	addu	$2,$4,$2
	.set	macro
	.set	reorder

	move	$4,$20
	sw	$5,64($sp)
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
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$2,34($sp)
	.set	macro
	.set	reorder

	lw	$5,64($sp)
	addiu	$4,$2,14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,20			# 0x14
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	move	$4,$20
	sb	$2,16($21)
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sb	$0,15($21)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L351
	lw	$31,108($sp)
	.set	macro
	.set	reorder

$L346:
 #APP
 # 185 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	addiu	$2,$19,20
$L322:
 #APP
 # 207 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$4, 0($2)	# atomic_sub_return
	subu	$3, $4, 1
	sc	$3, 0($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	beq	$3,$0,$L322
 #APP
 # 228 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$4,$2,$L350
	lui	$4,%hi($LC24)
$L353:
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC24)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L351
	lw	$31,108($sp)
	.set	macro
	.set	reorder

$L348:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L352
	move	$4,$20
	.set	macro
	.set	reorder

$L350:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L353
	lui	$4,%hi($LC24)
	.set	macro
	.set	reorder

$L347:
	j	$L342
	.end	GenerateTCPFINACK
	.size	GenerateTCPFINACK, .-GenerateTCPFINACK
	.section	.rodata.str1.4
	.align	2
$LC25:
	.ascii	"HTTP/1.1 401 The web site is blocked by administrator\015"
	.ascii	"\012Server: Embedded HTTP Server 2.00\015\012Connection:"
	.ascii	" close\015\012\015\012<HTML><HEAD><TITLE>401 The web sit"
	.ascii	"e is blocked by administrator</TITLE></HEAD>\012<BODY BG"
	.ascii	"COLOR=\"#ffffff\"><H4>401 The web site is blocked by adm"
	.ascii	"inistrator</H4></BODY></HTML>\012\000"
	.text
	.align	2
	.globl	GenerateHTTP401
	.set	nomips16
	.ent	GenerateHTTP401
	.type	GenerateHTTP401, @function
GenerateHTTP401:
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
	move	$17,$4
	lui	$4,%hi($LC20)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC20)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	sw	$2,128($sp)
	.set	macro
	.set	reorder

	lw	$5,128($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	addiu	$4,$2,12
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L355
	move	$16,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L390
	lui	$18,%hi(block_message)
	.set	macro
	.set	reorder

	lw	$4,%lo(block_message)($18)
	beq	$4,$0,$L359
$L388:
	jal	strlen
	move	$6,$2
	move	$4,$17
	li	$5,96			# 0x60
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L391
	move	$21,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	move	$20,$2
	lw	$2,0($2)
	move	$4,$21
	andi	$2,$2,0xf
	sll	$22,$2,2
	addu	$22,$20,$22
	lw	$3,12($22)
	ext	$3,$3,4,4
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sll	$5,$2,2
	.set	macro
	.set	reorder

	lw	$4,%lo(block_message)($18)
	beq	$4,$0,$L364
$L389:
	jal	strlen
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$4,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$16
	.set	macro
	.set	reorder

	move	$19,$2
	addiu	$fp,$2,4
$L386:
	lw	$23,0($fp)
	andi	$2,$23,0x1
	bne	$2,$0,$L392
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$21
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	sw	$2,128($sp)
	.set	macro
	.set	reorder

	lw	$3,128($sp)
	move	$5,$2
	li	$6,16			# 0x10
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$3,-16
	.set	macro
	.set	reorder

	lw	$2,4($19)
	bne	$23,$2,$L386
	.set	noreorder
	.set	nomacro
	jal	rtl_get_len_from_hh
	move	$4,$19
	.set	macro
	.set	reorder

	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$4,$21
	.set	macro
	.set	reorder

 #APP
 # 185 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	addiu	$2,$16,20
$L368:
 #APP
 # 207 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, 0($2)	# atomic_sub_return
	subu	$4, $3, 1
	sc	$4, 0($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L368
 #APP
 # 228 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L393
	move	$5,$0
$L399:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$4,$21
	.set	macro
	.set	reorder

	lw	$19,0($20)
	lw	$2,12($20)
	lw	$3,16($20)
	andi	$19,$19,0xf
	sll	$19,$19,2
	sw	$3,12($20)
	sw	$2,16($20)
	addu	$19,$20,$19
	lhu	$3,0($19)
	lhu	$2,2($19)
	lw	$16,12($19)
	sh	$3,2($19)
	sh	$2,0($19)
	lw	$5,%lo(block_message)($18)
	ext	$16,$16,4,4
	sll	$16,$16,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L370
	addu	$16,$19,$16
	.set	macro
	.set	reorder

	move	$4,$5
	.set	noreorder
	.set	nomacro
	jal	strlen
	sw	$5,128($sp)
	.set	macro
	.set	reorder

	lw	$5,128($sp)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$6,$2,1
	.set	macro
	.set	reorder

	lw	$2,8($19)
$L398:
	move	$4,$17
	lw	$16,4($19)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sw	$2,4($19)
	.set	macro
	.set	reorder

	lw	$4,12($22)
	lw	$3,0($20)
	lui	$18,%hi(url_cache)
	ext	$4,$4,4,4
	andi	$3,$3,0xf
	addu	$3,$4,$3
	sll	$3,$3,2
	subu	$16,$16,$3
	addu	$16,$16,$2
	sw	$16,8($19)
	addiu	$17,$18,%lo(url_cache)
	lhu	$2,12($17)
	sb	$0,13($19)
	move	$4,$21
	sh	$2,14($19)
	lw	$2,12($19)
	sh	$0,18($19)
	ins	$2,$0,10,1
	ori	$2,$2,0x1800
	ins	$2,$0,8,1
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sw	$2,12($19)
	.set	macro
	.set	reorder

	lw	$4,12($22)
	lw	$22,0($20)
	ext	$4,$4,4,4
	andi	$22,$22,0xf
	sll	$22,$22,2
	sll	$4,$4,2
	subu	$22,$2,$22
	sltu	$3,$4,21
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L372
	addiu	$22,$22,-14
	.set	macro
	.set	reorder

	li	$16,20			# 0x14
	.set	noreorder
	.set	nomacro
	j	$L376
	li	$7,8			# 0x8
	.set	macro
	.set	reorder

$L373:
	.set	noreorder
	.set	nomacro
	beq	$2,$7,$L394
	lbu	$5,1($6)
	.set	macro
	.set	reorder

	addu	$16,$16,$5
	sltu	$2,$16,$4
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L396
	move	$5,$22
	.set	macro
	.set	reorder

$L376:
	addu	$2,$19,$16
$L401:
	lbu	$2,0($2)
	addu	$6,$19,$16
	sltu	$5,$2,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L373
	addiu	$3,$16,2
	.set	macro
	.set	reorder

	addiu	$16,$16,1
	sltu	$2,$16,$4
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L401
	addu	$2,$19,$16
	.set	macro
	.set	reorder

$L372:
	move	$5,$22
$L396:
	andi	$22,$22,0xffff
	sh	$0,16($19)
	move	$6,$0
	move	$4,$19
	addiu	$22,$22,6
	lw	$23,16($20)
	lw	$16,12($20)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	sll	$22,$22,8
	.set	macro
	.set	reorder

	move	$4,$21
 #APP
 # 136 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
		.set	push		# csum_tcpudp_nofold
	.set	noat
	addu	$23, $16
	sltu	$1, $23, $16
	addu	$23, $1
	addu	$23, $22
	sltu	$1, $23, $22
	addu	$23, $1
	addu	$23, $2
	sltu	$1, $23, $2
	addu	$23, $1
	.set	pop
 # 0 "" 2
 # 78 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat
	sll	$1, $23, 16
	addu	$23, $1
	sltu	$1, $23, $1
	srl	$23, $23, 16
	addu	$23, $1
	xori	$23, 0xffff
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$23,16($19)
	.set	macro
	.set	reorder

	addiu	$2,$2,-14
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	sh	$2,2($20)
	lbu	$2,14($17)
	sh	$0,6($20)
	lw	$3,0($20)
	sb	$2,8($20)
	lhu	$4,%lo(url_cache)($18)
	sh	$0,10($20)
	lw	$2,8($20)
	addiu	$4,$4,2
	sh	$4,4($20)
	lw	$6,4($20)
	lw	$4,12($20)
	andi	$5,$3,0xf
	addu	$3,$6,$3
	sltu	$6,$3,$6
	addu	$3,$3,$2
	addu	$6,$3,$6
	sltu	$2,$6,$2
	addu	$6,$6,$4
	addu	$2,$6,$2
	sltu	$3,$2,$4
	sll	$5,$5,2
	addu	$3,$3,$2
	addu	$5,$20,$5
	addiu	$2,$20,16
$L377:
	lw	$4,0($2)
	addiu	$2,$2,4
	addu	$3,$3,$4
	sltu	$4,$3,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L377
	addu	$3,$4,$3
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
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
	move	$4,$21
	sh	$3,10($20)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sb	$2,15($17)
	.set	macro
	.set	reorder

	move	$5,$2
	addiu	$4,$sp,24
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,54			# 0x36
	.set	macro
	.set	reorder

	lhu	$3,2($19)
	lui	$2,%hi(finStateClientPort)
	move	$4,$21
	sh	$3,%lo(finStateClientPort)($2)
	lw	$3,8($19)
	lui	$2,%hi(finStateClientSeq)
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sw	$3,%lo(finStateClientSeq)($2)
	.set	macro
	.set	reorder

$L355:
	lw	$31,172($sp)
$L397:
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

$L391:
 #APP
 # 185 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	addiu	$2,$16,20
$L362:
 #APP
 # 207 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, 0($2)	# atomic_sub_return
	subu	$4, $3, 1
	sc	$4, 0($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L362
 #APP
 # 228 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L395
	lui	$4,%hi($LC24)
$L400:
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC24)
	.set	macro
	.set	reorder

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
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,176
	.set	macro
	.set	reorder

$L394:
	addu	$3,$19,$3
	addiu	$23,$sp,16
	addiu	$16,$16,6
	move	$5,$3
	move	$4,$23
	addu	$16,$19,$16
	addiu	$fp,$sp,20
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$3,128($sp)
	.set	macro
	.set	reorder

	move	$4,$fp
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	lw	$3,128($sp)
	move	$5,$fp
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$4,$3
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$23
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L396
	move	$5,$22
	.set	macro
	.set	reorder

$L390:
 #APP
 # 185 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	addiu	$2,$16,20
$L358:
 #APP
 # 207 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, 0($2)	# atomic_sub_return
	subu	$4, $3, 1
	sc	$4, 0($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L358
 #APP
 # 228 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	bne	$3,$2,$L355
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L397
	lw	$31,172($sp)
	.set	macro
	.set	reorder

$L359:
	lui	$4,%hi($LC25)
	.set	noreorder
	.set	nomacro
	j	$L388
	addiu	$4,$4,%lo($LC25)
	.set	macro
	.set	reorder

$L370:
	lui	$18,%hi($LC25)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$18,%lo($LC25)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$18,%lo($LC25)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L398
	lw	$2,8($19)
	.set	macro
	.set	reorder

$L364:
	lui	$4,%hi($LC25)
	.set	noreorder
	.set	nomacro
	j	$L389
	addiu	$4,$4,%lo($LC25)
	.set	macro
	.set	reorder

$L393:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L399
	move	$5,$0
	.set	macro
	.set	reorder

$L395:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L400
	lui	$4,%hi($LC24)
	.set	macro
	.set	reorder

$L392:
	j	$L386
	.end	GenerateHTTP401
	.size	GenerateHTTP401, .-GenerateHTTP401
	.section	.rodata.str1.4
	.align	2
$LC26:
	.ascii	"%s\000"
	.text
	.align	2
	.globl	register_customRspStr
	.set	nomips16
	.ent	register_customRspStr
	.type	register_customRspStr, @function
register_customRspStr:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	jal	unregister_customRspStr
	move	$16,$4

	beq	$16,$0,$L403
	nop

	jal	strlen
	move	$4,$16

	addiu	$4,$2,1
	jal	__kmalloc
	li	$5,208			# 0xd0

	lui	$3,%hi(block_message)
	beq	$2,$0,$L405
	sw	$2,%lo(block_message)($3)

	move	$6,$16
	lw	$31,20($sp)
	lw	$16,16($sp)
	lui	$5,%hi($LC26)
	move	$4,$2
	addiu	$5,$5,%lo($LC26)
	j	sprintf
	addiu	$sp,$sp,24

$L405:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

$L403:
	lui	$16,%hi($LC25)
	jal	strlen
	addiu	$4,$16,%lo($LC25)

	move	$4,$2
	jal	__kmalloc
	li	$5,208			# 0xd0

	lui	$3,%hi(block_message)
	beq	$2,$0,$L405
	sw	$2,%lo(block_message)($3)

	addiu	$6,$16,%lo($LC25)
	lw	$31,20($sp)
	lw	$16,16($sp)
	lui	$5,%hi($LC26)
	move	$4,$2
	addiu	$5,$5,%lo($LC26)
	j	sprintf
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	register_customRspStr
	.size	register_customRspStr, .-register_customRspStr
	.section	.rodata.str1.4
	.align	2
$LC27:
	.ascii	"%u.%u.%u.%u\000"
	.text
	.align	2
	.globl	IP2NAME
	.set	nomips16
	.ent	IP2NAME
	.type	IP2NAME, @function
IP2NAME:
	.frame	$sp,32,$31		# vars= 0, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	move	$16,$5
	sw	$4,32($sp)
	move	$5,$0
	move	$4,$16
	jal	memset
	li	$6,16			# 0x10

	lbu	$3,34($sp)
	lbu	$2,35($sp)
	lbu	$6,32($sp)
	lbu	$7,33($sp)
	lui	$5,%hi($LC27)
	move	$4,$16
	addiu	$5,$5,%lo($LC27)
	sw	$3,16($sp)
	jal	sprintf
	sw	$2,20($sp)

	move	$2,$16
	lw	$31,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	IP2NAME
	.size	IP2NAME, .-IP2NAME
	.section	.rodata.str1.4
	.align	2
$LC28:
	.ascii	"%s\012\000"
	.align	2
$LC29:
	.ascii	"connectip = %s\012\000"
	.align	2
$LC30:
	.ascii	"pkt---IP:%s, syn_cnt:%d, fin_cnt:%d, udp_nct:%d, icmp_cn"
	.ascii	"t:%d, scan_cnt:%d\012\000"
	.align	2
$LC31:
	.ascii	"attack_saddr[%s]=%s,attack_daddr[%s]=%s\012\000"
	.align	2
$LC32:
	.ascii	"op_mode = %d, lan_addr = %s, lan_mask = %s, item = 0x%x,"
	.ascii	"item_0 = 0x%x\012\000"
	.text
	.align	2
	.set	nomips16
	.ent	dos_read_proc
	.type	dos_read_proc, @function
dos_read_proc:
	.frame	$sp,80,$31		# vars= 8, regs= 10/0, args= 32, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$fp,72($sp)
	sw	$23,68($sp)
	sw	$22,64($sp)
	sw	$21,60($sp)
	sw	$20,56($sp)
	sw	$19,52($sp)
	sw	$18,48($sp)
	sw	$17,44($sp)
	sw	$16,40($sp)
	lui	$5,%hi($LC28)
	lui	$6,%hi(dos_flag)
	addiu	$5,$5,%lo($LC28)
	addiu	$6,$6,%lo(dos_flag)
	lui	$fp,%hi(ip_name0)
	lui	$16,%hi(ConnectedIp)
	lui	$17,%hi(ConnectedIp+1024)
	lui	$20,%hi($LC29)
	move	$19,$4
	jal	seq_printf
	addiu	$16,$16,%lo(ConnectedIp)

	addiu	$17,$17,%lo(ConnectedIp+1024)
	addiu	$20,$20,%lo($LC29)
	addiu	$18,$fp,%lo(ip_name0)
$L411:
	lw	$4,0($16)
	bne	$4,$0,$L422
	addiu	$16,$16,4

	bne	$16,$17,$L411
	nop

	lui	$16,%hi(dos_pkt)
$L427:
	jal	local_bh_disable
	addiu	$16,$16,%lo(dos_pkt)

	lb	$2,0($16)
	lui	$18,%hi(dos_pkt+40920)
	lui	$21,%hi($LC30)
	li	$17,1			# 0x1
	addiu	$18,$18,%lo(dos_pkt+40920)
	addiu	$21,$21,%lo($LC30)
	beq	$2,$17,$L423
	addiu	$20,$fp,%lo(ip_name0)

$L412:
	beq	$16,$18,$L413
	nop

	addiu	$16,$16,40
$L428:
	lb	$2,0($16)
	bne	$2,$17,$L412
	nop

$L423:
	lw	$4,28($16)
	jal	IP2NAME
	move	$5,$20

	lw	$7,4($16)
	lw	$10,8($16)
	lw	$9,12($16)
	lw	$8,16($16)
	lw	$3,24($16)
	move	$6,$2
	move	$4,$19
	move	$5,$21
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	jal	seq_printf
	sw	$3,28($sp)

	bne	$16,$18,$L428
	addiu	$16,$16,40

	addiu	$16,$16,-40
$L413:
	lui	$16,%hi(attack_daddr)
	lui	$17,%hi(attack_saddr)
	lui	$18,%hi(rtl_dos_name)
	lui	$21,%hi(attack_daddr+108)
	lui	$2,%hi(ip_name1)
	addiu	$16,$16,%lo(attack_daddr)
	addiu	$17,$17,%lo(attack_saddr)
	addiu	$18,$18,%lo(rtl_dos_name)
	addiu	$21,$21,%lo(attack_daddr+108)
	addiu	$23,$fp,%lo(ip_name0)
	j	$L418
	addiu	$22,$2,%lo(ip_name1)

$L415:
	lw	$4,0($17)
	beq	$4,$0,$L417
	nop

$L416:
	jal	IP2NAME
	move	$5,$23

	lw	$4,0($16)
	lw	$20,0($18)
	move	$5,$22
	jal	IP2NAME
	sw	$2,36($sp)

	lw	$7,36($sp)
	lui	$5,%hi($LC31)
	move	$6,$20
	move	$4,$19
	addiu	$5,$5,%lo($LC31)
	sw	$2,20($sp)
	jal	seq_printf
	sw	$20,16($sp)

$L417:
	addiu	$16,$16,4
	addiu	$17,$17,4
	beq	$16,$21,$L425
	addiu	$18,$18,4

$L418:
	lw	$2,0($16)
	beq	$2,$0,$L415
	nop

	j	$L416
	lw	$4,0($17)

$L422:
	jal	IP2NAME
	move	$5,$18

	move	$6,$2
	move	$4,$19
	jal	seq_printf
	move	$5,$20

	bne	$16,$17,$L411
	nop

	j	$L427
	lui	$16,%hi(dos_pkt)

$L425:
	jal	local_bh_enable
	nop

	lui	$2,%hi(lan_addr)
	lw	$4,%lo(lan_addr)($2)
	lui	$2,%hi(op_mode)
	lb	$6,%lo(op_mode)($2)
	addiu	$5,$fp,%lo(ip_name0)
	jal	IP2NAME
	sw	$6,32($sp)

	move	$7,$2
	lui	$2,%hi(lan_mask)
	lw	$4,%lo(lan_mask)($2)
	lui	$2,%hi(ip_name1)
	addiu	$5,$2,%lo(ip_name1)
	jal	IP2NAME
	sw	$7,36($sp)

	lui	$3,%hi(item)
	lw	$8,%lo(item)($3)
	lui	$3,%hi(item_0)
	lw	$3,%lo(item_0)($3)
	lw	$6,32($sp)
	lw	$7,36($sp)
	lui	$5,%hi($LC32)
	move	$4,$19
	addiu	$5,$5,%lo($LC32)
	sw	$2,16($sp)
	sw	$8,20($sp)
	jal	seq_printf
	sw	$3,24($sp)

	lw	$31,76($sp)
	lw	$fp,72($sp)
	lw	$23,68($sp)
	lw	$22,64($sp)
	lw	$21,60($sp)
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,80

	.set	macro
	.set	reorder
	.end	dos_read_proc
	.size	dos_read_proc, .-dos_read_proc
	.align	2
	.globl	strtok
	.set	nomips16
	.ent	strtok
	.type	strtok, @function
strtok:
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
	beq	$4,$0,$L430
	move	$17,$5

	move	$16,$4
	lui	$18,%hi(___strtok)
$L431:
	move	$4,$16
	jal	strspn
	move	$5,$17

	addu	$16,$16,$2
	lb	$2,0($16)
	beq	$2,$0,$L437
	move	$5,$17

	jal	strpbrk
	move	$4,$16

	beq	$2,$0,$L434
	nop

	lb	$3,0($2)
	bne	$3,$0,$L438
	nop

$L434:
	sw	$2,%lo(___strtok)($18)
	lw	$31,28($sp)
	move	$2,$16
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

$L438:
	sb	$0,0($2)
	j	$L434
	addiu	$2,$2,1

$L437:
	move	$16,$0
	move	$2,$16
	sw	$0,%lo(___strtok)($18)
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

$L430:
	lui	$18,%hi(___strtok)
	lw	$16,%lo(___strtok)($18)
	bne	$16,$0,$L431
	move	$2,$16

	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	strtok
	.size	strtok, .-strtok
	.section	.rodata.str1.4
	.align	2
$LC33:
	.ascii	" \000"
	.text
	.align	2
	.set	nomips16
	.ent	dos_single_write
	.type	dos_single_write, @function
dos_single_write:
	.frame	$sp,136,$31		# vars= 80, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-136
	sw	$31,132($sp)
	sw	$fp,128($sp)
	sw	$23,124($sp)
	sw	$22,120($sp)
	sw	$21,116($sp)
	sw	$20,112($sp)
	sw	$19,108($sp)
	sw	$18,104($sp)
	sw	$17,100($sp)
	sw	$16,96($sp)
	sltu	$2,$6,2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L461
	move	$16,$6
	.set	macro
	.set	reorder

$L440:
	li	$16,-14			# 0xfffffffffffffff2
$L459:
	move	$2,$16
	lw	$31,132($sp)
	lw	$fp,128($sp)
	lw	$23,124($sp)
	lw	$22,120($sp)
	lw	$21,116($sp)
	lw	$20,112($sp)
	lw	$19,108($sp)
	lw	$18,104($sp)
	lw	$17,100($sp)
	lw	$16,96($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,136
	.set	macro
	.set	reorder

$L461:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L440
	addu	$2,$5,$6
	.set	macro
	.set	reorder

	or	$3,$5,$6
	or	$3,$3,$2
	lw	$2,24($28)
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L440
	lui	$2,%hi(dos_flag)
	.set	macro
	.set	reorder

	addiu	$4,$2,%lo(dos_flag)
 #APP
 # 3483 "net/rtl/fastpath/filter.c" 1
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
	bne	$6,$0,$L440
	addiu	$4,$sp,16
	.set	macro
	.set	reorder

	addiu	$2,$2,%lo(dos_flag)
	li	$5,80			# 0x50
	move	$3,$4
 #APP
 # 52 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($2)
	subu	$5,1
	sb	$1,($3)
	beqz	$1,2f
	addiu	$3,1
	bnez	$5,1b
	addiu	$2,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	lui	$19,%hi($LC33)
	lui	$20,%hi($L457)
	addiu	$19,$19,%lo($LC33)
	addiu	$20,$20,%lo($L457)
	li	$17,1			# 0x1
	lui	$fp,%hi(per_syn_threshold)
	lui	$23,%hi(whole_icmp_threshold)
	lui	$22,%hi(whole_udp_threshold)
	lui	$21,%hi(whole_fin_threshold)
$L441:
	.set	noreorder
	.set	nomacro
	jal	strtok
	move	$5,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L459
	move	$18,$2
	.set	macro
	.set	reorder

	move	$4,$18
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	move	$6,$0
	.set	macro
	.set	reorder

	sltu	$3,$17,15
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L442
	sll	$3,$17,2
	.set	macro
	.set	reorder

	addu	$3,$20,$3
	lw	$3,0($3)
	j	$3
	.rdata
	.align	2
	.align	2
$L457:
	.word	$L442
	.word	$L443
	.word	$L444
	.word	$L445
	.word	$L446
	.word	$L447
	.word	$L448
	.word	$L449
	.word	$L450
	.word	$L451
	.word	$L452
	.word	$L453
	.word	$L454
	.word	$L455
	.word	$L456
	.text
$L443:
	lui	$3,%hi(op_mode)
	sb	$2,%lo(op_mode)($3)
$L442:
	addiu	$17,$17,1
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L441
	move	$4,$0
	.set	macro
	.set	reorder

$L456:
	lui	$3,%hi(item_0)
	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$2,%lo(item_0)($3)
	.set	macro
	.set	reorder

$L455:
	lui	$3,%hi(block_time)
	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$2,%lo(block_time)($3)
	.set	macro
	.set	reorder

$L454:
	lui	$3,%hi(per_icmp_threshold)
	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$2,%lo(per_icmp_threshold)($3)
	.set	macro
	.set	reorder

$L453:
	lui	$3,%hi(per_udp_threshold)
	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$2,%lo(per_udp_threshold)($3)
	.set	macro
	.set	reorder

$L452:
	lui	$3,%hi(per_fin_threshold)
	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$2,%lo(per_fin_threshold)($3)
	.set	macro
	.set	reorder

$L451:
	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$2,%lo(per_syn_threshold)($fp)
	.set	macro
	.set	reorder

$L450:
	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$2,%lo(whole_icmp_threshold)($23)
	.set	macro
	.set	reorder

$L449:
	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$2,%lo(whole_udp_threshold)($22)
	.set	macro
	.set	reorder

$L448:
	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$2,%lo(whole_fin_threshold)($21)
	.set	macro
	.set	reorder

$L447:
	lui	$3,%hi(whole_syn_threshold)
	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$2,%lo(whole_syn_threshold)($3)
	.set	macro
	.set	reorder

$L446:
	lui	$3,%hi(item)
	sw	$2,%lo(item)($3)
	lui	$2,%hi(item_backup)
	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$0,%lo(item_backup)($2)
	.set	macro
	.set	reorder

$L445:
	move	$4,$18
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lui	$3,%hi(lan_mask)
	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$2,%lo(lan_mask)($3)
	.set	macro
	.set	reorder

$L444:
	move	$4,$18
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lui	$3,%hi(lan_addr)
	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$2,%lo(lan_addr)($3)
	.set	macro
	.set	reorder

	.end	dos_single_write
	.size	dos_single_write, .-dos_single_write
	.section	.rodata.str1.4
	.align	2
$LC34:
	.ascii	"[ip_range_filter]%d.%d.%d.%d ===> %d.%d.%d.%d  [Blocked]"
	.ascii	"\012\000"
	.align	2
$LC35:
	.ascii	"[mac_filter]%d.%d.%d.%d(%02x:%02x:%02x:%02x:%02x:%02x) ="
	.ascii	"==> %d.%d.%d.%d [Blocked]\012\000"
	.align	2
$LC36:
	.ascii	"[url_key_filter]%d.%d.%d.%d ==> %d.%d.%d.%d(%s)  [Blocke"
	.ascii	"d]\012\000"
	.align	2
$LC37:
	.ascii	"[schedule_filter]%d.%d.%d.%d ===> %d.%d.%d.%d  [Blocked]"
	.ascii	"\012\000"
	.align	2
$LC38:
	.ascii	"Unkown Type [Blocked]\012\000"
	.text
	.align	2
	.globl	log_fastFilter
	.set	nomips16
	.ent	log_fastFilter
	.type	log_fastFilter, @function
log_fastFilter:
	.frame	$sp,184,$31		# vars= 8, regs= 9/0, args= 136, gp= 0
	.mask	0x80ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-184
	sw	$31,180($sp)
	sw	$23,176($sp)
	sw	$22,172($sp)
	sw	$21,168($sp)
	sw	$20,164($sp)
	sw	$19,160($sp)
	sw	$18,156($sp)
	sw	$17,152($sp)
	sw	$16,148($sp)
	move	$21,$5
	move	$16,$4
	jal	do_gettimeofday
	addiu	$4,$sp,136

	jal	rtl_ip_hdr
	move	$4,$21

	move	$4,$21
	jal	rtl_eth_hdr
	move	$22,$2

	li	$3,2			# 0x2
	beq	$16,$3,$L465
	nop

	slt	$2,$16,3
	beq	$2,$0,$L468
	li	$2,3			# 0x3

	li	$2,1			# 0x1
	beq	$16,$2,$L472
	lui	$4,%hi($LC38)

$L473:
	jal	printk
	addiu	$4,$4,%lo($LC38)

	lw	$31,180($sp)
$L474:
	lw	$23,176($sp)
$L475:
	lw	$22,172($sp)
	lw	$21,168($sp)
	lw	$20,164($sp)
	lw	$19,160($sp)
	lw	$18,156($sp)
	lw	$17,152($sp)
	lw	$16,148($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,184

$L468:
	beq	$16,$2,$L466
	li	$2,4			# 0x4

	bne	$16,$2,$L473
	lui	$4,%hi($LC38)

	beq	$22,$0,$L474
	lw	$31,180($sp)

	lw	$3,16($22)
	lw	$6,12($22)
	lui	$4,%hi($LC37)
	andi	$2,$3,0xff
	addiu	$4,$4,%lo($LC37)
$L471:
	andi	$10,$6,0xff
	srl	$9,$3,24
	ext	$8,$3,16,8
	ext	$7,$6,8,8
	ext	$3,$3,8,8
	srl	$5,$6,24
	ext	$6,$6,16,8
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	lw	$31,180($sp)
	lw	$23,176($sp)
	lw	$22,172($sp)
	lw	$21,168($sp)
	lw	$20,164($sp)
	lw	$19,160($sp)
	lw	$18,156($sp)
	lw	$17,152($sp)
	lw	$16,148($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,184

$L466:
	beq	$22,$0,$L474
	lw	$31,180($sp)

	lw	$3,16($22)
	lw	$6,12($22)
	lui	$4,%hi($LC36)
	andi	$2,$3,0xff
	sw	$2,32($sp)
	lui	$2,%hi(log_info)
	andi	$10,$6,0xff
	srl	$9,$3,24
	ext	$8,$3,16,8
	ext	$7,$6,8,8
	ext	$3,$3,8,8
	srl	$5,$6,24
	addiu	$4,$4,%lo($LC36)
	ext	$6,$6,16,8
	addiu	$2,$2,%lo(log_info)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,36($sp)

	lw	$31,180($sp)
	lw	$23,176($sp)
	lw	$22,172($sp)
	lw	$21,168($sp)
	lw	$20,164($sp)
	lw	$19,160($sp)
	lw	$18,156($sp)
	lw	$17,152($sp)
	lw	$16,148($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,184

$L465:
	beq	$2,$0,$L474
	lw	$31,180($sp)

	beq	$22,$0,$L475
	lw	$23,176($sp)

	move	$4,$21
	jal	rtl_eth_hdr
	lw	$19,12($22)

	move	$4,$21
	jal	rtl_eth_hdr
	move	$23,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$20,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$18,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$17,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$16,$2

	andi	$3,$19,0xff
	lw	$11,16($22)
	sw	$3,16($sp)
	lwl	$5,87($23)
	lwl	$4,91($23)
	lwl	$3,95($23)
	lwr	$5,84($23)
	lwr	$4,88($23)
	lwr	$3,92($23)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$3,28($sp)
	lbu	$3,96($23)
	lui	$4,%hi($LC35)
	andi	$10,$11,0xff
	sb	$3,32($sp)
	lbu	$3,97($23)
	srl	$13,$11,24
	ext	$12,$11,16,8
	sb	$3,33($sp)
	lwl	$6,101($20)
	lwl	$5,105($20)
	lwl	$3,109($20)
	lwr	$6,98($20)
	lwr	$5,102($20)
	lwr	$3,106($20)
	sw	$6,36($sp)
	sw	$5,40($sp)
	sw	$3,44($sp)
	lbu	$3,110($20)
	ext	$7,$19,8,8
	ext	$11,$11,8,8
	sb	$3,48($sp)
	lbu	$3,111($20)
	srl	$5,$19,24
	addiu	$4,$4,%lo($LC35)
	sb	$3,49($sp)
	lwl	$8,115($18)
	lwl	$6,119($18)
	lwl	$3,123($18)
	lwr	$8,112($18)
	lwr	$6,116($18)
	lwr	$3,120($18)
	sw	$8,52($sp)
	sw	$6,56($sp)
	sw	$3,60($sp)
	lbu	$3,124($18)
	ext	$6,$19,16,8
	sb	$3,64($sp)
	lbu	$3,125($18)
	sb	$3,65($sp)
	lwl	$9,129($17)
	lwl	$8,133($17)
	lwl	$3,137($17)
	lwr	$9,126($17)
	lwr	$8,130($17)
	lwr	$3,134($17)
	sw	$9,68($sp)
	sw	$8,72($sp)
	sw	$3,76($sp)
	lbu	$3,138($17)
	sb	$3,80($sp)
	lbu	$3,139($17)
	sb	$3,81($sp)
	lwl	$9,143($16)
	lwl	$8,147($16)
	lwl	$3,151($16)
	lwr	$9,140($16)
	lwr	$8,144($16)
	lwr	$3,148($16)
	sw	$9,84($sp)
	sw	$8,88($sp)
	sw	$3,92($sp)
	lbu	$3,152($16)
	sb	$3,96($sp)
	lbu	$3,153($16)
	sb	$3,97($sp)
	lwl	$9,157($2)
	lwl	$8,161($2)
	lwl	$3,165($2)
	lwr	$9,154($2)
	lwr	$8,158($2)
	lwr	$3,162($2)
	sw	$9,100($sp)
	sw	$8,104($sp)
	sw	$3,108($sp)
	lbu	$3,166($2)
	sb	$3,112($sp)
	lbu	$2,167($2)
	sw	$13,116($sp)
	sw	$12,120($sp)
	sb	$2,113($sp)
	sw	$11,124($sp)
	jal	scrlog_printk
	sw	$10,128($sp)

	lw	$31,180($sp)
	lw	$23,176($sp)
	lw	$22,172($sp)
	lw	$21,168($sp)
	lw	$20,164($sp)
	lw	$19,160($sp)
	lw	$18,156($sp)
	lw	$17,152($sp)
	lw	$16,148($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,184

$L472:
	beq	$22,$0,$L474
	lw	$31,180($sp)

	lw	$3,16($22)
	lui	$4,%hi($LC34)
	lw	$6,12($22)
	andi	$2,$3,0xff
	j	$L471
	addiu	$4,$4,%lo($LC34)

	.set	macro
	.set	reorder
	.end	log_fastFilter
	.size	log_fastFilter, .-log_fastFilter
	.align	2
	.globl	do_filter
	.set	nomips16
	.ent	do_filter
	.type	do_filter, @function
do_filter:
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
	lui	$17,%hi(table_list_head)
	lw	$16,%lo(table_list_head)($17)
	addiu	$17,$17,%lo(table_list_head)
	beq	$16,$17,$L477
	move	$18,$4

	li	$19,-1			# 0xffffffffffffffff
	j	$L483
	li	$20,1			# 0x1

$L486:
	beq	$2,$20,$L485
	lui	$2,%hi(log_enable)

	lw	$16,0($16)
	beq	$16,$17,$L480
	move	$2,$0

$L483:
	move	$6,$16
	move	$7,$16
	move	$4,$18
	jal	scan_table
	move	$5,$0

	bne	$2,$19,$L486
	nop

$L477:
	move	$2,$0
$L480:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

$L485:
	lw	$3,%lo(log_enable)($2)
	beq	$3,$0,$L480
	li	$2,1			# 0x1

	lw	$4,16($16)
	jal	log_fastFilter
	move	$5,$18

	j	$L480
	li	$2,1			# 0x1

	.set	macro
	.set	reorder
	.end	do_filter
	.size	do_filter, .-do_filter
	.align	2
	.globl	FilterWeb_v2
	.set	nomips16
	.ent	FilterWeb_v2
	.type	FilterWeb_v2, @function
FilterWeb_v2:
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

	jal	need_filter
	move	$4,$16

	bne	$2,$0,$L488
	lw	$31,20($sp)

	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

$L488:
	move	$4,$16
	lw	$16,16($sp)
	j	do_filter
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	FilterWeb_v2
	.size	FilterWeb_v2, .-FilterWeb_v2
	.align	2
	.globl	filter_enter
	.set	nomips16
	.ent	filter_enter
	.type	filter_enter, @function
filter_enter:
	.frame	$sp,48,$31		# vars= 0, regs= 7/0, args= 16, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$21,40($sp)
	sw	$20,36($sp)
	sw	$19,32($sp)
	sw	$18,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$17,%hi(item)
	lw	$2,%lo(item)($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L492
	move	$16,$4
	.set	macro
	.set	reorder

	lui	$2,%hi(table_list_head+24)
	lw	$2,%lo(table_list_head+24)($2)
	sltu	$2,$2,2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L625
	lui	$3,%hi(none_dos_drop_pkt_cnt)
	.set	macro
	.set	reorder

$L492:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_type
	move	$4,$16
	.set	macro
	.set	reorder

	beq	$2,$0,$L626
$L494:
	li	$2,4			# 0x4
$L493:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,48
	.set	macro
	.set	reorder

$L626:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_protocol
	move	$4,$16
	.set	macro
	.set	reorder

	li	$3,8			# 0x8
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L493
	li	$2,4			# 0x4
	.set	macro
	.set	reorder

	lui	$2,%hi(table_list_head+24)
	lw	$2,%lo(table_list_head+24)($2)
	sltu	$2,$2,2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L668
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	FilterWeb_v2
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L627
	lui	$2,%hi(fast_path_cusRsp401_func)
	.set	macro
	.set	reorder

	lw	$2,%lo(fast_path_cusRsp401_func)($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L495
	lui	$3,%hi(url_cache)
	.set	macro
	.set	reorder

	addiu	$3,$3,%lo(url_cache)
	lbu	$4,15($3)
	li	$2,1			# 0x1
	beq	$4,$2,$L628
	lbu	$3,16($3)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L668
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)
	lw	$2,%lo(fast_path_cusRspTCPEndAck_func)($2)
	beq	$2,$0,$L497
$L622:
	.set	noreorder
	.set	nomacro
	jalr	$2
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L493
	move	$2,$0
	.set	macro
	.set	reorder

$L625:
	lw	$4,%lo(none_dos_drop_pkt_cnt)($3)
	li	$2,1			# 0x1
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	j	$L493
	sw	$4,%lo(none_dos_drop_pkt_cnt)($3)
	.set	macro
	.set	reorder

$L495:
	lw	$2,%lo(item)($17)
$L668:
	andi	$2,$2,0x1
	bne	$2,$0,$L629
$L499:
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$16
	.set	macro
	.set	reorder

	lbu	$3,9($2)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L494
	lui	$3,%hi(none_dos_drop_pkt_cnt)
	.set	macro
	.set	reorder

	lw	$4,%lo(none_dos_drop_pkt_cnt)($3)
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	j	$L493
	sw	$4,%lo(none_dos_drop_pkt_cnt)($3)
	.set	macro
	.set	reorder

$L627:
	lw	$2,%lo(fast_path_cusRsp401_func)($2)
	bne	$2,$0,$L622
$L497:
	.set	noreorder
	.set	nomacro
	j	$L493
	move	$2,$0
	.set	macro
	.set	reorder

$L629:
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$4,%hi($LC21)
	addiu	$4,$4,%lo($LC21)
	lw	$21,0($2)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	move	$18,$2
	.set	macro
	.set	reorder

	lui	$4,%hi($LC22)
	addiu	$4,$4,%lo($LC22)
	lui	$20,%hi(wan_ppp_dev)
	sw	$2,%lo(wan_ppp_dev)($20)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	lui	$19,%hi(wan_dev)
	.set	macro
	.set	reorder

	move	$4,$16
	andi	$21,$21,0xf
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	sw	$2,%lo(wan_dev)($19)
	.set	macro
	.set	reorder

	beq	$2,$0,$L499
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,%lo(wan_dev)($19)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L669
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,%lo(wan_ppp_dev)($20)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L499
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

$L669:
	sll	$21,$21,2
	andi	$2,$2,0x3fe
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L501
	addu	$21,$18,$21
	.set	macro
	.set	reorder

	lw	$2,12($18)
	lui	$3,%hi(ConnectedIp)
	addiu	$3,$3,%lo(ConnectedIp)
	andi	$4,$2,0xff
	sll	$4,$4,2
	addu	$3,$4,$3
	andi	$4,$2,0x3ff
	lw	$5,0($3)
	sll	$3,$4,5
	sll	$4,$4,3
	addu	$4,$4,$3
	lui	$3,%hi(dos_pkt)
	addiu	$3,$3,%lo(dos_pkt)
	.set	noreorder
	.set	nomacro
	beq	$2,$5,$L630
	addu	$3,$3,$4
	.set	macro
	.set	reorder

	lw	$4,28($3)
	beq	$2,$4,$L504
$L503:
	lb	$4,0($3)
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L501
	lui	$5,%hi(cur_p_pkt)
	.set	macro
	.set	reorder

	sw	$2,28($3)
	li	$2,1			# 0x1
	sb	$2,0($3)
	sw	$3,%lo(cur_p_pkt)($5)
$L505:
	lw	$2,%lo(item)($17)
	andi	$3,$2,0x22
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L508
	move	$3,$0
	.set	macro
	.set	reorder

	lbu	$4,9($18)
	li	$3,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$4,$3,$L631
	lw	$7,%lo(cur_p_pkt)($5)
	.set	macro
	.set	reorder

	move	$3,$0
$L508:
	andi	$4,$2,0x44
$L667:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L670
	andi	$4,$2,0x88
	.set	macro
	.set	reorder

	lbu	$7,9($18)
	li	$4,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$7,$4,$L632
	lw	$6,%lo(cur_p_pkt)($5)
	.set	macro
	.set	reorder

$L513:
	lw	$2,%lo(item)($17)
$L699:
	andi	$4,$2,0x88
$L670:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L671
	andi	$4,$2,0x110
	.set	macro
	.set	reorder

	lbu	$7,9($18)
	li	$4,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$7,$4,$L633
	lw	$6,%lo(cur_p_pkt)($5)
	.set	macro
	.set	reorder

$L516:
	lw	$2,%lo(item)($17)
$L697:
	andi	$4,$2,0x110
$L671:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L672
	li	$4,3145728			# 0x300000
	.set	macro
	.set	reorder

	lbu	$7,9($18)
	li	$4,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$7,$4,$L634
	lw	$6,%lo(cur_p_pkt)($5)
	.set	macro
	.set	reorder

$L519:
	lw	$2,%lo(item)($17)
$L698:
	li	$4,3145728			# 0x300000
$L672:
	and	$4,$2,$4
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L673
	andi	$4,$2,0x200
	.set	macro
	.set	reorder

	lw	$4,0($18)
	lbu	$7,9($18)
	li	$6,1			# 0x1
	andi	$4,$4,0xf
	.set	noreorder
	.set	nomacro
	beq	$7,$6,$L635
	lw	$2,%lo(cur_p_pkt)($5)
	.set	macro
	.set	reorder

$L522:
	lw	$2,%lo(item)($17)
	andi	$4,$2,0x200
$L673:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L674
	ext	$2,$2,22,1
	.set	macro
	.set	reorder

	lbu	$6,9($18)
	li	$4,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$6,$4,$L611
	lw	$2,%lo(cur_p_pkt)($5)
	.set	macro
	.set	reorder

	lw	$4,24($2)
$L526:
	li	$5,17			# 0x11
	beq	$6,$5,$L636
$L529:
	lw	$2,%lo(item)($17)
	ext	$5,$2,23,1
	beq	$5,$0,$L532
	slt	$4,$4,11
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L525
	lui	$4,%hi(LogFlag)
	.set	macro
	.set	reorder

	lw	$5,%lo(LogFlag)($4)
$L617:
	ori	$5,$5,0x200
	sw	$5,%lo(LogFlag)($4)
	lw	$5,12($18)
	lui	$4,%hi(attack_saddr+24)
	sw	$5,%lo(attack_saddr+24)($4)
	lw	$5,16($18)
	lui	$4,%hi(attack_daddr+24)
	sw	$5,%lo(attack_daddr+24)($4)
$L525:
	ext	$2,$2,22,1
$L674:
	beq	$2,$0,$L501
$L534:
	lui	$4,%hi(item_0)
	lw	$2,%lo(item_0)($4)
	andi	$5,$2,0x1
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L675
	andi	$2,$2,0x2
	.set	macro
	.set	reorder

	lbu	$5,9($18)
	li	$2,6			# 0x6
	beq	$5,$2,$L637
$L536:
	lw	$2,%lo(item_0)($4)
	andi	$2,$2,0x2
$L675:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L676
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lbu	$2,9($18)
	li	$4,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L550
	li	$4,6			# 0x6
	.set	macro
	.set	reorder

	beq	$2,$4,$L550
$L549:
	lw	$2,%lo(item)($17)
$L676:
	li	$4,536870912			# 0x20000000
	and	$5,$2,$4
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L552
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	lbu	$2,9($18)
	beq	$2,$5,$L638
	li	$4,17			# 0x11
	beq	$2,$4,$L555
	li	$4,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L639
	li	$2,20			# 0x14
	.set	macro
	.set	reorder

$L554:
	lw	$2,%lo(item)($17)
$L552:
	li	$4,1073741824			# 0x40000000
	and	$5,$2,$4
	beq	$5,$0,$L557
	lw	$2,0($18)
	lbu	$5,9($18)
	andi	$6,$2,0xf
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$5,$2,$L640
	li	$2,8			# 0x8
	.set	macro
	.set	reorder

$L558:
	lw	$2,%lo(item)($17)
$L557:
	bltz	$2,$L641
$L559:
	andi	$4,$2,0x8000
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L677
	li	$4,65536			# 0x10000
	.set	macro
	.set	reorder

	lbu	$4,9($18)
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L642
	lui	$2,%hi(_tcpDosScanBitmap)
	.set	macro
	.set	reorder

$L562:
	lw	$2,%lo(item)($17)
$L692:
	li	$4,65536			# 0x10000
$L677:
	and	$5,$2,$4
	beq	$5,$0,$L563
	lbu	$5,9($18)
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$5,$2,$L643
	li	$2,2			# 0x2
	.set	macro
	.set	reorder

$L564:
	lw	$2,%lo(item)($17)
$L563:
	andi	$4,$2,0x800
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L678
	ext	$4,$2,18,1
	.set	macro
	.set	reorder

	lw	$4,12($18)
	lw	$2,16($18)
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L644
	lui	$2,%hi(LogFlag)
	.set	macro
	.set	reorder

$L567:
	lw	$2,%lo(item)($17)
	ext	$4,$2,18,1
$L678:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L679
	li	$4,131072			# 0x20000
	.set	macro
	.set	reorder

	lbu	$4,9($18)
	li	$2,17			# 0x11
	beq	$4,$2,$L645
$L569:
	lw	$2,%lo(item)($17)
$L691:
	li	$4,131072			# 0x20000
$L679:
	and	$5,$2,$4
	beq	$5,$0,$L571
	lbu	$5,9($18)
	li	$2,17			# 0x11
	beq	$5,$2,$L646
$L572:
	lw	$2,%lo(item)($17)
$L571:
	andi	$4,$2,0x4000
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L680
	andi	$4,$2,0x400
	.set	macro
	.set	reorder

	lhu	$2,6($18)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	andi	$4,$2,0x2000
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L574
	andi	$2,$2,0x1fff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L574
	sll	$2,$2,3
	.set	macro
	.set	reorder

	lw	$5,0($18)
	lhu	$4,2($18)
	andi	$5,$5,0xf
	sll	$5,$5,2
	subu	$2,$2,$5
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$4,$2,$4
	li	$2,65536			# 0x10000
	sltu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L681
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	ori	$3,$3,0x4000
	sw	$3,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+44)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+44)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+44)
	sw	$4,%lo(attack_daddr+44)($2)
$L574:
	lw	$2,%lo(item)($17)
$L681:
	andi	$4,$2,0x400
$L680:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L682
	li	$4,524288			# 0x80000
	.set	macro
	.set	reorder

	lw	$2,0($18)
	lbu	$4,9($18)
	andi	$5,$2,0xf
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L647
	sll	$5,$5,2
	.set	macro
	.set	reorder

$L576:
	lw	$2,%lo(item)($17)
$L696:
	li	$4,524288			# 0x80000
$L682:
	and	$5,$2,$4
	beq	$5,$0,$L577
	lbu	$5,9($18)
	li	$2,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$5,$2,$L648
	lui	$2,%hi(lan_addr)
	.set	macro
	.set	reorder

$L578:
	lw	$2,%lo(item)($17)
$L577:
	ext	$4,$2,24,1
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L683
	ext	$4,$2,25,1
	.set	macro
	.set	reorder

	lw	$2,0($18)
	lbu	$4,9($18)
	andi	$5,$2,0xf
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L649
	li	$4,8			# 0x8
	.set	macro
	.set	reorder

$L580:
	lw	$2,%lo(item)($17)
$L695:
	ext	$4,$2,25,1
$L683:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L684
	ext	$4,$2,26,1
	.set	macro
	.set	reorder

	lbu	$4,9($18)
	li	$2,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L650
	li	$4,1792			# 0x700
	.set	macro
	.set	reorder

$L584:
	lw	$2,%lo(item)($17)
$L694:
	ext	$4,$2,26,1
$L684:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L685
	li	$4,134217728			# 0x8000000
	.set	macro
	.set	reorder

	lbu	$4,9($18)
	li	$2,6			# 0x6
	beq	$4,$2,$L651
$L587:
	lw	$2,%lo(item)($17)
$L693:
	li	$4,134217728			# 0x8000000
$L685:
	and	$5,$2,$4
	beq	$5,$0,$L589
	lbu	$5,9($18)
	li	$2,6			# 0x6
	beq	$5,$2,$L652
$L590:
	lw	$2,%lo(item)($17)
$L589:
	ext	$4,$2,28,1
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L686
	andi	$4,$2,0x1000
	.set	macro
	.set	reorder

	lw	$2,12($18)
	li	$4,224			# 0xe0
	andi	$5,$2,0xf0
	.set	noreorder
	.set	nomacro
	beq	$5,$4,$L592
	li	$4,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	beq	$2,$4,$L592
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L687
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

$L592:
	lui	$2,%hi(LogFlag)
	lw	$4,%lo(LogFlag)($2)
	li	$3,268435456			# 0x10000000
	or	$3,$4,$3
	sw	$3,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+84)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+84)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+84)
	sw	$4,%lo(attack_daddr+84)($2)
	lw	$2,%lo(item)($17)
$L687:
	andi	$4,$2,0x1000
$L686:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L688
	andi	$2,$2,0x2000
	.set	macro
	.set	reorder

	lui	$2,%hi(lan_addr)
	lw	$5,%lo(lan_addr)($2)
	lui	$2,%hi(lan_mask)
	lw	$4,%lo(lan_mask)($2)
	lw	$2,12($18)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	xor	$2,$2,$5
	and	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L689
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lbu	$4,9($18)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L595
	lui	$2,%hi(LogFlag)
	.set	macro
	.set	reorder

	lw	$3,%lo(LogFlag)($2)
	ori	$3,$3,0x1000
	sw	$3,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+36)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+36)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+36)
	sw	$4,%lo(attack_daddr+36)($2)
$L595:
	lw	$2,%lo(item)($17)
$L689:
	andi	$2,$2,0x2000
$L688:
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L653
	lui	$2,%hi(ConnectedIp)
	.set	macro
	.set	reorder

$L596:
	beq	$3,$0,$L499
$L623:
	.set	noreorder
	.set	nomacro
	j	$L493
	move	$2,$0
	.set	macro
	.set	reorder

$L637:
	lw	$2,0($18)
	andi	$2,$2,0xf
	sll	$2,$2,2
	addu	$2,$18,$2
	lbu	$8,12($2)
	andi	$8,$8,0xf0
	sra	$8,$8,2
	sltu	$5,$8,21
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L536
	lui	$12,%hi($L544)
	.set	macro
	.set	reorder

	addiu	$2,$2,20
	addiu	$8,$8,-20
	addiu	$12,$12,%lo($L544)
	move	$5,$0
	li	$11,10			# 0xa
	li	$10,3			# 0x3
	li	$9,4			# 0x4
$L614:
	sltu	$6,$5,$8
	beq	$6,$0,$L536
	lbu	$6,0($2)
	sltu	$7,$6,9
	bne	$7,$0,$L654
$L538:
	lw	$3,12($18)
	lui	$2,%hi(attack_saddr+100)
	sw	$3,%lo(attack_saddr+100)($2)
	lw	$5,16($18)
	lui	$2,%hi(attack_daddr+100)
	li	$3,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	j	$L536
	sw	$5,%lo(attack_daddr+100)($2)
	.set	macro
	.set	reorder

$L630:
	lw	$4,28($3)
	bne	$2,$4,$L503
	sb	$0,0($3)
$L501:
	.set	noreorder
	.set	nomacro
	j	$L534
	move	$3,$0
	.set	macro
	.set	reorder

$L646:
	lhu	$2,6($18)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0x3fff
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L571
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lw	$2,0($18)
	lhu	$5,2($18)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $5

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xf
	sll	$2,$2,2
	andi	$5,$5,0xffff
	subu	$5,$5,$2
	lhu	$2,4($21)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	slt	$2,$2,$5
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L571
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	or	$4,$3,$4
	sw	$4,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+56)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+56)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+56)
	.set	noreorder
	.set	nomacro
	j	$L572
	sw	$4,%lo(attack_daddr+56)($2)
	.set	macro
	.set	reorder

$L645:
	lhu	$2,2($21)
	li	$6,1792			# 0x700
	.set	noreorder
	.set	nomacro
	beq	$2,$6,$L570
	li	$5,4352			# 0x1100
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$5,$L570
	li	$4,4864			# 0x1300
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L690
	lui	$2,%hi(LogFlag)
	.set	macro
	.set	reorder

	lhu	$2,0($21)
	beq	$2,$6,$L570
	beq	$2,$5,$L570
	.set	noreorder
	.set	nomacro
	bne	$2,$4,$L691
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

$L570:
	lui	$2,%hi(LogFlag)
$L690:
	lw	$4,%lo(LogFlag)($2)
	li	$3,262144			# 0x40000
	or	$3,$4,$3
	sw	$3,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+60)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+60)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+60)
	.set	noreorder
	.set	nomacro
	j	$L569
	sw	$4,%lo(attack_daddr+60)($2)
	.set	macro
	.set	reorder

$L643:
	lbu	$5,13($21)
	andi	$5,$5,0x3f
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L563
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lw	$6,0($18)
	lw	$2,12($21)
	lhu	$5,2($18)
	andi	$6,$6,0xf
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $5

 # 0 "" 2
 #NO_APP
	sll	$6,$6,2
	andi	$5,$5,0xffff
	ext	$2,$2,4,4
	subu	$5,$5,$6
	sll	$2,$2,2
	.set	noreorder
	.set	nomacro
	beq	$5,$2,$L565
	lui	$2,%hi(LogFlag)
	.set	macro
	.set	reorder

	lw	$3,%lo(LogFlag)($2)
$L701:
	or	$4,$3,$4
	sw	$4,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+52)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+52)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+52)
	.set	noreorder
	.set	nomacro
	j	$L564
	sw	$4,%lo(attack_daddr+52)($2)
	.set	macro
	.set	reorder

$L644:
	lw	$3,%lo(LogFlag)($2)
	ori	$3,$3,0x800
	sw	$3,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+32)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+32)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+32)
	.set	noreorder
	.set	nomacro
	j	$L567
	sw	$4,%lo(attack_daddr+32)($2)
	.set	macro
	.set	reorder

$L640:
	sll	$6,$6,2
	addu	$6,$18,$6
	lbu	$5,0($6)
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L558
	lui	$2,%hi(LogFlag)
	.set	macro
	.set	reorder

	lw	$3,%lo(LogFlag)($2)
	or	$4,$3,$4
	sw	$4,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+92)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+92)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+92)
	sw	$4,%lo(attack_daddr+92)($2)
	.set	noreorder
	.set	nomacro
	j	$L557
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

$L642:
	lbu	$4,13($21)
	addiu	$2,$2,%lo(_tcpDosScanBitmap)
	andi	$4,$4,0x3f
	addu	$2,$4,$2
	lb	$2,0($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L692
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	ori	$3,$3,0x8000
	sw	$3,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+48)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+48)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+48)
	.set	noreorder
	.set	nomacro
	j	$L562
	sw	$4,%lo(attack_daddr+48)($2)
	.set	macro
	.set	reorder

$L639:
	lw	$5,0($18)
	lhu	$4,2($18)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$5,$5,0xf
	andi	$4,$4,0xffff
$L606:
	sll	$5,$5,2
	subu	$4,$4,$5
	slt	$2,$4,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L552
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lui	$2,%hi(LogFlag)
	lw	$4,%lo(LogFlag)($2)
	li	$3,536870912			# 0x20000000
	or	$3,$4,$3
	sw	$3,%lo(LogFlag)($2)
$L619:
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+88)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+88)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+88)
	.set	noreorder
	.set	nomacro
	j	$L554
	sw	$4,%lo(attack_daddr+88)($2)
	.set	macro
	.set	reorder

$L653:
	lw	$4,12($18)
	addiu	$2,$2,%lo(ConnectedIp)
	andi	$5,$4,0xff
	sll	$5,$5,2
	addu	$2,$5,$2
	andi	$5,$4,0x3ff
	lw	$6,0($2)
	sll	$2,$5,5
	sll	$5,$5,3
	addu	$5,$5,$2
	lui	$2,%hi(dos_pkt)
	addiu	$2,$2,%lo(dos_pkt)
	.set	noreorder
	.set	nomacro
	beq	$4,$6,$L655
	addu	$2,$2,$5
	.set	macro
	.set	reorder

	lw	$5,28($2)
	beq	$4,$5,$L599
$L598:
	lb	$5,0($2)
	bne	$5,$0,$L596
	sw	$4,28($2)
	li	$4,1			# 0x1
	sb	$4,0($2)
	lui	$4,%hi(cur_p_pkt)
	sw	$2,%lo(cur_p_pkt)($4)
$L600:
	lhu	$4,32($2)
	lhu	$5,4($18)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$6, $5

 # 0 "" 2
 #NO_APP
	andi	$6,$6,0xffff
	beq	$6,$4,$L605
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	bne	$4,$0,$L596
$L605:
	lhu	$4,6($18)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	andi	$6,$4,0x3fff
	beq	$6,$0,$L596
	lhu	$6,34($2)
	sll	$7,$4,3
	andi	$7,$7,0xffff
	slt	$7,$7,$6
	bne	$7,$0,$L601
	andi	$4,$4,0x2000
	beq	$4,$0,$L656
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $5

 # 0 "" 2
 #NO_APP
	sh	$5,32($2)
	lw	$5,0($18)
	lhu	$4,2($18)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	ext	$5,$5,0,4
	andi	$5,$5,0x00ff
	sll	$5,$5,2
	subu	$6,$6,$5
	addu	$6,$6,$4
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L623
	sh	$6,34($2)
	.set	macro
	.set	reorder

	j	$L499
$L652:
	lbu	$2,13($21)
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L589
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	or	$4,$3,$4
	sw	$4,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+80)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+80)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+80)
	.set	noreorder
	.set	nomacro
	j	$L590
	sw	$4,%lo(attack_daddr+80)($2)
	.set	macro
	.set	reorder

$L651:
	lbu	$2,13($21)
	andi	$4,$2,0x17
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L588
	andi	$5,$2,0x3
	.set	macro
	.set	reorder

	li	$4,3			# 0x3
	.set	noreorder
	.set	nomacro
	beq	$5,$4,$L588
	andi	$2,$2,0x11
	.set	macro
	.set	reorder

	li	$4,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$4,$L693
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

$L588:
	lui	$2,%hi(LogFlag)
	lw	$4,%lo(LogFlag)($2)
	li	$3,67108864			# 0x4000000
	or	$3,$4,$3
	sw	$3,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+76)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+76)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+76)
	.set	noreorder
	.set	nomacro
	j	$L587
	sw	$4,%lo(attack_daddr+76)($2)
	.set	macro
	.set	reorder

$L650:
	lhu	$2,0($21)
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L585
	li	$4,4864			# 0x1300
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L585
	li	$4,34560			# 0x8700
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$4,$L694
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

$L585:
	lhu	$4,2($21)
	li	$2,34560			# 0x8700
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L694
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lui	$2,%hi(LogFlag)
	lw	$4,%lo(LogFlag)($2)
	li	$3,33554432			# 0x2000000
	or	$3,$4,$3
	sw	$3,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+72)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+72)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+72)
	.set	noreorder
	.set	nomacro
	j	$L584
	sw	$4,%lo(attack_daddr+72)($2)
	.set	macro
	.set	reorder

$L649:
	sll	$5,$5,2
	addu	$5,$18,$5
	lbu	$2,0($5)
	beq	$2,$4,$L581
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L695
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

$L581:
	lw	$2,12($18)
	li	$4,224			# 0xe0
	andi	$5,$2,0xf0
	.set	noreorder
	.set	nomacro
	beq	$5,$4,$L582
	li	$4,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	beq	$2,$4,$L582
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L695
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

$L582:
	lui	$2,%hi(LogFlag)
	lw	$4,%lo(LogFlag)($2)
	li	$3,16777216			# 0x1000000
	or	$3,$4,$3
	sw	$3,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+68)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+68)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+68)
	.set	noreorder
	.set	nomacro
	j	$L580
	sw	$4,%lo(attack_daddr+68)($2)
	.set	macro
	.set	reorder

$L647:
	addu	$5,$18,$5
	lbu	$4,0($5)
	li	$2,8			# 0x8
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L696
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lui	$2,%hi(lan_addr)
	lw	$5,%lo(lan_addr)($2)
	lui	$2,%hi(lan_mask)
	lw	$4,%lo(lan_mask)($2)
	lw	$2,12($18)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	xor	$2,$2,$5
	and	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L696
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	ori	$3,$3,0x400
	sw	$3,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+28)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+28)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+28)
	.set	noreorder
	.set	nomacro
	j	$L576
	sw	$4,%lo(attack_daddr+28)($2)
	.set	macro
	.set	reorder

$L648:
	lw	$6,%lo(lan_addr)($2)
	lui	$2,%hi(lan_mask)
	lw	$5,%lo(lan_mask)($2)
	lw	$2,12($18)
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2
	rotr	$2, $2, 16

 # 0 "" 2
 #NO_APP
	xor	$2,$2,$6
	and	$2,$2,$5
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L577
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	or	$4,$3,$4
	sw	$4,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+64)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+64)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+64)
	.set	noreorder
	.set	nomacro
	j	$L578
	sw	$4,%lo(attack_daddr+64)($2)
	.set	macro
	.set	reorder

$L633:
	lui	$4,%hi(block)
	lw	$8,%lo(block)($4)
	li	$7,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$8,$7,$L657
	lui	$7,%hi(attack_saddr+12)
	.set	macro
	.set	reorder

$L517:
	lw	$7,12($6)
	addiu	$7,$7,1
	sw	$7,12($6)
	lui	$6,%hi(per_udp_threshold)
	lw	$6,%lo(per_udp_threshold)($6)
	slt	$7,$6,$7
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L516
	andi	$2,$2,0x80
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L697
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	blez	$6,$L516
	lui	$2,%hi(LogFlag)
	.set	macro
	.set	reorder

	lw	$7,12($18)
	lui	$6,%hi(attack_saddr+12)
	sw	$7,%lo(attack_saddr+12)($6)
	lw	$8,16($18)
	lw	$6,%lo(LogFlag)($2)
	lui	$7,%hi(attack_daddr+12)
	sw	$8,%lo(attack_daddr+12)($7)
	ori	$6,$6,0x80
	li	$7,1			# 0x1
	sw	$7,%lo(block)($4)
	.set	noreorder
	.set	nomacro
	j	$L516
	sw	$6,%lo(LogFlag)($2)
	.set	macro
	.set	reorder

$L634:
	lui	$4,%hi(block)
	lw	$8,%lo(block)($4)
	.set	noreorder
	.set	nomacro
	beq	$8,$7,$L658
	lui	$7,%hi(attack_saddr+16)
	.set	macro
	.set	reorder

$L520:
	lw	$7,16($6)
	addiu	$7,$7,1
	sw	$7,16($6)
	lui	$6,%hi(per_icmp_threshold)
	lw	$6,%lo(per_icmp_threshold)($6)
	slt	$7,$6,$7
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L519
	andi	$2,$2,0x100
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L698
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	blez	$6,$L519
	lui	$2,%hi(LogFlag)
	.set	macro
	.set	reorder

	lw	$7,12($18)
	lui	$6,%hi(attack_saddr+16)
	sw	$7,%lo(attack_saddr+16)($6)
	lw	$8,16($18)
	lw	$6,%lo(LogFlag)($2)
	lui	$7,%hi(attack_daddr+16)
	sw	$8,%lo(attack_daddr+16)($7)
	ori	$6,$6,0x100
	li	$7,1			# 0x1
	sw	$7,%lo(block)($4)
	.set	noreorder
	.set	nomacro
	j	$L519
	sw	$6,%lo(LogFlag)($2)
	.set	macro
	.set	reorder

$L632:
	lbu	$7,13($21)
	li	$4,1			# 0x1
	andi	$7,$7,0x3f
	.set	noreorder
	.set	nomacro
	bne	$7,$4,$L513
	lui	$4,%hi(block)
	.set	macro
	.set	reorder

	lw	$8,%lo(block)($4)
	.set	noreorder
	.set	nomacro
	beq	$8,$7,$L659
	lui	$7,%hi(attack_saddr+8)
	.set	macro
	.set	reorder

$L514:
	lw	$7,8($6)
	addiu	$7,$7,1
	sw	$7,8($6)
	lui	$6,%hi(per_fin_threshold)
	lw	$6,%lo(per_fin_threshold)($6)
	slt	$7,$6,$7
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L513
	andi	$2,$2,0x40
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L699
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	blez	$6,$L513
	lui	$2,%hi(LogFlag)
	.set	macro
	.set	reorder

	lw	$7,12($18)
	lui	$6,%hi(attack_saddr+8)
	sw	$7,%lo(attack_saddr+8)($6)
	lw	$8,16($18)
	lw	$6,%lo(LogFlag)($2)
	lui	$7,%hi(attack_daddr+8)
	sw	$8,%lo(attack_daddr+8)($7)
	ori	$6,$6,0x40
	li	$7,1			# 0x1
	sw	$7,%lo(block)($4)
	.set	noreorder
	.set	nomacro
	j	$L513
	sw	$6,%lo(LogFlag)($2)
	.set	macro
	.set	reorder

$L635:
	sll	$4,$4,2
	addu	$4,$18,$4
	lbu	$4,0($4)
	li	$6,8			# 0x8
	beq	$4,$6,$L523
	bne	$4,$0,$L522
$L523:
	lui	$4,%hi(block)
	lw	$6,%lo(block)($4)
	li	$4,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$6,$4,$L660
	lui	$4,%hi(attack_saddr+20)
	.set	macro
	.set	reorder

$L524:
	lw	$4,20($2)
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	j	$L522
	sw	$4,20($2)
	.set	macro
	.set	reorder

$L631:
	lbu	$4,13($21)
	li	$3,2			# 0x2
	andi	$4,$4,0x3f
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L508
	move	$3,$0
	.set	macro
	.set	reorder

	lui	$4,%hi(block)
	lw	$6,%lo(block)($4)
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$6,$3,$L661
	lui	$3,%hi(attack_saddr+4)
	.set	macro
	.set	reorder

$L507:
	lw	$6,4($7)
	lui	$3,%hi(per_syn_threshold)
	addiu	$6,$6,1
	sw	$6,4($7)
	lw	$3,%lo(per_syn_threshold)($3)
	slt	$6,$3,$6
	beq	$6,$0,$L615
	andi	$2,$2,0x20
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L700
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	blez	$3,$L700
	lw	$6,12($18)
	lui	$3,%hi(attack_saddr+4)
	lui	$2,%hi(LogFlag)
	sw	$6,%lo(attack_saddr+4)($3)
	lw	$7,16($18)
	lw	$3,%lo(LogFlag)($2)
	lui	$6,%hi(attack_daddr+4)
	sw	$7,%lo(attack_daddr+4)($6)
	ori	$3,$3,0x20
	li	$6,1			# 0x1
	sw	$6,%lo(block)($4)
	sw	$3,%lo(LogFlag)($2)
$L615:
	lw	$2,%lo(item)($17)
$L700:
	.set	noreorder
	.set	nomacro
	j	$L508
	move	$3,$0
	.set	macro
	.set	reorder

$L628:
	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)
	lw	$2,%lo(fast_path_cusRspTCPFinAck_func)($2)
	bne	$2,$0,$L622
	.set	noreorder
	.set	nomacro
	j	$L493
	move	$2,$0
	.set	macro
	.set	reorder

$L641:
	lw	$2,0($18)
	lbu	$4,9($18)
	andi	$5,$2,0xf
	li	$2,1			# 0x1
	beq	$4,$2,$L662
$L560:
	.set	noreorder
	.set	nomacro
	j	$L559
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

$L662:
	sll	$5,$5,2
	addu	$5,$18,$5
	lbu	$2,0($5)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L559
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lui	$2,%hi(icmp_echo_request)
	lw	$2,%lo(icmp_echo_request)($2)
	bne	$2,$0,$L560
	lui	$2,%hi(LogFlag)
	lw	$4,%lo(LogFlag)($2)
	li	$3,-2147483648			# 0xffffffff80000000
	or	$3,$4,$3
	sw	$3,%lo(LogFlag)($2)
	lw	$4,12($18)
	lui	$2,%hi(attack_saddr+96)
	li	$3,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+96)($2)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+96)
	sw	$4,%lo(attack_daddr+96)($2)
	.set	noreorder
	.set	nomacro
	j	$L559
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

$L550:
	lhu	$2,2($21)
	li	$4,1792			# 0x700
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L551
	li	$6,4352			# 0x1100
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$6,$L551
	li	$5,4864			# 0x1300
	.set	macro
	.set	reorder

	beq	$2,$5,$L551
	lhu	$2,0($21)
	beq	$2,$4,$L551
	beq	$2,$6,$L551
	.set	noreorder
	.set	nomacro
	bne	$2,$5,$L676
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

$L551:
	lw	$2,12($18)
	lw	$4,16($18)
	bne	$2,$4,$L549
	lui	$3,%hi(attack_saddr+104)
	sw	$2,%lo(attack_saddr+104)($3)
	lw	$4,16($18)
	lui	$2,%hi(attack_daddr+104)
	li	$3,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	j	$L549
	sw	$4,%lo(attack_daddr+104)($2)
	.set	macro
	.set	reorder

$L654:
	sll	$6,$6,2
	addu	$6,$12,$6
	lw	$6,0($6)
	j	$6
	.rdata
	.align	2
	.align	2
$L544:
	.word	$L540
	.word	$L540
	.word	$L541
	.word	$L542
	.word	$L538
	.word	$L538
	.word	$L538
	.word	$L538
	.word	$L543
	.text
$L540:
	addiu	$2,$2,1
	.set	noreorder
	.set	nomacro
	j	$L614
	addiu	$5,$5,1
	.set	macro
	.set	reorder

$L543:
	lbu	$6,1($2)
	.set	noreorder
	.set	nomacro
	bne	$6,$11,$L538
	addiu	$2,$2,10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L614
	addiu	$5,$5,10
	.set	macro
	.set	reorder

$L542:
	lbu	$6,1($2)
	.set	noreorder
	.set	nomacro
	bne	$6,$10,$L538
	addiu	$2,$2,3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L614
	addiu	$5,$5,3
	.set	macro
	.set	reorder

$L541:
	lbu	$6,1($2)
	.set	noreorder
	.set	nomacro
	bne	$6,$9,$L538
	addiu	$2,$2,4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L614
	addiu	$5,$5,4
	.set	macro
	.set	reorder

$L638:
	lhu	$2,6($18)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0x3fff
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L552
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lw	$2,0($18)
	lhu	$5,2($18)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $5

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xf
	andi	$5,$5,0xffff
	sll	$2,$2,2
	subu	$2,$5,$2
	slt	$2,$2,4
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L552
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	or	$4,$3,$4
	.set	noreorder
	.set	nomacro
	j	$L619
	sw	$4,%lo(LogFlag)($2)
	.set	macro
	.set	reorder

$L555:
	lw	$5,0($18)
	lhu	$4,2($18)
	li	$2,8			# 0x8
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4

 # 0 "" 2
 #NO_APP
	andi	$5,$5,0xf
	.set	noreorder
	.set	nomacro
	j	$L606
	andi	$4,$4,0xffff
	.set	macro
	.set	reorder

$L655:
	lw	$5,28($2)
	bne	$4,$5,$L598
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L623
	sb	$0,0($2)
	.set	macro
	.set	reorder

	j	$L499
$L504:
	lb	$4,0($3)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L501
	lui	$5,%hi(cur_p_pkt)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L505
	sw	$3,%lo(cur_p_pkt)($5)
	.set	macro
	.set	reorder

$L532:
	slt	$4,$4,201
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L533
	lui	$4,%hi(LogFlag)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L525
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

$L599:
	lb	$5,0($2)
	li	$4,1			# 0x1
	bne	$5,$4,$L596
	lui	$4,%hi(cur_p_pkt)
	.set	noreorder
	.set	nomacro
	j	$L600
	sw	$2,%lo(cur_p_pkt)($4)
	.set	macro
	.set	reorder

$L656:
	sh	$0,34($2)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L623
	sh	$0,32($2)
	.set	macro
	.set	reorder

	j	$L499
$L533:
	lw	$5,%lo(LogFlag)($4)
	.set	noreorder
	.set	nomacro
	j	$L617
	lw	$2,%lo(item)($17)
	.set	macro
	.set	reorder

$L636:
	lhu	$6,36($2)
	beq	$6,$0,$L530
	lhu	$5,2($21)
	beq	$6,$5,$L531
	addiu	$4,$4,1
	sw	$4,24($2)
	move	$5,$6
$L531:
	bne	$5,$0,$L529
$L530:
	lhu	$5,2($21)
	.set	noreorder
	.set	nomacro
	j	$L529
	sh	$5,36($2)
	.set	macro
	.set	reorder

$L611:
	lhu	$6,36($2)
	bne	$6,$0,$L612
	lw	$4,24($2)
$L527:
	lhu	$5,2($21)
$L665:
	sh	$5,36($2)
	.set	noreorder
	.set	nomacro
	j	$L526
	lbu	$6,9($18)
	.set	macro
	.set	reorder

$L612:
	lhu	$5,2($21)
	beq	$6,$5,$L663
	lw	$4,24($2)
	move	$5,$6
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L527
	sw	$4,24($2)
	.set	macro
	.set	reorder

$L664:
	.set	noreorder
	.set	nomacro
	j	$L526
	lbu	$6,9($18)
	.set	macro
	.set	reorder

$L663:
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L664
	lw	$4,24($2)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L665
	lhu	$5,2($21)
	.set	macro
	.set	reorder

$L660:
	lw	$6,%lo(attack_saddr+20)($4)
	lw	$4,12($18)
	bne	$6,$4,$L524
	.set	noreorder
	.set	nomacro
	j	$L522
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L657:
	lw	$8,%lo(attack_saddr+12)($7)
	lw	$7,12($18)
	bne	$8,$7,$L517
	.set	noreorder
	.set	nomacro
	j	$L516
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L658:
	lw	$8,%lo(attack_saddr+16)($7)
	lw	$7,12($18)
	bne	$8,$7,$L520
	.set	noreorder
	.set	nomacro
	j	$L519
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L565:
	lhu	$2,6($18)
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0x2000
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L564
	lui	$2,%hi(LogFlag)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L701
	lw	$3,%lo(LogFlag)($2)
	.set	macro
	.set	reorder

$L661:
	lw	$8,%lo(attack_saddr+4)($3)
	lw	$6,12($18)
	.set	noreorder
	.set	nomacro
	bne	$8,$6,$L507
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L667
	andi	$4,$2,0x44
	.set	macro
	.set	reorder

$L601:
	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	ori	$3,$3,0x2000
	sw	$3,%lo(LogFlag)($2)
	lw	$3,12($18)
	lui	$2,%hi(attack_saddr+40)
	sw	$3,%lo(attack_saddr+40)($2)
	lw	$3,16($18)
	lui	$2,%hi(attack_daddr+40)
	sw	$3,%lo(attack_daddr+40)($2)
	.set	noreorder
	.set	nomacro
	j	$L493
	move	$2,$0
	.set	macro
	.set	reorder

$L659:
	lw	$8,%lo(attack_saddr+8)($7)
	lw	$7,12($18)
	bne	$8,$7,$L514
	.set	noreorder
	.set	nomacro
	j	$L513
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	.end	filter_enter
	.size	filter_enter, .-filter_enter
	.section	.rodata.str1.4
	.align	2
$LC39:
	.ascii	"type=%d num=%d\012\000"
	.align	2
$LC40:
	.ascii	"index=%d\011\000"
	.align	2
$LC41:
	.ascii	"mac=%02x:%02x:%02x:%02x:%02x:%02x\012\000"
	.align	2
$LC42:
	.ascii	"index=%d\011\011\000"
	.align	2
$LC43:
	.ascii	"url=%s\012\000"
	.align	2
$LC44:
	.ascii	"start=%x\011\000"
	.align	2
$LC45:
	.ascii	"end=%x\011\000"
	.align	2
$LC46:
	.ascii	"day=%x\012\000"
	.align	2
$LC47:
	.ascii	"unknow table type\012\000"
	.align	2
$LC48:
	.ascii	"Mode:%d %s\012\000"
	.text
	.align	2
	.set	nomips16
	.ent	filter_table_read_proc
	.type	filter_table_read_proc, @function
filter_table_read_proc:
	.frame	$sp,80,$31		# vars= 8, regs= 10/0, args= 32, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$fp,72($sp)
	sw	$23,68($sp)
	sw	$22,64($sp)
	sw	$21,60($sp)
	sw	$20,56($sp)
	sw	$19,52($sp)
	sw	$18,48($sp)
	sw	$17,44($sp)
	sw	$16,40($sp)
	lui	$16,%hi(kmalloc_caches)
	addiu	$16,$16,%lo(kmalloc_caches)
	sw	$4,80($sp)
	lw	$4,36($16)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	beq	$2,$0,$L703
	move	$18,$2

	move	$4,$2
	move	$5,$0
	jal	memset
	li	$6,512			# 0x200

	lw	$4,32($16)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	beq	$2,$0,$L721
	move	$16,$2

	move	$4,$2
	move	$5,$0
	jal	memset
	li	$6,256			# 0x100

	lui	$2,%hi(table_list_head)
	lw	$19,%lo(table_list_head)($2)
	addiu	$2,$2,%lo(table_list_head)
	beq	$19,$2,$L705
	lui	$20,%hi($LC40)

	lui	$fp,%hi($LC45)
	lui	$23,%hi($LC46)
	lui	$22,%hi($LC41)
	addiu	$20,$20,%lo($LC40)
	addiu	$fp,$fp,%lo($LC45)
	addiu	$23,$23,%lo($LC46)
	j	$L714
	addiu	$22,$22,%lo($LC41)

$L723:
	beq	$2,$3,$L709
	li	$3,2			# 0x2

	beq	$2,$3,$L722
	lui	$5,%hi($LC47)

	move	$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC47)

	move	$4,$18
	jal	strcat
	move	$5,$16

$L710:
	lw	$19,0($19)
$L724:
	lui	$2,%hi(table_list_head)
	addiu	$2,$2,%lo(table_list_head)
	beq	$19,$2,$L726
	lui	$2,%hi(url_filter_mode)

$L714:
	lw	$7,24($19)
	lw	$6,16($19)
	lui	$5,%hi($LC39)
	move	$4,$18
	addiu	$7,$7,-1
	jal	sprintf
	addiu	$5,$5,%lo($LC39)

	lw	$2,16($19)
	li	$3,3			# 0x3
	bne	$2,$3,$L723
	li	$3,4			# 0x4

	lw	$17,8($19)
	addiu	$21,$19,8
	beq	$17,$21,$L710
	nop

$L712:
	lw	$6,20($17)
	lui	$5,%hi($LC42)
	move	$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC42)

	move	$4,$18
	jal	strcat
	move	$5,$16

	lui	$5,%hi($LC43)
	addiu	$6,$17,28
	move	$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC43)

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$17,0($17)
	bne	$17,$21,$L712
	lui	$2,%hi(table_list_head)

	lw	$19,0($19)
	addiu	$2,$2,%lo(table_list_head)
	bne	$19,$2,$L714
	nop

$L705:
	lui	$2,%hi(url_filter_mode)
$L726:
	lw	$6,%lo(url_filter_mode)($2)
	lw	$4,80($sp)
	lui	$5,%hi($LC48)
	addiu	$5,$5,%lo($LC48)
	jal	seq_printf
	move	$7,$18

	jal	kfree
	move	$4,$18

	jal	kfree
	move	$4,$16

$L703:
	lw	$31,76($sp)
$L725:
	lw	$fp,72($sp)
	lw	$23,68($sp)
	lw	$22,64($sp)
	lw	$21,60($sp)
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,80

$L722:
	lw	$17,8($19)
	addiu	$21,$19,8
	beq	$17,$21,$L710
	nop

$L711:
	lw	$6,20($17)
	move	$4,$16
	jal	sprintf
	move	$5,$20

	move	$4,$18
	jal	strcat
	move	$5,$16

	lbu	$2,30($17)
	lbu	$6,28($17)
	lbu	$7,29($17)
	sw	$2,16($sp)
	lbu	$2,31($17)
	move	$4,$16
	move	$5,$22
	sw	$2,20($sp)
	lbu	$2,32($17)
	sw	$2,24($sp)
	lbu	$2,33($17)
	jal	sprintf
	sw	$2,28($sp)

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$17,0($17)
	bne	$17,$21,$L711
	nop

	j	$L724
	lw	$19,0($19)

$L709:
	lw	$17,8($19)
	addiu	$2,$19,8
	beq	$17,$2,$L710
	sw	$2,32($sp)

$L713:
	lw	$6,20($17)
	move	$4,$16
	jal	sprintf
	move	$5,$20

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$6,28($17)
	lui	$5,%hi($LC44)
	move	$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC44)

	addiu	$21,$17,28
	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$6,4($21)
	move	$4,$16
	jal	sprintf
	move	$5,$fp

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$6,8($21)
	move	$4,$16
	jal	sprintf
	move	$5,$23

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$17,0($17)
	lw	$3,32($sp)
	bne	$17,$3,$L713
	nop

	j	$L724
	lw	$19,0($19)

$L721:
	jal	kfree
	move	$4,$18

	j	$L725
	lw	$31,76($sp)

	.set	macro
	.set	reorder
	.end	filter_table_read_proc
	.size	filter_table_read_proc, .-filter_table_read_proc
	.align	2
	.globl	filter_item_regist
	.set	nomips16
	.ent	filter_item_regist
	.type	filter_item_regist, @function
filter_item_regist:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$3,%hi(table_list_head)
	lw	$2,%lo(table_list_head)($3)
	addiu	$3,$3,%lo(table_list_head)
	bne	$2,$3,$L733
	nop

	j	$L732
	li	$2,-1			# 0xffffffffffffffff

$L735:
	lw	$2,0($2)
	beq	$2,$3,$L728
	nop

$L733:
	lw	$6,16($2)
	bne	$6,$4,$L735
	addiu	$6,$2,8

	lw	$3,8($2)
	sw	$5,4($3)
	sw	$3,0($5)
	sw	$6,4($5)
	lw	$3,24($2)
	sw	$5,8($2)
	addiu	$3,$3,1
	sw	$3,24($2)
	li	$2,3			# 0x3
	beq	$4,$2,$L738
	li	$2,2			# 0x2

	lw	$31,20($sp)
$L739:
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

$L728:
	li	$2,-1			# 0xffffffffffffffff
$L732:
	lw	$31,20($sp)
	j	$31
	addiu	$sp,$sp,24

$L738:
	bne	$3,$2,$L739
	lw	$31,20($sp)

	jal	Add_Pattern_ACL_For_ContentFilter
	nop

	j	$L732
	move	$2,$0

	.set	macro
	.set	reorder
	.end	filter_item_regist
	.size	filter_item_regist, .-filter_item_regist
	.align	2
	.globl	filter_table_flush
	.set	nomips16
	.ent	filter_table_flush
	.type	filter_table_flush, @function
filter_table_flush:
	.frame	$sp,56,$31		# vars= 0, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

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
	lui	$20,%hi(table_list_head)
	lw	$16,%lo(table_list_head)($20)
	addiu	$20,$20,%lo(table_list_head)
	beq	$16,$20,$L741
	lw	$19,0($16)

	li	$18,1048576			# 0x100000
	li	$17,2097152			# 0x200000
	ori	$18,$18,0x100
	ori	$17,$17,0x200
	li	$21,3			# 0x3
	li	$22,1			# 0x1
$L745:
	lw	$2,8($16)
	addiu	$fp,$16,8
	beq	$2,$fp,$L742
	lw	$23,0($2)

	j	$L743
	move	$3,$23

$L747:
	move	$23,$3
$L743:
	lw	$5,4($2)
	move	$4,$2
	sw	$5,4($3)
	sw	$3,0($5)
	sw	$18,0($2)
	jal	kfree
	sw	$17,4($2)

	lw	$3,24($16)
	move	$2,$23
	addiu	$3,$3,-1
	sw	$3,24($16)
	bne	$23,$fp,$L747
	lw	$3,0($23)

$L742:
	lw	$2,16($16)
	beq	$2,$21,$L748
	nop

$L744:
	lw	$2,4($16)
	lw	$3,0($16)
	move	$4,$16
	sw	$2,4($3)
	sw	$3,0($2)
	sw	$18,0($16)
	jal	kfree
	sw	$17,4($16)

	lw	$2,24($20)
	addiu	$2,$2,-1
	sw	$2,24($20)
	beq	$19,$20,$L741
	lw	$2,0($19)

$L749:
	move	$16,$19
	j	$L745
	move	$19,$2

$L748:
	lw	$2,24($16)
	bne	$2,$22,$L744
	nop

	jal	Del_Pattern_ACL_For_ContentFilter
	nop

	lw	$2,4($16)
	lw	$3,0($16)
	move	$4,$16
	sw	$2,4($3)
	sw	$3,0($2)
	sw	$18,0($16)
	jal	kfree
	sw	$17,4($16)

	lw	$2,24($20)
	addiu	$2,$2,-1
	sw	$2,24($20)
	bne	$19,$20,$L749
	lw	$2,0($19)

$L741:
	li	$3,1			# 0x1
	lui	$2,%hi(filter_rule_num)
	sw	$3,%lo(filter_rule_num)($2)
	lw	$31,52($sp)
	lui	$2,%hi(log_enable)
	lw	$fp,48($sp)
	lw	$23,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	sw	$0,%lo(log_enable)($2)
	lui	$2,%hi(url_filter_mode)
	sw	$0,%lo(url_filter_mode)($2)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	filter_table_flush
	.size	filter_table_flush, .-filter_table_flush
	.section	.rodata.str1.4
	.align	2
$LC49:
	.ascii	"init\000"
	.align	2
$LC50:
	.ascii	"flush\000"
	.align	2
$LC51:
	.ascii	"enable_log\000"
	.align	2
$LC52:
	.ascii	"white\000"
	.align	2
$LC53:
	.ascii	"black\000"
	.align	2
$LC54:
	.ascii	"add:\000"
	.align	2
$LC55:
	.ascii	"#\000"
	.align	2
$LC56:
	.ascii	";\000"
	.text
	.align	2
	.set	nomips16
	.ent	filter_table_single_write
	.type	filter_table_single_write, @function
filter_table_single_write:
	.frame	$sp,72,$31		# vars= 16, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$fp,64($sp)
	sw	$23,60($sp)
	sw	$22,56($sp)
	sw	$21,52($sp)
	sw	$20,48($sp)
	sw	$19,44($sp)
	sw	$18,40($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	lui	$20,%hi(kmalloc_caches)
	addiu	$20,$20,%lo(kmalloc_caches)
	lw	$4,40($20)
	move	$21,$5
	li	$5,208			# 0xd0
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	move	$16,$6
	.set	macro
	.set	reorder

	move	$17,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L752
	li	$19,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	move	$5,$0
	li	$6,1024			# 0x400
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	memset
	lui	$18,%hi(get_info_from_usr_space)
	.set	macro
	.set	reorder

	addiu	$4,$18,%lo(get_info_from_usr_space)
	.set	noreorder
	.set	nomacro
	jal	strlen
	sw	$17,20($sp)
	.set	macro
	.set	reorder

	move	$6,$2
	addiu	$4,$18,%lo(get_info_from_usr_space)
	.set	noreorder
	.set	nomacro
	jal	memset
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L753
	or	$3,$21,$16
	.set	macro
	.set	reorder

	addu	$2,$21,$16
	or	$2,$3,$2
	lw	$3,24($28)
	move	$19,$16
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L755
	move	$6,$16
	.set	macro
	.set	reorder

	move	$5,$21
	addiu	$4,$18,%lo(get_info_from_usr_space)
 #APP
 # 710 "net/rtl/fastpath/filter_v2.c" 1
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
$L755:
	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L791
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L785
	addiu	$3,$18,%lo(get_info_from_usr_space)
	.set	macro
	.set	reorder

$L756:
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$18,%lo(get_info_from_usr_space)
	.set	macro
	.set	reorder

	addu	$2,$17,$2
	sb	$0,-1($2)
	lw	$18,20($sp)
	lui	$16,%hi($LC49)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC49)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC49)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L757
	lw	$18,20($sp)
	.set	macro
	.set	reorder

	addiu	$16,$sp,20
	lui	$18,%hi($LC33)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$18,%lo($LC33)
	.set	macro
	.set	reorder

	lw	$2,20($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L753
	lui	$20,%hi(filter_tlb)
	.set	macro
	.set	reorder

	addiu	$20,$20,%lo(filter_tlb)
	addiu	$18,$18,%lo($LC33)
	move	$5,$18
$L792:
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	sll	$3,$2,3
	addu	$3,$20,$3
	lw	$5,4($3)
	.set	noreorder
	.set	nomacro
	jal	filter_table_regist
	move	$4,$2
	.set	macro
	.set	reorder

	lw	$2,20($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L792
	move	$5,$18
	.set	macro
	.set	reorder

$L759:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

$L752:
	move	$2,$19
$L790:
	lw	$31,68($sp)
	lw	$fp,64($sp)
	lw	$23,60($sp)
	lw	$22,56($sp)
	lw	$21,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,72
	.set	macro
	.set	reorder

$L757:
	lui	$16,%hi($LC50)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC50)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC50)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L786
	lw	$18,20($sp)
	.set	macro
	.set	reorder

	lui	$16,%hi($LC51)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC51)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC51)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L761
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(log_enable)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	kfree
	sw	$3,%lo(log_enable)($2)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L790
	move	$2,$19
	.set	macro
	.set	reorder

$L785:
	move	$2,$17
 #APP
 # 52 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($3)
	subu	$16,1
	sb	$1,($2)
	beqz	$1,2f
	addiu	$2,1
	bnez	$16,1b
	addiu	$3,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	j	$L756
$L786:
	.set	noreorder
	.set	nomacro
	jal	filter_table_flush
	move	$4,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L790
	move	$2,$19
	.set	macro
	.set	reorder

$L753:
	move	$4,$17
$L791:
	.set	noreorder
	.set	nomacro
	jal	kfree
	li	$19,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	move	$2,$19
	lw	$31,68($sp)
	lw	$fp,64($sp)
	lw	$23,60($sp)
	lw	$22,56($sp)
	lw	$21,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,72
	.set	macro
	.set	reorder

$L761:
	lw	$18,20($sp)
	lui	$16,%hi($LC52)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC52)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC52)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L762
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(url_filter_mode)
	.set	noreorder
	.set	nomacro
	j	$L759
	sw	$3,%lo(url_filter_mode)($2)
	.set	macro
	.set	reorder

$L762:
	lw	$18,20($sp)
	lui	$16,%hi($LC53)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC53)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC53)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L763
	lui	$2,%hi(url_filter_mode)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L759
	sw	$0,%lo(url_filter_mode)($2)
	.set	macro
	.set	reorder

$L763:
	lw	$18,20($sp)
	lui	$16,%hi($LC54)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC54)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC54)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L759
	lw	$18,20($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC54)
	.set	macro
	.set	reorder

	addu	$2,$18,$2
	sw	$2,20($sp)
	lb	$2,0($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L753
	addiu	$16,$sp,20
	.set	macro
	.set	reorder

	lui	$5,%hi($LC55)
	addiu	$5,$5,%lo($LC55)
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,20($sp)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L791
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	li	$3,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L753
	lui	$18,%hi($LC33)
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$18,%lo($LC33)
	.set	macro
	.set	reorder

	lw	$3,20($sp)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L791
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	li	$21,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$21,$L753
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$18,%lo($LC33)
	.set	macro
	.set	reorder

	lw	$3,20($sp)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L791
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$21,$L753
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	addiu	$22,$18,%lo($LC33)
	li	$fp,92			# 0x5c
	addiu	$18,$sp,16
	li	$23,32			# 0x20
	lui	$5,%hi($LC56)
$L793:
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$5,%lo($LC56)
	.set	macro
	.set	reorder

	move	$4,$2
	sw	$2,16($sp)
$L787:
	.set	noreorder
	.set	nomacro
	jal	check_esc_end
	li	$5,92			# 0x5c
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L765
	lw	$21,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$4,$21
	.set	macro
	.set	reorder

	addu	$2,$21,$2
	li	$3,59			# 0x3b
	lui	$5,%hi($LC56)
	sb	$3,0($2)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$5,%lo($LC56)
	.set	macro
	.set	reorder

	lw	$2,20($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L787
	lw	$4,16($sp)
	.set	macro
	.set	reorder

$L765:
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$5,$22
	.set	macro
	.set	reorder

	move	$21,$2
	lw	$2,16($sp)
	bne	$2,$0,$L784
	.set	noreorder
	.set	nomacro
	j	$L791
	move	$4,$17
	.set	macro
	.set	reorder

$L768:
	jal	strlen
	addu	$2,$21,$2
	sb	$23,0($2)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$5,$22
	.set	macro
	.set	reorder

$L784:
	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	check_esc_end
	li	$5,92			# 0x5c
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L768
	move	$4,$21
	.set	macro
	.set	reorder

	lb	$2,0($21)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L769
	move	$6,$21
	.set	macro
	.set	reorder

	move	$7,$0
$L773:
	beq	$2,$fp,$L788
$L770:
	lb	$2,1($6)
	addiu	$7,$7,1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L773
	addiu	$6,$6,1
	.set	macro
	.set	reorder

$L769:
	lw	$4,28($20)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L759
	move	$5,$2
	.set	macro
	.set	reorder

	addiu	$2,$2,8
	sw	$2,12($5)
	sw	$5,0($5)
	sw	$5,4($5)
	sw	$2,8($5)
	sw	$0,24($5)
	lui	$4,%hi(filter_rule_num)
	lw	$2,%lo(filter_rule_num)($4)
	lw	$3,24($sp)
	move	$4,$21
	sw	$2,20($5)
	sw	$3,16($5)
	.set	noreorder
	.set	nomacro
	jal	strlen
	sw	$5,28($sp)
	.set	macro
	.set	reorder

	sltu	$2,$2,41
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L789
	lw	$5,28($sp)
	.set	macro
	.set	reorder

	addiu	$4,$5,28
	li	$2,39			# 0x27
	li	$3,39			# 0x27
$L778:
 #APP
 # 52 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($21)
	subu	$2,1
	sb	$1,($4)
	beqz	$1,2f
	addiu	$4,1
	bnez	$2,1b
	addiu	$21,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
$L776:
	addu	$3,$5,$3
	sb	$0,28($3)
	.set	noreorder
	.set	nomacro
	jal	filter_item_regist
	li	$4,3			# 0x3
	.set	macro
	.set	reorder

	lui	$4,%hi(filter_rule_num)
	lw	$3,%lo(filter_rule_num)($4)
	lw	$2,20($sp)
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L759
	sw	$3,%lo(filter_rule_num)($4)
	.set	macro
	.set	reorder

	lb	$2,0($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L793
	lui	$5,%hi($LC56)
	.set	macro
	.set	reorder

	j	$L759
$L788:
	lb	$2,1($6)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L771
	move	$5,$7
	.set	macro
	.set	reorder

	move	$4,$6
	move	$3,$7
$L772:
	addu	$5,$21,$5
	sb	$2,0($5)
	lb	$2,2($4)
	addiu	$3,$3,1
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L772
	move	$5,$3
	.set	macro
	.set	reorder

$L771:
	addu	$5,$21,$5
	.set	noreorder
	.set	nomacro
	j	$L770
	sb	$0,0($5)
	.set	macro
	.set	reorder

$L789:
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$4,$21
	.set	macro
	.set	reorder

	lw	$5,28($sp)
	move	$3,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L776
	addiu	$4,$5,28
	.set	macro
	.set	reorder

	j	$L778
	.end	filter_table_single_write
	.size	filter_table_single_write, .-filter_table_single_write
	.align	2
	.globl	rtl_mac_filter_fastpath
	.set	nomips16
	.ent	rtl_mac_filter_fastpath
	.type	rtl_mac_filter_fastpath, @function
rtl_mac_filter_fastpath:
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
	move	$17,$0
	move	$16,$4
	move	$19,$5
	li	$20,6			# 0x6
	addu	$2,$19,$17
$L801:
	move	$4,$16
	jal	rtl_skb_mac_header
	lbu	$18,0($2)

	addu	$2,$2,$17
	lbu	$2,6($2)
	bne	$18,$2,$L800
	addiu	$17,$17,1

	bne	$17,$20,$L801
	addu	$2,$19,$17

	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	li	$2,1			# 0x1
	j	$31
	addiu	$sp,$sp,40

$L800:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	rtl_mac_filter_fastpath
	.size	rtl_mac_filter_fastpath, .-rtl_mac_filter_fastpath
	.align	2
	.set	nomips16
	.ent	T.1546
	.type	T.1546, @function
T.1546:
	.frame	$sp,64,$31		# vars= 8, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

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
	lw	$18,80($sp)
	lw	$19,84($sp)
	subu	$2,$5,$7
	move	$17,$4
	sw	$0,0($19)
	move	$16,$5
	sw	$6,72($sp)
	move	$22,$7
	sw	$0,0($18)
	move	$20,$4
	sw	$2,16($sp)
	move	$21,$0
	move	$23,$0
	li	$fp,13			# 0xd
$L809:
	lb	$2,0($20)
	beq	$2,$fp,$L817
	li	$3,10			# 0xa

	beq	$2,$3,$L818
	andi	$2,$21,0x1

$L804:
	move	$21,$0
	lw	$5,72($sp)
$L819:
	move	$4,$20
	jal	memcmp
	move	$6,$22

	beq	$2,$0,$L808
	lw	$3,16($sp)

	addiu	$23,$23,1
	sltu	$2,$3,$23
	beq	$2,$0,$L809
	addiu	$20,$20,1

$L806:
	lw	$31,60($sp)
$L820:
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
	j	$31
	addiu	$sp,$sp,64

$L808:
	addu	$22,$23,$22
	sw	$22,0($18)
	addu	$17,$17,$22
	lb	$4,0($17)
	li	$3,13			# 0xd
	beq	$4,$3,$L811
	move	$2,$0

	sltu	$2,$16,$22
	bne	$2,$0,$L820
	lw	$31,60($sp)

	move	$2,$0
	j	$L812
	li	$5,13			# 0xd

$L813:
	bne	$4,$0,$L806
	addiu	$17,$17,1

$L812:
	lb	$3,1($17)
	addiu	$2,$2,1
	addu	$4,$2,$22
	bne	$3,$5,$L813
	sltu	$4,$16,$4

$L811:
	sw	$2,0($19)
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
	li	$2,1			# 0x1
	j	$31
	addiu	$sp,$sp,64

$L817:
	andi	$2,$21,0x1
	bne	$2,$0,$L804
	addiu	$21,$21,1

	slt	$2,$21,4
	beq	$2,$0,$L820
	lw	$31,60($sp)

	j	$L819
	lw	$5,72($sp)

$L818:
	beq	$2,$0,$L804
	addiu	$21,$21,1

	slt	$2,$21,4
	beq	$2,$0,$L820
	lw	$31,60($sp)

	j	$L819
	lw	$5,72($sp)

	.set	macro
	.set	reorder
	.end	T.1546
	.size	T.1546, .-T.1546
	.section	.rodata.str1.4
	.align	2
$LC57:
	.ascii	"GET \000"
	.align	2
$LC58:
	.ascii	"Host: \000"
	.text
	.align	2
	.globl	rtl_url_filter_fastpath
	.set	nomips16
	.ent	rtl_url_filter_fastpath
	.type	rtl_url_filter_fastpath, @function
rtl_url_filter_fastpath:
	.frame	$sp,72,$31		# vars= 16, regs= 8/0, args= 24, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$22,64($sp)
	sw	$21,60($sp)
	sw	$20,56($sp)
	sw	$19,52($sp)
	sw	$18,48($sp)
	sw	$17,44($sp)
	sw	$16,40($sp)
	move	$17,$4
	move	$4,$5
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$16,$5
	.set	macro
	.set	reorder

	sltu	$2,$2,2
	bne	$2,$0,$L822
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,9($2)
	move	$20,$2
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L838
	lui	$2,%hi(kmalloc_caches+44)
	.set	macro
	.set	reorder

$L822:
	move	$16,$0
$L824:
	move	$2,$16
	lw	$31,68($sp)
	lw	$22,64($sp)
	lw	$21,60($sp)
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,72
	.set	macro
	.set	reorder

$L838:
	lw	$19,0($20)
	lw	$4,%lo(kmalloc_caches+44)($2)
	andi	$19,$19,0xf
	sll	$19,$19,2
	addu	$19,$20,$19
	lw	$18,12($19)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	ext	$18,$18,4,4
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L822
	move	$17,$2
	.set	macro
	.set	reorder

	lw	$22,0($20)
	sll	$18,$18,2
	addu	$18,$19,$18
	lui	$19,%hi($LC57)
	move	$4,$18
	addiu	$5,$19,%lo($LC57)
	li	$6,4			# 0x4
	lhu	$20,2($20)
	andi	$22,$22,0xf
 #APP
 # 20 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$20, $20

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	jal	memcmp
	andi	$20,$20,0xffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L828
	sll	$21,$22,3
	.set	macro
	.set	reorder

	subu	$21,$20,$21
	lui	$6,%hi($LC58)
	addiu	$20,$sp,24
	addiu	$2,$sp,28
	addiu	$6,$6,%lo($LC58)
	move	$4,$18
	move	$5,$21
	li	$7,6			# 0x6
	sw	$2,20($sp)
	.set	noreorder
	.set	nomacro
	jal	T.1546
	sw	$20,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L828
	lw	$3,24($sp)
	.set	macro
	.set	reorder

	lw	$4,28($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L827
	move	$2,$0
	.set	macro
	.set	reorder

	addu	$3,$18,$3
	move	$2,$17
 #APP
 # 52 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($3)
	subu	$4,1
	sb	$1,($2)
	beqz	$1,2f
	addiu	$2,1
	bnez	$4,1b
	addiu	$3,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	lw	$2,28($sp)
$L827:
	addu	$2,$17,$2
	sb	$0,0($2)
	move	$5,$21
	addiu	$2,$sp,32
	addiu	$6,$19,%lo($LC57)
	move	$4,$18
	li	$7,4			# 0x4
	sw	$20,16($sp)
	.set	noreorder
	.set	nomacro
	jal	T.1546
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L828
	lw	$2,32($sp)
	.set	macro
	.set	reorder

	addiu	$2,$2,-9
	.set	noreorder
	.set	nomacro
	blez	$2,$L828
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$3,28($sp)
	lw	$4,24($sp)
	addu	$3,$17,$3
	addu	$18,$18,$4
 #APP
 # 52 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($18)
	subu	$2,1
	sb	$1,($3)
	beqz	$1,2f
	addiu	$3,1
	bnez	$2,1b
	addiu	$18,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	lw	$2,32($sp)
	lw	$3,28($sp)
	move	$4,$17
	addu	$2,$17,$2
	addu	$3,$2,$3
	.set	noreorder
	.set	nomacro
	jal	strlen
	sb	$0,0($3)
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$19,$2
	.set	macro
	.set	reorder

	sltu	$19,$19,$2
	.set	noreorder
	.set	nomacro
	bne	$19,$0,$L831
	move	$18,$2
	.set	macro
	.set	reorder

	addu	$20,$17,$2
	lb	$2,0($20)
	beq	$2,$0,$L831
	move	$4,$0
$L834:
	addu	$4,$17,$4
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L832
	addiu	$19,$19,1
	.set	macro
	.set	reorder

	li	$16,1			# 0x1
$L833:
	lui	$6,%hi($LC26)
	lui	$4,%hi(log_info)
	addiu	$6,$6,%lo($LC26)
	addiu	$4,$4,%lo(log_info)
	li	$5,32			# 0x20
	.set	noreorder
	.set	nomacro
	jal	snprintf
	move	$7,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	lui	$2,%hi(url_filter_mode)
	lw	$2,%lo(url_filter_mode)($2)
	li	$3,1			# 0x1
	bne	$2,$3,$L824
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L835
	lui	$3,%hi(url_filter_flag)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L824
	li	$16,1			# 0x1
	.set	macro
	.set	reorder

$L828:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L824
	move	$16,$0
	.set	macro
	.set	reorder

$L832:
	lb	$2,1($20)
	move	$4,$19
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L834
	addiu	$20,$20,1
	.set	macro
	.set	reorder

$L831:
	.set	noreorder
	.set	nomacro
	j	$L833
	move	$16,$0
	.set	macro
	.set	reorder

$L835:
	.set	noreorder
	.set	nomacro
	j	$L824
	sw	$2,%lo(url_filter_flag)($3)
	.set	macro
	.set	reorder

	.end	rtl_url_filter_fastpath
	.size	rtl_url_filter_fastpath, .-rtl_url_filter_fastpath
	.globl	filter_tlb
	.data
	.align	2
	.type	filter_tlb, @object
	.size	filter_tlb, 40
filter_tlb:
	.word	0
	.word	0
	.word	1
	.word	rtl_ip_range_filter_fastpath
	.word	2
	.word	rtl_mac_filter_fastpath
	.word	3
	.word	rtl_url_filter_fastpath
	.word	4
	.word	rtl_schedule_filter_fastpath
	.globl	filter_table_proc_fops
	.align	2
	.type	filter_table_proc_fops, @object
	.size	filter_table_proc_fops, 108
filter_table_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	filter_table_single_write
	.space	28
	.word	filter_table_single_open
	.space	4
	.word	single_release
	.space	52
	.globl	dos_proc_fops
	.align	2
	.type	dos_proc_fops, @object
	.size	dos_proc_fops, 108
dos_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	dos_single_write
	.space	28
	.word	dos_single_open
	.space	4
	.word	single_release
	.space	52
	.globl	fast_path_cusRsp401_func
	.section	.bss,"aw",@nobits
	.align	2
	.type	fast_path_cusRsp401_func, @object
	.size	fast_path_cusRsp401_func, 4
fast_path_cusRsp401_func:
	.space	4
	.globl	fast_path_cusRspTCPFinAck_func
	.align	2
	.type	fast_path_cusRspTCPFinAck_func, @object
	.size	fast_path_cusRspTCPFinAck_func, 4
fast_path_cusRspTCPFinAck_func:
	.space	4
	.globl	fast_path_cusRspTCPEndAck_func
	.align	2
	.type	fast_path_cusRspTCPEndAck_func, @object
	.size	fast_path_cusRspTCPEndAck_func, 4
fast_path_cusRspTCPEndAck_func:
	.space	4
	.section	.rodata.str1.4
	.align	2
$LC59:
	.ascii	"NULL\000"
	.align	2
$LC60:
	.ascii	"PerSynFlood\000"
	.align	2
$LC61:
	.ascii	"PerFinFlood\000"
	.align	2
$LC62:
	.ascii	"PerUdpFlood\000"
	.align	2
$LC63:
	.ascii	"PerIcmpFlood\000"
	.align	2
$LC64:
	.ascii	"PerPingFlood\000"
	.align	2
$LC65:
	.ascii	"TcpUdpPortScan\000"
	.align	2
$LC66:
	.ascii	"IcmpSmurf\000"
	.align	2
$LC67:
	.ascii	"IpLand\000"
	.align	2
$LC68:
	.ascii	"IpSpoof\000"
	.align	2
$LC69:
	.ascii	"TearDrop\000"
	.align	2
$LC70:
	.ascii	"PingOfDeath\000"
	.align	2
$LC71:
	.ascii	"TcpScan\000"
	.align	2
$LC72:
	.ascii	"TcpSynWithData\000"
	.align	2
$LC73:
	.ascii	"UdpBomb\000"
	.align	2
$LC74:
	.ascii	"UdpEchoChargen\000"
	.align	2
$LC75:
	.ascii	"UdpFraggle\000"
	.align	2
$LC76:
	.ascii	"IcmpEchoStorm\000"
	.align	2
$LC77:
	.ascii	"UdpSnorkAttack\000"
	.align	2
$LC78:
	.ascii	"TcpInvalidFlags\000"
	.align	2
$LC79:
	.ascii	"TcpRstAttack\000"
	.align	2
$LC80:
	.ascii	"BcMcSrcIpAttack\000"
	.align	2
$LC81:
	.ascii	"ShortIpPkt\000"
	.align	2
$LC82:
	.ascii	"IcmpPingSweep\000"
	.align	2
$LC83:
	.ascii	"IcmpEchoWithoutReply\000"
	.align	2
$LC84:
	.ascii	"TcpInvalidOptions\000"
	.align	2
$LC85:
	.ascii	"LoopBackEchoChargen\000"
	.align	2
$LC86:
	.ascii	"MaxDosType\000"
	.rdata
	.align	2
	.type	rtl_dos_name, @object
	.size	rtl_dos_name, 112
rtl_dos_name:
	.word	$LC59
	.word	$LC60
	.word	$LC61
	.word	$LC62
	.word	$LC63
	.word	$LC64
	.word	$LC65
	.word	$LC66
	.word	$LC67
	.word	$LC68
	.word	$LC69
	.word	$LC70
	.word	$LC71
	.word	$LC72
	.word	$LC73
	.word	$LC74
	.word	$LC75
	.word	$LC76
	.word	$LC77
	.word	$LC78
	.word	$LC79
	.word	$LC80
	.word	$LC81
	.word	$LC82
	.word	$LC83
	.word	$LC84
	.word	$LC85
	.word	$LC86
	.local	item
	.comm	item,4,4
	.local	item_0
	.comm	item_0,4,4
	.local	item_backup
	.comm	item_backup,4,4
	.local	block_time
	.comm	block_time,4,4
	.local	log_enable
	.comm	log_enable,4,4
	.local	url_filter_mode
	.comm	url_filter_mode,4,4
	.local	res3
	.comm	res3,4,4
	.local	none_dos_drop_pkt_cnt
	.comm	none_dos_drop_pkt_cnt,4,4
	.local	url_count
	.comm	url_count,1,1
	.local	block
	.comm	block,4,4
	.local	block_count
	.comm	block_count,4,4
	.local	icmp_echo_request_time
	.comm	icmp_echo_request_time,4,4
	.local	icmp_echo_request
	.comm	icmp_echo_request,4,4
	.local	url_cache
	.comm	url_cache,20,4
	.local	endStateClientPort
	.comm	endStateClientPort,2,2
	.local	endStateClientSeq
	.comm	endStateClientSeq,4,4
	.local	finStateClientPort
	.comm	finStateClientPort,2,2
	.local	finStateClientSeq
	.comm	finStateClientSeq,4,4
	.local	block_message
	.comm	block_message,4,4
	.local	res12
	.comm	res12,4,4
	.local	url_filter_flag
	.comm	url_filter_flag,4,4
	.local	get_info_from_usr_space
	.comm	get_info_from_usr_space,1024,4
	.local	log_info
	.comm	log_info,32,4
	.local	filter_rule_num
	.comm	filter_rule_num,4,4
	.local	LogFlag
	.comm	LogFlag,4,4
	.local	ConnectedIp
	.comm	ConnectedIp,1024,4
	.local	dos_pkt
	.comm	dos_pkt,40960,4
	.local	cur_p_pkt
	.comm	cur_p_pkt,4,4
	.local	dos_timer
	.comm	dos_timer,28,4
	.local	_tcpDosScanBitmap
	.comm	_tcpDosScanBitmap,64,4
	.local	op_mode
	.comm	op_mode,1,1
	.local	whole_syn_threshold
	.comm	whole_syn_threshold,4,4
	.local	whole_fin_threshold
	.comm	whole_fin_threshold,4,4
	.local	whole_udp_threshold
	.comm	whole_udp_threshold,4,4
	.local	whole_icmp_threshold
	.comm	whole_icmp_threshold,4,4
	.local	per_syn_threshold
	.comm	per_syn_threshold,4,4
	.local	per_fin_threshold
	.comm	per_fin_threshold,4,4
	.local	per_udp_threshold
	.comm	per_udp_threshold,4,4
	.local	per_icmp_threshold
	.comm	per_icmp_threshold,4,4
	.local	lan_addr
	.comm	lan_addr,4,4
	.local	lan_mask
	.comm	lan_mask,4,4
	.local	attack_saddr
	.comm	attack_saddr,108,4
	.local	attack_daddr
	.comm	attack_daddr,108,4
	.local	dos_flag
	.comm	dos_flag,80,4
	.local	ip_name0
	.comm	ip_name0,16,4
	.local	ip_name1
	.comm	ip_name1,16,4
	.globl	table_list_head
	.section	.bss
	.align	2
	.type	table_list_head, @object
	.size	table_list_head, 32
table_list_head:
	.space	32
	.globl	wan_dev
	.align	2
	.type	wan_dev, @object
	.size	wan_dev, 4
wan_dev:
	.space	4
	.globl	wan_ppp_dev
	.align	2
	.type	wan_ppp_dev, @object
	.size	wan_ppp_dev, 4
wan_ppp_dev:
	.space	4
	.globl	___strtok
	.align	2
	.type	___strtok, @object
	.size	___strtok, 4
___strtok:
	.space	4
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
