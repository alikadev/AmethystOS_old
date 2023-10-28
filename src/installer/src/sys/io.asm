[bits 32]

section .text

%macro real_mode 1
	; Stack
	mov [%1], esp        ; Save the stack
	mov esp,  0xFFFF     ; Set the new stack pointer

	; Put the data segments to the DATA16_SEG
	mov eax,  DATA16_SEG
	mov ds,   eax
	mov es,   eax
	mov fs,   eax
	mov gs,   eax
	mov ss,   eax

	; Apply segments
	jmp CODE16_SEG:%%.goto_real_mode_cr0

%%.goto_real_mode_cr0:
	; Exit protected mode
	mov eax,  cr0        ; Load CR0
	and al,   ~1         ; Exit protected mode
	mov cr0,  eax        ; Save CR0

	[bits 16]
	jmp 0:%%.goto_real_mode_real

%%.goto_real_mode_real:
	[bits 16]
	; Set the segment to the default
	xor ax,   ax
	mov ds,   ax
	mov es,   ax
	mov fs,   ax
	mov gs,   ax
	mov ss,   ax

	; Enable interrupts
	sti
%endmacro

%macro protected_mode 1
	[bits 16]
	; Disable interrupts
	cli

	; Enter protected mode
	mov eax, cr0         ; Load CR0
	or  eax, 1           ; Enter protected mode
	mov cr0, eax         ; Save protected mode

	jmp CODE_SEG:%%.protected
%%.protected:
	[bits 32]
	; Restore the segment to the DATA_SEG
	mov eax, DATA_SEG
	mov ds,  eax
	mov es,  eax
	mov fs,  eax
	mov gs,  eax
	mov ss,  eax

	; Restore the stack pointer
	mov esp, [%1]
%endmacro

%macro call_start 0 
	push ebp      ; save the base pointer value
	mov  ebp, esp ; Set the new base pointer value
%endmacro

%macro call_end 0 
	mov esp, ebp ; Deallocate deallocate variables (if exist)
	pop ebp      ; Restore the caller's base pointer value
%endmacro

global halt
halt:
	cli
	hlt

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

section .bss