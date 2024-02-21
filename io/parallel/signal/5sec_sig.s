	.file	"5sec_sig.c"
	.text
	.type	alrm_handler, @function
alrm_handler:
.LFB51:
	.cfi_startproc
	endbr64
	movl	$0, loop(%rip)
	ret
	.cfi_endproc
.LFE51:
	.size	alrm_handler, .-alrm_handler
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB52:
	.cfi_startproc
	endbr64
	pushq	%rax
	.cfi_def_cfa_offset 16
	popq	%rax
	.cfi_def_cfa_offset 8
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$5, %edi
	call	alarm@PLT
	leaq	alrm_handler(%rip), %rsi
	movl	$14, %edi
	call	signal@PLT
	movl	loop(%rip), %eax
	testl	%eax, %eax
	je	.L5
	movl	$0, %edx
.L4:
	addq	$1, %rdx
	movl	loop(%rip), %eax
	testl	%eax, %eax
	jne	.L4
.L3:
	leaq	.LC0(%rip), %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$0, %edi
	call	exit@PLT
.L5:
	movl	$0, %edx
	jmp	.L3
	.cfi_endproc
.LFE52:
	.size	main, .-main
	.data
	.align 4
	.type	loop, @object
	.size	loop, 4
loop:
	.long	1
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
