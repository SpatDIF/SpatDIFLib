/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: ???
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright © 2013 by ??? @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include <iostream>
#include <string>
#include "sdConst.h"
using namespace std;

int main(void){
    double x[] = {3.0, 4.0, 5.0};
    double y = 6.0;
    double z[3];
    string sx = "1 2 3";
    string sy = "12.50";
    

    cout << doublesToString(x,3) << endl;
    cout << doubleToString(y) << endl;
    stringToDoubles(sx,z,3);
    cout << z[0] << ' ' << z[1] << ' ' << z[2] << endl;
    cout << stringToDouble(sy) << endl;
    
    return 0;
}
