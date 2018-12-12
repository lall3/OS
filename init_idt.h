/*
 * This file was added by Shanay.
 * It is the header file to init_idt.c
 * 
 */

#ifndef _INIT_IDT_H
#define _INIT_IDT_H

#include "x86_desc.h"
#include "lib.h"
#include "exception_handler.h"
#include "i8259.h"
#include "keyboard.h"
#include "rtc.h"
#include "system_calls.h"
#include "system_call_linkage.h"
#include "scheduling.h"


#define KEYBOARD_INDEX	0x21
#define RTC_INDEX		0x28
#define PIT_INDEX		0x20
#define IDT_SIZE		256
#define SYS_CALL		0x80



/* initalizes the interrupt descriptor table */
void init_idt();

#endif


