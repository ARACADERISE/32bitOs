[extern _idt]
[extern isr1_handler]

idtdesc:
	dw 4095
	dd _idt

global isr1

isr1:
	pusha
	call isr1_handler
	popa
	iretd

global loadIDT

loadIDT:
	call isr1_handler
	lidt [idtdesc]
	;sti
	ret
