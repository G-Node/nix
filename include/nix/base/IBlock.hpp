// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_BLOCK_H
#define NIX_I_BLOCK_H

#include <nix/base/IEntityWithMetadata.hpp>
#include <nix/base/ISource.hpp>
#include <nix/base/IDataArray.hpp>
#include <nix/base/IDataFrame.hpp>
#include <nix/base/ITag.hpp>
#include <nix/base/IMultiTag.hpp>
#include <nix/base/IGroup.hpp>
#include <nix/Compression.hpp>
#include <nix/NDSize.hpp>
#include <nix/Identity.hpp>

#include <string>
#include <vector>
#include <memory>

namespace nix {
namespace base {

/**
 * @brief Interface for implementations of the Block entity.
 *
 * See {@link nix::Block} for a more detailed description.
 */
class NIXAPI IBlock : virtual public base::IEntityWithMetadata {

public:

    virtual std::string resolveEntityId(const nix::Identity &ident) const = 0;

    virtual bool hasEntity(const nix::Identity &ident) const = 0;

    virtual std::shared_ptr<base::IEntity> getEntity(const nix::Identity &ident) const = 0;

    virtual std::shared_ptr<base::IEntity> getEntity(ObjectType type, ndsize_t index) const = 0;

    virtual ndsize_t entityCount(ObjectType type) const = 0;

    virtual bool removeEntity(const nix::Identity &ident) = 0;

    template<typename T>
    std::shared_ptr<T> getEntity(const nix::Identity &ident) const {
        return std::dynamic_pointer_cast<T>(this->getEntity(ident));
    }

    template<typename T>
    std::shared_ptr<T> getEntity(const std::string &name_or_id) const {
        ObjectType ot = objectToType<T>::value;
        return std::dynamic_pointer_cast<T>(this->getEntity({name_or_id, ot}));
    }

    template<typename T>
    std::shared_ptr<T> getEntity(ndsize_t index) const {
        ObjectType ot = objectToType<T>::value;
        return std::dynamic_pointer_cast<T>(this->getEntity(ot, index));
    }

    //--------------------------------------------------

    virtual std::shared_ptr<base::ISource> createSource(const std::string &name, const std::string &type) = 0;


    virtual bool deleteSource(const std::string &name_or_id) = 0;

    //--------------------------------------------------
    // Methods concerning data arrays
    //--------------------------------------------------

    virtual std::shared_ptr<base::IDataArray> createDataArray(const std::string &name, const std::string &type,
                                                              DataType data_type, const NDSize &shape,
                                                              const Compression &compression) = 0;

    //--------------------------------------------------
    // Methods concerning data frame
    //--------------------------------------------------

    virtual std::shared_ptr<base::IDataFrame> createDataFrame(const std::string &name,
                                                              const std::string &type,
                                                              const std::vector<Column> &cols) = 0;

    //--------------------------------------------------
    // Methods concerning tags.
    //--------------------------------------------------

    virtual std::shared_ptr<base::ITag> createTag(const std::string &name, const std::string &type,
                                                  const std::vector<double> &position) = 0;

    //--------------------------------------------------
    // Methods concerning multi tags.
    //--------------------------------------------------

    // TODO evaluate if DataArray can be replaced by shared_ptr<IDataArray>
    virtual std::shared_ptr<base::IMultiTag> createMultiTag(const std::string &name, const std::string &type,
                                                            const DataArray &positions) = 0;

    //--------------------------------------------------
    // Methods concerning groups.
    //--------------------------------------------------

    virtual std::shared_ptr<base::IGroup> createGroup(const std::string &name, const std::string &type) = 0;


    virtual ~IBlock() {}
};

} // namespace base

template<>
struct objectToType<base::IBlock> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::Block;
};
} // namespace nix


#endif // NIX_I_BLOCK_H
