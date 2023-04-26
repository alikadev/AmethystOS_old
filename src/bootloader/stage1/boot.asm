[org 0x7C00]
[bits 16]

jmp _boot
nop

; ===== FAT HEADERS ===== ;
; Boot record
bs_oem_name:				db 'MSDOS5.0'
bs_byte_per_sector:			dw 512
bpb_sector_per_cluster:		db 1
bpb_reserved_sector_count:	dw 1
bpb_number_fats:			db 2
bpb_root_entry_count:		dw 0xE0
bpb_total_sector_16bit:		dw 2880
bpb_media:					db 0xF0
bpb_sectors_per_fat:		dw 9
bpb_sectors_per_track:		dw 18
bpb_head_count:				dw 2
bpb_hidden_sectors:			dd 0
bpb_large_sectors:			dd 0

; Extended boot record
bs_drive_number:			db 0
							db 0 			; reserved
bs_boot_signature:			db 0x29
bs_volume_id:				dd 0x00000000
bs_volume_label:			db 'AMETHYSTDSK'
bs_file_system_type:		db 'FAT12   '

; ===== BOOTCODE ===== ;
_boot:
	mov 	[bs_drive_number],	dl 			; Save disk drive number

	; Setup data segment
	mov 	ax,			0 					; Can't access DS/ES directly
	mov 	ds,			ax 					; Set Data Segment
	mov 	es,			ax 					; Set Extended Segment

	; Setup stack
	mov 	ss,			ax					; Set stack to 0
	mov 	sp,			0 					; Set stack point to 0

	jmp 	word  		0:start 			; Far jump to next line

start:
	mov  	si, 		msg_welcome 		; Print greetings
	call 	puts

	; Small app: 
	; Read 8 first byte of each sector
	mov 	ax,			0 					; Set first LBA address
.mainloop:
	call  	puth 							; Print the address
	push 	ax 								; Save the address
	call 	lbachs 							; Get the CHS address
	mov  	ax, 		':' 				; Print ':' in video
	call    putc 							; ...

	mov 	ax,			1 					; Set the number of sector to read at 1
	call 	read 							; Read the sector (or will halt)
	mov  	ax, 		[buffer] 			; Print the first 2 bytes
	call 	puth
	mov  	ax, 		[buffer+2] 			; Print the 2 next bytes
	call 	puth
	mov  	ax, 		[buffer+4] 			; Print the 2 next bytes
	call 	puth
	mov  	ax, 		[buffer+6] 			; Print the 2 next bytes
	call 	puth
	mov 	al,			0x0A 				; Print a new line
	call 	putc
	mov 	ah, 		0x0 				; Wait keyboard interrupt
	int 	0x16
	pop 	ax 								; Restore the LBA address
	inc 	ax 								; Increment it
	jmp 	.mainloop  						; Loop back

;
; Read from disk
; IN  AL number of sector
;     CH Cylinder number
;     CL Sector number
;     DH Head count
; OUT [buffer]
read:
	mov 	dl, 	[bs_drive_number] 		; Hard disk number
	mov  	bx,		0 						; ES:BX -> Address of
	mov 	es, 	bx 						; pointer
	mov 	bx, 	buffer 					; BX is 7E00
	
	mov		ah, 	2 						; Disk interrupt
	int 	0x13 							; Read sector
	jc 		.error 							; If carry is set -> Error
.end:
	ret
.error:
	mov 	al,		ah 						; Set error code in AL to be printed
	mov  	si, 	err_disk_read			; Set message read failure in SI
	call 	puts 							; Print the message
	call 	puth 							; Print the error code
	jmp 	halt 							; Halt the machine

;
; Convert an LBA address to a CHS address
; IN  AX LDA address
; OUT CL sector number
;     CH cylinder
;     DH head
lbachs:
	push ax									; Save ax
	push dx 								; Save dl

	xor dx, dx								; DX = 0 
	div word [bpb_sectors_per_track]		; AX = LBA / SectorPerTrack
											; DX = LBA % SectorPerTrack
	inc dx 									; DX = LBA % SectorPerTrack + 1
	mov cx, dx								; Put it in CX
	
	xor dx, dx								; DX = 0
	div word [bpb_head_count]				; AX = LBA / SectorPerTrack / heads
											; DX = LBA % SectorPerTrack / heads

	mov dh, dl 								; DH is head
	mov ch, al 								; CH = cylinder
	shl ah, 6 							
	or  cl, ah 								; Put upper 2 bits of cylinder in CL

	pop ax 									; Restore dl
	mov dl, al
	pop ax 									; restore ax
	ret

;
; Print AL in to the screen, 0x0A is interpreted as 0x0D, 0x0A
putc:
	push  	ax 								; Save AX
	mov 	ah, 	0x0E			 		; Write character mode
	cmp 	al, 	0x0A				 	; Check if AL is LF
	je  	.line 							;  > Draw new line
.end: 
	int 	0x10							; Interrupt Video
	pop 	ax 								; Restore AX
	ret
.line:
	mov 	al,		0x0D 					; Set AL to CR
	int 	0x10
	mov 	al,		0x0A 					; Set AL to LF
	jmp 	.end 							; Jump to end

;
; Print the CSTR referenced by SI
puts:
	push 	si 								; Save SI
	push 	ax 								; Save AX
.fetch:
	lodsb 									; Load next char from SI in AL
	or  	al, 	al 						; Check if AL is 0
	jz  	.end 							;  > End
	call 	putc 							; Print char
	jmp 	.fetch 							; Fetch next
.end:
	pop 	ax 								; Restore AX
	pop 	si 								; Restore SI
	ret
;
; Print AX in hexadecimal
; Source: https://github.com/ApplePy/osdev/blob/master/bootsect.asm
puth:
	push    ax 
	push    bx
	push    cx
	mov  	bx, 	ax
	mov  	cx, 	4
.next:
	mov  	ax, 	bx
	rol  	bx, 	4
	shr  	ax, 	12
	cmp  	ax, 	10
	jl   	.skip
	add  	ax, 	7
.skip:
	add  	ax, 	48
	call 	putc
	loop 	.next

	mov  	ax, 	0x20
	call 	putc

	pop 	cx
	pop 	bx
	pop 	ax
	ret

halt:
	cli
	hlt

msg_welcome: 		db "Booted", 0x0A, 0
err_disk_read: 		db "F:READ=", 0

times 510 - ($ - $$) db 0
db 0x55, 0xAA

buffer: