org 0x7E00
use16

jmp enter_pm

include "gdt.asm"

enter_pm:
	in al, 0x92
	or al, 0x02
	out 0x92, al
	
	cli
	lgdt [gdt_desc]
	
	mov eax, cr0
	or eax, 0x01
	mov cr0, eax

	jmp 0x08:start_pm


use32
start_pm:
	mov ax, dataseg
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	;jmp $

	jmp 0x08:0x5000
;jmp $

times 512 - ($ - $$) db 0
