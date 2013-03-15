
#ifndef PAN_BLOCKITERATOR_H_INCLUDED
#define PAN_BLOCKITERATOR_H_INCLUDED

#include <iterator>
#include <pandora/Block.hpp>

namespace pandora {

class BlockIterator : public std::iterator<std::input_iterator_tag, Block>
{

protected:

  File  file;
  Group group;
  size_t index, size;

public:
  BlockIterator(File file, Group group);
  BlockIterator(const BlockIterator &other);

  Block operator*() const;
  BlockIterator &operator++();

  BlockIterator begin() const;
  BlockIterator end() const;

  void operator=(const BlockIterator &other);

  bool operator==(const BlockIterator &other) const;
  bool operator!=(const BlockIterator &other) const;

  virtual ~BlockIterator() {}
};

} /* namespace pandora */
#endif /* PAN_BLOCKITERATOR_H_INCLUDED */
