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
        // the default constructor initializes the date with the current date
        sdDate date;
        // output the result
        std::cout << date.getYear() << '-' << date.getMonth() << '-' << date.getDay() << endl;
    }
    
    {
        // autoInit disabled
        sdDate noDate(false);
        // output the result
        std::cout << noDate.getYear() << '-' << noDate.getMonth() << '-' << noDate.getDay() << endl;
    }
    
    {
        // manual initialization
        sdDate myDate(1979, 12, 4); // my birth day
        // output the result
        std::cout << myDate.getYear() << '-' << myDate.getMonth() << '-' << myDate.getDay() << endl;
    }
    
    {
        // manual initialization with a string, formatted by ISO 8601 
        sdDate yourDate(string("1979-05-07")); // the birth day of my wife
        // output the result
        std::cout << yourDate.getYear() << '-' << yourDate.getMonth() << '-' << yourDate.getDay() << endl;
    }
    
    {
        sdDate date(false); // uninitialized
        
        //set params
        date.setYear(2010);
        date.setMonth(12);
        date.setDay(31);
        
        //get params as an array
        unsigned short dateArray[3];
        date.getDate(&dateArray[0]);
        
        std::cout << dateArray[0] << '-' << dateArray[1] << '-' << dateArray[2] << endl;
        
        date.setCurrentDate();
        string dateString = date.getDateAsString();
        std::cout << dateString << endl;
    }
    return 0;
}
