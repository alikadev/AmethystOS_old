D_BUILD		?=	build
D_SRC		:=	 src
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
CFLAGS	 	:=	-nolibc -ffreestanding -fno-pie -c -m32 -std=c11
LDFLAGS		:=	-T $(LINKER)

.PHONY: all clean

all: always print clean $(OUT)

print:
	@echo "Bootloader Second Stage"

$(OUT) : $(OBJ_ASM) $(OBJ_C)
	@echo "\e[1;32m  Building\e[0m" $(notdir $@)
	@$(LD) -o $@ $(LDFLAGS) $(OBJ_ASM) $(OBJ_C)

$(D_BUILD)/%.asm.o: $(D_SRC)/%.asm
	@echo "\e[1;32m  Compiling\e[0m" $(notdir $<)
	@$(ASM) -o $@ $(ASMFLAGS) $<

$(D_BUILD)/%.c.o: $(D_SRC)/%.c
	@echo "\e[1;32m  Compiling\e[0m" $(notdir $<)
	@$(CC) -o $@ $(CFLAGS) $<

clean:
	@echo "\e[1;32m  Cleaning\e[0m"
	@rm -f $(D_BUILD)/*

always:
	@mkdir -p $(D_BUILD)