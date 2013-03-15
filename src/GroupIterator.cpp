/*
 * GroupIterator.cpp
 *
 *  Created on: 15.03.2013
 *      Author: stoewer
 */

#include <pandora/GroupIterator.hpp>

namespace pandora {

template<class T> GroupIterator<T>::GroupIterator(File file, Group group)
  : file(file), group(group)
{
  index = 0;
  size  = group.objectCount();
}

template<class T> GroupIterator<T>::GroupIterator(const GroupIterator<T> &other)
  : file(other.file), group(other.group), index(other.index), size(other.size)
{
}

template<class T> GroupIterator<T> &GroupIterator<T>::operator++() {
  index++;
  return *this;
}

template<class T> GroupIterator<T> &GroupIterator<T>::begin() const {
  GroupIterator<T> iter(*this);
  iter.index = 0;
  return iter;
}

template<class T> GroupIterator<T> &GroupIterator<T>::end() const {
  GroupIterator<T> iter(*this);
  iter.index = size - 1;
  return iter;
}

template<class T> void GroupIterator<T>::operator=(const GroupIterator<T> &other) {
  file  = other.file;
  group = other.group;
  index = other.index;
  size  = other.size;
}

template<class T> bool GroupIterator<T>::operator==(const GroupIterator<T> &other) const {
  return group == other.group && index == other.index;
}

template<class T> bool GroupIterator<T>::operator!=(const GroupIterator<T> &other) const {
  return group != other.group || index != other.index;
}


} /* namespace pandora */
