#ifndef ____sdOSCMessage__
#define ____sdOSCMessage__

#include "sdOSCConverter.h"

/*!
 sdOSCMessage generates and interprets a valid OSC message consisting of multiple nibbles (blocks of 4 bytes)
 */
class sdOSCMessage: public sdOSCConverter{

    
private:
    vector<char> address;
    vector<char> typetags;
    vector<char> arguments;
    vector<int> delimeters;
    
    /*!
     this function looks for \0 character from the startIndex byte and return the length of string
     @param startIndex the onset of search point
     */
    int getLengthOfString(int startIndex);
    
public:
    inline vector<char> getAddress(void);
    inline vector<char> getTypetags(void);
    inline vector<char> getArgument(void);
    inline vector<int> getDelimiters(void);

    inline string getAddressAsString(void);
    inline string getTypetagsAsString(void);
    inline string getArgumentsAsString(void);

    
    /*!
     constructor. the OSC address must be specified.
     @param oscAddress the OSC address string
     */
    sdOSCMessage(string address);
    sdOSCMessage(vector<char> message);
    
    /*!
     output osc conformed message
     */
    vector<char> getOSCMessage(void);
    
    /*!
     interpret raw OSC message and store it in the local buffer
     */
    void setOSCMessage(vector<char> message);

    /*!
     append an int and add an typetag
     */
    void appendInt(int value);
    
    /*!
     append an float and add an typetag
     */
    void appendFloat(float value);

    /*!
     append an string and add an typetag
     */
    void appendString(string str);
    
    int getArgumentAsInt(int index);
    float getArgumentAsFloat(int index);
    string getArgumentAsString(int index);

};

inline vector<char> sdOSCMessage::getAddress(void){
    return address;
}

vector<char> sdOSCMessage::getTypetags(void){
    return typetags;
}

vector<char> sdOSCMessage::getArgument(void){
    return arguments;
}

inline vector<int> sdOSCMessage::getDelimiters(void){
    return delimeters;
}

string sdOSCMessage::getAddressAsString(void){
    return address.data();
}

string sdOSCMessage::getTypetagsAsString(void){
    return typetags.data();
}

// to be implemented
string sdOSCMessage::getArgumentsAsString(void){
    
    string rstr;
    return rstr;
}

#endif