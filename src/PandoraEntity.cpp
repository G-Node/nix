/*
 * PandoraEntity.cpp
 *
 *  Created on: 29.05.2013
 *      Author: stoewer
 */

#include <ctime>

#include <pandora/PandoraEntity.hpp>
#include <pandora/Util.hpp>

using namespace std;

namespace pandora {

/* SEE: PandoraEntity.hpp */
PandoraEntity::PandoraEntity(File *file, Group group, std::string id) :
    file(file), group(group), entity_id(id)
{
  setUpdatedAt();
  setCreatedAt();
}

/* SEE: PandoraEntity.hpp */
PandoraEntity::PandoraEntity(File *file, Group group, std::string id, time_t time) :
    file(file), group(group), entity_id(id)
{
  setUpdatedAt();
  forceCreatedAt(time);
}

/* SEE: PandoraEntity.hpp */
string PandoraEntity::id() const {
  return entity_id;
}

/* SEE: PandoraEntity.hpp */
void PandoraEntity::type(string type) {
  group.setAttr("type", type);
}

/* SEE: PandoraEntity.hpp */
string PandoraEntity::type() const {
  string type;
  group.getAttr("type", type);
  return type;
}

/* SEE: PandoraEntity.hpp */
void PandoraEntity::name(string name) {
  group.setAttr("name", name);
}

/* SEE: PandoraEntity.hpp */
string PandoraEntity::name() const {
  string name;
  group.getAttr("name", name);
  return name;
}

/* SEE: PandoraEntity.hpp */
void PandoraEntity::definition(string definition) {
  group.setAttr("definition", definition);
}

/* SEE: PandoraEntity.hpp */
string PandoraEntity::definition() const {
  string definition;
  group.getAttr("definition", definition);
  return definition;
}

/* SEE: PandoraEntity.hpp */
time_t PandoraEntity::updatedAt() const {
  string t;
  group.getAttr("updated_at", t);
  return util::strToTime(t);
}

/* SEE: PandoraEntity.hpp */
void PandoraEntity::setUpdatedAt() {
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
  return file == file && group == group && entity_id == entity_id;
}

/* SEE: PandoraEntity.hpp */
bool PandoraEntity::operator!=(const PandoraEntity &other) const {
  return !(file == file && group == group && entity_id == entity_id);
}

/* SEE: PandoraEntity.hpp */
int PandoraEntity::compare(const PandoraEntity &other) const {
  int cmp = name().compare(other.name());
  if (cmp == 0) {
    cmp = entity_id.compare(other.entity_id);
  }
  return cmp;
}

/* SEE: PandoraEntity.hpp */
ostream& operator<<(ostream &out, const PandoraEntity &ent) {
  out << "Entity: {name = " << ent.name();
  out << ", type = " << ent.type();
  out << ", id = " << ent.entity_id << "}";
  return out;
}

/* SEE: PandoraEntity.hpp */
PandoraEntity::~PandoraEntity()
{}

} /* namespace pandora */
