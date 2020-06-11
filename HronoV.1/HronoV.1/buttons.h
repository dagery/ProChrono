/*
 * buttons.h
 *
 * Created: 20.01.2020 1:05:00
 *  Author: admin
 */ 

#include <avr/io.h>

#ifndef BUTTONS_H_
#define BUTTONS_H_

#define BUTTONS_DDR DDRC
#define BUTTONS_PORT PORTC
#define BUTTONS_PIN PINC

#define BUTTON_UP  PORTC0
#define BUTTON_DOWN PORTC1
#define BUTTON_ENTER PORTC2


void buttons_init();
uint8_t button_down(uint8_t);
uint8_t button_pressed(uint8_t);
uint8_t button_up(uint8_t);
void buttons_update();

#endif /* BUTTONS_H_ */