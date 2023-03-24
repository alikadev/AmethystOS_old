D_BUILD	?=	build
OUT 	?=	boot1.bin

ASM 	?=	nasm

SRC 	:=	$(shell find . -name '*.asm')

ASMFLAGS	:=	-f bin
.PHONY: all clean

all: print always $(OUT)
print:
	@echo Building bootloader first stage

$(OUT) : $(SRC)
	@echo '  Building $<'
	@$(ASM) $(ASMFLAGS) -o $@ $<

clean:
	@echo $(OUT)
	@rm $(D_BUILD)/*

always:
	@mkdir -p $(D_BUILD)