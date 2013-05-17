#ifndef PAN_ODMLTREEITERATOR_H_INCLUDED
#define PAN_ODMLTREEITERATOR_H_INCLUDED

#include <iterator>
#include <stdexcept>
#include <iterator>
#include <pandora/Section.hpp>
#include <pandora/SectionIterator.hpp>
#include <list>
#include <string>

namespace pandora {
class TreeIterator : public std::iterator<std::input_iterator_tag, Section>
{
protected:
  Section root;

private:
  std::list<SectionIterator> q;
  std::list<size_t> l;
  SectionIterator currentIter;
  size_t depth, level;
  bool at_end;
  void push_children();

public:
  TreeIterator(const Section parent, uint depth = 0);
  TreeIterator(const TreeIterator &other);

  Section operator*() const;
  TreeIterator &operator++();

  TreeIterator begin() const;
  TreeIterator end() const;

  void operator=(const TreeIterator &other);

  bool operator==(const TreeIterator &other) const;
  bool operator!=(const TreeIterator &other) const;

  virtual ~TreeIterator() {}
};



} /* namespace pandora */
#endif /* PAN_ODMLTREEITERATOR_H_INCLUDED */
