#ifndef PAN_SECTIONITERATOR_H_INCLUDED
#define PAN_SECTIONITERATOR_H_INCLUDED

#include <iterator>
#include <pandora/Section.hpp>

namespace pandora {

class SectionIterator : public std::iterator<std::input_iterator_tag, Section>
{

private:
  size_t nextIndex(size_t start) const;
  size_t lastIndex() const;
  bool matchesParent(Group group) const;

protected:

  File  file;
  Group group;
  size_t index, size;
  std::string parent;
  size_t first;

public:
  SectionIterator(File file, Group group, std::string parent_id = "");
  SectionIterator(const SectionIterator &other);

  Section operator*() const;
  SectionIterator &operator++();

  SectionIterator begin() const;
  SectionIterator end() const;

  void operator=(const SectionIterator &other);

  bool operator==(const SectionIterator &other) const;
  bool operator!=(const SectionIterator &other) const;

  virtual ~SectionIterator() {}
};

} /* namespace pandora */
#endif /* PAN_SECTIONITERATOR_H_INCLUDED */
