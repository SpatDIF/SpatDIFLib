//
//  sdEntityExtension.h
//  

#ifndef _sdEntityExtension_h
#define _sdEntityExtension_h

#include <string>
#include <vector>
#include "sdEntity.h"

/*! a pure abstract class for entity extensions.
  the subclasses of this class must override getValue function
 */
class sdEntityExtension: public sdEntity{
    
protected:
    /*! sub classes must define their descriptors*/
    const static std::vector <EDescriptor>descriptors;
public:
    /*! returns vector of EDescriptor that a subclass of this class is responsible for */
    static std::vector<EDescriptor> getDescriptors(void);
    
    /*! value query. Sub class of this class must override this function */
    virtual void* getValue(float time, EDescriptor descriptor);
    

};

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
    double time_offset;/*< Starting position within media file */
    double gain;/*< gain value of the media*/
};



class sdEntityExtensionLoop public sdEntityExtension{
    string type;
    double points[2];
    double wait_time;
};

/*
class sdEntityExtensionInterpolation: public sdEntityExtension{
    
    
};

class sdEntityExtensionLoop: public sdEntityExtension{
    
    
};

class sdEntityExtensionDistanceCue: public sdEntityExtension{
    
    
};

class sdEntityExtensionDirectToOne: public sdEntityExtension{
    
    
};

class sdEntityExtensionHaredWareOut: public sdEntityExtension{
    
    
};

 */

#endif
