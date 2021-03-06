/*
 * systick.c
 * By Devon Cormack 01/27/15
 */
#include "systick.h"
#include "stm32f4xx.h"
#include "LED.h" //For the LED code and values.

static const int WAIT_1_SECOND = 16000000;

/*
 * void systick_init(void)
 *
 * Set the values of CS, CR, and RV to initialize and enable SYSTICK.
 *
 */
void systick_init(void){
	//Disable interrupts for setup.
	__asm ("  cpsid i \n" );
	//Initially disable the timer
	SYSTICK->CS = 0;
	//Load a number into RV corresponding to the number
	//of processor cycles between interrupts.
	SYSTICK->RV = WAIT_1_SECOND;
	//Load 0 into CV to clear current value
	SYSTICK->CV = 0;
	//Load 7 into CS to start interrupts again.
	SYSTICK->CS = 7;
	//Re-enable interrupts
	__asm ("  cpsie i \n" );
}


/*
 * void handle_systick_blink(void)
 *
 * handle the systick timer events
 * Part 1 - Flashes green light for 1 second.
 */
void handle_systick_blink(void){
	//Get the current state of the LEDs from the IDR

	//Change the state of the LEDs
	toggle_LED(LED_GREEN_ON);
	//Update the state of the LEDs
}


/*
 * The systick Interrupt Service Routine
 */
void __attribute__ ((interrupt)) systick_handler(void)
{
	/* This code makes the green light blink on and off
	 * each second.
	 */
	handle_systick_blink();

}
