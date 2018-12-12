/* Added by Ian. Code by Mitri.
 * keyboard.c - functions to interact with keyboard input
 */

#include "keyboard.h"






// 2-13, 16-27, 30-41, 
#define VIDEO 0xB8000
#define NUM_COLS 80
#define NUM_ROWS 25
#define ATTRIB 0x7
#define TOTAL_SCREEN_SIZE	NUM_COLS*NUM_ROWS
#define BUFFER_SIZE	128
#define KEYBOARD_INPUT_SIZE	128
#define CTRL_ONE_SIZE	80*17



#define BUFFER_SIZE_TAB	125

#define END_OF_PRINTABLE_CHARS	54
#define RELEASE	0x80
#define	PRESSED_SCANCODE	0x7F
#define SIZE_OF_FILE_NAME	11
#define MAX_FILE_NAME 32



static char* video_mem = (char *)VIDEO;



int terminal_count = 0;

int current_viewed_terminal;
static int shift_pressed = 0;
static int caps_lock_pressed = 0;
static int ctrl_pressed = 0;
static int alt_pressed = 0;





/*  char array that stores the keystroke values for keyboard */
unsigned char keyb_us[KEYBOARD_INPUT_SIZE] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};


unsigned char keyb_us_shift[KEYBOARD_INPUT_SIZE] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+', '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
 '"', '~',   0,		/* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', '<', '>', '?',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};



unsigned char keyb_us_caps[KEYBOARD_INPUT_SIZE] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

unsigned char keyb_us_shift_caps[KEYBOARD_INPUT_SIZE] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':',	/* 39 */
 '"', '~',   0,		/* Left shift */
 '|', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', '<', '>', '?',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

typedef struct terminal_t{
	uint8_t term_id;	//will be used when multiple temrinals are used
	uint8_t is_active;	//used as a boolean flag to check if active or not
	
	uint8_t entire_screen[NUM_COLS*NUM_ROWS];
	int screen_x;
	int screen_y;
	uint8_t buffer[BUFFER_SIZE];	//used to keep track of buffer for each terminal
	uint8_t prev_buffer[BUFFER_SIZE];
	int prev_buffer_size;
	uint8_t curr_index;		//the current index in the bufer to write to/read upto
	int end_of_term;
	int read_is_waiting;
	int enter_has_been_pressed;
}terminal_t;


struct terminal_t all_terminals[3];





// Checkpoint 1/2
/*   
 *   init_keyb(void)
 *   DESCRIPTION: enables the IRQ1 on Master PIC 	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none 
 *   SIDE EFFECTS: enables IRQ1 on Master PIC.
 */
void term_open(void){
	cli();
	term_flush();
	int i;
	struct terminal_t make_terminal;		//initializing terminal and its member variables accordingly
	make_terminal.term_id = terminal_count;
	current_terminal_id = make_terminal.term_id;
	current_viewed_terminal = make_terminal.term_id;
	terminal_count++;

	for(i=0; i<NUM_COLS*NUM_ROWS; i++){
		make_terminal.entire_screen[i] = '\0';
	}
	for(i=0; i<BUFFER_SIZE; i++){
		make_terminal.buffer[i] = '\0';
		make_terminal.prev_buffer[i] = '\0';
	}
	make_terminal.end_of_term = 0;
	make_terminal.enter_has_been_pressed = 0;
	make_terminal.screen_x = 0;
	make_terminal.screen_y = 0;
	make_terminal.curr_index = 0;
	make_terminal.is_active = 1;
	make_terminal.read_is_waiting = 0;
	make_terminal.prev_buffer_size = 0;
	all_terminals[make_terminal.term_id] = make_terminal;
	enable_irq(1);	//irq_num of keyboard is 1;
	sti();
}


// Checkpoint 1/2
/*   
 *   term_read(int32_t fd, uint8_t * buffer, uint32_t count)
 *   DESCRIPTION: does nothing 	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none 
 *   SIDE EFFECTS: none
 */
