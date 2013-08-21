//
//  sdOSCResponder.h
//

#ifndef ____sdOSCResponder__
#define ____sdOSCResponder__



class sdOSCResponder{
    
private:
    std::string destIP;
    unsigned int destPort;
    unsigned int port;

public:
    void begin();
    void end();
    
};

#endif /* defined(____sdOSCResponder__) */
