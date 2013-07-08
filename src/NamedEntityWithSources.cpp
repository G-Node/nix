#include "pandora/NamedEntityWithSources.hpp"

namespace pandora {

NamedEntityWithSources::NamedEntityWithSources(File *file, Group group,
    std::string id) :
    NamedEntity(file, group, id) {
}
/* SEE: NamedEntity.hpp */
NamedEntityWithSources::NamedEntityWithSources(File *file, Group group,
    std::string id, time_t time) :
    NamedEntity(file, group, id, time) {
}

size_t NamedEntityWithSources::sourceCount() const {
  size_t count = 0;
  if (group.hasData("sources")) {
    DataSet dataset = group.openData("sources");
    PSize size = dataset.size();
    return size[0];
  }
  return count;
}

bool NamedEntityWithSources::hasSource(std::string source_id) const {
  if (group.hasData("sources")) {
    if (sourceCount() == 0)
      return false;
    std::vector<std::string> s = sources();
    for (size_t i = 0; i < s.size(); i++) {
      if (s[i].compare(source_id) == 0) {
        return true;
      }
    }
  } else {
    return false;
  }
  return false;
}

std::vector<std::string> NamedEntityWithSources::sources() const {
  std::vector<std::string> s;
  if (group.hasData("sources")) {
    DataSet ds = group.openData("sources");
    Selection fileSel = ds.createSelection();
    PSize start = { 0 };
    PSize count = { sourceCount() };
    fileSel.select(count, start);
    ds.read(s, fileSel, true);
  }
  return s;
}

void NamedEntityWithSources::addSource(const Source &source) {
  addSource(source.id());
}

void NamedEntityWithSources::addSource(std::string source_id) {
  std::vector<std::string> vals;
  vals.push_back(source_id);
  PSize start;
  DataSet ds((H5::DataSet()));
  if (group.hasData("sources")) {
    ds = group.openData("sources");
    PSize size = ds.size();
    PSize newSize = size + 1;
    ds.extend(newSize);
    start = size;
  } else {
    Charon<std::vector<std::string> > charon(vals);
    PSize size = { 1 };
    PSize maxsize = { H5S_UNLIMITED };
    PSize chunks = DataSet::guessChunking(size, DataType::Double);
    ds = DataSet::create(group.h5Group(), charon.getFileType(), "sources", size,
        &maxsize, &chunks);
    start = {0};
  }
  Selection fileSel = ds.createSelection();
  PSize count = { 1 };
  fileSel.select(count, start);
  ds.write(vals, fileSel);
}

void NamedEntityWithSources::sources(std::vector<std::string> s){
  group.removeData("sources");
  for(size_t i = 0; i<s.size();i ++){
    addSource(s[i]);
  }
}

NamedEntityWithSources::~NamedEntityWithSources() {
  // TODO Auto-generated destructor stub
}
} //namespace
