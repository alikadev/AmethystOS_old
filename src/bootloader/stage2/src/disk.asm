[bits 16]

section .text

global disk_reset
global disk_get_info
global disk_read

;
; Reset disk controller
; IN   DL  DiskID
; OUT  CF  Set on error
;      AH  Status
disk_reset:
	xor 	ah, 		ah
	int 	0x13
	ret

;
; Get informations about the disk
; IN   DL  DiskID
; OUT  CF  Set on error
;      AH  Status
;      BL  Drive type
;      CH  Low 8 bits of maximum cylinder number
;      CL  (5-0) Maximum number of sector
;      CL  (7-6) High 2 bits ofmaxumum number of cylinder
;      DH  Maximum number of head
;      DL  DiskID
disk_get_info:
	mov  	ah,			0x8
	int 	0x13
	ret

;
; Read sectors from disk into memory
; IN   AL  number of sectors to read (must be nonzero)
;      CL  (0-5) sector number 1-63 (bits 0-5)
;      CL  (6-7) high two bits of cylinder
;      CH  low eight bits of cylinder number
;      DH  head number
;      DL  drive number
;      ES:BX  data buffer
; OUT  CF  Set on error
;      AH  Status
disk_read:
	mov 	ah,			0x2
	int 	0x13
	ret

