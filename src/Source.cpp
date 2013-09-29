// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file src/Source.cpp
 * @brief Implementation of methods and functions related to the class Source.
 */

#include <pandora/Util.hpp>
#include <pandora/File.hpp>
#include <pandora/Group.hpp>
#include <pandora/Source.hpp>

using namespace std;

namespace pandora {


Source::Source(const Source &source)
: EntityWithMetadata(source.file, source.group, source.entity_id)
{
	source_group = source.source_group;
}


Source::Source(File file, Group group, const std::string &id)
: EntityWithMetadata(file, group, id)
{
	source_group = group.openGroup("sources");
}


Source::Source(File file, Group group, const std::string &id, time_t time)
: EntityWithMetadata(file, group, id, time)
{
	source_group = group.openGroup("sources");
}

  
Source::size_type Source::childCount() const
{
  return source_group.objectCount();
}
  
Source Source::getChild(size_type index) const
{
  string id = source_group.objectName(index);
	return Source(file, source_group.openGroup(id, false), id);
}
  
bool Source::hasSource(const string &id) const {
	return source_group.hasGroup(id);
}


Source Source::getSource(const string &id) const {
	return Source(file, source_group.openGroup(id, false), id);
}


Source Source::getSource(size_t index) const {
	string id = source_group.objectName(index);
	return Source(file, source_group.openGroup(id, false), id);
}

size_t Source::sourceCount() const {
	return source_group.objectCount();
}


std::vector<Source> Source::sources() const {
	return collectIf(predCollectAll, true, 1);
}


Source Source::createSource(const string &name, const string &type) {
	string id = util::createId("source");

	while(source_group.hasObject(id)) {
		id = util::createId("source");
	}

	Source s(file, source_group.openGroup(id, true), id);
	s.name(name);
	s.type(type);

	return s;
}


ostream& operator<<(ostream &out, const Source &ent) {
	out << "Source: {name = " << ent.name();
	out << ", type = " << ent.type();
	out << ", id = " << ent.id() << "}";
	return out;
}


Source& Source::operator=(const Source &other) {
	if (*this != other) {
		this->file = other.file;
		this->group = other.group;
		this->entity_id = other.entity_id;
		this->source_group = other.source_group;
	}
	return *this;
}



Source::~Source() {
	//dtor
}

} // end namespace pandora
