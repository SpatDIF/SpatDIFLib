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
#include <string.h>

int main(void){

	char* loadedString;
	char temp[512];
	sdScene *scene = sdScene_new();
	unsigned int length = 0;


	FILE *fptr = fopen("turenas_insect.xml", "r");
	if(!fptr){
		printf("sdLoader: file open error\n");
		exit(EXIT_FAILURE);
	}
	while(fgets(temp, 512, fptr) != NULL){
		length += strlen(temp);
		realloc(loadedString, length * sizeof(char));
		if(!loadedString){
			printf("sdLoader: loadedString allocation error\n");
			exit(EXIT_FAILURE);
		}
		strcat(loadedString, temp);
	}
	fclose(fptr);
	free(loadedString); 

	// load a scene from the loadedString to scene
	//sdLoader_sceneFromXML(scene, loadedString);

	return 0;
}
