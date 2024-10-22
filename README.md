# AmethystOS

## A simple OS for i386 architecture

_AmethystOS_ is a really simple _OS_ for `i386` (`x86`) computer architecture.

_AmethystOS_ will not be the next _Linux_, neither the next _DOS_... I don't think anyone will ever use it. It's just a fun project that I have!

# _OS_ main parts

This _OS_ contains 3 main parts, the _Bootloader_, the _Installer_ and the _Kernel_. The _userspace_ will be in the _Kernel_ because it is not a commercial _OS_.

## The _Bootloader_

The bootloader is separated in 2 part, the _First Stage_ and the _Second Stage_.

The _First Stage_ is the one on the bootloader. It is used to load and run the second stage of the bootloader from the disk. It also contains the filesystem header (`FAT12`).

The _Second Stage_ is the one in the filesystem root directory. The file is named `boot.bin`. Its job is to load the kernel, setup the `CPU` (`GDT`, `32bit Protected Mode`), setup the stack and run the _installer_ or the _kernel_.

## The _Installer_

The _installer_ is only present in the installation device (`USB-Stick`, `floppy`, ...). It is used to install the `OS`. The _Bootloader_ is partaged between the _installer_ and the _kernel_. This cause the OS to not have to reboot after installation.

## The _Kernel_

The _kernel_ initialize all the drivers and load the `kernel-shell`.

The _userspace_ is in the kernel because I don't want this `OS` to be too complex for the moment and I am still discovering a lot about `OSes`.

# Dependancies

I will only try to build this _OS_ on _Linux_ and _MacOS_. If you can make it work on _Windows_, good job!

- `make`
- `nasm`
- `gcc` (i386/x86-64 gcc or a cross-compiler)
- `ld` (i386/x86-64 ld or a cross-linker)
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

On _ARM Macs_ (_Silicon_), you should use a cross-compiler like `gcc-13` and `ld-13`.

``` shell
git clone https://github.com/alikadev/AmethystOS.git
cd AmethystOS
make CC=gcc-13 LD=ld-13
```

## Testing

You can test the `iso` with `qemu-system-i386`, `qemu-system-x86_64` and `bochs`. I don't publish my `bochs`'s config because it is broken... 

## Installing the OS

The `DISK` is the name of the _USB-Stick_ (or _Floppy_) you use. It uses `dd` so if you don't like it, you still have the binary.

``` shell
sudo make install DISK=/dev/sdx
```

### Run it

Restart your computer, go in the boot menu and choose the device. 

Do not boot in _UEFI_, __UEFI is not supported__.

The installer will present you the different devices where you can install the OS. Select one and the OS will be installed, loaded and run without any reboot!
