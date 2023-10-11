section .text

global outb
outb:
	push ebp
	mov ebp, esp
	mov dx, [esp + 8]
	mov al, [esp + 12]
	out dx, al
	pop ebp
	ret

global outw
outw:
	push ebp
	mov ebp, esp
	mov dx, [esp + 8]
	mov ax, [esp + 12]
	out dx, ax
	pop ebp
	ret

global inb
inb:
	push ebp
	mov ebp, esp
	mov dx, [esp + 8]
	in al, dx
	pop ebp
	ret

global inw
inw:
	push ebp
	mov ebp, esp
	mov dx, [esp + 8]
	in ax, dx
	pop ebp
	ret

global halt
halt:
	cli
	hlt