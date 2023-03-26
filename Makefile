# +===========================+
# | Author: Alikadev          |
# | Version: 0.1.2            |
# |                           |
# | (May only work in my PC)  |
# +===========================+

MAKEFLAGS += --no-print-directory

#
# DIRECTORIES
D_BUILD		:=	build
D_DEBUG 	:=	debug
D_SRC 		:=	src

D_SRC_BOOT	:=	$(D_SRC)/bootloader
D_SRC_KERN	:=	$(D_SRC)/kernel

#
# TOOLS
ASM 		?= 	nasm
MKFS_FAT	?=	mkfs.fat
DD 			?=	dd
MCOPY		?=	mcopy

#
# FILES
BOOT_OUT	:=	$(D_BUILD)/bootloader.bin
BOOT_SRC	:=	$(shell find $(D_SRC_BOOT) -name '*.asm')

KERN_OUT	:=	$(D_BUILD)/kernel.bin
KERN_SRC	:=	

IMAGE_OUT	:= 	AmethystOS.img

LINKER		?=	$(D_SRC)/linker.ld

#
# FLAGS
ASMFLAGS	?=	-f bin
LDFLAGS		?=	-T $(LINKER)

#
# IMPLEMENTATION
#
.PHONY: bootloader kernel image clean all

all: clean image
kernel: $(KERN_OUT)
bootloader:	$(BOOT_OUT)
image: $(IMAGE_OUT)
clean: 
	@rm -rf build/*
	@rm -f $(KERN_OUT)
	@rm -f $(BOOT_OUT)
	@rm -f $(IMAGE_OUT)

#
# BOOTLOADER
$(BOOT_OUT) : $(BOOT_SRC)
	@echo Building bootloader module
	@$(ASM) $(ASMFLAGS) -o $@ $<

#
# KERNEL
$(KERN_OUT) : $(KERN_SRC)
	@echo Building kernel module
	@touch $@
#	@$(ASM) $(ASMFLAGS) -o $@ $<

#
# OUTPUT
$(IMAGE_OUT) : $(KERN_OUT) $(BOOT_OUT)
	@echo Creating image output
	@$(DD) if=/dev/zero of=$(IMAGE_OUT) bs=512 count=2880
	@$(MKFS_FAT) -F 12 -n "AMETHYSTOS" $(IMAGE_OUT)
	@$(DD) if=$(BOOT_OUT) of=$(IMAGE_OUT) conv=notrunc
	@$(MCOPY) -i $(IMAGE_OUT) $(KERN_OUT) "::kernel.bin"
