#include <stdio.h>
#include <sys/io.h>
#include <kernel/isr.h>
#include <stddef.h>

#define INFO_BG_COLOR 0x00
#define INFO_FG_COLOR 0x0F

#define FATAL_BG_COLOR 0x00
#define FATAL_FG_COLOR 0x28


void *isr_list[] =
{
	isr_division_error,                // 0x00
	isr_debug,                         // 0x01
	isr_nmi,                           // 0x02
	isr_breakpoint,                    // 0x03
	isr_overflow,                      // 0x04
	isr_bound_range_exceeded,          // 0x05
	isr_invalid_opcode,                // 0x06
	isr_device_not_available,          // 0x07
	isr_double_fault,                  // 0x08
	isr_coprocessor_segment_overrun,   // 0x09
	isr_invalid_tss,                   // 0x0A
	isr_segment_not_present,           // 0x0B
	isr_stack_segment_fault,           // 0x0C
	isr_general_protected_fault,       // 0x0D
	isr_page_fault,                    // 0x0E
	NULL,                              // 0x0F
	isr_x87_floating_point_exception,  // 0x10
	isr_alignement_check,              // 0x11
	isr_machine_check,                 // 0x12
	isr_smid_floating_point_exception, // 0x13
	isr_virtualization_exception,      // 0x14
	isr_control_protected_exception,   // 0x15
	NULL,                              // 0x16
	NULL,                              // 0x17
	NULL,                              // 0x18
	NULL,                              // 0x19
	NULL,                              // 0x1A
	NULL,                              // 0x1B
	hypervisor_injection_exception,    // 0x1C
	vmm_communication_exception,       // 0x1D
	security_exception,                // 0x1E
	NULL,                              // 0x1F
};

void isr_division_error(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nDivision by 0, halt");
	halt();
}

void isr_debug(void)
{
	set_bg_color(INFO_BG_COLOR);
	set_fg_color(INFO_FG_COLOR);
	printf("\nDebug is not supported");
	return_interrupt();
}

void isr_nmi(void)
{
	set_bg_color(INFO_BG_COLOR);
	set_fg_color(INFO_FG_COLOR);
	printf("\nNMI are not supported");
	return_interrupt();
}

void isr_breakpoint(void)
{
	set_bg_color(INFO_BG_COLOR);
	set_fg_color(INFO_FG_COLOR);
	printf("\nBreakpoint are not supported!");
	return_interrupt();
}

void isr_overflow(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nOverflow, halt");
	halt();
}

void isr_bound_range_exceeded(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nBound range exceeded, halt");
	halt();
}

void isr_invalid_opcode(void)
{
	set_bg_color(INFO_BG_COLOR);
	set_fg_color(INFO_FG_COLOR);
	printf("\nInvalid opcode");
	return_interrupt();
}

void isr_device_not_available(void)
{
	set_bg_color(INFO_BG_COLOR);
	set_fg_color(INFO_FG_COLOR);
	printf("\nDevice is not available");
	return_interrupt();
}

void isr_double_fault(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\ndouble fault, halt");
	halt();
}

void isr_coprocessor_segment_overrun(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nCoprocessor segment overrun, halt");
	halt();
}

void isr_invalid_tss(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nInvalid TSS, halt");
	halt();
}

void isr_segment_not_present(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nSegment is not present, halt");
	halt();
}

void isr_stack_segment_fault(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nStack segment fault, halt");
	halt();
}

void isr_general_protected_fault(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nInvalid protected fault, halt");
	halt();
}

void isr_page_fault(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nPage fault, halt");
	halt();
}

void isr_x87_floating_point_exception(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nX87 floating-point exception, halt");
	halt();
}

void isr_alignement_check(void)
{
	set_bg_color(INFO_BG_COLOR);
	set_fg_color(INFO_FG_COLOR);
	printf("\nAlignement check");
	return_interrupt();
}

void isr_machine_check(void)
{
	set_bg_color(INFO_BG_COLOR);
	set_fg_color(INFO_FG_COLOR);
	printf("\nMachine check");
	return_interrupt();
}

void isr_smid_floating_point_exception(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nSMID floating-point exception, halt");
	halt();
}

void isr_virtualization_exception(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nVirtualization exception, halt");
	halt();
}

void isr_control_protected_exception(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\ncontrol protected exception, halt");
	halt();
}

void hypervisor_injection_exception(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nhypervisor injection exception, halt");
	halt();
}

void vmm_communication_exception(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nVMM communication exception, halt");
	halt();
}

void security_exception(void)
{
	set_bg_color(FATAL_BG_COLOR);
	set_fg_color(FATAL_FG_COLOR);
	printf("\nSecurity exception, halt");
	halt();
}

