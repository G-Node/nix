#include <pandora/TreeIterator.hpp>

using namespace std;

namespace pandora {

TreeIterator::TreeIterator(const Section other) :
                                root(other), currentIter(other.children()) {
 push_children();
}

TreeIterator::TreeIterator(const TreeIterator &other) :
                                root(other.root), q(other.q), currentIter(other.currentIter) {
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
  while(q.size() > 0){
    currentIter = q.front();
    currentIter.begin();
    q.pop_front();
    if(currentIter!=currentIter.end()){
      push_children();
      break;
    }
  }
  return *this;
}

TreeIterator TreeIterator::begin() const {
  TreeIterator iter(*this);
  iter.q.erase(iter.q.begin(), iter.q.end());
  iter.currentIter = root.children();
  return iter;
}

TreeIterator TreeIterator::end() const {
  TreeIterator iter(*this);
  iter.q.erase(iter.q.begin(), iter.q.end());
  iter.currentIter = iter.currentIter.end();
  return iter;
}

void TreeIterator::operator=(const TreeIterator &other) {
  root = other.root;
  q = other.q;
  currentIter = other.currentIter;
}

bool TreeIterator::operator==(const TreeIterator &other) const {
  return root == other.root && q.size() == other.q.size() && ((currentIter == other.currentIter) || q.size() == 0) ;

}

bool TreeIterator::operator!=(const TreeIterator &other) const {
  return root != other.root || currentIter != other.currentIter || q.size() != other.q.size();
}

void TreeIterator::push_children(){
  if(currentIter != currentIter.end()){
     Section s = *currentIter;
     q.push_back(s.children());
   }
}

}
