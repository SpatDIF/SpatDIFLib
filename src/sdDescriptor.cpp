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

#define ADD_EVENT( descriptor ) [](sdProtoEntity * entity, double time, std::string value){ return entity->addProtoEvent<descriptor>(time, sdDescriptor<descriptor>::stringTo(value), entity);}

#define ADD_META( descriptor ) [](sdProtoEntity * entity, std::string value){ return entity->addProtoMeta<descriptor>(sdDescriptor<descriptor>::stringTo(value), entity);}

#define DEFINE_DESCRIPTOR( descriptor, string ) sdDescriptorSpec(descriptor, string, ADD_EVENT(descriptor), ADD_META(descriptor))


#define ADD_NO_EVENT( descriptor ) [](sdProtoEntity * entity, double time, std::string value){ \
    throw MetaOnlyDescriptorException(" "); \
    return entity->addProtoEvent<descriptor>(time, sdDescriptor<descriptor>::stringTo(value), entity);\
}

#define DEFINE_DESCRIPTOR_NO_EVENT( descriptor, string ) sdDescriptorSpec(descriptor, string, ADD_NO_EVENT(descriptor), ADD_META(descriptor))


#include "sdDescriptor.h"
#include "sdEntity.h"

// the following table defines the relationship between extensions and descriptors, descriptor enum and string

