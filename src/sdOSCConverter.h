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

#ifndef ____sdOSCConverter__
#define ____sdOSCConverter__

#include <string>
#include <iostream>
#include <vector>


/*!
 this class provides block (4 bytes block) to int, float, string converter and vice versa.
 if the length of given strings multiples of four, a block of \0 (null) will be added at the end automatically 
 in order to conform to the OSC-string regulation.
 All functions are declared as static; They can be employed without instantiation of this class.
*/

class sdOSCConverter{
    
public:
    /*! convert an int value to a 4-bytes block
    @param value the int value to be converted
    */
    template <typename T>
    static std::vector<unsigned char> toBlock(T value);

    /*! convert a 4-bytes block to an int
    @param block the block to be converted
    */
    template <typename T>
    static T blockTo(std::vector<unsigned char> block);
//
//    /*! convert a float value to a 4-bytes block 
//    @param value the float value to be converted
//    */
//    static std::vector<unsigned char> floatToBlock(float value);
//
//    /*! convert a 4-bytes block to a float
//    @param block the block to be converted
//    */
//    static float blockToFloat(std::vector<unsigned char> block);
//
//    /*! convert a string to 4-bytes blocks
//    @param str the string to be converted
//     */
//    static std::vector<unsigned char> stringToBlocks(std::string str);
//
//    /*! convert 4-bytes blocks to a string
//    @param blocks the blocks to be converted
//    */
//    static std::string blocksToString(std::vector<unsigned char> blocks);
};

template <typename T>
inline std::vector<unsigned char> sdOSCConverter::toBlock(T value){
    unsigned char *c = reinterpret_cast<unsigned char*>(&value);
    std::vector<unsigned char> block;
    block.push_back(c[3]);
    block.push_back(c[2]);
    block.push_back(c[1]);
    block.push_back(c[0]);
    return block;
}

template <>
inline std::vector<unsigned char> sdOSCConverter::toBlock(std::string value){
    
    int rest = value.size() % 4 ;
    int numberOfNulls = 4-rest;
    std::vector<unsigned char> blocks;
    for(int i = 0; i < static_cast<int>(value.size()); i++){
        blocks.push_back(value[i]);
    }
    for (int i = 0; i < numberOfNulls; i++) {
        blocks.push_back('\0');
    }
    return blocks;
}

template <typename T>
inline  T sdOSCConverter::blockTo(std::vector<unsigned char> block){
    unsigned char c[4];
    c[0] = block[3];
    c[1] = block[2];
    c[2] = block[1];
    c[3] = block[0];
    T *value = reinterpret_cast<T*>(&c);
    return *value;
}

template <>
inline std::string sdOSCConverter::blockTo(std::vector<unsigned char> block){
    std::string str;
    
    for(int i = 0; i < static_cast<int>(block.size()); i++){
        if(block[i] == '\0'){
            break;
        }
        str += block[i];
    }
    return str;
}


//
//inline std::vector<unsigned char> sdOSCConverter::intToBlock(int value){
//    unsigned char *c = reinterpret_cast<unsigned char*>(&value);
//    std::vector<unsigned char> block;
//    block.push_back(c[3]);
//    block.push_back(c[2]);
//    block.push_back(c[1]);
//    block.push_back(c[0]);
//    return block;
//}
//
//inline int sdOSCConverter::blockToInt(std::vector<unsigned char> block){
//    unsigned char c[4];
//    c[0] = block[3];
//    c[1] = block[2];
//    c[2] = block[1];
//    c[3] = block[0];
//    int *value = reinterpret_cast<int*>(&c);
//    return *value;
//}
//
//inline std::vector<unsigned char> sdOSCConverter::floatToBlock(float value){
//    unsigned char *c = reinterpret_cast<unsigned char*>(&value);
//    std::vector<unsigned char> block;
//    block.push_back(c[3]);
//    block.push_back(c[2]);
//    block.push_back(c[1]);
//    block.push_back(c[0]);
//    return block;
//}
//
//inline float sdOSCConverter::blockToFloat(std::vector<unsigned char> block){
//    unsigned char c[4];
//    c[0] = block[3];
//    c[1] = block[2];
//    c[2] = block[1];
//    c[3] = block[0];
//    float *value = reinterpret_cast<float*>(&c);
//    return *value;
//}


#endif