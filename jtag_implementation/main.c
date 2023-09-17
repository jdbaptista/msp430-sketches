#include <msp430.h>
#include <jtag_fsm.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	volatile uint16_t output;
	volatile uint8_t ir_output;
	while (true) {
	    initFSM();
	    ir_output = IR_SHIFT((uint8_t) 0x83);
	    output = DR_SHIFT((uint16_t) 0xa000);
	    output = DR_SHIFT((uint16_t) 0x0012);
	    output = DR_SHIFT((uint16_t) 0x5555);
	}
}
