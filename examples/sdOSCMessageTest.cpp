#include <iostream>
#include <vector>
#include <iomanip>

#include "sdOSCMessage.h"

using namespace std;

void dumpBytes(vector<unsigned char> bytes){
    vector<unsigned char>::iterator it = bytes.begin();
    unsigned char c;
    char o;
    int count = 0;
    while (it != bytes.end()) {
        c = *it;
        o = static_cast<char>(c);
        if(o == '\0'){
            o = '_';
        }
        cout << noskipws << std::hex << setfill('0') << setw(2) << static_cast<int>(c) << "(" << o << ") ";
        it++; count++;
        
        if (count % 4 == 0){
            cout << endl;
        }
    }
    cout << "number of bytes:" << std::dec << bytes.size() << endl;
}

int main(void){
    
    sdOSCMessage message("/test/sample");
    message.appendInt(1234567);
    message.appendFloat(-1.234567);
    message.appendString("this is a test string");
    message.appendFloat(2.345);

    cout << "---------------" << endl;
    {
        cout << "getAddress:" << endl;
        vector<unsigned char>address = message.getAddress();
        dumpBytes(address);
    }
    cout << "---------------" << endl;
    {
        cout << "getTypetags:" << endl;
        vector<unsigned char>typetags = message.getTypetags();
        dumpBytes(typetags);
    }
    cout << "---------------" << endl;
    {
        cout << "getArguments:" << endl;
        vector<unsigned char>arguments = message.getArguments();
        dumpBytes(arguments);
    }
    cout << "---------------" << endl;
    {
        cout << "getDelimiters:" << endl;
        vector<int> delimiters = message.getDelimiters();
        cout << "number of delimeters:" << delimiters.size() << endl;

        vector<int>::iterator it = delimiters.begin();
        while(it != delimiters.end()){
            int x = *it;
            cout << std::dec << x << ' ';
            it++;
        }
        cout << endl;
    }
    cout << "---------------" << endl;
    {
        cout << "getOSCMessage:" << endl;
        vector<unsigned char>oscMessage = message.getOSCMessage();
        dumpBytes(oscMessage);
    }
    cout << "---------------" << endl;
    {
        cout << "getAddressAsString:" << message.getAddressAsString() << endl;
        cout << "getTypetagsAsString:" << message.getTypetagsAsString() << endl;
        cout << "getDelimitersAsString:" << message.getDelimitersAsString() << endl;

        cout << "getArgumentAsInt:" << message.getArgumentAsInt(0) << endl;
        cout << "getArgumentAsFloat:" << message.getArgumentAsFloat(1) << endl;
        cout << "getArgumentAsString:" << message.getArgumentAsString(2) << endl;
        cout << "getEntireArgumentsAsString:\n" << message.getAllArgumentsAsString() << endl;

    }
    cout << "---------------" << endl;
    {
        // create a new one from raw OSCMessage
        vector<unsigned char> rawMessage = message.getOSCMessage();
        sdOSCMessage decodedMessage(rawMessage);
        cout << "from a new sdOSCMessage created by a raw mesage." << endl;
        cout << "getAddressAsString:" << decodedMessage.getAddressAsString() << endl;
        cout << "getTypetagsAsString:" << decodedMessage.getTypetagsAsString() << endl;
        cout << "getDelimitersAsString:" << decodedMessage.getDelimitersAsString() << endl;
        cout << "getArgumentAsInt:" << decodedMessage.getArgumentAsInt(0) << endl;
        cout << "getArgumentAsFloat:" << decodedMessage.getArgumentAsFloat(1) << endl;
        cout << "getArgumentAsString:" << decodedMessage.getArgumentAsString(2) << endl;
        cout << "getEntireArgumentsAsString:\n" << decodedMessage.getAllArgumentsAsString() << endl;

    }
    return 0;
    
}