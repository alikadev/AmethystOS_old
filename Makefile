# +===========================+
# | Author: Alikadev          |
# | Version: 0.1.2            |
# |                           |
# | (May only work in my PC)  |
# +===========================+

MAKEFLAGS += --no-print-directory

# ================== #
# Project Specifiers #
# ================== #

#
# DIRECTORIES
# 
D_BUILD		:=	build
D_DEBUG 	:=	debug
D_SRC 		:=	src

D_SRC_BOOT	:=	$(D_SRC)/bootloader
D_SRC_KERN	:=	$(D_SRC)/kernel

#
# BUILDING TOOLS
# 
ASM 		?= 	nasm
MKFS_FAT	?=	mkfs.fat
DD 			?=	dd
MCOPY		?=	mcopy

#
# FILES
# 
BOOTLOADER		:=	$(D_BUILD)/bootloader.bin
BOOTLOADER_SRC	:=	$(shell find $(D_SRC_BOOT) -name '*.asm')

KERNEL		:=	$(D_BUILD)/kernel.bin
KERNEL_SRC	:=	$(shell find $(D_SRC_KERN) -name '*.asm')

LINKER		?=	$(D_SRC)/linker.ld

IMAGE_OUT	:= 	AmethystOS.img

#
# FLAGS
# 
ASMFLAGS	?=	-f bin
LDFLAGS		?=	-T $(LINKER)

#
# IMPLEMENTATION
#
.PHONY: bootloader kernel image clean all

all: clean image
kernel: $(KERNEL)
bootloader:	$(BOOTLOADER)
image: $(IMAGE_OUT)
clean: 
	@rm -rf build/*
	@rm -f $(KERNEL)
	@rm -f $(BOOTLOADER)
	@rm -f $(IMAGE_OUT)

#
# BOOTLOADER
$(BOOTLOADER) : $(BOOTLOADER_SRC)
	@echo Building bootloader module
	@$(ASM) $(ASMFLAGS) -o $@ $<

#
# KERNEL
$(KERNEL) : $(KERNEL_SRC)
	@echo Building kernel module
	@$(ASM) $(ASMFLAGS) -o $@ $<

#
# OUTPUT
$(IMAGE_OUT) : $(BOOTLOADER) $(KERNEL)
	@echo Creating the image output
	@$(DD) if=/dev/zero of=$(IMAGE_OUT) bs=512 count=2880 status=none
	@echo Adding fs to the image
	@$(MKFS_FAT) -F 12 -n "AMETHYSTOS" $(IMAGE_OUT)
	@echo Copying the bootloader in the image
	@$(DD) if=$(BOOTLOADER) of=$(IMAGE_OUT) conv=notrunc status=none
	@echo Copying the kernel binary in the image
	@$(MCOPY) -i $(IMAGE_OUT) $(KERNEL) "::kernel.bin"
