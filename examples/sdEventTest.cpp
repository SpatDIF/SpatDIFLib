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
#include <string>
#include <iostream>

// sdEvent is pure abstract class. we need to make a subclass
class TestEvent: public sdEvent{
    
    // these four functions should be overrided in the real subclass in your project
    bool setValue(const EDescriptor descriptor, void* const value){return true;};
    bool setValue(const std::string descriptor, const std::string value){return true;};
    std::string getDescriptorAsString(void) const{return std::string();};
    std::string getValueAsString(void) const{return std::string();};
};

using namespace std;

int main(void){

    {
        // initialization with the constructor
        double position[3] = {2.0, 4.0, 1.0};
        TestEvent event(0.5, SD_POSITION, static_cast<void*>(position));
        
        cout << "time:" << event.getTime();
        cout << "descriptor:" << event.getDescriptor();
        double* dptr = static_cast<double*>(event.getValue());
        cout << "value:" << dptr[0] << ' ' << dptr[1] << ' ' << dptr[2] << endl;
        
        event.getTimeAsString();
        event.getDescriptor();
        
    }
    return 0;
}
