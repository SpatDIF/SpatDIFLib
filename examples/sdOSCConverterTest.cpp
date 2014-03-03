#include <iostream>
#include <vector>
#include <iomanip>
#include "sdOSCConverter.h"

using namespace std;

void dumpBytes(vector<unsigned char> bytes) {
    cout << "number of bytes:" << std::dec << bytes.size() << endl;
    cout << "content:" << endl;
    
    vector<unsigned char>::iterator it = bytes.begin();
    int count = 0;
    while (it != bytes.end()) {
        unsigned char c = *it;
        cout << hex <<  setw(2) << setfill('0') << static_cast<int>(c) << ' ';
        it++;
        count++;
        if(count % 4 == 0){
            cout << endl;
        }
    }
    
    
}

int main (void){

    // conform to http://opensoundcontrol.org/spec-1_0-examples#OSCstrings

    sdOSCConverter converter;
    
    int ioriginal = 1000;
    cout << "original:" << ioriginal << endl;;
    vector<unsigned char> ic = converter.intToNibble(ioriginal);
    dumpBytes(ic);
    cout << "decoded:" << std::dec << converter.nibbleToInt(ic) << endl << endl;
    
    float foriginal = 1.234;
    cout << "original:" << foriginal << endl;
    vector<unsigned char> fc = converter.floatToNibble(foriginal);
    dumpBytes(fc);
    cout << "decoded:" << converter.nibbleToFloat(fc) << endl << endl;
    
    string soriginal = "hello";
    cout << "original:" << soriginal << endl;
    vector<unsigned char> sc = converter.stringToNibbles(soriginal);
    dumpBytes(sc);
    cout << "decoded:" << converter.nibblesToString(sc) << endl << endl;
    
    soriginal = "testing!";
    cout << "original:" << soriginal << endl;
    sc = converter.stringToNibbles(soriginal);
    dumpBytes(sc);
    cout << "decoded:" << converter.nibblesToString(sc) << endl;
    
    return 0;
}
