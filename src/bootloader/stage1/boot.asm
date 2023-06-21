[org 0x7C00]
[bits 16]

jmp _boot
nop

; TODO: Find new name for labels
; TODO: Smaller name for FAT header entry
; TODO: More error handling and error message
; TODO: Warnings at load_boots2

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
							db 0 					; reserved
bs_boot_signature:			db 0x29
bs_volume_id:				dd 0x00000000
bs_volume_label:			db 'AMETHYSTDSK'
bs_file_system_type:		db 'FAT12   '

; ===== BOOTCODE ===== ;

%macro  	print		1
	mov 	si,			%1
	call  	puts
%endmacro

%macro 		halt_cpu 	0
	cli
	hlt
%endmacro

_boot:
	print 	s_welcome 								; Debug in screen
	mov 	[bs_drive_number],	dl 					; Save disk drive number

	; Setup data segment
	mov 	ax,			0 							; Can't access DS/ES directly
	mov 	ds,			ax 							; Set Data Segment
	mov 	es,			ax 							; Set Extended Segment

	; Setup stack
	mov 	ss,			ax							; Set stack to 0
	mov 	sp,			0 							; Set stack point to 0

	; Setup CS (via a far jump)
	jmp 	0:init 									; Jump far to 0:init. Will set CS

init:
.setup_fat:
	; Read drive parameters instead of relying on
	; data on formatted disk
	mov 	ah, 		0x08  						; Interrupt disk, get format informations
	int 	0x13 									; Do interrupt
	jc 		disk_error								; If error, go HALT

	; CL = Sector number
	and 	cl, 		0x3F 					 	; Remove high 2 bits
	xor 	ch, 		ch 							; CH = 0 (CX = 00000000 00NNNNNN)
	mov 	[bpb_sectors_per_track], cx				; Save sector number

	; DH = Head number
	inc 	dh 										; Increment number of head (start counting from 1)
	mov 	[bpb_head_count], 	dh					; Save head number
	
	; Compute LBA of root directory
	;   = fats * sectors_per_fat + reserved
	mov 	ax, 		[bpb_sectors_per_fat]  		; AX = (sectors_per_fat)
	mov 	bl, 		[bpb_number_fats] 			; BL = (fats)
	xor 	bh, 		bh  						; Zero BH (BX = 00000000 NNNNNNNN)
	mul 	bx 										; AX = (fats * sectors_per_fat)
	add 	ax, 		[bpb_reserved_sector_count]	; AX = (fats * sectors_per_fat + reserved)
	push 	ax 										; Push into the stack

	; Compute size of root directory
	;   = (32 * number_of_entries) / bytes_per_sector
	mov  	ax,			[bpb_sectors_per_fat]		; AX = (number_of_entries)
	shl  	ax,			5 							; AX = (number_of_entries * 32)
	mov  	bx,			[bs_byte_per_sector] 		; BX = bytes_per_sector
	div  	bx										; AX = (number_of_entries * 32) - bytes_per_sector

read_entries:
	print	s_search_boot_bin 						; Debug in screen
	mov 	ax, 		19							; Get the LBA of the 
	call 	lbachs 									; Get the CHS address
	call 	read 									; Read the disk at the CHS address
	mov  	di,			buffer						; DI is address of current entry name
	xor  	bx,			bx 							; BX is count
	; we will skip the first entry because it is the disk name
.check_entries:
	; Check if we finish
	cmp  	bx, 		[bpb_root_entry_count]		; Compare with number of entry
	jge		.fail_found 							; If we check all, fail
	; Compare with what we search
	mov 	si, 		s_boots2 					; The file name to search
	mov  	cx,			11							; Size of the string to compare (filename size)
	push 	di 										; Save DI (will be modified)
	repe 	cmpsb									; Compare the strings
	pop 	di 										; Restore DI
	je 		load_boots2 							; If they are the same, we found it!
 	; Increment the counter
	inc  	bx  									; Increment the number of element readed
	add  	di,			32  						; Increment the entry pointer
	jmp 	.check_entries 							; Look back
.fail_found:
	print	s_err_not_found							; Print error string
	halt_cpu 										; Halt CPU
load_boots2:
	; Getting the size of the file
	; WARN: Remainder is not calculated...
	print 	s_load_boot_bin
	mov  	eax,  		[di+28]  					; EAX = size of file in byte
	mov  	bx,			[bs_byte_per_sector]  		; BX = Number byte per sector
	div   	bx 				  						; AL = size in sector, DX = remainder
	xor  	ah,  		ah 							; Clear garbage in AH
	or  	dx, 		dx 							; Compare the remainder with 0
	jz		.next 									; If remainder is not 0, will grab last sector
	inc     ax 										; Increment the number of sector to grab
