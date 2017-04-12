# OS1
For fun OS project. rarely developed but highly informative

## Goals
 - New Name (OS1 is rubbish)
 - Practice ASM and C
 - Learn about low-level device interaction
 - Fill spare time(?)

## Building

To build the system you need:
The Netwide Assembler (__NASM__) to build ASM files,

The GNU Compiler Collection (__GCC__) to build C files,

The GNU Binutils for the Linker (__ld__) and objcopy,

__GNU Make__ is the build system in use

To configure your workspace run

````
 make configure
````

__WARNING:__ This will not check if you have the prerequisites installed

To build the binary files run

````
 make
````

To build a bootable image file (images/floppy.img) run

````
 make pack
 (sudo) make install
````

_Note:_ You may need SuperUser permissions to mount and unmount the image. This is needed to make linux handle FAT12 copying of files.

## Running and Debugging

Currently, the only supported emulator is QEMU.

Install it to try the system.

To run the OS use the included _run.sh_ file.

To debug the OS, the GNU Debugger (GDB) is also needed.

The included _debug.sh_ script uses GDB and TMUX, however, symbol files in DWARF format are automatically built and are available for manual debugging in these directories:

__loader/build__

__kernel/build__

## License

__LIMEWIRE License__ v4.9

The "Do what you want cause a pirate is free" License:

"You are a Pirate!"

(Do what you want with this code, I don't care :) )
