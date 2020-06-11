/*
 * settings.h
 *
 * Created: 27.04.2020 19:40:10
 *  Author: bazsem
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_

#define MODE_SINGLE 0
#define MODE_MULTIPLE 1

#define LANG_RUS 0
#define LANG_ENG 1
#define LANG_UKR 2

#define UNITS_METRIC 0
#define UNITS_IMPERIAL 1

struct Settings
{
	uint8_t mode;
	uint8_t units_type;
	uint8_t language;
	float bullet_mass;
};

extern struct Settings settings;

#endif /* SETTINGS_H_ */