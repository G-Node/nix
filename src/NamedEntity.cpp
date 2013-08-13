// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file NamedEntity.cpp
 * @brief Implementations of the class PandoraEntity.
 */
#include <ctime>

#include <pandora/Util.hpp>
#include <pandora/Group.hpp>
#include <pandora/NamedEntity.hpp>

using namespace std;

namespace pandora {


NamedEntity::NamedEntity(Group group, string id)
  : PandoraEntity(group, id)
{
  // nothing to do
}


NamedEntity::NamedEntity(Group group, string id, time_t time)
  : PandoraEntity(group, id, time)
{
  // nothing to do
}


void NamedEntity::type(string type) {
  group.setAttr("type", type);
  forceUpdatedAt();
}


string NamedEntity::type() const {
  string type;
  group.getAttr("type", type);
  return type;
}


void NamedEntity::name(string name) {
  group.setAttr("name", name);
  forceUpdatedAt();
}


string NamedEntity::name() const {
  string name;
  group.getAttr("name", name);
  return name;
}


void NamedEntity::definition(string definition) {
  group.setAttr("definition", definition);
  forceUpdatedAt();
}


string NamedEntity::definition() const {
  string definition;
  group.getAttr("definition", definition);
  return definition;
}


int NamedEntity::compare(const NamedEntity &other) const {
  int cmp = name().compare(other.name());
  if (cmp == 0) {
    cmp = entity_id.compare(other.id());
  }
  return cmp;
}


ostream& operator<<(ostream &out, const NamedEntity &ent) {
  out << "NamedEntity: {name = " << ent.name();
  out << ", type = " << ent.type();
  out << ", id = " << ent.id() << "}";
  return out;
}


NamedEntity::~NamedEntity() {
  // nothing to do
}

} /* namespace pandora */