uint32_t term_read(int32_t fd, uint8_t * buffer, uint32_t count){
	int i;
	all_terminals[current_terminal_id].read_is_waiting = 1;
	//all_terminals[current_terminal_id].curr_index = 0; uncomment if it breaks when typing along with executing processes.
	while((all_terminals[current_terminal_id].curr_index<BUFFER_SIZE) && !(all_terminals[current_terminal_id].enter_has_been_pressed) && (all_terminals[current_terminal_id].curr_index<count)){

	}
	
	disable_irq(0);
	cli();

	for(i=0; i<all_terminals[current_terminal_id].prev_buffer_size; i++){
		if((i==count-1) || (all_terminals[current_terminal_id].prev_buffer[i] == '\n')){
			break;
		}
		
		buffer[i] = all_terminals[current_terminal_id].prev_buffer[i];
	}
	

	buffer[i] = '\n';
	all_terminals[current_terminal_id].read_is_waiting = 0;
	all_terminals[current_terminal_id].enter_has_been_pressed = 0;
	all_terminals[current_terminal_id].curr_index = 0;
	sti();
	enable_irq(0);
	
	
	return all_terminals[current_viewed_terminal].prev_buffer_size+1;
}


/*
Wrapper functions for the open/close/read/write
*/

int32_t term_open_wrapper(const uint8_t* fname){
	term_open();
	return 0;
}

int32_t term_write_wrapper(int32_t fd, const void* buf, int32_t nbytes){
	term_write(buf, nbytes);
	all_terminals[current_terminal_id].curr_index = 0;
	return 0;
}

int32_t term_close_wrapper(int32_t fd){
	term_close();
	return 0;
}

int32_t term_read_wrapper(int32_t fd, void* buf, int32_t nbytes){
	return term_read(fd, buf, nbytes);

}




// Checkpoint 1
/*   
 *   term_write(uint8_t * buffer, uint32_t count)
 *   DESCRIPTION: only way to interact with the terminal in order to print something     
 *   INPUTS: buffer to print, number of characters to print
 *   OUTPUTS: none
 *   RETURN VALUE: none 
 *   SIDE EFFECTS: enables IRQ1 on Master PIC.
 */
uint32_t term_write(const uint8_t * buffer, uint32_t count){
	
	cli();
	int i;
	for(i=0; i<count; i++){
		if(buffer[i]!=NULL){
			term_putc(buffer[i]);
		}
		else{
			term_putc(' ');
		}
	}
	sti();

	return 0;
}



// Checkpoint 1
/*   
 *   init_keyb(void)
 *   DESCRIPTION: enables the IRQ1 on Master PIC 	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none 
 *   SIDE EFFECTS: enables IRQ1 on Master PIC.
 */
void term_close(void){
	/*cli();
	clear();
	screen_x = 0;
	screen_y = 0;
	struct terminal_t make_terminal;		//initializing terminal and its member variables accordingly
	make_terminal.term_id = current_terminal_count;
	current_terminal_count++;
	make_terminal.curr_index = 0;
	make_terminal.is_active = 1;
	enable_irq(1);	//irq_num of keyboard is 1;
	sti();*/
}







// Checkpoint 1
/*   
 *   keyb_handler(void) 
 *   DESCRIPTION: handles keystrokes and prints to screen	     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none 
 *   SIDE EFFECTS: takes keyboard input and echos to the screen
 */
