// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <pandora/util/ReferenceList.hpp>
#include <pandora/PSize.hpp>
#include <pandora/Selection.hpp>
#include <pandora/DataSet.hpp>

using namespace std;

namespace pandora {
namespace util {

ReferenceList::ReferenceList(const Group &group, const std::string &ds_name)
  : group(group), ds_name(ds_name)
{}

bool ReferenceList::has(const std::string &id) const {

}

std::vector<std::string> ReferenceList::get() {

}

void ReferenceList::set(const std::vector<std::string> &ids) {

}

void ReferenceList::add(const std::string &id) {

}

bool ReferenceList::remove(const std::string &id) {

}

} // namespace util
} // namespace pandora
