
#ifndef PAN_DATAARRAYITERATOR_H_INCLUDED
#define PAN_DATAARRAYITERATOR_H_INCLUDED

#include <iterator>
#include <pandora/DataArray.hpp>

namespace pandora {

class DataArrayIterator : public std::iterator<std::input_iterator_tag, DataArray>
{

protected:

  mutable File  *file;
  Group group;
  size_t index, size;

public:
  DataArrayIterator(File *file, Group group);
  DataArrayIterator(const DataArrayIterator &other);

  DataArray operator*() const;
  DataArrayIterator &operator++();

  DataArrayIterator begin() const;
  DataArrayIterator end() const;

  void operator=(const DataArrayIterator &other);

  bool operator==(const DataArrayIterator &other) const;
  bool operator!=(const DataArrayIterator &other) const;

  virtual ~DataArrayIterator() {}
};

} /* namespace pandora */
#endif /* PAN_DATAARRAYITERATOR_H_INCLUDED */
