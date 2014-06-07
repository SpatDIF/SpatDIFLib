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

#include "sdScene_c.h"
// sdScene dummy
typedef struct sdScene sdScene;

// wrapper functions 
sdScene* sdScene_new();
void sdScene_setInfo(sdScene* scene, sdInfo* info);
void sdScene_getInfo(sdScene* scene);
EOrdering sdScene_getOrdering();
char* getOrderingAsString();



#ifdef __cplusplus
}
#endif
#endif

