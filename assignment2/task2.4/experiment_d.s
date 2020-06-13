	.file	"experiment_d.cpp"
	.text
	.section	.text._ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEE6_M_runEv,"axG",@progbits,_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEE6_M_runEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEE6_M_runEv
	.type	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEE6_M_runEv, @function
_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEE6_M_runEv:
.LFB3110:
	.cfi_startproc
	jmp	*8(%rdi)
	.cfi_endproc
.LFE3110:
	.size	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEE6_M_runEv, .-_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEE6_M_runEv
	.section	.text._ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED2Ev,"axG",@progbits,_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED2Ev
	.type	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED2Ev, @function
_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED2Ev:
.LFB3091:
	.cfi_startproc
	leaq	16+_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE(%rip), %rax
	movq	%rax, (%rdi)
	jmp	_ZNSt6thread6_StateD2Ev@PLT
	.cfi_endproc
.LFE3091:
	.size	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED2Ev, .-_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED2Ev
	.weak	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED1Ev
	.set	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED1Ev,_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED2Ev
	.section	.text._ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED0Ev,"axG",@progbits,_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED5Ev,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED0Ev
	.type	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED0Ev, @function
_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED0Ev:
.LFB3093:
	.cfi_startproc
	leaq	16+_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE(%rip), %rax
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	%rax, (%rdi)
	call	_ZNSt6thread6_StateD2Ev@PLT
	movq	%rbx, %rdi
	movl	$16, %esi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	_ZdlPvm@PLT
	.cfi_endproc
.LFE3093:
	.size	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED0Ev, .-_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED0Ev
	.text
	.p2align 4,,15
	.globl	_Z12threaded_fibv
	.type	_Z12threaded_fibv, @function
_Z12threaded_fibv:
.LFB2490:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movabsq	$100000000000, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movsd	.LC0(%rip), %xmm2
.L8:
	movl	$1, %ebx
	jmp	.L9
	.p2align 4,,10
	.p2align 3
.L7:
	addq	$1, %rbx
	cmpq	%rbp, %rbx
	je	.L8
.L9:
	pxor	%xmm1, %xmm1
	cvtsi2sd	%ebx, %xmm1
	ucomisd	%xmm2, %xmm1
	jb	.L7
	movsd	.LC1(%rip), %xmm0
	call	pow@PLT
	movsd	.LC0(%rip), %xmm2
	jmp	.L7
	.cfi_endproc
.LFE2490:
	.size	_Z12threaded_fibv, .-_Z12threaded_fibv
	.p2align 4,,15
	.globl	_Z3fibi
	.type	_Z3fibi, @function
_Z3fibi:
.LFB2489:
	.cfi_startproc
	pxor	%xmm1, %xmm1
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movsd	.LC1(%rip), %xmm0
	cvtsi2sd	%edi, %xmm1
	call	pow@PLT
	divsd	.LC2(%rip), %xmm0
	call	round@PLT
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	cvttsd2si	%xmm0, %eax
	ret
	.cfi_endproc
.LFE2489:
	.size	_Z3fibi, .-_Z3fibi
	.section	.text.unlikely,"ax",@progbits
.LCOLDB3:
	.text
.LHOTB3:
	.p2align 4,,15
	.globl	_Z9benchmarkv
	.type	_Z9benchmarkv, @function
_Z9benchmarkv:
.LFB2491:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA2491
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	movl	$16, %edi
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	leaq	16+_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE(%rip), %r13
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	leaq	_Z12threaded_fibv(%rip), %r12
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	subq	$32, %rsp
	.cfi_def_cfa_offset 80
	movq	$0, 8(%rsp)
	leaq	24(%rsp), %rbp
	leaq	8(%rsp), %rbx
.LEHB0:
	call	_Znwm@PLT
.LEHE0:
	movq	pthread_create@GOTPCREL(%rip), %r14
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	movq	%r13, (%rax)
	movq	%r12, 8(%rax)
	movq	%r14, %rdx
	movq	%rax, 24(%rsp)
.LEHB1:
	call	_ZNSt6thread15_M_start_threadESt10unique_ptrINS_6_StateESt14default_deleteIS1_EEPFvvE@PLT
