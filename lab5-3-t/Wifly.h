#include <stdint.h>

#include <USART3.h>
#include <USART2.h>


//Possible types of the ping
#define PING 1

//Possible ids of the ping
#define ID 7

//Define the ping data structure
typedef struct {
	uint32_t type;
	uint32_t id;
}Ping_t;

void Wifly_Send_Ping();

void Wifly_Receive_Ping(uint8_t byte);

void Wifly_Print_Last_Received_Message(void);

