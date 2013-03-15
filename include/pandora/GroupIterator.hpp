#ifndef PAN_GROUPITERATOR_H_INCLUDED
#define PAN_GROUPITERATOR_H_INCLUDED

#include <iterator>
#include <pandora/File.hpp>
#include <pandora/Group.hpp>

namespace pandora {

template<class T> class GroupIterator : public std::iterator<std::input_iterator_tag, T>
{

protected:

  File  file;
  Group group;
  size_t index, size;

public:
  GroupIterator(File file, Group group);
  GroupIterator(const GroupIterator<T> &other);

  virtual T operator*() const = 0;
  GroupIterator &operator++();

  GroupIterator<T> &begin() const;
  GroupIterator<T> &end() const;

  void operator=(const GroupIterator<T> &other);

  bool operator==(const GroupIterator<T> &other) const;
  bool operator!=(const GroupIterator<T> &other) const;

  virtual ~GroupIterator() {}
};

} /* namespace pandora */

#endif /* PAN_GROUPITERATOR_H_INCLUDED */
