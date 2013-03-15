
#ifndef PAN_BLOCKITERATOR_H_INCLUDED
#define PAN_BLOCKITERATOR_H_INCLUDED

#include <pandora/Block.hpp>
#include <pandora/GroupIterator.hpp>


namespace pandora {

class BlockIterator : public GroupIterator<Block>
{

public:

  Block operator*() const;

  virtual ~BlockIterator() {}
};

} /* namespace pandora */
#endif /* PAN_BLOCKITERATOR_H_INCLUDED */
