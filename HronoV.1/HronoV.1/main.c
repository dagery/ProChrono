/*
 * HronoV.1.c
 *
 * Created: 25.12.2019 18:10:48
 * Author : admin
 */ 
#include "conf.h"
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <avr/eeprom.h>

#include "oled/lcd.h"
#include "usart.h"
#include "buttons.h"
#include "menu.h"
#include "state.h"
#include "settings.h"

#define TIMER_PERIOD 0.016 //ms

const uint8_t buletImg[] PROGMEM = 
{
	// 'bulet', 21x9px
	0x00, 0xff, 0x80, 0x19, 0xff, 0xe0, 0x01, 0xfd, 0xf0, 0x31, 0x7e, 0xf8, 0x01, 0x7f, 0xf8, 0x61,
	0xff, 0xf8, 0x01, 0xff, 0xf0, 0xc1, 0xff, 0xe0, 0x00, 0xff, 0x80
};



struct DATA
{
	double timeCounter;     // total time
	double speed;           // speed    
	double energy;
	
	int _buff_counter;
	double lastShoots[25];
	double maxSpeed;
	double minSpeed;
}data;

void DisplayUpdate();
void DisplayMenu();
void Calculate();

uint8_t state;

void Init(){
	
	uart0_init(BAUD_CALC(115200));
	
	// Initialize ports for buttons
	DDRC	= (0 << PORTC0)|(0 <<PORTC1)|(0 << PORTC2); // Set buttons ports to inputs
	PORTC	= (1 << PORTC0)|(1 <<PORTC1)|(1 << PORTC2); // Enable pull-up resistors
	DDRD = (0 << PORTD3)|(0 << PORTD2);
	//Initialize Timer0 for time measurements
	TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
	TCCR0B=(0<<WGM02) | (0<<CS02) | (0<<CS01) | (0<<CS00); //Timer Off
	TCNT0=0x00;
	OCR0A=0x00;
	OCR0B=0x00;

	//Initialize interrupts
	// Timer/Counter 0 Interrupt(s) initialization
	TIMSK0=(0<<OCIE0B) | (0<<OCIE0A) | (1<<TOIE0); 
	// External Interrupt(s) initialization
	// INT0: On
	// INT0 Mode: Rising Edge
	// INT1: On
	// INT1 Mode: Rising Edge
	// Interrupt on any change on pins PCINT0-7: Off
	// Interrupt on any change on pins PCINT8-14: Off
	// Interrupt on any change on pins PCINT16-23: Off
	EICRA=(1<<ISC11) | (1<<ISC10) | (1<<ISC01) | (1<<ISC00);
	EIMSK=(1<<INT1) | (1<<INT0);
	EIFR=(1<<INTF1) | (1<<INTF0);
	PCICR=(0<<PCIE2) | (0<<PCIE1) | (0<<PCIE0);
	
	//Initialize OLED Display
	lcd_init(LCD_DISP_ON); 
	//Draw welcome screen:
	#ifdef WELCOME_SCREEN
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts("v");
		lcd_puts(VERSION);
		lcd_charMode(DOUBLESIZE);
		lcd_gotoxy(2,4);
		lcd_puts("ProHrono");
		lcd_charMode(NORMALSIZE);
		lcd_drawBitmap(64 , 20 , buletImg , 21,9 , WHITE);
		lcd_display();
		_delay_ms(3000);
	#endif
	
	stateSet(STATE_MAIN_SCREEN_UPDATE);
}

