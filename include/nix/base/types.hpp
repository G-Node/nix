// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASE_TYPES_H
#define NIX_BASE_TYPES_H

#include <nix/base/Entity.hpp>
#include <nix/base/NamedEntity.hpp>
#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/EntityWithSources.hpp>
#include <nix/base/IProperty.hpp>
#include <nix/base/IFeature.hpp>
#include <nix/base/ISection.hpp>
#include <nix/base/ISource.hpp>
#include <nix/base/IBlock.hpp>
#include <nix/base/ITag.hpp>
#include <nix/base/IMultiTag.hpp>
#include <nix/base/IDataArray.hpp>

namespace nix {
namespace base {

template class Entity<base::IProperty>;
template class Entity<base::IFeature>;
template class NamedEntity<base::ISection>;
template class EntityWithMetadata<base::ISource>;
template class EntityWithMetadata<base::IBlock>;
template class EntityWithSources<base::ITag>;
template class EntityWithSources<base::IMultiTag>;
template class EntityWithSources<base::IDataArray>;

}
}

#endif
