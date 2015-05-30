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

#include "sdExtensible.h"
#include "sdEntityExtensionMedia.h"
#include "sdEntityExtensionSourceWidth.h"

sdEntityExtension* sdExtensible::addExtension(EExtension extension){
    sdEntityExtension *ext= getExtension(extension);
    switch (extension) {
        case SD_MEDIA:{
            sdEntityExtensionMedia* mediaExtension = new sdEntityExtensionMedia();
            extensionVector.push_back(mediaExtension);
            for(int i = 0; i < sdEventExtensionMedia::numberOfDescriptors; i++){
                sdRedirector rd;
                rd.descriptor = sdEventExtensionMedia::descriptors[i].getDescriptor();
                rd.descriptorString = sdEventExtensionMedia::descriptors[i].getDescriptorAsString();
                rd.responsibleExtension = static_cast<sdEntityExtension*>(mediaExtension);
                redirectorVector.push_back(rd);
            }
            return mediaExtension;
            break;
        }
        case SD_SOURCE_WIDTH:{
            sdEntityExtensionSourceWidth* sourceWidthExtension = new sdEntityExtensionSourceWidth();
            extensionVector.push_back(sourceWidthExtension);
            for(int i = 0; i < sdEventExtensionSourceWidth::numberOfDescriptors; i++){
                sdRedirector rd;
                rd.descriptor = sdEventExtensionSourceWidth::descriptors[i].getDescriptor();
                rd.descriptorString = sdEventExtensionSourceWidth::descriptors[i].getDescriptorAsString();
                rd.responsibleExtension = static_cast<sdEntityExtension*>(sourceWidthExtension);
                redirectorVector.push_back(rd);
            }
            return sourceWidthExtension;
            break;
        }
            
        default:
            break;
    }
    return NULL;
}

void sdExtensible::removeExtension(EExtension extension){
    
    std::vector <sdEntityExtension*>::iterator it =  extensionVector.begin();
    while(it != extensionVector.end()){
        if((*it)->getExtensionName() == extension){
            extensionVector.erase(it);
            return;
        }
        it++;
    }
}

sdEntityExtension* sdExtensible::getExtension(EExtension extension){
    std::vector <sdEntityExtension*>::iterator it = extensionVector.begin();
    if(it != extensionVector.end()){
        sdEntityExtension* ext = *it;
        if(ext->getExtensionName() == extension){
            return *it;
        }
        it++;
    }
    return NULL;
}

sdEntityExtension* sdExtensible::getResponsibleExtension(EDescriptor descriptor){
    std::vector <sdRedirector>::iterator it = redirectorVector.begin();
    while(it != redirectorVector.end()){
        sdRedirector rd = *it;
        if(rd.descriptor == descriptor){
            return rd.responsibleExtension;
        }
        it++;
    }
    return NULL;
}
