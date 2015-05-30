/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Test Source Width extentions
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include <string>
#include <set>
#include "sdScene.h"
#include "sdEntityCore.h"

void populateMultiset(std::multiset<sdEvent*, sdEventCompare> eventSet);

int main(void){
    
    using namespace std;
    
    // a scene without extensions
    sdScene scene;
    scene.addExtension(SD_SOURCE_WIDTH);
    
    cout << "Number of activated extensions: " << scene.getNumberOfActivatedExtensions() << std::endl;
    
    cout << "list of activated extension:" << endl;
    vector<string> extensionStringVector = scene.getActivatedExtensionsAsStrings();
    vector<string>::iterator it = extensionStringVector.begin();
    while(it != extensionStringVector.end()){
        cout << *it << endl;
        it++;
    }
    
    sdEntityCore* entityOne = scene.addEntity("voice1");

    // the following messages add event to the core
    double width = 105.0;
    
    entityOne->addEvent(5.0, SD_SOURCE_WIDTH_WIDTH, static_cast<void*>(&width));
    entityOne->addEvent(string("10.0"), string("width"), string("99"));

    sdEvent * eventOne = entityOne->getNextEvent(2.0, SD_SOURCE_WIDTH_WIDTH);
    sdEvent * eventTwo = entityOne->getNextEvent(9.0, SD_SOURCE_WIDTH_WIDTH);

    cout << "event one " << eventOne->getValueAsString() << endl;
    cout << "event two " << eventTwo->getValueAsString() << endl;

    scene.dump();
    scene.removeExtension(SD_SOURCE_WIDTH);

    return 0;
}

