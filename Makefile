CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
AS = nasm
ASFLAGS = -f elf32

all: Vayu-x86_64.iso

src/boot.o: src/boot.s
	$(AS) $(ASFLAGS) src/boot.s -o src/boot.o

src/kernel.o: src/kernel.c
	$(CC) $(CFLAGS) -c src/kernel.c -o src/kernel.o

iso/boot/vayu.bin: src/boot.o src/kernel.o linker.ld
	$(CC) -m32 -T linker.ld -o iso/boot/vayu.bin -nostdlib src/boot.o src/kernel.o -lgcc

Vayu-x86_64.iso: boot/vayu.bin
	grub-mkrescue -o Vayu-x86_64.iso iso

clean:
	rm -f src/*.o iso/boot/vayu.bin Vayu-x86_64.iso