const std::vector<sdSpec::sdExtensionSpec> sdSpec::spatDIFSpec= {
    sdExtensionSpec(EExtension::SD_INFO, "info", {
        DEFINE_DESCRIPTOR_NO_EVENT(SD_INFO_AUTHOR, "author"),
        DEFINE_DESCRIPTOR_NO_EVENT(SD_INFO_ANNOTATION, "annotation"),
        DEFINE_DESCRIPTOR_NO_EVENT(SD_INFO_DATE, "date"),
        DEFINE_DESCRIPTOR_NO_EVENT(SD_INFO_DURATION, "duration"),
        DEFINE_DESCRIPTOR_NO_EVENT(SD_INFO_HOST, "host"),
        DEFINE_DESCRIPTOR_NO_EVENT(SD_INFO_LOCATION, "location"),
        DEFINE_DESCRIPTOR_NO_EVENT(SD_INFO_TITLE, "title"),
        DEFINE_DESCRIPTOR_NO_EVENT(SD_INFO_SESSION, "session")
    }),
    sdExtensionSpec(EExtension::SD_CORE, "core", {
        DEFINE_DESCRIPTOR(SD_TYPE,"type"),
        DEFINE_DESCRIPTOR(SD_PRESENT,"present"),
        DEFINE_DESCRIPTOR(SD_POSITION, "position"),
        DEFINE_DESCRIPTOR(SD_ORIENTATION, "orientation"),
    }),
    sdExtensionSpec(EExtension::SD_MEDIA, "media", {
        DEFINE_DESCRIPTOR(SD_MEDIA_ID, "id"),
        DEFINE_DESCRIPTOR(SD_MEDIA_TYPE, "type"),
        DEFINE_DESCRIPTOR(SD_MEDIA_LOCATION, "location"),
        DEFINE_DESCRIPTOR(SD_MEDIA_CHANNEL, "channel"),
        DEFINE_DESCRIPTOR(SD_MEDIA_GAIN, "gain"),
        DEFINE_DESCRIPTOR(SD_MEDIA_TIME_OFFSET, "time-offset")
    },SD_MEDIA_ID),
    sdExtensionSpec(EExtension::SD_LOOP, "loop",{
        DEFINE_DESCRIPTOR(SD_LOOP_TYPE, "type"),
        DEFINE_DESCRIPTOR(SD_LOOP_POINTS, "points"),
        DEFINE_DESCRIPTOR(SD_LOOP_WAIT_TIME, "wait-time")
    }),
    sdExtensionSpec((EExtension::SD_INTERPOLATION), "interpolation",{
        DEFINE_DESCRIPTOR(SD_INTERPOLATION_TYPE, "type")
    }),
    // 5.2.1 Pointset
    sdExtensionSpec((EExtension::SD_POINTSET), "pointset",{
        DEFINE_DESCRIPTOR(SD_POINTSET_ID, "id"),
        DEFINE_DESCRIPTOR(SD_POINTSET_UNIT, "unit"),
        DEFINE_DESCRIPTOR(SD_POINTSET_CLOSED, "closed"),
        DEFINE_DESCRIPTOR(SD_POINTSET_SIZE, "size"),
        DEFINE_DESCRIPTOR(SD_POINTSET_POINT, "point"),
        DEFINE_DESCRIPTOR(SD_POINTSET_HANDLE, "handle")
    },SD_POINTSET_ID),
    // 5.2.2 Geometry
    sdExtensionSpec((EExtension::SD_GEOMETRY), "geometry",{
        DEFINE_DESCRIPTOR(SD_GEOMETRY_TRANSLATE, "translate"),
        DEFINE_DESCRIPTOR(SD_GEOMETRY_ROTATE, "rotate"),
        DEFINE_DESCRIPTOR(SD_GEOMETRY_SCALE, "scale"),
        DEFINE_DESCRIPTOR(SD_POINTSET_SIZE, "size"),
        DEFINE_DESCRIPTOR(SD_GEOMETRY_SHEAR, "shear"),
        DEFINE_DESCRIPTOR(SD_GEOMETRY_REFLECT , "reflect")
    }),
    // 5.2.3 Automation
    sdExtensionSpec((EExtension::SD_AUTOMATION), "automation", {
        DEFINE_DESCRIPTOR(SD_AUTOMATION_DURATION, "duration"),
        DEFINE_DESCRIPTOR(SD_AUTOMATION_DELAY, "delay"),
        DEFINE_DESCRIPTOR(SD_AUTOMATION_FUNCTION, "function")
        //DEFINE_DESCRIPTOR(SD_AUTOMATION_POINTSET, "pointset"),
        //DEFINE_DESCRIPTOR(SD_AUTOMATION_LOOP, "loop")
    }),
    // 5.2.4 Shape
    sdExtensionSpec((EExtension::SD_SHAPE), "shape", {
        DEFINE_DESCRIPTOR(SD_SHAPE_ID, "id"),
        DEFINE_DESCRIPTOR(SD_SHAPE_DIRECTION, "direction"),
        DEFINE_DESCRIPTOR(SD_SHAPE_CLOSED, "closed"),
        DEFINE_DESCRIPTOR(SD_SHAPE_TYPE, "type")
    },SD_SHAPE_ID),
    // 5.3.1.1 Trajectory Generator
    sdExtensionSpec((EExtension::SD_TRAJECTORY), "trajectory" , {
        DEFINE_DESCRIPTOR(SD_TRAJECTORY_POINTSET, "pointset"),
        DEFINE_DESCRIPTOR(SD_TRAJECTORY_INTERPOLATION, "interpolation"),
        DEFINE_DESCRIPTOR(SD_TRAJECTORY_GEOMETRY, "geometry" ),
        DEFINE_DESCRIPTOR(SD_TRAJECTORY_SHAPE, "shape"),
        DEFINE_DESCRIPTOR(SD_TRAJECTORY_AUTOMATION, "automation")
    }),
    // 5.3.1.2 Group
    sdExtensionSpec((EExtension::SD_GROUP), "group",{
        DEFINE_DESCRIPTOR(SD_GROUP_ID, "id"),
        DEFINE_DESCRIPTOR(SD_GROUP_PRESENT, "present"),
        DEFINE_DESCRIPTOR(SD_GROUP_POSITION, "position"),
        DEFINE_DESCRIPTOR(SD_GROUP_ORIENTATION, "orientation"),
        DEFINE_DESCRIPTOR(SD_GROUP_MEMBERSHIP, "group-membership")
    },SD_GROUP_ID),
    // 5.3.2.1 Source spread
    sdExtensionSpec((EExtension::SD_SOURCE_SPREAD), "source-spread",{
        DEFINE_DESCRIPTOR(SD_SOURCE_SPREAD_SPREAD, "spread"),
    }),
    // 5.3.3.1 Diestance cue
    sdExtensionSpec((EExtension::SD_DISTANCE_CUES), "distance-cues",{
        DEFINE_DESCRIPTOR(SD_DISTANCE_CUES_REFERENCE_DISTANCE, "reference-distance"),
        DEFINE_DESCRIPTOR(SD_DISTANCE_CUES_MAXIMUM_DISTANCE, "maximum-distance"),
        DEFINE_DESCRIPTOR(SD_DISTANCE_CUES_MAXIMUM_ATTENUATION, "maximum-attenuation"),
        DEFINE_DESCRIPTOR(SD_DISTANCE_CUES_ATTENUATION_MODEL, "attenuation-model"),
        DEFINE_DESCRIPTOR(SD_DISTANCE_CUES_ABSORPTION_MODEL, "absorption-model")
    }),
    // 5.3.4.1 Sink Entity
    sdExtensionSpec((EExtension::SD_SINK_ENTITY),"sink-entity",{
       //descriptor is identical to source
    }),
    // 5.3.4.2 Direct-to-One sink
    sdExtensionSpec((EExtension::SD_DIRECT_TO_ONE), "direct-to-one",{
        DEFINE_DESCRIPTOR(SD_DIRECT_TO_ONE_DIRECT_TO_ONE, "direct-to-one")
    }),
    // 5.3.5.1 Hardware-out
    sdExtensionSpec((EExtension::SD_HARDWARE_OUT), "hardware-out",{
        DEFINE_DESCRIPTOR(SD_HARDWARE_OUT_PHYSICAL_CHANNEL, "physical-channel"),
        DEFINE_DESCRIPTOR(SD_HARDWARE_OUT_GAIN, "gain")
    }),
    // 5.4 Private Extension
    sdExtensionSpec((EExtension::SD_PRIVATE), "private", {}) // no descriptor defined
};

