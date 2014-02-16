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

#ifndef ____sdSaver__
#define ____sdSaver__

#include <string>
#include "sdScene.h"

using namespace std;


/*!
 convert a sdScene to XML, JSON, or YAML strings
 */
class sdSaver
{

public:
    
    /*! converts a scene to XML string
     @param sdScene a sdScene
     */
    static string XMLFromScene(sdScene *scene);
    
    /*! converts a scene to JSON string
     @param sdScene a sdScene
     */
    static string JSONFromScene(sdScene *scene);
    
    /*! converts a scene to JSON string
     @param sdScene a sdScene
     */
    static string YAMLFromScene(sdScene *scene);


};


/*!
 a helper class for sdSaver. This packs sdEvent, entityName and kind together.
 This class is used only within sdSaver
 */

class sdGlobalEvent{
    
private:
    sdEvent *event; /*< a pointer to event */
    string entityName; /*!< name of attached entity */
    EKind kind; /*< kind of attached entity */
    
public:
    sdGlobalEvent(sdEvent *event, string entityName, EKind kind);
    
    sdEvent* getEvent(void);
    string getEntityName(void);
    EKind getKind(void);
    string getKindAsString(void);
};

/*** inline implementation ***/

inline sdGlobalEvent::sdGlobalEvent(sdEvent *event, string entityName, EKind kind){
    sdGlobalEvent::event = event;
    sdGlobalEvent::entityName = entityName;
    sdGlobalEvent::kind = kind;
}

inline sdEvent* sdGlobalEvent::getEvent(void){
    return event;
}

inline string sdGlobalEvent::getEntityName(void){
    return entityName;
}

inline EKind sdGlobalEvent::getKind(void){
    return kind;
}



/*!
 a helper class for sorting a vector of sdGlobalEvent
 */
class sdGlobalEventCompare{
public:
    bool operator()(sdGlobalEvent e1, sdGlobalEvent e2){
        if(e1.getEvent()->getTime() < e2.getEvent()->getTime()){
            //e1 before e2 on the time line
            return true;
        }else if(e1.getEvent()->getTime() > e2.getEvent()->getTime()){
            //e2 before e1 on the time line
            return false;
        }else{
            // e1 and e2 at the same time
            
            //1. check kind --- if different, source first
            if(e1.getKind() != e2.getKind()){
                if(e1.getKind() == SD_SOURCE){
                    return true;
                }else{
                    return false;
                }
            }else{
                //if kind is same
                //2. order by name of attached entity
                string e1EntityName = e1.getEntityName();
                string e2EntityName = e2.getEntityName();
                
                if(e1EntityName < e2EntityName){
                    return true;
                }else if(e1EntityName > e2EntityName){
                    return false;
                }else{
                    // if same entity
                    //3. check descriptor --- core first
                    bool e1Core = sdEntityCore::isCoreDescriptor(e1.getEvent()->getDescriptor());
                    bool e2Core = sdEntityCore::isCoreDescriptor(e2.getEvent()->getDescriptor());
                    if(e1Core == true && e2Core == false){// core before extension
                        return true; 
                    }else if(e1Core == false && e2Core == true){//extension before core
                        return false;
                    }else{
                        //4. sort descriptor alphabetically
                        if(e1.getEvent()->getDescriptorAsString() < e2.getEvent()->getDescriptorAsString()){
                            return true;
                        }else{
                            return false;
                        }
                    }
                }
            }
        }
        
    }
};

#endif /* defined(____sdSaver__) */
