/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#include "sdDate.h"
#include "sdcDate.h"
#include <string.h>

extern "C" {

	sdDate* sdDate_new(unsigned short year, unsigned short month, unsigned short day){
		return new sdDate(year, month, day);
	}

	void sdDate_delete(sdDate* date){
		delete date;
	}

	void sdDate_setDate(sdDate* date, unsigned short year, unsigned short month, unsigned short day){
		date->setDate(year, month, day);
	}

	void sdDate_setCurrentDate(sdDate* date){
		date->setCurrentDate();
	}

	void sdDate_setYear(sdDate* date, unsigned short year){
		date->setYear(year);
	}

	void sdDate_setMonth(sdDate* date, unsigned short month){
		date->setMonth(month);
	}

	void sdDate_setDay(sdDate* date, unsigned short day){
		date->setDay(day);
	}

	unsigned short sdDate_getYear(sdDate* date){
		return date->getYear();
	}

	unsigned short sdDate_getMonth(sdDate* date){
		return date->getMonth();
	}

	unsigned short sdDate_getDay(sdDate* date){
		return date->getDay();
	}
	
	void sdDate_getDateAsString(sdDate* date, char* dateString){
		strcpy( dateString, date->getDateAsString().c_str());
	}

	void sdDate_setDateWithString(sdDate* date, char* dateString){
		date->setDate(dateString);
	}

}