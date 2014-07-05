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
/*! sdDate
	abstract class for descriptor
*/

class sdDescriptor{
	
public:
	EDescriptor descriptor;
	std::string descriptorString;
	bool interpolatable;

};

#endif