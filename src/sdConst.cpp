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

#include "sdConst.h"
#include "sdEntity.h"

const std::vector<sdExtension::sdESpec> sdExtension::spatDIFSpec= {
    sdESpec(EExtension::SD_CORE, "core", {
        sdDSpec(SD_TYPE, "type", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_TYPE>(time, stringTo<EType>(value));}),
        sdDSpec(SD_PRESENT, "present", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_PRESENT>(time, stringTo<bool>(value));}),
        sdDSpec(SD_POSITION, "position", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_POSITION>(time,stringToArray<double, 3>(value) );}),
        sdDSpec(SD_ORIENTATION, "orientation", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_ORIENTATION>(time, stringToArray<double, 3>(value) );})
    }),
    sdESpec(EExtension::SD_MEDIA, "media", {
        sdDSpec(SD_MEDIA_ID, "id", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_ID>(time, value);}),
        sdDSpec(SD_MEDIA_TYPE, "type", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_TYPE>(time, value);}),
        sdDSpec(SD_MEDIA_LOCATION, "location", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_LOCATION>(time, value);}),
        sdDSpec(SD_MEDIA_CHANNEL, "channel", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_CHANNEL>(time, std::stoi(value));}),
        sdDSpec(SD_MEDIA_GAIN, "gain", [](sdEntity * entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_GAIN>(time, std::stod(value));}),
        sdDSpec(SD_MEDIA_TIME_OFFSET, "time-offset", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_MEDIA_TIME_OFFSET>(time, std::stod(value));})
    }),
    sdESpec((EExtension::SD_SOURCE_WIDTH), "source-width",{
        sdDSpec(SD_SOURCE_WIDTH_WIDTH, "width", [](sdEntity* entity, double time, std::string value){
            return entity->addProtoEvent<SD_SOURCE_WIDTH_WIDTH>(time, std::stod(value));
        ;})
    })

};
            
