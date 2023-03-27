D_BUILD 	?=	build
D_KERNEL	:=	$(D_BUILD)/kernel
D_SRC 		:=	src
D_INC 		:=	include

ASM 		?=	nasm
LD			?= 	ld

OUT  	 	=	$(D_BUILD)/kernel.bin

LINKER		?=	linker.ld

SRC			:=	$(shell find $(D_SRC) -name '*.asm')
OBJ			:=	$(subst $(D_SRC), $(D_KERNEL), $(SRC:%.asm=%.o))

ASMFLAGS 	:=  -f elf32 -I$(D_INC)
LDFLAGS		:=	-T $(LINKER) 

.PHONY: all clean

all: always print clean $(OUT)
print:
	@echo Building kernel module

#
# Output
$(OUT): $(OBJ)
	@echo '  Linking objects to kernel.bin'
	@$(LD) $(LDFLAGS) -o $@ $(OBJ)

#
# Objects
$(D_KERNEL)/%.o: $(D_SRC)/%.asm
	@echo '  Building $<'
	@$(ASM) $(ASMFLAGS) -o $@ $< 

#
# Clean
clean:
	@rm -f $(OUT)
	@rm -rf $(D_KERNEL)/*

#
# Always
always:
	@mkdir $(D_KERNEL)