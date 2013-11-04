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
#include <pandora/EntityWithMetadata.hpp>

using namespace std;

namespace pandora {

EntityWithMetadata::EntityWithMetadata(File file, Group group, const string &id)
  : NamedEntity(group, id), file(file)
{

}


EntityWithMetadata::EntityWithMetadata(File file, Group group, const string &id, time_t time)
  : NamedEntity(group, id, time), file(file)
{

}

EntityWithMetadata::~EntityWithMetadata() {

}

void EntityWithMetadata::metadata(const Section &metadata){
  if(!this->file.hasSection(metadata.id())){
     throw runtime_error("EntityWithMetadata::metadata: cannot set metadata because Section does not exist in this file!");
   }
   else{
     this->group.setAttr("metadata", metadata.id());
     forceUpdatedAt();
   }
}

Section EntityWithMetadata::metadata() const{
  if(!hasMetadata()){
    throw runtime_error("EntityWithMetadata::metadata: This entity does not reference metadata!");
  }
  std::string sectionId;
  group.getAttr("metadata", sectionId);
  return file.getSection(sectionId);
}

bool EntityWithMetadata::removeMetadata(){
  if(hasMetadata())
    group.removeAttr("metadata");
  return true;
}

bool EntityWithMetadata::hasMetadata() const{
  std::string sectionId;
  group.getAttr("metadata", sectionId);
  return (sectionId.length() > 0);
}

} //namespace
