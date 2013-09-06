//
//  sdEntityExtension.h
//  

#ifndef _sdEntityExtension_h
#define _sdEntityExtension_h

#include <string>
#include <vector>
#include "sdConst.h"
#include "sdEntity.h"

using namespace std;



/*! a pure abstract class for entity extensions.
  the subclasses of this class must override getValue function
 */
class sdEntityExtension: public sdEntity{
    
protected:
    /*! sub classes must define their descriptors*/
    const static vector <EDescriptor>descriptors;
public:
    /*! returns vector of EDescriptor that a subclass of this class is responsible for */
    static vector<EDescriptor> getDescriptors(void);
    
    
    virtual EExtension getExtensionName(void) = 0;
    virtual string getExtensionNameAsString(void) = 0;

       
};

#endif
