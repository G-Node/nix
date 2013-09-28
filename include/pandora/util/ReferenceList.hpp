// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file pandora/util/ReferenceList.hpp
 */

#ifndef PAN_REFERENCE_LIST_H_INCLUDED
#define PAN_REFERENCE_LIST_H_INCLUDED

#include <pandora/Group.hpp>
#include <pandora/PandoraEntity.hpp>

namespace pandora {
namespace util {


class ReferenceList {

  Group group;
  std::string ds_name;

  ReferenceList(const Group &group, const std::string &ds_name);

  bool has(const std::string &id) const;

  inline bool has(const PandoraEntity &entity) const;

  std::vector<std::string> get();

  void set(const std::vector<std::string> &ids);

  inline void set(const std::vector<PandoraEntity> &entities);

  void add(const std::string &id);

  inline void add(const PandoraEntity &entity);

  bool remove(const std::string &id);

  inline bool remove(const PandoraEntity &entity);

};

// implementations of inline methods

bool ReferenceList::has(const PandoraEntity &entity) const {
  return has(entity.id());
}

void ReferenceList::set(const std::vector<PandoraEntity> &entities) {
  std::vector<std::string> ids(entities.size());
  for (size_t i = 0; i < entities.size(); i++) {
    ids[i] = entities[i].id();
  }
  set(ids);
}

void ReferenceList::add(const PandoraEntity &entity) {
  add(entity.id());
}

inline bool ReferenceList::remove(const PandoraEntity &entity) {
  return remove(entity.id());
}

} // namespace util
} // namespace pandora

#endif // PAN_REFERENCE_LIST_H_INCLUDED
