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
 	sdScene* sdLoader_sceneFromXML(char* xmlString){
 		return sdLoader::sceneFromXML(xmlString);
 	}

	sdScene* sdLoader_sceneFromJSON(char* jsonString){
 		return sdLoader::sceneFromJSON(jsonString);
 	}

 	sdScene* sdLoader_sceneFromYAML(char* yamlString){
 		return sdLoader::sceneFromYAML(yamlString);
 	}
}