D_BUILD		?=	build
D_SRC		:=	src
D_INCLUDE 	:=	include
OUT			?=	boots2.bin

LINKER		:=	linker.ld

ASM			?=	nasm
CC 			?=	gcc
LD 			?=	ld

SRC_ASM		:=	$(shell find $(D_SRC) -name '*.asm')
OBJ_ASM		:=	$(subst $(D_SRC), $(D_BUILD), $(SRC_ASM:%.asm=%.asm.o))

SRC_C		:=	$(shell find $(D_SRC) -name '*.c')
OBJ_C		:=	$(subst $(D_SRC), $(D_BUILD), $(SRC_C:%.c=%.c.o))

ASMFLAGS 	:=	-f elf32
CFLAGS	 	:=	-nolibc -ffreestanding -fno-pie -c -m32 -std=c99 -I$(D_INCLUDE)
LDFLAGS		:=	-T $(LINKER)

.PHONY: all clean

all: always print clean $(OUT)

print:
	@printf "Kernel\n"

$(OUT) : $(OBJ_ASM) $(OBJ_C)
	@printf "\e[1;32m  Building\e[0m $(notdir $@)\n"
	@$(LD) -o $@ $(LDFLAGS) $(OBJ_ASM) $(OBJ_C)

$(D_BUILD)/%.asm.o: $(D_SRC)/%.asm
	@mkdir -p $(dir $@)
	@printf "\e[1;32m  Compiling\e[0m $(notdir $<)\n"
	@$(ASM) -o $@ $(ASMFLAGS) $<

$(D_BUILD)/%.c.o: $(D_SRC)/%.c
	@mkdir -p $(dir $@)
	@printf "\e[1;32m  Compiling\e[0m $(notdir $<)\n"
	@$(CC) -o $@ $(CFLAGS) $<

clean:
	@printf "\e[1;32m  Cleaning\e[0m\n"
	@rm -f $(D_BUILD)/*

always:
	@mkdir -p $(D_BUILD)