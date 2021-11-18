[org 0x3000]
[bits 32]

;
; File Manager:
;	Name, Memory Addr, Size(sectors)
;
db	'Bootloader ', 0x1, \
	'Bootloader2', 0x3, \
	'Font       ', 0x4, \
	'Kernel     ', 0x9

dw	0x7C00, \
	0x7E00, \
	0x2000, \
	0x5000,

times 512 - ($ - $$) db 0
