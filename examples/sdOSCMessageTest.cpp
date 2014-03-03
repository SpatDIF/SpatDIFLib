#include <iostream>
#include <vector>
#include <iomanip>

#include "sdOSCMessage.h"

using namespace std;

int main(void){
    
    sdOSCMessage message("/test/sample");
    message.appendInt(1234567);
    message.appendFloat(-1.234567);
    message.appendString("this is a test string");
    message.appendFloat(2.345);
    
    vector<char> rawMessage =message.getOSCMessage();
    
    vector<char>::iterator it = rawMessage.begin();
    char c;
    int count = 0;
    while (it != rawMessage.end()) {
        c = *it;
        cout << noskipws << c << '('  <<  setfill('0') << setw(3) << static_cast<int>(c) << ") ";
        it++; count++;
        if (count % 4 == 0){
            cout << endl;
        }
    }
    vector<int> delimiters = message.getDelimiters();
    vector<int>::iterator dit = delimiters.begin();
    cout << "delimiters:" ;
    while(dit != delimiters.end()){
        cout << *dit << ' ' ;
        dit++;
    }
    cout << endl;
    cout << "size:" << count <<endl;
    
    cout << "address as string:" << message.getAddressAsString() << endl;
    cout << "typetags as string:" << message.getTypetagsAsString() << endl;

    sdOSCMessage decodedMessage(rawMessage);
    cout << "sdOSCMessage from raw mesage:" << endl;
    cout << "address:" << decodedMessage.getAddressAsString();
    cout << "typetags:" << decodedMessage.getTypetagsAsString();

    return 0;
    
}