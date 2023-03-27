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
MKFS_FAT	?=	mkfs.fat
DD 			?=	dd
MCOPY		?=	mcopy

#
# FILES
# 
BOOTLOADER		:=	$(D_BUILD)/bootloader.bin
KERNEL		:=	$(D_BUILD)/kernel.bin

IMAGE_OUT	:= 	AmethystOS.img

#
# FLAGS
# 
ASMFLAGS	?=	-f bin -Wall
LDFLAGS		?=	-T $(LINKER)

#
# IMPLEMENTATION
#
.PHONY: bootloader kernel image clean all

all: always clean image
kernel: always $(KERNEL)
bootloader:	always $(BOOTLOADER)
image: always $(IMAGE_OUT)
clean: 
	@rm -rf build/*
	@rm -f $(KERNEL)
	@rm -f $(BOOTLOADER)
	@rm -f $(IMAGE_OUT)

#
# BOOTLOADER
$(BOOTLOADER) : $(BOOTLOADER_SRC)
	@$(MAKE) -C $(D_SRC)/bootloader all D_BUILD=$(abspath $(D_BUILD))

#
# KERNEL
$(KERNEL) : $(KERNEL_SRC)
	@$(MAKE) -C $(D_SRC)/kernel all D_BUILD=$(abspath $(D_BUILD))

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

always:
	@mkdir -p $(D_BUILD)