
#ifndef _RTC_H
#define _RTC_H


#include "types.h"
#include "i8259.h"
#include "lib.h"
#include "keyboard.h"
#include "../syscalls/ece391syscall.h"

#define RTC_PORT_1			0x70
#define RTC_PORT_2			0x71
#define RTC_REG_A			0x8A
#define RTC_REG_B			0x8B
#define RTC_REG_C			0x0C
#define RTC_REG_D			0x0D
#define INIT_FREQ			0x20
#define MIN_FREQ			15
#define MAX_FREQ			6
#define RTC_INITIAL_RATE 	0x0F
#define RTC_BITMASK_BIT_6	0x40
#define RTC_INITIAL_FREQ	32


void init_rtc(void);
int32_t open_rtc(const uint8_t* fname);
int32_t close_rtc(int32_t fd);
int32_t read_rtc(int32_t fd, void* buf, int32_t nbytes);
int32_t write_rtc(int32_t fd, const void* buf, int32_t nbytes);
void rtclock_handler(void);

#endif
