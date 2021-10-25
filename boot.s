org 0x7C00
use16

mov byte [drive_num], dl

xor ax, ax
mov es, ax
mov ds, ax

cli
mov bp, 0x7C00
mov ss, ax
mov sp, bp
sti

mov ax, 0x07E0
mov es, ax
xor bx, bx

mov ah, 0x02
mov al, 0x01
mov ch, 0x00
mov cl, 0x02
mov dh, 0x00
mov dl, 0x80
int 0x13
jc failed

mov ax, 0x0200
mov es, ax
xor bx, bx

mov ah, 0x02
mov al, 0x02
mov ch, 0x00
mov cl, 0x03
mov dh, 0x00
mov dl, 0x80
int 0x13
jc failed

mov ax, 0x0500
mov es, ax
xor bx, bx

mov ah, 0x02
mov al, 0x08
mov ch, 0x00
mov cl, 0x05
mov dh, 0x00
mov dl, 0x80
int 0x13
jc failed

mov ax, 0x02
int 0x10

jmp 0x0:0x7E00

jmp $

drive_num: db 0x0

failed:
	mov ah, 0x0e
	mov al, 'F'
	int 0x10

	cli
	hlt

times 510 - ($ - $$) db 0
dw 0xaa55
