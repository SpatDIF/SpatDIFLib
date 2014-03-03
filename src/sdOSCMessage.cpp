

#include "sdOSCMessage.h"
#include "sdConst.h"

sdOSCMessage::sdOSCMessage(string address){
    sdOSCMessage::address = stringToNibbles(address); // conform to nibbles (4 byte block)
    typetags.push_back(','); // the length is unknown, conform later
}

sdOSCMessage::sdOSCMessage(vector<unsigned char> oscMessage){
    setOSCMessage(oscMessage);
}

vector<unsigned char> sdOSCMessage::getOSCMessage(){
    vector<unsigned char> OSCMessage;
    vector<unsigned char> conformedTypetags = getTypetags(); // with null padding
                                   
    OSCMessage.insert(OSCMessage.end(), address.begin(), address.end()); // concatenate address
    OSCMessage.insert(OSCMessage.end(), conformedTypetags.begin(), conformedTypetags.end()); // concatenate conformed type tags
    OSCMessage.insert(OSCMessage.end(), arguments.begin(), arguments.end()); // concatenate arguments

    return OSCMessage;
}

void sdOSCMessage::setOSCMessage(vector<unsigned char> newMessage){
    
    int cursor = 0, length = 0;
    address.clear();
    typetags.clear();
    arguments.clear();
    
    // get address
    length = getLengthOfOSCString(newMessage, cursor); // get the length of address
    address.insert(address.begin(), newMessage.begin(), newMessage.begin()+length);
    
    // get type tag
    cursor = length;
    length = getLengthOfOSCString(newMessage, cursor);
    typetags.insert(typetags.begin(), newMessage.begin()+cursor, newMessage.begin()+cursor+length);
    
    // get arguments;
    cursor = length;
    length = getLengthOfOSCString(newMessage, cursor);
    arguments.insert(arguments.begin(), newMessage.begin()+cursor, newMessage.begin()+cursor+length);
}

vector<unsigned char> sdOSCMessage::nullPadding(vector<unsigned char> string){
    int size = string.size();
    int numNullsToBeAdded = 4 - (size % 4);
    while(numNullsToBeAdded--){
        string.push_back('\0');
    }
    return string;
}

int sdOSCMessage::getLengthOfOSCString(vector<unsigned char> OSCString, int onset){
    
    vector<unsigned char>::iterator it = OSCString.begin() + onset;
    int count = 0;
    while (it != arguments.end()) {
        unsigned char byte = *it;
        count++; // number of bytes including null unsigned character
        if(byte == '\0'){ // if null
            break;
        }
        it++;
    }
    return count + ( 4 - (count % 4));
}

void sdOSCMessage::appendInt(int value){
    delimiters.push_back(arguments.size());
    typetags.push_back('i');
    vector<unsigned char> nibble = intToNibble(value);
    arguments.insert(arguments.end(),nibble.begin(),nibble.end());
}

void sdOSCMessage::appendFloat(float value){
    delimiters.push_back(arguments.size());
    typetags.push_back('f');
    vector<unsigned char> nibble = floatToNibble(value);
    arguments.insert(arguments.end(),nibble.begin(),nibble.end());
}

void sdOSCMessage::appendString(string str){
    delimiters.push_back(arguments.size());
    typetags.push_back('s');
    vector<unsigned char> nibbles = stringToNibbles(str);
    arguments.insert(arguments.end(),nibbles.begin(),nibbles.end());
}

int sdOSCMessage::getArgumentAsInt(int index){
    int posDelimiter = delimiters[index];
    vector<unsigned char> nibble;
    nibble.insert(nibble.end(), arguments.begin()+posDelimiter , arguments.begin()+posDelimiter+4);
    return nibbleToInt(nibble);
}

float sdOSCMessage::getArgumentAsFloat(int index){
    int posDelimiter = delimiters[index];
    vector<unsigned char> nibble;
    nibble.insert(nibble.end(), arguments.begin()+posDelimiter , arguments.begin()+posDelimiter+4);
    return nibbleToFloat(nibble);
}

string sdOSCMessage::getArgumentAsString(int index){
    int posDelimiter = delimiters[index];
    vector<unsigned char> nibbles;
    int length = getLengthOfOSCString(arguments, posDelimiter);
    nibbles.insert(nibbles.end(), arguments.begin()+posDelimiter, arguments.begin()+posDelimiter+length);
    return nibblesToString(nibbles);
}

string sdOSCMessage::getEntireArgumentsAsString(void){
    int numArguments = typetags.size() -1;
    string str;
    for(int i = 0; i < numArguments; i++){
        switch(typetags[i+1]){ // we need to skip ','
            case 'i':
                str += intToString(getArgumentAsInt(i));
                break;
            case 'f':
                str += floatToString(getArgumentAsFloat(i));
                break;
            case 's':
                str += getArgumentAsString(i);
                break;
        }
        str += " ";
    }
    return str;
}



