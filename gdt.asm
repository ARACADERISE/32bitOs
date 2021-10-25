gdt_null:
	dq 0x0

gdt_code:
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 10011010b
	db 11001111b
	db 0x00

gdt_data:
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 10010010b
	db 11001111b
	db 0x00

gdt_end:

gdt_desc:
	gdt_size:
		dw gdt_end - gdt_null - 1
		dd gdt_null

codeseg equ gdt_code - gdt_null
dataseg equ gdt_data - gdt_null
