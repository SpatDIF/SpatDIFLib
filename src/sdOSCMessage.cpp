

#include "sdOSCMessage.h"
#include "sdConst.h"

sdOSCMessage::sdOSCMessage(){

}

sdOSCMessage::sdOSCMessage(string address){
    sdOSCMessage::address = stringToBlocks(address); // conform to blocks (4 byte block)
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
    
    int cursor = 0, length = 0, lengthWithNullPaddings = 0;
    clear();
    // get address
    lengthWithNullPaddings = getLengthOfOSCString(newMessage, cursor, true); // get the length of address with null paddings
    address.insert(address.begin(), newMessage.begin(), newMessage.begin()+lengthWithNullPaddings);


    // get type tag ... without null padding because typetags can be added later
    cursor += lengthWithNullPaddings;
    length = getLengthOfOSCString(newMessage, cursor, false);
    lengthWithNullPaddings = getLengthOfOSCString(newMessage, cursor, true);
    typetags.insert(typetags.begin(), newMessage.begin()+cursor, newMessage.begin()+cursor+length);
    
    // get arguments ... with null padding;
    cursor += lengthWithNullPaddings;
    length = newMessage.size() - cursor;
    arguments.insert(arguments.begin(), newMessage.begin()+cursor, newMessage.begin()+cursor+length);

    //cout << "size of address:" << address.size() << endl;
    //cout << "size of typetags:" << typetags.size() << endl;
    //cout << "size of arguments:" << arguments.size() << endl;

    cursor = 0;
    // analyze delimiters
    int numArguements = typetags.size()-1;//because of ','
    for(int i = 0; i < numArguements; i++){
        delimiters.push_back(cursor);
        switch(typetags[i+1]){
            case 'i':
            case 'f':
                cursor += 4;
                break;
            case 's':
                cursor += getLengthOfOSCString(arguments, cursor, true);
                break;
            default:
                cout << "sdOSCMessage: unknown type tag" << endl;
        }
    }
    
}

vector<unsigned char> sdOSCMessage::nullPadding(vector<unsigned char> string){
    int size = string.size();
    int numNullsToBeAdded = 4 - (size % 4);
    while(numNullsToBeAdded--){
        string.push_back('\0');
    }
    return string;
}

int sdOSCMessage::getLengthOfOSCString(vector<unsigned char> OSCString, int onset, bool includingNullPaddings){
    
    vector<unsigned char>::iterator it = OSCString.begin() + onset;
    int count = 0;
    while (it != arguments.end()) {
        unsigned char byte = *it;
        if(byte == '\0'){ // if null
            break;
        }
        count++; // number of bytes including null unsigned character
        it++;
    }
    if (includingNullPaddings)
    {
        count = count + ( 4 - (count % 4 ));
    }

    return count;
}

void sdOSCMessage::appendInt(int value){
    delimiters.push_back(arguments.size());
    typetags.push_back('i');
    vector<unsigned char> block = intToBlock(value);
    arguments.insert(arguments.end(),block.begin(),block.end());
}

void sdOSCMessage::appendInts(int *ints, int number){
    for (int i = 0; i < number; i++){
        appendInt(ints[i]);
    }
}

void sdOSCMessage::appendFloat(float value){
    delimiters.push_back(arguments.size());
    typetags.push_back('f');
    vector<unsigned char> block = floatToBlock(value);
    arguments.insert(arguments.end(),block.begin(),block.end());
}

void sdOSCMessage::appendFloats(float *floats, int number){
    for (int i = 0; i < number; i++){
        appendFloat(floats[i]);
    }
}

void sdOSCMessage::appendString(string str){
    delimiters.push_back(arguments.size());
    typetags.push_back('s');
    vector<unsigned char> blocks = stringToBlocks(str);
    arguments.insert(arguments.end(),blocks.begin(),blocks.end());
}

int sdOSCMessage::getArgumentAsInt(int index){
    int posDelimiter = delimiters[index];
    vector<unsigned char> block;
    block.insert(block.end(), arguments.begin()+posDelimiter , arguments.begin()+posDelimiter+4);
    return blockToInt(block);
}

float sdOSCMessage::getArgumentAsFloat(int index){
    int posDelimiter = delimiters[index];
    vector<unsigned char> block;
    block.insert(block.end(), arguments.begin()+posDelimiter , arguments.begin()+posDelimiter+4);
    return blockToFloat(block);
}

string sdOSCMessage::getArgumentAsString(int index){
    int posDelimiter = delimiters[index];
    vector<unsigned char> blocks;
    int length = getLengthOfOSCString(arguments, posDelimiter, false);
    blocks.insert(blocks.end(), arguments.begin()+posDelimiter, arguments.begin()+posDelimiter+length);
    return blocksToString(blocks);
}

string sdOSCMessage::getAllArgumentsAsString(void){
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

string sdOSCMessage::getMessageAsString(void){
    string str;
    str = getAddressAsString();
    str += ' ';
    str += getTypetagsAsString();
    str += ' ';
    str += getAllArgumentsAsString();
    return str;
}

