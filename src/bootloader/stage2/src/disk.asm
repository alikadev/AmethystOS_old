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