void keyb_handler(void) {

	cli();
	unsigned char scancode;
	scancode = inb(KEYB_PORT); 
	send_eoi(1);               //send eoi and allow other interrupts to happen since sensitive information is taken in already, this allows multiple interrupts to happen
	
	int terminal_before = current_terminal_id;
	current_terminal_id = current_viewed_terminal;
	//if MSB is 1, then a key is being released, so if key is released, don't do anything
	
	if (scancode & RELEASE){
		scancode = scancode & PRESSED_SCANCODE;
		if((scancode == LEFT_SHIFT) || (scancode == RIGHT_SHIFT)){
			shift_pressed = 0;
		}
		else if(scancode == CTRL){
			ctrl_pressed = 0;
		}
		else if(scancode == ALT){
			alt_pressed = 0;
		}
		
		current_terminal_id = terminal_before;
		sti();
		asm("leave;" 
			"iret;");
	}
	else {
		if(ctrl_pressed){
			if(scancode == EL){
				term_clear();
				current_terminal_id = terminal_before;
				sti();
				asm("leave;" 
					"iret;");
			}
		}
		if(alt_pressed){
			if(scancode == F1){
				if(current_viewed_terminal != 0){
					current_terminal_id = 0;
					current_viewed_terminal = 0;
					int pos = all_terminals[current_terminal_id].screen_x + (all_terminals[current_terminal_id].screen_y)*NUM_COLS;
					term_flush();
					all_terminals[current_terminal_id].screen_x = 0;
					all_terminals[current_terminal_id].screen_y = 0;
					term_write(all_terminals[current_terminal_id].entire_screen, pos);
					//page_video_mem(USER_VID_VIR);
					//unpage_vid(USER_VID_VIR1);
					//unpage_vid(USER_VID_VIR2);
				}
				
				current_terminal_id = terminal_before;
				sti();
				asm("leave;" 
					"iret;");
			}
			else if(scancode == F2){
				if(current_viewed_terminal != 1){
					if(!all_terminals[1].is_active){
						term_open();
						uint8_t shell[6] = "shell";
						set_no_parent(1);
						execute(shell);
					}
					else{
						current_viewed_terminal = 1;
						current_terminal_id = 1;
						int pos = all_terminals[current_terminal_id].screen_x + (all_terminals[current_terminal_id].screen_y)*NUM_COLS;
						term_flush();
						all_terminals[current_terminal_id].screen_x = 0;
						all_terminals[current_terminal_id].screen_y = 0;
						term_write(all_terminals[current_terminal_id].entire_screen, pos);
						//page_video_mem(USER_VID_VIR1);
						//unpage_vid(USER_VID_VIR);
						//unpage_vid(USER_VID_VIR2);
					}
				}
				
				current_terminal_id = terminal_before;
				sti();
				asm("leave;" 
					"iret;");
			}
			else if(scancode == F3){
				if(current_viewed_terminal != 2){
					if(!all_terminals[2].is_active){
						term_open();
						uint8_t shell[6] = "shell";
						set_no_parent(1);
						execute(shell);
					}
					else{
						current_viewed_terminal = 2;
						current_terminal_id = 2;
						int pos = all_terminals[current_terminal_id].screen_x + (all_terminals[current_terminal_id].screen_y)*NUM_COLS;
						term_flush();
						all_terminals[current_terminal_id].screen_x = 0;
						all_terminals[current_terminal_id].screen_y = 0;
						term_write(all_terminals[current_terminal_id].entire_screen, pos);
						//page_video_mem(USER_VID_VIR2);
						//unpage_vid(USER_VID_VIR1);
						//unpage_vid(USER_VID_VIR);
					}
				}
				
				current_terminal_id = terminal_before;
				sti();
				asm("leave;" 
					"iret;");
			}
		}
		
		if((scancode == LEFT_SHIFT) || (scancode == RIGHT_SHIFT)){
			shift_pressed = 1;
			
			current_terminal_id = terminal_before;
			sti();
			asm("leave;" 
				"iret;");
		}
		else if(scancode == CTRL){
			ctrl_pressed = 1;
			
			current_terminal_id = terminal_before;
			sti();
			asm("leave;" 
				"iret;");
		}
		else if(scancode == ALT){
			alt_pressed = 1;
			
			current_terminal_id = terminal_before;
			sti();
			asm("leave;" 
				"iret;");
		}
		else if(scancode == CAPS_LOCK){
			caps_lock_pressed = !caps_lock_pressed;
			
			current_terminal_id = terminal_before;
			sti();
			asm("leave;" 
				"iret;");
		}
		else if(scancode == BACKSPACE){
			if(all_terminals[current_terminal_id].curr_index>0){
				all_terminals[current_terminal_id].curr_index--;
				//printf("%d", buf_screen_y+all_terminals[current_terminal_id].curr_index);
				if(all_terminals[current_terminal_id].screen_x>0){
					all_terminals[current_terminal_id].screen_x--;
					*(uint8_t *)(video_mem + (((all_terminals[current_terminal_id].screen_y)*NUM_COLS + all_terminals[current_terminal_id].screen_x) << 1)) = ' ';
					*(uint8_t *)(video_mem + (((all_terminals[current_terminal_id].screen_y)*NUM_COLS + all_terminals[current_terminal_id].screen_x) << 1) + 1) = ATTRIB;
				}
				else{

					all_terminals[current_terminal_id].screen_x = NUM_COLS - 1;
					all_terminals[current_terminal_id].screen_y--;
					*(uint8_t *)(video_mem + (((all_terminals[current_terminal_id].screen_y)*NUM_COLS + all_terminals[current_terminal_id].screen_x) << 1)) = ' ';
					*(uint8_t *)(video_mem + (((all_terminals[current_terminal_id].screen_y)*NUM_COLS + all_terminals[current_terminal_id].screen_x) << 1) + 1) = ATTRIB;
				}
			}
			
			current_terminal_id = terminal_before;
			sti();
			asm("leave;" 
			"iret;");
		}
		else if(scancode == SPACE){
			if(all_terminals[current_terminal_id].curr_index<BUFFER_SIZE - 1){
				all_terminals[current_terminal_id].buffer[all_terminals[current_terminal_id].curr_index++] = keyb_us[scancode];
				term_write(all_terminals[current_terminal_id].buffer + all_terminals[current_terminal_id].curr_index - 1, 1);
			}
		}
		else if(scancode == TAB){
			if(all_terminals[current_terminal_id].curr_index<BUFFER_SIZE-4){
				all_terminals[current_terminal_id].buffer[all_terminals[current_terminal_id].curr_index++] = keyb_us[SPACE];
				all_terminals[current_terminal_id].buffer[all_terminals[current_terminal_id].curr_index++] = keyb_us[SPACE];
				all_terminals[current_terminal_id].buffer[all_terminals[current_terminal_id].curr_index++] = keyb_us[SPACE];
				all_terminals[current_terminal_id].buffer[all_terminals[current_terminal_id].curr_index++] = keyb_us[SPACE];
				term_write(all_terminals[current_terminal_id].buffer + all_terminals[current_terminal_id].curr_index - 1, 4);
			}
		}
		else if(scancode == ENTER){
			if(all_terminals[current_terminal_id].read_is_waiting){
				int i;
				for(i=0; i<all_terminals[current_terminal_id].curr_index; i++){
					all_terminals[current_terminal_id].prev_buffer[i] = all_terminals[current_terminal_id].buffer[i];
				}
				all_terminals[current_terminal_id].prev_buffer_size = all_terminals[current_terminal_id].curr_index;
				all_terminals[current_viewed_terminal].enter_has_been_pressed = 1;
			}
			all_terminals[current_terminal_id].curr_index = 0;
			uint8_t enter[1];
			enter[0] = '\n';
			term_write(enter, 1);
		}
		else if(scancode>1 && scancode<END_OF_PRINTABLE_CHARS){
			if(all_terminals[current_terminal_id].curr_index<BUFFER_SIZE - 1){
				if(!shift_pressed && !caps_lock_pressed){		
					all_terminals[current_terminal_id].buffer[all_terminals[current_terminal_id].curr_index++] = keyb_us[scancode];
				}
				else if(shift_pressed && caps_lock_pressed){
					all_terminals[current_terminal_id].buffer[all_terminals[current_terminal_id].curr_index++] = keyb_us_shift_caps[scancode];
				}
				else if(shift_pressed){
					all_terminals[current_terminal_id].buffer[all_terminals[current_terminal_id].curr_index++] = keyb_us_shift[scancode];
				}
				else if(caps_lock_pressed){
					all_terminals[current_terminal_id].buffer[all_terminals[current_terminal_id].curr_index++] = keyb_us_caps[scancode];
				}
				term_write(all_terminals[current_terminal_id].buffer + all_terminals[current_terminal_id].curr_index - 1, 1);
			}
			
		}		
		
	}
	
	current_terminal_id = terminal_before;
	sti();
	asm("leave;" 
		"iret;");
}