.LEHE1:
	movq	24(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L15
	movq	(%rdi), %rax
	call	*8(%rax)
.L15:
	movq	$0, 16(%rsp)
	movl	$16, %edi
.LEHB2:
	call	_Znwm@PLT
.LEHE2:
	movq	%r12, 8(%rax)
	leaq	16(%rsp), %r12
	movq	%r14, %rdx
	movq	%rbp, %rsi
	movq	%r13, (%rax)
	movq	%r12, %rdi
	movq	%rax, 24(%rsp)
.LEHB3:
	call	_ZNSt6thread15_M_start_threadESt10unique_ptrINS_6_StateESt14default_deleteIS1_EEPFvvE@PLT
.LEHE3:
	movq	24(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L19
	movq	(%rdi), %rax
	call	*8(%rax)
.L19:
	movq	%rbx, %rdi
.LEHB4:
	call	_ZNSt6thread4joinEv@PLT
	movq	%r12, %rdi
	call	_ZNSt6thread4joinEv@PLT
.LEHE4:
	cmpq	$0, 16(%rsp)
	jne	.L25
	cmpq	$0, 8(%rsp)
	jne	.L25
	addq	$32, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L25:
	.cfi_restore_state
	call	_ZSt9terminatev@PLT
.L28:
	jmp	.L26
.L29:
	movq	%rax, %rbx
	jmp	.L21
.L27:
	jmp	.L23
.L30:
	movq	%rax, %rbx
	jmp	.L17
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA2491:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2491-.LLSDACSB2491
.LLSDACSB2491:
	.uleb128 .LEHB0-.LFB2491
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB2491
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L30-.LFB2491
	.uleb128 0
	.uleb128 .LEHB2-.LFB2491
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L27-.LFB2491
	.uleb128 0
	.uleb128 .LEHB3-.LFB2491
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L29-.LFB2491
	.uleb128 0
	.uleb128 .LEHB4-.LFB2491
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L28-.LFB2491
	.uleb128 0
.LLSDACSE2491:
	.text
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC2491
	.type	_Z9benchmarkv.cold.4, @function
_Z9benchmarkv.cold.4:
.LFSB2491:
.L26:
	.cfi_def_cfa_offset 80
	.cfi_offset 3, -48
	.cfi_offset 6, -40
	.cfi_offset 12, -32
	.cfi_offset 13, -24
	.cfi_offset 14, -16
	cmpq	$0, 16(%rsp)
	jne	.L25
.L23:
	cmpq	$0, 8(%rsp)
	jne	.L25
	movq	%rax, %rdi
.LEHB5:
	call	_Unwind_Resume@PLT
.L21:
	movq	24(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L22
	movq	(%rdi), %rax
	call	*8(%rax)
.L22:
	movq	%rbx, %rax
	jmp	.L23
.L17:
	movq	24(%rsp), %rdi
	testq	%rdi, %rdi
	je	.L18
	movq	(%rdi), %rax
	call	*8(%rax)
.L18:
	movq	%rbx, %rdi
	call	_Unwind_Resume@PLT
.LEHE5:
	.cfi_endproc
.LFE2491:
	.section	.gcc_except_table
.LLSDAC2491:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC2491-.LLSDACSBC2491
.LLSDACSBC2491:
	.uleb128 .LEHB5-.LCOLDB3
	.uleb128 .LEHE5-.LEHB5
	.uleb128 0
	.uleb128 0
.LLSDACSEC2491:
	.section	.text.unlikely
	.text
	.size	_Z9benchmarkv, .-_Z9benchmarkv
	.section	.text.unlikely
	.size	_Z9benchmarkv.cold.4, .-_Z9benchmarkv.cold.4
.LCOLDE3:
	.text
.LHOTE3:
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB2492:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	_Z9benchmarkv
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE2492:
	.size	main, .-main
	.weak	_ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE
	.section	.rodata._ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE,"aG",@progbits,_ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE,comdat
	.align 32
	.type	_ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE, @object
	.size	_ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE, 59
_ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE:
	.string	"NSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE"
	.weak	_ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE
	.section	.data.rel.ro._ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE,"awG",@progbits,_ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE,comdat
	.align 8
	.type	_ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE, @object
	.size	_ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE, 24
_ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE
	.quad	_ZTINSt6thread6_StateE
	.weak	_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE
	.section	.data.rel.ro.local._ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE,"awG",@progbits,_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE,comdat
	.align 8
	.type	_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE, @object
	.size	_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE, 40
_ZTVNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE:
	.quad	0
	.quad	_ZTINSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEEE
	.quad	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED1Ev
	.quad	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEED0Ev
	.quad	_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJPFvvEEEEEE6_M_runEv
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1080016896
	.align 8
.LC1:
	.long	2610427048
	.long	1073341303
	.align 8
.LC2:
	.long	2610427048
	.long	1073865591
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.ident	"GCC: (Debian 8.3.0-6) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
