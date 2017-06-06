// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_SECTION_H
#define NIX_I_SECTION_H


#include <nix/base/IEntity.hpp>
#include <nix/base/INamedEntity.hpp>
#include <nix/base/IProperty.hpp>
#include <nix/DataType.hpp>
#include <nix/Value.hpp>
#include <nix/NDSize.hpp>
#include <nix/ObjectType.hpp>

#include <string>
#include <vector>

namespace nix {
namespace base {

class NIXAPI IFile;
/**
 * @brief Interface for implementations of the Section entity.
 *
 * See {@link nix::Section} for a more detailed description.
 */
class NIXAPI ISection : virtual public base::INamedEntity  {

public:

    //--------------------------------------------------
    // Attribute getter and setter
    //--------------------------------------------------

    virtual void repository(const std::string &repository) = 0;


    virtual boost::optional<std::string> repository() const = 0;


    virtual void repository(const boost::none_t t) = 0;


    virtual void link(const std::string &id) = 0;


    virtual std::shared_ptr<ISection> link() const = 0;


    virtual void link(const none_t t) = 0;


    virtual void mapping(const std::string &mapping) = 0;


    virtual boost::optional<std::string> mapping() const = 0;


    virtual void mapping(const none_t t) = 0;

    //--------------------------------------------------
    // Methods for parent access
    //--------------------------------------------------

    virtual std::shared_ptr<ISection> parent() const = 0;

    //--------------------------------------------------
    // Methods for child section access
    //--------------------------------------------------

    virtual ndsize_t sectionCount() const = 0;


    virtual bool hasSection(const std::string &name_or_id) const = 0;


    virtual std::shared_ptr<ISection> getSection(const std::string &name_or_id) const = 0;


    virtual std::shared_ptr<ISection> getSection(ndsize_t index) const = 0;


    virtual std::shared_ptr<ISection> createSection(const std::string &name, const std::string &type) = 0;


    virtual bool deleteSection(const std::string &name_or_id) = 0;

    //--------------------------------------------------
    // Methods for property access
    //--------------------------------------------------

    virtual ndsize_t propertyCount() const = 0;


    virtual bool hasProperty(const std::string &name_or_id) const = 0;


    virtual std::shared_ptr<IProperty> getProperty(const std::string &name_or_id) const = 0;


    virtual std::shared_ptr<IProperty> getProperty(ndsize_t index) const = 0;


    virtual std::shared_ptr<IProperty> createProperty(const std::string &name, const DataType &dtype) = 0;


    virtual std::shared_ptr<IProperty> createProperty(const std::string &name, const Value &value) = 0;


    virtual std::shared_ptr<IProperty> createProperty(const std::string &name, const std::vector<Value> &values) = 0;


    virtual bool deleteProperty(const std::string &name_or_id) = 0;


    virtual std::shared_ptr<IFile> parentFile() const = 0;


    virtual ~ISection() {}

};


} // namespace base

template<>
struct objectToType<nix::base::ISection> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::Section;
};

} // namespace nix

#endif // NIX_I_SECTION_H
