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
	char author[] = "johannes";
	char location[] = "cologne";
	char annotation[] = "this is test";
	char session[] = "1.2";
	char date[] = "2014-5-31";
	char host[] = "myHost";

	sdInfo_setAuthor(info, author);
	sdInfo_setLocation(info, location);
	sdInfo_setAnnotation(info, annotation);
	sdInfo_setSession(info, session);
	sdInfo_setDateWithString(info, date);
	sdInfo_setHost(info, host);


	printf("---info---\n");
	printf("author:%s\n", sdInfo_getAuthor(info));
	printf("location:%s\n", sdInfo_getLocation(info));
	printf("annotation:%s\n", sdInfo_getAnnotation(info));
	printf("session:%s\n",sdInfo_getSession(info));
	printf("date:%s\n",sdInfo_getDateAsString(info));
	printf("host:%s\n",sdInfo_getHost(info));

	return 0;
}

