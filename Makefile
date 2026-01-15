# OrionOS Makefile

all: os-image.bin

# 1. Compile the Bootloader
boot.bin: boot.asm
	nasm -f bin boot.asm -o boot.bin

# 2. Compile the Kernel Entry
entry.o: entry.asm
	nasm -f elf32 entry.asm -o entry.o

# 3. Compile the Kernel C code
kernel.o: kernel.c
	gcc -m32 -ffreestanding -c kernel.c -o kernel.o

# 4. Link everything into a flat binary kernel
kernel.bin: entry.o kernel.o
	ld -m elf_i386 -e _start -Ttext 0x1000 entry.o kernel.o --oformat binary -o kernel.bin

# 5. Build the final OS image
os-image.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > os-image.bin

# 6. Run the OS in QEMU
run: os-image.bin
	qemu-system-x86_64 -drive format=raw,file=os-image.bin

clean:
	rm -f *.bin *.o