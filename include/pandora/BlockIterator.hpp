
#ifndef PAN_BLOCKITERATOR_H_INCLUDED
#define PAN_BLOCKITERATOR_H_INCLUDED

#include <iterator>
#include <H5Cpp.h>
#include <pandora/Block.hpp>


namespace pandora {

class BlockIterator : public std::iterator<std::input_iterator_tag, Block>
{
private:

  File file;
  size_t index, size;

public:
  BlockIterator(File file);
  BlockIterator(const BlockIterator &other);

  bool operator!=(BlockIterator &other);
  bool operator==(BlockIterator &other);

  Block &operator*();
  BlockIterator &operator++();

  BlockIterator &begin();
  BlockIterator &end();

  virtual ~BlockIterator();
};

} /* namespace pandora */
#endif /* PAN_BLOCKITERATOR_H_INCLUDED */
