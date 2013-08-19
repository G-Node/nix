// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file EntityWithMetadata.cpp
 * @brief Implementation of all methods of the class EntityWithMetadata.
 */

#include <pandora/Util.hpp>
#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/EntityWithMetadata.hpp>

using namespace std;

namespace pandora {

EntityWithMetadata::EntityWithMetadata(File file, Group group, string id)
  : NamedEntity(group, id), file(file)
{

}


EntityWithMetadata::EntityWithMetadata(File file, Group group, string id, time_t time)
  : NamedEntity(group, id, time), file(file)
{

}

EntityWithMetadata::~EntityWithMetadata() {

}

} //namespace
