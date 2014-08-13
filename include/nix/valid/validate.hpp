// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_VALIDATE_H
#define NIX_VALIDATE_H

#include <nix/Platform.hpp>
#include <nix/valid/result.hpp>
#include <cstdarg>

#include <nix/types.hpp>

namespace nix {

namespace valid {

/**
  * @brief Block entity validator
  * 
  * Function taking a Block entity and returning {@link Result} object
  *
  * @param block Block entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const Block &block);

/**
  * @brief DataArray entity validator
  * 
  * Function taking a DataArray entity and returning {@link Result}
  * object
  *
  * @param data_array DataArray entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const DataArray &data_array);

/**
  * @brief Tag entity validator
  * 
  * Function taking a Tag entity and returning {@link Result}
  * object
  *
  * @param tag Tag entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const Tag &tag);

/**
  * @brief Property entity validator
  * 
  * Function taking a Property entity and returning {@link Result}
  * object
  *
  * @param property Property entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const Property &property);

/**
  * @brief MultiTag entity validator
  * 
  * Function taking a MultiTag entity and returning {@link Result} object
  *
  * @param data_tag MultiTag entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const MultiTag &data_tag);

/**
  * @brief Dimension entity validator
  * 
  * Function taking a Dimension entity and returning {@link Result}
  * object
  *
  * @param dim Dimension entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const Dimension &dim);

/**
  * @brief RangeDimension entity validator
  * 
  * Function taking a RangeDimension entity and returning {@link Result}
  * object
  *
  * @param range_dim RangeDimension entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const RangeDimension &range_dim);

/**
  * @brief SampledDimension entity validator
  * 
  * Function taking a SampledDimension entity and returning
  * {@link Result} object
  *
  * @param sampled_dim SampledDimension entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const SampledDimension &sampled_dim);

/**
  * @brief SetDimension entity validator
  * 
  * Function taking a SetDimension entity and returning {@link Result}
  * object
  *
  * @param set_dim SetDimension entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const SetDimension &set_dim);

/**
  * @brief Feature entity validator
  * 
  * Function taking a Feature entity and returning {@link Result} object
  *
  * @param feature Feature entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const Feature &feature);

/**
  * @brief Section entity validator
  * 
  * Function taking a Section entity and returning {@link Result} object
  *
  * @param section Section entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const Section &section);

/**
  * @brief Source entity validator
  * 
  * Function taking a Source entity and returning {@link Result} object
  *
  * @param source Source entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const Source &source);

/**
  * @brief File entity validator
  * 
  * Function taking a File entity and returning {@link Result} object
  *
  * @param file File entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const File &file);

} // namespace valid
} // namespace nix

#endif // NIX_VALIDATE_H
