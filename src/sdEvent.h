#ifndef ____sdEvent__
#define ____sdEvent__

#include <set>
#include <string>
#include <iostream>
#include <sstream>
#include "sdConst.h"

using namespace std;

class sdEntity;

/*!
 maintains a set of event data. The constructor is intentionally declared as a protected function. This must be invoked through  sdEntity or its subclasses. This process ensures that all existing instances of sdEvents belong to a sdEntity and are aware of required extensions. The class is also responsible for the allocation of void* value by checking the descriptor. Thus, desciptor and value should be simultaneously set by set or setVlaue functions.
*/
class sdEvent{
    

protected:
    float time;/*!< time of the event */
    EDescriptor descriptor;/*!<descriptor of the event*/
    void *value;/*!<value of the event. sdEntity allocates the void pointer automatically*/
    
    /*! 
     construcor is intentionally declared as protected. this class must be instantiated through sdEntity::addEvent
     */
    sdEvent(void){};
    
    /*!
     constructor with initialization.
     @param time time of the event
     @param descriptor descriptor of the event. declared in sdConst.h
     @param value void pointer to value to be copied. Proper size of memory will be internally allocated.
     */
    sdEvent(float time, EDescriptor descriptor, void* value);
    
    /*! 
     this constructor facilitates to load string based data to the entity. it converts all strings to proper data types and invoke the function above.
     */
    sdEvent(string time, string descriptor, string value);
    
    /*!
     internal functions for conversion from Enum to string
     @param descriptor descriptor declared in sdConst.h
     */
     virtual string convert(EDescriptor descriptor) = 0;
    
    /*!
     internal functions for conversion from Enum to string
     @param kind kind declared in sdConst.h
     */
    virtual string convert(EKind kind) = 0;
    
    /*!
     internal functions for conversion from Enum to string
     @param type type declared in sdConst.h
     */
    virtual string convert(EType type) = 0;
    
    /*!
     converts string to enums. The return value should be casted to the proper enum.
     @param enum
     */
    virtual int convert(string str) = 0;
    
public:
    /*!
     returns time as Float
     */
    float getTime(void);
    /*!
     returns time as string
     */
    string getTimeAsString(void);
    
    /*!
     returns descriptor as Enum
     */
    EDescriptor getDescriptor(void);
    
    /*!
     returns descriptor as string
     */
    string getDescriptorAsString(void);

    /*!
     returns value through void pointer. It should be casted to propoer type manually.
     */
    void* getValue(void);

    /*!
     returns value as string
     */
    string getValueAsString(void);

    /*!
     set al three parameters at once
     @param time time of event
     @param desciptor desciprot of event declared in sdConst.h
     @param value value of event
     */
    void set(float time, EDescriptor descriptor, void* value);
    
    /*!
     set time of the event
     @param time time of the event
     */
    void setTime(float time);

    /*! 
     descriptor and value should be set at once. because this function allocates memory to void* value based on the descriptor
    */
    void setValue(EDescriptor descriptor, void* value);
};

/*** inline implementation ***/
inline sdEvent::sdEvent(float time, EDescriptor descriptor, void* value){
    this->set(time, descriptor, value);
}

inline float sdEvent::getTime(void){
    return time;
}

inline string sdEvent::getTimeAsString(void){
    ostringstream timeStringStream;
    timeStringStream << time;
    return timeStringStream.str();
}

inline EDescriptor sdEvent::getDescriptor(void){
    return descriptor;
}


inline void* sdEvent::getValue(void){
    return value;
}

inline void sdEvent::set(float time, EDescriptor descriptor, void* value){
    setTime(time);
    setValue(descriptor, value);
}

inline void sdEvent::setTime(float time){
    sdEvent::time = time;
}

/*!
 a helper class for sorting events
*/
class sdEventCompare{
public:
    bool operator()(sdEvent* e1, sdEvent* e2){
        if(e1->getTime() < e2->getTime()) // sort by time
            return true;
        else
            return false;
    }
};

#endif /* defined(____sdEvent__) */

