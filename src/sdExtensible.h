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



#ifndef ____sdExtensible__
#define ____sdExtensible__

#include "sdConst.h"
#include "sdEntityExtension.h"


/*! this class add an inherited class the functionalities to attach and manage an extension */

class sdExtensible {
    
protected:
    
    /* to store the association between a descriptor and responsible extension */
    typedef struct{
        EDescriptor descriptor;
        std::string descriptorString;
        sdEntityExtension* responsibleExtension;
    } sdRedirector;
    
    /*! adding extension to the entity. invoked only by sdScene*/
    sdEntityExtension* addExtension(EExtension extension);

    /*! removing extension from the entity. invoked only by sdScene*/
    void removeExtension(EExtension extension);

    /*! contains instances of sdEntityExtenstions */
    std::vector <sdEntityExtension*>extensionVector;
    
    /*! contains pairs of EDescriptor descriptor and pointer to a responsible extension */
    std::vector <sdRedirector>redirectorVector;

    
    /*! @name Extension Handling
     @{
     */
    
    /*! get the vector of sdExtensions */
    std::vector <sdEntityExtension*>getExtensionVector();
    
    /*! returns a pointer to a sdEntityExtension in the extensionVector. returns NULL if not exists. */
    sdEntityExtension* getExtension(EExtension extension);

    /*! @} */

public:
    /*! returns a pointer to a sdEntityExtension in the extensionVector. returns NULL if not exists */
    sdEntityExtension* getResponsibleExtension(EDescriptor descriptor);
    
    
};

inline std::vector <sdEntityExtension*>sdExtensible::getExtensionVector(){
    return extensionVector;
}

#endif 
