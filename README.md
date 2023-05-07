# AmethystOS

## A simple OS for x86 architecture

This is a really (really) simple `OS` for `x86` computer.

I do it because I always liked to developpe in low level and I really wanted to do a simple OS (similar to `DOS` maybe).

# Current status

For the moment, the OS boot and search the `boots2.bin` file in the FS. Then, it will read the `boots2.bin` file and execute it. The second stage of the boot loader write a message.

I know, this is the best OS that you ever seen!

It will be updated in next days!

# Dependancies

```
POSIX (I will only try to build it on Linux and MacOS)
make
nasm
mkfs.fat
dd
mcopy
```

# Quick start

## Building the AmethystOS.img

The Makefile will build the bootloader (boots1 and boots2) and install them in the image.

``` shell
git clone https://github.com/alikadev/AmethystOS.git
cd AmethystOS
make all
```

## Installing the OS

The _Device_ is the name of the _USB-Stick_ (or _Floppy_) you use.

``` shell
sudo dd if=AmethystOS.img of=/dev/Device
```

## Run it

Restart your computer, go in the boot menu and choose the device. Do not boot in UEFI, __UEFI is not supported__.
