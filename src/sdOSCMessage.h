
#pragma once

#include "sdOSCConverter.h"
#include <sstream>

/*!
 sdOSCMessage generates and interprets a valid OSC message consisting of multiple blocks (blocks of 4 bytes)
 */
class sdOSCMessage: public sdOSCConverter{

    
protected:

    std::vector<unsigned char> address; /*!< OSC address pattern in OSC-String format */
    std::vector<unsigned char> typetags;  /*!< OSC typetags in OSC-String format */
    std::vector<unsigned char> arguments; /*!< OSC Arguments in 4-bytes-block sequence */
    std::vector<size_t> delimiters; /*!< stores delimiters of OSC arguments. This increase the speed of data access */
    
    /*!
     this function looks for \0 unsigned character from the onset byte and return the length of string
     @param startIndex the onset of search point
     */
    int getLengthOfOSCString(std::vector<unsigned char> OSCString, size_t onset, bool includingNullPaddings);
    std::vector<unsigned char> nullPadding(std::vector<unsigned char> string);
    
public:
#pragma mark constructors
    
    /*! constructor. with initialization of typetags and address pattern. */
    sdOSCMessage(const std::string &address = "/spatdif");
    
    /*! constructor. initialize message with OSC-conformed 4-bytes-block sequence */
    sdOSCMessage(const std::vector<unsigned char> &message);
    

#pragma mark setter
    /*! @param address new OSC address pattern
     set address pattern*/
    void setAddress(const std::string &address);

    /*! template function for appending arguments */
    template <typename T>
    void appendArgument(const T &args);

    /*! interpret raw OSC message and store it in the local buffer */
    void setOSCMessage(const std::vector<unsigned char> &message);
    
    
#pragma mark getter
    
    /*! returns OSC address pattern in OSC-string format */
    std::vector<unsigned char> getAddress(void);
    /*! returns OSC address pattern as a string */
    std::string getAddressAsString(void);
    
    /*! returns OSC typetags in OSC-string format */
    std::vector<unsigned char> getTypetags(void);

    /*! returns OSC address pattern as a string */
    std::string getTypetagsAsString(void);

    /*! @param index argument index
     returns specified argument at index as T.*/
    template <typename T>
    T getArgument(int index);
    /*! returns specified argument as a string*/
    std::string getArgumentAsString(int index);

    /*! returns all Arguments in in 4-bytes-block sequence */
    std::vector<unsigned char> getArguments(void);
    /*! returns all arguments as a string*/
    std::string getAllArgumentsAsString(void);
    /*! returns the number of OSC arguments */
    size_t getNumberOfArguments();

    /*! returns delimiters as vector of int */
    std::vector<size_t> getDelimiters(void);
    /*! returns delimiters as a string */
    std::string getDelimitersAsString(void);

    /*! output osc conformed message */
    std::vector<unsigned char> getOSCMessage(void);
    /*! returns the entire message as a string*/
    std::string getMessageAsString(void);


#pragma mark deletion
    /*! delete entire content. typeyag initialized */
    void clear();
};

#pragma mark implementation
#pragma mark protected functions

inline int sdOSCMessage::getLengthOfOSCString(std::vector<unsigned char> OSCString, size_t onset, bool includingNullPaddings){
    
    std::vector<unsigned char>::iterator it = OSCString.begin() + onset;
    int count = 0;
    while (it != arguments.end()) {
        unsigned char byte = *it;
        if(byte == '\0'){ // if null
            break;
        }
        count++; // number of bytes including null unsigned character
        it++;
    }
    if (includingNullPaddings){
        count = count + ( 4 - (count % 4 ));
    }
    return count;
}

inline std::vector<unsigned char> sdOSCMessage::nullPadding(std::vector<unsigned char> string){
    size_t size = string.size();
    int numNullsToBeAdded = 4 - (size % 4);
    while(numNullsToBeAdded--){
        string.push_back('\0');
    }
    return string;
}

#pragma mark public functions
#pragma mark connstructor

inline sdOSCMessage::sdOSCMessage(const std::string &address){
    sdOSCMessage::address = toBlock(address); // conform to blocks (4 byte block)
    typetags.push_back(','); // the length is unknown, conform later
}

inline sdOSCMessage::sdOSCMessage(const std::vector<unsigned char> &oscMessage){
    setOSCMessage(oscMessage);
}

#pragma mark setters
inline void sdOSCMessage::setAddress(const std::string &address){
    sdOSCMessage::address = toBlock(address);
}

template <typename T>
inline void sdOSCMessage::appendArgument(const T &arg){
    this->delimiters.push_back(arguments.size());
    if (typeid(T) == typeid(int)){
        typetags.push_back('i');
    }else if(typeid(T) == typeid(float)){
        typetags.push_back('f');
    }else if(typeid(T) == typeid(double)){
        typetags.push_back('f');
    }else if(typeid(T) == typeid(std::string)){
        typetags.push_back('s');
    }else if(typeid(T) == typeid(char*)){
        typetags.push_back('s');
    }
    std::vector<unsigned char> block = toBlock(arg);
    
    arguments.insert(arguments.end(), block.begin(), block.end());
}