/*
* void putc(uint8_t c);
*   Inputs: uint_8* c = character to print
*   Return Value: void
*	Function: Output a character to the console 
*/

void
term_putc(uint8_t c)
{
	//int i;
	if((c>31 && c<127)  || (c==NEW_LINE) || (c==CARRIAGE_RETURN)){
		if(all_terminals[current_terminal_id].screen_y==(NUM_ROWS-1)){
			if(c == '\n'){
				if(current_terminal_id==current_viewed_terminal){
					shift_video_memory_up();
				}
				shift_entire_screen_up();
				all_terminals[current_terminal_id].screen_x = 0;
			}
			else{
				if(all_terminals[current_terminal_id].screen_x==(NUM_COLS-1)){
					all_terminals[current_terminal_id].end_of_term = 1;
					if(current_terminal_id==current_viewed_terminal){
						*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1)) = c;
						*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1) + 1) = ATTRIB;
					}
					all_terminals[current_terminal_id].entire_screen[(NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x)] = c;
					all_terminals[current_terminal_id].screen_x = 0;
					all_terminals[current_terminal_id].screen_y = NUM_ROWS-1;
				}
				else{
					if(all_terminals[current_terminal_id].end_of_term){
						
						//printf("used shift memory and end of term: %d", all_terminals[current_terminal_id].end_of_term);
						if(current_terminal_id==current_viewed_terminal){
							shift_video_memory_up();
							*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1)) = c;
							*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1) + 1) = ATTRIB;
						}
						shift_entire_screen_up();
						//int pos = all_terminals[current_terminal_id].screen_x + (all_terminals[current_terminal_id].screen_y)*NUM_COLS;
						//for(i=0; i<pos; i++){
							all_terminals[current_terminal_id].entire_screen[(NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x)] = c;
						//}
						all_terminals[current_terminal_id].screen_x++;
						all_terminals[current_terminal_id].screen_y = NUM_ROWS-1;
						all_terminals[current_terminal_id].end_of_term = 0;
					}
					else{
						if(current_terminal_id==current_viewed_terminal){
							*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1)) = c;
							*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1) + 1) = ATTRIB;
						}
						//int pos = all_terminals[current_terminal_id].screen_x + (all_terminals[current_terminal_id].screen_y)*NUM_COLS;
						//for(i=0; i<pos; i++){
							all_terminals[current_terminal_id].entire_screen[(NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x)] = c;
						//}
						all_terminals[current_terminal_id].screen_x++;
						all_terminals[current_terminal_id].screen_y = (all_terminals[current_terminal_id].screen_y + (all_terminals[current_terminal_id].screen_x / NUM_COLS)) % NUM_ROWS;
					}
				}
			}
		}
		else{
			if(c == '\n'){
				all_terminals[current_terminal_id].screen_x = 0;
				all_terminals[current_terminal_id].screen_y++;
			}
			else{
				if(current_terminal_id==current_viewed_terminal){
					*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1)) = c;
					*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1) + 1) = ATTRIB;
				}
				//int pos = all_terminals[current_terminal_id].screen_x + (all_terminals[current_terminal_id].screen_y)*NUM_COLS;
				//for(i=0; i<pos; i++){
					all_terminals[current_terminal_id].entire_screen[(NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x)] = c;
				//}
				all_terminals[current_terminal_id].screen_x++;
				if(all_terminals[current_terminal_id].screen_x>=NUM_COLS){
					all_terminals[current_terminal_id].screen_x = 0;
					all_terminals[current_terminal_id].screen_y++;
				}
				else{
					all_terminals[current_terminal_id].screen_y = (all_terminals[current_terminal_id].screen_y + (all_terminals[current_terminal_id].screen_x / NUM_COLS)) % NUM_ROWS;
				}
			}
		}
	}
	else{
		if(all_terminals[current_terminal_id].screen_y==(NUM_ROWS-1)){
				if(all_terminals[current_terminal_id].screen_x==(NUM_COLS-1)){
					all_terminals[current_terminal_id].end_of_term = 1;
					if(current_terminal_id==current_viewed_terminal){
						*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1)) = ' ';
						*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1) + 1) = ATTRIB;
					}
					//int pos = all_terminals[current_terminal_id].screen_x + (all_terminals[current_terminal_id].screen_y)*NUM_COLS;
					//for(i=0; i<pos; i++){
						all_terminals[current_terminal_id].entire_screen[(NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x)] = ' ';
					//}
					all_terminals[current_terminal_id].screen_x = 0;
					all_terminals[current_terminal_id].screen_y = NUM_ROWS-1;
				}
				else{
					if(all_terminals[current_terminal_id].end_of_term){
						
						//printf("used shift memory and end of term: %d", all_terminals[current_terminal_id].end_of_term);
						if(current_terminal_id==current_viewed_terminal){
							shift_video_memory_up();
							*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1)) = ' ';
							*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1) + 1) = ATTRIB;
						}
						shift_entire_screen_up();
						//int pos = all_terminals[current_terminal_id].screen_x + (all_terminals[current_terminal_id].screen_y)*NUM_COLS;
						//for(i=0; i<pos; i++){
							all_terminals[current_terminal_id].entire_screen[(NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x)] = ' ';
						//}
						all_terminals[current_terminal_id].screen_x++;
						all_terminals[current_terminal_id].screen_y = NUM_ROWS-1;
						all_terminals[current_terminal_id].end_of_term = 0;
					}
					else{
						if(current_terminal_id==current_viewed_terminal){
							*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1)) = ' ';
							*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1) + 1) = ATTRIB;
						}
						//int pos = all_terminals[current_terminal_id].screen_x + (all_terminals[current_terminal_id].screen_y)*NUM_COLS;
						//for(i=0; i<pos; i++){
							all_terminals[current_terminal_id].entire_screen[(NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x)] = ' ';
						//}
						all_terminals[current_terminal_id].screen_x++;
						all_terminals[current_terminal_id].screen_y = (all_terminals[current_terminal_id].screen_y + (all_terminals[current_terminal_id].screen_x / NUM_COLS)) % NUM_ROWS;
					}
				}
		}
		else{
				if(current_terminal_id==current_viewed_terminal){
					*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1)) = ' ';
					*(uint8_t *)(video_mem + ((NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x) << 1) + 1) = ATTRIB;
				}
				//int pos = all_terminals[current_terminal_id].screen_x + (all_terminals[current_terminal_id].screen_y)*NUM_COLS;
				//for(i=0; i<pos; i++){
					all_terminals[current_terminal_id].entire_screen[(NUM_COLS*(all_terminals[current_terminal_id].screen_y) + all_terminals[current_terminal_id].screen_x)] = ' ';
				//}
				all_terminals[current_terminal_id].screen_x++;
				if(all_terminals[current_terminal_id].screen_x>=NUM_COLS){
					all_terminals[current_terminal_id].screen_x = 0;
					all_terminals[current_terminal_id].screen_y++;
				}
				else{
					all_terminals[current_terminal_id].screen_y = (all_terminals[current_terminal_id].screen_y + (all_terminals[current_terminal_id].screen_x / NUM_COLS)) % NUM_ROWS;
				}
		}
		
	}
}