int main(void)
{
	Init();
	_delay_ms(100);
	sei();
    while (1) 
	{
		switch(stateGet()){
			case STATE_MAIN_SCREEN_UPDATE:
				Calculate();
				DisplayUpdate();
				stateSet(STATE_IDLE);
			break;
			case STATE_MENU_SCREEN_UPDATE:
				DisplayMenu();
				stateSet(STATE_MENU_IDLE);
			break; 
		}
		
	
		if(stateGet() == STATE_MENU_IDLE){
			uint8_t curr_selecteed = 0;
			for(curr_selecteed = 0; curr_selecteed < 4; curr_selecteed++){
				if(menu[curr_selecteed].selected)
					break;
			}
			
			if(button_down(BUTTON_UP)){
				if(curr_selecteed > 0){
					menu[curr_selecteed].selected = 0; 
					curr_selecteed--;
					menu[curr_selecteed].selected = 1; 
					stateSet(STATE_MENU_SCREEN_UPDATE);
				}
			}else if (button_down(BUTTON_DOWN)){
				if(curr_selecteed < 3){
					menu[curr_selecteed].selected = 0;
					curr_selecteed++;
					menu[curr_selecteed].selected = 1;
					stateSet(STATE_MENU_SCREEN_UPDATE);
				}
			}else if (button_down(BUTTON_ENTER)){
				menu[curr_selecteed].onSelected(&menu[curr_selecteed]);
			}
				
		}else{
			if (button_down(BUTTON_ENTER))
			{
				stateSet(STATE_MENU_SCREEN_UPDATE);
			}
		}
		
		buttons_update();
    }
}

void Calculate(){
	float time = data.timeCounter; //time in ms
	if(!(time <= 0)){
		data.speed = ((double)DISTANCE/1000.0)/(time/1000.0);

		data.energy = ((settings.bullet_mass/1000.0) * (data.speed * data.speed))/2.0;
	
		if(data._buff_counter++ > 25) data._buff_counter = 0;
	
		data.lastShoots[data._buff_counter] = data.speed;
	
		data.maxSpeed = data.lastShoots[0];
		int i;
		for (i = 0; i < 25; i++)
		{
			if(data.maxSpeed < data.lastShoots[i])
			data.maxSpeed = data.lastShoots[i];
		} 
	}else{
		data.speed = 0;
		data.energy = 0;
		data.maxSpeed = 0;
	}
}



void DisplayMenu(){
	
	char buffer[32] = {};
	lcd_clrscr();
	lcd_gotoxy(9 , 0);
	lcd_puts("Menu");

	int i;
	for (i = 0 ; i < 4; i++)
	{
		lcd_gotoxy(0 , 1 + i);
		
		if(menu[i].selected)
			lcd_puts(">");
		else
			lcd_puts(" ");
		
		if(menu[i].vars_count > 0){
			if(menu[i].vars_count > 1){
				sprintf_P(buffer , menu[i].str ,(*menu[i].vars)[(*menu[i].var_selected)]);
			}else{
				sprintf_P(buffer , menu[i].str ,(*menu[i].var_inputFloat));
			}
		}else{
			sprintf_P(buffer , menu[i].str);
		}
		lcd_puts(buffer);
		lcd_display();
	}
	
	
	
}

void DisplayUpdate(){
	
	char buffer[32] = {};
	lcd_clrscr();
	lcd_charMode(DOUBLESIZE);
	lcd_gotoxy(0,0);
	sprintf(buffer , "%05.1lf m/s" , data.speed);
	lcd_puts(buffer);
	lcd_gotoxy(0,2);
	if(data.energy < 1){
		sprintf(buffer , "%05.2f mJ" , data.energy*1000.0);
	}else{
		sprintf(buffer , "%05.1f  J" , data.energy);
	}
	lcd_puts(buffer);
	
	lcd_charMode(NORMALSIZE);
	lcd_gotoxy(0,5);
	sprintf(buffer , "max:%05.1lf m/s" , data.maxSpeed);
	lcd_puts(buffer);
	lcd_display();
}

ISR(INT1_vect){
	if(stateGet() == STATE_IDLE){
		TCNT0=0x00;
		TCCR0B=(1<<CS00);
		data.timeCounter = 0;
		stateSet(STATE_PROCESSING);
	}
}

ISR(INT0_vect){
	TCCR0B=(0<<CS00);
	data.timeCounter += TCNT0 * (double)TIMER_PERIOD;
	if(stateGet() == STATE_PROCESSING){
		stateSet(STATE_MAIN_SCREEN_UPDATE);
	}
}

ISR(TIMER0_OVF_vect){
	data.timeCounter += (double)TIMER_PERIOD;
	if(data.timeCounter > 80){
		TCCR0B = (0<<CS00);
		TCNT0=0x00;
		data.timeCounter = 0;
		stateSet(STATE_IDLE);
	}
}