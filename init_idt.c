/*
 * This file was added by Shanay.
 * It initializes the idt and is called in kernel.c just before the pic is initialized.
 */
#include "init_idt.h"

/* 
 *   init_idt()
 *   DESCRIPTION: Initialise the idt by filling in the appropriate entries (gates).
 *                
 * 		              
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: sets up the exception handler pointers, the keyboard handler and other interrupt handlers.
 */
void
init_idt(){

	//sets each entry for the exceptions
		
	SET_IDT_ENTRY(idt[0], handle_division_by_zero);
	SET_IDT_ENTRY(idt[1], handle_debug);
	SET_IDT_ENTRY(idt[2], handle_non_maskable_int);
	SET_IDT_ENTRY(idt[3], handle_breakpoint);
	SET_IDT_ENTRY(idt[4], handle_overflow);
	SET_IDT_ENTRY(idt[5], handle_bound_range_exceeded);
	SET_IDT_ENTRY(idt[6], handle_invalid_opcode);
	SET_IDT_ENTRY(idt[7], handle_device_not_available);
	SET_IDT_ENTRY(idt[8], handle_double_fault);
	SET_IDT_ENTRY(idt[9], handle_coprocessor_segment_overrun);
	SET_IDT_ENTRY(idt[10], handle_invalid_tss);
	SET_IDT_ENTRY(idt[11], handle_segment_not_present);
	SET_IDT_ENTRY(idt[12], handle_stack_segment_fault);
	SET_IDT_ENTRY(idt[13], handle_general_protection_fault);
	SET_IDT_ENTRY(idt[14], handle_page_fault);
	SET_IDT_ENTRY(idt[15], handle_reserved);
	SET_IDT_ENTRY(idt[16], handle_floating_point);
	SET_IDT_ENTRY(idt[17], handle_alignemnt_check);
	SET_IDT_ENTRY(idt[18], handle_machine_check);
	SET_IDT_ENTRY(idt[19], handle_SIMD_floating_point);
	SET_IDT_ENTRY(idt[20], handle_virt_exception);
	SET_IDT_ENTRY(idt[21], handle_reserved);
	SET_IDT_ENTRY(idt[22], handle_reserved);
	SET_IDT_ENTRY(idt[23], handle_reserved);
	SET_IDT_ENTRY(idt[24], handle_reserved);
	SET_IDT_ENTRY(idt[25], handle_reserved);
	SET_IDT_ENTRY(idt[26], handle_reserved);
	SET_IDT_ENTRY(idt[27], handle_reserved);
	SET_IDT_ENTRY(idt[28], handle_reserved);
	SET_IDT_ENTRY(idt[29], handle_reserved);
	SET_IDT_ENTRY(idt[30], handle_security_exception);
	SET_IDT_ENTRY(idt[31], handle_reserved);


	int i;
	for(i = 0; i <IDT_SIZE ; i++) {
		
		idt[i].dpl = 0;
		idt[i].present = 1;
		idt[i].seg_selector = KERNEL_CS;
		idt[i].reserved4 = 0;
		idt[i].reserved0 = 0;
		idt[i].reserved1 = 1;
		idt[i].reserved2 = 1;
		idt[i].size = 1;
		if(i>31){				//setting all the entries after exceptions to another handler and sets the gate to the appropriate value
			SET_IDT_ENTRY(idt[i], handle_all_others);
			idt[i].present = 0;
			idt[i].reserved3 = 0;
		}
		else {
			idt[i].reserved3 = 1;
		}
		
		
	}

	

	
	
	SET_IDT_ENTRY(idt[KEYBOARD_INDEX], keyb_handler);
	idt[KEYBOARD_INDEX].present = 1;
	
	
	SET_IDT_ENTRY(idt[RTC_INDEX], rtclock_handler);
	idt[RTC_INDEX].present = 1;

	SET_IDT_ENTRY(idt[PIT_INDEX], pit_handler);
	idt[PIT_INDEX].present = 1;
	idt[PIT_INDEX].dpl = 3;
	
	SET_IDT_ENTRY(idt[SYS_CALL], system_call_handler);
	idt[SYS_CALL].dpl = 3;
	idt[SYS_CALL].present = 1;
	idt[SYS_CALL].reserved3 = 1;


	
	//printf( "Interrupts Successfully Enabled.\n");
}


