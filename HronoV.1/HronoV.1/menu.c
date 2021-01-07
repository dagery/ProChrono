
#include "menu.h"
#include "state.h"
#include "settings.h"
#include <stdint.h>
#include <avr/pgmspace.h>

#define false (uint8_t)0
#define true (uint8_t)1

const char menuText_Mode[] PROGMEM = "Mode : %s";
const char menuText_Language[] PROGMEM = "Language : %s";
const char menuText_BulletMass[] PROGMEM = "Bullet mass: %.1f g";
const char menuText_Exit[] PROGMEM = "Exit";

static char *menuVars_Mode[] = {"Single" , "Multiple"};
static char *menuVars_Language[] = {"RU" , "EN" , "UA"};


struct MenuElement menu[] = {
	// First menu element, always selected
	// Mode
	{
		.selected = true,
		.str = menuText_Mode,
		
		.vars = menuVars_Mode,
		.var_selected = &settings.mode,
		.vars_count = 2,
		
		.onSelected = &scrollValue
	},
	//BulletMass
	{
		.selected = false,
		.str = menuText_BulletMass,
		
		.var_inputFloat = &settings.bullet_mass,
		.vars_count = 1,
		
		.onSelected = &FloatInput
	},
	//Language
	{
		.selected = false,
		.str = menuText_Language,
		
		.vars = &menuVars_Language,
		.var_selected = &settings.language,
		.vars_count = 3,
		
		.onSelected = &scrollValue
	},
	// Exit
	{
		.selected = false,
		.str = menuText_Exit,
		.onSelected = &onExit
	}
};
struct MenuElement *currentMenuActiveRoot;

void empty(){}

void FloatInput(struct MenuElement *mElement){
	
}



//Exit to previous menuElement
void onExit(struct MenuElement *mElement){
	//*(mElement)->selected = 0;
	mElement->selected = false;
	menu[0].selected = true;
	stateSet(STATE_MAIN_SCREEN_UPDATE);
}
/*
	Function for scrolling menu element parameters every
	time selection button pressed.  
*/
void scrollValue(struct MenuElement *mElement){
	*(mElement)->var_selected += 1;
	if (*mElement->var_selected >= mElement->vars_count) {*mElement->var_selected = 0;};
	stateSet(STATE_MENU_SCREEN_UPDATE);
}