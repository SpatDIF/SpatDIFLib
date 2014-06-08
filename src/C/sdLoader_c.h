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

#ifndef ____sdLoader_c_
#define ____sdLoader_c_


#ifdef __cplusplus
extern "C" {
#endif

#include "sdScene_c.h"
// sdScene dummy
typedef struct sdLoader sdLoader;

// wrapper functions 
void sdLoader_sceneFromXML(sdScene *scene, char* xmlString);
void sdLoader_sceneFromJSON(sdScene *scene, char* jsonString);
void sdLoader_sceneFromYAML(sdScene *scene, char* yamlString);

#ifdef __cplusplus
}
#endif
#endif

