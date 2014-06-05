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

#include "sdInfo.h"
#include "sdcInfo.h"
#include <string.h>

extern "C" {

    sdInfo* sdInfo_new(){
    	return new sdInfo();
    }

    void sdInfo_setAuthor(sdInfo* info, char* author){
    	info->setAuthor(string(author));
    }

    void sdInfo_setHost(sdInfo* info, char* host){
    	info->setHost(string(host));
    }

    void sdInfo_setAnnotation(sdInfo* info, char* annotation){
    	info->setAnnotation(string(annotation));
    }

    void sdInfo_setLocation(sdInfo* info, char* location){
    	info->setLocation(string(location));
    }

    void sdInfo_setDateWithString(sdInfo* info, char* dateString){
    	info->setDate(string(dateString));
    }

    void sdInfo_setSession(sdInfo* info, char* session){
    	info->setSession(string(session));
    }

    const char* sdInfo_getAuthor(sdInfo* info){
    	return info->getAuthor().c_str();
    }

    const char* sdInfo_getHost(sdInfo* info){
    	return info->getHost().c_str();
    }

    const char* sdInfo_getAnnotation(sdInfo* info){
    	return info->getAnnotation().c_str();
    }

    const char* sdInfo_getLocation(sdInfo* info){
    	return info->getLocation().c_str();
    }

    const char* sdInfo_getDateAsString(sdInfo* info){
    	return info->getDateAsString().c_str();
    }

    const char* sdInfo_getSession(sdInfo* info){
    	return info->getSession().c_str();
    }
}