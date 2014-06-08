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
sdScene* sdLoader_sceneFromXML(char* xmlString);
sdScene* sdLoader_sceneFromJSON(char* jsonString);
sdScene* sdLoader_sceneFromYAML(char* yamlString);

#ifdef __cplusplus
}
#endif
#endif

