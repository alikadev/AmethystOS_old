org 0x7C00
bits 16

%define ENDL 0x0D, 0x0A

;
; FAT12 header
;
jmp start
nop

; ==================== ;
;      BOOT RECORD     ;
; ==================== ;
; boot record
bdb_oem:					db 'MWSIN4.1'
bdb_byte_per_sector:		dw 512
bdb_sectors_per_cluster:	db 1
bdb_reserved_sector:		dw 1
bdb_fat_count:				db 2
bdb_dir_entries_count:		dw 0x0E0
bdb_total_sectors:			dw 2880 
bdb_media_descriptor_type:	db 0xF0				; F0 = 3.5" floppy disk
bdb_sectors_per_fat:		dw 9
bdb_sectors_per_track:		dw 18
bdb_heads:					dw 2
bdb_hidder_sectors:			dd 0
bdb_large_sectors_count:	dd 0

; Extended boot record
ebr_drive_number:			db 0
							db 0 			 	; reserved
ebr_signature:				db 0x29
ebr_volume_id:				dw 0x0000, 0x0000	; serial number
ebr_volume_label:			db 'AMETHYSTOS '	; 11 B
ebr_system_id:				db 'FAT12   '		; 8 B

; ==================== ;
;    IMPLEMENTATION    ;
; ==================== ;

start:
	; Setup data segment
	mov ax, 0 		; Can't write directly in DS/ES
	mov ds, ax		; Set DS, 0
	mov es, ax 		; Set ES, 0

	; Setup stack
	mov ss, ax		; Set stack to 0
	mov sp, 0 		; Set stack point to start point

	push es
	push word .after
	retf

.after:
	; Read something from floppy
	; BIOS should set DL to drive number
	mov [ebr_drive_number], dl

	mov si, msg_loading
	call puts

	; Read drive parameters instead of relying on
	; data on formatted disk
	push es
	mov ah, 0x08 
	int 0x13
	jc floppy_error
	pop es

	and cl, 0x3F 					; Remove top 2 bits
	xor ch, ch 						; CH = 0
	mov [bdb_sectors_per_track], cx ; Sector count

	inc dh
	mov [bdb_heads], dh 			; Head count

	; Compute LDBA of root directory
	;   = reserved + fats * sectors_per_fat
	mov ax, [bdb_sectors_per_fat]	; Compute LBA of root directory
									;   reserved + fats * sectors_per_fat
	mov bl, [bdb_fat_count]
	xor bh, bh
	mul bx 							; DX:AX = (fat * sectors_per_fat)
	add ax, [bdb_reserved_sector] 	; ax = LDA of root directory
	push ax

	; Compute size of root directory
	;   = (32 * number_of_entries) / bytes_per_sector
	mov ax, [bdb_sectors_per_fat]
	shl ax, 5 						; AX *= 32
	xor dx, dx 						; DX = 0
	div word [bdb_byte_per_sector]  ; Number of sectord we need to read

	test dx, dx 					; if DX != 0, add 1
	jz .root_dir_after
	inc ax 							; division remainder != 0, add 1

.root_dir_after:
	mov cl, al 						; Number of sectors to read = size of root directory
	pop ax 							; AX = LBA of root directory
	mov dl, [ebr_drive_number]	 	; DL = Drive number 
	mov bx, buffer 					; es:bx = buffer
	call disk_read
	; Search for kerel.bin
	xor bx, bx
	mov di, buffer

.search_kernel:
	mov si, file_kernel_bin
	mov cx, 11
	push di
	repe cmpsb 
	pop di
	je .found_kernel

	add di, 32
	inc bx
	cmp bx, [bdb_dir_entries_count]
	jl .search_kernel
	jmp kernel_not_found_error

.found_kernel:
	; DI should have the address of the entry
	mov ax, [di + 26]
	mov [kernel_cluster], ax

	; load FAT from disk in memory
	mov ax, [bdb_reserved_sector]
	mov bx, buffer
	mov cl, [bdb_sectors_per_fat]
	mov dl, [ebr_drive_number]
	call disk_read

	; read kernel and process FAT chain
	mov bx, KERNEL_LOAD_SEGMENT
	mov es, bx
	mov bx, KERNEL_LOAD_OFFSET

.load_kernel_loop:
	mov ax, [kernel_cluster]
	add ax, 31 				; first cluster = (kernel_cluster_number -2) * sectors_per_cluster + + cluster_number
	
	mov cl, 1
	mov dl, [ebr_drive_number]
	call disk_read

	add bx, [bdb_byte_per_sector]

	; compute location of next cluster
	mov ax, [kernel_cluster]
	mov cx, 3
	mul cx
	mov cx, 2
	div cx

	mov si, buffer
	add si, ax
	mov ax, [ds:si]
	or dx, dx
	jz .even

.odd:
	shr ax, 4
	jmp .next_cluster_after
.even:
	and ax, 0x0FFF
.next_cluster_after:
	cmp ax, 0x0FF8
	jae .read_finish
	mov [kernel_cluster], ax 
	jmp .load_kernel_loop

.read_finish:
	mov dl, [ebr_drive_number]
	mov ax, KERNEL_LOAD_SEGMENT
	mov ds, ax
	mov es, ax

	jmp KERNEL_LOAD_SEGMENT:KERNEL_LOAD_OFFSET

	jmp wait_key_and_reboot

	cli
	hlt

; ==================== ;

kernel_not_found_error:
	mov si, msg_kernel_not_found
	call puts
	jmp wait_key_and_reboot

; ==================== ;

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

; ==================== ;

; Wait key input and reboot the OS
wait_key_and_reboot:
	mov ah, 0 			; Mode get key input
	int 0x16 			; Interrupt keyboard
	jmp 0xFFFF:0 		; Jump to BIOS start

; ==================== ;

; Print the floppy error and reboot
floppy_error:
	mov si, msg_floppy_error 	; Load the error message
	call puts 					; Print error message
	jmp wait_key_and_reboot 	; reboot

; ==================== ;

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

; ==================== ;

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

; ==================== ;

; Reset disk controller
; IN  DI: Drive number
disk_reset:
	pusha 				; Save registers
	mov ah, 0 			; Set mode 0 (reset)
	int 0x13 			; Reset controller
	jc floppy_error 	; If it fails, stop
	popa 				; Restore registers
	ret

; ==================== ;
;         DATA         ;
; ==================== ;

msg_loading:			db 'Loading...', ENDL, 0
msg_floppy_error: 		db 'E: Read floppy', ENDL, 0
msg_kernel_not_found: 	db 'E: kernel not found', ENDL, 0
file_kernel_bin: 		db 'KERNEL  BIN'
kernel_cluster: 		dw 0

KERNEL_LOAD_SEGMENT 	equ 0x2000
KERNEL_LOAD_OFFSET		equ 0

times 510 - ($ - $$) db 0
db 0x55, 0xAA

buffer: