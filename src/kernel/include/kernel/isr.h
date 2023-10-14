#ifndef __KERNEL__ISR_H_
#define __KERNEL__ISR_H_

void isr_division_error(void);
void isr_debug(void);
void isr_nmi(void);
void isr_breakpoint(void);
void isr_overflow(void);
void isr_bound_range_exceeded(void);
void isr_invalid_opcode(void);
void isr_device_not_available(void);
void isr_double_fault(void);
void isr_coprocessor_segment_overrun(void);
void isr_invalid_tss(void);
void isr_segment_not_present(void);
void isr_stack_segment_fault(void);
void isr_general_protected_fault(void);
void isr_page_fault(void);
void isr_x87_floating_point_exception(void);
void isr_alignement_check(void);
void isr_machine_check(void);
void isr_smid_floating_point_exception(void);
void isr_virtualization_exception(void);
void isr_control_protected_exception(void);
void hypervisor_injection_exception(void);
void vmm_communication_exception(void);
void security_exception(void);

#endif