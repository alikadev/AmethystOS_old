D_BUILD	?=	build
D_SRC  	= 	src
OUT 	?=	boots2.bin

ASM 	?=	nasm

SRC 	:=	$(shell find $(D_SRC) -name '*.asm')

ASMFLAGS	:=	-f bin
.PHONY: all clean

all: print always $(OUT)
print:
	@echo Building bootloader second stage

$(OUT) : $(SRC)
	@echo '  Building $<'
	@$(ASM) $(ASMFLAGS) -o $@ $(SRC)

clean:
	@echo $(OUT)
	@rm $(D_BUILD)/*

always:
	@mkdir -p $(D_BUILD)