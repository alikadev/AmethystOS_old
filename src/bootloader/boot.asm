org 0x7C00
bits 16

;
; FAT12 header
;
jmp _start
nop

; boot record
bdb_oem:					db 'MWSIN4.1'
bdb_byte_per_sector:		dw 512
bdb_sectors_per_cluster:	db 1
bdb_reserved_sector:		dw 1
bdb_fat_count:				db 2
bdb_dir_entries_count:		dw 0x0E0
bdb_total_sectors:			dw 2880 
bdb_media_descriptor_type:	db 0xF0						; F0 = 3.5" floppy disk
bdb_sectors_per_fat:		dw 9
bdb_sectors_per_track:		dw 18
bdb_heads:					dw 2
bdb_hidder_sectors:			dd 0
bdb_large_sectors_count:	dd 0

; Extended boot record
ebr_drive_number:			db 0
							db 0 						; reserved
ebr_signature:				db 0x29
ebr_volume_id:				db 0x00, 0x00, 0x00, 0x00	; serial number
ebr_volume_label:			db 'AMETHYSTOS '			; 11 B
ebr_system_id:				db 'FAT12   '				; 8 B


;
; Bootloader
;

%define ENDL 0x0D, 0x0A

_start:
	; Setup data segment
	mov ax, 0 		; Can't write directly in DS/ES
	mov ds, ax		; Set DS, 0
	mov es, ax 		; Set ES, 0

	; Setup stack
	mov ss, ax		; Set stack to 0
	mov sp, 0 		; Set stack point to start point

	mov [ebr_drive_number], dl
	mov ax, 1
	mov cl, 1
	mov bx, 0x7E00
	call disk_read

	; Print HELLO message
	mov si, msg_hello 	; Load message
	call puts		; Print the message

.main_loop: 		; Program main loop
	call getc		; Get keyboard key
	call putc		; Print the key
	jmp .main_loop	; Loop back


; Get the pressed key
; OUT AH: The Scancode of the key pressed down
; OUT AL: The ASCII character og the key
getc:
	mov ah, 0x00	; Read press key service
	int 0x16		; Interrupt
	ret


; Print a char in the TTY screen
; IN  AL the char
putc:
	push ax			; Save AX
	mov ah, 0x0E 	; Service print TTY
	int 0x10 		; Interrupt video
	pop ax 			; Restore AX
	ret

; Print a string in the TTY screen
; IN  SI: The address
puts:
	push si 		; Save SI
	push ax 		; Save AX

.while:
	lodsb 			; Load next char in string in AL
	or al, al 		; Check if AL is \0
	jz .end 		; Jump to .end if process end

	mov ah, 0x0E 	; Service print TTY
	int 0x10		; Interupt video
	jmp .while

.end:
	pop ax			; Restore AX
	pop si 			; Restore SI
	ret

; Print hexadecimal byte
; IN  AL: The byte
putx:
	push ax 		; Save AX
	shr al, 4 		; Get 4 upper bits
	call putlx 		; Print the upper bits
	pop ax 			; Restore AX
	call putlx 		; Print the lower bits
	ret

; Print hexadecimal value
; IN  AL[bytes 0-3]: The value
putlx:
	push ax 			; Save AX

	and al, 0x0F 		; Zero 4 upper bits
	cmp al, 0x09 		; Compare to 9
	jg .greater 		; Goto print for greater than 9
	add al, '0' 		; AL + '0' = The numeric
	jmp .end 			; Print and done
.greater:
	add al, 'A' - 0x0A 	; AL + 'A'-0xA = The alphanumeric
.end
	mov ah, 0x0E 		; Display,
	int 0x10 			;   TTY

	pop ax 				; Restore AX
	ret

wait_key_and_reboot:
	mov ah, 0
	int 0x16
	jmp 0xFFFF:0

floppy_error:
	mov si, msg_floppy_error
	call puts
	jmp wait_key_and_reboot


; Convert an LBA addr to a CHS addr
; IN  AX LDA address
; OUT CX [bits 0-5]: sector number
;     CX [bits 6-15]: cylinder
;     DH: head
lba_to_chs:
	push ax								; Save ax
	push dx 							; Save dl

	xor dx, dx							; DX = 0 
	div word [bdb_sectors_per_track]	; AX = LBA / SectorPerTrack
										; DX = LBA % SectorPerTrack
	inc dx 								; DX = LBA % SectorPerTrack + 1
	mov cx, dx							; Put it in CX
	
	xor dx, dx							; DX = 0
	div word [bdb_heads]				; AX = LBA / SectorPerTrack / heads
										; DX = LBA % SectorPerTrack / heads

	mov dh, dl 							; DH is head
	mov ch, al 							; CH = cylinder
	shl ah, 6 							
	or  cl, ah 							; Put upper 2 bits of cylinder in CL

	pop ax 								; Restore dl
	mov dl, al
	pop ax 								; restore ax
	ret

; Read sectors from disk
; IN  AX: LBA addr
; IN  CL: Number of sectors to read {up 128}
; IN  DL: Drive number
; IN  ES:BX: Memory addr where to store data
disk_read:
	push ax 			; Save registers
	push bx
	push cx
	push di

	push cx 			; Save CX old value
	call lba_to_chs 	; Get CHS address
	pop ax 				; Put CX old value in AX

	mov ah, 0x02 		; Mode read
	mov di, 3 			; Number of try (3)

.retry:
	pusha 				; Save regs
	stc 				; Set carry flag 
	int 0x13 			; Interrupt read
						; Disable carry if it success
	jnc .done 			; Check carry
	
	popa 				; Reload regs
	call disk_reset 	; Reset disk controller

	dec di 				; Decrement try counter
	test di, di 		; If counter is different
	jnz .retry  		;   to zero, retry

.fail:
	jmp floppy_error 	; Else, error

.done:
	popa 				; Reload regs

	pop di 				; Restore registers
	pop cx
	pop bx
	pop ax

	ret


; Reset disk controller
; IN  DI: Drive number
disk_reset:
	pusha 				; Save registers
	mov ah, 0 			; Set mode 0 (reset)
	int 0x13 			; Reset controller
	jc floppy_error 	; If it fails, stop
	popa 				; Restore registers
	ret





msg_hello: db 'Welcome to the AmethystOS Bootloader', ENDL, 0
msg_floppy_error: db 'Fail to read floppy!', ENDL, 0


times 510 - ($ - $$) db 0
db 0x55, 0xAA