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
  tagged, untagged, indexed
};

class Representation: public PandoraEntity {
private:
  std::string rdata;
  RepresentationType rtype;
public:
  Representation(const Representation &representation);

  Representation(File *file, H5::Group h5group, std::string id, std::string data_array_id, RepresentationType type);

  void data(std::string dataarray_id);

  //void data(const DataArray da);

  std::string data() const;

  void type(pandora::RepresentationType type);

  RepresentationType type() const;

  virtual ~Representation();
};

}
#endif /* REPRESENTATION_HPP_ */
