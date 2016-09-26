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

#pragma once
#include <string>
#include <vector>

#include "sdDate.h"

#pragma mark declarations

/*! sdInfo
 is responsible for keeping, adding, removing and modifying the "info" part of the meta section
 */
class sdInfo{
    
protected:
    std::string author; ///< creator of the scene*/
    std::string host; ///< auhotring tool used for creating the scene*/
    sdDate date; ///< storing date*/
    std::string session; ///< session number*/
    std::string location; ///< studio or venue location */
    std::string annotation; ///* general comments about this scene */
    std::string title; ///< title of the piece
    double duration; ///< duration of the piece
    
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
     @param title title of the piece
     @param duration duration of the piece
     */
    sdInfo(const std::string &author, const std::string &host, const sdDate &date, const std::string &session, const std::string &location, const std::string &annotation, const std::string &title = "untitled", const double &duration = 0.0);
    
    /*! construct with parameters (date as string)
     @param author creator of the scene
     @param host auhotring tool used for creating the scene
     @param date storing date
     @param session session number
     @param location studio or venue location
     @param annotation general comments about this scene
     @param title title of the piece
     @param duration duration of the piece
     */
    sdInfo(const std::string &author, const std::string &host, const std::string &date, const std::string &session, const std::string &location, const std::string &annotation, const std::string &title = "untitled", const double &duration = 0.0);
    
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
    
    /*! sets title
     @param title title of the piece
     */
    void setTitle(const std::string &title);

    /*! sets location
     @param duration duration of the piece in second
     */
    void setDuration(const double &duration);
    
    
    /*! sets all members at once. also used by some constructors
     @param author creator of the scene
     @param host auhotring tool used for creating the scene
     @param date storing date
     @param session session number
     @param location studio or venue location
     @param annotation general comments about this scene
     */
    void set(const std::string &author, const std::string &host, const sdDate &date, const std::string &session, const std::string &location, const std::string &annotation, const std::string &title, const double &duration);

    /*! @} */
    
    /*! @name Getters 
     @{ 
     */
    const std::string &getAuthor(void) const; ///<@brief returns author info
    const std::string &getHost(void) const; ///<@brief returns host info
    const sdDate &getDate(void) const; ///< @brief returns date object
    std::string getDateAsString(void) const; ///< @brief returns date as ISO 8601 string
    const std::string &getSession(void) const; ///< @brief returns session info
    const std::string &getLocation(void) const; ///< @brief returns location info
    const std::string &getAnnotation(void) const; ///< @brief returns annotation
    const std::string &getTitle() const; ///< @brief returns title
    const double &getDuration(void) const; ///< @brief retusn duration
    /*! @} */

};

#pragma mark implementations

inline sdInfo::sdInfo(const std::string &author, const std::string &host, const sdDate &date, const std::string &session, const std::string &location, const std::string &annotation, const std::string &title, const double &duration){
    set(author, host, date, session, location, annotation, title, duration);
}

inline sdInfo::sdInfo(const std::string &author, const std::string &host, const std::string &date, const std::string &session, const std::string &location, const std::string &annotation, const std::string &title, const double &duration){
    const sdDate dt(date);
    set(author, host, dt, session, location, annotation, title, duration);
}

inline sdInfo::sdInfo(const char* author, const char* host, const char* date, const char* session, const char* location, const char* annotation){
    const sdDate dt(date);
    // just for avoiding ambiguity
    set(std::string(author), std::string(host), dt, std::string(session), std::string(location), std::string(annotation), "untitled", 0.0);
}

/* setters */

inline void sdInfo::set(const std::string &author, const std::string &host, const sdDate &date, const std::string &session, const std::string &location, const std::string &annotation, const std::string &title, const double &duration){
    this->author = author;
    this->host = host;
    this->date = date;
    this->location = location;
    this->session = session;
    this->annotation = annotation;
    this->title = title;
    this->duration = duration;
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

inline void sdInfo::setTitle(const std::string &title){
    this->title = title;
}

inline void sdInfo::setDuration(const double &duration){
    this->duration = duration;
}

/* getters */
inline const std::string &sdInfo::getAuthor() const{
    return author;
}

inline const std::string &sdInfo::getHost() const{
    return host;
}

inline const sdDate &sdInfo::getDate() const{
    return date;
}

inline std::string sdInfo::getDateAsString() const{
    return date.getDateAsString();
}

inline const std::string &sdInfo::getSession() const{
    return session;
}

inline const std::string &sdInfo::getLocation() const{
    return location;
}

inline const std::string &sdInfo::getAnnotation() const{
    return annotation;
}

inline const std::string &sdInfo::getTitle() const{
    return title;
}

inline const double &sdInfo::getDuration() const{
    return duration;
}
