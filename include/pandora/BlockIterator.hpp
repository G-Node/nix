/*
 * BlockIterator.hpp
 *
 *  Created on: 07.03.2013
 *      Author: stoewer
 */

#ifndef BLOCKITERATOR_HPP_
#define BLOCKITERATOR_HPP_

#include <iterator>
#include <H5Cpp.h>
#include <pandora/Block.hpp>


namespace pandora {

class BlockIterator : public std::iterator<std::input_iterator_tag, Block>
{
private:

  File &file;
  H5::H5Object &h5group;
  int index, size;

public:
  BlockIterator(File &file);
  BlockIterator(BlockIterator &iter);

  bool operator!=(BlockIterator &iter);
  bool operator==(BlockIterator &iter);

  Block operator*();
  BlockIterator &operator++();

  BlockIterator &begin();
  BlockIterator &end();

  virtual ~BlockIterator();
};

} /* namespace pandora */
#endif /* BLOCKITERATOR_HPP_ */
