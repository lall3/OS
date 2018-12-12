/*
contains pit handler and scheduling
*/

#ifndef _SCHEDULING_H
#define _SCHEDULING_H

#include "types.h"
#include "i8259.h"
#include "lib.h"
#include "keyboard.h"
#include "i8259.h"
#include "system_calls.h"

#define PIT_NUM 		0
#define PIT_PORT_1 		0x40
#define PIT_PORT_2 		0x41
#define PIT_PORT_3 		0x42
#define MODE_REG 		0x43
#define MAX_FREQ_PIT 	1193182
#define MODE_1			0x36
#define MASK			0xFF
#define DIV_25HZ		11931.82  //29829

/*void pit_init(int div);*/
void pit_handler();
void PIT_switch_process(pcb_t* next_process);
int next_process();
void pit_init();


#endif
