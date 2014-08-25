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

#ifndef ____sdInterpolation__
#define ____sdInterpolation__

/*! sdInterpolation
	an abstract strategy class for the functionality of interpolation.
	The subclasses of this abstract class provides sdEntities with the functionality of interpolation.
	all subclass of sdInterpolation must override the virtual abstract function, interpolate()
*/

#include "sdConst.h"

class sdInterpolation{

protected:
    typedef enum{
        SD_ENTITY_INTERPOLATION_IMPOSSIBLE,
        SD_ENTITY_INTERPOLATION_ACTIVATED,
        SD_ENTITY_INTERPOLATION_DEACTIVATED
    } EInterpolation;
    
    std::vector<EInterpolation> interpolationVector; /*! < activation status for each interpolation */

    
    /*! linear interpolation template */
    template <typename T>
    T interpolate(T valueA, T valueB, double weight);

    /*! 2D interpolation template */
    template <typename T>
    std::vector<T> interpolate(std::vector<T> valueA, std::vector<T> valueB, double weight);

    
public:
    
    /*!
     activate an interpolation of a descriptor.
     @param descriptor the values of this descriptor will be interpolated
     */
    virtual bool activateInterpolation(const EDescriptor descriptor) = 0;
    
    /*!
     deactivate an interpolation of a descriptor.
     @param descriptor the interpolation to the values of this descriptor will be deactivated
     */
    virtual bool deactivateInterpolation(const EDescriptor descriptor) = 0;
    
    /*!
     ask if the interpolation for th descriptor is activated
     @param descriptor the target descriptor
     */

    virtual bool isInterpolationActivated(const EDescriptor descriptor) = 0;
    
};

template <typename T>
inline std::vector<T> sdInterpolation::interpolate(std::vector<T> vectorA, std::vector<T> vectorB, double weight){
    
    if(vectorA.size() != vectorB.size()){
        std::cout << "sdInterpolation input vector size unmatched" << std::endl;
    }
    
    std::vector<T> interpolatedVector;
    size_t size = vectorA.size();
    for(int i = 0; i < size; i++){
        interpolatedVector.push_back(interpolate(vectorA[i], vectorB[i], weight));
    }
    return interpolatedVector;
}

template <typename T>
inline T sdInterpolation::interpolate(T valueA, T valueB, double weight){
    return (valueB - valueA)  * weight + valueA;
}

#endif