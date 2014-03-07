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
    vector<unsigned char> ic = converter.intToBlock(ioriginal);
    dumpBytes(ic);
    cout << "decoded:" << std::dec << converter.blockToInt(ic) << endl << endl;
    
    float foriginal = 1.234;
    cout << "original:" << foriginal << endl;
    vector<unsigned char> fc = converter.floatToBlock(foriginal);
    dumpBytes(fc);
    cout << "decoded:" << converter.blockToFloat(fc) << endl << endl;
    
    string soriginal = "hello";
    cout << "original:" << soriginal << endl;
    vector<unsigned char> sc = sdOSCConverter::stringToBlocks(soriginal); // here call it directly
    dumpBytes(sc);
    cout << "decoded:" << converter.blocksToString(sc) << endl << endl;
    
    soriginal = "testing!";
    cout << "original:" << soriginal << endl;
    sc = sdOSCConverter::stringToBlocks(soriginal);
    dumpBytes(sc);
    cout << "decoded:" << converter.blocksToString(sc) << endl;
    
    return 0;
}
