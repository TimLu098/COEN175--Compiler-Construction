foo:
	pushl %ebp
	movl %esp, %ebp
	subl $0, %esp
movl   x, $1
movl   y, $2
movl   z, $3
	movl %ebp, %esp
	popl %ebp
	ret
	.globl foo
	.comm	x, 4
	.comm	y, 4
	.comm	z, 4
