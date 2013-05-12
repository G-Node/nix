#include <pandora/TreeIterator.hpp>

using namespace std;

namespace pandora {

TreeIterator::TreeIterator(const Section parent) :
                                parent(parent), currentIter(parent.children()) {
  cerr << "Constructor.parent.id: " << parent.id() << endl;
  if(currentIter != currentIter.end()){
    Section s = *currentIter;
    cerr << "Contructor.s.name(): " << s.name() << endl;
    q.push_back(s.children());
  }

}

TreeIterator::TreeIterator(const TreeIterator &other) :
                                parent(other.parent), q(other.q), currentIter(other.currentIter) {
}

Section TreeIterator::operator*() const {
  return *currentIter;
}

TreeIterator &TreeIterator::operator++() {
  cerr << "TreeIterator++: " << endl;
  cerr << (*currentIter).name() << endl;
  ++currentIter;

  if (currentIter != currentIter.end()) {
    Section s = *currentIter;
    cerr << "\t next section.name: " << s.name() << endl;
    cerr << "\t pushed children: "<< endl;
    q.push_back(s.children());
    for(SectionIterator iter = (*currentIter).children(); iter != iter.end(); ++iter){
      Section s2 = *iter;
      cerr << "\t\t"<< s2.name() << endl;
    }
    cerr << "\t new queue.size: " << q.size() << endl;
    return *this;
  }

  cerr << "\t end of currentIter" << endl;
  while(q.size() > 0){
    cerr << "\t switch to new iter" << endl;
    currentIter = q.front();
    q.pop_front();
    if(currentIter != currentIter.end()){
      Section s = *currentIter;
      cerr << "\t push children of: " << s.name() << endl;
      q.push_back(s.children());
      for(SectionIterator iter = s.children(); iter != iter.end(); ++iter){
        Section s2 = *iter;
        cerr << "\t\t" << s2.name() << endl;
      }
      break;
    }
    cerr << "\t new queue.size: " << q.size() << endl;
  }
  return *this;
}

TreeIterator TreeIterator::begin() const {
  TreeIterator iter(*this);
  iter.q.erase(iter.q.begin(), iter.q.end());
  iter.currentIter = parent.children();
  return iter;
}

TreeIterator TreeIterator::end() const {
  TreeIterator iter(*this);
  iter.q.erase(iter.q.begin(), iter.q.end());
  iter.currentIter = currentIter.end();
  /*
  cerr << "TreeIterator.end().currentIter: " << (iter.currentIter == iter.currentIter.end()) << endl;
  cerr << "TreeIterator.end().q.size: " << iter.q.size() << endl;
   */
  return iter;
}

void TreeIterator::operator=(const TreeIterator &other) {
  parent = other.parent;
  q = other.q;
  currentIter = other.currentIter;
}

bool TreeIterator::operator==(const TreeIterator &other) const {
  return parent == other.parent && currentIter == other.currentIter && q.size() == other.q.size();

}

bool TreeIterator::operator!=(const TreeIterator &other) const {
  /*
  cerr << "TreeIterator.!= parent == other.parent: " << (parent == other.parent) << endl;
  cerr << "TreeIterator.!= currentIter == otherIter: " << (currentIter == other.currentIter) << endl;
  cerr << "TreeIterator.!= q.size == other.q.size: " << (q.size() == other.q.size()) << endl;
   */
  return parent != other.parent || currentIter != other.currentIter || q.size() != other.q.size();
}

}
