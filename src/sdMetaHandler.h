/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief an abstract class of spat DIF event and its template subclass
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */


#ifndef ____sdMetaHandler__
#define ____sdMetaHandler__


/*!
 this class provides functionalities to add, find, delete meta data (descriptor - value pair without time)
 */
class sdMetaHandler{
    
protected:
    std::vector< std::shared_ptr<sdProtoMeta> > metas; /*!< maintains pointers to all relevant sdMetas */
    /*! find meta with the descriptor */
    std::vector<std::shared_ptr<sdProtoMeta>>::const_iterator findMeta(const EDescriptor &descriptor) const;

    /*! add a pointer a meta to the global event vector */
    virtual void addGlobalMetaAlias(std::shared_ptr<sdProtoMeta> meta) = 0;


public:
    /*! add meta data with the specified descriptor and value to the entity
     @param value value to be added
     @returns a pointer to meta data
     */
    template <EDescriptor D>
    sdMeta<D> * const addMeta(typename sdDescriptor<D>::type value, sdEntity *entity);
    
    
    /*! this function is the only way to instantiate sdMeta.*/
    template <EDescriptor D>
    std::shared_ptr<sdProtoMeta> addProtoMeta(typename sdDescriptor<D>::type value, sdEntity *entity);

    /*! return number of registerd events in the events
     @returns number of meta data defined for this entities
     */
    size_t getNumberOfMetas() const;
    
    /*! returns metadata of specified descriptor
     @returns metadata
     */
    template <EDescriptor D>
    const sdMeta<D> * const getMeta() const;
    
    /*! remove an event from the meta vector
     @param event the event to be removed
     */
    bool removeMeta(const std::shared_ptr<sdProtoMeta> &meta);

    /*! remove an meta from the meta vector
     @param a pointer to a raw proto event to be removed.
     */
    bool removeMeta(const sdProtoMeta * const meta);
    
    /*! remove an event with the specified descriptor
     @param descriptor the descriptor of sdEvent to be removed */
    bool removeMeta(const EDescriptor &descriptor);
    
    /*! remove all events in the events */
    void removeAllMetas();
    
};

template <EDescriptor D>
inline sdMeta<D> * const sdMetaHandler::addMeta(typename sdDescriptor<D>::type value, sdEntity *entity){
    return dynamic_cast<sdMeta<D>*>(addProtoMeta<D>(value, entity).get());
}

template <EDescriptor D>
inline std::shared_ptr<sdProtoMeta> sdMetaHandler::addProtoMeta(typename sdDescriptor<D>::type value, sdEntity *entity){
    
    // remove if already exist
    removeMeta(D);
    
    // add
    auto meta = std::shared_ptr<sdProtoMeta>(new sdMeta<D>(entity, value));
    metas.push_back(meta);
    addGlobalMetaAlias(meta);
    return meta;
}

inline size_t sdMetaHandler::getNumberOfMetas() const{
    return metas.size();
}

template <EDescriptor D>
inline const sdMeta<D> * const sdMetaHandler::getMeta() const{
    auto it = findMeta(D);
    if(it == metas.end()){return nullptr;}
    return dynamic_cast<const sdMeta<D> *>((*it).get());
}

inline std::vector<std::shared_ptr<sdProtoMeta>>::const_iterator sdMetaHandler::findMeta(const EDescriptor &descriptor) const{
    for (auto it = metas.begin(); it != metas.end(); it++) {
        if( (descriptor == (*it)->getDescriptor()) ){ return it; }
    }
    return metas.end();
}

inline bool sdMetaHandler::removeMeta(const sdProtoMeta * const meta){
    for (auto it = metas.begin(); it != metas.end(); it++) {
        if((*it).get() == meta){
            metas.erase(it);
            return true;
        }
    }
    return false;
}

inline bool sdMetaHandler::removeMeta(const std::shared_ptr<sdProtoMeta> &meta){
    return removeMeta(dynamic_cast<sdProtoMeta*>(meta.get()));
}

inline bool sdMetaHandler::removeMeta(const EDescriptor &descriptor){
    auto it = findMeta(descriptor);
    if(it == metas.end()) return false;
    metas.erase(it);
    return true;
}

inline void sdMetaHandler::removeAllMetas(){
    metas.clear();
}

#endif 
