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

D_BUILD				:=	build
D_BUILD_BOOT_S1		:=	$(D_BUILD)/boots1
D_BUILD_BOOT_S2		:=	$(D_BUILD)/boots2

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

IMAGE_OUT	:= 	AmethystOS.img

#
# FLAGS
# 
ASMFLAGS	?=	-f bin -Wall
LDFLAGS		?=	-T $(LINKER)

#
# IMPLEMENTATION
#
.PHONY: bootloader kernel image clean all install

all: always clean image
bootloader:	always $(BOOT_S1) $(BOOT_S2)
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
# OUTPUT
$(IMAGE_OUT) : bootloader
	@echo "Burnable Image"
	@echo "\e[1;32m  Creating\e[0m output image"
	@$(DD) if=/dev/zero of=$(IMAGE_OUT) bs=512 count=2880 status=none
	@echo "\e[1;32m  Writing\e[0m FS"
	@$(MKFS_FAT) -F 12 -n "AMETHYSTOS" $(IMAGE_OUT) >  /dev/null
	@echo "\e[1;32m  Writing\e[0m boot sector"
	@$(DD) if=$(BOOT_S1) of=$(IMAGE_OUT) conv=notrunc status=none
	@echo "\e[1;32m  Copying\e[0m boots2.bin"
	@$(MCOPY) -i $(IMAGE_OUT) $(BOOT_S2) "::boots2.bin"

install: $(DISK)

always:
	@mkdir -p $(D_BUILD)