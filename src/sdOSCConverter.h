/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief sdOSCConverter provides utility functions to convert float, int, string to
 * OSC-compatible blocks 
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
 this class provides block to int, float, string converter and vice versa.
 if the length of given strings multiples of four, a block of \0 (null) will be added at the end automatically in order to conform to the OSC-string regulation.
*/
class sdOSCConverter{
    
public:
    inline vector<unsigned char> intToBlock(int value);
    inline int blockToInt(vector<unsigned char> block);
    inline vector<unsigned char> floatToBlock(float value);
    inline float blockToFloat(vector<unsigned char> block);
    inline vector<unsigned char> stringToBlocks(string str);
    inline string blocksToString(vector<unsigned char> blocks);
};

vector<unsigned char> sdOSCConverter::intToBlock(int value){
    unsigned char *c = reinterpret_cast<unsigned char*>(&value);
    vector<unsigned char> block;
    block.push_back(c[3]);
    block.push_back(c[2]);
    block.push_back(c[1]);
    block.push_back(c[0]);
    return block;
}

int sdOSCConverter::blockToInt(vector<unsigned char> block){
    unsigned char c[4];
    c[0] = block[3];
    c[1] = block[2];
    c[2] = block[1];
    c[3] = block[0];
    int *value = reinterpret_cast<int*>(&c);
    return *value;
}

vector<unsigned char> sdOSCConverter::floatToBlock(float value){
    unsigned char *c = reinterpret_cast<unsigned char*>(&value);
    vector<unsigned char> block;
    block.push_back(c[3]);
    block.push_back(c[2]);
    block.push_back(c[1]);
    block.push_back(c[0]);
    return block;
}

float sdOSCConverter::blockToFloat(vector<unsigned char> block){
    unsigned char c[4];
    c[0] = block[3];
    c[1] = block[2];
    c[2] = block[1];
    c[3] = block[0];
    float *value = reinterpret_cast<float*>(&c);
    return *value;
}

vector<unsigned char> sdOSCConverter::stringToBlocks(string str){
    
    int rest = str.size() % 4 ;
    int numberOfNulls = 4-rest;
    vector<unsigned char> blocks;
    for(int i = 0; i < str.size(); i++){
        blocks.push_back(str[i]);
    }
    for (int i = 0; i < numberOfNulls; i++) {
        blocks.push_back('\0');
    }
    return blocks;
}

string sdOSCConverter::blocksToString(vector<unsigned char> blocks){
    string str;
    
    for(int i = 0; i < blocks.size(); i++){
        if(blocks[i] == '\0'){
            break;
        }
        str += blocks[i];
    }
    return str;
}


#endif