/*
 * string.c - routines for use with strings.
 */

#include "string.h"

/**
 * uint8_t strcmp(uint8_t *str1, uint8_t *str2):
 *
 * str1 - string to compare
 * str2 - string to compare
 *
 * returns 1 if str1 == str2, 0 if not.
 */
uint8_t strcmp(uint8_t *str1, uint8_t *str2){
	uint8_t match = 1;
	while( 1 ){
		//Check if *str1 and *str2 don't match or are null
		if(*str1 != *str2){
			return 0;
		}
		else if(*str1 == '\0' || *str2 == '\0'){
			if(*str1 == '\0' && *str2 == '\0'){
				return 1;
			}
			else{
				return 0;
			}
		}
		//Since *str1 and *str2 are both the same and non-null
		//move to the next characters
		str1++;
		str2++;
	}
}
