#ifndef ____sdOSCMessage__
#define ____sdOSCMessage__

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
#pragma mark get osc message parts
    
    /*! returns OSC address pattern in OSC-string format */
    std::vector<unsigned char> getAddress(void);

    /*! returns OSC typetags in OSC-string format */
    std::vector<unsigned char> getTypetags(void);

    /*! returns all Arguments in in 4-bytes-block sequence */
    std::vector<unsigned char> getArguments(void);
    
    /*! returns delimiters as vector of int */
    std::vector<size_t> getDelimiters(void);

#pragma get message as strings
    /*! returns OSC address pattern as a string */
    std::string getAddressAsString(void);

    /*! returns OSC address pattern as a string */
    std::string getTypetagsAsString(void);

    /*! returns delimiters as a string */
    std::string getDelimitersAsString(void);

    /*! @param index argument index
     returns specified argument at index as int.*/
    template <typename T>
    T getArgument(int index);

    /*! returns all arguments as a string*/
    std::string getAllArgumentsAsString(void);

    /*! returns the entire message as a string*/
    std::string getMessageAsString(void);

    /*! constructor. typeyags initialized. */
    sdOSCMessage();

    /*! constructor. with initialization of typetags and address pattern. */
    sdOSCMessage(std::string address);

    /*! constructor. initialize message with OSC-conformed 4-bytes-block sequence */
    sdOSCMessage(std::vector<unsigned char> message);
    
    /*! @param address new OSC address pattern
    set address pattern*/
    void setAddress(std::string address);

    /*! output osc conformed message */
    std::vector<unsigned char> getOSCMessage(void);
    
    /*! interpret raw OSC message and store it in the local buffer */
    void setOSCMessage(std::vector<unsigned char> message);

    /*! template function for appending arguments */
    template <typename T>
    void appendArgument(const T &args);

    /*! returns the number of OSC arguments */
    size_t getNumberOfArgument();

    /*! delete entire content. typeyag initialized */
    void clear();

};

inline std::vector<unsigned char> sdOSCMessage::getAddress(void){
    return address;
}

inline std::vector<unsigned char> sdOSCMessage::getTypetags(void){
    return nullPadding(typetags); // size must be multiple of 4 
}

inline std::vector<unsigned char> sdOSCMessage::getArguments(void){
    return arguments;
}

inline std::vector<size_t> sdOSCMessage::getDelimiters(void){
    return delimiters;
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
    return str;
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
    return str;
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

inline size_t sdOSCMessage::getNumberOfArgument(){
    return delimiters.size();
}


inline sdOSCMessage::sdOSCMessage(){
    typetags.push_back(','); // the length is unknown, conform later
}

inline sdOSCMessage::sdOSCMessage(std::string address){
    sdOSCMessage::address = toBlock(address); // conform to blocks (4 byte block)
    sdOSCMessage(); // call default constructor
}

inline sdOSCMessage::sdOSCMessage(std::vector<unsigned char> oscMessage){
    setOSCMessage(oscMessage);
}

inline std::vector<unsigned char> sdOSCMessage::getOSCMessage(){
    std::vector<unsigned char> OSCMessage;
    std::vector<unsigned char> conformedTypetags = getTypetags(); // with null padding
    
    OSCMessage.insert(OSCMessage.end(), address.begin(), address.end()); // concatenate address
    OSCMessage.insert(OSCMessage.end(), conformedTypetags.begin(), conformedTypetags.end()); // concatenate conformed type tags
    OSCMessage.insert(OSCMessage.end(), arguments.begin(), arguments.end()); // concatenate arguments
    
    return OSCMessage;
}

inline void sdOSCMessage::setAddress(std::string address){
    sdOSCMessage::address = toBlock(address);
}

inline void sdOSCMessage::setOSCMessage(std::vector<unsigned char> newMessage){
    
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

inline std::vector<unsigned char> sdOSCMessage::nullPadding(std::vector<unsigned char> string){
    size_t size = string.size();
    int numNullsToBeAdded = 4 - (size % 4);
    while(numNullsToBeAdded--){
        string.push_back('\0');
    }
    return string;
}

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
    if (includingNullPaddings)
    {
        count = count + ( 4 - (count % 4 ));
    }
    
    return count;
}

template <typename T>
inline void sdOSCMessage::appendArgument(const T &arg){
    this->delimiters.push_back(arguments.size());
    
    if (typeid(T) == typeid(int)){
        this->typetags.push_back('i');
    }else if(typeid(T) == typeid(float)){
        this->typetags.push_back('f');
    }else if(typeid(T) == typeid(std::string)){
        this->typetags.push_back('s');
    }


    //this->arguments.insert(arguments.end(), block.begin(), block.end());
}

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

inline std::string sdOSCMessage::getAllArgumentsAsString(void){
    size_t numArguments = typetags.size() -1;
    std::string str;
    for(int i = 0; i < numArguments; i++){
        switch(typetags[i+1]){ // we need to skip ','
            case 'i':
                str += toString(getArgument<int>(i));
                break;
            case 'f':
                str += toString(getArgument<float>(i));
                break;
            case 's':
                str += getArgument<std::string>(i);
                break;
        }
        str += " ";
    }
    return str;
}

inline std::string sdOSCMessage::getMessageAsString(void){
    std::string str;
    str = getAddressAsString();
    str += ' ';
    str += getTypetagsAsString();
    str += ' ';
    str += getAllArgumentsAsString();
    return str;
}


inline void sdOSCMessage::clear(void){
    address.clear();
    typetags.clear();
    arguments.clear();
    delimiters.clear();
    typetags.push_back(',');
}

#endif