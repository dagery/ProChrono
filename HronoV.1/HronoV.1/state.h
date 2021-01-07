#ifndef STATE_H_
#define STATE_H_


#define STATE_IDLE					0
#define STATE_PROCESSED				1
#define STATE_PROCESSING			2
#define STATE_MAIN_SCREEN_UPDATE    3
#define STATE_ERORR					4
#define STATE_MENU_SCREEN_UPDATE    5
#define STATE_MENU_IDLE			    6

uint8_t stateGet(void); //Get current device state 
uint8_t stateSet(uint8_t); //Set device state

uint8_t *ptrState; //Faster way to access current state variable 

#endif