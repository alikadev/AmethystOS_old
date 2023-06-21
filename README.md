# AmethystOS

## A simple OS for x86 architecture

_AmethystOS_ is a really simple _OS_ for `i386` (`x86`) computer architecture.

_AmethystOS_ will not be the next _Linux_... I don't think anyone will ever use it. It's just a fun project that I have!

# _OS_ main parts

This _OS_ contains (will contain) 2 main parts, the _Bootloader_ and the _Kernel_. The _userspace_ will be in the _Kernel_ because I want it like this.

## The _Bootloader_

__Warning__: The current _Bootloader_ do not work like this, I need to update the source code `:(`.

The bootloader will be in 2 places, the _Boot Sector_ and the `boot.bin` file in the root directory.

The _Bootloader_ in the _Boot Sector_ will setup the stack and load+run `boot.bin` file.

The _Bootloader_ in the `boot.bin` will setup the `GDT`, enable the `protected mode` (`32 bit mode`) and load the _Kernel_.

## The _Kernel_

__Warning__: The current _OS_ do not contains a _Kernel_.

The _Kernel_ will load the differents _drivers_, create _syscalls_ and put the user in the _userspace_.

# Dependancies

I will only try to build this _OS_ on _Linux_ and _MacOS_. If you can make it work on _Windows_, gg!

- `make`
- `nasm`
- `gcc` (x86-64 gcc or a cross-compiler)
- `ld` (x86-64 ld or a cross-linker)
- `mkfs.fat`
- `dd`
- `mcopy`

# Quick start

## Building the AmethystOS.img

The Makefile will build the bootloader (boots1 and boots2) and install them in the image.

``` shell
git clone https://github.com/alikadev/AmethystOS.git
cd AmethystOS
make
```

## Installing the OS

The _Device_ is the name of the _USB-Stick_ (or _Floppy_) you use.

``` shell
sudo dd if=AmethystOS.img of=/YOUR_DEVICE
```

## Run it

Restart your computer, go in the boot menu and choose the device. 

Do not boot in _UEFI_, __UEFI is not supported__.
