D_BUILD	?=	build
OUT 	?=	boots1.bin

ASM 	?=	nasm

SRC 	:=	$(shell find . -name '*.asm')

ASMFLAGS	:=	-f bin
.PHONY: all clean

all: print always clean $(OUT)
print:
	@echo "Bootloader First Stage"

$(OUT) : $(SRC)
	@echo "\e[1;32m  Building\e[0m" $(notdir $@)
	@$(ASM) $(ASMFLAGS) -o $@ $<

clean:
	@echo "\e[1;32m  Cleaning\e[0m"
	@rm -f $(D_BUILD)/*

always:
	@mkdir -p $(D_BUILD)