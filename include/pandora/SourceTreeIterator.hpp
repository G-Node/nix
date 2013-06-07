#ifndef PAN_SOURCETREEITERATOR_H_INCLUDED
#define PAN_SOURCETREEITERATOR_H_INCLUDED

#include <iterator>
#include <stdexcept>
#include <iterator>
#include <pandora/Source.hpp>
#include <pandora/SourceIterator.hpp>
#include <list>
#include <string>

namespace pandora {
class SourceTreeIterator : public std::iterator<std::input_iterator_tag, Source>
{
protected:
  Source root;

private:
  std::list<SourceIterator> q;
  std::list<size_t> l;
  SourceIterator currentIter;
  std::string type;
  size_t depth, level;
  bool at_end;
  void push_children();

public:
  SourceTreeIterator(const Source parent, std::string type = "", uint depth = 0);
  SourceTreeIterator(const SourceTreeIterator &other);

  Source operator*() const;
  SourceTreeIterator &operator++();

  SourceTreeIterator begin() const;
  SourceTreeIterator end() const;

  void operator=(const SourceTreeIterator &other);

  bool operator==(const SourceTreeIterator &other) const;
  bool operator!=(const SourceTreeIterator &other) const;

  virtual ~SourceTreeIterator() {}
};



} /* namespace pandora */
#endif /* PAN_SECTIONTREEITERATOR_H_INCLUDED */
