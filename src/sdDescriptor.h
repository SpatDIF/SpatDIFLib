/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#ifndef ____sdDescriptor__
#define ____sdDescriptor__

#include <string>
#include "sdConst.h"

using namespace std;
/*! sdDescriptor
	This class contains the name of descriptor in enum, string and interpolation flag of the descriptor. An instance of this class will be used as a static member of entities, in order to keep the name and interpolability of the value.
*/

class sdDescriptor{
	
private:
	EDescriptor descriptor;
	string descriptorString;
	bool interpolability;

public:

	/*! 
		constructor. All the member variable must be initialized at once when instantiated.
	*/
	sdDescriptor(EDescriptor descriptor, string descriptorString, bool interpolability);
	EDescriptor getDescriptor() const;
	string getDescriptorAsString() const;
	bool getInterpolability() const;
};

inline sdDescriptor::sdDescriptor(EDescriptor descriptor, string descriptorString, bool interpolability){
	sdDescriptor::descriptor = descriptor;
	sdDescriptor::descriptorString = descriptorString;
	sdDescriptor::interpolability = interpolability;
}

inline  EDescriptor sdDescriptor::getDescriptor() const{
	return descriptor;
}

inline string sdDescriptor::getDescriptorAsString() const{
	return descriptorString;
}

inline bool sdDescriptor::getInterpolability() const{
	return interpolability;
}

// utility functions

inline string getDescriptorAsString(EDescriptor descriptor, const sdDescriptor* descriptors, int numberOfDescriptors){

    for(int i = 0; i < numberOfDescriptors; i++){
        if(descriptors[i].getDescriptor() == descriptor){
            return descriptors[i].getDescriptorAsString();
        }
    }
    return string("error");
}

inline EDescriptor getDescriptorAsEnum(string descriptor, const sdDescriptor* descriptors, int numberOfDescriptors){
    for(int i = 0; i < numberOfDescriptors; i++){
        if(descriptors[i].getDescriptorAsString() == descriptor){
            return descriptors[i].getDescriptor();
        }
    }
    return SD_ERROR;
}
#endif