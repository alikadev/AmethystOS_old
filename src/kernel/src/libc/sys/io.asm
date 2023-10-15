section .text

global halt
halt:
	cli
	hlt

global return_interrupt
return_interrupt:
	iret