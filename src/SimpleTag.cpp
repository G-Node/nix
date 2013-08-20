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

#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/Block.hpp>
#include <pandora/SimpleTag.hpp>

using namespace std;

namespace pandora {


SimpleTag::SimpleTag(const SimpleTag &tag)
  : EntityWithSources(tag.file, tag.block, tag.group, tag.entity_id)
{}


SimpleTag::SimpleTag(File file, const Block block, Group group, std::string id)
  : EntityWithSources(file, block, group, id)
{}


SimpleTag::SimpleTag(File file, const Block block, Group group, std::string id, time_t time)
  : EntityWithSources(file, block, group, id, time)
{}



SimpleTag::~SimpleTag()
{
  // TODO Auto-generated destructor stub
}


} /* namespace pandora */
