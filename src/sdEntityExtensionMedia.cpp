#include "sdEntityExtensionMedia.h"

const EExtension sdEntityExtensionMedia::extensionName = SD_MEDIA;
const string sdEntityExtensionMedia::extensionNameAsString = string("media");
const int sdEntityExtensionMedia::numberOfRelevantDescriptors = 6;
const EDescriptor sdEntityExtensionMedia::relevantDescriptors[] = {
    SD_MEDIA_ID,
    SD_MEDIA_TYPE,
    SD_MEDIA_LOCATION,
    SD_MEDIA_CHANNEL,
    SD_MEDIA_TIME_OFFSET,
    SD_MEDIA_GAIN
};


sdEventExtensionMedia::sdEventExtensionMedia(double time, EDescriptor descriptor, void* value){
    setTime(time);
    setValue(descriptor, value);
}

sdEventExtensionMedia::sdEventExtensionMedia(string time, string descriptor, string value){
    setTime(time); // declared in the super class
    setValue(descriptor, value); // private function
}


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

string sdEventExtensionMedia::getDescriptorAsString(void){
    string str;
    switch(descriptor){
        case SD_MEDIA_ID:
            str = string("id");break;
        case SD_MEDIA_TYPE:
            str = string("type");break;
        case SD_MEDIA_LOCATION:
            str =string("location");break;
        case SD_MEDIA_CHANNEL:
            str = string("channel");break;
        case SD_MEDIA_TIME_OFFSET:
            str = string("offset");break;
        case SD_MEDIA_GAIN:
            str =string("gain");break;
        default:
            break;
    }
    return str;
}

string sdEventExtensionMedia::getValueAsString(void){
    string str;
    switch (descriptor) {
        case SD_MEDIA_ID:
        case SD_MEDIA_TYPE:
        case SD_MEDIA_LOCATION:
            str = *static_cast<string*>(value);
            break;
        case SD_MEDIA_CHANNEL:{
            int *channel;
            ostringstream os;
            channel = static_cast<int *>(value);
            os << *channel;
            str = os.str();
            break;
        }
        case SD_MEDIA_TIME_OFFSET:
        case SD_MEDIA_GAIN:{
            double *doubleValue;
            ostringstream os;
            doubleValue = static_cast<double *>(value);
            os << *doubleValue;
            str = os.str();
            break;
        }
        default:
            break;
    }
    return str;
}


void sdEventExtensionMedia::setValue(EDescriptor descriptor, void* value){
    
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
            break;
        }
    }
}

void sdEventExtensionMedia::setValue(string descriptor, string value){

    
}


sdEvent* sdEntityExtensionMedia::addEvent(double time, EDescriptor descriptor, void* value){
    
    sdEvent *event = NULL;
    removeEvent(time, descriptor);
    event = static_cast<sdEvent*>(new sdEventExtensionMedia(time, descriptor, value));
    eventSet.insert(event);
    return event;
    
}

sdEvent* sdEntityExtensionMedia::addEvent(string time, string descriptor, string value){

    return NULL;

}


void sdEntityExtensionMedia::removeEvent(double time, EDescriptor descriptor){
    
    
}
