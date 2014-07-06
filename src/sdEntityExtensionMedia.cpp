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

#include "sdEntityExtensionMedia.h"

/*** sdEventExtensionMedia ***/

// constructors
sdEventExtensionMedia::sdEventExtensionMedia(double time, EDescriptor descriptor, void* value){
    setTime(time); // declared in sdEntity.h
    setValue(descriptor, value); 
}

sdEventExtensionMedia::sdEventExtensionMedia(string time, string descriptor, string value){
    setTime(time); 
    setValue(descriptor, value); 
}

// destructor
sdEventExtensionMedia::~sdEventExtensionMedia(){
    
    switch (descriptor) {
        case SD_MEDIA_ID:{
            string* valuePointer = static_cast<string*>(value);
            delete valuePointer;
            break;
        }
        case SD_MEDIA_TYPE:{
            string* valuePointer = static_cast<string*>(value);
            delete valuePointer;
            break;
        }
        case SD_MEDIA_LOCATION:{
            string* valuePointer = static_cast<string*>(value);
            delete valuePointer;
            break;
        }
        case SD_MEDIA_CHANNEL:{
            int *valuePointer = static_cast<int*>(value);
            delete valuePointer;
            break;
        }
        case SD_MEDIA_TIME_OFFSET:{
            double *valuePointer = static_cast<double*>(value);
            delete valuePointer;
            break;
        }
        case SD_MEDIA_GAIN:{
            double *valuePointer = static_cast<double*>(value);
            delete valuePointer;
            break;
        }
        default:
            break;
    }
}

// returns descriptor of the event as a string
string sdEventExtensionMedia::getDescriptorAsString(void){
    for(int i = 0; i< sdEntityExtensionMedia::numberOfRelevantDescriptors; i++){
        if(sdEntityExtensionMedia::relevantDescriptors[i].getDescriptor() == descriptor){
            return sdEntityExtensionMedia::relevantDescriptors[i].getDescriptorAsString();
        }
    }
    return string("error");
}

// returns value of the event as a string
string sdEventExtensionMedia::getValueAsString(void){
    string str;
    switch (descriptor) {
        case SD_MEDIA_ID:
        case SD_MEDIA_TYPE:
        case SD_MEDIA_LOCATION:
            str = *static_cast<string*>(value);
            break;
        case SD_MEDIA_CHANNEL:{
            str = intToString(*static_cast<int *>(value));
            break;
        }
        case SD_MEDIA_TIME_OFFSET:
        case SD_MEDIA_GAIN:{
            str = doubleToString(*static_cast<double *>(value));
            break;
        }
        default:
            break;
    }
    return str;
}

// set value with enum and pointer
bool sdEventExtensionMedia::setValue(EDescriptor descriptor, void* value){
    
    sdEventExtensionMedia::descriptor = descriptor;
    switch (sdEventExtensionMedia::descriptor) {
        case SD_MEDIA_ID:
        case SD_MEDIA_TYPE:
        case SD_MEDIA_LOCATION:{
            string *valueString = new string(*static_cast<string*>(value));
            sdEventExtensionMedia::value = static_cast<void*>(valueString);
            break;
        }
        case SD_MEDIA_CHANNEL:{
            int *channelValue = static_cast<int*>(value);
            sdEvent::value = static_cast<int*>(new int(*channelValue));
            break;
        }
        case SD_MEDIA_TIME_OFFSET:
        case SD_MEDIA_GAIN:{
            double *doubleValue = static_cast<double*>(value);
            sdEvent::value = static_cast<double*>(new double(*doubleValue));
            break;
        }
        default:{
            return false;
            break;
        }
    }
    return true;
}

// set value with strings
bool sdEventExtensionMedia::setValue(string descriptor, string value){
    
    for(int i = 0; i < sdEntityExtensionMedia::numberOfRelevantDescriptors; i++){
        if(sdEntityExtensionMedia::relevantDescriptors[i].getDescriptorAsString() == descriptor){
            sdEventExtensionMedia::descriptor = 
                sdEntityExtensionMedia::relevantDescriptors[i].getDescriptor();
        }
    }

    switch (sdEventExtensionMedia::descriptor) {
        case SD_MEDIA_ID:
        case SD_MEDIA_TYPE:
        case SD_MEDIA_LOCATION:{
            sdEventExtensionMedia::value = static_cast<void*>(new string(value));
            break;
        }
        case SD_MEDIA_CHANNEL:{
            sdEventExtensionMedia::value = static_cast<void*>(new int(stringToInt(value)));
            break;
        }
        case SD_MEDIA_TIME_OFFSET:
        case SD_MEDIA_GAIN:{
            sdEventExtensionMedia::value = static_cast<void*>(new double(stringToDouble(value)));
            break;
        }
        default:{
            return false;
            break;
        }
    }

    return true;
}

/*** sdEntityExtensionMedia ***/

const EExtension sdEntityExtensionMedia::extensionName = SD_MEDIA;
const string sdEntityExtensionMedia::extensionNameAsString = string("media");
const int sdEntityExtensionMedia::numberOfRelevantDescriptors = 6;
const sdDescriptor sdEntityExtensionMedia::relevantDescriptors[sdEntityExtensionMedia::numberOfRelevantDescriptors] = {
    sdDescriptor(SD_MEDIA_ID, string("id"), false),
    sdDescriptor(SD_MEDIA_TYPE, string("type"), false),
    sdDescriptor(SD_MEDIA_LOCATION, string("location"), true),
    sdDescriptor(SD_MEDIA_CHANNEL, string("channel"), false),
    sdDescriptor(SD_MEDIA_TIME_OFFSET, string("time-offset"), false),
    sdDescriptor(SD_MEDIA_GAIN, string("gain"), true)
};

sdEvent* sdEntityExtensionMedia::addEvent(double time, EDescriptor descriptor, void* value){
    
    sdEvent *event = NULL;
    sdEntity::removeEvent(time, descriptor);
    event = static_cast<sdEvent*>(new sdEventExtensionMedia(time, descriptor, value));
    eventSet.insert(event);
    return event;
    
}

sdEvent* sdEntityExtensionMedia::addEvent(string time, string descriptor, string value){

    sdEvent *event = NULL;
    removeEvent(time, descriptor);
    event = static_cast<sdEvent*>(new sdEventExtensionMedia(time, descriptor, value));
    eventSet.insert(event);
    return event;
}

void sdEntityExtensionMedia::removeEvent(string time, string descriptor){
    EDescriptor dc;
    for(int i = 0; i < sdEntityExtensionMedia::numberOfRelevantDescriptors; i++){
        if(sdEntityExtensionMedia::relevantDescriptors[i].getDescriptorAsString() == descriptor){
            dc = sdEntityExtensionMedia::relevantDescriptors[i].getDescriptor();
        }
    }

    sdEntity::removeEvent(stringToDouble(time), dc);
}
