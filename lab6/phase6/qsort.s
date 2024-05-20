exchange:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$exchange.size, %esp
#Generating Dereference
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
#Generating Dereference
	movl	12(%ebp), %eax
	movl	(%eax), %eax
#Assign Dereference
	movl	8(%ebp), %ecx
#Assignment Dereference integer
	mov	%eax, (%ecx)
#Assign Dereference
	movl	12(%ebp), %eax
#Assignment Dereference integer
	movl	-4(%ebp), %ecx
	mov	%ecx, (%eax)
#Generating Return
	movl	$0, %eax
	jmp	exchange.exit

exchange.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	exchange.size, 4
	.globl	exchange

partition:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$partition.size, %esp
#Generating Dereference
	#Generating add 	
	#Generating multiply 	
	movl	12(%ebp), %eax
	imul	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	movl	%ecx, -12(%ebp)
	#Generating subtract 	
	movl	12(%ebp), %eax
	subl	$1, %eax
	movl	%eax, -4(%ebp)
	#Generating add 	
	movl	16(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -8(%ebp)
#Generating While
.L0:
	#Generating less than 	
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L1
	#Generating subtract 	
	movl	-8(%ebp), %eax
	subl	$1, %eax
	movl	%eax, -8(%ebp)
#Generating While
.L2:
	#Generating greater than 	
#Generating Dereference
	#Generating add 	
	#Generating multiply 	
	movl	-8(%ebp), %eax
	imul	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	cmpl	-12(%ebp), %ecx
	setg	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L3
	#Generating subtract 	
	movl	-8(%ebp), %eax
	subl	$1, %eax
	movl	%eax, -8(%ebp)
	jmp	.L2
.L3:
	#Generating add 	
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
#Generating While
.L4:
	#Generating less than 	
#Generating Dereference
	#Generating add 	
	#Generating multiply 	
	movl	-4(%ebp), %eax
	imul	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	cmpl	-12(%ebp), %ecx
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L5
	#Generating add 	
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
	jmp	.L4
.L5:
#Generating If
	#Generating less than 	
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L6
#Generating Address
	#Generating add 	
	#Generating multiply 	
	movl	-8(%ebp), %eax
	imul	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	pushl	%ecx
#Generating Address
	#Generating add 	
	#Generating multiply 	
	movl	-4(%ebp), %eax
	imul	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	pushl	%ecx
	call	exchange
	addl	$8, %esp
.L6:
	jmp	.L0
.L1:
#Generating Return
	movl	-8(%ebp), %eax
	jmp	partition.exit

partition.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	partition.size, 16
	.globl	partition

quickSort:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$quickSort.size, %esp
#Generating If
	#Generating greater than 	
	movl	16(%ebp), %eax
	cmpl	12(%ebp), %eax
	setg	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L8
	pushl	16(%ebp)
	pushl	12(%ebp)
	pushl	8(%ebp)
	call	partition
	addl	$12, %esp
	movl	%eax, -4(%ebp)
	pushl	-4(%ebp)
	pushl	12(%ebp)
	pushl	8(%ebp)
	call	quickSort
	addl	$12, %esp
	pushl	16(%ebp)
	#Generating add 	
	movl	-4(%ebp), %eax
	addl	$1, %eax
	pushl	%eax
	pushl	8(%ebp)
	call	quickSort
	addl	$12, %esp
.L8:
#Generating Return
	movl	$0, %eax
	jmp	quickSort.exit

quickSort.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	quickSort.size, 4
	.globl	quickSort

main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
#Generating For
	movl	$0, %eax
	movl	%eax, -84(%ebp)
.L10:
	#Generating less than 	
	movl	-84(%ebp), %eax
	cmpl	$20, %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L11
	#Generating remainder 	
	call	rand
	movl	$20, %ecx
	cltd	
	idivl	%ecx
#Assign Dereference
	#Generating add 	
#Generating Address
	leal	-80(%ebp), %eax
	#Generating multiply 	
	movl	%eax, -88(%ebp)
	movl	-84(%ebp), %eax
	imul	$4, %eax
	movl	-88(%ebp), %ecx
	addl	%eax, %ecx
#Assignment Dereference integer
	mov	%edx, (%ecx)
	#Generating add 	
	movl	-84(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -84(%ebp)
	jmp	.L10
.L11:
	#Generating subtract 	
	movl	$20, %eax
	subl	$1, %eax
	pushl	%eax
	pushl	$0
#Generating Address
	leal	-80(%ebp), %eax
	pushl	%eax
	call	quickSort
	addl	$12, %esp
#Generating For
	movl	$0, %eax
	movl	%eax, -84(%ebp)
.L12:
	#Generating less than 	
	movl	-84(%ebp), %eax
	cmpl	$20, %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L13
#Generating Dereference
	#Generating add 	
#Generating Address
	leal	-80(%ebp), %eax
	#Generating multiply 	
	movl	%eax, -92(%ebp)
	movl	-84(%ebp), %eax
	imul	$4, %eax
	movl	-92(%ebp), %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	pushl	%ecx
#Generating Address
	leal	.L15, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	#Generating add 	
	movl	-84(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -84(%ebp)
	jmp	.L12
.L13:

main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 92
	.globl	main

	.data
.L15:	.asciz	"%d\012"
