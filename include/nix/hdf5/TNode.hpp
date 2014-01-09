// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Authors: Christian Kellner <kellner@bio.lmu.de>


#ifndef NIX_TNODE_H
#define NIX_TNODE_H

#include <vector>
#include <functional>

// TODO this is currently not used and I am not sure if we really sill need it.

namespace nix {
namespace hdf5 {


template<typename T>
class TNode {

public:
  typedef size_t size_type;

  //pure virtual functions
  virtual size_type childCount() const = 0;
  virtual T         getChild(size_type index) const = 0;

  std::vector<T>    collectIf(std::function<bool(const T &)> predicate,
                              bool exclude_root = false,
                              int max_depth = -1) const;


  virtual ~TNode() { };

  //predicate helper
  static bool predCollectAll(const T &node) { return true; };

private:
  const T *self() const { return static_cast<const T *>(this); }; //CRTP
  void collectRec(const TNode<T> &node,
                  std::vector<T> &results,
                  std::function<bool(const T &)> predicate,
                  int level,
                  int max_depth) const;
};


//----------------------------------------------------------------------------------------
//

/**
 * Return a vector of base type T nodes for which the predicate evaluates to true.
 *
 * @param predicate     Predicate function that will be called for each node.
 *                      Return true to collect.
 * @param exclude_root  Whether or not to include the root node.
 * @param max_depth     The maximum recursion depth.
 *
 * @return A vector of T nodes for which predicate was true.
 */
template<typename T>
std::vector<T> TNode<T>::collectIf(std::function<bool(const T &)> predicate,
                                   bool exclude_root,
                                   int max_depth) const
{
    std::vector<T> results;
    if (!exclude_root && predicate(*self())) {
        results.push_back(*self());
    }

    collectRec(*this, results, predicate, 1, max_depth);
    return results;
}


template<typename T>
void TNode<T>::collectRec(const TNode<T> &node,
                          std::vector<T> &results,
                          std::function<bool(const T &)> predicate,
                          int level,
                          int max_depth) const
{
  if (max_depth > 0 && level > max_depth) {
    return;
  }

  size_type source_count = node.childCount();
  std::vector<T> my_children;

  for (size_type i = 0; i < source_count; i++) {
    T s = node.getChild(i);

    if (predicate(s)) {
      results.push_back(s);
    }

    my_children.push_back(s);
  }

  for (size_type i = 0; i < my_children.size(); i++) {
    collectRec(my_children[i], results, predicate, level + 1, max_depth);
  }
}

} // namespace hdf5
} // namespace nix

#endif //NIX_TNODE_H
