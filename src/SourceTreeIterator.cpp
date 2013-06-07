#include <pandora/SourceTreeIterator.hpp>

using namespace std;

namespace pandora {

SourceTreeIterator::SourceTreeIterator(const Source other, std::string type, uint depth) :
      root(other), currentIter(other.children(type)), type(type), depth(depth) {
  level = 1;
  at_end = currentIter == currentIter.end();
  push_children();
}

SourceTreeIterator::SourceTreeIterator(const SourceTreeIterator &other) :
      root(other.root), q(other.q), currentIter(other.currentIter), type(other.type), depth(other.depth), level(other.level) {
}

Source SourceTreeIterator::operator*() const {
  return *currentIter;
}

SourceTreeIterator &SourceTreeIterator::operator++() {
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

SourceTreeIterator SourceTreeIterator::begin() const {
  SourceTreeIterator iter(*this);
  iter.q.erase(iter.q.begin(), iter.q.end());
  iter.l.erase(iter.l.begin(), iter.l.end());
  iter.currentIter = root.children();
  iter.level = 0;
  iter.depth = depth;
  iter.at_end = false;
  return iter;
}

SourceTreeIterator SourceTreeIterator::end() const {
  SourceTreeIterator iter(*this);
  iter.q.erase(iter.q.begin(), iter.q.end());
  iter.l.erase(iter.l.begin(), iter.l.end());
  iter.currentIter = iter.currentIter.end();
  iter.level = depth;
  iter.at_end = true;
  return iter;
}

void SourceTreeIterator::operator=(const SourceTreeIterator &other) {
  root = other.root;
  q = other.q;
  l = other.l;
  currentIter = other.currentIter;
  depth = other.depth;
  level = other.level;
}

bool SourceTreeIterator::operator==(const SourceTreeIterator &other) const {
  return root == other.root && q.size() == other.q.size() && ((currentIter == other.currentIter)
      || q.size() == 0) && at_end == other.at_end;

}

bool SourceTreeIterator::operator!=(const SourceTreeIterator &other) const {
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

void SourceTreeIterator::push_children() {
  if (currentIter != currentIter.end()) {
    Source s = *currentIter;
    q.push_back(s.children(type));
    l.push_back(level+1);
  }
}

}
