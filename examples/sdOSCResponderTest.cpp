/** @file
 *
 * @ingroup spatdiflib-examples
 *
 * @brief Example: Test #sdOSCResponder
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#include "sdMain.h"
#include <string>
#include <iostream>
#include <iomanip>

int main(void){
  using namespace std;
  
  sdScene scene;
  sdEntityCore *myEntity = scene.addEntity("myEntity");
  myEntity->addEvent(string("1.0"), string("position"), string("0.0 0.1 0.2"));

  sdOSCResponder oscResponder(&scene);
  scene.dump();
  std::cout.setf( std::ios::showpoint );  

  vector<sdOSCMessage> returnedMessageVector;

  cout << "----set/get queryTime----" << endl;
  {
    sdOSCMessage setQueryTime("/spatdifcmd/setQueryTime");
    setQueryTime.appendFloat(1.52);
  	oscResponder.forwardOSCMessage(setQueryTime);   
  	sdOSCMessage getQueryTime("/spatdifcmd/getQueryTime");
   	returnedMessageVector = oscResponder.forwardOSCMessage(getQueryTime);
   	cout << returnedMessageVector[0].getMessageAsString() << endl;
  }
  cout << "----set/get writeTime----" << endl;
  {
    sdOSCMessage setWriteTime("/spatdifcmd/setWriteTime");
    setWriteTime.appendFloat(3.14);
    oscResponder.forwardOSCMessage(setWriteTime);   
    sdOSCMessage getWriteTime("/spatdifcmd/getWriteTime");
    returnedMessageVector = oscResponder.forwardOSCMessage(getWriteTime);
    cout << returnedMessageVector[0].getMessageAsString() << endl;
  }
  
  cout << "----set/get Interval----" << endl;
  {
    sdOSCMessage setInterval("/spatdifcmd/setInterval");
    setInterval.appendFloat(2.25);
    oscResponder.forwardOSCMessage(setInterval);   
    sdOSCMessage getInterval("/spatdifcmd/getInterval");
    returnedMessageVector = oscResponder.forwardOSCMessage(getInterval);
    cout << returnedMessageVector[0].getMessageAsString() << endl;
  }
  
  cout << "----set/get Position----" << endl;
  {
  // both query and write at 10.0
    sdOSCMessage setWriteTime("/spatdifcmd/setWriteTime");
    setWriteTime.appendFloat(10.0);
    oscResponder.forwardOSCMessage(setWriteTime);
    sdOSCMessage setQueryTime("/spatdifcmd/setQueryTime");
    setQueryTime.appendFloat(10.0);
    oscResponder.forwardOSCMessage(setQueryTime);

    sdOSCMessage setPosition("/spatdifcmd/setPosition");
    setPosition.appendString("myEntity");
    float pos[3] = {1.0, 2.0, 3.0};
    setPosition.appendFloats(pos, 3);
    oscResponder.forwardOSCMessage(setPosition);

    sdOSCMessage getPosition("/spatdifcmd/getPosition");
    getPosition.appendString("myEntity");
    returnedMessageVector = oscResponder.forwardOSCMessage(getPosition);
    cout << returnedMessageVector[0].getMessageAsString() << endl;
  }
  {
    // search event with next command
    cout << "query time at: 5.0. ask for next event" << endl;
    sdOSCMessage setQueryTime("/spatdifcmd/setQueryTime");
    setQueryTime.appendFloat(5.0);
    oscResponder.forwardOSCMessage(setQueryTime);

    sdOSCMessage getNextPosition("/spatdifcmd/getNextPosition");
    getNextPosition.appendString("myEntity");
    returnedMessageVector = oscResponder.forwardOSCMessage(getNextPosition);
    cout << returnedMessageVector[0].getMessageAsString() << endl;
  }
  {
    // search event with previous command
    cout << "query time at: 15.0. ask for previous event" << endl;
    sdOSCMessage setQueryTime("/spatdifcmd/setQueryTime");
    setQueryTime.appendFloat(15.0);
    oscResponder.forwardOSCMessage(setQueryTime);

    sdOSCMessage getPreviousPosition("/spatdifcmd/getPreviousPosition");
    getPreviousPosition.appendString("myEntity");
    returnedMessageVector = oscResponder.forwardOSCMessage(getPreviousPosition);
    cout << returnedMessageVector[0].getMessageAsString() << endl;
  }
  cout << "----set/get Orientation----" << endl;
  {
    sdOSCMessage setOrientation("/spatdifcmd/setOrientation");
    setOrientation.appendString("myEntity");

    sdOSCMessage setWriteTime("/spatdifcmd/setWriteTime");
    setWriteTime.appendFloat(10.0);
    oscResponder.forwardOSCMessage(setWriteTime);
    sdOSCMessage setQueryTime("/spatdifcmd/setQueryTime");
    setQueryTime.appendFloat(10.0);
    oscResponder.forwardOSCMessage(setQueryTime);

    float ori[3] = {1.0, 2.0, 3.0};
    setOrientation.appendFloats(ori, 3);
    oscResponder.forwardOSCMessage(setOrientation);

    sdOSCMessage getOrientation("/spatdifcmd/getOrientation");
    getOrientation.appendString("myEntity");
    returnedMessageVector = oscResponder.forwardOSCMessage(getOrientation);
    cout << returnedMessageVector[0].getMessageAsString() << endl;
  }

  return 0;
}
