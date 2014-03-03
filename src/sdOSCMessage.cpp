

#include "sdOSCMessage.h"


sdOSCMessage::sdOSCMessage(string address){
    sdOSCMessage::address = stringToNibbles(address); // conform to nibbles (4 byte block)
    typetags.push_back(','); // the length is unknown, conform later
}

sdOSCMessage::sdOSCMessage(vector<char> oscMessage){
    setOSCMessage(oscMessage);
}

vector<char> sdOSCMessage::getOSCMessage(){
    vector<char> OSCMessage;
    vector<char> conformedTypetags;
    
    int numNulls = 4-(typetags.size() % 4);
    conformedTypetags = typetags;
    
    for(int i = 0; i< numNulls;i++){
        conformedTypetags.push_back('\0');
    }
                                   
    OSCMessage.insert(OSCMessage.end(), address.begin(), address.end()); // concatenate address
    OSCMessage.insert(OSCMessage.end(), conformedTypetags.begin(), conformedTypetags.end()); // concatenate conformed type tags
    OSCMessage.insert(OSCMessage.end(), arguments.begin(), arguments.end()); // concatenate arguments

    return OSCMessage;
}

void sdOSCMessage::setOSCMessage(vector<char> message){
    
    int cursor = 0, length = 0, numberOfArguments;
    address.clear();
    typetags.clear();
    arguments.clear();
    
    // get address
    length = getLengthOfString(cursor); // get the length of address
    address.insert(address.begin(), message.begin(), message.begin()+length);
    
    // get type tag
    cursor = length;
    length = getLengthOfString(cursor);
    typetags.insert(typetags.begin(), message.begin()+cursor, message.begin()+cursor+length);
    numberOfArguments = getTypetagsAsString().size() - 1;
    
    // get arguments;
    cursor = length;
    length = getLengthOfString(cursor);
    arguments.insert(arguments.begin(), message.begin()+cursor, message.begin()+cursor+length);
    
    
    
    

}

int sdOSCMessage::getLengthOfString(int startIndex){
    
    vector<char>::iterator it = arguments.begin();
    int count = 0;
    while (it != arguments.end()) {
        char byte = *it;
        count++; // number of bytes including null character
        if(byte == '\0'){ // if null
            break;
        }
        it++;
    }
    return count + ( 4 - (count % 4));
}

void sdOSCMessage::appendInt(int value){
    delimeters.push_back(arguments.size());
    typetags.push_back('i');
    vector<char> nibble = intToNibble(value);
    arguments.insert(arguments.end(),nibble.begin(),nibble.end());
}

void sdOSCMessage::appendFloat(float value){
    delimeters.push_back(arguments.size());
    typetags.push_back('f');
    vector<char> nibble = floatToNibble(value);
    arguments.insert(arguments.end(),nibble.begin(),nibble.end());
}

void sdOSCMessage::appendString(string str){
    delimeters.push_back(arguments.size());
    typetags.push_back('s');
    vector<char> nibbles = stringToNibbles(str);
    arguments.insert(arguments.end(),nibbles.begin(),nibbles.end());
}



int getArgumentAsInt(int index){
    int argument;
    return argument;
}

float getArgumentAsFloat(int index){
    float argument;
    return argument;
}

string getArgumentAsString(int index){
    string argument;
    return argument;
}


