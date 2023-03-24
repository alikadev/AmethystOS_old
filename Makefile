# +===========================+
# | Author: Alikadev          |
# | Version: 0.1.2            |
# |                           |
# | (May only work in my PC)  |
# +===========================+

MAKEFLAGS += --no-print-directory


# Project specific
D_INC		:= 	include
D_BUILD		:=	build
D_DEBUG 	:=	debug
D_SRC 		:=	src

# Compilator used
ASM 		?= 	nasm
LD 			?=	ld
COPY		?=	cp

OUT			?= 	amethystos.bin

LINKER		?=	$(D_SRC)/linker.ld
SRCASM		:=	$(shell find $(D_SRC) -name '*.asm')
OBJS		:=	$(subst $(D_SRC), $(D_BUILD), $(SRCASM:%.asm=%.bin))

ASMFLAGS	?=	-f bin
LDFLAGS		?=	-T $(LINKER)

# Implementation
default: build
all: clean default
build: $(OBJS)

help:
	@echo "Not already implemented"
	@exit 0

clean:
	@mkdir -p $(D_BUILD)
	@rm -rf $(D_BUILD)/*

$(D_BUILD)/%.bin: $(D_SRC)/%.asm
	@mkdir -p $(dir $@)
	@echo Building $<
	@$(ASM) $(ASMFLAGS) $< -o $@

$(OUT): $(OBJS)
	@echo Copying the bootloader to the ouput file
	@$(COPY) $(OBJS) $(OUT) 

