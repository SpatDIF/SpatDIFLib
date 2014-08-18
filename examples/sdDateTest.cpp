/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Examine all functionalities of the #sdDate class
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include "sdDate.h"
#include <string>
#include <iostream>

int main(void){
    using namespace std;
    
    {
        // the default constructor initializes the date with the current date using system calls
        sdDate date;
        cout << "init with current date, using system calls:" << endl;
        cout << date.getDateAsString() << endl;
    }
    
    {
        // autoInit disabled
        sdDate noDate(false);
        cout << "auto init disabled:" << endl;
        cout << noDate.getDateAsString() << endl;
    }
    
    {
        // manual initialization
        cout << "manual initialization:" << endl;
        sdDate myDate(1979, 12, 4); // my birth day
        cout << myDate.getDateAsString() << endl;
    }
    
    {
        // manual initialization with a string, formatted by ISO 8601
        sdDate manualDate("2011-10-11"); // my date of marrige
        cout << "manual initialization from a string:" << endl;
        cout << manualDate.getDateAsString() << endl;
    }
    
    {
        // you can assign a sdDate object to a string. it will be automatically converted to a ISO 8601 string
        sdDate date; // initialized with current date
        string convertedString = date; // converted to string
        cout << "converted date:" << endl;
        cout << convertedString << endl;
    }
    
    {
        sdDate date(false); // uninitialized
        
        //set date with setters
        date.setYear(2010);
        date.setMonth(12);
        date.setDay(31);
        
        cout << "get year, month, and day one by one:" << endl;
        std::cout << date.getYear() << '-' << date.getMonth() << '-' << date.getDay() << endl;
        
        cout << "reset to current date manually:" << endl;
        date.setCurrentDate();
        string dateString = date.getDateAsString();
        std::cout << dateString << endl;
    }
    
    {
        unsigned short array[3];
        sdDate myDate(2002, 3, 2);
        myDate.getDate(array);
        
        cout << "get date as an array:" << endl;
        std::cout << array[0] << '-' << array[1] << '-' << array[2] << std::endl;
        
    }
    return 0;
}
