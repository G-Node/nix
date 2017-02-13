// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>

#include "DataArrayFS.hpp"
#include "hdf5/h5x/H5DataSet.hpp" // FIXME
#include "DimensionFS.hpp"

namespace nix {
namespace file {

DataArrayFS::DataArrayFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                         const bfs::path &loc)
    : EntityWithSourcesFS(file, block, loc),
      dimensions(bfs::path(loc) / bfs::path("dimensions"), file->fileMode()) {
}

DataArrayFS::DataArrayFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
	const std::string &loc) : DataArrayFS(file, block, bfs::path(loc)){
}


DataArrayFS::DataArrayFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                         const bfs::path &loc, const std::string &id, const std::string &type, const std::string &name)
    : DataArrayFS(file, block, loc, id, type, name, util::getTime()) {
}

DataArrayFS::DataArrayFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
	const std::string &loc, const std::string &id, const std::string &type, const std::string &name) : DataArrayFS(file, block, bfs::path(loc), id, type, name) {
}


DataArrayFS::DataArrayFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                         const std::string &loc, const std::string &id, const std::string &type,
                         const std::string &name, time_t time)
    : DataArrayFS(file, block, bfs::path(loc), id, type, name, time) {
}

DataArrayFS::DataArrayFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
	const bfs::path &loc, const std::string &id, const std::string &type,
	const std::string &name, time_t time)
	: EntityWithSourcesFS(file, block, loc, id, type, name, time),
	dimensions(bfs::path(loc) / bfs::path(name + "/" + "dimensions"), file->fileMode()) {
}

//--------------------------------------------------
// Element getters and setters
//--------------------------------------------------

boost::optional<std::string> DataArrayFS::label() const {
    boost::optional<std::string> ret;
    std::string value;
    if (hasAttr("label")) {
        getAttr("label", value);
        ret = value;
    }
    return ret;
}


void DataArrayFS::label(const std::string &label) {
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
    std::string value;
    if (hasAttr("unit")) {
        getAttr("unit", value);
        ret = value;
    }
    return ret;
}


void DataArrayFS::unit(const std::string &unit) {
    if (unit.empty()) { // TODO maybe switch to unsetting
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
std::vector<double> DataArrayFS::polynomCoefficients() const {
    std::vector<double> polynom_coefficients;
    if (hasAttr("polynom_coefficients")) {
        getAttr("polynom_coefficients", polynom_coefficients);
    }
    return polynom_coefficients;
}


void DataArrayFS::polynomCoefficients(const std::vector<double> &coefficients) {
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


ndsize_t DataArrayFS::dimensionCount() const {
    return dimensions.subdirCount();
}


std::shared_ptr<base::IDimension> DataArrayFS::getDimension(ndsize_t index) const {
    std::shared_ptr<base::IDimension> dim;
    boost::optional<bfs::path> p = dimensions.findByNameOrAttribute("index", util::numToStr(index));
    if (p) {
        dim = openDimensionFS(p->string(), fileMode());
    }
    return dim;
}


std::shared_ptr<base::ISetDimension> DataArrayFS::createSetDimension(ndsize_t index) {
    SetDimensionFS dim(dimensions.location(), index, fileMode());
    return std::make_shared<SetDimensionFS>(dim);
}


std::shared_ptr<base::IRangeDimension> DataArrayFS::createRangeDimension(ndsize_t index, const std::vector<double> &ticks) {
    RangeDimensionFS dim(dimensions.location(), index, ticks, fileMode());
    return std::make_shared<RangeDimensionFS>(dim);
}


std::shared_ptr<base::IRangeDimension> DataArrayFS::createAliasRangeDimension() {
    RangeDimensionFS dim(dimensions.location(), 1, *this, fileMode());
    return std::make_shared<RangeDimensionFS>(dim);
}


std::shared_ptr<base::ISampledDimension> DataArrayFS::createSampledDimension(ndsize_t index, double sampling_interval) {
    SampledDimensionFS dim(dimensions.location(), index, sampling_interval, fileMode());
    return std::make_shared<SampledDimensionFS>(dim);
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

bool DataArrayFS::deleteDimensions() {
    if (fileMode() != FileMode::ReadOnly) {
        this->dimensions.removeAll();
        return true;
    } else {
        return false;
    }
}


//--------------------------------------------------
// Other methods and functions
//--------------------------------------------------


DataArrayFS::~DataArrayFS() {
}


void DataArrayFS::createData(DataType dtype, const NDSize &size) {
    setDtype(dtype);
    dataExtent(size);
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
    if (!hasAttr("extent")) {
        return NDSize{};
    }
    std::vector<int> ext;
    getAttr("extent", ext);
    NDSize extent(ext.size());
    for (ndsize_t i = 0; i < ext.size(); i++) {
        extent[i] = ext[i];
    }
    return extent;
}

void DataArrayFS::dataExtent(const NDSize &extent) {
    std::vector<int> ext;
    for (ndsize_t i = 0; i < extent.size(); i++) {
        ext.push_back(extent[i]);
    }
    if (hasAttr("extent")){
        removeAttr("extent");
    }
    setAttr("extent", ext);
}

DataType DataArrayFS::dataType(void) const {
    if (!hasAttr("dtype")) {
        return DataType::Nothing;
    }
    std::string dtype;
    getAttr("dtype", dtype);
    return nix::string_to_data_type(dtype);
}


void DataArrayFS::setDtype(nix::DataType dtype) {
    if (hasAttr("dtype")) {
        removeAttr("dtype");
    }
    setAttr("dtype", nix::data_type_to_string(dtype));
}

} // ns nix::file
} // ns nix