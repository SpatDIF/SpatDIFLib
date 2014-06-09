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

#include "sdScene.h"
#include "sdScene_c.h"

 extern "C" {
 	sdScene* sdScene_new(void){
 		return new sdScene();
 	}

 	void sdScene_setInfo(sdScene* scene, sdInfo* info){
 		scene->setInfo(*info);
 	}

  	void sdScene_getInfo(sdScene* scene, sdInfo* info){
 		*info = scene->getInfo();
 	}

 	const char* sdScene_getOrderingAsString(sdScene* scene){
 		string type = scene->getOrderingAsString();
 		return type.c_str();
 	}


}