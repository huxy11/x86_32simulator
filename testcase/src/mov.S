#include "trap.h"

.globl _start
_start:
	mov $0x04030201, %eax
	mov %eax, (%edi)
	mov $0x08070605, %eax
	mov %eax, 0x4(%edi)
	lgdt (%edi)

	lgdt (0x000000ff)

	mov %eax, %cs
	mov %eax, %ss
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	mov $0x80000000, %ebp
	mov $0x80000000, %esp
	mov $0x02, %eax
	mov %eax, 0x8(%edi)
	mov $0x05, %eax
	mov %eax, 0xc(%edi)
	mov $0x02, %eax
	
	push (%edx)
	pop (%ecx)

	nop
	push (%edx)


	idiv %ecx

	idiv %cx

	repnz scas %es:(%edi), %eax

	mov %edi, %eax
	mov (%edi), %eax

	add $0x0, %eax
	add $0x1, %eax
	mov $0x2000000, %edi
	mov $0x3, %ecx
	rep stos %eax, %es:(%edi)


	mov $0x7fffffff, %eax
	add $0x02, %eax
	
	mov $0xffffffff, %eax
	add $0x02, %eax

	
	mov $0, %eax
	cmp $99, %eax
	je 0x100004

	cmp $0x1, %eax	
	sub $-0x1, %esp
	sub $0x1, %esp
	sub $0x7F, %esp
	
	movl $0b11101110, %eax
	movl $0b11111111, %ebx
	test %eax, %ebx
	
	movl $0x1, %eax
	movl $0x1, %ebx
	test %eax, %ebx
	
	mov $0xffffffff, %eax
	mov $0xffffffff, %ebx
	test %eax, %ebx

	push %eax
	push %ebx
	push %ecx
	push %edx
	push %ax

	pop %ax
	pop %eax
	pop %ebx
	pop %ecx
	pop %eax



	call 0x100025
	ret



	test %eax, %eax
	test %ebx, %eax

	cmp $0x1, %eax	
	je 0x10052
	movl $0, %eax
	movw $2, %ax
	movl $0, %ebx
	movl $0, %ecx
	movl $0, %edx

	movl $32768, %ecx
	movw $0, %bx
	movb $0, %bh

	movl $1, 0x1234
	movw $1, 0x1234
	movb $1, 0x1234

	movl $1, (%ecx)
	movw $1, (%ecx)
	movb $1, (%ecx)

	movl $1, (%ecx, %ebx, 4)
	movw $1, (%ecx, %ebx, 4)
	movb $1, (%ecx, %ebx, 4)

	movl $1, 2(%ecx)
	movw $1, 2(%ecx)
	movb $1, 2(%ecx)

	movl $1, -2(%ecx)
	movw $1, -2(%ecx)
	movb $1, -2(%ecx)

	movl $1, 2(%ecx, %ebx, 4)
	movw $1, 2(%ecx, %ebx, 4)
	movb $1, 2(%ecx, %ebx, 4)

	movl $1, -2(%ecx, %ebx, 4)
	movw $1, -2(%ecx, %ebx, 4)
	movb $1, -2(%ecx, %ebx, 4)

	movl $1, 8192(%ecx)
	movw $1, 8192(%ecx)
	movb $1, 8192(%ecx)

	movl $1, -8192(%ecx)
	movw $1, -8192(%ecx)
	movb $1, -8192(%ecx)

	movl $1, 8192(%ecx, %ebx, 4)
	movw $1, 8192(%ecx, %ebx, 4)
	movb $1, 8192(%ecx, %ebx, 4)

	movl $1, -8192(%ecx, %ebx, 4)
	movw $1, -8192(%ecx, %ebx, 4)
	movb $1, -8192(%ecx, %ebx, 4)






	movl %eax, %ebx
	movw %ax, %bx
	movb %ah, %bl

	movl %eax, 0x1234
	movw %ax, 0x1234
	movb %al, 0x1234
	movb %ah, 0x1234

	movl %eax, (%ebx)

	movl %eax, (%ecx, %ebx, 4)
	movw %ax, (%ecx, %ebx, 4)
	movb %ah, (%ecx, %ebx, 4)

	movl %eax, 2(%ecx)
	movw %ax, 2(%ecx)
	movb %ah, 2(%ecx)

	movl %eax, 2(%ecx, %ebx, 4)
	movw %ax, 2(%ecx, %ebx, 4)
	movb %ah, 2(%ecx, %ebx, 4)

	movl %eax, 8192(%ecx)
	movw %ax, 8192(%ecx)
	movb %ah, 8192(%ecx)

	movl %eax, 8192(%ecx, %ebx, 4)
	movw %ax, 8192(%ecx, %ebx, 4)
	movb %ah, 8192(%ecx, %ebx, 4)

	movl %edx, %ebx
	movw %dx, %bx
	movb %dh, %bl

	movl %edx, 0x1234
	movw %dx, 0x1234
	movb %dl, 0x1234
	movb %dh, 0x1234

	movl %edx, (%ebx)

	movl %edx, (%ecx, %ebx, 4)
	movw %dx, (%ecx, %ebx, 4)
	movb %dh, (%ecx, %ebx, 4)

	movl %edx, 2(%ecx)
	movw %dx, 2(%ecx)
	movb %dh, 2(%ecx)

	movl %edx, 2(%ecx, %ebx, 4)
	movw %dx, 2(%ecx, %ebx, 4)
	movb %dh, 2(%ecx, %ebx, 4)

	movl %edx, 8192(%ecx)
	movw %dx, 8192(%ecx)
	movb %dh, 8192(%ecx)

	movl %edx, 8192(%ecx, %ebx, 4)
	movw %dx, 8192(%ecx, %ebx, 4)
	movb %dh, 8192(%ecx, %ebx, 4)





	movl 0x1234, %eax
	movw 0x1234, %ax
	movb 0x1234, %al
	movb 0x1234, %ah

	movl (%ebx), %eax

	movl (%ecx, %ebx, 4), %eax
	movw (%ecx, %ebx, 4), %ax
	movb (%ecx, %ebx, 4), %ah

	movl 2(%ecx), %eax
	movw 2(%ecx), %ax
	movb 2(%ecx), %ah

	movl 2(%ecx, %ebx, 4), %eax
	movw 2(%ecx, %ebx, 4), %ax
	movb 2(%ecx, %ebx, 4), %ah

	movl 8192(%ecx), %eax
	movw 8192(%ecx), %ax
	movb 8192(%ecx), %ah

	movl 8192(%ecx, %ebx, 4), %eax
	movw 8192(%ecx, %ebx, 4), %ax
	movb 8192(%ecx, %ebx, 4), %ah

	movl 0x1234, %edx
	movw 0x1234, %dx
	movb 0x1234, %dl
	movb 0x1234, %dh

	movl (%ebx), %edx

	movl (%ecx, %ebx, 4), %edx
	movw (%ecx, %ebx, 4), %dx
	movb (%ecx, %ebx, 4), %dh

	movl 2(%ecx), %edx
	movw 2(%ecx), %dx
	movb 2(%ecx), %dh

	movl 2(%ecx, %ebx, 4), %edx
	movw 2(%ecx, %ebx, 4), %dx
	movb 2(%ecx, %ebx, 4), %dh

	movl 8192(%ecx), %edx
	movw 8192(%ecx), %dx
	movb 8192(%ecx), %dh

	movl 8192(%ecx, %ebx, 4), %edx
	movw 8192(%ecx, %ebx, 4), %dx
	movb 8192(%ecx, %ebx, 4), %dh

	HIT_GOOD_TRAP

