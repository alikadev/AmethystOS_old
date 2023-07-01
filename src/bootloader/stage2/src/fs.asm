[bits 16]

global fs_init
global fs_read
%include "disk.inc"
%include "io.inc"

; SECTOR PER FAT OFFSET
OFF_SECPFAT equ 22
; NUMBER FATS OFFSET
OFF_NUMFAT equ 16
; NUMBER RESERVER SECTOR OFFSET
OFF_NUMRESSEC equ 14
; NUMBER ROOT ENTRIES OFFSET
OFF_NUMROTENT equ 17
; BYTE PER SECTOR
OFF_BYTPERSEC equ 11

;
; Init the FS
; IN   AL  The drive id
; OUT  CF  Set if failure
fs_init:
	xor  	dh,			dh				; Clear DL
	mov 	[diskID], 	dl
	mov  	dl,			al 				; DL must contains Disk ID
	; Reset disk
	call 	disk_reset
	jc   	error.reset_failure
	; Get disk informations
	call  	disk_get_info
	jc 		error.get_info_failure
	; Get number of sector per track
	and 	cl, 		0x3F 			; Remove high 2 bits
	xor 	ch, 		ch 				; CH = 0 (CX = 00000000 00NNNNNN)
	mov 	[sectors_per_track], cx		; Save sector number
	; Get number of head
	inc  	dh
	mov     [head_count], dh
	; Get the address of the first sector
	xor 	ax,		ax 								; LBA = 0 (first sector)
	call    lba_to_chs
	mov     al, 1 									; Only read 1 sector
	mov  	bx,		0 								; ES:BX -> Address of
	mov 	es, 	bx 								; pointer
	mov 	bx, 	buffer 							; BX is 7E00
	mov 	dl, 	[diskID] 						; DL = Disk ID
	call  	disk_read 
	jc 		error.read_disk_failure
	; Compute LBA of root directory
	;   = fats * sectors_per_fat + reserved
	mov 	ax, 		[buffer + OFF_SECPFAT] 		; AX = (sectors_per_fat)
	mov 	bl, 		[buffer + OFF_NUMFAT]		; BL = (fats)
	xor 	bh, 		bh  						; Zero BH (BX = 00000000 NNNNNNNN)
	mul 	bx 										; AX = (fats * sectors_per_fat)
	add 	ax, 		[buffer + OFF_NUMRESSEC]	; AX = (fats * sectors_per_fat + reserved)
	mov   	[rootdir], 	ax

	; Compute size of root directory
	;   = (32 * number_of_entries) / bytes_per_sector
	mov  	ax,			[buffer + OFF_NUMROTENT]	; AX = (number_of_entries)
	shl  	ax,			5 							; AX = (number_of_entries * 32)
	mov  	bx,			[buffer + OFF_BYTPERSEC]	; BX = bytes_per_sector
	mov   	[bytes_per_sector],bx  					; Save byte per sector for later
	div  	bx										; AX = (number_of_entries * 32) / bytes_per_sector
	mov  	[rootdirlen],	ax

	; Get all useful data
	mov  	ax,			[buffer + OFF_NUMROTENT] 	; Root entry count
	mov  	[entry_cnt],ax
	ret

;
; Find and read a file in the FS
; Do not support sub directories
; IN  SI    Address of string that contains the path
;     ES:BX Data buffer
; OUT DI    Address of the entry in the FS
extern fs_read
fs_read:
	; Save data
	mov     [filename],	si
	push 	es 										; Save ES
	push 	bx 										; Save BX
	mov 	bx, 		0  							; Set ES to 0
	mov 	es, 		bx
	; Read root directory
	mov 	ax,			[rootdir]					; LBA = 0 (first sector)
	call    lba_to_chs
	mov     bx,			buffer
	mov     al,		 	[rootdirlen]				; Size of root directory
	mov 	dl, 		[diskID] 					; DL = Disk ID
	call  	disk_read 
	jc 		error.read_disk_failure
	mov  	di,			bx
	xor  	bx,			bx 							; Zero entry number
.check_entries:
	; Check if we finish
	cmp  	bx, 		[entry_cnt]					; Compare with number of entry
	jge		error.file_not_found					; If we check all, fail
	; Compare with what we search
	mov 	si, 		[filename] 					; The file name to search
	mov  	cx,			11							; Size of the string to compare (filename size)
	push 	di 										; Save DI (will be modified)
	repe 	cmpsb									; Compare the strings
	pop 	di 										; Restore DI
	je 		.file_found 							; If they are the same, we found it!
 	; Increment the counter
	inc  	bx  									; Increment the number of element readed
	add  	di,			32  						; Increment the entry pointer
	jmp 	.check_entries 							; Look back
