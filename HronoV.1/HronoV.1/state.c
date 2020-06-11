/*
 * state.c
 *
 * Created: 27.04.2020 19:43:27
 *  Author: admin
 */ 
#include <stdio.h>
#include "state.h"

extern uint8_t STATE = STATE_IDLE; 

uint8_t stateGet(void){
	return STATE;
}

uint8_t stateSet(uint8_t state){
	STATE = state;
	return 1;
}