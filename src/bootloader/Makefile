D_BUILD	?=	build
ASM 	?=	nasm
OUT  	?=	$(D_BUILD)/bootloader.bin

SRC 	:=	$(shell find . -name '*.asm')

ASMFLAGS	:=	-f bin
.PHONY: all clean

all: print $(OUT)
print:
	@echo Building bootloader module

$(OUT) : $(SRC)
	@echo '  Building $<'
	@$(ASM) $(ASMFLAGS) -o $@ $<

clean:
	@rm $(OUT)