/*
 * Potentiometer.c
 * Added by Devon Cormack 01/29/2015
 *
 */

#include "Potentiometer.h"
#include "stm32f4xx.h"

//Assign the history some default values.
//history[0] is the most recent input value
//history[3] is the least recent
static uint32_t history[4] = {(0x0),(0x0),(0x0),(0x0)};

//Enables GPIO Port A1 for analog use.
void enablePA1Analog(){
	//Enable port A
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	//Set PA1 to analog so the POT can communicate.
	GPIOA->MODER |= (0xC); //PA1 analog is 11 in bits 2 and 3
}

//Initializes the potentiometer for use
void ADC_init(void){

	//Prepare GPIOA1 for analog input.
	enablePA1Analog();

	//Turn on the ADC clock on
	RCC->APB2ENR |= ADC_ENABLE;

	//Set the sequence length of the ADC read to 1 port
	ADC->SQR1 &= ADC_SET_SEQUENCE_LENGTH_1; //this could  be 0

	//Set the first regular sequence conversion to be from port A1
	ADC->SQR3 |= ADC_SET_1ST_REGULAR_SEQ_CONVERSION; //does stepping on the rest matter here?

	//Set the sample time for the ADC to 15 for channel 1
	ADC->SMPR2 |= (0x8);

	//Now that everything is set up, enable conversions.
	ADC->CR2 |= ADC_CR2_ADON;
	uint32_t test = ADC->CR2;
}

//Reads in a value from the Analog to Digital Converter and returns it
uint32_t ADC_read(void){
	//Initiate a read by writing 1 to SWSTART (bit 30)
	ADC->CR2 |= ADC_CR2_SWSTART;
	uint32_t read = 0;
	while(read == 0){
		//The status hasn't been converted yet, check again
		read = ADC->SR;
		uint32_t mask = ADC_SR_EOC;
		read = read & mask;
	}

	//Clear the status read bit by reading from Data Register.
	uint32_t data = ADC->DR & ADC_DR_MASK;
	return data;
}

/**
 * ADC_sample_on_USER:
 * Samples the potentiometer when the USER input button
 * is pressed. Waits until the user input button is pressed
 * before sampling. Returns sampled value.
 */
uint32_t ADC_sample_on_USER(void){
	//Wait for USER press
	USER_debounce();
	return ADC_read();
}

/**
 * ADC_sample_on_USER_with_History:
 * Samples the potentiometer when the USER input button
 * is pressed. Waits until the user input button is pressed
 * before sampling. Prints out history after sampling.
 */
void ADC_sample_on_USER_with_History(void){
	//Wait for USER press
	USER_debounce();
	addToHistory( ADC_read() );
	printHistory();
}

/**
 * printHistory:
 *
 * Prints the last 4 samples from the ADC to the screen in a nice format.
 *
 */
void printHistory(void){
	static char numbers[] = "1234";
	int i;
	print("The last 4 samples were: \n\r\0");
	for(i = 0; i < 4; i++){
		print("Sample: \0");
		printHex(history[i]);
	}
}

/**
 * addToHistory(uint32_t):
 *
 * Adds the given value to the history.
 *
 */
void addToHistory(uint32_t value){
	int i;
	uint32_t temp;
	for(i = 2; i >= 0; i--){
		history[i+1] = history[i];
	}
	history[0] = value;
}
