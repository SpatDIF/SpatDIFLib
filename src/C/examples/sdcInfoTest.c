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


#include "sdcInfo.h"
#include <stdio.h>

int main(void){

	sdInfo *info = sdInfo_new();
	char author1[] = "johannes";
	char host1[] = "myHost";
	char date1[] = "2014-5-31";
	char session1[] = "1.2";
	char location1[] = "cologne";
	char annotation1[] = "this is first test";

	char author2[] = "thomas";
	char host2[] = "yourHost";
	char date2[] = "2014-6-1";
	char session2[] = "1.3";
	char location2[] = "zuerich";
	char annotation2[] = "this is second test";


	sdInfo_setAuthor(info, author1);
	sdInfo_setHost(info, host1);
	sdInfo_setDateWithString(info, date1);
	sdInfo_setSession(info, session1);
	sdInfo_setLocation(info, location1);
	sdInfo_setAnnotation(info, annotation1);

	printf("---info---\n");
	printf("author:%s\n", sdInfo_getAuthor(info));
	printf("host:%s\n",sdInfo_getHost(info));
	printf("date:%s\n",sdInfo_getDateAsString(info));
	printf("session:%s\n",sdInfo_getSession(info));
	printf("location:%s\n", sdInfo_getLocation(info));
	printf("annotation:%s\n", sdInfo_getAnnotation(info));

	sdInfo_set(info, author2, host2, date2, session2, location2, annotation2 );

	printf("---info---\n");
	printf("author:%s\n", sdInfo_getAuthor(info));
	printf("host:%s\n",sdInfo_getHost(info));
	printf("date:%s\n",sdInfo_getDateAsString(info));
	printf("session:%s\n",sdInfo_getSession(info));
	printf("location:%s\n", sdInfo_getLocation(info));
	printf("annotation:%s\n", sdInfo_getAnnotation(info));

	// create an instance of sdDate manually and pass it to get Date.
	sdDate *date = sdDate_new(0,0,0);
	sdInfo_getDate(info, date);

	printf("%d/%d/%d \n", sdDate_getYear(date), sdDate_getMonth(date), sdDate_getDay(date));


	return 0;
}

