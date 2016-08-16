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

#include "sdDescriptors.h"
#include "sdEntity.h"

// the following table defines the relationship between extensions and descriptors 

const std::vector<sdExtension::sdESpec> sdExtension::spatDIFSpec= {
    sdESpec(EExtension::SD_CORE, "core", {
        sdDSpec(SD_TYPE, "type", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_TYPE>(time, sdDescriptor<SD_TYPE>::stringTo(value), entity);}),
        sdDSpec(SD_PRESENT, "present", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_PRESENT>(time, sdDescriptor<SD_PRESENT>::stringTo(value), entity);}),
        sdDSpec(SD_POSITION, "position", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_POSITION>(time,sdDescriptor<SD_POSITION>::stringTo(value), entity);}),
        sdDSpec(SD_ORIENTATION, "orientation", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_ORIENTATION>(time, sdDescriptor<SD_ORIENTATION>::stringTo(value), entity);}),
        sdDSpec(SD_GROUP_MEMBERSHIP, "group-membership", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_GROUP_MEMBERSHIP>(time, sdDescriptor<SD_GROUP_MEMBERSHIP>::stringTo(value), entity);})
    }),
    sdESpec(EExtension::SD_MEDIA, "media", {
        sdDSpec(SD_MEDIA_ID, "id", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_ID>(time, sdDescriptor<SD_MEDIA_ID>::stringTo(value), entity);}),
        sdDSpec(SD_MEDIA_TYPE, "type", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_TYPE>(time, sdDescriptor<SD_MEDIA_TYPE>::stringTo(value), entity);}),
        sdDSpec(SD_MEDIA_LOCATION, "location", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_LOCATION>(time, sdDescriptor<SD_MEDIA_LOCATION>::stringTo(value), entity);}),
        sdDSpec(SD_MEDIA_CHANNEL, "channel", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_CHANNEL>(time, sdDescriptor<SD_MEDIA_CHANNEL>::stringTo(value), entity);}),
        sdDSpec(SD_MEDIA_GAIN, "gain", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_GAIN>(time, sdDescriptor<SD_MEDIA_GAIN>::stringTo(value), entity);}),
        sdDSpec(SD_MEDIA_TIME_OFFSET, "time-offset", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_TIME_OFFSET>(time, sdDescriptor<SD_MEDIA_TIME_OFFSET>::stringTo(value), entity);})
    }),
    sdESpec(EExtension::SD_LOOP, "loop",{
        sdDSpec(SD_LOOP_TYPE, "type", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_LOOP_TYPE>(time, sdDescriptor<SD_LOOP_TYPE>::stringTo(value), entity);}),
        sdDSpec(SD_LOOP_POINTS, "points", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_LOOP_POINTS>(time, sdDescriptor<SD_LOOP_POINTS>::stringTo(value), entity);}),
        sdDSpec(SD_LOOP_WAIT_TIME, "wait-time", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_LOOP_WAIT_TIME>(time, sdDescriptor<SD_LOOP_WAIT_TIME>::stringTo(value), entity);})
    }),
    sdESpec((EExtension::SD_INTERPOLATION), "interpolation",{
        sdDSpec(SD_INTERPOLATION_TYPE, "type", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_INTERPOLATION_TYPE>(time, sdDescriptor<SD_INTERPOLATION_TYPE>::stringTo(value), entity);})
    }),
    sdESpec((EExtension::SD_POINTSET), "pointset",{
        sdDSpec(SD_POINTSET_ID, "id", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_POINTSET_ID>(time, sdDescriptor<SD_POINTSET_ID>::stringTo(value), entity);}),
        sdDSpec(SD_POINTSET_UNIT, "unit", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_POINTSET_UNIT>(time, sdDescriptor<SD_POINTSET_UNIT>::stringTo(value), entity);}),
        sdDSpec(SD_POINTSET_CLOSED, "closed", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_POINTSET_CLOSED>(time, sdDescriptor<SD_POINTSET_CLOSED>::stringTo(value), entity);}),
        sdDSpec(SD_POINTSET_SIZE, "size", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_POINTSET_SIZE>(time, sdDescriptor<SD_POINTSET_SIZE>::stringTo(value), entity);}),
        sdDSpec(SD_POINTSET_POINT, "point", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_POINTSET_POINT>(time, sdDescriptor<SD_POINTSET_POINT>::stringTo(value), entity);}),
        sdDSpec(SD_POINTSET_HANDLE, "handle", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_POINTSET_HANDLE>(time, sdDescriptor<SD_POINTSET_HANDLE>::stringTo(value), entity);})
    }),
    sdESpec((EExtension::SD_SOURCE_SPREAD), "source-spread",{
        sdDSpec(SD_SOURCE_SPREAD_SPREAD, "spread", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_SOURCE_SPREAD_SPREAD>(time, sdDescriptor<SD_SOURCE_SPREAD_SPREAD>::stringTo(value), entity);})
    }),
    sdESpec((EExtension::SD_SINK_ENTITY),"sink-entity",{
       //descriptor is identical to source
    }),
    sdESpec((EExtension::SD_HARDWARE_OUT), "hardware-out",{
        sdDSpec(SD_HARDWARE_OUT_PHYSICAL_CHANNEL, "physical-channel", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(time, sdDescriptor<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>::stringTo(value), entity);}),
        sdDSpec(SD_HARDWARE_OUT_GAIN, "gain", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_HARDWARE_OUT_GAIN>(time, sdDescriptor<SD_HARDWARE_OUT_GAIN>::stringTo(value), entity);}),
    })

};
            
