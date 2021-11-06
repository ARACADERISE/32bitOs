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

xor ax, ax
mov es, ax

mov bl, 0x02
mov di, 0x7E00

mov dx, 0x1F2
mov al, 0x03
out dx, al

mov dx, 0x1F3
mov al, 0x02
out dx, al

call load

mov bl, 0x03
mov di, 0x2000

mov dx, 0x1F2
mov al, 0x04
out dx, al

mov dx, 0x1F3
mov al, 0x05
out dx, al

call load

mov bl, 0x07
mov di, 0x5000

mov dx, 0x1F2
mov al, 0x08
out dx, al

mov dx, 0x1F3
mov al, 0x09
out dx, al

call load

mov ax, 0x02
int 0x10

mov dl, [drive_num]
mov [0x1500], dl

jmp 0x0000:0x7E00

load:
	mov dx, 0x1F6
	mov al, [drive_num]
	and al, 0x0F
	or al, 0x0A0
	out dx, ax

	mov dx, 0x1F4
	xor al, al
	out dx, al
	
	mov dx, 0x1F5
	xor al, al
	out dx, al

	mov dx, 0x1F7
	mov al, 0x20
	out dx, al

	.loop:
		in al, dx
		test al, 8
		je .loop

		mov cx, 256
		mov dx, 0x1F0
		rep insw

		mov dx, 0x3F6
		in al, dx
		in al, dx
		in al, dx
		in al, dx
		
		cmp bl, 0x0
		je .done
		
		dec bl
		mov dx, 0x1F7
		jmp .loop
.done:
	ret

drive_num: db 0x0

print:
	mov ah, 0x0e
	.loop:
		mov al, [si]
		cmp al, 0x0
		je ._end
		int 0x10
		inc si
		jmp .loop
	._end:
		ret


failed:
	mov si, [failed_msg]
	call print

	cli
	hlt

failed_msg: db 'Failed to load.'

times 510 - ($ - $$) db 0
dw 0xaa55