/*
* void term_clear(void);
*   Inputs: none
*   Return Value: void
*	Function: clears the console and puts the cursor at the start of the screen 
*/

void
term_clear(void)
{
    int32_t i;
    uint8_t temp_buffer[BUFFER_SIZE];
    int start_of_buffer = all_terminals[current_terminal_id].screen_x + all_terminals[current_terminal_id].screen_y*NUM_COLS;
    start_of_buffer -= all_terminals[current_terminal_id].curr_index;
    int position = start_of_buffer - start_of_buffer%NUM_COLS;
    for(i=0; i<(start_of_buffer%NUM_COLS); i++){
    	temp_buffer[i] = *(uint8_t *)(video_mem + ((position+i) << 1));
    }
    
    for(i=0; i<NUM_ROWS*NUM_COLS; i++) {
        *(uint8_t *)(video_mem + (i << 1)) = ' ';
        *(uint8_t *)(video_mem + (i << 1) + 1) = ATTRIB;
    }
    
	all_terminals[current_terminal_id].screen_x = 0;
	all_terminals[current_terminal_id].screen_y = 0;

	term_write(temp_buffer, start_of_buffer%NUM_COLS);
    term_write(all_terminals[current_terminal_id].buffer, all_terminals[current_terminal_id].curr_index);

	for(i=0; i<NUM_COLS*2; i++){
		all_terminals[current_terminal_id].entire_screen[i] = *(uint8_t *)(video_mem + (i << 1));
	}
	
}


