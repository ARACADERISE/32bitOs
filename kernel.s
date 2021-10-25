use32

org 0x2000

mov [0xb8000], byte 'H'
mov [0xb8002], byte 'E'

jmp $

times 1024 - ($ - $$) db 0
