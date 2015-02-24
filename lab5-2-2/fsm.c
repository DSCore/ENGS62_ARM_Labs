#include <fsm.h>
#include <stdint.h>

#include <LED.h>
#include <mutex.h>
#include <USART2.h>

static uint32_t fsm_mutex;


static Ping_t buffer;
static uint32_t rcvd = 0;

static uint32_t message_reported = 0;

static state_t state = STATE_RESET;

/** Callback functions for states: **/

//Initialization states
static uint8_t out_of_CMD = 0;
static uint8_t has_been_initialized = 0;
static uint8_t init_input = '\0';
static uint8_t *init_input_end = &init_input;

void state_INIT_USART2_callback_fn(uint8_t byte){
	//Do nothing, no user input allowed.
}

void state_INIT_USART3_callback_fn(uint8_t byte){
	USART2_putchar(byte);
	*init_input_end = byte;
	init_input_end++;
	init_input_end = '\0';
	if(strcmp(&init_input,"AOK\0") || strcmp(&init_input,"ERR\0")){
		out_of_CMD = 1;
	}
	if(fsm_get_state() == STATE_INIT && has_been_initialized != 1 && out_of_CMD == 1){
		fsm_set_state(STATE_INIT_a);
	}
	if(strcmp(&init_input,"AOK\0") == 1){
		//Advance the fsm to the next state.
			if( fsm_lock() == FSM_LOCK_ACQUIRED )
			{
				if(fsm_get_state() == STATE_INIT_a){
					fsm_set_state(STATE_INIT_b);
				}
				else if(fsm_get_state() == STATE_INIT_b){
					fsm_set_state(STATE_INIT_c);
				}
				else if(fsm_get_state() == STATE_INIT_c){
									fsm_set_state(STATE_INIT_d);
								}
				else if(fsm_get_state() == STATE_INIT_d){
									fsm_set_state(STATE_INIT_e);
								}
				else if(fsm_get_state() == STATE_INIT_e){
									fsm_set_state(STATE_INIT_f);
								}
				else if(fsm_get_state() == STATE_INIT_f){
									fsm_set_state(STATE_INIT_g);
								}
				else if(fsm_get_state() == STATE_INIT_g){
									fsm_set_state(STATE_INIT_h);
								}
				else if(fsm_get_state() == STATE_INIT_h){
									fsm_set_state(STATE_INIT_i);
								}
				else if(fsm_get_state() == STATE_INIT_i){
									fsm_set_state(STATE_INIT_j);
								}
				else if(fsm_get_state() == STATE_INIT_j){
									fsm_set_state(STATE_RESET);
								}

				fsm_unlock();
			}
	}
}

//void state_INIT_a_USART3_callback_fn(uint8_t byte){
//	USART2_putchar(byte);
//	init_input_end = byte;
//	init_input_end++;
//	init_input_end = '\0';
//	if(strcmp(init_input,"") == 1){
//		//Move to next state
//	}
//}
//
//void state_INIT_b_USART3_callback_fn(uint8_t byte){
//	USART2_putchar(byte);
//	init_input_end = byte;
//	init_input_end++;
//	init_input_end = '\0';
//	if(strcmp(init_input,"") == 1){
//		//Move to next state
//	}
//}
//
//void state_INIT_c_USART3_callback_fn(uint8_t byte){
//	USART2_putchar(byte);
//	init_input_end = byte;
//	init_input_end++;
//	init_input_end = '\0';
//	if(strcmp(init_input,"") == 1){
//		//Move to next state
//	}
//}
//
//void state_INIT_d_USART3_callback_fn(uint8_t byte){
//	USART2_putchar(byte);
//	init_input_end = byte;
//	init_input_end++;
//	init_input_end = '\0';
//	if(strcmp(init_input,"") == 1){
//		//Move to next state
//	}
//}
//
//void state_INIT_e_USART3_callback_fn(uint8_t byte){
//	USART2_putchar(byte);
//	init_input_end = byte;
//	init_input_end++;
//	init_input_end = '\0';
//	if(strcmp(init_input,"") == 1){
//		//Move to next state
//	}
//}
//
//void state_INIT_f_USART3_callback_fn(uint8_t byte){
//	USART2_putchar(byte);
//	init_input_end = byte;
//	init_input_end++;
//	init_input_end = '\0';
//	if(strcmp(init_input,"") == 1){
//		//Move to next state
//	}
//}
//
//void state_INIT_g_USART3_callback_fn(uint8_t byte){
//	USART2_putchar(byte);
//	init_input_end = byte;
//	init_input_end++;
//	init_input_end = '\0';
//	if(strcmp(init_input,"") == 1){
//		//Move to next state
//	}
//}
//
//void state_INIT_h_USART3_callback_fn(uint8_t byte){
//	USART2_putchar(byte);
//	init_input_end = byte;
//	init_input_end++;
//	init_input_end = '\0';
//	if(strcmp(init_input,"") == 1){
//		//Move to next state
//	}
//}
//
//void state_INIT_i_USART3_callback_fn(uint8_t byte){
//	USART2_putchar(byte);
//	init_input_end = byte;
//	init_input_end++;
//	init_input_end = '\0';
//	if(strcmp(init_input,"") == 1){
//		//Move to next state
//	}
//}
//
//void state_INIT_j_USART3_callback_fn(uint8_t byte){
//	USART2_putchar(byte);
//	init_input_end = byte;
//	init_input_end++;
//	init_input_end = '\0';
//	if(strcmp(init_input,"") == 1){
//		//Move to next state
//	}
//}

