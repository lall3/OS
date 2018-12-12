/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts
 * are enabled and disabled.
 * Mask all interrupts with 0xFF */
uint8_t mask = 0xFF; /* IRQs 0-15 */


/*
Adapt the initialization code from Linux to initialize the PIC, the keyboard, and the RTC. Set up a general-purpose
infrastructure similar to what is done in the Linux kernel. You need to handle the keyboard and RTC interrupts, but
you also need to make sure that these devices are initialized before taking interrupts. We suggest that you first mask
out all interrupts on the PIC, then initialize the PIC, initialize the devices, and, as part of each device’s initialization,
enable its associated interrupt on the PIC. The handler addresses should be installed dynamically/indirectly via a data
structure used by the default handlers (as in Linux). You may also want to review the RTC data sheet linked from the
class web page.
For the checkpoint, your OS must execute the test interrupts handler (provided in lib.c) when an RTC interrupt
occurs. When a keyboard interrupt occurs, you must echo the correct characters to the screen. These simple tests will
determine if you have the IDT entries set up correctly, the PIC enabled, and the devices initialized and able to generate
interrupts
*/


// Checkpoint 1
/* void i8269_init(void)
 *   Initialize the 8259 PIC 
 *   DESCRIPTION: Send Interrupt Control Words (ICW) to both the
 *                Master PIC and the Slave PIC. ICW's and MASTER_
 *                PORT and SLAVE_PORT are defined in i8529.h header
 *                file.     
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: initializes the Master PIC and Slave PIC
 */
void
i8259_init(void)
{

//	https://k.lse.epita.fr/internals/8259a_controller.html
//	mask all interrupts? may not be neede

 	/* send Interrupt Control Word 1 to Master and to Slave PIC's. */
	outb( ICW1, MASTER_8259_PORT);
	outb( ICW1, SLAVE_8259_PORT);

	/* send Interrupt Control Word 2 to Master and Slave PIC's.
	 * ICW2 is different for both Master and Slave 
	 */
	outb( ICW2_MASTER, MASTER_8259_PORT + 1 ); //write to 0x21
	outb( ICW2_SLAVE, SLAVE_8259_PORT + 1); // write to 0xA1

	/* send Interrupt Control Word 3 to Master and Slave PIC's.
	 * ICW3 is different for both Master and Slave 
	 */
	outb( ICW3_MASTER, MASTER_8259_PORT + 1);
	outb( ICW3_SLAVE, SLAVE_8259_PORT + 1);

	/* send Interrupt Control Word 4 to Master and Slave PIC's. */
	outb( ICW4, MASTER_8259_PORT + 1);
	outb( ICW4, SLAVE_8259_PORT + 1);
	/*masking all other interrupts*/
	outb(mask, MASTER_8259_PORT + 1); //needs data port 
	outb(mask, SLAVE_8259_PORT + 1); //data port
}




// Checkpoint 1
/* 
 *   enable_irq(uint32_t irq_num)
 *   DESCRIPTION: Unmask interrupts to the inputted IRQ .
 *                Enable (unmask) the specified IRQ.
 *   INPUTS: irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: enables the inputted IRQ line that is passed in 
 * 				   via irq_num.
 */
void
enable_irq(uint32_t irq_num)
{
	uint8_t bitmask, slave, master;

	//check if irq_num is a valid input
	if(irq_num > 15 || irq_num < 0) {
		return;
	}
	//Master PIC 	, check if irq_num is valid for master pic
	else if(irq_num >= 0 && irq_num <= 7) {
		bitmask = (~(1 << irq_num));
		master = inb(MASTER_8259_PORT + 1); //read from 0x21
		outb( (master & bitmask), (MASTER_8259_PORT + 1)); 
	}
	//Slave PIC, check if irq_num is a valid input for slave
	else if(irq_num > 7 && irq_num < 15) {
		bitmask = (~(1<<(irq_num-8)));
		slave = inb(SLAVE_8259_PORT + 1); //read from 0xA1
		outb( (slave & bitmask), (SLAVE_8259_PORT + 1));
	}
}




// Checkpoint 1
/* 
 *   disable_irq(uint32_t irq_num)
 *   DESCRIPTION: Disable (mask) the specified IRQ  
 *   INPUTS: irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: disable the inputted IRQ line
 */
void
disable_irq(uint32_t irq_num)
{
	uint8_t bitmask, slave, master;

	//check if irq_num is a valid input
	if(irq_num > 15 || irq_num < 0) {
		return;
	}
	//Master PIC 	, check if irq_num is valid for master pic
	else if(irq_num >= 0 && irq_num <= 7) {
		master = inb(MASTER_8259_PORT + 1); //read in from x21
		bitmask = (1<<irq_num);
		outb( (master | bitmask), (MASTER_8259_PORT + 1));
	}
	//Slave PIC, check if irq_num is a valid input for slave
	else if(irq_num > 7 && irq_num < 15) {
		slave = inb(SLAVE_8259_PORT + 1); //read in from 0xA1
		bitmask = (1<<(irq_num - 8));
		outb( (slave | bitmask), (SLAVE_8259_PORT + 1));
	}
}



// Checkpoint 1
/* 
 *   send_eoi(uint32_t irq_num)
 *   DESCRIPTION: Send end-of-interrupt signal for the 
 *                specified IRQ.
 *   INPUTS: irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none 
 *   SIDE EFFECTS: sends an end_of_interrupt signal 
 * 				   to the inputted irq_num
 */
void
send_eoi(uint32_t irq_num)
{
	//check if irq_num is a valid input
	//not possible for it to be greater than 15
	//or less than 0 due to the PIC only having
	//0-15 ports
	if(irq_num > 15 || irq_num < 0) {
		return;
	}
		
	//check if irq_num is in master or if it is in slave
	else if(irq_num <= 7 && irq_num >= 0) {
 		//send EOI to master pic
		outb( (EOI | irq_num), MASTER_8259_PORT); //EOI defined in header
		//
	}
	//Slave PIC, check if irq_num is a valid input for slave
	else if(irq_num > 7 && irq_num <= 15) {
		//send EOI to slave pic
		//decrease irq_num by 8 and OR with EOI
		//for proper signal
		outb( (EOI | (irq_num - 8)) , SLAVE_8259_PORT);
		outb( (EOI + 2) , MASTER_8259_PORT);
	}


}



