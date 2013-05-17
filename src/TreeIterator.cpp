#include <pandora/TreeIterator.hpp>

using namespace std;

namespace pandora {

TreeIterator::TreeIterator(const Section other, uint depth) :
      root(other), currentIter(other.children()),depth(depth) {
  level = 1;
  at_end = currentIter == currentIter.end();
  push_children();
}

TreeIterator::TreeIterator(const TreeIterator &other) :
      root(other.root), q(other.q), currentIter(other.currentIter), depth(other.depth), level(other.level) {
}

Section TreeIterator::operator*() const {
  return *currentIter;
}

TreeIterator &TreeIterator::operator++() {
  ++currentIter;
  if (currentIter != currentIter.end()) {
    push_children();
    return *this;
  }
  while (q.size() > 0 && (depth == 0 || l.front() <= depth)) {
    currentIter = q.front();
    q.pop_front();
    level = l.front();
    l.pop_front();
    if (currentIter != currentIter.end()) {
      push_children();
      break;
    }
  }
  if(currentIter == currentIter.end() && (q.size() == 0 || (depth != 0 && level >= depth))){
    at_end = true;
    level = depth;
    q.erase(q.begin(),q.end());
  }
  return *this;
}

TreeIterator TreeIterator::begin() const {
  TreeIterator iter(*this);
  iter.q.erase(iter.q.begin(), iter.q.end());
  iter.l.erase(iter.l.begin(), iter.l.end());
  iter.currentIter = root.children();
  iter.level = 0;
  iter.depth = depth;
  iter.at_end = false;
  return iter;
}

TreeIterator TreeIterator::end() const {
  TreeIterator iter(*this);
  iter.q.erase(iter.q.begin(), iter.q.end());
  iter.l.erase(iter.l.begin(), iter.l.end());
  iter.currentIter = iter.currentIter.end();
  iter.level = depth;
  iter.at_end = true;
  return iter;
}

void TreeIterator::operator=(const TreeIterator &other) {
  root = other.root;
  q = other.q;
  l = other.l;
  currentIter = other.currentIter;
  depth = other.depth;
  level = other.level;
}

bool TreeIterator::operator==(const TreeIterator &other) const {
  return root == other.root && q.size() == other.q.size() && ((currentIter == other.currentIter)
      || q.size() == 0) && at_end == other.at_end;

}

bool TreeIterator::operator!=(const TreeIterator &other) const {
/*
  cerr << "\t root: " << (root != other.root) << endl;
  cerr << "\t iter: " << (currentIter != other.currentIter) << endl;
  cerr << "\t iter.end(): " << (currentIter != currentIter.end()) << endl;
  cerr << "\t Queue: " << (q.size() != other.q.size()) << endl;
  cerr << "\t depth: " << (depth != other.depth) << endl;
  cerr << "\t atEnd: " << (at_end != other.at_end) << endl;
*/
  return root != other.root || currentIter != other.currentIter || q.size() != other.q.size() ||
      depth != other.depth || at_end != other.at_end;
}

void TreeIterator::push_children() {
  if (currentIter != currentIter.end()) {
    Section s = *currentIter;
    q.push_back(s.children());
    l.push_back(level+1);
  }
}

}
