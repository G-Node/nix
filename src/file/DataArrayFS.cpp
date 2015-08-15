// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>

#include <nix/file/DataArrayFS.hpp>
#include <nix/hdf5/DataSetHDF5.hpp> // FIXME
#include <nix/file/DimensionFS.hpp>

using namespace std;
using namespace nix::base;
using namespace boost::filesystem;

namespace nix {
namespace file {


DataArrayFS::DataArrayFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const string &loc)
    : EntityWithSourcesFS(file, block, loc),
      dimensions(loc + path::preferred_separator + "dimensions", file->fileMode()) {
}


DataArrayFS::DataArrayFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc,
                         const string &id, const string &type, const string &name)
    : DataArrayFS(file, block, loc, id, type, name, util::getTime()) {
}


DataArrayFS::DataArrayFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc,
                             const string &id, const string &type, const string &name, time_t time)
    : EntityWithSourcesFS(file, block, loc, id, type, name, time),
      dimensions(loc + path::preferred_separator  + name + path::preferred_separator + "dimensions", file->fileMode()) {
}

//--------------------------------------------------
// Element getters and setters
//--------------------------------------------------

boost::optional<std::string> DataArrayFS::label() const {
    boost::optional<std::string> ret;
    string value;
    if (hasAttr("label")) {
        getAttr("label", value);
        ret = value;
    }
    return ret;
}


void DataArrayFS::label(const string &label) {
    if (label.empty()) {
        throw EmptyString("label");
    }
    else {
        setAttr("label", label);
        forceUpdatedAt();
    }
}


void DataArrayFS::label(const none_t t) {
    if (hasAttr("label")) {
        removeAttr("label");
    }
    forceUpdatedAt();
}


boost::optional<std::string> DataArrayFS::unit() const {
    boost::optional<std::string> ret;
    string value;
    if (hasAttr("unit")) {
        getAttr("value", value);
        ret = value;
    }
    return ret;
}


void DataArrayFS::unit(const string &unit) {
    if (unit.empty()) {
        throw EmptyString("unit");
    }
    else {
        setAttr("unit", unit);
        forceUpdatedAt();
    }
}


void DataArrayFS::unit(const none_t t) {
    if (hasAttr("unit")) {
        removeAttr("unit");
    }
    forceUpdatedAt();
}


// TODO use defaults
boost::optional<double> DataArrayFS::expansionOrigin() const {
    boost::optional<double> ret;
    double expansion_origin;
    if (hasAttr("expansion_origin")) {
        getAttr("expansion_origin", expansion_origin);
        ret = expansion_origin;
    }
    return ret;
}


void DataArrayFS::expansionOrigin(double expansion_origin) {
    setAttr("expansion_origin", expansion_origin);
    forceUpdatedAt();
}


void DataArrayFS::expansionOrigin(const none_t t) {
    if (hasAttr("expansion_origin")) {
        removeAttr("expansion_origin");
    }
    forceUpdatedAt();
}

// TODO use defaults
vector<double> DataArrayFS::polynomCoefficients() const {
    vector<double> polynom_coefficients;
    if (hasAttr("polynom_coefficients")) {
        getAttr("polynom_coefficients", polynom_coefficients);
    }
    return polynom_coefficients;
}


void DataArrayFS::polynomCoefficients(const vector<double> &coefficients) {
    setAttr("polynom_coefficients", coefficients);
    forceUpdatedAt();
}


void DataArrayFS::polynomCoefficients(const none_t t) {
    if (hasAttr("polynom_coefficients")) {
        removeAttr("polynom_coefficients");
    }
    forceUpdatedAt();
}

//--------------------------------------------------
// Methods concerning dimensions
//--------------------------------------------------


size_t DataArrayFS::dimensionCount() const {
    return dimensions.subdirCount();
}


shared_ptr<IDimension> DataArrayFS::getDimension(size_t index) const {
    shared_ptr<IDimension> dim;
    path p = dimensions.sub_dir_by_index(index);
    if (!p.empty()) {
        dim = openDimensionFS(p.string(), index, fileMode());
    }
    return dim;
}


std::shared_ptr<base::ISetDimension> DataArrayFS::createSetDimension(size_t index) {
    SetDimensionFS dim(dimensions.location(), index, fileMode());
    return make_shared<SetDimensionFS>(dim);
}


