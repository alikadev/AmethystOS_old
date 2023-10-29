
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