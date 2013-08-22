//
//  sdConstant.h
//

#ifndef ____sdConstant__
#define ____sdConstant__

#include <string>

using namespace std;
/*!
    enum for descriptor. internally all descriptors are indicated by this enum
 */
typedef enum {
    SD_TYPE = 1,
    SD_PRESENT = 2,
    SD_POSITION = 4,
    SD_ORIENTATION = 8
} EDescriptor;

/*!
 enum for "type" descriptor
 */
typedef enum{
    SD_POINT
} EType;

/*!
 enum for kind. all sdEntityCore must define their kind when instantiated
 */
typedef enum {
    SD_SOURCE,
    SD_SINK
} EKind;

/*!
 enum for extension. all sdEntityExtension hold one of these enum as a static variable in order to identify themselves
 */
typedef enum {
    SD_MEDIA
} EExtension;

#endif