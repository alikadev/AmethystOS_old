extern syscall_list

global dispatch
dispatch:
	; Remove the upper 16 bits of eax
	push ax                        ; Save AX
	xor eax, eax                   ; Clear EAX
	pop ax                         ; Restore AX

	; Get the address of the syscall
	push ebx                       ; Save EBX
	mov ebx, 4                     ; Move 4 into EBX (4 bytes by address)
	mul ebx                        ; EAX *= EBX
	mov ebx, [syscall_list + eax]  ; EBX = Address of the syscall
	mov eax, ebx                   ; EAX = EBX
	pop ebx                        ; Restore EBX

	; Call the syscall
	call eax
	iret                           ; Return int