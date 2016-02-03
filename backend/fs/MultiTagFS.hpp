// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
// Author: Jan Grewe <jan.grewe@g-node.org>

#ifndef NIX_MULTITAG_HPP
#define NIX_MULTITAG_HPP

#include <nix/base/IMultiTag.hpp>
#include "BaseTagFS.hpp"

#include <string>
#include <vector>
#include <memory>

namespace nix {
namespace file {


class MultiTagFS : public BaseTagFS, virtual public base::IMultiTag {

public:


    /**
     * Standard constructor for new MultiTag
     */
    MultiTagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc);

    /**
     * Standard constructor for new MultiTag
     */
    MultiTagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc,
               const std::string &id, const std::string &type, const std::string &name, const DataArray &positions);


    /**
     * Standard constructor for new MultiTag with time
     */
    MultiTagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc,
               const std::string &id, const std::string &type, const std::string &name, const DataArray &positions, time_t time);


    std::shared_ptr<base::IDataArray> positions() const;


    void positions(const std::string &name_or_id);


    bool hasPositions() const;


    std::shared_ptr<base::IDataArray> extents() const;


    void extents(const std::string &name_or_id);


    void extents(const none_t t);


    std::vector<std::string> units() const;


    void units(const std::vector<std::string> &units);


    void units(const none_t t);


    virtual ~MultiTagFS();

private:

    bool checkDimensions(const DataArray &a, const DataArray &b) const;

};


} // namespace file
} // namespace nix

#endif //NIX_MULTITAG_HPP
