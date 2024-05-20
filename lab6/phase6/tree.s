insert:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$insert.size, %esp
#Generating If
#Generating Not
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L0
#Generating Cast 
	#Generating multiply 	
	movl	$4, %eax
	imul	$3, %eax
	pushl	%eax
	call	malloc
	addl	$4, %esp
	movl	%eax, 8(%ebp)
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$0, %eax
	fldl	12(%ebp)
	fstpl	(%eax)
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$4, %eax
	fldl	null
	fstpl	(%eax)
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$8, %eax
	fldl	null
	fstpl	(%eax)
	jmp	.L1
.L0:
#Generating If
	#Generating less than 	
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$0, %eax
	movl	(%eax), %eax
	movl	12(%ebp), %ecx
	cmpl	%eax, %ecx
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L2
#Generating Cast 
	pushl	12(%ebp)
#Generating Cast 
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	insert
	addl	$8, %esp
	#Generating add 	
	movl	8(%ebp), %ecx
	addl	$4, %ecx
	fldl	%eax
	fstpl	(%ecx)
	jmp	.L3
.L2:
#Generating If
	#Generating greater than 	
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$0, %eax
	movl	(%eax), %eax
	movl	12(%ebp), %ecx
	cmpl	%eax, %ecx
	setg	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L4
#Generating Cast 
	pushl	12(%ebp)
#Generating Cast 
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	insert
	addl	$8, %esp
	#Generating add 	
	movl	8(%ebp), %ecx
	addl	$8, %ecx
	fldl	%eax
	fstpl	(%ecx)
.L4:
.L3:
.L1:
#Generating Return
	movl	8(%ebp), %eax
	jmp	insert.exit

insert.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	insert.size, 0
	.globl	insert

search:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$search.size, %esp
#Generating If
#Generating Not
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L6
#Generating Return
	movl	$0, %eax
	jmp	search.exit
.L6:
#Generating If
	#Generating less than 	
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$0, %eax
	movl	(%eax), %eax
	movl	12(%ebp), %ecx
	cmpl	%eax, %ecx
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L8
#Generating Return
	pushl	12(%ebp)
#Generating Cast 
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	search
	addl	$8, %esp
	jmp	search.exit
.L8:
#Generating If
	#Generating greater than 	
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$0, %eax
	movl	(%eax), %eax
	movl	12(%ebp), %ecx
	cmpl	%eax, %ecx
	setg	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L10
#Generating Return
	pushl	12(%ebp)
#Generating Cast 
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	search
	addl	$8, %esp
	jmp	search.exit
.L10:
#Generating Return
	movl	$1, %eax
	jmp	search.exit

search.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	search.size, 0
	.globl	search

preorder:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$preorder.size, %esp
#Generating If
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	je	.L12
#Generating Dereference
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$0, %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	pushl	%eax
#Generating Address
	leal	.L15, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
#Generating Cast 
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	preorder
	addl	$4, %esp
#Generating Cast 
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	preorder
	addl	$4, %esp
.L12:
#Generating Return
	movl	$0, %eax
	jmp	preorder.exit

preorder.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	preorder.size, 0
	.globl	preorder

inorder:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$inorder.size, %esp
#Generating If
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	je	.L16
#Generating Cast 
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	inorder
	addl	$4, %esp
#Generating Dereference
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$0, %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	pushl	%eax
#Generating Address
	leal	.L15, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
#Generating Cast 
#Generating Dereference
	#Generating add 	
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	inorder
	addl	$4, %esp
.L16:
#Generating Return
	movl	$0, %eax
	jmp	inorder.exit

inorder.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	inorder.size, 0
	.globl	inorder

main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	movl	$0, %eax
	movl	%eax, -48(%ebp)
#Generating While
.L19:
	#Generating less than 	
	movl	-48(%ebp), %eax
	cmpl	$8, %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L20
	#Generating add 	
#Generating Address
	leal	-44(%ebp), %eax
	#Generating multiply 	
	movl	%eax, -52(%ebp)
	movl	-48(%ebp), %eax
	imul	$4, %eax
	movl	-52(%ebp), %ecx
	addl	%eax, %ecx
	fldl	-48(%ebp)
	fstpl	(%ecx)
	#Generating add 	
	movl	-48(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -48(%ebp)
	jmp	.L19
.L20:
#Generating Cast 
	movl	null, %eax
	movl	%eax, -4(%ebp)
#Generating Address
	#Generating add 	
#Generating Address
	leal	-44(%ebp), %eax
	addl	$28, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
#Generating Address
	#Generating add 	
#Generating Address
	leal	-44(%ebp), %eax
	addl	$16, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
#Generating Address
	#Generating add 	
#Generating Address
	leal	-44(%ebp), %eax
	addl	$4, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
#Generating Address
	#Generating add 	
#Generating Address
	leal	-44(%ebp), %eax
	addl	$0, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
#Generating Address
	#Generating add 	
#Generating Address
	leal	-44(%ebp), %eax
	addl	$20, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
#Generating Address
	#Generating add 	
#Generating Address
	leal	-44(%ebp), %eax
	addl	$8, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
#Generating Address
	#Generating add 	
#Generating Address
	leal	-44(%ebp), %eax
	addl	$12, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
#Generating Address
	#Generating add 	
#Generating Address
	leal	-44(%ebp), %eax
	addl	$24, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
#Generating Address
	leal	.L22, %eax
	pushl	%eax
	call	printf
	addl	$4, %esp
	pushl	-4(%ebp)
	call	preorder
	addl	$4, %esp
#Generating Address
	leal	.L24, %eax
	pushl	%eax
	call	printf
	addl	$4, %esp
	pushl	-4(%ebp)
	call	inorder
	addl	$4, %esp

main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 52
	.globl	main

	.comm	null, 4
	.data
.L15:	.asciz	"%d\012"
.L24:	.asciz	"inorder traversal:\012"
.L22:	.asciz	"preorder traversal:\012"
