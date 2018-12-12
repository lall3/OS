/*
This file was added by Shanay.
This is the header to exception_handler.c

*/


#ifndef _EXCEPTION_HANDLER_H
#define _EXCEPTION_HANDLER_H

#define VIDEO 0xB8000
#define NUM_COLS 80
#define NUM_ROWS 25
#define ERROR_MSG_LENGTH 2000


#include "lib.h"
#include "x86_desc.h"
#include "system_calls.h"



int32_t exception_flag;


void set_ef(int flag);
int get_ef();

void handle_division_by_zero();
void handle_debug();
void handle_non_maskable_int();
void handle_breakpoint();
void handle_overflow();
void handle_bound_range_exceeded();
void handle_invalid_opcode();
void handle_device_not_available();
void handle_double_fault();
void handle_coprocessor_segment_overrun();
void handle_invalid_tss();
void handle_segment_not_present();
void handle_stack_segment_fault();
void handle_general_protection_fault();
void handle_page_fault();
void handle_reserved();
void handle_floating_point();
void handle_alignemnt_check();
void handle_machine_check();
void handle_SIMD_floating_point();
void handle_virt_exception();
void handle_security_exception();
void handle_all_others();

void blue_screen_of_death(char * message, int count);



#endif
