
;
; Reset disk controller
; IN   DL  DiskID
; OUT  CF  Set on error
;      AH  Status
extern disk_reset

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
extern disk_get_info

;
; Read sectors from disk into memory
; IN   AL  number of sectors to read (must be nonzero)
;      CH  low eight bits of cylinder number
;      CL  (0-5) sector number 1-63 (bits 0-5)
;      CL  (6-7) high two bits of cylinder (bits 6-7, hard disk only)
;      DH  head number
;      DL  drive number (bit 7 set for hard disk)
;      ES:BX  data buffer
; OUT  CF  Set on error
;      AH  Status
extern disk_read