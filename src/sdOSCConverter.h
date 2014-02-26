/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief
 *
 *
 *
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#ifndef __libspatdif__sdOSCConverter__
#define __libspatdif__sdOSCConverter__
#include <string>
#include <iostream>

using namespace std;

/*!
 add inheritd classes functionalities to convert normal float, int, string to osc compatible sequence of binary and vise versa.
 */
class sdOSCConverter{

public:
    inline string floatToOSC(float value);
    
    
};


string sdOSCConverter:: floatToOSC(float value){
    char *c = reinterpret_cast<char*>(&value);
    string converted;
    cout.setf(std::ios::hex, std::ios::basefield);
    cout << c[0] << endl;
    cout << c[1] << endl;
    cout << c[2] << endl;
    cout << c[3] << endl;
    
    float *returned = reinterpret_cast<float*>(c);
    cout << *returned << endl;
    return converted;
}

#endif /* defined(__libspatdif__sdOSCConverter__) */
