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
#include "sdLoader_c.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){

	char* loadedString;
	FILE *fptr = fopen("turenas_insect.xml", "r");
	if(!fptr){
		printf("sdLoader: file open error\n");
		exit(EXIT_FAILURE);
	}

	fclose(fptr);
	sdScene *scene = sdLoader_sceneFromXML(loadedString);

	return 0;
}