.next:
	push 	ax 										; Saving it for later
	; Getting the address of the file
	; first cluster = (kernel_cluster_number -2) * sectors_per_cluster + + cluster_number
	; WARN: May only work on floppy... 31 is a MAGIC NUMBER
	mov   	ax,			[di + 26] 					; AX = cluster
	add  	ax,  		31 							; Add previous sectors
	call 	lbachs
	; Read the file
	print 	s_exec_boot_bin
	pop   	ax  									; Restore the size
	call 	read 									; Read the file in the buffer
	jmp		buffer 									; SI point to the file content
	halt_cpu

;
; Read from disk
; IN  AL number of sector
;     CH Cylinder number
;     CL Sector number
;     DH Head count
; OUT [buffer]
read:
	mov 	dl, 	[bs_drive_number] 				; Hard disk number
	mov  	bx,		0 								; ES:BX -> Address of
	mov 	es, 	bx 								; pointer
	mov 	bx, 	buffer 							; BX is 7E00
	
	mov		ah, 	2 								; Disk interrupt
	int 	0x13 									; Read sector
	jc 		disk_error								; If carry is set -> Error
.end:
	ret


disk_error:
	mov 	al,		ah 								; Set error code in AL to be printed
	print  	s_err_disk_read							; Set message read failure in SI
	call 	puth 									; Print the error code
	halt_cpu										; Halt the machine

;
; Convert an LBA address to a CHS address
; IN  AX LDA address
; OUT CL sector number
;     CH cylinder
;     DH head
lbachs:
	push ax											; Save ax
	push dx 										; Save dl

	xor dx, dx										; DX = 0 
	div word [bpb_sectors_per_track]				; AX = LBA / SectorPerTrack
													; DX = LBA % SectorPerTrack
	inc dx 											; DX = LBA % SectorPerTrack + 1
	mov cx, dx										; Put it in CX
	
	xor dx, dx										; DX = 0
	div word [bpb_head_count]						; AX = LBA / SectorPerTrack / heads
													; DX = LBA % SectorPerTrack / heads

	mov dh, dl 										; DH is head
	mov ch, al 										; CH = cylinder
	shl ah, 6 							
	or  cl, ah 										; Put upper 2 bits of cylinder in CL

	pop ax 											; Restore dl
	mov dl, al
	pop ax 											; restore ax
	ret

;
; Print AL in to the screen, 0x0A is interpreted as 0x0D, 0x0A
putc:
	push  	ax 										; Save AX
	mov 	ah, 	0x0E					 		; Write character mode
	cmp 	al, 	0x0A						 	; Check if AL is LF
	je  	.line 									;  > Draw new line
.end: 
	int 	0x10									; Interrupt Video
	pop 	ax 										; Restore AX
	ret
.line:
	mov 	al,		0x0D 							; Set AL to CR
	int 	0x10
	mov 	al,		0x0A 							; Set AL to LF
	jmp 	.end 									; Jump to end

;
; Print the CSTR referenced by SI
puts:
	push 	si 										; Save SI
	push 	ax 										; Save AX
.fetch:
	lodsb 											; Load next char from SI in AL
	or  	al, 	al 								; Check if AL is 0
	jz  	.end 									;  > End
	call 	putc 									; Print char
	jmp 	.fetch 									; Fetch next
.end:
	pop 	ax 										; Restore AX
	pop 	si 										; Restore SI
	ret

;
; Print AX in hexadecimal
; Source: https://github.com/ApplePy/osdev/blob/master/bootsect.asm
puth:
	pusha
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

	popa
	ret

cluster:		dw 0

s_welcome: 			db "Welcome in AmethystOS!", 0x0A, 0
s_search_boot_bin:	db "Searching boot.bin", 0x0A, 0
s_load_boot_bin:	db "Loading boot.bin", 0x0A, 0
s_exec_boot_bin:	db "Executing boot.bin", 0x0A, 0
s_err_disk_read: 	db "F:READ=", 0
s_err_not_found:	db "F:NotFound", 0
s_boots2:			db "BOOT    BIN"
s_file_found: 		db "File is found"

times 510 - ($ - $$) db 0
db 0x55, 0xAA

buffer: