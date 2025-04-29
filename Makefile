main:
	i686-elf-gcc -c kernel/kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	i686-elf-gcc -T linker.ld -o keystone.bin -ffreestanding -O2 -nostdlib boot.s kernel.o -lgcc
grub:
	cp keystone.bin Grub/boot/keystone.bin
	grub-mkrescue -o keystone.iso Grub
qemu:
	qemu-system-i386 -cdrom keystone.iso
clean:
	rm -f keystone.bin keystone.iso kernel.o Grub/boot/keystone.bin kernel.o
all: clean main
dev: clean main grub qemu