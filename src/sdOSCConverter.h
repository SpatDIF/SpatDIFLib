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
    inline vector<char> intToNibble(int value);
    inline int nibbleToInt(vector<char> nibble);
    inline vector<char> floatToNibble(float value);
    inline float nibbleToFloat(vector<char> nibble);
    inline vector<char> stringToNibbles(string str);
    inline string nibblesToString(vector<char> nibbles);
};

vector<char> sdOSCConverter::intToNibble(int value){
    char *c = reinterpret_cast<char*>(&value);
    vector<char> nibble;
    nibble.push_back(c[0]);
    nibble.push_back(c[1]);
    nibble.push_back(c[2]);
    nibble.push_back(c[3]);
    return nibble;
}

int sdOSCConverter::nibbleToInt(vector<char> nibble){
    char c[4];
    c[0] = nibble[0];
    c[1] = nibble[1];
    c[2] = nibble[2];
    c[3] = nibble[3];
    int *value = reinterpret_cast<int*>(&c);
    return *value;
}

vector<char> sdOSCConverter::floatToNibble(float value){
    char *c = reinterpret_cast<char*>(&value);
    vector<char> nibble;
    nibble.push_back(c[0]);
    nibble.push_back(c[1]);
    nibble.push_back(c[2]);
    nibble.push_back(c[3]);
    return nibble;
}

float sdOSCConverter::nibbleToFloat(vector<char> nibble){
    char c[4];
    c[0] = nibble[0];
    c[1] = nibble[1];
    c[2] = nibble[2];
    c[3] = nibble[3];
    float *value = reinterpret_cast<float*>(&c);
    return *value;
}

vector<char> sdOSCConverter::stringToNibbles(string str){
    
    int rest = str.size() % 4 ;
    int numberOfNulls = 4-rest;
    vector<char> nibbles;
    for(int i = 0; i < str.size(); i++){
        nibbles.push_back(str[i]);
    }
    for (int i = 0; i < numberOfNulls; i++) {
        nibbles.push_back('\0');
    }
    return nibbles;
}

string sdOSCConverter::nibblesToString(vector<char> nibbles){
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