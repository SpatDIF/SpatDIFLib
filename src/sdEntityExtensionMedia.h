#ifndef _sdEntityExtensionMedia_h
#define _sdEntityExtensionMedia_h

#include <string>
#include "sdConst.h"
#include "sdEntityExtension.h"

using namespace std;

class sdScene;

/*!
 a media extension. This extension contains information about an audio media
 */
class sdEntityExtensionMedia: public sdEntityExtension{
    friend class sdScene;
    
    /*!
     this constructor should be involed from an instance of sdScene
     */
    sdEntityExtensionMedia(){};
    
    string id; /*< unique identifier */
    string type; /*< where the content comes from */
    string location; /*< location of the file or stream*/
    int channel; /*< If type has more channels, define the channel that is taken as input*/
    double timeOffset;/*< Starting position within media file */
    double gain;/*< gain value of the media*/
    
    static const EExtension extensionName; /*< identification of the class */
    
public:
    EExtension getExtensionName(void);
    string getExtensionNameAsString(void);
    
    /*!
     query method. it simply returns a pointer to the buffer, where the designated data are stored. returns null if not found. This is a pure abstract function.
     @param time specify time
     @param descriptor specify descriptor defined in sdConst.h
     */
    void* getValue(float time, EDescriptor descriptor);

};

/*** inline implementation ***/
inline EExtension sdEntityExtensionMedia::getExtensionName(void){
    return extensionName;
}

inline string sdEntityExtensionMedia::getExtensionNameAsString(void){
    return string("media");
}

#endif
