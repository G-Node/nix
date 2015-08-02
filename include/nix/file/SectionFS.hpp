// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SECTION_FS_H
#define NIX_SECTION_FS_H

#include <nix.hpp>
#include <nix/file/NamedEntityFS.hpp>
#include <nix/base/ISection.hpp>
#include <nix/Section.hpp>

#include <string>
#include <memory>

namespace nix {
namespace file {

class SectionFS : public NamedEntityFS, virtual public base::ISection,
                    public std::enable_shared_from_this<SectionFS> {

private:

    // TODO: consider writing parent_section as soft link into file
    std::shared_ptr<base::ISection> parent_section;
    //optGroup property_group, section_group;

public:

    /**
     * Standard constructor for existing entity
     */
    SectionFS(const std::shared_ptr<base::IFile> &file, const std::string &loc);

    /**
     * Standard constructor for existing entity
     */
    SectionFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::ISection> &parent,
              const std::string &loc);

    /**
     * Standard constructor for new entity
     */
    SectionFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
              const std::string &type, const std::string &name);

    /**
     * Standard constructor for new entity with parent.
     */
    SectionFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::ISection> &parent,
              const std::string &loc, const std::string &id, const std::string &type, const std::string &name);

    /**
     * Constructor that preserves the creation time.
     */
    SectionFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
              const std::string &type, const std::string &name, time_t time);

    /**
     * Constructor with parent that preserves the creation time.
     */
    SectionFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::ISection> &parent,
              const std::string &loc, const std::string &id, const std::string &type,
              const std::string &name, time_t time);


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


    ndsize_t sectionCount() const;


    bool hasSection(const std::string &name_or_id) const;


    std::shared_ptr<base::ISection> getSection(const std::string &name_or_id) const;


    std::shared_ptr<base::ISection> getSection(ndsize_t index) const;


    std::shared_ptr<base::ISection> createSection(const std::string &name, const std::string &type);


    bool deleteSection(const std::string &name_or_id);

    //--------------------------------------------------
    // Methods for property access
    //--------------------------------------------------


    ndsize_t propertyCount() const;


    bool hasProperty(const std::string &name_or_id) const;


    std::shared_ptr<base::IProperty> getProperty(const std::string &name_or_id) const;


    std::shared_ptr<base::IProperty> getProperty(ndsize_t index) const;


    std::shared_ptr<base::IProperty> createProperty(const std::string &name, const DataType &dtype);


    std::shared_ptr<base::IProperty> createProperty(const std::string &name, const Value &value);


    std::shared_ptr<base::IProperty> createProperty(const std::string &name, const std::vector<Value> &values);


    bool deleteProperty(const std::string &name_or_id);

    //--------------------------------------------------
    // Ohter methods and operators
    //--------------------------------------------------

    virtual ~SectionFS();

};


} // namespace nix::file
} // namespace nix

#endif // NIX_SECTION_FS_H