.file_found:
	mov 	al,			[di + 0x0B]					; Read file flags
	bt  	ax,			4   						; Check if it is a directory
	jc 		error.entry_is_directory
	mov  	ax,			[di + 0x1A] 				; Load the file first cluster
	or  	ax,			0 							; Check if the first cluster is valid
	jz 		error.file_first_cluster_is_zero
	mov 	ebx,		[di + 0x1C]					; Load the size of the file
	; Calculate size in sector
	mov  	eax,  		[di+28]  					; EAX = size of file in byte
	xor  	ebx,		ebx 						; Clear EBX
	mov  	bx,			[bytes_per_sector]  		; BX = Number byte per sector
	div   	ebx 			  						; AL = size in sector, DX = remainder
	or  	edx, 		edx 						; Compare the remainder with 0
	jz		.load 									; If remainder is not 0, will grab last sector
	inc     ax 										; Increment the number of sector to grab
.load:
	; Getting the address of the file
	; first cluster = (kernel_cluster_number -2) * sectors_per_cluster + + cluster_number
	; WARN: May only work on floppy... 31 is a MAGIC NUMBER
	mov   	ax,			[di + 26] 					; AX = Logical cluster
	add  	ax,  		31 							; Add previous sectors
	call 	lba_to_chs
	; Read the file
	pop  	bx
	pop 	es
	mov 	dl, 	[diskID] 						; DL = Disk ID
	call 	disk_read								; Read the file in the buffer
	jc  	error.read_disk_failure
	ret

error:
.reset_failure:
	print	s_reset_failure
	jmp 	.error_number_exit

.get_info_failure:
	print	s_get_info_failure
	jmp 	.error_number_exit

.read_disk_failure:
	print 	s_read_disk_failure
	jmp  	.error_number_exit

.file_not_found:
	print  	s_file_not_found
	jmp 	.exit

.entry_is_directory:
	print  	s_entry_is_directory
	jmp 	.exit

.file_first_cluster_is_zero:
	print  	s_file_first_cluster_is_zero
	jmp 	.exit

.error_number_exit:
	mov 	al,			ah
	and  	ax,			0xF
	call 	puth
	mov  	al,			ENDL
	call 	putc
.exit:
	stc
	ret


;
; Convert an LBA address to a CHS address
; IN   AX  LDA address
; OUT  CL  Sector number
;      CH  Cylinder
;      DH  Head
lba_to_chs:
	push ax											; Save ax
	push dx 										; Save dl

	xor dx, dx										; DX = 0 
	div word [sectors_per_track]					; AX = LBA / SectorPerTrack
													; DX = LBA % SectorPerTrack
	inc dx 											; DX = LBA % SectorPerTrack + 1
	mov cx, dx										; Put it in CX
	
	xor dx, dx										; DX = 0
	div word [head_count]							; AX = LBA / SectorPerTrack / heads
													; DX = LBA % SectorPerTrack / heads

	mov dh, dl 										; DH is head
	shl ah, 6 							
	or  cl, ah 										; Put upper 2 bits of cylinder in CL

	pop ax 											; Restore dl
	mov dl, al
	pop ax 											; restore ax
	ret

section .data

s_reset_failure: db "[ERROR]: Fail to reset the disk: ", 0
s_get_info_failure: db "[ERROR]: Fail to get disk informations: ", 0
s_read_disk_failure: db "[ERROR]: Fail to read the disk: ", 0
s_file_not_found: db "[ERROR]: File not found!", 0xA, 0
s_entry_is_directory: db "[ERROR]: This is not a file but a directory", 0xA, 0
s_file_first_cluster_is_zero: db "[ERROR]: The file first cluster is 0", 0xA, 0
s_file_found: db "File is founded", 0xA, 0
ENDL equ 0xA

section .bss

filename:			resb 11
diskID: 			resb 1
sectors_per_track: 	resb 2
head_count: 		resb 2
entry_cnt: 			resb 2
rootdir: 			resb 2
rootdirlen: 		resb 2
bytes_per_sector:	resb 2
; Inner buffer
buffer: 			resb 10000