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

#include "sdDescriptorSpec.h"
#include "sdEntity.h"
#include "sdDataSetHandler.h"


#pragma mark functor against runtime query and manipulation using string


// the following table defines the relationship between extensions and descriptors, descriptor enum and string

template<EDescriptor D>
inline std::function<void(sdDataSetHandler*, std::string, std::string)> getAddDataFunc(){
    return [](sdDataSetHandler * handler, std::string identifier, std::string valueString){
        EExtension ext = sdSpec::getExtensionOfDescriptor(D);
        std::shared_ptr<sdProtoDataSet> dataset = handler->getProtoDataSet(ext, identifier);
        dataset->setValue<D>(sdDescriptorSpec<D>::stringTo(valueString));
    };
}
    
template<EDescriptor D>
inline std::function<std::shared_ptr<sdProtoMeta>(sdEntity* , std::string)> getAddMetaFunc(){
    return [](sdEntity * entity, std::string value)->std::shared_ptr<sdProtoMeta>{
        return entity->addProtoMeta<D>(sdDescriptorSpec<D>::stringTo(value), entity);
    };
}

template<EDescriptor D>
inline std::function<std::shared_ptr<sdProtoEvent> (sdEntity * , double , std::string ,bool)> getAddEventFunc(){
    return [](sdEntity * entity, double time, std::string value, bool autosort)->std::shared_ptr<sdProtoEvent>{
        return entity->addProtoEvent<D>(time, sdDescriptorSpec<D>::stringTo(value), entity, autosort);
    };
}

template<EDescriptor D>
inline std::function<std::string(std::shared_ptr<sdProtoHolder>)> getGetDataAsStringFunc(){
    return [](std::shared_ptr<sdProtoHolder> protoHolder)->std::string{
        auto holder = static_cast<sdHolder<typename sdDescriptorSpec<D>::type> *>(protoHolder.get());
        typename sdDescriptorSpec<D>::type value = (*holder).item;
        return sdDescriptorSpec<D>::toString(value);
    };
}

template<EDescriptor D>
inline sdSpec::sdDescriptorRelationships defineDescriptor(const std::string &string){
    return sdSpec:: sdDescriptorRelationships(D, string, getAddDataFunc<D>(), getAddMetaFunc<D>(), getAddEventFunc<D>(), getGetDataAsStringFunc<D>());
}

#pragma mark special case

// these two descriptors will be stored in one vector 
template<>
inline sdSpec::sdDescriptorRelationships defineDescriptor<SD_POINTSET_POINT>(const std::string &string){
    return sdSpec::sdDescriptorRelationships(SD_POINTSET_POINT, string, getAddDataFunc<SD_POINTSET_POINT_OR_HANDLE>(), getAddMetaFunc<SD_POINTSET_POINT_OR_HANDLE>(), getAddEventFunc<SD_POINTSET_POINT_OR_HANDLE>(), getGetDataAsStringFunc<SD_POINTSET_POINT_OR_HANDLE>());
}

template<>
inline sdSpec::sdDescriptorRelationships defineDescriptor<SD_POINTSET_HANDLE>(const std::string &string){
    return sdSpec::sdDescriptorRelationships(SD_POINTSET_HANDLE, string, getAddDataFunc<SD_POINTSET_POINT_OR_HANDLE>(), getAddMetaFunc<SD_POINTSET_POINT_OR_HANDLE>(), getAddEventFunc<SD_POINTSET_POINT_OR_HANDLE>(), getGetDataAsStringFunc<SD_POINTSET_POINT_OR_HANDLE>());
}



template<EDescriptor D>
inline std::function<std::shared_ptr<sdProtoEvent> (sdEntity * , double , std::string , bool)> getAddNoEventFunc(){
    return [](sdEntity * entity, double time, std::string value,bool)->std::shared_ptr<sdProtoEvent>{
        throw MetaOnlyDescriptorException(" ");
        return entity->addProtoEvent<D>(time, sdDescriptorSpec<D>::stringTo(value), entity);
    };
}

