[bits 16]

global nmi_enable
global nmi_disable

nmi_enable:
	push   	ax
	in 		al,			0x70
	and    	al,			0x7F
	out  	0x70,		al
	in   	al,			0x71
	pop   	ax
	ret

nmi_disable:
	push   	ax
	in 		al,			0x70
	or    	al,			0x80
	out  	0x70,		al
	in   	al,			0x71
	pop   	ax
	ret
