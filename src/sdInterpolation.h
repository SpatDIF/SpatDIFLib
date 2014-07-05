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

/*! sdInterpolation
	an abstract strategy class for the functionality of interpolation.
	The subclasses of this abstract class provides sdEntities with the functionality of interpolation.
	all subclass of sdInterpolation must override the virtual abstract function, interpolate()
*/

#include "sdConst.h"

class sdInterpolation{

public:
	/*!
		virtual abstract function. should be overrided in subclasses.
	*/
	virtual void* interpolate(sdEntity* entity, EDescriptor descriptor, double time) = 0;

};

/*! sdLinearInterpolation
	provides the linear interpolation functionality
*/

class sdLinearInterpolation: public sdInterpolation{
	void* interpolate(sdEntity* entity, EDescriptor descriptor, double time);
};

inline void* sdLinearInterpolation::interpolate(sdEntity* entity, EDescriptor descriptor, double time)
{

}