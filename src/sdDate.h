//
//  sdDate.h
//

#ifndef ____sdDate__
#define ____sdDate__

#include <vector>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

/*! sdDate
 maintains the data of a date (year, month, day) and offers utility functionalities listed below.
 - interpretation and generation of ISO 8601 formmated strings.
 - initialization of the member variables with the current date employing the system calls
 */

class sdDate{
    
private:
    unsigned short year; /*!< year of the date*/
    unsigned short month; /*!< month of the date*/
    unsigned short day; /*!< day of the date*/
    
public:
    /*! @name Constructors/Destructors
     @{
     */
    
    /*! a constructor. if the autoInit frag is set to true, the constructor initializes the date using the sytem calls.
     @param autoInit if true, the date will be initialized using the system call. if false, uninitialized. the default value is true.
     */
    sdDate(bool autoInit = true);
    
    /*! construct with parameters
     @param year year of the date
     @param month month of the date
     @param day day of the date
     */
    sdDate(unsigned short year, unsigned short month, unsigned short day);
    
    /*! construct with date as string
     @param dateString ISO 8601 string. must be YYYY-MM-DD format.
     */
    sdDate(string dateString);
    
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
    void setDate(string dateString);
    
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
     @date an allocated buffer of unsigned short. the size must be greater than 3.
     */
    void getDate(unsigned short *date);
    
    /*! generates ISO 8601 string from the given data and return it*/
    string getDateAsString(void);
    
    /*! gets year.*/
    unsigned short getYear(void);
    
    /*! gets month.*/
    unsigned short getMonth(void);
    
    /*! gets day.*/
    unsigned short getDay(void);
    
    /*! @} */
};

/*** implementation ***/

// constructors/destructors
inline sdDate::sdDate(bool autoInit){
    if(autoInit){
        setCurrentDate();
    }
}

inline sdDate::sdDate(unsigned short year, unsigned short month, unsigned short day){
    setDate(year,month,day);
}

inline sdDate::sdDate(string dateString){
    setDate(dateString);
}

// setters
inline void sdDate::setDate(unsigned short year, unsigned short month, unsigned short day){
    sdDate::year = year;
    sdDate::month = month;
    sdDate::day = day;
}

inline void sdDate::setDate(string dateString){
    istringstream is(dateString);
    char dummy;
    is >> year >> dummy >> month >> dummy >> day;
}

inline void sdDate::setYear(unsigned short year){
    sdDate::year = year;
}

inline void sdDate::setMonth(unsigned short month){
    sdDate::month = month;
}

inline void sdDate::setDay(unsigned short day){
    sdDate::day = day;
}

inline void sdDate::setCurrentDate(void){
    time_t t = time(0);
    struct tm * now = localtime( & t );
    year = now->tm_year + 1900;
    month = now->tm_mon + 1;
    day = now->tm_mday;
}

inline void sdDate::getDate(unsigned short *date){
    date[0] = year;
    date[1] = month;
    date[2] = day;
}

inline string sdDate::getDateAsString(void){
	ostringstream os;
    os << year << '-' << month << '-' << day;
    return os.str();
}

// getters
inline unsigned short sdDate::getYear(void){
    return year;
}

inline unsigned short sdDate::getMonth(void){
    return month;
}

inline unsigned short sdDate::getDay(void){
    return day;
}
#endif /* defined(____sdDate__) */
