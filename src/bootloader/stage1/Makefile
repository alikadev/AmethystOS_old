D_BUILD	?=	build
OUT 	?=	boots1.bin

ASM 	?=	nasm

SRC 	:=	$(shell find . -name '*.asm')

ASMFLAGS	:=	-f bin
.PHONY: all clean

all: print always clean $(OUT)
print:
	@printf "Bootloader First Stage\n"

$(OUT) : $(SRC)
	@printf "\e[1;32m  Building\e[0m $(notdir $@)\n"
	@$(ASM) $(ASMFLAGS) -o $@ $<

clean:
	@printf "\e[1;32m  Cleaning\e[0m\n"
	@rm -f $(D_BUILD)/*

always:
	@mkdir -p $(D_BUILD)