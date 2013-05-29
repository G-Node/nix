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

namespace pandora {

/* SEE: SimpleTag.hpp */
SimpleTag::SimpleTag(const SimpleTag &tag) :
    PandoraEntity(tag.file, tag.group, tag.entity_id)
{}

/* SEE: SimpleTag.hpp */
SimpleTag::SimpleTag(File *file, Group group, std::string id) :
    PandoraEntity(file, group, id)
{}

SimpleTag::~SimpleTag()
{
  // TODO Auto-generated destructor stub
}

} /* namespace pandora */
