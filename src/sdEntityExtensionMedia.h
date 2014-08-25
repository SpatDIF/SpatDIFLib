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

#ifndef ____sdEntityExtensionMedia__
#define ____sdEntityExtensionMedia__

#include <string>
#include "sdConst.h"
#include "sdEntityExtension.h"
#include "sdEventExtensionMedia.h"
#include "sdDescriptor.h"


/*!
 a media extension. This extension contains information about an audio media
 */
class sdEntityExtensionMedia: public sdEntityExtension{
    
    friend class sdEntityCore; /*< only entityCore can spawn instances of this class*/

private:

    
    /*!
     this constructor should be invoked from an instance of sdEntityCore
     */
    sdEntityExtensionMedia(){
        for(int i = 0; i < sdEventExtensionMedia::numberOfDescriptors; i++){
            if(sdEventExtensionMedia::descriptors[i].getInterpolability()){
                interpolationVector.push_back(SD_ENTITY_INTERPOLATION_DEACTIVATED);
            }else{
                interpolationVector.push_back(SD_ENTITY_INTERPOLATION_IMPOSSIBLE);
            }
        }
    };
    
public:
    
    static const EExtension extensionName; /*< identification of the class */
    static const std::string extensionNameAsString; /*< identification of the class */

    /*! @name Event handling
     @{
     */
    
    /*! a method for adding a new event to the entity*/
    sdEvent* addEvent(const double time, const EDescriptor descriptor, void* const value);
    
    /*! same as above but you can specify arguments with strings */
    sdEvent* addEvent(const std::string time, const std::string descriptor, const std::string value);
    
    void removeEvent(const std::string time, const std::string descriptor);

    /*!
     @}
    */
    
    /*!
     activate an interpolation of a descriptor.
     @param descriptor the values of this descriptor will be interpolated
     */
    bool activateInterpolation(const EDescriptor descriptor);
    
    /*!
     deactivate an interpolation of a descriptor.
     @param descriptor the interpolation to the values of this descriptor will be deactivated
     */
    bool deactivateInterpolation(const EDescriptor descriptor);
    
    /*!
     ask if the interpolation for th descriptor is activated
     @param descriptor the target descriptor
     */
    bool isInterpolationActivated(const EDescriptor descriptor);
    
    
    EExtension getExtensionName(void);
    std::string getExtensionNameAsString(void);
    
};

inline EExtension sdEntityExtensionMedia::getExtensionName(void){
    return extensionName;
}

inline std::string sdEntityExtensionMedia::getExtensionNameAsString(void){
    return extensionNameAsString;
}

#endif
