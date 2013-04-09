#ifndef PAN_PROPERTYITERATOR_H_INCLUDED
#define PAN_PROPERTYITERATOR_H_INCLUDED

#include <iterator>
#include <stdexcept>

#include <pandora/Property.hpp>

namespace pandora {

class PropertyIterator: public std::iterator<std::input_iterator_tag, Property> {

protected:

  Section section;
  Group group;
  size_t index, size;

public:
  PropertyIterator(const Section &section, Group group);
  PropertyIterator(const PropertyIterator &other);

  Property operator*() const;
  PropertyIterator &operator++();

  PropertyIterator begin() const;
  PropertyIterator end() const;

  void operator=(const PropertyIterator &other);

  bool operator==(const PropertyIterator &other) const;
  bool operator!=(const PropertyIterator &other) const;

  virtual ~PropertyIterator() {
  }
};

} /* namespace pandora */
#endif /* PAN_PROPERTY_H_INCLUDED */
