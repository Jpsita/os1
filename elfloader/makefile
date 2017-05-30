INC=-I../include
CURR_DIR=$(shell pwd)

build: build/elfloader.rnb symbols/elfloader.sym

symbols/elfloader.sym: build/elfloader.elf
	objcopy --only-keep-debug "build/elfloader.elf" "symbols/elfloader.sym"
build/efloader.rnb: build/elfloader.elf
	objcopy -O binary "build/elfloader.elf" "build/elfloader.rnb" -g -S -R .eh_frame
configure:
	mkdir -p build
	mkdir -p smbols
clean:
	rm -rf build/*
	rm -rf symbols/*