void state1_USART2_callback_fn(uint8_t byte)
{
	USART3_putchar(byte); //forward to wifi module
}

void state1_USART3_callback_fn(uint8_t byte)
{
	USART2_putchar(byte); //forward to console
}

void receive_message_systick_callback(){
	//Print out the received message
	USART2_putstr("\n\rReceived!\n\r\0");
	Wifly_Print_Last_Received_Message();
	//Set the message reported flag to 1;
	message_reported = 1;
	//Disable the systick timer
	set_systick_disabled();
	//Restart the state to send the message again.
	if( fsm_lock() == FSM_LOCK_ACQUIRED )
		{
			if(fsm_get_state() == STATE_2){
				fsm_set_state(STATE_2);
			}
			fsm_unlock();
		}
}

void state2_USART2_callback_fn(uint8_t byte)
{
	//Take no input, we're in PING mode
	//USART3_putchar(byte); //forward to wifi module
}

void state2_USART3_callback_fn(uint8_t byte)
{
	Wifly_Receive_Ping(byte);
//	USART2_putchar(byte+48); //TODO: This is debug statement, delete it
}



void fsm_init(void)
{
	mutex_init(&fsm_mutex);
	while( fsm_lock() != FSM_LOCK_ACQUIRED );
	fsm_set_state(STATE_INIT);
	fsm_unlock();
}

uint8_t fsm_lock(void)
{
	uint8_t retval = FSM_LOCK_FAILED;

	if( mutex_lock(&fsm_mutex) == MUTEX_LOCKED )
		retval = FSM_LOCK_ACQUIRED;

	return retval;
}

void fsm_unlock(void)
{
	mutex_unlock(&fsm_mutex);
}

state_t fsm_get_state(void)
{
	return state;
}

void fsm_set_state(state_t new_state)
{
	/* only do this if the FSM has been locked! */
	if( fsm_mutex == MUTEX_LOCKED )
	{
		state = new_state;

		switch( state )
		{
		case STATE_RESET:
		case STATE_1:
		default:
			/* Initialize the LEDs */
			LED_init();

			/* Initialize the USART2 for x-over internet communication */
			USART2_init(state1_USART2_callback_fn);

			/* Initialize the USART3 for x-over internet communication */
			USART3_init(state1_USART3_callback_fn);

			/* Turn on just the blue LED */
			LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_ON | LED_GREEN_OFF );
			break;

		case STATE_2:
			/* Turn on the orange LED only */
			LED_update( LED_ORANGE_ON | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_OFF );

			//Set up the USART interrupts to buffer the received data from the PING
			USART3_init(state2_USART3_callback_fn);
			USART2_init(state2_USART2_callback_fn);

			//Send a ping message to the server
			Wifly_Send_Ping();

			//Start up a 1 second timer - wait 1 second then print
			message_reported = 0;
			systick_init(receive_message_systick_callback);
			set_systick_time(1);
			break;

		case STATE_INIT:
			//Set up the USART interrupts to handle the initialization correctly
			USART3_init(state_INIT_USART3_callback_fn);
			USART2_init(state_INIT_USART2_callback_fn);
//			USART2_putstr("Initializing!\n\r\0");
			init_input = '\0';
			USART3_putstr("EXIT\n\r\0");
//			out_of_CMD = 1;
//			break;

		case STATE_INIT_a:
//			USART2_putstr("$$$\n\r\0");
			init_input = '\0';
			USART3_putstr("$$$\n\r\0");
			break;

		case STATE_INIT_b:
			USART2_putstr("set wlan join 1\n\r\0");
			init_input = '\0';
			USART3_putstr("set wlan join 1\n\r\0");
			break;

		case STATE_INIT_c:
			USART2_putstr("set wlan auth 4\n\r\0");
			init_input = '\0';
			USART3_putstr("set wlan auth 4\n\r\0");
			break;

		case STATE_INIT_d:
			USART2_putstr("set wlan ssid ENGS62\n\r\0");
			init_input = '\0';
			USART3_putstr("set wlan ssid ENGS62\n\r\0");
			break;

		case STATE_INIT_e:
			USART2_putstr("set wlan phrase Engs62connect\n\r\0");
			init_input = '\0';
			USART3_putstr("set wlan phrase Engs62connect\n\r\0");
			break;

		case STATE_INIT_f:
			USART2_putstr("set ip dhcp 1");
			init_input = '\0';
			USART3_putstr("set ip dhcp 1");
			break;

		case STATE_INIT_g:
			USART2_putstr("set ip addr 129.170.66.33\n\r\0");
			init_input = '\0';
			USART3_putstr("set ip addr 129.170.66.33\n\r\0");
			break;

		case STATE_INIT_h:
			USART2_putstr("set ip remote 8880\n\r\0");
			init_input = '\0';
			USART3_putstr("set ip remote 8880\n\r\0");
			break;

		case STATE_INIT_i:
			USART2_putstr("set ip protocol 0\n\r\0");
			init_input = '\0';
			USART3_putstr("set ip protocol 0\n\r\0");
			break;

		case STATE_INIT_j:
			USART2_putstr("save\n\r\0");
			init_input = '\0';
			USART3_putstr("save\n\r\0");
			has_been_initialized = 1;
			break;
		}
	}
}
