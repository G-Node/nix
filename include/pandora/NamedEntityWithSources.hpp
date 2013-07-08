
#ifndef NAMEDENTITYWITHSOURCES_HPP_
#define NAMEDENTITYWITHSOURCES_HPP_

#include <string>
#include <vector>
#include <iostream>

#include <pandora/NamedEntity.hpp>
#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/DataSet.hpp>
#include <pandora/Charon.hpp>
#include <pandora/Source.hpp>

namespace pandora {
class NamedEntityWithSources: public pandora::NamedEntity {
public:
  NamedEntityWithSources(File *file, Group group, std::string id);

  NamedEntityWithSources(File *file, Group group, std::string id, time_t time);

  size_t sourceCount() const;

  bool hasSource(std::string source_id) const;

  void addSource(const Source &source);

  void addSource(std::string source_id);

  void removeSource(std::string source_id);

  void sources(std::vector<std::string>);

  std::vector<std::string> sources() const;

  virtual ~NamedEntityWithSources();
};

} //of namespace
#endif /* NAMEDENTITYWITHSOURCES_HPP_ */
