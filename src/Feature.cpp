// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/Feature.hpp>
#include <nix/util/util.hpp>

namespace nix {

void Feature::data(const DataArray &data) {
    util::checkEntityInput(data);
    if (!data.isValidEntity()) {
        throw UninitializedEntity();
    }
    backend()->data(data.id());
}

void Feature::data(const DataFrame &data) {
    util::checkEntityInput(data);
    if (!data.isValidEntity()) {
        throw UninitializedEntity();
    }
    backend()->data(data.id());
}

void Feature::data(const std::string &name_or_id) {
    util::checkNameOrId(name_or_id);
    backend()->data(name_or_id);
}

std::string link_type_to_string(LinkType ltype) {

    std::string str;

    switch(ltype) {

    case LinkType::Tagged:    str = "Tagged";    break;
    case LinkType::Untagged:  str = "Untagged";  break;
    case LinkType::Indexed:   str = "Indexed";   break;
    
    }

    return str;
}

std::ostream &operator<<(std::ostream &out, const LinkType ltype) {
    out << "LinkType::" << link_type_to_string(ltype);
    return out;
}

std::string target_type_to_string(TargetType ttype) {
    std::string str;

    switch(ttype) {
    case TargetType::DataArray:    str = "DataArray";  break;
    case TargetType::DataFrame:    str = "DataFrame";  break;
    }
    return str;
} 

std::ostream &operator<<(std::ostream &out, const TargetType ttype) {
    out << "TargetType::" << target_type_to_string(ttype);
    return out;
}

}
