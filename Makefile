CC = clang
FLAGS = -std=c17 -m32 -march=i386 -ffreestanding -fno-builtin -nostdinc -O1 -c kernel.c -o kernel.o
ASM = fasm
ASM_FILES = boot second_stage
BIN_FILES = $(ASM_FILES:%=%.bin)

run: $(ASM_FILES) makeC
	dd if=/dev/zero of=OS.bin bs=512 count=3000
	cat $(BIN_FILES) kernel.bin test_font.bin > temp.bin
	dd if=temp.bin of=OS.bin conv=notrunc
	rm -rf temp.bin
	qemu-system-i386 -drive format=raw,file=OS.bin,if=ide,index=0,media=disk


$(ASM_FILES):
	fasm $@.s $@.bin
	fasm test_font.s test_font.bin

makeC:
	$(CC) $(FLAGS)
	ld -m elf_i386 -Tkernel.ld kernel.o --oformat binary -o kernel.bin

clean:
	rm -rf *.o
	rm -rf *.bin