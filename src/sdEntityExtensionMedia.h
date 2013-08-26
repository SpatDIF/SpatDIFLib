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
    
protected:
    
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
    
public:
    static const EExtension extensionName; /*< identification of the class */
};

#endif
