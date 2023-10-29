[bits 32]

section .text

%include "sys/std.inc"

global halt
halt:
	cli
	hlt
