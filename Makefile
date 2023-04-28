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
D_SRC 		:=	src
D_SRC_BOOT		:=	$(D_SRC)/bootloader/stage1

D_BUILD		:=	build
D_BUILD_BOOT	:=	$(D_BUILD)/boot

#
# BUILDING TOOLS
# 
MKFS_FAT	?=	mkfs.fat
DD 			?=	dd
MCOPY		?=	mcopy

#
# FILES
# 
BOOT		:=	$(D_BUILD)/boot.bin

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
bootloader:	always $(BOOT)
image: always $(IMAGE_OUT)
clean: 
	@rm -rf build/*
	@rm -f $(BOOT)
	@rm -f $(IMAGE_OUT)

#
# BOOTLOADER STAGE 1
$(BOOT) : $(D_SRC_BOOT)
	@$(MAKE) -C $< all D_BUILD=$(abspath $(D_BUILD_BOOT)) OUT=$(abspath $(BOOT))
	@echo

#
# OUTPUT
$(IMAGE_OUT) : $(BOOT)
	@echo Creating the image output
	@$(DD) if=/dev/zero of=$(IMAGE_OUT) bs=512 count=2880 status=none
	@echo Adding fs to the image
	@$(MKFS_FAT) -F 12 -n "AMETHYSTOS" $(IMAGE_OUT)
	@echo Copying the bootloader in the image
	@$(DD) if=$(BOOT) of=$(IMAGE_OUT) conv=notrunc status=none
	@echo Adding files in the image disk
	@$(MCOPY) -i $(IMAGE_OUT) test.txt "::test1.txt"
	@$(MCOPY) -i $(IMAGE_OUT) test.txt "::test2.txt"
	@$(MCOPY) -i $(IMAGE_OUT) test.txt "::test3.txt"
	@$(MCOPY) -i $(IMAGE_OUT) test.txt "::test4.txt"

install: $(DISK)

always:
	@mkdir -p $(D_BUILD)