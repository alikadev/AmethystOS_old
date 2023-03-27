%define CR  0x0D
%define LF  0x0A
%define DEL 0x7F


; Print char in TTY
; @param AH The char to draw
extern putc

; Print string in TTY
; @param SI The address of the string
extern puts

; Get string from keyboard
; @return AH The scancode of the key
; @return AL The ASCII code of the key
extern getc

; Get a string from the user
; @param di The buffer
; @param ax The size
extern gets