void shift_video_memory_up(){
	int32_t i;
    for(i=0; i<(NUM_ROWS-1)*(NUM_COLS); i++) {
        *(uint8_t *)(video_mem + (i << 1)) = *(uint8_t *)(video_mem + ((i+NUM_COLS) << 1));
        *(uint8_t *)(video_mem + (i << 1) + 1) = *(uint8_t *)(video_mem + ((i+NUM_COLS) << 1) + 1);
    }
	for(i=0; i<NUM_COLS; i++){
		*(uint8_t *)(video_mem + ((i+((NUM_ROWS-1)*NUM_COLS)) << 1)) = ' ';
        *(uint8_t *)(video_mem + ((i+((NUM_ROWS-1)*NUM_COLS)) << 1) + 1) = ATTRIB;
	}
}

void shift_entire_screen_up(){
	int32_t i;
    for(i=0; i<(NUM_ROWS-1)*(NUM_COLS); i++) {
        all_terminals[current_terminal_id].entire_screen[i] = all_terminals[current_terminal_id].entire_screen[i+NUM_COLS];
    }
	for(i=0; i<NUM_COLS; i++){
		all_terminals[current_terminal_id].entire_screen[i+((NUM_ROWS-1)*NUM_COLS)] = '\0';
	}
}


void
term_flush(void)
{
	int i;
	for(i=0; i<NUM_ROWS*NUM_COLS; i++) {
        *(uint8_t *)(video_mem + (i << 1)) = ' ';
        *(uint8_t *)(video_mem + (i << 1) + 1) = ATTRIB;
    }
	
}

void set_term_id(int new_id){
	current_terminal_id = new_id;
}


int get_term_id(){
	return current_terminal_id;
}

void set_current_viewed_terminal(int new_viewed_id){
	current_viewed_terminal = new_viewed_id;
}


int get_current_viewed_terminal(){
	return current_viewed_terminal;
}


