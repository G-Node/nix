
#ifndef PAN_BLOCKITERATOR_H_INCLUDED
#define PAN_BLOCKITERATOR_H_INCLUDED

#include <iterator>
#include <H5Cpp.h>
#include <pandora/Block.hpp>
#include <pandora/GroupIterator.hpp>


namespace pandora {

class BlockIterator : public GroupIterator<Block>
{
private:

  File file;
  size_t index, size;

public:
  BlockIterator(File file);
  BlockIterator(const BlockIterator &other);

  Block &operator*() const;
  BlockIterator &operator++();

  BlockIterator &begin() const;
  BlockIterator &end() const;

  void operator=(const BlockIterator &other);

  virtual ~BlockIterator();
};

} /* namespace pandora */
#endif /* PAN_BLOCKITERATOR_H_INCLUDED */
