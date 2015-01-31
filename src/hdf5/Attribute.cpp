
#include <nix/hdf5/Attribute.hpp>
#include <NDSize.hpp>
#include <nix/hdf5/ExceptionHDF5.hpp>
#include <H5Ipublic.h>

namespace nix {
namespace hdf5 {


void Attribute::read(H5::DataType mem_type, const NDSize &size, void *data) {
    herr_t status = H5Aread(hid, mem_type.getId(), data);
    H5Error::check(status, "Attribute::read(): Could not read data");
}

void Attribute::read(H5::DataType mem_type, const NDSize &size, std::string *data) {
    StringWriter writer(size, data);
    read(mem_type, size, *writer);
    writer.finish();

    hid_t space = getSpace();

    herr_t status = H5Dvlen_reclaim(mem_type.getId(), space, H5P_DEFAULT, *writer);
    H5Error::check(status, "Attribute::read(): Could not reclaim variable length data");

    H5Sclose(space);

}

void Attribute::write(H5::DataType mem_type, const NDSize &size, const void *data) {
    herr_t status = H5Awrite(hid, mem_type.getId(), data);
    H5Error::check(status, "Attribute::write(): Could not write data");
}

void Attribute::write(H5::DataType mem_type, const NDSize &size, const std::string *data) {
    StringReader reader(size, data);
    write(mem_type, size, *reader);
}


hid_t Attribute::getSpace() const {

    hid_t space = H5Aget_space(hid);
    if (space < 0) {
        throw H5Exception("Attribute::getSpace(): Dould not get data space");
    }

    return space;
}


NDSize Attribute::extent() const {
    hid_t space = getSpace();

    //Fixme: duplicated code with DataSet::size()
    int ndims = H5Sget_simple_extent_ndims(space);
    if (ndims < 0) {
        throw H5Exception("DataSet::size(): could not obtain number of dimensions");
    }
    size_t rank = static_cast<size_t>(ndims);
    NDSize dims(rank);
    int res = H5Sget_simple_extent_dims(space, dims.data(), nullptr);

    if (res < 0) {
        throw H5Exception("DataSet::size(): could not obtain extents");
    }

    H5Sclose(space);

    return dims;
}
} //nix::hdf5::
} //nix::