CC = clang
FLAGS = -std=c17 -m32 -march=i386 -ffreestanding -fno-builtin -nostdinc -O1 -c kernel.c -o kernel.o
ASM = fasm
ASM_FILES = boot second_stage
BIN_FILES = $(ASM_FILES:%=%.bin)

run: $(ASM_FILES) makeC
	dd if=/dev/zero of=OS.bin bs=512 count=17
	cat $(BIN_FILES) test_font.bin kernel.bin > temp.bin
	dd if=temp.bin of=OS.bin conv=notrunc
	rm -rf temp.bin
	qemu-system-i386 -drive format=raw,file=OS.bin,if=ide,index=0,media=disk


$(ASM_FILES):
	nasm -f bin -o $@.bin $@.s
	nasm -f bin -o test_font.bin test_font.s
	nasm -felf32 -o idt.o idt.asm
	#fasm $@.s $@.bin
	#fasm test_font.s test_font.bin

makeC:
	$(CC) $(FLAGS)
	ld -m elf_i386 -Tkernel.ld kernel.o idt.o --oformat binary -o kernel.bin

clean:
	rm -rf *.o
	rm -rf *.bin
