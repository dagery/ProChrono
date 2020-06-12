/*
 * menu.h
 *
 * Created: 05.02.2020 19:50:13
 *  Author: admin
 */ 


#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>

extern struct MenuElement{
	uint8_t selected;
	char *str;      //Displayed element name
	
	char *(*vars)[];	//Displayed element vars
	float *var_inputFloat;
	uint8_t *var_selected;
	uint8_t vars_count;
	
	void (*onSelected)(struct MenuElement *menuElement);

};

extern struct MenuElement *currentMenuActiveRoot;
extern struct MenuElement menu[];

void onExit(struct MenuElement*);
void FloatInput(struct MenuElement*);
void empty(void);
void scrollValue(struct MenuElement*);

#endif /* MENU_H_ */