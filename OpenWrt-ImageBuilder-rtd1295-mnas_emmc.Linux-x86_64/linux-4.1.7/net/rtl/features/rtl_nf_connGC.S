/*
 * Copyright (C) 2017 Realtek Semiconductor Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

	.file	1 "rtl_nf_connGC.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.text
	.align	2
	.globl	rtl_nf_conn_GC_init
	.set	nomips16
	.ent	rtl_nf_conn_GC_init
	.type	rtl_nf_conn_GC_init, @function
rtl_nf_conn_GC_init:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$6,%hi(Tcp_State_Hash_Head)
	addiu	$6,$6,%lo(Tcp_State_Hash_Head)
	move	$4,$0
	move	$3,$0
	li	$5,11			# 0xb
$L3:
	sll	$4,$4,3
	addu	$4,$6,$4
	lw	$2,4($4)
	addiu	$3,$3,1
	beq	$2,$0,$L2
	move	$4,$3

	sw	$2,4($2)
	sw	$2,0($2)
$L2:
	bne	$3,$5,$L3
	nop

	lui	$3,%hi(Udp_State_Hash_Head)
	addiu	$3,$3,%lo(Udp_State_Hash_Head)
	lw	$2,4($3)
	beq	$2,$0,$L4
	nop

	sw	$2,4($2)
	sw	$2,0($2)
$L4:
	lw	$2,12($3)
	beq	$2,$0,$L18
	lui	$16,%hi(prot_limit)

	sw	$2,4($2)
	sw	$2,0($2)
$L18:
	addiu	$4,$16,%lo(prot_limit)
	li	$6,12			# 0xc
	jal	memset
	move	$5,$0

	lui	$7,%hi(prot_counters)
	lui	$4,%hi(_prot_limit)
	addiu	$7,$7,%lo(prot_counters)
	addiu	$4,$4,%lo(_prot_limit)
	move	$2,$0
	li	$6,12			# 0xc
$L6:
	addu	$5,$7,$2
	addu	$3,$4,$2
	addiu	$2,$2,4
	sw	$0,0($5)
	bne	$2,$6,$L6
	sw	$0,0($3)

	li	$3,10			# 0xa
	lui	$2,%hi(drop_priority_max_idx)
	lui	$10,%hi(nf_conntrack_max)
	sw	$3,%lo(drop_priority_max_idx)($2)
	lw	$3,%lo(nf_conntrack_max)($10)
	li	$7,2			# 0x2
	li	$11,1374355456			# 0x51eb0000
	mul	$7,$7,$3
	ori	$11,$11,0x851f
	li	$2,2			# 0x2
	addiu	$5,$16,%lo(prot_limit)
	sw	$2,%lo(prot_limit)($16)
	li	$2,90			# 0x5a
	sw	$2,4($5)
	li	$2,60			# 0x3c
	sw	$2,8($5)
	move	$2,$0
	addu	$6,$4,$2
	lui	$17,%hi(rtl_newGC_session_status_flags)
	addiu	$2,$2,4
	li	$9,12			# 0xc
	sw	$0,%lo(rtl_newGC_session_status_flags)($17)
	addu	$8,$5,$2
	multu	$7,$11
	mfhi	$7
	srl	$7,$7,5
	beq	$2,$9,$L7
	sw	$7,0($6)

$L16:
	lw	$7,0($8)
	lw	$3,%lo(nf_conntrack_max)($10)
	addu	$6,$4,$2
	addiu	$2,$2,4
	mul	$7,$7,$3
	addu	$8,$5,$2
	multu	$7,$11
	mfhi	$7
	srl	$7,$7,5
	bne	$2,$9,$L16
	sw	$7,0($6)

$L7:
	li	$2,-859045888			# 0xffffffffcccc0000
	sll	$4,$3,2
	ori	$2,$2,0xcccd
	multu	$4,$2
	lui	$4,%hi(rtl_nf_conntrack_threshold)
	mfhi	$2
	srl	$2,$2,2
	subu	$5,$3,$2
	sltu	$5,$5,65
	bne	$5,$0,$L9
	sw	$2,%lo(rtl_nf_conntrack_threshold)($4)

	addiu	$2,$3,-64
	sw	$2,%lo(rtl_nf_conntrack_threshold)($4)
$L9:
	bne	$3,$0,$L17
	nop

$L10:
	sw	$0,%lo(rtl_newGC_session_status_flags)($17)
$L11:
	jal	rtl_nf_conn_GC_init_hooks
	nop

	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

$L17:
	jal	rtl_gc_threshold_check
	move	$4,$0

	bne	$2,$0,$L10
	lw	$3,%lo(rtl_newGC_session_status_flags)($17)

	li	$2,3			# 0x3
	beq	$3,$2,$L11
	lui	$3,%hi(jiffies)

	lw	$3,%lo(jiffies)($3)
	lui	$4,%hi(rtl_newGC_session_status_flags)
	sw	$2,%lo(rtl_newGC_session_status_flags)($4)
	addiu	$3,$3,1
	lui	$2,%hi(rtl_newGC_session_status_time)
	j	$L11
	sw	$3,%lo(rtl_newGC_session_status_time)($2)

	.set	macro
	.set	reorder
	.end	rtl_nf_conn_GC_init
	.size	rtl_nf_conn_GC_init, .-rtl_nf_conn_GC_init
	.align	2
	.globl	__conntrack_drop_check
	.set	nomips16
	.ent	__conntrack_drop_check
	.type	__conntrack_drop_check, @function
__conntrack_drop_check:
	.frame	$sp,56,$31		# vars= 0, regs= 9/0, args= 16, gp= 0
	.mask	0x80ff0000,-4
	.fmask	0x00000000,0
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
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_protonum
	move	$16,$4
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_port_by_dir
	move	$6,$0
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$0
	li	$6,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_port_by_dir
	move	$19,$2
	.set	macro
	.set	reorder

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_port_by_dir
	move	$21,$2
	.set	macro
	.set	reorder

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_port_by_dir
	move	$23,$2
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$0
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_ip_by_dir
	move	$22,$2
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$0
	li	$6,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_ip_by_dir
	move	$18,$2
	.set	macro
	.set	reorder

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_ip_by_dir
	move	$17,$2
	.set	macro
	.set	reorder

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_ip_by_dir
	move	$20,$2
	.set	macro
	.set	reorder

	li	$4,-268435456			# 0xfffffffff0000000
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $17
	rotr	$5, $5, 16

 # 0 "" 2
 #NO_APP
	li	$3,-536870912			# 0xffffffffe0000000
	and	$5,$5,$4
	beq	$5,$3,$L20
 #APP
 # 31 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $2
	rotr	$5, $5, 16

 # 0 "" 2
 #NO_APP
	and	$4,$5,$4
	beq	$4,$3,$L20
	.set	noreorder
	.set	nomacro
	beq	$18,$2,$L24
	sltu	$2,$19,4
	.set	macro
	.set	reorder

$L26:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L25
	sltu	$2,$21,4
	.set	macro
	.set	reorder

$L20:
	li	$2,-1			# 0xffffffffffffffff
$L22:
	lw	$31,52($sp)
	lw	$23,48($sp)
	lw	$22,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,56
	.set	macro
	.set	reorder

$L24:
	beq	$17,$20,$L20
	j	$L26
$L25:
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L22
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	sltu	$2,$23,4
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L22
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	sltu	$2,$22,4
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L20
	li	$3,36895			# 0x901f
	.set	macro
	.set	reorder

	beq	$19,$3,$L20
	beq	$21,$3,$L20
	.set	noreorder
	.set	nomacro
	beq	$23,$3,$L22
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$22,$3,$L22
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L22
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	.end	__conntrack_drop_check
	.size	__conntrack_drop_check, .-__conntrack_drop_check
	.align	2
	.globl	__nf_ct_refresh_acct_proto
	.set	nomips16
	.ent	__nf_ct_refresh_acct_proto
	.type	__nf_ct_refresh_acct_proto, @function
__nf_ct_refresh_acct_proto:
	.frame	$sp,56,$31		# vars= 8, regs= 6/0, args= 24, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$20,48($sp)
	sw	$19,44($sp)
	sw	$18,40($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	move	$16,$4
	move	$18,$5
	move	$17,$6
	move	$19,$7
	sw	$0,24($sp)
	.set	noreorder
	.set	nomacro
	jal	local_bh_disable
	lbu	$20,76($sp)
	.set	macro
	.set	reorder

	li	$5,10			# 0xa
	.set	noreorder
	.set	nomacro
	jal	rtl_test_bit
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L36
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_test_bit
	li	$5,3			# 0x3
	.set	macro
	.set	reorder

	li	$3,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L37
	move	$5,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L38
	lui	$2,%hi(jiffies)
	.set	macro
	.set	reorder

$L31:
	lw	$7,72($sp)
$L39:
	addiu	$2,$sp,24
	move	$4,$16
	move	$5,$18
	move	$6,$17
	.set	noreorder
	.set	nomacro
	jal	__nf_ct_refresh_acct_proto_hooks
	sw	$2,16($sp)
	.set	macro
	.set	reorder

$L36:
	jal	local_bh_enable
	lw	$31,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,56
	.set	macro
	.set	reorder

$L37:
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_set_ct_timeout_expires
	move	$4,$16
	.set	macro
	.set	reorder

	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	j	$L31
	sw	$2,24($sp)
	.set	macro
	.set	reorder

$L38:
	lw	$5,%lo(jiffies)($2)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_set_ct_timeout_expires
	addu	$5,$19,$5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_add_ct_timer
	move	$4,$16
	.set	macro
	.set	reorder

	li	$2,2048			# 0x800
	sw	$2,24($sp)
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$20,$2,$L32
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$20,$2,$L39
	lw	$7,72($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_udp_status
	move	$4,$16
	.set	macro
	.set	reorder

	andi	$2,$2,0x2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L34
	move	$4,$16
	.set	macro
	.set	reorder

	li	$5,2			# 0x2
	.set	noreorder
	.set	nomacro
	jal	rtl_list_move_tail
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L39
	lw	$7,72($sp)
	.set	macro
	.set	reorder

$L32:
	lw	$6,84($sp)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_list_move_tail
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L39
	lw	$7,72($sp)
	.set	macro
	.set	reorder

$L34:
	li	$5,2			# 0x2
	.set	noreorder
	.set	nomacro
	jal	rtl_list_move_tail
	move	$6,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L39
	lw	$7,72($sp)
	.set	macro
	.set	reorder

	.end	__nf_ct_refresh_acct_proto
	.size	__nf_ct_refresh_acct_proto, .-__nf_ct_refresh_acct_proto
	.align	2
	.globl	rtl_connGC_addList
	.set	nomips16
	.ent	rtl_connGC_addList
	.type	rtl_connGC_addList, @function
rtl_connGC_addList:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	move	$17,$5
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_ip_hdr
	move	$16,$4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L61
	lw	$31,28($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_skb_protocol
	move	$4,$16
	.set	macro
	.set	reorder

	li	$3,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L43
	li	$3,17			# 0x11
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L44
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L60
	lw	$31,28($sp)
	.set	macro
	.set	reorder

$L61:
	lw	$17,24($sp)
	lw	$16,20($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L44:
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_udp_status
	move	$4,$17
	.set	macro
	.set	reorder

	andi	$2,$2,0x2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L46
	move	$4,$17
	.set	macro
	.set	reorder

	li	$5,2			# 0x2
	.set	noreorder
	.set	nomacro
	jal	rtl_list_add_tail
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(prot_counters)
	addiu	$2,$2,%lo(prot_counters)
$L52:
 #APP
 # 68 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, 8($2)		# atomic_add
	addu	$3, 1
	sc	$3, 8($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L52
	lw	$31,28($sp)
	.set	macro
	.set	reorder

	lw	$17,24($sp)
	lw	$16,20($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L60:
	lui	$2,%hi(prot_counters)
$L42:
 #APP
 # 68 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, %lo(prot_counters)($2)		# atomic_add
	addu	$3, 1
	sc	$3, %lo(prot_counters)($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L42
	lw	$31,28($sp)
	.set	macro
	.set	reorder

	lw	$17,24($sp)
	lw	$16,20($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L43:
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_tcp_state
	move	$4,$17
	.set	macro
	.set	reorder

	lui	$3,%hi(Tcp_State_Hash_Head)
	sll	$2,$2,3
	addiu	$3,$3,%lo(Tcp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$2,4($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L57
	move	$4,$17
	.set	macro
	.set	reorder

	li	$5,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	rtl_list_add_tail
	move	$6,$0
	.set	macro
	.set	reorder

$L57:
	lui	$2,%hi(prot_counters)
	addiu	$2,$2,%lo(prot_counters)
$L51:
 #APP
 # 68 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, 4($2)		# atomic_add
	addu	$3, 1
	sc	$3, 4($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L51
	lw	$31,28($sp)
	.set	macro
	.set	reorder

	lw	$17,24($sp)
	lw	$16,20($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L46:
	li	$5,2			# 0x2
	.set	noreorder
	.set	nomacro
	jal	rtl_list_add_tail
	move	$6,$0
	.set	macro
	.set	reorder

	lui	$2,%hi(prot_counters)
	.set	noreorder
	.set	nomacro
	j	$L52
	addiu	$2,$2,%lo(prot_counters)
	.set	macro
	.set	reorder

	.end	rtl_connGC_addList
	.size	rtl_connGC_addList, .-rtl_connGC_addList
	.align	2
	.globl	rtl_death_action
	.set	nomips16
	.ent	rtl_death_action
	.type	rtl_death_action, @function
rtl_death_action:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	li	$3,-1			# 0xffffffffffffffff
	lw	$2,100($4)
	move	$16,$4
	addiu	$2,$4,100
$L63:
 #APP
 # 93 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/bitops.h" 1
		ll	$4, 0($2)		# set_bit
	ins	$4, $3, 9, 1
	sc	$4, 0($2)

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L63
	.set	noreorder
	.set	nomacro
	jal	nf_ct_delete_from_lists
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L70
	lw	$31,20($sp)
	.set	macro
	.set	reorder

 #APP
 # 185 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
$L65:
 #APP
 # 207 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, 0($16)	# atomic_sub_return
	subu	$2, $3, 1
	sc	$2, 0($16)
	.set	mips0

 # 0 "" 2
 #NO_APP
	beq	$2,$0,$L65
 #APP
 # 228 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1


 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L69
	lw	$31,20($sp)
	.set	macro
	.set	reorder

$L70:
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,24
	.set	macro
	.set	reorder

$L69:
	move	$4,$16
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	nf_conntrack_destroy
	addiu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	rtl_death_action
	.size	rtl_death_action, .-rtl_death_action
	.align	2
	.globl	drop_one_conntrack
	.set	nomips16
	.ent	drop_one_conntrack
	.type	drop_one_conntrack, @function
drop_one_conntrack:
	.frame	$sp,72,$31		# vars= 8, regs= 10/0, args= 24, gp= 0
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
	lui	$2,%hi(_prot_limit+4)
	lw	$3,%lo(_prot_limit+4)($2)
	lui	$2,%hi(prot_counters+4)
	lw	$2,%lo(prot_counters+4)($2)
	slt	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L126
	lui	$23,%hi(drop_priority_max_idx)
	.set	macro
	.set	reorder

	lui	$2,%hi(_prot_limit+8)
$L152:
	lw	$3,%lo(_prot_limit+8)($2)
	lui	$2,%hi(prot_counters+8)
	lw	$2,%lo(prot_counters+8)($2)
	slt	$2,$3,$2
	beq	$2,$0,$L82
	jal	local_bh_disable
	lw	$2,%lo(drop_priority_max_idx)($23)
	blez	$2,$L83
	lui	$20,%hi(drop_priority)
	lui	$22,%hi(Udp_State_Hash_Head)
	addiu	$20,$20,%lo(drop_priority)
	addiu	$22,$22,%lo(Udp_State_Hash_Head)
	move	$21,$0
	li	$19,-1			# 0xffffffffffffffff
$L89:
	lbu	$2,0($20)
	sltu	$3,$2,11
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L84
	addiu	$2,$2,-11
	.set	macro
	.set	reorder

	sll	$2,$2,3
	addu	$2,$22,$2
	lw	$18,4($2)
	lw	$17,0($18)
	.set	noreorder
	.set	nomacro
	beq	$18,$17,$L153
	lw	$2,%lo(drop_priority_max_idx)($23)
	.set	macro
	.set	reorder

	lw	$16,0($17)
	addiu	$17,$17,-140
	lb	$2,148($17)
	beq	$2,$19,$L133
$L85:
	beq	$2,$0,$L134
$L86:
	move	$17,$16
	.set	noreorder
	.set	nomacro
	beq	$18,$16,$L84
	lw	$2,0($16)
	.set	macro
	.set	reorder

	addiu	$17,$17,-140
	move	$16,$2
	lb	$2,148($17)
	bne	$2,$19,$L85
$L133:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L86
	sb	$2,148($17)
	.set	macro
	.set	reorder

$L134:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$0,$L86
$L130:
	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	jal	rtl_death_action
	move	$4,$17
	.set	macro
	.set	reorder

	lui	$2,%hi(rtl_newGC_session_status_flags)
	lw	$3,%lo(rtl_newGC_session_status_flags)($2)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L78
	lui	$3,%hi(jiffies)
	.set	macro
	.set	reorder

	lw	$3,%lo(jiffies)($3)
	.set	noreorder
	.set	nomacro
	j	$L150
	li	$4,3			# 0x3
	.set	macro
	.set	reorder

$L84:
	lw	$2,%lo(drop_priority_max_idx)($23)
$L153:
	addiu	$21,$21,1
	slt	$2,$21,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L89
	addiu	$20,$20,4
	.set	macro
	.set	reorder

$L83:
	jal	local_bh_enable
$L82:
	jal	local_bh_disable
	lw	$2,%lo(drop_priority_max_idx)($23)
	blez	$2,$L90
	lui	$19,%hi(drop_priority)
	addiu	$19,$19,%lo(drop_priority)
	move	$22,$0
	li	$18,-1			# 0xffffffffffffffff
	lui	$21,%hi(jiffies)
	li	$fp,2			# 0x2
$L104:
	lbu	$2,0($19)
	sltu	$3,$2,10
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L91
	lui	$3,%hi(Udp_State_Hash_Head)
	.set	macro
	.set	reorder

	lui	$3,%hi(Tcp_State_Hash_Head)
	sll	$2,$2,3
	addiu	$3,$3,%lo(Tcp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$2,4($2)
	sw	$2,24($sp)
	lw	$17,0($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$17,$L92
	move	$3,$0
	.set	macro
	.set	reorder

	lw	$16,0($17)
	addiu	$17,$17,-140
	lb	$2,148($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$18,$L135
	addiu	$3,$3,1
	.set	macro
	.set	reorder

$L93:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L154
	move	$4,$17
	.set	macro
	.set	reorder

$L94:
	lw	$4,24($sp)
$L155:
	.set	noreorder
	.set	nomacro
	beq	$4,$16,$L92
	lw	$2,0($16)
	.set	macro
	.set	reorder

	move	$17,$16
	addiu	$17,$17,-140
	move	$16,$2
	lb	$2,148($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$18,$L93
	addiu	$3,$3,1
	.set	macro
	.set	reorder

$L135:
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	sw	$3,28($sp)
	.set	macro
	.set	reorder

	sb	$2,148($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L94
	lw	$3,28($sp)
	.set	macro
	.set	reorder

	move	$4,$17
$L154:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_timer_expires
	sw	$3,28($sp)
	.set	macro
	.set	reorder

	lw	$5,%lo(jiffies)($21)
	lhu	$4,2($19)
	lui	$20,%hi(jiffies)
	subu	$5,$2,$5
	srl	$5,$5,7
	sltu	$4,$4,$5
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L137
	lw	$3,28($sp)
	.set	macro
	.set	reorder

$L95:
	slt	$2,$3,129
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L155
	lw	$4,24($sp)
	.set	macro
	.set	reorder

$L92:
	lw	$2,%lo(drop_priority_max_idx)($23)
	addiu	$22,$22,1
	slt	$2,$22,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L104
	addiu	$19,$19,4
	.set	macro
	.set	reorder

$L90:
	jal	local_bh_enable
	jal	local_bh_disable
	lw	$2,%lo(drop_priority_max_idx)($23)
	.set	noreorder
	.set	nomacro
	blez	$2,$L105
	lui	$19,%hi(drop_priority)
	.set	macro
	.set	reorder

	addiu	$19,$19,%lo(drop_priority)
	move	$22,$0
	li	$18,-1			# 0xffffffffffffffff
	lui	$21,%hi(jiffies)
	li	$fp,12			# 0xc
$L118:
	lbu	$2,0($19)
	sltu	$3,$2,10
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L106
	lui	$3,%hi(Udp_State_Hash_Head)
	.set	macro
	.set	reorder

	lui	$3,%hi(Tcp_State_Hash_Head)
	sll	$2,$2,3
	addiu	$3,$3,%lo(Tcp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$2,4($2)
	sw	$2,24($sp)
	lw	$17,0($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$17,$L107
	move	$3,$0
	.set	macro
	.set	reorder

	lw	$16,0($17)
	addiu	$17,$17,-140
	lb	$2,148($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$18,$L138
	addiu	$3,$3,1
	.set	macro
	.set	reorder

$L108:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L156
	move	$4,$17
	.set	macro
	.set	reorder

$L109:
	lw	$4,24($sp)
$L157:
	.set	noreorder
	.set	nomacro
	beq	$4,$16,$L107
	lw	$2,0($16)
	.set	macro
	.set	reorder

	move	$17,$16
	addiu	$17,$17,-140
	move	$16,$2
	lb	$2,148($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$18,$L108
	addiu	$3,$3,1
	.set	macro
	.set	reorder

$L138:
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	sw	$3,28($sp)
	.set	macro
	.set	reorder

	sb	$2,148($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L109
	lw	$3,28($sp)
	.set	macro
	.set	reorder

	move	$4,$17
$L156:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_timer_expires
	sw	$3,28($sp)
	.set	macro
	.set	reorder

	lw	$5,%lo(jiffies)($21)
	lhu	$4,2($19)
	lui	$20,%hi(jiffies)
	subu	$5,$2,$5
	srl	$5,$5,9
	sltu	$4,$4,$5
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L140
	lw	$3,28($sp)
	.set	macro
	.set	reorder

$L110:
	slt	$2,$3,129
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L157
	lw	$4,24($sp)
	.set	macro
	.set	reorder

$L107:
	lw	$2,%lo(drop_priority_max_idx)($23)
$L151:
	addiu	$22,$22,1
	slt	$2,$22,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L118
	addiu	$19,$19,4
	.set	macro
	.set	reorder

$L105:
	jal	local_bh_enable
	lui	$3,%hi(rtl_newGC_session_status_flags)
	lw	$5,%lo(rtl_newGC_session_status_flags)($3)
	li	$4,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$5,$4,$L81
	move	$2,$0
	.set	macro
	.set	reorder

	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	sw	$4,%lo(rtl_newGC_session_status_flags)($3)
	addiu	$3,$2,200
	lui	$2,%hi(rtl_newGC_session_status_time)
	sw	$3,%lo(rtl_newGC_session_status_time)($2)
	.set	noreorder
	.set	nomacro
	j	$L81
	move	$2,$0
	.set	macro
	.set	reorder

$L137:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L95
	lw	$3,28($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	li	$2,3			# 0x3
	move	$5,$22
	move	$6,$0
	li	$7,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks1
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	li	$4,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L128
	lw	$3,28($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$fp,$L97
	li	$2,3			# 0x3
	.set	macro
	.set	reorder

	move	$4,$17
	move	$5,$22
	move	$6,$0
	li	$7,1			# 0x1
	sw	$3,28($sp)
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks2
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L95
	lw	$3,28($sp)
	.set	macro
	.set	reorder

$L91:
	addiu	$2,$2,-11
	sll	$2,$2,3
	addiu	$3,$3,%lo(Udp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$2,4($2)
	sw	$2,24($sp)
	lw	$17,0($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$17,$L92
	move	$3,$0
	.set	macro
	.set	reorder

	lw	$16,0($17)
	addiu	$17,$17,-140
	lb	$2,148($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$18,$L141
	addiu	$3,$3,1
	.set	macro
	.set	reorder

$L99:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L158
	move	$4,$17
	.set	macro
	.set	reorder

$L100:
	lw	$4,24($sp)
$L159:
	.set	noreorder
	.set	nomacro
	beq	$4,$16,$L92
	lw	$2,0($16)
	.set	macro
	.set	reorder

	move	$17,$16
	addiu	$17,$17,-140
	move	$16,$2
	lb	$2,148($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$18,$L99
	addiu	$3,$3,1
	.set	macro
	.set	reorder

$L141:
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	sw	$3,28($sp)
	.set	macro
	.set	reorder

	sb	$2,148($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L100
	lw	$3,28($sp)
	.set	macro
	.set	reorder

	move	$4,$17
$L158:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_timer_expires
	sw	$3,28($sp)
	.set	macro
	.set	reorder

	lw	$5,%lo(jiffies)($21)
	lhu	$4,2($19)
	lui	$20,%hi(jiffies)
	subu	$5,$2,$5
	srl	$5,$5,7
	sltu	$4,$4,$5
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L143
	lw	$3,28($sp)
	.set	macro
	.set	reorder

$L101:
	slt	$2,$3,129
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L159
	lw	$4,24($sp)
	.set	macro
	.set	reorder

	lw	$2,%lo(drop_priority_max_idx)($23)
	addiu	$22,$22,1
	slt	$2,$22,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L104
	addiu	$19,$19,4
	.set	macro
	.set	reorder

	j	$L90
$L143:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L101
	lw	$3,28($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	li	$2,12			# 0xc
	move	$5,$22
	move	$6,$0
	li	$7,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks1
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	li	$4,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L128
	lw	$3,28($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$fp,$L97
	li	$2,12			# 0xc
	.set	macro
	.set	reorder

	move	$4,$17
	move	$5,$22
	move	$6,$0
	li	$7,1			# 0x1
	sw	$3,28($sp)
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks2
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L101
	lw	$3,28($sp)
	.set	macro
	.set	reorder

$L128:
	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	jal	rtl_death_action
	move	$4,$17
	.set	macro
	.set	reorder

$L97:
	lui	$2,%hi(rtl_newGC_session_status_flags)
	lw	$3,%lo(rtl_newGC_session_status_flags)($2)
	bne	$3,$0,$L78
	lw	$3,%lo(jiffies)($20)
	li	$4,3			# 0x3
$L150:
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
	addiu	$3,$3,1
	sw	$4,%lo(rtl_newGC_session_status_flags)($2)
	lui	$2,%hi(rtl_newGC_session_status_time)
	sw	$3,%lo(rtl_newGC_session_status_time)($2)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,72
	.set	macro
	.set	reorder

$L78:
	li	$2,1			# 0x1
$L81:
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

$L140:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L110
	lw	$3,28($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	li	$2,3			# 0x3
	move	$5,$22
	li	$6,2			# 0x2
	move	$7,$0
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks1
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	li	$4,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L128
	lw	$3,28($sp)
	.set	macro
	.set	reorder

	li	$4,2			# 0x2
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L97
	li	$2,3			# 0x3
	.set	macro
	.set	reorder

	move	$4,$17
	move	$5,$22
	li	$6,2			# 0x2
	move	$7,$0
	sw	$3,28($sp)
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks2
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L110
	lw	$3,28($sp)
	.set	macro
	.set	reorder

$L106:
	addiu	$2,$2,-11
	sll	$2,$2,3
	addiu	$3,$3,%lo(Udp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$2,4($2)
	sw	$2,24($sp)
	lw	$17,0($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$17,$L107
	move	$3,$0
	.set	macro
	.set	reorder

	lw	$16,0($17)
	addiu	$17,$17,-140
	lb	$2,148($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$18,$L144
	addiu	$3,$3,1
	.set	macro
	.set	reorder

$L113:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L160
	move	$4,$17
	.set	macro
	.set	reorder

$L114:
	lw	$4,24($sp)
$L161:
	.set	noreorder
	.set	nomacro
	beq	$4,$16,$L107
	lw	$2,0($16)
	.set	macro
	.set	reorder

	move	$17,$16
	addiu	$17,$17,-140
	move	$16,$2
	lb	$2,148($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$18,$L113
	addiu	$3,$3,1
	.set	macro
	.set	reorder

$L144:
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	sw	$3,28($sp)
	.set	macro
	.set	reorder

	sb	$2,148($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L114
	lw	$3,28($sp)
	.set	macro
	.set	reorder

	move	$4,$17
$L160:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_timer_expires
	sw	$3,28($sp)
	.set	macro
	.set	reorder

	lw	$5,%lo(jiffies)($21)
	lhu	$4,2($19)
	lui	$20,%hi(jiffies)
	subu	$5,$2,$5
	srl	$5,$5,9
	sltu	$4,$4,$5
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L146
	lw	$3,28($sp)
	.set	macro
	.set	reorder

$L115:
	slt	$2,$3,129
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L161
	lw	$4,24($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L151
	lw	$2,%lo(drop_priority_max_idx)($23)
	.set	macro
	.set	reorder

$L146:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L115
	lw	$3,28($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	move	$5,$22
	li	$6,2			# 0x2
	move	$7,$0
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks1
	sw	$fp,16($sp)
	.set	macro
	.set	reorder

	li	$4,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L128
	lw	$3,28($sp)
	.set	macro
	.set	reorder

	li	$4,2			# 0x2
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L97
	move	$5,$22
	.set	macro
	.set	reorder

	move	$4,$17
	li	$6,2			# 0x2
	move	$7,$0
	sw	$3,28($sp)
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks2
	sw	$fp,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L115
	lw	$3,28($sp)
	.set	macro
	.set	reorder

$L126:
	jal	local_bh_disable
	lui	$23,%hi(drop_priority_max_idx)
	lw	$2,%lo(drop_priority_max_idx)($23)
	blez	$2,$L73
	lui	$20,%hi(drop_priority)
	lui	$22,%hi(Tcp_State_Hash_Head)
	addiu	$20,$20,%lo(drop_priority)
	addiu	$22,$22,%lo(Tcp_State_Hash_Head)
	move	$21,$0
	li	$19,-1			# 0xffffffffffffffff
$L80:
	lbu	$2,0($20)
	sltu	$3,$2,10
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L74
	sll	$2,$2,3
	.set	macro
	.set	reorder

	addu	$2,$22,$2
	lw	$18,4($2)
	lw	$17,0($18)
	.set	noreorder
	.set	nomacro
	beq	$18,$17,$L162
	lw	$2,%lo(drop_priority_max_idx)($23)
	.set	macro
	.set	reorder

	lw	$16,0($17)
	addiu	$17,$17,-140
	lb	$2,148($17)
	beq	$2,$19,$L147
$L75:
	beq	$2,$0,$L148
$L76:
	move	$17,$16
	.set	noreorder
	.set	nomacro
	beq	$18,$16,$L74
	lw	$2,0($16)
	.set	macro
	.set	reorder

$L149:
	addiu	$17,$17,-140
	move	$16,$2
	lb	$2,148($17)
	bne	$2,$19,$L75
$L147:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L76
	sb	$2,148($17)
	.set	macro
	.set	reorder

$L148:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$0,$L130
	move	$17,$16
	.set	noreorder
	.set	nomacro
	bne	$18,$16,$L149
	lw	$2,0($16)
	.set	macro
	.set	reorder

$L74:
	lw	$2,%lo(drop_priority_max_idx)($23)
$L162:
	addiu	$21,$21,1
	slt	$2,$21,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L80
	addiu	$20,$20,4
	.set	macro
	.set	reorder

$L73:
	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	j	$L152
	lui	$2,%hi(_prot_limit+8)
	.set	macro
	.set	reorder

	.end	drop_one_conntrack
	.size	drop_one_conntrack, .-drop_one_conntrack
	.align	2
	.globl	clean_from_lists
	.set	nomips16
	.ent	clean_from_lists
	.type	clean_from_lists, @function
clean_from_lists:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	move	$16,$4
	move	$17,$5
	.set	noreorder
	.set	nomacro
	jal	rtl_hlist_nulls_del_rcu
	move	$5,$0
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_hlist_nulls_del_rcu
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_protonum
	move	$5,$0
	.set	macro
	.set	reorder

	li	$3,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L180
	li	$3,17			# 0x11
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L181
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L182
	lui	$2,%hi(prot_counters)
	.set	macro
	.set	reorder

	move	$4,$16
$L185:
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_protonum
	move	$5,$0
	.set	macro
	.set	reorder

	li	$3,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L168
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_protonum
	move	$5,$0
	.set	macro
	.set	reorder

	li	$3,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L184
	move	$5,$17
	.set	macro
	.set	reorder

$L169:
	move	$4,$16
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	nf_ct_remove_expectations
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L168:
	move	$5,$17
$L184:
	.set	noreorder
	.set	nomacro
	jal	clean_from_lists_hooks
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_list_del
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$2,%hi(nf_conntrack_max)
	lw	$2,%lo(nf_conntrack_max)($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L183
	move	$4,$16
	.set	macro
	.set	reorder

$L186:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	lui	$2,%hi(rtl_newGC_session_status_flags)
	sw	$0,%lo(rtl_newGC_session_status_flags)($2)
	.set	noreorder
	.set	nomacro
	j	nf_ct_remove_expectations
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L180:
	lui	$2,%hi(prot_counters)
	addiu	$2,$2,%lo(prot_counters)
$L166:
 #APP
 # 111 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, 4($2)		# atomic_sub
	subu	$3, 1
	sc	$3, 4($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L185
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L166
$L181:
	lui	$2,%hi(prot_counters)
	addiu	$2,$2,%lo(prot_counters)
$L167:
 #APP
 # 111 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, 8($2)		# atomic_sub
	subu	$3, 1
	sc	$3, 8($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L185
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L167
$L183:
	.set	noreorder
	.set	nomacro
	jal	rtl_gc_threshold_check
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L186
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$2,%hi(rtl_newGC_session_status_flags)
	lw	$4,%lo(rtl_newGC_session_status_flags)($2)
	li	$3,3			# 0x3
	beq	$4,$3,$L169
	lui	$4,%hi(jiffies)
	lw	$4,%lo(jiffies)($4)
	sw	$3,%lo(rtl_newGC_session_status_flags)($2)
	lui	$2,%hi(rtl_newGC_session_status_time)
	addiu	$3,$4,1
	.set	noreorder
	.set	nomacro
	j	$L169
	sw	$3,%lo(rtl_newGC_session_status_time)($2)
	.set	macro
	.set	reorder

$L182:
$L165:
 #APP
 # 111 "/home/kai_wang/97F_release/branch-rtl819x-sdk-v3.4.10-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3
	ll	$3, %lo(prot_counters)($2)		# atomic_sub
	subu	$3, 1
	sc	$3, %lo(prot_counters)($2)
	.set	mips0

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L185
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L165
	.end	clean_from_lists
	.size	clean_from_lists, .-clean_from_lists
	.align	2
	.globl	conntrack_dointvec_minmax
	.set	nomips16
	.ent	conntrack_dointvec_minmax
	.type	conntrack_dointvec_minmax, @function
conntrack_dointvec_minmax:
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 24, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	lw	$2,56($sp)
	move	$16,$5
	jal	proc_dointvec_minmax
	sw	$2,16($sp)

	bne	$2,$0,$L188
	move	$17,$2

	beq	$16,$0,$L188
	move	$2,$0

	lui	$10,%hi(_prot_limit)
	lui	$9,%hi(prot_limit)
	li	$8,1374355456			# 0x51eb0000
	addiu	$10,$10,%lo(_prot_limit)
	addiu	$9,$9,%lo(prot_limit)
	ori	$8,$8,0x851f
	lui	$7,%hi(nf_conntrack_max)
	li	$6,12			# 0xc
$L189:
	addu	$3,$9,$2
	lw	$5,0($3)
	lw	$3,%lo(nf_conntrack_max)($7)
	addu	$4,$10,$2
	addiu	$2,$2,4
	mul	$5,$3,$5
	multu	$5,$8
	mfhi	$5
	srl	$5,$5,5
	bne	$2,$6,$L189
	sw	$5,0($4)

	li	$2,-859045888			# 0xffffffffcccc0000
	sll	$4,$3,2
	ori	$2,$2,0xcccd
	multu	$4,$2
	lui	$4,%hi(rtl_nf_conntrack_threshold)
	mfhi	$2
	srl	$2,$2,2
	subu	$5,$3,$2
	sltu	$5,$5,65
	bne	$5,$0,$L190
	sw	$2,%lo(rtl_nf_conntrack_threshold)($4)

	addiu	$2,$3,-64
	sw	$2,%lo(rtl_nf_conntrack_threshold)($4)
$L190:
	bne	$3,$0,$L194
	lui	$2,%hi(rtl_newGC_session_status_flags)

$L195:
	sw	$0,%lo(rtl_newGC_session_status_flags)($2)
$L188:
	move	$2,$17
	lw	$31,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,40

$L194:
	jal	rtl_gc_threshold_check
	move	$4,$0

	bne	$2,$0,$L195
	lui	$2,%hi(rtl_newGC_session_status_flags)

	lw	$4,%lo(rtl_newGC_session_status_flags)($2)
	li	$3,3			# 0x3
	beq	$4,$3,$L188
	lui	$4,%hi(jiffies)

	lw	$4,%lo(jiffies)($4)
	sw	$3,%lo(rtl_newGC_session_status_flags)($2)
	lui	$2,%hi(rtl_newGC_session_status_time)
	addiu	$3,$4,1
	j	$L188
	sw	$3,%lo(rtl_newGC_session_status_time)($2)

	.set	macro
	.set	reorder
	.end	conntrack_dointvec_minmax
	.size	conntrack_dointvec_minmax, .-conntrack_dointvec_minmax
	.align	2
	.globl	conntrack_dointvec
	.set	nomips16
	.ent	conntrack_dointvec
	.type	conntrack_dointvec, @function
conntrack_dointvec:
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 24, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	lw	$2,56($sp)
	move	$16,$5
	jal	proc_dointvec
	sw	$2,16($sp)

	bne	$2,$0,$L197
	move	$17,$2

	beq	$16,$0,$L197
	move	$2,$0

	lui	$10,%hi(_prot_limit)
	lui	$9,%hi(prot_limit)
	li	$8,1374355456			# 0x51eb0000
	addiu	$10,$10,%lo(_prot_limit)
	addiu	$9,$9,%lo(prot_limit)
	ori	$8,$8,0x851f
	lui	$7,%hi(nf_conntrack_max)
	li	$6,12			# 0xc
$L198:
	addu	$3,$9,$2
	lw	$5,0($3)
	lw	$3,%lo(nf_conntrack_max)($7)
	addu	$4,$10,$2
	addiu	$2,$2,4
	mul	$5,$3,$5
	multu	$5,$8
	mfhi	$5
	srl	$5,$5,5
	bne	$2,$6,$L198
	sw	$5,0($4)

	li	$2,-859045888			# 0xffffffffcccc0000
	sll	$4,$3,2
	ori	$2,$2,0xcccd
	multu	$4,$2
	lui	$4,%hi(rtl_nf_conntrack_threshold)
	mfhi	$2
	srl	$2,$2,2
	subu	$5,$3,$2
	sltu	$5,$5,65
	bne	$5,$0,$L199
	sw	$2,%lo(rtl_nf_conntrack_threshold)($4)

	addiu	$2,$3,-64
	sw	$2,%lo(rtl_nf_conntrack_threshold)($4)
$L199:
	bne	$3,$0,$L203
	lui	$2,%hi(rtl_newGC_session_status_flags)

$L204:
	sw	$0,%lo(rtl_newGC_session_status_flags)($2)
$L197:
	move	$2,$17
	lw	$31,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,40

$L203:
	jal	rtl_gc_threshold_check
	move	$4,$0

	bne	$2,$0,$L204
	lui	$2,%hi(rtl_newGC_session_status_flags)

	lw	$4,%lo(rtl_newGC_session_status_flags)($2)
	li	$3,3			# 0x3
	beq	$4,$3,$L197
	lui	$4,%hi(jiffies)

	lw	$4,%lo(jiffies)($4)
	sw	$3,%lo(rtl_newGC_session_status_flags)($2)
	lui	$2,%hi(rtl_newGC_session_status_time)
	addiu	$3,$4,1
	j	$L197
	sw	$3,%lo(rtl_newGC_session_status_time)($2)

	.set	macro
	.set	reorder
	.end	conntrack_dointvec
	.size	conntrack_dointvec, .-conntrack_dointvec
	.globl	conntrack_min
	.section	.bss,"aw",@nobits
	.align	2
	.type	conntrack_min, @object
	.size	conntrack_min, 4
conntrack_min:
	.space	4
	.globl	conntrack_max
	.data
	.align	2
	.type	conntrack_max, @object
	.size	conntrack_max, 4
conntrack_max:
	.word	100
	.globl	Tcp_State_Hash_Head
	.align	2
	.type	Tcp_State_Hash_Head, @object
	.size	Tcp_State_Hash_Head, 88
Tcp_State_Hash_Head:
	.word	0
	.word	0
	.word	1
	.word	syn_sent_list
	.word	2
	.word	syn_recv_list
	.word	3
	.word	established_list
	.word	4
	.word	fin_wait_list
	.word	5
	.word	close_wait_list
	.word	6
	.word	last_ack_list
	.word	7
	.word	time_wait_list
	.word	8
	.word	close_list
	.word	9
	.word	listen_list
	.word	10
	.word	0
	.globl	Udp_State_Hash_Head
	.align	2
	.type	Udp_State_Hash_Head, @object
	.size	Udp_State_Hash_Head, 16
Udp_State_Hash_Head:
	.byte	11
	.space	3
	.word	udp_unreply_list
	.byte	12
	.space	3
	.word	udp_assured_list
	.globl	drop_priority
	.align	2
	.type	drop_priority, @object
	.size	drop_priority, 40
drop_priority:
	.byte	8
	.space	1
	.half	60
	.byte	6
	.space	1
	.half	30
	.byte	5
	.space	1
	.half	60
	.byte	7
	.space	1
	.half	120
	.byte	4
	.space	1
	.half	120
	.byte	11
	.space	1
	.half	85
	.byte	1
	.space	1
	.half	110
	.byte	2
	.space	1
	.half	30
	.byte	12
	.space	1
	.half	10
	.byte	3
	.space	1
	.half	120
	.local	prot_counters
	.comm	prot_counters,12,4
	.local	_prot_limit
	.comm	_prot_limit,12,4
	.globl	drop_priority_max_idx
	.section	.bss
	.align	2
	.type	drop_priority_max_idx, @object
	.size	drop_priority_max_idx, 4
drop_priority_max_idx:
	.space	4
	.globl	rtl_newGC_session_status_flags
	.align	2
	.type	rtl_newGC_session_status_flags, @object
	.size	rtl_newGC_session_status_flags, 4
rtl_newGC_session_status_flags:
	.space	4
	.globl	rtl_newGC_session_status_time
	.align	2
	.type	rtl_newGC_session_status_time, @object
	.size	rtl_newGC_session_status_time, 4
rtl_newGC_session_status_time:
	.space	4
	.globl	prot_limit
	.align	2
	.type	prot_limit, @object
	.size	prot_limit, 12
prot_limit:
	.space	12
	.data
	.align	2
	.type	syn_sent_list, @object
	.size	syn_sent_list, 8
syn_sent_list:
	.word	syn_sent_list
	.word	syn_sent_list
	.align	2
	.type	syn_recv_list, @object
	.size	syn_recv_list, 8
syn_recv_list:
	.word	syn_recv_list
	.word	syn_recv_list
	.align	2
	.type	established_list, @object
	.size	established_list, 8
established_list:
	.word	established_list
	.word	established_list
	.align	2
	.type	fin_wait_list, @object
	.size	fin_wait_list, 8
fin_wait_list:
	.word	fin_wait_list
	.word	fin_wait_list
	.align	2
	.type	close_wait_list, @object
	.size	close_wait_list, 8
close_wait_list:
	.word	close_wait_list
	.word	close_wait_list
	.align	2
	.type	last_ack_list, @object
	.size	last_ack_list, 8
last_ack_list:
	.word	last_ack_list
	.word	last_ack_list
	.align	2
	.type	time_wait_list, @object
	.size	time_wait_list, 8
time_wait_list:
	.word	time_wait_list
	.word	time_wait_list
	.align	2
	.type	close_list, @object
	.size	close_list, 8
close_list:
	.word	close_list
	.word	close_list
	.align	2
	.type	listen_list, @object
	.size	listen_list, 8
listen_list:
	.word	listen_list
	.word	listen_list
	.align	2
	.type	udp_unreply_list, @object
	.size	udp_unreply_list, 8
udp_unreply_list:
	.word	udp_unreply_list
	.word	udp_unreply_list
	.align	2
	.type	udp_assured_list, @object
	.size	udp_assured_list, 8
udp_assured_list:
	.word	udp_assured_list
	.word	udp_assured_list
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
