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

#include "sdDescriptor.h"
#include "sdEntity.h"

// the following table defines the relationship between extensions and descriptors, descriptor enum and string

const std::vector<sdExtension::sdESpec> sdExtension::spatDIFSpec= {
    sdESpec(EExtension::SD_CORE, "core", {
        DEFINE_DESCRIPTOR(SD_TYPE,"type"),
        DEFINE_DESCRIPTOR(SD_PRESENT,"present"),
        DEFINE_DESCRIPTOR(SD_POSITION, "position"),
        DEFINE_DESCRIPTOR(SD_ORIENTATION, "orientation"),
    }),
    sdESpec(EExtension::SD_MEDIA, "media", {
        DEFINE_DESCRIPTOR(SD_MEDIA_ID, "id"),
        DEFINE_DESCRIPTOR(SD_MEDIA_TYPE, "type"),
        DEFINE_DESCRIPTOR(SD_MEDIA_LOCATION, "location"),
        DEFINE_DESCRIPTOR(SD_MEDIA_CHANNEL, "channel"),
        DEFINE_DESCRIPTOR(SD_MEDIA_GAIN, "gain"),
        DEFINE_DESCRIPTOR(SD_MEDIA_TIME_OFFSET, "time-offset")
    },SD_MEDIA_ID),
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
    },SD_POINTSET_ID),
    // 5.2.3 Automation
    sdESpec((EExtension::SD_AUTOMATION), "automation", {
        DEFINE_DESCRIPTOR(SD_AUTOMATION_DURATION, "duration"),
        DEFINE_DESCRIPTOR(SD_AUTOMATION_DELAY, "delay"),
        DEFINE_DESCRIPTOR(SD_AUTOMATION_FUNCTION, "function")
        //DEFINE_DESCRIPTOR(SD_AUTOMATION_POINTSET, "pointset"),
        //DEFINE_DESCRIPTOR(SD_AUTOMATION_LOOP, "loop")
    }),
    // 5.3.1.1 Trajectory Generator
    sdESpec((EExtension::SD_TRAJECTORY), "trajectory" , {
        DEFINE_DESCRIPTOR(SD_TRAJECTORY_POINTSET, "pointset"),
        DEFINE_DESCRIPTOR(SD_TRAJECTORY_INTERPOLATION, "interpolation"),
        DEFINE_DESCRIPTOR(SD_TRAJECTORY_GEOMETRY, "geometry" ),
        DEFINE_DESCRIPTOR(SD_TRAJECTORY_SHAPE, "shape"),
        DEFINE_DESCRIPTOR(SD_TRAJECTORY_AUTOMATION, "automation")
    }),
    // 5.3.1.2 Group
    sdESpec((EExtension::SD_GROUP), "group",{
        DEFINE_DESCRIPTOR(SD_GROUP_ID, "id"),
        DEFINE_DESCRIPTOR(SD_GROUP_PRESENT, "present"),
        DEFINE_DESCRIPTOR(SD_GROUP_POSITION, "position"),
        DEFINE_DESCRIPTOR(SD_GROUP_ORIENTATION, "orientation"),
        DEFINE_DESCRIPTOR(SD_GROUP_MEMBERSHIP, "group-membership")
    }),
    // 5.3.2.1 Source spread
    sdESpec((EExtension::SD_SOURCE_SPREAD), "source-spread",{
        DEFINE_DESCRIPTOR(SD_SOURCE_SPREAD_SPREAD, "spread"),
    }),
    // 5.3.3.1 Diestance cue
    sdESpec((EExtension::SD_DISTANCE_CUES), "distance-cues",{
        DEFINE_DESCRIPTOR(SD_DISTANCE_CUES_REFERENCE_DISTANCE, "reference-distance"),
        DEFINE_DESCRIPTOR(SD_DISTANCE_CUES_MAXIMUM_DISTANCE, "maximum-distance"),
        DEFINE_DESCRIPTOR(SD_DISTANCE_CUES_MAXIMUM_ATTENUATION, "maximum-attenuation"),
        DEFINE_DESCRIPTOR(SD_DISTANCE_CUES_ATTENUATION_MODEL, "attenuation-model"),
        DEFINE_DESCRIPTOR(SD_DISTANCE_CUES_ABSORPTION_MODEL, "absorption-model")
    }),
    // 5.3.4.1 Sink Entity
    sdESpec((EExtension::SD_SINK_ENTITY),"sink-entity",{
       //descriptor is identical to source
    }),
    // 5.3.4.2 Direct-to-One sink
    sdESpec((EExtension::SD_DIRECT_TO_ONE), "direct-to-one",{
        DEFINE_DESCRIPTOR(SD_DIRECT_TO_ONE_DIRECT_TO_ONE, "direct-to-one")
    }),
    // 5.3.5.1 Hardware-out
    sdESpec((EExtension::SD_HARDWARE_OUT), "hardware-out",{
        DEFINE_DESCRIPTOR(SD_HARDWARE_OUT_PHYSICAL_CHANNEL, "physical-channel"),
        DEFINE_DESCRIPTOR(SD_HARDWARE_OUT_GAIN, "gain")
    }),
    // 5.4 Private Extension
    sdESpec((EExtension::SD_PRIVATE), "private", {}) // no descriptor defined
};

