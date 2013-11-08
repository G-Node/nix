// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef PAN_REPRESENTATION_H_INCLUDE
#define PAN_REPRESENTATION_H_INCLUDE

#include <pandora/Block.hpp>
#include <pandora/DataArray.hpp>
#include <pandora/PandoraEntity.hpp>

namespace pandora {

/**
 * Enumeration for link types.
 *
 *  TODO remove "untagged" from link type
 */
enum class LinkType : int {
    TAGGED, UNTAGGED, INDEXED
};

/**
 * Converts a LinkType into a string.
 *
 * @param link_type   The type to convert.
 */
std::string linkTypeToString(LinkType link_type);

/**
 * Create a link type from a string. If no matching type
 * exists, an exception will be thrown.
 *
 * @return The link type that matches the string.
 */
LinkType linkTypeFromString(const std::string &str);

/**
 * Class that represents a pandora representation entity
 * TODO implement methods for DataArray.
 */
class Representation: public PandoraEntity {

protected:

    Block block;

public:

    /**
     * Copy constructor.
     */
    Representation(const Representation &representation);

    /**
     * Default constuctor.
     */
    Representation(Group group, const std::string &id, Block block);

    /**
     * Default constructor that preserves the creation time.
     */
    Representation(Group group, const std::string &id, time_t time, Block block);

    /**
     * Setter for the link type.
     *
     * @param type    The link type to set.
     */
    void linkType(LinkType type);

    /**
     * Getter for the link type.
     *
     * @return The current link type of the representation.
     */
    LinkType linkType() const;

    /**
     * Sets the data array associated with this representation.
     *
     * @param data    The data array to set.
     */
    void data(const DataArray &data);

    /**
     * Gets the data array associated with this representation.
     *
     * @return The associated data array.
     */
    DataArray data() const;

    /**
     * Destructor.
     */
    virtual ~Representation();

};

} // namespace

#endif //PAN_REPRESENTATION_H_INCLUDE
