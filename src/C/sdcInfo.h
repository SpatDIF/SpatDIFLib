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

#ifndef ____sdInfo_
#define ____sdInfo_


#ifdef __cplusplus
extern "C" {
#endif

#include "sdcDate.h"
// sdInfo dummy
typedef struct sdInfo sdInfo;

// wrapper functions 
sdInfo* sdInfo_new();
void sdInfo_setAuthor(sdInfo* info, char* author);
void sdInfo_setHost(sdInfo* info, char* host);
void sdInfo_setLocation(sdInfo* info, char* location);
void sdInfo_setSession(sdInfo* info, char* session);
void sdInfo_setAnnotation(sdInfo* info, char* annotation);
void sdInfo_setDate(sdInfo* info, sdDate* date);
void sdInfo_setDateWithString(sdInfo* info, char* dateString);
void sdInfo_set(sdInfo* info, char* author, char* host, char* dateString, char* session, char* location, char* annotation);
const char* sdInfo_getAuthor(sdInfo* info);
const char* sdInfo_getHost(sdInfo* info);
const char* sdInfo_getLocation(sdInfo* info);
const char* sdInfo_getSession(sdInfo* info);
const char* sdInfo_getAnnotation(sdInfo* info);
const char* sdInfo_getDateAsString(sdInfo* info);
void sdInfo_getDate(sdInfo* info, sdDate* date);


#ifdef __cplusplus
}
#endif
#endif

