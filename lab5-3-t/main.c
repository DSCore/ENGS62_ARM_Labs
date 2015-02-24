#include "fsm.h"
#include "USART2.h"
#include "USART3.h"
#include "userbutton.h"
#include "Wifly.h"

void USART2_callback_fn(uint8_t byte)
{
	USART3_putchar(byte); //forward to wifi module
}

void USART3_callback_fn(uint8_t byte)
{
	USART2_putchar(byte); //forward to console
}

void userbutton_callback_fn(void)
{
//	if( fsm_lock() == FSM_LOCK_ACQUIRED )
//	{
//		if(fsm_get_state() == STATE_1 || fsm_get_state == STATE_RESET){
//
//		}
//		fsm_set_state(STATE_RESET);
//		fsm_unlock();
//	}
}


void main(void)
{
	/* Set up the USART2 9600-8N1 and to call USART2_callback_fn when new data arrives */
	USART2_init(USART2_callback_fn);

	/* Set up the USART3 9600-8N1 and to call USART2_callback_fn when new data arrives */
	USART3_init(USART3_callback_fn);

	Wifly_Send_Ping();
//	/* Configure user pushbutton and call pushbutton_callback_fn when button press-released */
// 	userbutton_init(userbutton_callback_fn);

//	/* initialize the finite state machine */
//	fsm_init();

	/* Enable interrupts - do this after initializing the system */
	__asm ("  cpsie i \n" );

	//Put the wifi module into command mode
//	USART3_putchar('$');
//	USART3_putchar('$');
//	USART3_putchar('$');
//	USART2_putchar('$');
//	USART2_putchar('$');
//	USART2_putchar('$');//	USART2_putstr("$$$\n\r\0");

	/* Wait here forever - everything is now interrupt driven */
	while(1)
	{
		;;;
	}
}


/*
 * void printHex(uint32_t value):
 *
 * Prints the hexadecimal representation of value.
 */
void printHex(uint32_t value){
	//Print out 0x
	uint8_t zero = 48;
	USART2_putchar(48);
	uint8_t x = 120;
	USART2_putchar(x);

	static char hex_digits[] = "0123456789ABCDEF";

	//Print out the hexadecimal representation of the number
	uint32_t mask = 0xF0000000;
	int i;
	for( i = 7; i >= 0; i-- ){
		//Mask out all but the highest 4 bits
		uint32_t masked_value = value & mask;
		//Shift off all but the highest 4 bits
		masked_value = masked_value >> 4*i;
		mask = mask >> 4;
		//Print out the hexadecimal representation of the value
		USART2_putchar(hex_digits[masked_value]);
	}
	USART2_putchar('\n');
	USART2_putchar('\r');
}

