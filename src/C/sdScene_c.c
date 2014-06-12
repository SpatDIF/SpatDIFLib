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
    // sdEntityCore* sdScene_addSourceEntity(sdScene* scene, const char* name){
    // 	return  scene->addEntity(name, SD_SOURCE);
    // }

 	int sdScene_getNumberOfEntities(sdScene* scene){
 		return scene->getNumberOfEntities();
 	}

	void sdScene_removeEntity(sdScene* scene, const char* entityName){
		scene->removeEntity(entityName);
	}

	void sdScene_removeAllEntities(sdScene* scene){
		scene->removeAllEntities();
	}

	int sdScene_getNumberOfActivatedExtensions(sdScene* scene){
		return scene->getNumberOfActivatedExtensions();
	}
	
	void sdScene_addExtension(sdScene* scene, const char* extension){
		scene->addExtension(extension);
	}

	int sdScene_isExtensionActivated(sdScene* scene, const char* extension){
		scene->isExtensionActivated(string(extension));
	}
	void sdScene_removeExtension(sdScene* scene, const char* extension){
		scene->removeExtension(extension);
	}
	void sdScene_removeAllExtensions(sdScene* scene){
		scene->removeAllExtensions();
	}

	void sdScene_dump(sdScene* scene, char* dumpString){
		string str = scene->dump(false);
	}

}