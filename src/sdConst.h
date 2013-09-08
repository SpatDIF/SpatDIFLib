//
//  sdConstant.h
//

#ifndef ____sdConstant__
#define ____sdConstant__

#include <string>
#include <iostream>
#include <sstream>
using namespace std;
/*!
    enum for descriptor. internally all descriptors are handled with this Enum
 */
typedef enum {
    SD_TYPE ,
    SD_PRESENT,
    SD_POSITION,
    SD_ORIENTATION,
    
    //descriptor for media extension
    SD_MEDIA_ID,
    SD_MEDIA_TYPE,
    SD_MEDIA_LOCATION,
    SD_MEDIA_CHANNEL,
    SD_MEDIA_TIME_OFFSET,
    SD_MEDIA_GAIN,
    
    SD_ERROR
} EDescriptor;

/*!
 enum for "type" descriptor.
 Currently only "point" is declared in the specification.
 */
typedef enum{
    SD_POINT
} EType;

/*!
 enum for ordering
 */
typedef enum {
    SD_TIME,
    SD_TRACK
} EOrdering;

/*!
 enum for kind. All sdEntityCore must define their kind when instantiated.
 */
typedef enum {
    SD_SOURCE,
    SD_SINK
} EKind;

/*!
 enum for extension. all sdEntityExtension must have one of these enum as a static variable in order to identify themselves
 */
typedef enum {
    SD_MEDIA,
    SD_INTERPOLATION,
    SD_DIRECT_TO_ONE
} EExtension;





inline string doublesToString(double *db, int num){
    ostringstream os;
    for(int i=0;i<num;i++){
        os << db[i];
        if(i != (num-1))
            os << ' ';
    }
    return os.str();
}

inline string doubleToString(double db){
    return doublesToString(&db, 1);
}

inline double *stringToDoubles(string str, double *db, int num){
    istringstream is(str);
    for(int i = 0; i < num; i++) {
        is >> db[i];
    }
    return db;
}

inline double stringToDouble(string str){
    double db;
    stringToDoubles(str, &db, 1);
    return db;
}

inline int *stringToInts(string str, int *it, int num){
    istringstream is(str);
    for(int i = 0; i < num; i++) {
        is >> it[i];
    }
    return it;
}

inline int stringToInt(string str){
    int it;
    stringToInts(str, &it, 1);
    return it;
}

#endif