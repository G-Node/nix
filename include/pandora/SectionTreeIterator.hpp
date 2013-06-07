#ifndef PAN_SECTIONTREEITERATOR_H_INCLUDED
#define PAN_SECTIONTREEITERATOR_H_INCLUDED

#include <iterator>
#include <stdexcept>
#include <iterator>
#include <pandora/Section.hpp>
#include <pandora/SectionIterator.hpp>
#include <list>
#include <string>

namespace pandora {
class SectionTreeIterator : public std::iterator<std::input_iterator_tag, Section>
{
protected:
  Section root;

private:
  std::list<SectionIterator> q;
  std::list<size_t> l;
  SectionIterator currentIter;
  std::string type;
  size_t depth, level;
  bool at_end;
  void push_children();

public:
  SectionTreeIterator(const Section parent, std::string type = "", uint depth = 0);
  SectionTreeIterator(const SectionTreeIterator &other);

  Section operator*() const;
  SectionTreeIterator &operator++();

  SectionTreeIterator begin() const;
  SectionTreeIterator end() const;

  void operator=(const SectionTreeIterator &other);

  bool operator==(const SectionTreeIterator &other) const;
  bool operator!=(const SectionTreeIterator &other) const;

  virtual ~SectionTreeIterator() {}
};



} /* namespace pandora */
#endif /* PAN_SECTIONTREEITERATOR_H_INCLUDED */
