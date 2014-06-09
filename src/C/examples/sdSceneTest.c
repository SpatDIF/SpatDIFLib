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


#include "sdScene_c.h"
#include "sdInfo_c.h"
#include <stdio.h>

void printInfo(sdInfo* info);

int main(void){

	sdScene *scene = sdScene_new();
	sdInfo *info = sdInfo_new();
	sdInfo *newInfo = sdInfo_new();
	sdInfo_set(info, "chikashi", "myHost", "2014-6-9", "1.2", "Cologne", "This is a test");

	sdScene_setInfo(scene, info);
	sdScene_getInfo(scene, newInfo);


	printInfo(info);

	printf("ordering: %s\n",sdScene_getOrderingAsString(scene));

	return 0;
}

void printInfo(sdInfo* info){
	printf("INFO:\n");

	printf("author: %s\n", sdInfo_getAuthor(info));
	printf("host: %s\n", sdInfo_getHost(info));
	printf("date: %s\n", sdInfo_getDateAsString(info));
	printf("session: %s\n", sdInfo_getSession(info));
	printf("location: %s\n", sdInfo_getLocation(info));
	printf("annotation: %s\n", sdInfo_getAnnotation(info));

}