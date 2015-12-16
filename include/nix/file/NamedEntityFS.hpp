// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_NAMED_ENTITY_FS_H
#define NIX_NAMED_ENTITY_FS_H

#include <nix/base/INamedEntity.hpp>
#include <nix/file/EntityFS.hpp>

#include <string>
#include <memory>

namespace nix {
namespace file {


/**
 * Base class for all NIX entities e.g. Block, Section etc.
 */
class NamedEntityFS: virtual public base::INamedEntity, public EntityFS {

public:

    /**
     * Standard constructor for existing entity
     */
    NamedEntityFS(const std::shared_ptr<base::IFile> &file, const std::string &loc);

    /**
     * Standard constructor for new entity
     */
    NamedEntityFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id, const std::string &type,
                    const std::string &name);

    /**
     * Standard constructor for new entity that preserves the creation time.
     */
    NamedEntityFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id, const std::string &type,
                    const std::string &name, time_t time);


    void type(const std::string &type);


    std::string type() const;


    std::string name() const;


    boost::optional<std::string> definition() const;


    void definition(const std::string &definition);


    void definition(const none_t t);


    int compare(const std::shared_ptr<INamedEntity> &other) const;


    ~NamedEntityFS();

};


} // namespace nix::file
} // namespace nix

#endif // NIX_NAMED_ENTITY_FS_H