/*
 * buttons.c
 *
 * Created: 20.01.2020 1:05:38
 *  Author: admin
 */ 

#include "buttons.h"

uint8_t buttons_down = 0;
uint8_t buttons_pressed = 0;
uint8_t buttons_up = 0;

uint8_t buttons_scanArray[]={BUTTON_UP , BUTTON_DOWN , BUTTON_ENTER};

void buttons_init(){
	BUTTONS_DDR = (0 << BUTTON_UP)|(0 << BUTTON_DOWN)|(0 << BUTTON_ENTER);
}

uint8_t button_down(uint8_t bCode){
	if(buttons_down & (1 << bCode)){
		return 1;
	}else{
		return 0;
	}	
}

uint8_t button_pressed(uint8_t bCode){
	if(buttons_pressed & (1 << bCode)){
		return 1;
	}else{
		return 0;
	}
}

uint8_t button_up(uint8_t bCode){
	if(buttons_up & (1 << bCode)){
		return 1;
	}else{
		return 0;
	}
}

void buttons_update(){
	int i;
	for(i = 0; i < sizeof(buttons_scanArray); i++){
		if(BUTTONS_PIN & (1 << buttons_scanArray[i])){
			if(buttons_down & (1 << buttons_scanArray[i]) ){
				buttons_down &= ~(1 << buttons_scanArray[i]);
				buttons_pressed |= (1 << buttons_scanArray[i]);
				return;
			}else if(buttons_pressed & (1 << buttons_scanArray[i])){
				return;
			}else{
				buttons_down |= (1 << buttons_scanArray[i]);
			}
		}else{
			if (buttons_pressed & (1 << buttons_scanArray[i]))
			{
				buttons_pressed &= ~(1 << buttons_scanArray[i]);
				buttons_up |= (1 << buttons_scanArray[i]);
			} 
			else
			{
				buttons_up &= ~(1 << buttons_scanArray[i]);
			}
		}
	}
}