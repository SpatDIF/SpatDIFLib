//
//  sdScene.cpp
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include "sdConst.h"
#include "sdScene.h"
#include "sdConverter.h"

using namespace std;

void* sdScene::getValue(std::string name, float time, EDescriptor descriptor,  void* value){
    sdEntityCore *entity = getEntity(name);
//    return entity->getValue(time, descriptor, );
}

sdEntityCore* sdScene::getEntity(std::string name){
    vector <sdEntityCore*>::iterator it = entityVector.begin();
    while( it != entityVector.end() ){
		string entityName = (*it)->getName();
        if(entityName == name){
            return *it;
        }
		it++;
	}
    return NULL;
}

unsigned int sdScene::getNumberOfEntities(void){
    return entityVector.size();
}


sdEntityCore* sdScene::addEntity(string name, EKind kind){
    sdEntityCore* entityCore = new sdEntityCore(name, kind); // private constructor

    // attach extensions here
    
    
    // insert the entity in the vector
    entityVector.push_back(entityCore);
    
    // return the pointer
    return entityCore;
}

void sdScene::removeEntity(std::string name){
    sdEntityCore* entity = getEntity(name);
    if(entity){
        removeEntity(entity);
    }
}

void sdScene::removeEntity(sdEntityCore *entity){

}

void addExtension(sdEntityExtension *extension){
    // tobe coded
}

void sdScene::removeExtension(EExtension extension){
    // tobe coded

}

void sdScene::dump(void){
    using namespace std;
    cout << "---------- Meta ----------"<< endl;
    cout << "author:" << info.getAuthor() <<endl;
    cout << "annotaion:" <<  info.getAnnotation() <<endl;
    cout << "number of entity used:" <<  getNumberOfEntities()<<endl;

    cout << "---------- Entities --------" << endl;
    vector <sdEntityCore*>::iterator it = entityVector.begin();
    while( it != entityVector.end() )
	{
		string entityName = (*it)->getName();
        string kind = sdConverter::convert((*it)->getKind());
        cout << ">>entity name:" << entityName <<endl;
        cout << "kind:" << kind <<endl;
        cout << "number of events:" << (*it)->getNumberOfEvents()<< endl;
        set <sdEvent*, sdEventCompare> evnts = (*it)->getEventSet();
        set <sdEvent*, sdEventCompare>::iterator itt = evnts.begin();

        while(itt != evnts.end()){
            sdEvent *evt = *itt;
            string desc, valueStr;
            desc = sdConverter::convert(evt->descriptor);
            switch(evt->descriptor){
                case SD_TYPE:
                {
                    EType *type = static_cast<EType*>(evt->value);
                    valueStr = sdConverter::convert(*type);
                    break;
                }
                case SD_PRESENT:
                {
                    valueStr = *((bool*)evt->value) ? string("true") : string("false");
                    break;
                }
                case SD_POSITION:
                {
                    float* pos = static_cast<float*>(evt->value);
                    ostringstream stream;
                    stream << pos[0] << " " << pos[1] << " " << pos[2];
                    valueStr = stream.str();
                    break;
                }
                case SD_ORIENTATION:
                {
                    float* ori = static_cast<float*>(evt->value);
                    ostringstream stream;
                    stream << ori[0] << " " << ori[1] << " " << ori[2];
                    valueStr = stream.str();
                    break;
                }
            }
            cout << "[time:" << setw(6) << left << evt->time <<
            " descriptor:" << setw(10) << left << desc <<
            " value:" << setw(16)  << left << valueStr << "]" << endl;
            itt++;
        }
		it++;
	}
 

    
}
