// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifndef ZONED_IO_HPP
#define ZONED_IO_HPP

#include <nix/DataArray.hpp>

namespace nix {

class ZonedIO : public DataIO {
public:
    ZonedIO(DataArray da, NDSize count, NDSize offset)
            : array(std::move(da)), offset(std::move(offset)), count(std::move(count)) {

        if (this->offset + this->count > array.dataExtent()) {
            //TODO: fix OOB error's ctor to only optionally take the size_t params
            throw OutOfBounds("Trying to create ZonedIO which is out of bounds", 0);
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


}


#endif