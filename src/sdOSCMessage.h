#ifndef ____sdOSCMessage__
#define ____sdOSCMessage__

#include "sdOSCConverter.h"
#include <sstream>

/*!
 sdOSCMessage generates and interprets a valid OSC message consisting of multiple blocks (blocks of 4 bytes)
 */
class sdOSCMessage: public sdOSCConverter{

    
private:
    vector<unsigned char> address;
    vector<unsigned char> typetags;
    vector<unsigned char> arguments;
    vector<int> delimiters;
    
    /*!
     this function looks for \0 unsigned character from the onset byte and return the length of string
     @param startIndex the onset of search point
     */
    int getLengthOfOSCString(vector<unsigned char> OSCString, int onset, bool includingNullPaddings);
    vector<unsigned char> nullPadding(vector<unsigned char> string);
    
public:
    inline vector<unsigned char> getAddress(void);
    inline vector<unsigned char> getTypetags(void);
    inline vector<unsigned char> getArguments(void);
    inline vector<int> getDelimiters(void);

    inline string getAddressAsString(void);
    inline string getTypetagsAsString(void);
    inline string getArgumentsAsString(void);
    inline string getDelimitersAsString(void);
    
    /*!
     constructor. the OSC address must be specified.
     @param oscAddress the OSC address string
     */
    sdOSCMessage(string address);
    sdOSCMessage(vector<unsigned char> message);
    
    /*!
     output osc conformed message
     */
    vector<unsigned char> getOSCMessage(void);
    
    /*!
     interpret raw OSC message and store it in the local buffer
     */
    void setOSCMessage(vector<unsigned char> message);

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
    string getEntireArgumentsAsString(void);
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


// to be implemented
string sdOSCMessage::getArgumentsAsString(void){
    
    string rstr;
    return rstr;
}

#endif