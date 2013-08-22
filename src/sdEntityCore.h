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

class sdScene;

class sdEntityCore: public sdEntity{
    friend class sdScene; // only sdScene can instantiate this class
    
private:
    /*! the name of the entity. This name is invariable (const)*/
    const std::string name;
    
    /*! contains instances of sdEntityExtenstions */
    std::vector <sdEntityExtension>extensionVector;
    
    /*! define the kind of the entity. This property is declared as const and invariable (const). */
    const EKind kind;
    
    /*! constructor. intentionally declared as a private function. 
     the sdEntityCore class must be instantiated through the addEntity function of sdScene. this guarantees that all instances of sdEntityCore are be aware of their parent scene and activated extensions.
     @param name the name of new Entity
     @param kind the kind of new Entity. default = SD_SOURCE
     */
    sdEntityCore(std::string name, EKind kind = SD_SOURCE) :name(name), kind(kind){} ;// constructor with an initializer
    
public:
    /*!  returns the name of the entity */
    std::string getName(void);
    
    /*! returns the kind of the entity */
    EKind getKind(void);
    
    /*! a overrieded query function. it simply returns a pointer to the buffer, where the designated data are stored. returns null if not found. 
     @param time
     @param descriptor
     */
    void* getValue(float time, EDescriptor descriptor);
    
    /*! a overrieded query function. it returns a std::set of sdEvents with specified descriptor whose time parameter is between start and end time
     @param start start time
     @param end end time
     @param descriptor
     */
    std::set <sdEvent*, sdEventCompare>getValue(float start, float end, EDescriptor descriptor);

};


/*** inline implementation ***/

inline EKind sdEntityCore::getKind(void){
    return kind;
}

inline std::string sdEntityCore::getName(void){
    return name;
}

#endif /* defined(____sdEntityCore__) */
