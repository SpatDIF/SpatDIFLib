
#include <string>
#include <sstream>
#include "sdConverter.h"

using namespace std;

string sdConverter::convert(EDescriptor descriptor){
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

string sdConverter::convert(EKind kind){
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

string sdConverter::convert(EType type){
    string str;
    switch(type){
        case SD_POINT:
            str = string("point");break;
        default:
            break;
    }
    return str;
}

string sdConverter::convert(EExtension extension){
    string str;
    switch(extension){
        case SD_MEDIA:
            str = string("media");break;
        default:
            break;
    }
    return str;
}

string sdConverter::convert(sdEvent* event){
    string str;
    switch(event->descriptor){
        case SD_PRESENT:{
            bool *present;
            present = static_cast<bool*>(event->value);
            str = *present ? string("true") : string("false");
            break;
        }
        case SD_POSITION:{
            float *position;
            ostringstream os;
            position = static_cast<float *>(event->value);
            os << position[0] << ' ' << position[1] << ' ' << position[2];
            str = os.str();
            break;
        }
        case SD_ORIENTATION:{
            float *orientation;
            ostringstream os;
            orientation = static_cast<float *>(event->value);
            os << orientation[0] << ' ' << orientation[1] << ' ' << orientation[2];
            str = os.str();
            break;
        }
        default:
            break;
    }
    
    return str;
}


int sdConverter::convert(string str){

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
