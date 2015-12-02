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

#ifndef ____sdInfo__
#define ____sdInfo__

#include <string>
#include <vector>

#include "sdDate.h"

#pragma mark declarations

/*! sdInfo
 is responsible for keeping, adding, removing and modifying the "info" part of the meta section
 */
class sdInfo{
    
protected:
    std::string author; /*< creator of the scene*/
    std::string host; /*< auhotring tool used for creating the scene*/
    sdDate date; /*< storing date*/
    std::string session; /*< session number*/
    std::string location; /*< studio or venue location */
    std::string annotation; /*< general comments about this scene */

public:

    /*! default constructor */
    sdInfo(){};
    
    /*! construct with parameters
     @param author creator of the scene
     @param host auhotring tool used for creating the scene
     @param date storing date
     @param session session number
     @param location studio or venue location
     @param annotation general comments about this scene
     */
    sdInfo(const std::string &author, const std::string &host, const sdDate &date, const std::string &session, const std::string &location, const std::string &annotation);
    
    /*! construct with parameters (date as string)
     @param author creator of the scene
     @param host auhotring tool used for creating the scene
     @param date storing date
     @param session session number
     @param location studio or venue location
     @param annotation general comments about this scene
     */
    sdInfo(const std::string &author, const std::string &host, const std::string &date, const std::string &session, const std::string &location, const std::string &annotation);
    
    /*! construct with parameters (c-strings)
     @deprecated use std::string version instead
     @param author creator of the scene
     @param host auhotring tool used for creating the scene
     @param date storing date
     @param session session number
     @param location studio or venue location
     @param annotation general comments about this scene
     */
    sdInfo(const char* author, const char* host, const char* date, const char* session, const char* location, const char* annotation) __attribute__((deprecated("use std::string version instead")));
    /*! @name Setters 
     @{ 
     */
    
    /*! sets author 
     @param author creator of the scene
     */
    void setAuthor(const std::string &author);
    
    /*! sets host
     @param host auhotring tool used for creating the scene
     */
    void setHost(const std::string &host);
    
    /*! sets date
     @param date storing date
     */
    void setDate(const sdDate &date);
    
    /*! sets date
     @param date storing date with a ISO 8601 string
     */
    void setDate(const std::string &date);
    
    /*! sets session
     @param session session number
     */
    void setSession(const std::string &session);

    /*! sets location
     @param location studio or venue location
     */
    void setLocation(const std::string &location);

    /*! sets annotation
     @param annotation general comments about this scene
     */
    void setAnnotation(const std::string &annotation);
    
    /*! sets all members at once. also used by some constructors
     @param author creator of the scene
     @param host auhotring tool used for creating the scene
     @param date storing date
     @param session session number
     @param location studio or venue location
     @param annotation general comments about this scene
     */
    void set(const std::string &author, const std::string &host, const sdDate &date, const std::string &session, const std::string &location, const std::string &annotation);

    /*! @} */
    
    /*! @name Getters 
     @{ 
     */

    /*! gets author */
    std::string getAuthor(void) const;
        
    /*! gets host */
    std::string getHost(void) const;

    /*! gets date */
    sdDate getDate(void) const;

    /*! gets date as ISO 8601 string */
    std::string getDateAsString(void) const;
    
    /*! gets session */
    std::string getSession(void) const;
        
    /*! gets location */
    std::string getLocation(void) const;
    
    /*! gets annotation */
    std::string getAnnotation(void) const;

    /*! @} */

};

#pragma mark implementations

inline sdInfo::sdInfo(const std::string &author, const std::string &host, const sdDate &date, const std::string &session, const std::string &location, const std::string &annotation){
    set(author, host, date, session, location, annotation);
}

inline sdInfo::sdInfo(const std::string &author, const std::string &host, const std::string &date, const std::string &session, const std::string &location, const std::string &annotation){
    const sdDate dt(date);
    set(author, host, dt, session, location, annotation);
}

inline sdInfo::sdInfo(const char* author, const char* host, const char* date, const char* session, const char* location, const char* annotation){
    const sdDate dt(date);
    // just for avoiding ambiguity
    set(std::string(author), std::string(host), dt, std::string(session), std::string(location), std::string(annotation));
}

/* setters */

inline void sdInfo::set(const std::string &author, const std::string &host, const sdDate &date, const std::string &session, const std::string &location, const std::string &annotation){
    this->author = author;
    this->host = host;
    this->date = date;
    this->location = location;
    this->session = session;
    this->annotation = annotation;
}

inline void sdInfo::setAuthor(const std::string &author){
    this->author = author;
}

inline void sdInfo::setHost(const std::string &host){
    this->host = host;
}

inline void sdInfo::setDate(const sdDate &date){
    this->date = date;
}

inline void sdInfo::setDate(const std::string &date){
    sdDate dt(date);
    this->date = dt;
}

inline void sdInfo::setSession(const std::string &session){
    this->session = session;
}

inline void sdInfo::setLocation(const std::string &location){
    this->location = location;
}

inline void sdInfo::setAnnotation(const std::string &annotation){
    this->annotation = annotation;
}

/* getters */
inline std::string sdInfo::getAuthor() const{
    return author;
}

inline std::string sdInfo::getHost() const{
    return host;
}

inline sdDate sdInfo::getDate() const{
    return date;
}

inline std::string sdInfo::getDateAsString() const{
    return date.getDateAsString();
}

inline std::string sdInfo::getSession() const{
    return session;
}

inline std::string sdInfo::getLocation() const{
    return location;
}

inline std::string sdInfo::getAnnotation() const{
    return annotation;
}

#endif