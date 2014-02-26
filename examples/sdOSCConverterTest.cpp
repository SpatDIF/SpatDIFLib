#include <iostream>
#include <vector>
#include "sdOSCConverter.h"

using namespace std;
int main (void){

    sdOSCConverter converter;
    
    vector<char> ic = converter.intToNibble(1523421);
    cout << converter.nibbleToInt(ic) << endl;
    
    vector<char> fc = converter.floatToNibble(-2.31523);
    cout << converter.nibbleToFloat(fc) << endl;
    
    vector<char> sc = converter.stringToNibbles("testing");
    cout << converter.nibblesToString(sc);
    
    return 0;
}
