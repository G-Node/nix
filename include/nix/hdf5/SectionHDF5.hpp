// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SECTION_HDF5_H
#define NIX_SECTION_HDF5_H

#include <memory>

#include <nix/hdf5/NamedEntityHDF5.hpp>
#include <nix/base/ISection.hpp>
#include <nix/Section.hpp>

namespace nix {
namespace hdf5 {

class SectionHDF5 : public NamedEntityHDF5, virtual public base::ISection,
                    public std::enable_shared_from_this<SectionHDF5> {

private:

    // TODO: consider writing parent_section as soft link into file
    std::shared_ptr<base::ISection> parent_section;
    optGroup property_group, section_group;

public:

    /**
     * Standard constructor for existing entity
     */
    SectionHDF5(std::shared_ptr<base::IFile> file, const Group &group);
    
    /**
     * Standard constructor for existing entity
     */
    SectionHDF5(std::shared_ptr<base::IFile> file, std::shared_ptr<base::ISection> parent, const Group &group);

    /**
     * Standard constructor for new entity
     */
    SectionHDF5(std::shared_ptr<base::IFile> file, const Group &group, const std::string &id,
                const std::string &type, const std::string &name);

    /**
     * Standard constructor for new entity with parent.
     */
    SectionHDF5(std::shared_ptr<base::IFile> file, std::shared_ptr<base::ISection> parent, const Group &group,
                const std::string &id, const std::string &type, const std::string &name);

    /**
     * Constructor that preserves the creation time.
     */
    SectionHDF5(std::shared_ptr<base::IFile> file, const Group &group, const std::string &id,
                const std::string &type, const std::string &name, time_t time);

    /**
     * Constructor with parent that preserves the creation time.
     */
    SectionHDF5(std::shared_ptr<base::IFile> file, std::shared_ptr<base::ISection> parent, const Group &group,
                const std::string &id, const std::string &type, const std::string &name, time_t time);


    //--------------------------------------------------
    // Attribute getter and setter
    //--------------------------------------------------

    void repository(const std::string &repository);


    boost::optional<std::string> repository() const;


    void repository(const none_t t);


    void link(const std::string &id);


    std::shared_ptr<base::ISection> link() const;


    void link(const none_t t);


    void mapping(const std::string &mapping);


    boost::optional<std::string> mapping() const;


    void mapping(const none_t t);


    //--------------------------------------------------
    // Methods for parent access
    //--------------------------------------------------


    std::shared_ptr<base::ISection> parent() const;


    //--------------------------------------------------
    // Methods for child section access
    //--------------------------------------------------


    size_t sectionCount() const;


    bool hasSectionByName(const std::string &name) const;


    bool hasSection(const std::string &id) const;


    std::shared_ptr<base::ISection> getSection(const std::string &id) const;


    std::shared_ptr<base::ISection> getSection(size_t index) const;


    std::shared_ptr<base::ISection> getSectionByName(const std::string &name) const;


    std::shared_ptr<base::ISection> createSection(const std::string &name, const std::string &type);


    bool deleteSection(const std::string &id);

    //--------------------------------------------------
    // Methods for property access
    //--------------------------------------------------


    size_t propertyCount() const;


    bool hasProperty(const std::string &id) const;


    bool hasPropertyByName(const std::string &name) const;


    std::shared_ptr<base::IProperty> getProperty(const std::string &id) const;


    std::shared_ptr<base::IProperty> getProperty(size_t index) const;


    std::shared_ptr<base::IProperty> getPropertyByName(const std::string &name) const;


    std::shared_ptr<base::IProperty> createProperty(const std::string &name, const DataType &dtype);


    std::shared_ptr<base::IProperty> createProperty(const std::string &name, const Value &value);


    std::shared_ptr<base::IProperty> createProperty(const std::string &name, const std::vector<Value> &values);


    bool deleteProperty(const std::string &id);

    //--------------------------------------------------
    // Ohter methods and operators
    //--------------------------------------------------

    virtual ~SectionHDF5();

};


} // namespace hdf5
} // namespace nix

#endif // NIX_SECTION_HDF5_H
