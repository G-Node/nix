// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file PandoraEntity.cpp
 * @brief Implementations of the class PandoraEntity.
 */
#include <ctime>

#include <pandora/PandoraEntity.hpp>
#include <pandora/Util.hpp>

using namespace std;

namespace pandora {

/* SEE: PandoraEntity.hpp */
PandoraEntity::PandoraEntity(Group group, std::string id) :
    group(group), entity_id(id)
{
  setUpdatedAt();
  setCreatedAt();
}

/* SEE: PandoraEntity.hpp */
PandoraEntity::PandoraEntity(Group group, std::string id, time_t time) :
    group(group), entity_id(id)
{
  setUpdatedAt();
  forceCreatedAt(time);
}

/* SEE: PandoraEntity.hpp */
string PandoraEntity::id() const {
  return entity_id;
}

/* SEE: PandoraEntity.hpp */
time_t PandoraEntity::updatedAt() const {
  string t;
  group.getAttr("updated_at", t);
  return util::strToTime(t);
}

/* SEE: PandoraEntity.hpp */
void PandoraEntity::setUpdatedAt() {
  if (!group.hasAttr("updated_at")) {
    time_t t = time(NULL);
    group.setAttr("updated_at", util::timeToStr(t));
  }
}

/* SEE: PandoraEntity.hpp */
void PandoraEntity::forceUpdatedAt() {
  time_t t = time(NULL);
  group.setAttr("updated_at", util::timeToStr(t));
}

/* SEE: PandoraEntity.hpp */
time_t PandoraEntity::createdAt() const {
  string t;
  group.getAttr("created_at", t);
  return util::strToTime(t);
}

/* SEE: PandoraEntity.hpp */
void PandoraEntity::setCreatedAt() {
  if (!group.hasAttr("created_at")) {
    time_t t = time(NULL);
    group.setAttr("created_at", util::timeToStr(t));
  }
}

/* SEE: PandoraEntity.hpp */
void PandoraEntity::forceCreatedAt(time_t t) {
  group.setAttr("created_at", util::timeToStr(t));
}


/* SEE: PandoraEntity.hpp */
bool PandoraEntity::operator==(const PandoraEntity &other) const {
  return group == group && entity_id == entity_id;
}

/* SEE: PandoraEntity.hpp */
bool PandoraEntity::operator!=(const PandoraEntity &other) const {
  return group != group || entity_id != entity_id;
}

/* SEE: PandoraEntity.hpp */
int PandoraEntity::compare(const PandoraEntity &other) const {
  int  cmp = entity_id.compare(other.entity_id);
  return cmp;
}

/* SEE: PandoraEntity.hpp */
ostream& operator<<(ostream &out, const PandoraEntity &ent) {
  out << "Entity: {id = " << ent.entity_id << "}";
  return out;
}

/* SEE: PandoraEntity.hpp */
PandoraEntity::~PandoraEntity()
{}

} /* namespace pandora */
