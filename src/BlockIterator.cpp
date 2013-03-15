/*
 * BlockIterator.cpp
 *
 *  Created on: 07.03.2013
 *      Author: stoewer
 */

#include <string>

#include <pandora/BlockIterator.hpp>

using namespace std;

namespace pandora {


/* SEE: pandora/BlockIterator.hpp */
Block BlockIterator::operator*() const {
  string id = group.objectName(index);
  Block block(file, group.openGroup(id), id);
  return block;
}


} /* namespace pandora */
