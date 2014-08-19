/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Test #sdEvent
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include "sdEvent.h"
#include "sdDescriptor.h"
#include <string>
#include <iostream>

// sdEvent is pure abstract class.  
// this example shows how to make a subclass of a sdEvent, in order to make your own 
// event class

class MyEvent: public sdEvent{
    
public:
    // declare descriptors that related to this type of event
    const static unsigned int numberOfDescriptors;
    const static sdDescriptor descriptors[];

    // simply redirect to super class
    MyEvent(const double time, const EDescriptor descriptor, void* const value):
    sdEvent(time, descriptor, value){};


    // these four functions should be overrided in the real subclass in your project
    bool setValue(const EDescriptor descriptor, void* const value){return true;};
    bool setValue(const std::string descriptor, const std::string value){return true;};
    std::string getDescriptorAsString(void) const{return std::string();};
    std::string getValueAsString(void) const{return std::string();};
};

// declarations of Descriptors and number of descriptors

const unsigned MyEvent::numberOfDescriptors = 1;
const sdDescriptor MyEvent::descriptors[MyEvent::numberOfDescriptors] = {
    sdDescriptor(SD_POSITION, std::string("position"), true) // give global ENUM, string and interpolation flag
};

using namespace std;

int main(void){

    {
        // initialization with the constructor
        double position[3] = {2.0, 4.0, 1.0};
        MyEvent event(0.5, SD_POSITION, static_cast<void*>(position));
        
        cout << "time:" << event.getTime() << endl; 
        cout << "descriptor:" << event.getDescriptor() << endl;
        double* dptr = static_cast<double*>(event.getValue());
        cout << "value:" << dptr[0] << ' ' << dptr[1] << ' ' << dptr[2] << endl;
        
        event.getTimeAsString();
        event.getDescriptor();
        
    }
    return 0;
}
