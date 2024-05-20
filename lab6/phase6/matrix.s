allocate:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$allocate.size, %esp
	movl	$0, %eax
	movl	%eax, -4(%ebp)
#Generating Cast 
	#Generating multiply 	
	movl	8(%ebp), %eax
	imul	$4, %eax
	pushl	%eax
	call	malloc
	addl	$4, %esp
	movl	%eax, -8(%ebp)
#Generating While
.L0:
	#Generating less than 	
	movl	-4(%ebp), %eax
	cmpl	8(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L1
	#Generating multiply 	
	movl	8(%ebp), %eax
	imul	$4, %eax
	pushl	%eax
	call	malloc
	addl	$4, %esp
	#Generating add 	
	#Generating multiply 	
	movl	%eax, -12(%ebp)
	movl	-4(%ebp), %eax
	imul	$4, %eax
	movl	-8(%ebp), %ecx
	addl	%eax, %ecx
	fldl	-12(%ebp)
	fstpl	(%ecx)
	#Generating add 	
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
	jmp	.L0
.L1:
#Generating Return
	movl	-8(%ebp), %eax
	jmp	allocate.exit

allocate.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	allocate.size, 12
	.globl	allocate

initialize:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$initialize.size, %esp
	movl	$0, %eax
	movl	%eax, -4(%ebp)
#Generating While
.L2:
	#Generating less than 	
	movl	-4(%ebp), %eax
	cmpl	12(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L3
	movl	$0, %eax
	movl	%eax, -8(%ebp)
#Generating While
.L4:
	#Generating less than 	
	movl	-8(%ebp), %eax
	cmpl	12(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L5
	#Generating add 	
	movl	-4(%ebp), %eax
	addl	-8(%ebp), %eax
	#Generating add 	
#Generating Dereference
	#Generating add 	
	#Generating multiply 	
	movl	%eax, -12(%ebp)
	movl	-4(%ebp), %eax
	imul	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	#Generating multiply 	
	movl	-8(%ebp), %eax
	imul	$4, %eax
	addl	%eax, %ecx
	fldl	-12(%ebp)
	fstpl	(%ecx)
	#Generating add 	
	movl	-8(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -8(%ebp)
	jmp	.L4
.L5:
	#Generating add 	
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
	jmp	.L2
.L3:
#Generating Return
	movl	$0, %eax
	jmp	initialize.exit

initialize.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	initialize.size, 12
	.globl	initialize

display:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$display.size, %esp
	movl	$0, %eax
	movl	%eax, -4(%ebp)
#Generating While
.L6:
	#Generating less than 	
	movl	-4(%ebp), %eax
	cmpl	12(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L7
	movl	$0, %eax
	movl	%eax, -8(%ebp)
#Generating While
.L8:
	#Generating less than 	
	movl	-8(%ebp), %eax
	cmpl	12(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L9
#Generating Dereference
	#Generating add 	
	#Generating multiply 	
	movl	-4(%ebp), %eax
	imul	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	movl	%ecx, -12(%ebp)
#Generating Dereference
	#Generating add 	
	#Generating multiply 	
	movl	-8(%ebp), %eax
	imul	$4, %eax
	movl	-12(%ebp), %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	pushl	%ecx
#Generating Address
	leal	.L11, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	#Generating add 	
	movl	-8(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -8(%ebp)
	jmp	.L8
.L9:
	#Generating add 	
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
#Generating Address
	leal	.L13, %eax
	pushl	%eax
	call	printf
	addl	$4, %esp
	jmp	.L6
.L7:
#Generating Return
	movl	$0, %eax
	jmp	display.exit

display.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	display.size, 12
	.globl	display

deallocate:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$deallocate.size, %esp
	movl	$0, %eax
	movl	%eax, -4(%ebp)
#Generating While
.L14:
	#Generating less than 	
	movl	-4(%ebp), %eax
	cmpl	12(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L15
#Generating Dereference
	#Generating add 	
	#Generating multiply 	
	movl	-4(%ebp), %eax
	imul	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	pushl	%ecx
	call	free
	addl	$4, %esp
	#Generating add 	
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
	jmp	.L14
.L15:
#Generating Cast 
	movl	8(%ebp), %eax
	pushl	%eax
	call	free
	addl	$4, %esp
#Generating Return
	movl	$0, %eax
	jmp	deallocate.exit

deallocate.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	deallocate.size, 4
	.globl	deallocate

main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
#Generating Address
	leal	-8(%ebp), %eax
	pushl	%eax
#Generating Address
	leal	.L17, %eax
	pushl	%eax
	call	scanf
	addl	$8, %esp
	pushl	-8(%ebp)
	call	allocate
	addl	$4, %esp
	movl	%eax, -4(%ebp)
	pushl	-8(%ebp)
	pushl	-4(%ebp)
	call	initialize
	addl	$8, %esp
	pushl	-8(%ebp)
	pushl	-4(%ebp)
	call	display
	addl	$8, %esp
	pushl	-8(%ebp)
	pushl	-4(%ebp)
	call	deallocate
	addl	$8, %esp

main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 8
	.globl	main

	.data
.L13:	.asciz	"\012"
.L17:	.asciz	"%d"
.L11:	.asciz	"%d "
