#ifndef C_LABEL
/* Define a macro we can use to construct the asm name for a C symbol.  */
# define C_LABEL(name)	name##:
#endif
# ifndef END
#  define END(sym)
# endif

# ifndef JUMPTARGET
#  define JUMPTARGET(sym)	sym
# endif
# define cfi_restore(reg)		.cfi_restore reg
# define cfi_restore_state		.cfi_restore_state
# define cfi_def_cfa_offset(off)	.cfi_def_cfa_offset off
# define cfi_remember_state		.cfi_remember_state
# define cfi_offset(reg, off)		.cfi_offset reg, off

# ifndef ARM_SFI_MACROS
# define ARM_SFI_MACROS 1
/* This assembly macro is prepended to any load/store instruction,
   pulling the base register out of the addressing mode syntax and
   making it the first operand of the macro.  For example:
	ldr r0, [r1]
   becomes:
	sfi_breg r1, ldr r0, [\B]
   The \B stands in for the base register that is the first operand
   to the macro, so we can avoid error-prone repetition of the base
   register in two places on the line.

   This is used for all memory access through a base register other
   than PC or SP.  It's intended to support SFI schemes such as
   Native Client, where the OS will enforce that all load/store
   instructions use a special form.  In any such configuration,
   another sysdep.h file will have defined ARM_SFI_MACROS and
   provided its own assembly macros with the same interface.  */

        .macro sfi_breg basereg, insn, operands:vararg
                .macro _sfi_breg_doit B
                \insn \operands
                .endm
                _sfi_breg_doit \basereg
                .purgem _sfi_breg_doit
        .endm

/* This assembly macro replaces the "pld" instruction.
   The syntax:
	sfi_pld REGISTER, #OFFSET
   is exactly equivalent to:
	sfi_breg REGISTER, pld [\B, #OFFSET]
   (and ", #OFFSET" is optional).  We have a separate macro
   only to work around a bug in GAS versions prior to 2.23.2,
   that misparses the sfi_breg macro expansion in this case.  */

	.macro sfi_pld basereg, offset=#0
		pld [\basereg, \offset]
	.endm

/* This macro precedes any instruction that directly changes the SP.
   It's not needed for push/pop or for any kind of load or store that
   implicitly changes the SP via the ! syntax.  */
# define sfi_sp	/* Nothing to do.  */

# define BX(R)          bx      R
# define BXC(C, R)      bx##C   R
#  define BLX(R)        blx     R
#define DO_RET(R)       BX(R)
#define RETINSTR(C, R)  BXC(C, R)


# endif
