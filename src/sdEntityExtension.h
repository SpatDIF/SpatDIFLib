//
//  sdEntityExtension.h
//  

#ifndef _sdEntityExtension_h
#define _sdEntityExtension_h

#include <vector>
#include "sdEntity.h"

/*! a pure abstract class for entity extensions.
  the subclasses of this class must override getValue function
 */
class sdEntityExtension: public sdEntity{

protected:
    /*! sub classes must define their descriptors*/
    const static std::vector <EDescriptor>descriptors;
    const static string name;
public:
    /*! returns vector of EDescriptor that a subclass of this class is responsible for */
    static std::vector<EDescriptor> getDescriptors(void);
    
    /*! value query. Sub class of this class must override this function */
    virtual void* getValue(float time, EDescriptor descriptor);
    

};


/*
 
class sdEntityExtensionMedia: public sdEntityExtension{
    
    
    
    
    
};

sdEntityExtension::extensionName = SD_MEDIA;

class sdEntityExtensionInterpolation: public sdEntityExtension{
    
    
};

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
