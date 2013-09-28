/*
 * Representation.hpp
 *
 *  Created on: Mar 6, 2013
 *      Author: grewe
 */

#ifndef PAN_REPRESENTATION_H_INCLUDE
#define PAN_REPRESENTATION_H_INCLUDE

#include <pandora/PandoraEntity.hpp>

namespace pandora {

class DataArray;

/**
 * Enumeration for link types.
 */
// TODO remove "untagged" from link type
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

public:

  /**
   * Copy constructor.
   */
  Representation(const Representation &representation);

  /**
   * Default constuctor.
   */
  Representation(Group group, std::string id);

  /**
   * Default constructor that preserves the creation time.
   */
  Representation(Group group, std::string id, time_t time);

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
