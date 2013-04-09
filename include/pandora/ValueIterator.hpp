#ifndef PAN_VALUEITERATOR_H_INCLUDED
#define PAN_VALUEITERATOR_H_INCLUDED

#include <iterator>
#include <stdexcept>

#include <pandora/Property.hpp>
#include <pandora/Value.hpp>

namespace pandora {

class ValueIterator: public std::iterator<std::input_iterator_tag, Value> {

protected:

  Property property;
  Group group;
  size_t index, size;

public:
  ValueIterator(const Property &property, Group group);
  ValueIterator(const ValueIterator &other);

  Value operator*() const;
  ValueIterator &operator++();

  ValueIterator begin() const;
  ValueIterator end() const;

  void operator=(const ValueIterator &other);

  bool operator==(const ValueIterator &other) const;
  bool operator!=(const ValueIterator &other) const;

  virtual ~ValueIterator() {
  }
};

} /* namespace pandora */
#endif /* PAN_VALUE_H_INCLUDED */
