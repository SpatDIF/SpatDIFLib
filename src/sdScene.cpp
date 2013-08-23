//
//  sdScene.cpp
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include "sdConst.h"
#include "sdScene.h"

using namespace std;


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

void sdScene::setValue(std::string name, float time, EDescriptor descriptor,  void* value){
    
}

void* sdScene::getValue(std::string name, float time, EDescriptor descriptor){
    sdEntityCore* entity = getEntity(name);
    if (!entity) {
        return NULL;
    }
    return entity->getValue(time, descriptor);
}

vector <EExtension*> getActivatedExntesionVector(void){
    
}

void sdScene::addExtension(EExtension extension){
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
        string kind = (*it)->getKindAsString();
        cout << ">>entity name:" << entityName <<endl;
        cout << "kind:" << kind <<endl;
        cout << "number of events:" << (*it)->getNumberOfEvents()<< endl;
        set <sdEvent*, sdEventCompare> evnts = (*it)->getEventSet();
        set <sdEvent*, sdEventCompare>::iterator itt = evnts.begin();

        while(itt != evnts.end()){
            sdEvent *evt = *itt;
            string desc, valueStr;
            desc = evt->getDescriptorAsString();
            switch(evt->getDescriptor()){
                case SD_PRESENT:
                {
                    valueStr = *((bool*)evt->getValue()) ? string("true") : string("false");
                    break;
                }
                case SD_POSITION:
                {
                    float* pos = static_cast<float*>(evt->getValue());
                    ostringstream stream;
                    stream << pos[0] << " " << pos[1] << " " << pos[2];
                    valueStr = stream.str();
                    break;
                }
                case SD_ORIENTATION:
                {
                    float* ori = static_cast<float*>(evt->getValue());
                    ostringstream stream;
                    stream << ori[0] << " " << ori[1] << " " << ori[2];
                    valueStr = stream.str();
                    break;
                }
            }
            cout << "[time:" << setw(6) << left << evt->getTime() <<
            " descriptor:" << setw(10) << left << desc <<
            " value:" << setw(16)  << left << valueStr << "]" << endl;
            itt++;
        }
		it++;
	}
 

    
}