template<EDescriptor D>
inline sdSpec::sdDescriptorRelationships defineDescriptorNoEvent(const std::string &string){
    return sdSpec::sdDescriptorRelationships(D, string, getAddDataFunc<D>(), getAddMetaFunc<D>(), getAddNoEventFunc<D>(), getGetDataAsStringFunc<D>());
}

//! the descriptors which can be used without activation of extensions
const std::unordered_set<EExtension> sdSpec::coreSpec= {
    EExtension::SD_INFO,
    EExtension::SD_CORE,
    EExtension::SD_LOOP,
    EExtension::SD_INTERPOLATION,
    EExtension::SD_MEDIA
};


const std::vector<sdSpec::sdExtensionRelationships> sdSpec::spatDIFSpec= {
    sdExtensionRelationships(EExtension::SD_INFO, "info", {
        defineDescriptorNoEvent<SD_INFO_AUTHOR>("author"),
        defineDescriptorNoEvent<SD_INFO_ANNOTATION>("annotation"),
        defineDescriptorNoEvent<SD_INFO_DATE>("date"),
        defineDescriptorNoEvent<SD_INFO_DURATION>("duration"),
        defineDescriptorNoEvent<SD_INFO_HOST>("host"),
        defineDescriptorNoEvent<SD_INFO_LOCATION>("location"),
        defineDescriptorNoEvent<SD_INFO_TITLE>("title"),
        defineDescriptorNoEvent<SD_INFO_SESSION>("session")
    }, SD_INFO_TITLE), // dummy
    sdExtensionRelationships(EExtension::SD_CORE, "core", {
        defineDescriptor<SD_TYPE>("type"),
        defineDescriptor<SD_PRESENT>("present"),
        defineDescriptor<SD_POSITION>("position"),
        defineDescriptor<SD_ORIENTATION>("orientation")
    }),
    sdExtensionRelationships(EExtension::SD_MEDIA, "media", {
        defineDescriptor<SD_MEDIA_ID>("id"),
        defineDescriptor<SD_MEDIA_TYPE>("type"),
        defineDescriptor<SD_MEDIA_LOCATION>( "location"),
        defineDescriptor<SD_MEDIA_CHANNEL>( "channel"),
        defineDescriptor<SD_MEDIA_GAIN>( "gain"),
        defineDescriptor<SD_MEDIA_TIME_OFFSET>( "time-offset")
    }, SD_MEDIA_ID),
    sdExtensionRelationships(EExtension::SD_LOOP, "loop",{
        defineDescriptor<SD_LOOP_TYPE>("type"),
        defineDescriptor<SD_LOOP_POINTS>("points"),
        defineDescriptor<SD_LOOP_WAIT_TIME>("wait-time")
    }),
    sdExtensionRelationships((EExtension::SD_INTERPOLATION), "interpolation",{
        defineDescriptor<SD_INTERPOLATION_TYPE>("type")
    }),
    // 5.2.1 Pointset
    sdExtensionRelationships((EExtension::SD_POINTSET), "pointset",{
        defineDescriptor<SD_POINTSET_ID>("id"),
        defineDescriptor<SD_POINTSET_CLOSED>("closed"),
        defineDescriptor<SD_POINTSET_POINT>("point"),
        defineDescriptor<SD_POINTSET_HANDLE>("handle"),
        defineDescriptor<SD_POINTSET_POINT_OR_HANDLE>("point-or-handle") // internally used descriptor for storing mixed variable data
    }, SD_POINTSET_ID),
    // 5.2.2 Geometry
    sdExtensionRelationships((EExtension::SD_GEOMETRY), "geometry",{
        defineDescriptor<SD_GEOMETRY_TRANSLATE>("translate"),
        defineDescriptor<SD_GEOMETRY_ROTATE>("rotate"),
        defineDescriptor<SD_GEOMETRY_SCALE>("scale"),
        defineDescriptor<SD_GEOMETRY_SHEAR>("shear"),
        defineDescriptor<SD_GEOMETRY_REFLECT>("reflect")
    }),
    // 5.2.3 Automation
    sdExtensionRelationships((EExtension::SD_AUTOMATION), "automation", {
        defineDescriptor<SD_AUTOMATION_DURATION>( "duration"),
        defineDescriptor<SD_AUTOMATION_DELAY>( "delay"),
        defineDescriptor<SD_AUTOMATION_FUNCTION>( "function")
        //defineDescriptor<SD_AUTOMATION_POINTSET, "pointset"),
        //defineDescriptor<SD_AUTOMATION_LOOP, "loop")
    }),
    // 5.2.4 Shape
    sdExtensionRelationships((EExtension::SD_SHAPE), "shape", {
        defineDescriptor<SD_SHAPE_ID>("id"),
        defineDescriptor<SD_SHAPE_DIRECTION>("direction"),
        defineDescriptor<SD_SHAPE_CLOSED>("closed"),
        defineDescriptor<SD_SHAPE_TYPE>("type")
    }, SD_SHAPE_ID),
    // 5.3.1.1 Trajectory Generator
    sdExtensionRelationships((EExtension::SD_TRAJECTORY), "trajectory" , {
        defineDescriptor<SD_TRAJECTORY_POINTSET>("pointset"),
        defineDescriptor<SD_TRAJECTORY_INTERPOLATION>("interpolation"),
        defineDescriptor<SD_TRAJECTORY_GEOMETRY>("geometry" ),
        defineDescriptor<SD_TRAJECTORY_SHAPE>("shape"),
        defineDescriptor<SD_TRAJECTORY_AUTOMATION>("automation")
    }),
    // 5.3.1.2 Group
    sdExtensionRelationships((EExtension::SD_GROUP), "group",{
        defineDescriptor<SD_GROUP_ID>("id"),
        defineDescriptor<SD_GROUP_PRESENT>("present"),
        defineDescriptor<SD_GROUP_POSITION>("position"),
        defineDescriptor<SD_GROUP_ORIENTATION>("orientation"),
        defineDescriptor<SD_GROUP_MEMBERSHIP>("group-membership")
    }, SD_GROUP_ID),
    // 5.3.2.1 Source spread
    sdExtensionRelationships((EExtension::SD_SOURCE_SPREAD), "source-spread",{
        defineDescriptor<SD_SOURCE_SPREAD_SPREAD>( "spread"),
    }),
    // 5.3.3.1 Diestance cue
    sdExtensionRelationships((EExtension::SD_DISTANCE_CUES), "distance-cues",{
        defineDescriptor<SD_DISTANCE_CUES_REFERENCE_DISTANCE>("reference-distance"),
        defineDescriptor<SD_DISTANCE_CUES_MAXIMUM_DISTANCE>("maximum-distance"),
        defineDescriptor<SD_DISTANCE_CUES_MAXIMUM_ATTENUATION>("maximum-attenuation"),
        defineDescriptor<SD_DISTANCE_CUES_ATTENUATION_MODEL>("attenuation-model"),
        defineDescriptor<SD_DISTANCE_CUES_ABSORPTION_MODEL>("absorption-model")
    }),
    // 5.3.4.1 Sink Entity
    sdExtensionRelationships((EExtension::SD_SINK_ENTITY),"sink-entity",{
       //descriptor is identical to source
    }),
    // 5.3.4.2 Direct-to-One sink
    sdExtensionRelationships((EExtension::SD_DIRECT_TO_ONE), "direct-to-one",{
        defineDescriptor<SD_DIRECT_TO_ONE_DIRECT_TO_ONE>("direct-to-one")
    }),
    // 5.3.5.1 Hardware-out
    sdExtensionRelationships((EExtension::SD_HARDWARE_OUT), "hardware-out",{
        defineDescriptor<SD_HARDWARE_OUT_PHYSICAL_CHANNEL>( "physical-channel"),
        defineDescriptor<SD_HARDWARE_OUT_GAIN>("gain")
    }),
    // 5.4 Private Extension
    sdExtensionRelationships((EExtension::SD_PRIVATE), "private", {}) // no descriptor defined
};

