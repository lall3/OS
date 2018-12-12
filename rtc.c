/*
 * rtc.c - c file for enabling Real Time Clock RTC
 * 
 */
#include "rtc.h"

uint8_t rate;	//rate of rtc speed
volatile int is_rtc_enabled = 0;
int fd;








/*   Checkpoint 2
 *   init_rtc
 *   DESCRIPTION: initializes the RTC and enables RTC interrupts 
 *	 			  to PIC and processsor
 *                  
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none 
 *   SIDE EFFECTS: changes the rate of periodic interrupts, 
 *			       value of cmos registers and enables the rtc interrupt
 */
void init_rtc(void) {
	cli();			//cli
	/*setting the registers*/
	outb(RTC_REG_A, RTC_PORT_1);	// select Status Register A
	outb(INIT_FREQ, RTC_PORT_2);	// write to CMOS/RTC RAM

	/*turning on irq8*/
	outb(RTC_REG_B, RTC_PORT_1); // select Status Register B 
	unsigned char prev = inb(RTC_PORT_2);
	outb(RTC_REG_B, RTC_PORT_1);	
	outb(prev | RTC_BITMASK_BIT_6, RTC_PORT_2); //turn on bit 6 of register

	rate = RTC_INITIAL_RATE;


	//is_rtc_enabled = 0;
	/*changing interrupt rate*/
	/*outb(RTC_REG_A,RTC_PORT_1);
	unsigned char next = inb(RTC_PORT_2);
	outb(RTC_REG_A,RTC_PORT_1);
	outb((next & 0xF0)| rate,RTC_PORT_2); //rate is between 2 and 15*/
	
	sti();
	enable_irq(SLAVE_PIC_IRQ_NUM);	//enable cascaded irqs to reach the 8th one
	enable_irq(RTC_IRQ_NUM);	//enable rtc clock

	int initial_freq = RTC_INITIAL_FREQ;
	write_rtc(0, &initial_freq, 4);
	return;
}










/*   Checkpoint 2
 *   rtclock_handler
 *   DESCRIPTION: handles the RTC printing "1" to screen 
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none 
 *   SIDE EFFECTS:  calls test interrupts and creates flickering 
 *			       screen effect due to periodic interrupts being enabled
 */
void rtclock_handler(void) 
{
	cli();
	is_rtc_enabled = 1;
	outb(RTC_REG_C, RTC_PORT_1);	// select register C
	inb(RTC_PORT_2);		// just throw away contents
	//term_putc('1');	
	send_eoi(RTC_IRQ_NUM);			//end function
	sti();
	
	asm("leave;" 
		"iret;");
}





/*   Checkpoint 2
 *   open_rtc
 *   DESCRIPTION: simple helper function that initializes the RTC
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none 
 *   SIDE EFFECTS:  used to initialize the RTC
 */
int32_t open_rtc(const uint8_t* fname) {
	init_rtc();
	return 0;
}



/*   Checkpoint 2
 *   close_rtc
 *   DESCRIPTION: simple helper function that stops RTC
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none 
 *   SIDE EFFECTS:  used to disable interrupts from the RTC
 */
int32_t close_rtc(int32_t fd) {
	disable_irq(RTC_IRQ_NUM);
	return 0;
}




/*   Checkpoint 2
 *   read_rtc
 *   DESCRIPTION: loops while RTC is not enabled
 *   INPUTS: none
 *   OUTPUTS: return 0;
 *   RETURN VALUE: always return 0
 *   SIDE EFFECTS: loops while RTC is disabled
 */
int32_t read_rtc(int32_t fd, void* buf, int32_t nbytes) {
	is_rtc_enabled = 0;
	while(!is_rtc_enabled);
	return 0;
}





/*   Checkpoint 2
 *   write_rtc
 *   DESCRIPTION: simple helper function that initializes the RTC
 *   INPUTS: int buf, int count
 *   OUTPUTS: return 0 on success, -1 on failure
 *   RETURN VALUE: return 0 on success,-1 on failure 
 *   SIDE EFFECTS:  
 */
int32_t write_rtc(int32_t fd, const void* buf, int32_t nbytes) {
	int32_t* temp = (int32_t*)buf;
	int quotient = *temp;
	rate = 0;
	while (quotient != 1) {
		quotient /= 2;
		rate++;
	}

	rate = MIN_FREQ + 1 - rate;

	if (nbytes == 4 && rate >= 5 /*&& buf < 16*/) {			//because an int is 4 bytes
		outb(RTC_REG_A,RTC_PORT_1);
		unsigned char next = inb(RTC_PORT_2);
		outb(RTC_REG_A,RTC_PORT_1);
		outb((next & 0xF0)| rate,RTC_PORT_2); //rate is between 2 and 15
		return 0;
	}
	else return -1;
}
