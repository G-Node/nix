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

#include <pandora/NamedEntity.hpp>
#include <pandora/Util.hpp>

using namespace std;

namespace pandora {

/* SEE: NamedEntity.hpp */
NamedEntity::NamedEntity(File *file, Group group, std::string id) :
    PandoraEntity(file, group, id)
{
  setUpdatedAt();
  setCreatedAt();
}

/* SEE: NamedEntity.hpp */
NamedEntity::NamedEntity(File *file, Group group, std::string id, time_t time) :
    PandoraEntity(file, group, id, time)
{
  setUpdatedAt();
  forceCreatedAt(time);
}

/* SEE: NamedEntity.hpp */
void NamedEntity::type(string type) {
  group.setAttr("type", type);
  forceUpdatedAt();
}

/* SEE: NamedEntity.hpp */
string NamedEntity::type() const {
  string type;
  group.getAttr("type", type);
  return type;
}

/* SEE: NamedEntity.hpp */
void NamedEntity::name(string name) {
  group.setAttr("name", name);
  forceUpdatedAt();
}

/* SEE: NamedEntity.hpp */
string NamedEntity::name() const {
  string name;
  group.getAttr("name", name);
  return name;
}

/* SEE: NamedEntity.hpp */
void NamedEntity::definition(string definition) {
  group.setAttr("definition", definition);
  forceUpdatedAt();
}

/* SEE: NamedEntity.hpp */
string NamedEntity::definition() const {
  string definition;
  group.getAttr("definition", definition);
  return definition;
}

/* SEE: NamedEntity.hpp */
void NamedEntity::metadata(std::string section_id){
  if(file->hasSection(section_id)){
    group.setAttr("metadata", section_id);
    forceUpdatedAt();
  }
  else{
    throw std::runtime_error("Source::metadata(): You try to link to a metadata section that does not exist!");
  }
}

/* SEE: NamedEntity.hpp */
std::string NamedEntity::metadata() const{
  string section_id;
  group.getAttr("metadata", section_id);
  return section_id;
}

/* SEE: NamedEntity.hpp */
int NamedEntity::compare(const NamedEntity &other) const {
  int cmp = name().compare(other.name());
  if (cmp == 0) {
    cmp = entity_id.compare(other.id());
  }
  return cmp;
}


/* SEE: NamedEntity.hpp */
ostream& operator<<(ostream &out, const NamedEntity &ent) {
  out << "Entity: {name = " << ent.name();
  out << ", type = " << ent.type();
  out << ", id = " << ent.id() << "}";
  return out;
}

/* SEE: NamedEntity.hpp */
NamedEntity::~NamedEntity()
{}

} /* namespace pandora */
