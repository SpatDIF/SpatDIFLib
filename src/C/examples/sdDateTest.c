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


#include "sdDate_c.h"
#include <stdio.h>

int main(void){

	sdDate *date = sdDate_new(2014, 5, 27);
	printf("date A: %u-%u-%u\n", sdDate_getYear(date), sdDate_getMonth(date), sdDate_getDay(date));
	//set another date
	sdDate_setDate(date, 2013, 12, 25);
	printf("date B: %u-%u-%u\n", sdDate_getYear(date), sdDate_getMonth(date), sdDate_getDay(date));

	sdDate_setCurrentDate(date);
	printf("today: %u-%u-%u\n", sdDate_getYear(date), sdDate_getMonth(date), sdDate_getDay(date));

	sdDate_setYear(date, 1999);
	sdDate_setMonth(date, 12);
	sdDate_setDay(date, 31);

	char dateString[32];
	sdDate_getDateAsString(date, dateString);
	printf("the last day of the 20th century: %s\n", dateString );

	char dateC[] = "2005-10-11";
	sdDate_setDateWithString(date, dateC);
	printf("date C: %u-%u-%u\n", sdDate_getYear(date), sdDate_getMonth(date), sdDate_getDay(date));

	return 0;
}

