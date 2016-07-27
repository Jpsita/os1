#!/bin/bash

echo Running...
qemu-system-i386 -fda bootload.bin -m 512M
