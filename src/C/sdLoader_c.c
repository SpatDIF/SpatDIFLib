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

#include "sdLoader_c.h"
#include "sdLoader.h"
#include "sdScene_c.h"

 extern "C" {
 	void sdLoader_sceneFromXML(sdScene *scene, char* xmlString){
 		sdScene loadedScene = sdLoader::sceneFromXML(xmlString);
 	}

	void sdLoader_sceneFromJSON(sdScene *scene, char* jsonString){
 		sdScene loadedScene = sdLoader::sceneFromJSON(jsonString);
 	}

 	void sdLoader_sceneFromYAML(sdScene *scene, char* yamlString){
 		sdScene loadedScene = sdLoader::sceneFromYAML(yamlString);
 	}
}