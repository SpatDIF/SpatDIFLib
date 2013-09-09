//
//  sdEntityCore.h
//  

#ifndef ____sdEntityCore__
#define ____sdEntityCore__


#include <set>
#include <string>
#include "sdConst.h"
#include "sdEntity.h"
#include "sdEntityExtension.h"

using namespace std;

/*!
 this class stores event info (i.e. time, descriptor and value) of sdEntityCore. 
 */
class sdEventCore: public sdEvent{
    
    /*! only sdEntityCore can make an instance of this class */
    friend class sdEntityCore;

public:
    /*!
     constructor with initialization.
     @param time time of the event
     @param descriptor descriptor of the event. declared in sdConst.h
     @param value void pointer to value to be copied. Proper size of memory will be automatically allocated.
     */
    sdEventCore(double time, EDescriptor descriptor, void* value);
    
    /*! same as above but you can give all arguments with string */
    sdEventCore(string time, string descriptor, string value);
    
    /*! destructor destroy all allocated memory to the value pointer*/
    ~sdEventCore();
    
    /*! overrided method. get descriptor as string e.g. "position" */
    string getDescriptorAsString(void);
    
    /*! overrided method. get value as string e.g. "0.3 0.5 0.2"*/
    string getValueAsString(void);
    
private:
    /*! 
     @name private setter
     The modification of parameter after the instantiation is not allowed.
     All member variables of this class should be initialized through the constructor and should not be modified after that in order to increase the stability
     @{
     */
    
    /*! set value. this function allocate memory to the void pointer member variable according to the given EDescriptor */
    bool setValue(EDescriptor descriptor, void* value);
    
    /*! same as above but you can give all arguments with strings */
    bool setValue(string descriptor, string value);
    
    /*! @} */
};

class sdScene;

/*!
 sdEntityCore is responsible for
 - identifing its kind (source or sink)
 - maintaining its events
 - managing extensions
 - giveing pointer to a certain, ranged, or the entire list of event(s)
 - forwarding a query to a proper extension and answer to the client
 */

class sdEntityCore: public sdEntity{
    friend class sdScene; // only sdScene can instantiate this class
    
private:
    const static int numberOfCoreDescriptors;
    const static EDescriptor coreDescriptors[];
    const static string coreDescriptorStrings[];
    static bool isCoreDescriptor(EDescriptor descriptor);
    static bool isCoreDescriptor(string descriptor);
    
    typedef struct{
        EDescriptor descriptor;
        string descriptorString;
        EExtension extensionName;
        sdEntityExtension* responsibleExtension;
    } sdRedirector;
    
    /*! the name of the entity. This name is invariable (const)*/
    const string name;
    
    /*! contains instances of sdEntityExtenstions */
    vector <sdEntityExtension*>extensionVector;
    
    /*! contains pairs of EDescriptor descriptor and pointer to a responsible extension */
    vector <sdRedirector>redirectorVector;

    /*! define the kind of the entity. This property is declared as const and invariable (const). */
    EKind kind;
    EType type;
    
    /*! constructor. intentionally declared as a private function. 
     the sdEntityCore class must be instantiated through the addEntity function of sdScene. this guarantees that all instances of sdEntityCore are be aware of their parent scene and activated extensions.
     @param name the name of new Entity
     @param kind the kind of new Entity. default = SD_SOURCE
     */
    sdEntityCore(string name, EKind kind = SD_SOURCE, EType type = SD_POINT) :name(name), kind(kind){
        sdEntityCore::type = type;
    };
   
public:
    
    /*!
     @name Setter/Getter
     @{
     */
    
    /*! this returns a set of sdEvents with specified descriptor whose time parameter is between start and end time
     @param start start time
     @param end end time
     @param descriptor
     */
    
    /*!  returns the name of the entity */
    string getName(void);
    
    /*! returns the kind of the entity */
    EKind getKind(void);
    
    /*! returns the kind of the entity as a string*/
    string getKindAsString(void);

    /*! returns the type of the entity */
    EType getType(void);
    string getTypeAsString(void);
    
    vector <sdEntityExtension*>getExtensionVector(void);
    /*! This is the only method for adding an new event to the entity*/
    sdEvent* addEvent(double time, EDescriptor descriptor, void* value);
    
    /*! same as above but you can specify arguments with string */
    sdEvent* addEvent(string time, string descriptor, string value);

    void removeEvent(double time, EDescriptor descriptor);
    void removeEvent(string time, string descriptor);

    /*! @name Extensions
        @{
     */
    
    /*! adding extension to the entity*/
    sdEntityExtension* addExtension(EExtension extension);
    
    /*! removing extension from the entity*/
    void removeExtension(EExtension extension);

    /*! @} */
    
    /*! @name Utilities
     @{ */

    /*! this class provides static functions for conversion */
    static const EDescriptor stringToDescriptor(string str);
    static const string descriptorToString(EDescriptor descriptor);
    
    /*! @} */
     
    /*!overrided function*/
    void* getValue(double time, EDescriptor descriptor);

    /*!static function for sorting. employed by sdSaver*/
    static bool sortAlphabetically( sdEntityCore *leftEntity, sdEntityCore *rightEntity);
};


/*** inline implementation ***/
inline string sdEntityCore::getName(void){
    return name;
}

inline EKind sdEntityCore::getKind(void){
    return kind;
}

inline EType sdEntityCore::getType(void){
    return type;
}

inline vector <sdEntityExtension*>sdEntityCore::getExtensionVector(void){
    return extensionVector;
}

inline bool sdEntityCore::sortAlphabetically( sdEntityCore *leftEntity,  sdEntityCore *rightEntity){
    return leftEntity->getName().compare(rightEntity->getName()) ? false:true;
}

#endif /* defined(____sdEntityCore__) */