std::shared_ptr<base::IRangeDimension> DataArrayFS::createRangeDimension(size_t index, const std::vector<double> &ticks) {
    RangeDimensionFS dim(dimensions.location(), index, ticks, fileMode());
    return make_shared<RangeDimensionFS>(dim);
}


std::shared_ptr<base::IRangeDimension> DataArrayFS::createAliasRangeDimension() {
    RangeDimensionFS dim(dimensions.location(), 1, *this, fileMode());
    return make_shared<RangeDimensionFS>(dim);
}


std::shared_ptr<base::ISampledDimension> DataArrayFS::createSampledDimension(size_t index, double sampling_interval) {
    SampledDimensionFS dim(dimensions.location(), index, sampling_interval, fileMode());
    return make_shared<SampledDimensionFS>(dim);
}

/*
Group DataArrayFS::createDimensionGroup(size_t index) {
    boost::optional<Group> g = dimension_group(true);

    size_t dim_max = dimensionCount() + 1;
    if (index > dim_max || index <= 0)
        throw new runtime_error("Invalid dimension index: has to be 0 < index <= " + util::numToStr(dim_max));

    string str_id = util::numToStr(index);
    if (g->hasGroup(str_id)) {
        g->removeGroup(str_id);
    }

    return g->openGroup(str_id, true);
}
*/

bool DataArrayFS::deleteDimension(size_t index) {
    bool deleted = false;
    size_t dim_count = dimensionCount();
    string str_id = util::numToStr(index);
    if (dimensions.hasObject(str_id)) {
            deleted = dimensions.removeObjectByNameOrAttribute("entity_id", str_id);
        }
        if (deleted && index < dim_count) {
            for (size_t old_id = index + 1; old_id <= dim_count; old_id++) {
                string str_old_id = util::numToStr(old_id);
                string str_new_id = util::numToStr(old_id - 1);
                dimensions.renameSubdir(str_old_id, str_new_id);
            }
        }
    return deleted;
}

//--------------------------------------------------
// Other methods and functions
//--------------------------------------------------


DataArrayFS::~DataArrayFS() {
}


void DataArrayFS::createData(DataType dtype, const NDSize &size) {
    /*
    if (group().hasData("data")) {
        throw new std::runtime_error("DataArray alread exists"); //TODO: FIXME, better exception
    }

    group().createData("data", dtype, size); //FIXME: check if this 2-step creation is needed
    DataSet ds = group().openData("data");
    */ //FIXME
}

bool DataArrayFS::hasData() const {
    return hasObject("data");
}

void DataArrayFS::write(DataType dtype, const void *data, const NDSize &count, const NDSize &offset) {
    /*
    DataSet ds;

    if (!group().hasData("data")) {
        //FIXME: this case should actually never be possible, replace with exception?
        ds = group().createData("data", dtype, count);
    } else {
        ds = group().openData("data");
    }

    if (offset.size()) {
        Selection fileSel = ds.createSelection();
        fileSel.select(count, offset);
        Selection memSel(DataSpace::create(count, false));

        ds.write(dtype, data, fileSel, memSel);
    } else {
        ds.write(dtype, count, data);
    }
    */ // FIXME
}

void DataArrayFS::read(DataType dtype, void *data, const NDSize &count, const NDSize &offset) const {
    if (!hasObject("data")) {
        return;
    }
    /*
    DataSet ds = group().openData("data");

    if (offset.size()) {
        Selection fileSel = ds.createSelection();
        // if count.size() == 0, i.e. we want to read a scalar,
        // we have to supply something that fileSel can make sense of
        fileSel.select(count ? count : NDSize(offset.size(), 1), offset);
        Selection memSel(DataSpace::create(count, false));

        ds.read(dtype, data, fileSel, memSel);
    } else {
        ds.read(dtype, count, data);
    }
    */
}

NDSize DataArrayFS::dataExtent(void) const {
    if (!hasObject("data")) {
        return NDSize{};
    }
    // FIXME
    return NDSize{};
}

void DataArrayFS::dataExtent(const NDSize &extent) {
    if (!hasObject("data")) {
        throw runtime_error("Data field not found in DataArray!");
    }
    // FIXME
    // DataSet ds = group().openData("data");
    // ds.setExtent(extent);
}

DataType DataArrayFS::dataType(void) const {
    if (!hasObject("data")) {
        return DataType::Nothing;
    }
    // FIXME
    return DataType::Nothing;
}

} // ns nix::file
} // ns nix