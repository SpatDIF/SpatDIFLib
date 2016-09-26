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

#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <cstring>

/*!
 this class provides block (4 bytes block) to int, float, string converter and vice versa.
 if the length of given strings multiples of four, a block of \0 (null) will be added at the end automatically 
 in order to conform to the OSC-string regulation.
 All functions are declared as static; They can be employed without instantiation of this class.
*/

class sdOSCConverter{
    
protected:
    
    template <typename T>
    static std::vector<unsigned char>convertToBlock(const T &value);

public:
    /*! convert an int value to a 4-bytes block
    @param value the int value to be converted
    */
    template <typename T>
    static std::vector<unsigned char> toBlock(const T &value);

    /*! convert a 4-bytes block to an int
    @param block the block to be converted
    */
    template <typename T>
    static T blockTo(const std::vector<unsigned char> &block);
};

template <typename T>
inline std::vector<unsigned char>sdOSCConverter::convertToBlock(const T &value){
    const unsigned char *c = reinterpret_cast<const unsigned char*>(&value);
    std::vector<unsigned char> block;
    block.push_back(c[3]);
    block.push_back(c[2]);
    block.push_back(c[1]);
    block.push_back(c[0]);
    return std::move(block);
}

template <typename T>
inline std::vector<unsigned char> sdOSCConverter::toBlock(const T &value){
    return convertToBlock(value);
}

template <>
inline std::vector<unsigned char> sdOSCConverter::toBlock(const double &value){
    return convertToBlock(static_cast<float>(value));
}

template <>
inline std::vector<unsigned char> sdOSCConverter::toBlock(const unsigned int &value){
    return convertToBlock(static_cast<int>(value));
}

template <>
inline std::vector<unsigned char> sdOSCConverter::toBlock(const long &value){
    return convertToBlock(static_cast<int>(value));
}

template <>
inline std::vector<unsigned char> sdOSCConverter::toBlock(const unsigned long &value){
    return convertToBlock(static_cast<int>(value));
}

template <>
inline std::vector<unsigned char> sdOSCConverter::toBlock(const std::string &value){

    int rest = value.size() % 4;
    int numberOfNulls = 4 - rest;
    std::vector<unsigned char> blocks;
    for(int i = 0; i < value.size(); i++){
        blocks.push_back(value[i]);
    }
    for (int i = 0; i < numberOfNulls; i++) {
        blocks.push_back('\0');
    }
    return std::move(blocks);
}

template <typename T>
inline  T sdOSCConverter::blockTo(const std::vector<unsigned char> &block){
    unsigned char c[4];
    c[0] = block[3];
    c[1] = block[2];
    c[2] = block[1];
    c[3] = block[0];
    T *value = reinterpret_cast<T*>(&c);
    return *value;
}

template <>
inline std::string sdOSCConverter::blockTo(const std::vector<unsigned char> &block){
    std::string str;
    for(int i = 0; i < static_cast<int>(block.size()); i++){
        if(block[i] == '\0'){
            break;
        }
        str += block[i];
    }
    return std::move(str);
}

