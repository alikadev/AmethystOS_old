[bits 32]

%include "sys/std.inc"

global disk_read_int13
disk_read_int13:
	[bits 32]
	call_start

	mov al, [ebp + 08]
	mov [drive], al

	mov ax, 0x10
	mov [dap.size], ax

	xor ax, ax
	mov [dap.zero], ax

	mov ax, [ebp + 12]
	mov [dap.count], ax

	mov ax, [ebp + 16]
	mov [dap.offset], ax

	mov ax, [ebp + 20]
	mov [dap.segment], ax

	mov eax, [ebp + 24]
	mov [dap.lba + 4], eax

	mov eax, [ebp + 28]
	mov [dap.lba], eax

	; Exit protected mode
	real_mode last_esp
	[bits 16]

	; Reset disk controller
	mov dl, [drive] ; drive
	mov ah, 0x00
	int 0x13
	jc  reset_failure

	; Read the disk
	mov si, dap
	mov dl, [drive]
	mov ah, 0x42
	int 0x13
	jnc .end

	; Read the disk
	mov si, dap
	mov dl, [drive]
	mov ah, 0x42
	int 0x13
	jnc .end

	; Read the disk
	mov si, dap
	mov dl, [drive]
	mov ah, 0x42
	int 0x13
	jc read_failure

.end:
	; Enter protected mode
	protected_mode last_esp
	[bits 32]
	call_end
	mov eax, 0
	ret

global disk_get_param_int13
disk_get_param_int13:
	[bits 32]
	call_start

	; Save the drive number
	mov al, [ebp + 08]
	mov [drive], al

	; Goto real mode
	real_mode last_esp
	[bits 16]

	; Reset disk controller
	mov dl, [drive] ; drive
	mov ah, 0x00
	int 0x13
	jc  reset_failure

	; get drive parameters
	mov dl, [drive] ; drive
	mov ah, 0x08
	int 0x13
	jc  get_params_failure

	; Get number of sector per track
	and cl,    0x3F     ; Remove high 2 bits
	xor ch,    ch       ; CH = 0 (CX = 00000000 00NNNNNN)
	mov [spt], cx       ; Save sector number

	; Get number of head
	inc  	dh
	mov     [head], dh

	; Goto protected mode
	protected_mode last_esp
	[bits 32]

	; Copy sector per track into spt pointer
	mov ebx, [ebp + 12]
	mov ax, [spt]
	mov word [ebx], ax

	; Copy head count into head pointer
	mov ebx, [ebp + 16]
	mov ax, [head]
	mov word [ebx], ax

	; Exit
	call_end
	xor eax, eax
	ret
	
; =============== ;
; === FAILURE === ;
; =============== ;
reset_failure:
	[bits 16]
	protected_mode last_esp
	[bits 32]
	call_end
	or  eax, (1 << 31)
	ret

read_failure:
	mov [status], al
	[bits 16]
	protected_mode last_esp
	[bits 32]
	call_end
	ret

get_params_failure:
	mov [status], al
	[bits 16]
	protected_mode last_esp
	[bits 32]
	call_end
	xor eax, eax
	mov al, [status]
	ret

section .buffer

last_esp: dd 0

align 32
dap:
 .size:    db 0x10
 .zero:    db 0x00
 .count:   dw 0
 .offset:  dw 0
 .segment: dw 0
 .lba:     dq 0

status:   db 0
drive:    db 0

head:     dw 0
spt:      dw 0