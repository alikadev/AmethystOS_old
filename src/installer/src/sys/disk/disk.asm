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
	jc  .reset_failure

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
	jc .read_failure

.end:
	; Enter protected mode
	protected_mode last_esp
	[bits 32]
	call_end
	mov eax, 0
	ret

.reset_failure:
	[bits 16]
	protected_mode last_esp
	[bits 32]
	call_end
	or  eax, (1 << 30)
	ret

.read_failure:
	mov [status], al
	[bits 16]
	protected_mode last_esp
	[bits 32]
	call_end

	ret
	
section .buffer
align 32
dap:
 .size:    db 0x10
 .zero:    db 0x00
 .count:   dw 0
 .offset:  dw 0
 .segment: dw 0
 .lba:     dq 0

last_esp: dd 0
status:   db 0
drive:    db 0