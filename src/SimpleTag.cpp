// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file SimpleTag.cpp
 * @brief Implementations of the class SimpleTag.
 */

#include <pandora/SimpleTag.hpp>

using namespace std;

namespace pandora {


SimpleTag::SimpleTag(const SimpleTag &tag) :
    PandoraEntity(tag.file, tag.group, tag.entity_id), block(tag.block)
{}


SimpleTag::SimpleTag(File *file, const Block block, Group group, std::string id) :
    PandoraEntity(file, group, id), block(block)
{}


string SimpleTag::type() const {
  string type;
  group.getAttr("type", type);
  return type;
}


void SimpleTag::type(const string &type) {
  group.setAttr("type", type);
}


string SimpleTag::name() const {
  string name;
  group.getAttr("name", name);
  return name;
}


void SimpleTag::name(const string &name) {
  group.setAttr("name", name);
}


string SimpleTag::definition() const {
  string definition;
  group.getAttr("definition", definition);
  return definition;
}


void SimpleTag::definition(const string &definition) {
  group.setAttr("definition", definition);
}


vector<Source> SimpleTag::sources() const {
  vector<string> ids;
  vector<Source> sources;

  group.getAttr("sources", ids);

  for (size_t i = 0; i < ids.size(); i++) {
    Source s = block.findSource(ids[i]);
    sources.push_back(s);
  }

  return sources;
}

SimpleTag::~SimpleTag()
{
  // TODO Auto-generated destructor stub
}

} /* namespace pandora */
