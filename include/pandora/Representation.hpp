/*
 * Representation.hpp
 *
 *  Created on: Mar 6, 2013
 *      Author: grewe
 */

#ifndef PAN_REPRESENTATION_H_INCLUDE
#define PAN_REPRESENTATION_H_INCLUDE
#include <string>
#include <iostream>

#include <pandora/PandoraEntity.hpp>
#include <pandora/DataArray.hpp>

namespace pandora {

enum class RepresentationType {
  tagged=0, untagged=1, indexed=2
};

class Representation: public PandoraEntity {
public:
  Representation(const Representation &representation);

  Representation(File *file, H5::Group h5group, std::string id, std::string data_array_id, RepresentationType type);

  void data(std::string data_array_id);

  void data(const DataArray &data_array);

  std::string data() const;

  void type(pandora::RepresentationType type);

  RepresentationType type() const;

  virtual ~Representation();
};

}
#endif /* REPRESENTATION_HPP_ */
