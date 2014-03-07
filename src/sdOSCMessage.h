#ifndef ____sdOSCMessage__
#define ____sdOSCMessage__

#include "sdOSCConverter.h"
#include <sstream>

/*!
 sdOSCMessage generates and interprets a valid OSC message consisting of multiple blocks (blocks of 4 bytes)
 */
class sdOSCMessage: public sdOSCConverter{

    
private:

    vector<unsigned char> address; /*!< OSC address pattern in OSC-String format */
    vector<unsigned char> typetags;  /*!< OSC typetags in OSC-String format */
    vector<unsigned char> arguments; /*!< OSC Arguments in 4-bytes-block sequence */
    vector<int> delimiters; /*!< stores delimiters of OSC arguments. This increase the speed of data access */
    
    /*!
     this function looks for \0 unsigned character from the onset byte and return the length of string
     @param startIndex the onset of search point
     */
    int getLengthOfOSCString(vector<unsigned char> OSCString, int onset, bool includingNullPaddings);
    vector<unsigned char> nullPadding(vector<unsigned char> string);
    
public:

    /*! returns OSC address pattern in OSC-string format */
    inline vector<unsigned char> getAddress(void);

    /*! returns OSC typetags in OSC-string format */
    inline vector<unsigned char> getTypetags(void);

    /*! returns all Arguments in in 4-bytes-block sequence */
    inline vector<unsigned char> getArguments(void);
    
    /*! returns delimiters as vector of int */
    inline vector<int> getDelimiters(void);

    /*! returns OSC address pattern as a string */
    inline string getAddressAsString(void);

    /*! returns OSC address pattern as a string */
    inline string getTypetagsAsString(void);

    /*! returns delimiters as a string */
    inline string getDelimitersAsString(void);

    /*! @param index argument index
     returns specified argument at index as int.*/
    int getArgumentAsInt(int index);

    /*! @param index argument index
     returns specified argument at index as float.*/
    float getArgumentAsFloat(int index);

    /*! @param index argument index
     returns specified argument at index as string.*/
    string getArgumentAsString(int index);

    /*! returns all arguments as a string*/
    string getAllArgumentsAsString(void);

    /*! returns the entire message as a string*/
    string getMessageAsString(void);

    /*! constructor. typeyags initialized. */
    sdOSCMessage();

    /*! constructor. with initialization of typetags and address pattern. */
    sdOSCMessage(string address);

    /*! constructor. initialize message with OSC-conformed 4-bytes-block sequence */
    sdOSCMessage(vector<unsigned char> message);
    
    /*! @param address new OSC address pattern
    set address pattern*/
    void setAddress(string address);

    /*! output osc conformed message */
    vector<unsigned char> getOSCMessage(void);
    
    /*! interpret raw OSC message and store it in the local buffer */
    void setOSCMessage(vector<unsigned char> message);

    /*! @param value an int value to be appended.
     appends an int and add an typetag */
    void appendInt(int value);
    
    /*! 
     @param ints a pointer to int values to be appended.
     @param number number of ints to be added
     appends multiple ints at once */
    void appendInts(int *ints, int number);

    /*!
     @param value an float value to be appended.
     appends an float and add an typetag */
    void appendFloat(float value);

    /*! 
     @param floats a pointer to float values to be appended.
     @param number number of floats to be added
     appends multiple floats at once */
    void appendFloats(float *floats, int number);

    /*!
     @param str string to be appended
     appends an string and add an typetag */
    void appendString(string str);

    /*! 
     @param str strings to be appended
     appends multiple strings at once */
    void appendStrings(vector<string> str);
    
    /*! returns the number of OSC arguments */
    inline int getNumberOfArgument();

    /*! delete entire content. typeyag initialized */
    inline void clear();

};

vector<unsigned char> sdOSCMessage::getAddress(void){
    return address;
}

vector<unsigned char> sdOSCMessage::getTypetags(void){
    return nullPadding(typetags); // size must be multiple of 4 
}

vector<unsigned char> sdOSCMessage::getArguments(void){
    return arguments;
}

vector<int> sdOSCMessage::getDelimiters(void){
    return delimiters;
}

string sdOSCMessage::getAddressAsString(void){
    vector<unsigned char>::iterator it = address.begin();
    string str;
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

string sdOSCMessage::getTypetagsAsString(void){
    vector<unsigned char>::iterator it = typetags.begin();
    string str;
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

string sdOSCMessage::getDelimitersAsString(void){
    ostringstream os;
    vector<int>::iterator it = delimiters.begin();
    while(it != delimiters.end()){
        int i = *it;
        os << static_cast<int>(i) << ' ';
        it++;
    }
    return os.str();
}

int sdOSCMessage::getNumberOfArgument(){
    return delimiters.size();
}

void sdOSCMessage::clear(void){
    address.clear();
    typetags.clear();
    arguments.clear();
    delimiters.clear();
    typetags.push_back(','); 
}

#endif