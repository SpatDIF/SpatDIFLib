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

#ifndef ____sdDate_c_
#define ____sdDate_c_


#ifdef __cplusplus
extern "C" {
#endif

// sdDate dummy
typedef struct sdDate sdDate;

// wrapper functions 
sdDate* sdDate_new(unsigned short year, unsigned short month, unsigned short day);
void sdDate_delete(sdDate* date);

void sdDate_setDate(sdDate* date, unsigned short year, unsigned short month, unsigned short day);
void sdDate_setCurrentDate(sdDate* date);

void sdDate_setYear(sdDate* date, unsigned short year);
void sdDate_setMonth(sdDate* date, unsigned short month);
void sdDate_setDay(sdDate* date, unsigned short day);
void sdDate_setDateWithString(sdDate* date, char* dateString);

unsigned short sdDate_getYear(sdDate* date);
unsigned short sdDate_getMonth(sdDate* date);
unsigned short sdDate_getDay(sdDate* date);
void sdDate_getDateAsString(sdDate* date, char* dateString);

#ifdef __cplusplus
}
#endif
#endif

