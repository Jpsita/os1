KERNEL_DIR = kernel
LOADER_DIR = loader

.PHONY: build

build:
	$(MAKE) -C $(LOADER_DIR)
	$(MAKE) -C $(KERNEL_DIR)

clean:
	rm -rf images/*
	$(MAKE) -C $(LOADER_DIR) clean
	$(MAKE) -C $(KERNEL_DIR) clean

configure:
	mkdir images
	$(MAKE) -C $(LOADER_DIR) configure
	$(MAKE) -C $(KERNEL_DIR) configure

pack: images/floppy.img loader/build/bootloader.bin
	dd if=./loader/build/bootloader.bin of=./images/floppy.img conv=notrunc bs=512
images/floppy.img:
	dd if=/dev/zero of=./images/floppy.img bs=512 count=2880
	dd if=./fat_0.hex of=./images/floppy.img conv=notrunc bs=512 seek=1
	dd if=./fat_0.hex of=./images/floppy.img conv=notrunc bs=512 seek=10


install: kernel/build/kernel.rnb 
	mount -o loop images/floppy.img ./mountpoint -t msdos
	cp kernel/build/kernel.rnb mountpoint/kernel.rnb
	mkdir mountpoint/test
	sleep 2
	umount ./mountpoint
uninstall:
	mount -o loop images/floppy.img ./mountpoint -t msdos
	rm mountpoint/kernel.rnb
	rm -r mountpoint/test
	sleep 2
	umount ./mountpoint
