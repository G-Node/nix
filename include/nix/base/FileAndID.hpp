// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_FILEANDID_H
#define NIX_FILEANDID_H

#include <nix/File.hpp>

namespace nix {

class NIXAPI FileAndID {
protected:
    std::string   entity_id;
    File          entity_file;
    
public:

    FileAndID(const std::string &_id, const File &_file)
        : entity_id(_id), entity_file(_file)
    {
    }

    /**
     * Getter for the id of the entity.
     *
     * @return The id of the entity.
     */
    std::string id() const {
        return entity_id;
    }

protected:

    /**
     * Gets the file object the entity was returned by.
     *
     * @return object The file object the entity was returned by.
     */
    File file() const {
        return entity_file;
    }

} // namespace nix

#endif // NIX_FILEANDID_H
