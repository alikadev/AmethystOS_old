bits 16

%include "basicio.inc"
%include "string.inc"

section .text.boot

global _start
_start:
	mov si, msg_hello
	call puts

.main_loop:
	mov di, buffer 		; DI = Memory buffer
	mov ax, 32 			; AX = Buffer size
	call gets 			; Get command from user
	
	; Cmd HELP
	mov si, cmd_help
	call streq
	jc process_help
	
	; Cmd MREAD
	mov si, cmd_mread
	call streq
	jc process_not_already_implemented
	
	; Cmd MREAD
	mov si, cmd_mreadn
	call streq
	jc process_not_already_implemented

	; Cmd MWRITE
	mov si, cmd_mwrite
	call streq
	jc process_not_already_implemented

	; Cmd FLIST
	mov si, cmd_flist
	call streq
	jc process_not_already_implemented

	; Cmd FMAKE
	mov si, cmd_fmake
	call streq
	jc process_not_already_implemented

	; Cmd FREMOVE
	mov si, cmd_fremove
	call streq
	jc process_not_already_implemented

	; Cmd FREAD
	mov si, cmd_fread
	call streq
	jc process_not_already_implemented

	; Cmd FWRITE
	mov si, cmd_fwrite
	call streq
	jc process_not_already_implemented

	; Cmd JUMP
	mov si, cmd_jump
	call streq
	jc process_not_already_implemented

	; Cmd CALL
	mov si, cmd_call
	call streq
	jc process_not_already_implemented

	; Unknown 
	mov si, msg_unknwown_cmd
	call puts
	jmp .main_loop

section .text

process_help:
	mov si, msg_help
	call puts
	jmp _start.main_loop

process_not_already_implemented:
	mov si, msg_not_already_implemented
	call puts
	jmp _start.main_loop

; ================================= ;
;          SECTION  DATA            ;
; ================================= ;
section .data

msg_hello: db 'Welcome to the kernel', LF, 0

msg_not_already_implemented: db 'This CMD is not already implemented!', LF, LF, 0

cmd_help:		db 'HELP',		0
cmd_mread: 		db 'MREAD', 	0
cmd_mreadn:		db 'MREADN', 	0; 
cmd_mwrite:		db 'MWRITE', 	0;
cmd_flist: 		db 'FLIST',   	0
cmd_fmake: 		db 'FMAKE',   	0;
cmd_fremove:	db 'FREMOVE',  	0;
cmd_fread: 		db 'FREAD', 	0;
cmd_fwrite:		db 'FWRITE', 	0;
cmd_jump:		db 'JUMP', 		0
cmd_call:		db 'CALL', 		0

msg_unknwown_cmd:
db 'Unknwown cmd', LF, \
'Write HELP to print help message', LF, LF, 0

msg_help:
db 'List of CMD:', LF, \
'HELP    | Print This', LF, \
'MREAD   | Read memory (MREAD <addr> <n>)', LF, \
'MWRITE  | Write memory (MWRITE <addr> <n> <hex>)', LF, \
'FLIST   | List files', LF, \
'FMAKE   | Make a file (FMAKE <name>)', LF, \
'FREMOVE | Remove a file (FREMOVE <name>)', LF, \
'FREAD   | Copy file in memory (FREAD <file> <addr>)', LF, \
'FREADN  | Copy file in memory for N (FREAD <file> <addr> <n>)', LF, \
'FWRITE  | Write file from memory (FWRITE <file> <addr> <n>)', LF, \
'JUMP    | Jump to memory address (JUMP <addr>)', LF, \
'CALL    | Call to memory address (CALL <addr>)', LF, LF, 0

buffer:    times 32 db 0