inline void sdOSCMessage::setOSCMessage(const std::vector<unsigned char> &newMessage){
    
    size_t cursor = 0, length = 0, lengthWithNullPaddings = 0;
    clear();
    // get address
    lengthWithNullPaddings = getLengthOfOSCString(newMessage, cursor, true); // get the length of address with null paddings
    address.insert(address.begin(), newMessage.begin(), newMessage.begin()+lengthWithNullPaddings);
    
    
    // get type tag ... without null padding because typetags can be added later
    cursor += lengthWithNullPaddings;
    length = getLengthOfOSCString(newMessage, cursor, false);
    lengthWithNullPaddings = getLengthOfOSCString(newMessage, cursor, true);
    typetags.insert(typetags.begin(), newMessage.begin()+cursor, newMessage.begin()+cursor+length);
    
    // get arguments ... with null padding;
    cursor += lengthWithNullPaddings;
    length = newMessage.size() - cursor;
    arguments.insert(arguments.begin(), newMessage.begin()+cursor, newMessage.begin()+cursor+length);
    
    //cout << "size of address:" << address.size() << endl;
    //cout << "size of typetags:" << typetags.size() << endl;
    //cout << "size of arguments:" << arguments.size() << endl;
    
    cursor = 0;
    // analyze delimiters
    size_t numArguements = typetags.size()-1;//because of ','
    for(int i = 0; i < numArguements; i++){
        delimiters.push_back(cursor);
        switch(typetags[i+1]){
            case 'i':
            case 'f':
                cursor += 4;
                break;
            case 's':
                cursor += getLengthOfOSCString(arguments, cursor, true);
                break;
            default:
                std::cout << "sdOSCMessage: unknown type tag" << std::endl;
        }
    }
    
}

#pragma mark address
inline std::vector<unsigned char> sdOSCMessage::getAddress(void){
    return address;
}

inline std::string sdOSCMessage::getAddressAsString(void){
    std::vector<unsigned char>::iterator it = address.begin();
    std::string str;
    while(it != address.end()){
        unsigned char c = *it;
        if(!c){
            break;
        }
        str += static_cast<char>(c);
        it++;
    }
    return std::move(str);
}

#pragma mark typetags
inline std::vector<unsigned char> sdOSCMessage::getTypetags(void){
    return nullPadding(typetags); // size must be multiple of 4
}

inline std::string sdOSCMessage::getTypetagsAsString(void){
    std::vector<unsigned char>::iterator it = typetags.begin();
    std::string str;
    while(it != typetags.end()){
        unsigned char c = *it;
        if(!c){
            break;
        }
        str += static_cast<char>(c);
        it++;
    }
    return std::move(str);
}

#pragma mark arguments
template <typename T>
inline T sdOSCMessage::getArgument(int index){
    size_t posDelimiter = delimiters[index];
    std::vector<unsigned char> block;
    block.insert(block.end(), arguments.begin()+posDelimiter , arguments.begin()+posDelimiter+4);
    return blockTo<T>(block);
}

template <>
inline std::string sdOSCMessage::getArgument(int index){
    size_t posDelimiter = delimiters[index];
    std::vector<unsigned char> blocks;
    int length = getLengthOfOSCString(arguments, posDelimiter, false);
    blocks.insert(blocks.end(), arguments.begin()+posDelimiter, arguments.begin()+posDelimiter+length);
    return blockTo<std::string>(blocks);
}

inline std::string sdOSCMessage::getArgumentAsString(int index){
    switch(typetags[index+1]){ // we need to skip ','
        case 'i':
            return (sdUtils::toString(getArgument<int>(index)));
            break;
        case 'f':
            return (sdUtils::toString(getArgument<float>(index)));
            break;
        case 's':
            return (sdUtils::toString(getArgument<std::string>(index)));
            break;
    }
    return "";
}

inline std::vector<unsigned char> sdOSCMessage::getArguments(void){
    return arguments;
}

inline std::string sdOSCMessage::getAllArgumentsAsString(void){
    size_t numArguments = typetags.size() -1;
    std::string str;
    for(int i = 0; i < numArguments; i++){
        str += getArgumentAsString(i);
        if (i != numArguments -1) str += " ";
    }
    return std::move(str);
}

inline size_t sdOSCMessage::getNumberOfArguments(){
    return delimiters.size();
}

#pragma mark delimiters
inline std::vector<size_t> sdOSCMessage::getDelimiters(void){
    return delimiters;
}

inline std::string sdOSCMessage::getDelimitersAsString(void){
    std::ostringstream os;
    std::vector<size_t>::iterator it = delimiters.begin();
    while(it != delimiters.end()){
        size_t i = *it;
        os << static_cast<int>(i) << ' ';
        it++;
    }
    return os.str();
}

#pragma mark entire message
inline std::vector<unsigned char> sdOSCMessage::getOSCMessage(){
    std::vector<unsigned char> OSCMessage;
    std::vector<unsigned char> conformedTypetags = getTypetags(); // with null padding
    
    OSCMessage.insert(OSCMessage.end(), address.begin(), address.end()); // concatenate address
    OSCMessage.insert(OSCMessage.end(), conformedTypetags.begin(), conformedTypetags.end()); // concatenate conformed type tags
    OSCMessage.insert(OSCMessage.end(), arguments.begin(), arguments.end()); // concatenate arguments
    
    return std::move(OSCMessage);
}

inline std::string sdOSCMessage::getMessageAsString(void){
    std::string str;
    str = getAddressAsString();
    str += ' ';
    str += getTypetagsAsString();
    str += ' ';
    str += getAllArgumentsAsString();
    return std::move(str);
}

#pragma mark clear
inline void sdOSCMessage::clear(void){
    address.clear();
    typetags.clear();
    arguments.clear();
    delimiters.clear();
    typetags.push_back(',');
}

