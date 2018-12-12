/*  keyboard.h - header file for keyboard.c
 */
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"
#include "i8259.h"
#include "lib.h"
#include "rtc.h"
#include "files.h"
/* keyboard port number */
#define KEYB_PORT	0x60

#define LEFT_SHIFT	42
#define RIGHT_SHIFT	54
#define CTRL		29
#define CAPS_LOCK	58
#define BACKSPACE	14
#define TAB			15
#define	ENTER		28
#define SPACE		57
#define DELETE		83
#define ALT			56
#define EL			38
#define FOUR		5
#define FIVE		6
#define THREE		4
#define TWO			3
#define ONE			2
#define F1			59
#define F2			60
#define F3			61
#define MAX_FILES	17

#define NEW_LINE	10
#define	CARRIAGE_RETURN	13


/*

	int (*open)(const uint8_t* fname);
	int (*read)(int32_t fd, void* buf, int32_t nbytes);
	int (*write)(int32_t fd, const void* buf, int32_t nbytes);
	int (*close)(int32_t fd);
		
*/

int current_terminal_id;

int32_t term_open_wrapper(const uint8_t* fname);

int32_t term_write_wrapper(int32_t fd, const void* buf, int32_t nbytes);

int32_t term_close_wrapper(int32_t fd);

int32_t term_read_wrapper(int32_t fd, void* buf, int32_t nbytes);



/*initialize the keyboard IRQ2 on PIC*/
void term_open(void);

uint32_t term_read(int32_t fd, uint8_t * buffer, uint32_t count);

uint32_t term_write(const uint8_t * buffer, uint32_t count);

void term_close(void);


void term_putc(uint8_t c);


void term_clear(void);
void term_flush(void);

void shift_video_memory_up();
void shift_entire_screen_up();


void set_term_id(int new_id);
int get_term_id();
int get_current_viewed_terminal();

/*handles keystrokes putc to screen*/
void keyb_handler(void);

#endif
