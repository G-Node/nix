#ifndef PAN_SOURCEITERATOR_H_INCLUDED
#define PAN_SOURCEITERATOR_H_INCLUDED

#include <iterator>
#include <pandora/Source.hpp>
#include <stdexcept>
#include <list>
#include <string>

namespace pandora {

class SourceIterator : public std::iterator<std::input_iterator_tag, Source>
{
 private:
   size_t nextIndex(size_t start) const;
   size_t lastIndex() const;
   bool matchesType(Group group) const;

protected:
  mutable File  *file;
  Group group;
  std::string type;
  size_t index, size;
  size_t first;


public:
  SourceIterator(File *file, Group group, std::string type = "");
  SourceIterator(const SourceIterator &other);

  Source operator*() const;
  SourceIterator &operator++();

  SourceIterator begin() const;
  SourceIterator end() const;

  void operator=(const SourceIterator &other);

  bool operator==(const SourceIterator &other) const;
  bool operator!=(const SourceIterator &other) const;

  virtual ~SourceIterator() {}
};

} /* namespace pandora */
#endif /* PAN_SOURCEITERATOR_H_INCLUDED */
