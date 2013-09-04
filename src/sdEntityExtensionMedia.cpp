#include "sdEntityExtensionMedia.h"

const EExtension sdEntityExtensionMedia::extensionName = SD_MEDIA;

const EDescriptor sdEntityExtensionMedia::relevantDescriptors[] = {
    SD_MEDIA_ID,
    SD_MEDIA_TYPE,
    SD_MEDIA_LOCATION,
    SD_MEDIA_CHANNEL,
    SD_MEDIA_TIME_OFFSET,
    SD_MEDIA_GAIN
};


void* sdEntityExtensionMedia::getValue(double time, EDescriptor descriptor){
    
    // to be coded
    return NULL;
}


sdEvent* sdEntityExtensionMedia::getEvent(double, EDescriptor){
    
    
    return NULL;
}

sdEvent* sdEntityExtensionMedia::addEvent(double time, EDescriptor descriptor, void* value){
    
    
    return NULL;

    
}

sdEvent* sdEntityExtensionMedia::addEvent(string time, string descriptor, string value){
    return NULL;

}


