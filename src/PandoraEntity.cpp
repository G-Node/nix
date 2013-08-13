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

#include <pandora/Util.hpp>
#include <pandora/Group.hpp>
#include <pandora/PandoraEntity.hpp>

using namespace std;

namespace pandora {


PandoraEntity::PandoraEntity(Group group, string id) :
    group(group), entity_id(id)
{
  setUpdatedAt();
  setCreatedAt();
}


PandoraEntity::PandoraEntity(Group group, string id, time_t time) :
    group(group), entity_id(id)
{
  setUpdatedAt();
  forceCreatedAt(time);
}


string PandoraEntity::id() const {
  return entity_id;
}


time_t PandoraEntity::updatedAt() const {
  string t;
  group.getAttr("updated_at", t);
  return util::strToTime(t);
}


void PandoraEntity::setUpdatedAt() {
  if (!group.hasAttr("updated_at")) {
    time_t t = time(NULL);
    group.setAttr("updated_at", util::timeToStr(t));
  }
}


void PandoraEntity::forceUpdatedAt() {
  time_t t = time(NULL);
  group.setAttr("updated_at", util::timeToStr(t));
}


time_t PandoraEntity::createdAt() const {
  string t;
  group.getAttr("created_at", t);
  return util::strToTime(t);
}


void PandoraEntity::setCreatedAt() {
  if (!group.hasAttr("created_at")) {
    time_t t = time(NULL);
    group.setAttr("created_at", util::timeToStr(t));
  }
}


void PandoraEntity::forceCreatedAt(time_t t) {
  group.setAttr("created_at", util::timeToStr(t));
}


bool PandoraEntity::operator==(const PandoraEntity &other) const {
  return group == group && entity_id == entity_id;
}


bool PandoraEntity::operator!=(const PandoraEntity &other) const {
  return group != group || entity_id != entity_id;
}


int PandoraEntity::compare(const PandoraEntity &other) const {
  int  cmp = entity_id.compare(other.entity_id);
  return cmp;
}


ostream& operator<<(ostream &out, const PandoraEntity &ent) {
  out << "Entity: {id = " << ent.entity_id << "}";
  return out;
}

PandoraEntity::~PandoraEntity() {
  // nothing to do
}

} /* namespace pandora */
