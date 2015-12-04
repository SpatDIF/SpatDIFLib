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


    /*!
     @name internal utility functions for inherited classes
     @brief these three functions help the subclasss to implemment the following pure abstract functions.
     */
    
    /*!
     activate an interpolation of a descriptor.
     @param descriptor the values of this descriptor will be interpolated
     */
    bool activateInterpolation(const EDescriptor descriptor, const sdDescriptor* const descriptors, int numberOfDescriptors );
    
    /*!
     deactivate an interpolation of a descriptor.
     @param descriptor the interpolation to the values of this descriptor will be deactivated
     */
    bool deactivateInterpolation(const EDescriptor descriptor, const sdDescriptor* const descriptors, int numberOfDescriptors);
    
    /*!
     ask if the interpolation for th descriptor is activated
     @param descriptor the target descriptor
     */
    bool isInterpolationActivated(const EDescriptor descriptor, const sdDescriptor* const descriptors, int numberOfDescriptors);
    
public:
    
    /*! pure abstract function that should be implemneted in the subclasses */
    virtual bool activateInterpolation(const EDescriptor descriptor = SD_ALL) = 0;
    virtual bool deactivateInterpolation(const EDescriptor descriptor = SD_ALL) = 0;
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



inline bool sdInterpolation::activateInterpolation(const EDescriptor descriptor, const sdDescriptor* const descriptors, const int numberOfDescriptors){
    
    for(int i = 0; i < numberOfDescriptors; i++){
        if(descriptors[i].getDescriptor() == descriptor){
            if(interpolationVector[i] == SD_ENTITY_INTERPOLATION_IMPOSSIBLE){
                return false;
            }else{
                interpolationVector[i] = SD_ENTITY_INTERPOLATION_ACTIVATED;
                return true;
            }
        }
    }
    return false;
}

inline bool sdInterpolation::deactivateInterpolation(const EDescriptor descriptor, const sdDescriptor* const descriptors, const int numberOfDescriptors){
    for(int i = 0; i < numberOfDescriptors; i++){
        if(descriptors[i].getDescriptor() == descriptor){
            if(interpolationVector[i] == SD_ENTITY_INTERPOLATION_IMPOSSIBLE){
                return false;
            }else{
                interpolationVector[i] = SD_ENTITY_INTERPOLATION_DEACTIVATED;
                return true;
            }
        }
    }
    return false;
}

inline bool sdInterpolation::isInterpolationActivated(const EDescriptor descriptor, const sdDescriptor* const descriptors, const int numberOfDescriptors){
    for(int i = 0; i < numberOfDescriptors; i++){
        if(descriptors[i].getDescriptor() == descriptor){
            switch(interpolationVector[i]){
                case SD_ENTITY_INTERPOLATION_DEACTIVATED:
                case SD_ENTITY_INTERPOLATION_IMPOSSIBLE:
                    return false;
                case SD_ENTITY_INTERPOLATION_ACTIVATED:
                    return true;
                default:
                    return false;
            }
        }
    }
    return false;
}


#endif