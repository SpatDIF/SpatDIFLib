#include "sdEvent.h"

sdEvent::sdEvent(string time, string descriptor, string value){
    
    //set time from string
    istringstream is;
    is.str(time);
    is >> sdEvent::time;
    
    //set descriptor from string
    sdEvent::descriptor = static_cast<EDescriptor>(convert(descriptor));
    
    //set value
    switch (sdEvent::descriptor) {
            
        case SD_PRESENT:
        {
            bool present = value=="true" ? true : false;
            setValue(sdEvent::descriptor, static_cast<void*>(&present));
            break;
        }
        case SD_POSITION:
        {
            float position[3];
            istringstream is(value);
            is >> position[0] >> position[1] >> position[2];
            setValue(sdEvent::descriptor, static_cast<void*>(&position));
            break;
        }
        case SD_ORIENTATION:
        {
            float orientation[3];
            istringstream is(value);
            is >> orientation[0] >> orientation[1] >> orientation[2];
            setValue(sdEvent::descriptor, static_cast<void*>(&orientation));
            break;
        }
    }
}

string sdEvent::getDescriptorAsString(void){
    std::string str;
    switch (descriptor) {
        case SD_PRESENT:
            str = string("present");
            break;
        case SD_POSITION:
            str = string("position");
            break;
        case SD_ORIENTATION:
            str = string("orientation");
            break;
        default:
            break;
    }
    return str;
}

string sdEvent::getValueAsString(void){
    std::string str;
    switch (descriptor) {
        case SD_PRESENT:{
            bool *present;
            present = static_cast<bool*>(value);
            str = *present ? string("true") : string("false");
            break;
        }
        case SD_POSITION:{
            float *position;
            ostringstream os;
            position = static_cast<float *>(value);
            os << position[0] << ' ' << position[1] << ' ' << position[2];
            str = os.str();
            break;
        }
        case SD_ORIENTATION:{
            float *orientation;
            ostringstream os;
            orientation = static_cast<float *>(value);
            os << orientation[0] << ' ' << orientation[1] << ' ' << orientation[2];
            str = os.str();
        }
        default:{
            break;
        }
            
    }
    return str;
}

void sdEvent::setValue(EDescriptor descriptor, void* value){
    sdEvent::descriptor = descriptor;
    switch (sdEvent::descriptor) {
        case SD_PRESENT:{
            sdEvent::value = static_cast<void*>(new bool);
            break;
        }
        case SD_POSITION:{
            float *positionValueTo = new float[3];
            float *positionValueFrom = static_cast<float*>(value);
            positionValueTo[0] = positionValueFrom[0];
            positionValueTo[1] = positionValueFrom[1];
            positionValueTo[2] = positionValueFrom[2];
            sdEvent::value = static_cast<void*>(positionValueTo);
            break;
        }
        case SD_ORIENTATION:{
            float *orientationValueTo = new float[3];
            float *orientationValueFrom = static_cast<float*>(value);
            orientationValueTo[0] = orientationValueFrom[0];
            orientationValueTo[1] = orientationValueFrom[1];
            orientationValueTo[2] = orientationValueFrom[2];
            sdEvent::value = static_cast<void*>(orientationValueTo);
            break;
        }
        default:{
            break;
        }
    }
}

string sdEvent::convert(EDescriptor descriptor){
    string str;
    switch(descriptor){
        case SD_PRESENT:
            str = string("present");break;
        case SD_POSITION:
            str = string("position");break;
        case SD_ORIENTATION:
            str =string("orientation");break;
        default:
            break;
    }
    return str;
}

string sdEvent::convert(EKind kind){
    string str;
    switch(kind){
        case SD_SOURCE:
            str = string("source");break;
        case SD_SINK:
            str = string("sink");break;
        default:
            break;
    }
    return str;
}

string sdEvent::convert(EType type){
    string str;
    switch(type){
        case SD_POINT:
            str = string("point");break;
        default:
            break;
    }
    return str;
}

int sdEvent::convert(string str){
    
    int converted;
    
    //descriptor
    if(str == "present")
        converted = SD_PRESENT;
    else if(str == "position")
        converted = SD_POSITION;
    else if(str == "orientation")
        converted = SD_ORIENTATION;
    
    //type
    else if(str == "point")
        converted = SD_POINT;
    
    //kind
    else if(str == "source")
        converted = SD_SOURCE;
    else if(str == "sink")
        converted = SD_SINK;
    
    //extension
    else if(str == "media")
        converted = SD_MEDIA;
    
    return converted;
    
}

string sdGlobalEvent::getKindAsString(void){
    string str;
    switch (kind) {
        case SD_SOURCE:
            str = string("source");
            break;
        case SD_SINK:
            str = string("sink");
            break;
        default:
            break;
    }
    return str;
}