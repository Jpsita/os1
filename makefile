KERNEL_DIR = kernel
LOADER_DIR = loader
SHELL_DIR = shell
LIBKERNEL_DIR = libKernel
ROOT_DIR=$(shell pwd)
ELFLOADER_DIR = elfloader
TESTEXEC_DIR = test_exec
MY_CFLAGS = -fno-stack-protector -nostdlib
TARGET = i686-elf
export

.PHONY: build

build:
	$(MAKE) -C $(LOADER_DIR)
	$(MAKE) -C $(KERNEL_DIR)
	$(MAKE) -C $(LIBKERNEL_DIR)
	$(MAKE) -C $(SHELL_DIR)
	$(MAKE) -C $(ELFLOADER_DIR)
	$(MAKE) -C $(TESTEXEC_DIR)
clean:
	rm -rf images/*
	$(MAKE) -C $(LOADER_DIR) clean
	$(MAKE) -C $(KERNEL_DIR) clean
	$(MAKE) -C $(LIBKERNEL_DIR) clean
	$(MAKE) -C $(SHELL_DIR) clean
	$(MAKE) -C $(ELFLOADER_DIR) clean
	$(MAKE) -C $(TESTEXEC_DIR) clean

configure:
	mkdir -p images
	$(MAKE) -C $(LOADER_DIR) configure
	$(MAKE) -C $(KERNEL_DIR) configure
	$(MAKE) -C $(LIBKERNEL_DIR) configure
	$(MAKE) -C $(SHELL_DIR) configure
	$(MAKE) -C $(ELFLOADER_DIR) configure
	$(MAKE) -C $(TESTEXEC_DIR) configure

pack: images/floppy.img loader/build/bootloader.bin 
	dd if=./loader/build/bootloader.bin of=./images/floppy.img conv=notrunc bs=512
images/floppy.img:
	dd if=/dev/zero of=./images/floppy.img bs=512 count=2880
	dd if=./fat_0.hex of=./images/floppy.img conv=notrunc bs=512 seek=1
	dd if=./fat_0.hex of=./images/floppy.img conv=notrunc bs=512 seek=10


install: kernel/build/kernel.rnb shell/build/shell.rnb
	mount -o loop images/floppy.img /mountpoint -t msdos
	cp kernel/build/kernel.rnb /mountpoint/kernel.rnb
	cp shell/build/shell.rnb /mountpoint/shell.rnb
	cp elfloader/build/elfl.rnb /mountpoint/elfl.rnb
	cp test_exec/build/test.elf /mountpoint/test.elf
	sleep 2
	umount /mountpoint
uninstall:
	mount -o loop images/floppy.img /mountpoint -t msdos
	rm /mountpoint/kernel.rnb
	rm /mountpoint/shell.rnb
	rm /mountpoint/elfl.rnb
	rm /mountpoint/test.elf
	sleep 2
	umount /mountpoint
