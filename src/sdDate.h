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

#include <vector>
#include <ctime>
#include <string>
#include <sstream>
#include <iostream>

#pragma mark decralations

/*! sdDate
 maintains data of a date (year, month, day) and offers utility functionalities listed below.
 - interpretation and generation of ISO 8601 formmated strings.
 - initialization of the member variables with the current date employing the system calls
 */

class sdDate{
    
protected:

    unsigned short year; /*!< year of the date */
    unsigned short month; /*!< month of the date */
    unsigned short day; /*!< day of the date */
    
public:
    /*! @name Constructors/Destructors
     @{
     */
    
    /*! a constructor. if the autoInit frag is set to true, the constructor initializes the date using the sytem calls.
     @param autoInit if true, the date will be initialized using the system call. if false, uninitialized. the default value is true.
     */
    sdDate(const bool autoInit = true);
    
    /*! construct with parameters
     @param year year of the date
     @param month month of the date
     @param day day of the date
     */
    sdDate(unsigned short year, unsigned short month, unsigned short day);
    
    
    /*! construct with cstr
     @param cstr ISO 8601 string. must be YYYY-MM-DD format.
     */
    sdDate(const char* const cDateString);
    
    /*! construct with date as string
     @param dateString ISO 8601 string. must be YYYY-MM-DD format.
     */
    sdDate(const std::string &dateString);
    
    /*! @} */
    
    /*! @name Setters 
     @{
     */
    
    /*! a date setter.
     @param year year of the date
     @param month month of the date
     @param day day od the date
     */
    void setDate(unsigned short year, unsigned short month, unsigned short day);
    
    /*! a date setter with a ISO 8601 string.
     @param dateString ISO 8601 standard string. must be YYYY-MM-DD format. hyphens are not omittable.
     */
    void setDate(const std::string &dateString);
    
    /*! sets year.*/
    void setYear(unsigned short year);
    
    /*! sets month.*/
    void setMonth(unsigned short month);
    
    /*! sets day.*/
    void setDay(unsigned short day);
    
    /*! set member variables to current date using the system call */
    void setCurrentDate(void);
    
    /*! @} */
    
    /*! @name Getters 
     @{
     */
    
    /*! gets date as an array.
     @deprecated use std::vector<unsigned short>getDate() instead
     @param date an allocated buffer of unsigned short. the size of unsighed short array must be exactly 3.
     */
    void getDate(unsigned short (&date)[3]) const __attribute__((deprecated("use std::vector<unsigned int>getDate() instead")));
    
    /*! gets date as a vector.
     @param date an allocated buffer of unsigned short. the size of unsighed short array must be exactly 3.
     @return std::vector<unsinged short> that contains year, month, and day.
     */
    std::vector<unsigned short> getDate() const;
    
    /*! generates ISO 8601 string from the given data and return it*/
    std::string getDateAsString(void) const;

    /*! gets year.*/
    unsigned short getYear(void) const;
    
    /*! gets month.*/
    unsigned short getMonth(void) const;
    
    /*! gets day.*/
    unsigned short getDay(void) const;
    
    /*! @} */
    
    
    /*! @name conversion functions 
     @{
     */
    
    /*! conversion function that internally calls getDateAsString() */
    operator std::string(){return getDateAsString();};
    
    /*! @} */

};

#pragma mark implementations

// constructors/destructors
inline sdDate::sdDate(const bool autoInit){
    if(autoInit){
        setCurrentDate();
    }
}

inline sdDate::sdDate(const unsigned short year, const unsigned short month, const unsigned short day){
    setDate(year,month,day);
}

inline sdDate::sdDate(const char* const cDateString){
    setDate(std::string(cDateString));
}

inline sdDate::sdDate(const std::string &dateString){
    setDate(dateString);
}

// setters
inline void sdDate::setDate(const unsigned short year, const unsigned short month, const unsigned short day){
    this->year = year;
    this->month = month;
    this->day = day;
}

inline void sdDate::setDate(const std::string &dateString){
    std::istringstream is(dateString);
    char dummy1, dummy2;
    is >> year >> dummy1 >> month >> dummy2 >> day;
    if(!(dummy1 == '-' && dummy2 == '-')){
        this->setCurrentDate();
    }
}

inline void sdDate::setYear(const unsigned short year){
    this->year = year;
}

inline void sdDate::setMonth(const unsigned short month){
    this->month = month;
}

inline void sdDate::setDay(const unsigned short day){
    this->day = day;
}

inline void sdDate::setCurrentDate(void){
    time_t t = time(0);
    struct tm * now = localtime( & t );
    year = now->tm_year + 1900;
    month = now->tm_mon + 1;
    day = now->tm_mday;
}

inline void sdDate::getDate(unsigned short (&date)[3]) const{
    date[0] = year;
    date[1] = month;
    date[2] = day;
}

inline std::vector<unsigned short> sdDate::getDate() const{
    return std::move(std::vector<unsigned short>{year, month, day});
}

inline std::string sdDate::getDateAsString(void) const{
    std::ostringstream os;
    os << year << '-' << month << '-' << day;
    return os.str();
}

inline unsigned short sdDate::getYear(void) const{
    return year;
}

inline unsigned short sdDate::getMonth(void) const{
    return month;
}

inline unsigned short sdDate::getDay(void) const{
    return day;
}

