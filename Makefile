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
D_SRC 				:=	src
D_SRC_BOOT_S1		:=	$(D_SRC)/bootloader/stage1
D_SRC_BOOT_S2		:=	$(D_SRC)/bootloader/stage2
D_SRC_KERNEL0		:=	$(D_SRC)/kernel

D_BUILD				:=	build
D_BUILD_BOOT_S1		:=	$(D_BUILD)/boots1
D_BUILD_BOOT_S2		:=	$(D_BUILD)/boots2
D_BUILD_KERNEL0		:=	$(D_BUILD)/kernel0

#
# BUILDING TOOLS
# 
MKFS_FAT	?=	mkfs.fat
DD 			?=	dd
MCOPY		?=	mcopy

#
# FILES
# 
BOOT_S1		:=	$(D_BUILD)/boots1.bin
BOOT_S2		:=	$(D_BUILD)/boots2.bin
KERNEL0 	:=	$(D_BUILD)/kernel0.bin
IMAGE_OUT	:= 	AmethystOS.img

#
# FLAGS
# 
ASMFLAGS	?=	-f bin -Wall
LDFLAGS		?=	-T $(LINKER)

#
# IMPLEMENTATION
#
.PHONY: bootloader kernel0 image clean all install

all: always clean image
bootloader:	always $(BOOT_S1) $(BOOT_S2)
kernel0: always $(KERNEL0)
image: always $(IMAGE_OUT)
clean: 
	@rm -rf build/*
	@rm -f $(BOOT_S1)
	@rm -f $(IMAGE_OUT)

#
# BOOTLOADER STAGE 1
$(BOOT_S1) : $(D_SRC_BOOT_S1)
	@$(MAKE) -C $< all D_BUILD=$(abspath $(D_BUILD_BOOT_S1)) OUT=$(abspath $(BOOT_S1))
	@echo

#
# BOOTLOADER STAGE 2
$(BOOT_S2) : $(D_SRC_BOOT_S2)
	@$(MAKE) -C $< all D_BUILD=$(abspath $(D_BUILD_BOOT_S2)) OUT=$(abspath $(BOOT_S2))
	@echo

#
# KENREL FILE N°0
$(KERNEL0) : $(D_SRC_KERNEL0)
	@$(MAKE) -C $< all D_BUILD=$(abspath $(D_BUILD_KERNEL0)) OUT=$(abspath $(KERNEL0))
	@echo

#
# OUTPUT
$(IMAGE_OUT) : bootloader kernel0
	@printf "Burnable Image\n"
	@printf "\e[1;32m  Creating\e[0m output image\n"
	@$(DD) if=/dev/zero of=$(IMAGE_OUT) bs=512 count=2880 status=none
	@printf "\e[1;32m  Writing\e[0m FS\n"
	@$(MKFS_FAT) -F 12 -n "AMETHYSTOS" $(IMAGE_OUT) >  /dev/null
	@printf "\e[1;32m  Writing\e[0m boot sector\n"
	@$(DD) if=$(BOOT_S1) of=$(IMAGE_OUT) conv=notrunc status=none
	@printf "\e[1;32m  Copying\e[0m boots2.bin\n"
	@$(MCOPY) -i $(IMAGE_OUT) $(BOOT_S2) "::boot.bin"
	@$(MCOPY) -i $(IMAGE_OUT) $(KERNEL0) "::kern0.bin"

install: $(DISK)
	sudo dd if=$(IMAGE_OUT) of=$(DISK) bs=512

always:
	@mkdir -p $(D_BUILD)