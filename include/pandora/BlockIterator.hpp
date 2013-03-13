
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

  bool operator!=(const BlockIterator &other) const;
  bool operator==(const BlockIterator &other) const;

  Block &operator*() const;
  BlockIterator &operator++();

  BlockIterator &begin() const;
  BlockIterator &end() const;

  void operator=(const BlockIterator &other);

  virtual ~BlockIterator();
};

} /* namespace pandora */
#endif /* PAN_BLOCKITERATOR_H_INCLUDED */
