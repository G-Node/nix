// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifndef DATA_VIEW_HPP
#define DATA_VIEW_HPP

#include <nix/DataArray.hpp>

namespace nix {

class DataView : public DataSet {
public:
    DataView(DataArray da, NDSize count, NDSize offset)
            : array(std::move(da)), offset(std::move(offset)), count(std::move(count)) {

        if (this->offset + this->count > array.dataExtent()) {
            throw OutOfBounds("Trying to create DataView which is out of bounds");
        }
    }

    // the DataIO interface implementation
    virtual void dataExtent(const NDSize &extent);
    virtual NDSize dataExtent() const;

protected:
    void ioRead(DataType dtype,
                void *data,
                const NDSize &count,
                const NDSize &offset) const override;

    void ioWrite(DataType dtype,
                 const void *data,
                 const NDSize &count,
                 const NDSize &offset) override;

private:
    NDSize transform_coordinates(const NDSize &c, const NDSize &o) const;

private:
    DataArray array;
    NDSize    offset;
    NDSize    count;

};

} // nix::

#endif // DATA_VIEW_HPP