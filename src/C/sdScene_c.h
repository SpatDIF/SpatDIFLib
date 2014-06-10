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

#ifndef ____sdScene_c_
#define ____sdScene_c_


#ifdef __cplusplus
extern "C" {
#endif

#include "sdInfo_c.h"
#include "sdScene_c.h"
// sdScene dummy
typedef struct sdScene sdScene;

// wrapper functions 
sdScene* sdScene_new();
void sdScene_setInfo(sdScene* scene, sdInfo* info);
void sdScene_getInfo(sdScene* scene, sdInfo* info);
const char* sdScene_getOrderingAsString(sdScene* scene);


int sdScene_getNumberOfEntities(sdScene* scene);
void sdScene_removeEntity(sdScene* scene, const char* entityName);
void sdScene_removeAllEntities(sdScene* scene);
int sdScene_getNumberOfActivatedExtensions(sdScene* scene);

#ifdef __cplusplus
}
#endif
#endif

