//
//  sdConstant.h
//

#ifndef ____sdConstant__
#define ____sdConstant__

#include <string>
#include <sstream>
using namespace std;
/*!
    enum for descriptor. internally all descriptors are handled with this Enum
 */
typedef enum {
    SD_ERROR,
    
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
    SD_MEDIA_GAIN
    
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

inline string doubleToString(double db){
    ostringstream timeStringStream;
    timeStringStream << time;
    return timeStringStream.str();
}

inline double stringToDouble(string str){
    double output;
    istringstream is;
    is.str(str);
    is >> output;
    return output;
}


#endif