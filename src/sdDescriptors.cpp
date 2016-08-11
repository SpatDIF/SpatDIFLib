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
            return entity->addProtoEvent<SD_TYPE>(time, stringTo<EType>(value), entity);}),
        sdDSpec(SD_PRESENT, "present", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_PRESENT>(time, stringTo<bool>(value), entity);}),
        sdDSpec(SD_POSITION, "position", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_POSITION>(time,stringToArray<double, 3>(value), entity);}),
        sdDSpec(SD_ORIENTATION, "orientation", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_ORIENTATION>(time, stringToArray<double, 3>(value), entity);}),
        sdDSpec(SD_GROUP_MEMBERSHIP, "group-membership", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_GROUP_MEMBERSHIP>(time, value, entity);})
    }),
    sdESpec(EExtension::SD_MEDIA, "media", {
        sdDSpec(SD_MEDIA_ID, "id", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_ID>(time, value, entity);}),
        sdDSpec(SD_MEDIA_TYPE, "type", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_TYPE>(time, value, entity);}),
        sdDSpec(SD_MEDIA_LOCATION, "location", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_LOCATION>(time, value, entity);}),
        sdDSpec(SD_MEDIA_CHANNEL, "channel", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_CHANNEL>(time, std::stoi(value), entity);}),
        sdDSpec(SD_MEDIA_GAIN, "gain", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_GAIN>(time, std::stod(value), entity);}),
        sdDSpec(SD_MEDIA_TIME_OFFSET, "time-offset", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_TIME_OFFSET>(time, std::stod(value), entity);})
    }),
    sdESpec((EExtension::SD_INTERPOLATION), "interpolation",{
        sdDSpec(SD_INTERPOLATION_TYPE, "type", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_INTERPOLATION_TYPE>(time, value, entity);})
    }),
    sdESpec((EExtension::SD_POINTSET), "pointset",{
        sdDSpec(SD_POINTSET_ID, "id", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_POINTSET_ID>(time, value, entity);}),
        sdDSpec(SD_POINTSET_UNIT, "unit", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_POINTSET_UNIT>(time, value, entity);}),
        sdDSpec(SD_POINTSET_CLOSED, "closed", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_POINTSET_CLOSED>(time, stringTo<bool>(value) , entity);}),
        sdDSpec(SD_POINTSET_SIZE, "size", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_POINTSET_SIZE>(time, std::stoi(value), entity);}),
        sdDSpec(SD_POINTSET_POINT, "point", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_POINTSET_POINT>(time, stringToArray<double, 3>(value), entity);}),
        sdDSpec(SD_POINTSET_HANDLE, "handle", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_POINTSET_HANDLE>(time, stringToArray<double, 3>(value), entity);})
    }),
    sdESpec((EExtension::SD_SOURCE_SPREAD), "source-spread",{
        sdDSpec(SD_SOURCE_SPREAD_SPREAD, "spread", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_SOURCE_SPREAD_SPREAD>(time, std::stod(value), entity);})
    }),
    sdESpec((EExtension::SD_SINK_ENTITY),"sink-entity",{
       //descriptor is identical to source
    }),
    sdESpec((EExtension::SD_HARDWARE_OUT), "hardware-out",{
        sdDSpec(SD_HARDWARE_OUT_PHYSICAL_CHANNEL, "physical-channel", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>(time, std::stoi(value), entity);}),
        sdDSpec(SD_HARDWARE_OUT_GAIN, "gain", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_HARDWARE_OUT_GAIN>(time, std::stod(value), entity);}),
    })

};
            
