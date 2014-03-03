/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief sdOSCConverter provides utility functions to convert float, int, string to
 * OSC-compatible nibbles (4-byte blocks).
 *
 *
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#ifndef __sdOSCConverter__
#define __sdOSCConverter__

#include <string>
#include <iostream>
#include <vector>

using namespace std;

/*!
 this class provides nibble to int, float, string converter and vice versa.
 if the length of given strings multiples of four, a nibble of \0 (null) will be added at the end automatically in order to conform to the OSC-string regulation.
*/
class sdOSCConverter{
    
public:
    inline vector<unsigned char> intToNibble(int value);
    inline int nibbleToInt(vector<unsigned char> nibble);
    inline vector<unsigned char> floatToNibble(float value);
    inline float nibbleToFloat(vector<unsigned char> nibble);
    inline vector<unsigned char> stringToNibbles(string str);
    inline string nibblesToString(vector<unsigned char> nibbles);
};

vector<unsigned char> sdOSCConverter::intToNibble(int value){
    unsigned char *c = reinterpret_cast<unsigned char*>(&value);
    vector<unsigned char> nibble;
    nibble.push_back(c[3]);
    nibble.push_back(c[2]);
    nibble.push_back(c[1]);
    nibble.push_back(c[0]);
    return nibble;
}

int sdOSCConverter::nibbleToInt(vector<unsigned char> nibble){
    unsigned char c[4];
    c[0] = nibble[3];
    c[1] = nibble[2];
    c[2] = nibble[1];
    c[3] = nibble[0];
    int *value = reinterpret_cast<int*>(&c);
    return *value;
}

vector<unsigned char> sdOSCConverter::floatToNibble(float value){
    unsigned char *c = reinterpret_cast<unsigned char*>(&value);
    vector<unsigned char> nibble;
    nibble.push_back(c[3]);
    nibble.push_back(c[2]);
    nibble.push_back(c[1]);
    nibble.push_back(c[0]);
    return nibble;
}

float sdOSCConverter::nibbleToFloat(vector<unsigned char> nibble){
    unsigned char c[4];
    c[0] = nibble[3];
    c[1] = nibble[2];
    c[2] = nibble[1];
    c[3] = nibble[0];
    float *value = reinterpret_cast<float*>(&c);
    return *value;
}

vector<unsigned char> sdOSCConverter::stringToNibbles(string str){
    
    int rest = str.size() % 4 ;
    int numberOfNulls = 4-rest;
    vector<unsigned char> nibbles;
    for(int i = 0; i < str.size(); i++){
        nibbles.push_back(str[i]);
    }
    for (int i = 0; i < numberOfNulls; i++) {
        nibbles.push_back('\0');
    }
    return nibbles;
}

string sdOSCConverter::nibblesToString(vector<unsigned char> nibbles){
    string str;
    
    for(int i = 0; i < nibbles.size(); i++){
        if(nibbles[i] == '\0'){
            break;
        }
        str += nibbles[i];
    }
    return str;
}


#endif