#include <iostream>
#include <vector>
#include "sdOSCConverter.h"

using namespace std;

void dumpBytes(vector<char> bytes) {
    cout << "number of bytes:" << bytes.size() << endl;
    cout << "content:";
    
    vector<char>::iterator it = bytes.begin();
    while (it != bytes.end()) {
        char c = *it;
        cout << c << "(" << static_cast<int>(c) << ") ";
        it++;
    }
    cout << endl;
    
}

int main (void){

    sdOSCConverter converter;
    
    int ioriginal = 123456789;
    cout << "original:" << ioriginal << endl;;
    vector<char> ic = converter.intToNibble(ioriginal);
    dumpBytes(ic);
    cout << "decoded:" << converter.nibbleToInt(ic) << endl << endl;
    
    float foriginal = -1.23456789;
    cout << "original:" << foriginal << endl;
    vector<char> fc = converter.floatToNibble(foriginal);
    dumpBytes(fc);
    cout << "decoded:" << converter.nibbleToFloat(fc) << endl << endl;
    
    string soriginal = "testing";
    cout << "original:" << soriginal << endl;
    vector<char> sc = converter.stringToNibbles(soriginal);
    dumpBytes(sc);
    cout << "decoded:" << converter.nibblesToString(sc) << endl << endl;
    
    soriginal = "testing!";
    cout << "original:" << soriginal << endl;
    sc = converter.stringToNibbles(soriginal);
    dumpBytes(sc);
    cout << "decoded:" << converter.nibblesToString(sc) << endl;
    
    return 0;
}
