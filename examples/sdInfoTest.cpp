/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Test all functionalities of the #sdInfo class
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include "sdInfo.h"
#include <string>
#include <iostream>

int main(void){
    using namespace std;

    {
        sdInfo info;
        
        info.setAuthor(string("John"));
        info.setHost(string("sdInfoTest"));
        info.setDate(sdDate("2000-01-01"));
        info.setSession(string("1.1"));
        info.setLocation(string("ZHDK"));
        info.setAnnotation(string("this is a test"));
        
        cout << "author:" << info.getAuthor() << endl;
        cout << "host:" << info.getHost() << endl;
        cout << "date:" << info.getDateAsString() << endl;
        cout << "session:" << info.getSession() << endl;
        cout << "location:" << info.getLocation() << endl;
        cout << "annotation:" << info.getAnnotation() << endl;
        cout << "-----" << endl;
    }
    
    {
        sdInfo info(string("Tom"), string("sdInfoTest"), sdDate(string("2012-04-03")), string("1.2"), string("ESB"), string("this is second test"));
        
        cout << "author:" << info.getAuthor() << endl;
        cout << "host:" << info.getHost() << endl;
        cout << "date:" << info.getDateAsString() << endl;
        cout << "session:" << info.getSession() << endl;
        cout << "location:" << info.getLocation() << endl;
        cout << "annotation:" << info.getAnnotation() << endl;
        cout << "-----" << endl;

    }
    
    {
        sdInfo info(string("Kevin"), string("sdInfoTest"), string("2012-05-01"), string("1.3"), string("SFEM"), string("this is third test"));
        
        cout << "author:" << info.getAuthor() << endl;
        cout << "host:" << info.getHost() << endl;
        cout << "date:" << info.getDateAsString() << endl;
        cout << "session:" << info.getSession() << endl;
        cout << "location:" << info.getLocation() << endl;
        cout << "annotation:" << info.getAnnotation() << endl;
        cout << "-----" << endl;
        
    }
    return 0;
}
