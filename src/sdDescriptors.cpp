/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#define ADD_EVENT( descriptor ) [](sdEntity * entity, double time, std::string value){ return entity->addProtoEvent<descriptor>(time, sdDescriptor<descriptor>::stringTo(value), entity);}

#define ADD_META( descriptor ) [](sdEntity * entity, std::string value){ return entity->addProtoMeta<descriptor>(sdDescriptor<descriptor>::stringTo(value), entity);}

#define DEFINE_DESCRIPTOR( descriptor, string ) sdDSpec(descriptor, string, ADD_EVENT(descriptor), ADD_META(descriptor))

#include "sdDescriptors.h"
#include "sdEntity.h"

// the following table defines the relationship between extensions and descriptors 

const std::vector<sdExtension::sdESpec> sdExtension::spatDIFSpec= {
    sdESpec(EExtension::SD_CORE, "core", {
        DEFINE_DESCRIPTOR(SD_TYPE,"type"),
        DEFINE_DESCRIPTOR(SD_PRESENT,"present"),
        DEFINE_DESCRIPTOR(SD_POSITION, "position"),
        DEFINE_DESCRIPTOR(SD_ORIENTATION, "orientation"),
        DEFINE_DESCRIPTOR(SD_GROUP_MEMBERSHIP, "group-membership")
    }),
    sdESpec(EExtension::SD_MEDIA, "media", {
        DEFINE_DESCRIPTOR(SD_MEDIA_ID, "id"),
        DEFINE_DESCRIPTOR(SD_MEDIA_TYPE, "type"),
        DEFINE_DESCRIPTOR(SD_MEDIA_LOCATION, "location"),
        DEFINE_DESCRIPTOR(SD_MEDIA_CHANNEL, "channel"),
        DEFINE_DESCRIPTOR(SD_MEDIA_GAIN, "gain"),
        DEFINE_DESCRIPTOR(SD_MEDIA_TIME_OFFSET, "time-offset")
    }),
    sdESpec(EExtension::SD_LOOP, "loop",{
        DEFINE_DESCRIPTOR(SD_LOOP_TYPE, "type"),
        DEFINE_DESCRIPTOR(SD_LOOP_POINTS, "points"),
        DEFINE_DESCRIPTOR(SD_LOOP_WAIT_TIME, "wait-time")
    }),
    sdESpec((EExtension::SD_INTERPOLATION), "interpolation",{
        DEFINE_DESCRIPTOR(SD_INTERPOLATION_TYPE, "type")
    }),
    sdESpec((EExtension::SD_POINTSET), "pointset",{
        DEFINE_DESCRIPTOR(SD_POINTSET_ID, "id"),
        DEFINE_DESCRIPTOR(SD_POINTSET_UNIT, "unit"),
        DEFINE_DESCRIPTOR(SD_POINTSET_CLOSED, "closed"),
        DEFINE_DESCRIPTOR(SD_POINTSET_SIZE, "size"),
        DEFINE_DESCRIPTOR(SD_POINTSET_POINT, "point"),
        DEFINE_DESCRIPTOR(SD_POINTSET_HANDLE, "handle")
    }),
    sdESpec((EExtension::SD_SOURCE_SPREAD), "source-spread",{
        DEFINE_DESCRIPTOR(SD_SOURCE_SPREAD_SPREAD, "spread")
    }),
    sdESpec((EExtension::SD_SINK_ENTITY),"sink-entity",{
       //descriptor is identical to source
    }),
    sdESpec((EExtension::SD_HARDWARE_OUT), "hardware-out",{
        DEFINE_DESCRIPTOR(SD_HARDWARE_OUT_PHYSICAL_CHANNEL, "physical-channel"),
        DEFINE_DESCRIPTOR(SD_HARDWARE_OUT_GAIN, "gain")
    })
};
            
