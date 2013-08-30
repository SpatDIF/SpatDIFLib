//
//  sdSaver.h
//

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
    typedef enum {
        SD_TIME,
        SD_TRACK
    } EOrdering;
    
    /*! converts a scene to XML string
     @param sdScene a sdScene
     */
    static string XMLFromScene(sdScene *scene, EOrdering ordering = SD_TIME);

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
    const string entityName; /*!< name of attached entity */
    const EKind kind; /*< kind of attached entity */
    const sdEvent *event; /*< a pointer to event */
    
public:
    sdGlobalEvent(sdEvent *event, string entityName, EKind kind);
    sdEvent* getEvent(void);
    string getEntityName(void);
    EKind getKind(void);
    string getKindAsString(void);
};

/*** inline imlmentation ***/

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

inline void sdGlobalEvent::setEntityName(string entityName){
    sdGlobalEvent::entityName = entityName;
}

inline void sdGlobalEvent::setKind(EKind kind){
    sdGlobalEvent::kind = kind;
}

/*!
 a helper class for sorting a vector of sdGlobalEvent
 */
class sdGlobalEventCompare{
public:
    bool operator()(sdGlobalEvent e1, sdGlobalEvent e2){
        if(e1.getTime() < e2.getTime()){
            return true;
        }else if(e1.getTime() == e2.getTime()){
            if(e1.getKind() == SD_SOURCE){ // source first then sink
                return true;
            }else{
                return false;
            }
        }
        else{
            return false;
        }
    }
};

#endif /* defined(____sdSaver__) */
