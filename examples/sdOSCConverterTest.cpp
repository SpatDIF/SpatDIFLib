#include "sdOSCConverter.h"
#include <string>
#include <iostream>

int main(void){
  sdOSCConverter converter;
  cout << converter.floatToOSC(5.678) << endl;
  
  return 0;
}
