//
//  sdInfo.h
//

#ifndef ____sdInfo__
#define ____sdInfo__

#include <string>
#include <vector>
#include "sdConst.h"
#include "sdDate.h"

using namespace std;

/*! sdInfo
 is responsible for keeping, adding, removing and modifying the "info" part of the meta section
 */
class sdInfo{
    
private:
    string author; /*< creator of the scene*/
    string host; /*< auhotring tool used for creating the scene*/
    sdDate date; /*< storing date*/
    string session; /*< session number*/
    string location; /*< studio or venue location */
    string annotation; /*< general comments about this scene */

public:
    
    /*! constructor */
    sdInfo();

    /*! construct with parameters
     @param author creator of the scene
     @param host auhotring tool used for creating the scene
     @param date storing date
     @param session session number
     @param location studio or venue location
     @param annotation general comments about this scene
     */
    sdInfo(string author, string host, sdDate date, string session, string location, string annotation);
    
    /*! construct with parameters (date as string)
     @param author creator of the scene
     @param host auhotring tool used for creating the scene
     @param date storing date
     @param session session number
     @param location studio or venue location
     @param annotation general comments about this scene
     */
    sdInfo(string author, string host, string date, string session, string location, string annotation);
    
    /*! @name Setters 
     @{ 
     */
    
    /*! sets author 
     @param author creator of the scene
     */
    void setAuthor(string author);
    
    /*! sets host
     @param host auhotring tool used for creating the scene
     */
    void setHost(string host);
    
    /*! sets date
     @param date storing date
     */
    void setDate(sdDate date);
    
    /*! sets date
     @param date storing date with a ISO 8601 string
     */
    void setDate(string date);
    
    /*! sets session
     @param session session number
     */
    void setSession(string session);

    /*! sets location
     @param location studio or venue location
     */
    void setLocation(string location);

    /*! sets annotation
     @param annotation general comments about this scene
     */
    void setAnnotation(string annotation);
    
    /*! sets all members at once
     @param author creator of the scene
     @param host auhotring tool used for creating the scene
     @param date storing date
     @param session session number
     @param location studio or venue location
     @param annotation general comments about this scene
     */
    void set(string author, string host, sdDate date, string session, string location, string annotation);

    /*! @} */
    
    /*! @name Getters 
     @{ 
     */

    /*! gets author */
    string getAuthor(void);
        
    /*! gets host */
    string getHost(void);

    /*! gets date */
    sdDate getDate(void);

    /*! gets date as ISO8601 string */
    string getDateAsString(void);
    
    /*! gets session */
    string getSession(void);
        
    /*! gets location */
    string getLocation(void);
    
    /*! gets annotation */
    string getAnnotation(void);

    /*! @} */

};

/*** inline implementations ***/

inline sdInfo::sdInfo(){

}

inline sdInfo::sdInfo(string author, string host, sdDate date, string session, string location, string annotation){
    set(author, host, date, session, location, annotation);
}

inline sdInfo::sdInfo(string author, string host, string date, string session, string location, string annotation){
    sdDate dt(date);
    set(author, host, dt, session, location, annotation);
}

/* setters */

inline void sdInfo::set(string author, string host, sdDate date, string session, string location, string annotation){
    sdInfo::author = author;
    sdInfo::host = host;
    sdInfo::date = date;
    sdInfo::location = location;
    sdInfo::session = session;
    sdInfo::annotation = annotation;
}

inline void sdInfo::setAuthor(string author){
    sdInfo::author = author;
}

inline void sdInfo::setHost(string host){
    sdInfo::host = host;
}

inline void sdInfo::setDate(sdDate date){
    sdInfo::date = date;
}

inline void sdInfo::setDate(string date){
    sdDate dt;
    dt.setDate(date);
    sdInfo::date = dt;
}

inline void sdInfo::setSession(string session){
    sdInfo::session = session;
}

inline void sdInfo::setLocation(string location){
    sdInfo::location = location;
}

inline void sdInfo::setAnnotation(string annotation){
    sdInfo::annotation = annotation;
}

/* getters */
inline string sdInfo::getAuthor(){
    return author;
}

inline string sdInfo::getHost(){
    return host;
}

inline sdDate sdInfo::getDate(){
    return date;
}

inline string sdInfo::getDateAsString(){
    return date.getDateAsString();
}

inline string sdInfo::getSession(){
    return session;
}

inline string sdInfo::getLocation(){
    return location;
}

inline string sdInfo::getAnnotation(){
    return annotation;
}

#endif /* defined(____sdInfo__) */
