[org 0x7E00]
use16

jmp setup_vesa

%include "gdt.asm"

setup_vesa:
		
	
	;sti

	xor ax, ax
	mov es, ax
	mov ah, 0x4F
	mov di, vbe_info_block
	int 0x10

	cmp ax, 0x4F
	jne err

	mov ax, word[vbe_info_block.video_modes]
	mov [offset], ax
	mov ax, word[vbe_info_block.video_modes+2]
	mov [t_segment], ax

	;mov ax, [t_segment]
	mov fs, ax
	mov si, [offset]

	jmp find_mode
errr:
	mov ah, 0x0e
	mov al, 'E'
	int 0x10

	cli
	hlt

find_mode:
	mov dx, [fs:si]
	add si, 2
	mov [offset], si
	mov [mode], dx

	cmp word[mode], 0xFFFF
	je end

	mov ax, 0x4F01
	mov cx, [mode]
	mov di, mode_info_block
	int 0x10

	cmp ax, 0x4F
	jne err

	mov ax, [width]
	cmp ax, [mode_info_block.width]
	jne next_mode

	mov ax, [height]
	cmp ax, [mode_info_block.height]
	jne next_mode
	
	mov al, [bpp]
	cmp al, [mode_info_block.bpp]
	jne next_mode
	
	mov ax, 0x4F02
	mov bx, [mode]
	or bx, 0x4000
	xor di, di
	int 0x10

	cmp ax, 0x4F
	jne err

	jmp enter_pm

end:
	mov ax, 0x0E4E
	int 0x10

	cli
	hlt
next_mode:
	mov ax, [t_segment]
	mov fs, ax
	mov si, [offset]
	jmp find_mode
err:
	mov ah, 0x0e
	mov al, 'F'
	int 0x10

	cli
	hlt

enter_pm:

	in al, 0x92
	or al, 0x02
	out 0x92, al
	
	cli
	lgdt [gdt_desc]
	
	mov eax, cr0
	or eax, 0x01
	mov cr0, eax

	jmp codeseg:start_pm


use32
start_pm:
	mov ax, dataseg
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov esp, 0x90000
	mov esi, mode_info_block
	mov edi, 0x4000
	mov ecx, 64
	rep movsd

	jmp codeseg:0x5000

hex_val: db '0x0000'

width:		dw 1024
height:		dw 768
bpp:		db 32
t_segment:	dw 0x0
offset:		dw 0x0
mode:		dw 0x0

times 512 - ($ - $$) db 0

vbe_info_block:
	.vbe_signature:			db 'VBE2'
	.vbe_version:			dw 0x0
	.oem_string_pointer:		dd 0x0
	.capabilities:			dd 0x0
	.video_modes:			dd 0x0
	.total_memory:			dw 0x0
	.oem_software_rev:		dw 0x0
	.oem_vendor_name_pointer:	dd 0x0
	.oem_product_name_pointer:	dd 0x0
	.oem_product_revision_pointer:	dd 0x0
	.reserved:	times 222 db 0
	.oem_data:	times 256 db 0

mode_info_block:
	.attributes:		dw 0x0	; 0-2 byte
	.window_a:		db 0x0	; 2-3 byte
	.window_b:		db 0x0	; 3-4 byte
	.granularity:		dw 0x0	; 4-6 byte
	.window_size:		dw 0x0	; 6-8 byte
	.segment_a:		dw 0x0	; 8-10 byte
	.segment_b:		dw 0x0	; 10-12 byte
	.win_func_ptr:		dd 0x0	; 12-16 byte
	.pitch:			dw 0x0	; 16-18 byte
	.width:			dw 0x0	; 18-20 byte
	.height:		dw 0x0	; 20-22 byte
	.w_char:		db 0x0	; 22-23 byte
	.y_char:		db 0x0	; 23-24 byte
	.planes:		db 0x0	; 24-25 byte
	.bpp:			db 0x0	; 25-26 byte
	.banks:			db 0x0	; 26-27 byte
	.memory_model:		db 0x0	; 27-28 byte
	.bank_size:		db 0x0	; 28-29 byte
	.image_pages:		db 0x0	; 29-30 byte
	.reserved1:		db 0x1	; 30-31 byte
	.red_mask_size:		db 0x0	; 31-32 byte
	.red_field_pos:		db 0x0	; 32-33 byte
	.green_mask_size:	db 0x0	; 33-34 byte
	.green_field_pos:	db 0x0	; 34-35 byte
	.blue_mask_size:	db 0x0	; 35-36 byte
	.blue_field_pos:	db 0x0	; 36-37 byte
	.reserved_mask_size:	db 0x0	; 37-38 byte
	.reserved_field_pos:	db 0x0	; 38-39 byte
	.direct_color_mode_info:db 0x0	; 39-40 byte
	.physical_base_ptr:	dd 0x0	; 40-44 byte
	.reserved2:		dd 0x0	; 44-48 byte
	.reserved3:		dw 0x0	; 48-50 byte
	.linear_bytes_psl:	dw 0x0	; 50-52 byte
	.bank_number_of_ip:	db 0x0	; 52-53 byte
	.linear_number_of_ip:	db 0x0	; 53-54 byte
	.linear_red_mask_size:	db 0x0	; 54-55 byte
	.linear_red_field_pos:	db 0x0	; 55-56 byte
	.linear_green_mask_size:db 0x0	; 56-57 byte
	.linear_green_field_pos:db 0x0	; 57-58 byte
	.linear_blue_mask_size: db 0x0	; 58-59 byte
	.linear_blue_field_pos: db 0x0	; 59-60 byte
	.linear_res_mask_sie:   db 0x0	; 60-61 byte
	.linear_res_field_pos:	db 0x0	; 61-62 byte
	.max_pixel_clock:	dd 0x0	; 62-66 byte
	.reserved4: times 190 db 0

times 1536 - ($ - $$) db 0
