/*
This file was added by Shanay.
This has all the exception handlers,
which are the first 32 entries in the IDT.


*/
#include "exception_handler.h"
#define ATTRIB 0x17



/* void set_ef(int flag)
 *   DESCRIPTION: sets the exception flag accordingly when an exception is called. * 		  	     
 *   INPUTS: flag
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: sets the exception_flag variable, defined in exception_handler.h
 */
void set_ef(int flag){
	if(flag == 0) exception_flag = 0;
	if(flag == 1) exception_flag = 1;
}

/* void get_ef()
 *   DESCRIPTION: Halt calls this to check if exception_flag was set 
 *   INPUTS: none
 *   OUTPUTS: exception_flag
 *   RETURN VALUE: exception_flag
 *   SIDE EFFECTS: none
 */
int get_ef(){
	return exception_flag;
}

/* void handle_division_by_zero(void)
 *   DESCRIPTION: Is called when theres a division of zero error. Its called via an entry in the IDT.
 * 		  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_division_by_zero(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Divide By Zero Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".hdbz: hlt; jmp .hdbz;");
}

/* void handle_division_by_zero(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_debug(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Debug Error Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".hd: hlt; jmp .hd;");
}

/* void handle_division_by_zero(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_non_maskable_int(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "NonMaskable Int Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".hnmi: hlt; jmp .hnmi;");
}

/* void handle_division_by_zero(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_breakpoint(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Breakpoint Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".hb: hlt; jmp .hb;");
}

/* void handle_division_by_zero(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_overflow(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Overflow Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".ho: hlt; jmp .ho;");
}

/* void handle_division_by_zero(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_bound_range_exceeded(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Bound Range Exceeded Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".hbre: hlt; jmp .hbre;");
}

/* void handle_invalid_opcode(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_invalid_opcode(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Invalid Opcode Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".hio: hlt; jmp .hio;");
}

/* void handle_device_not_available(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_device_not_available(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Device Not Available Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".hdna: hlt; jmp .hdna;");
}

/* void handle_double_fault(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_double_fault(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Handle Double Fault Excpetion";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".hdf: hlt; jmp .hdf;");
}

/* void handle_coprocessor_segment_overrun(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_coprocessor_segment_overrun(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Coprocessor Segment Overrun Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".hcso: hlt; jmp .hcso;");
}

/* void handle_invalid_tss(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_invalid_tss(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Invalid TSS Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".hit: hlt; jmp .hit;");
}

/* void handle_segment_not_present(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_segment_not_present(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Segment Not Present Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".hsnp: hlt; jmp .hsnp;");
}

/* handle_stack_segment_fault(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_stack_segment_fault(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Stack Segment Fault";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".hssf: hlt; jmp .hssf;");
}

/* void handle_general_protection_fault(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_general_protection_fault(){
		exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "General Protection Fault";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".hgpf: hlt; jmp .hgpf;");
}

/* void handle_page_fault(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_page_fault(){
	exception_flag = 1;
	
	int CR2;
	asm volatile("movl %%cr2, %0"
	:"=r"(CR2)
	:
	:"eax","memory","cc"
	);
	printf("CR2: %d", CR2);
	
//	char error_message[ERROR_MSG_LENGTH] = "Page Fault";
//	int msg_size = strlen(error_message);
//	blue_screen_of_death(error_message, msg_size);
	
	cli();
	asm volatile(".pg_ft: hlt; jmp .pg_ft;");
}

/* void handle_reserved(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_reserved(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "You have broken the system";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	cli();
	while(1);
	//asm volatile(".1: hlt; jmp .1;");
}

/* void handle_floating_point(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_floating_point(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Floating Point Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	while(1);
	//asm volatile(".1: hlt; jmp .1;");
}

/* void handle_alignemnt_check(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_alignemnt_check(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Alignemnt Check Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	while(1);
//	asm volatile(".1: hlt; jmp .1;");
}

/* void handle_machine_check(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_machine_check(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Machine Check Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	while(1);
//	asm volatile(".1: hlt; jmp .1;");
}

/* void handle_SIMD_floating_point(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_SIMD_floating_point(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "SIMD Floating Point Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	while(1);
//	asm volatile(".1: hlt; jmp .1;");
}

/* void handle_virt_exception(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_virt_exception(){
		exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Virt Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);

	
	cli();
	while(1);
//	asm volatile(".1: hlt; jmp .1;");
}

/* void handle_security_exception(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_security_exception(){
		exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "Security Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	while(1);
//	asm volatile(".1: hlt; jmp .1;");
}

/* void handle_all_others(void)	  	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints an error message and sned the system in a quiet loop.
 */
void handle_all_others(){
	exception_flag = 1;
	char error_message[ERROR_MSG_LENGTH] = "All Other Exception";
	int msg_size = strlen(error_message);
	blue_screen_of_death(error_message, msg_size);
	
	cli();
	while(1);
//	asm volatile(".1: hlt; jmp .1;");
}


/* void blue_screen_of_death(char * message, int count)  	     
 *   INPUTS: message to print, message size
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: creates a blue screen with the error message
 */
void blue_screen_of_death(char * message, int count){
	int32_t i;
	int j;
	int middleu, middled;
	static char* video_mem = (char *)VIDEO;
	j = 0;
	middled = (NUM_ROWS*NUM_COLS/2) - count/2;
    middleu = (NUM_ROWS*NUM_COLS/2) + count/2;
	for(i=0; i<NUM_ROWS*NUM_COLS; i++) {
		if((i<=middleu) && (i>=middled)){
			*(uint8_t *)(video_mem + (i << 1)) = message[j];
			*(uint8_t *)(video_mem + (i << 1) + 1) = ATTRIB;
			j++;
		}
		else{
			*(uint8_t *)(video_mem + (i << 1)) = ' ';
			*(uint8_t *)(video_mem + (i << 1) + 1) = ATTRIB;
		}
    }
}

