/*
 * BlockIterator.cpp
 *
 *  Created on: 07.03.2013
 *      Author: stoewer
 */

#include <string>

#include <pandora/Block.hpp>
#include <pandora/BlockIterator.hpp>

using namespace std;

namespace pandora {

/* SEE: pandora/BlockIterator.hpp */
BlockIterator::BlockIterator(File file)
  : file(file), index(0), size(file.blockCount())
{
  // nothing left to do
}

/* SEE: pandora/BlockIterator.hpp */
BlockIterator::BlockIterator(const BlockIterator &other)
  : file(other.file), index(other.index), size(other.size)
{
  // nothing left to do
}

/* SEE: pandora/BlockIterator.hpp */
BlockIterator &BlockIterator::begin() const {
  BlockIterator iter(*this);
  iter.index = 0;
  return iter;
}

/* SEE: pandora/BlockIterator.hpp */
BlockIterator &BlockIterator::end() const {
  BlockIterator iter(*this);
  iter.index = iter.size - 1;
  return iter;
}

/* SEE: pandora/BlockIterator.hpp */
bool BlockIterator::operator!=(const BlockIterator &other) const {
  return !(this->file == other.file && this->index == other.index);
}

/* SEE: pandora/BlockIterator.hpp */
bool BlockIterator::operator==(const BlockIterator &other) const {
  return (this->file == other.file && this->index == other.index);
}

/* SEE: pandora/BlockIterator.hpp */
BlockIterator &BlockIterator::operator++() {
  index++;
  return *this;
}

/* SEE: pandora/BlockIterator.hpp */
Block &BlockIterator::operator*() const {
  Block block(file.getBlock(index));
  return block;
}

void BlockIterator::operator=(const BlockIterator &other) {
  this->file  = other.file;
  this->index = other.index;
  this->size  = other.size;
}

/* SEE: pandora/BlockIterator.hpp */
BlockIterator::~BlockIterator() {
  // TODO Auto-generated destructor stub
}

} /* namespace pandora */
