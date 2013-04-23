#include <iostream>
#include <hdf5.h>
#include <pandora/DataSet.hpp>

namespace pandora {

DataSet::DataSet(H5::DataSet dset)
	: h5dset(dset)
{
}


DataSet DataSet::create(const H5::CommonFG &parent,
	const std::string &name,
	DataType dtype,
	const PSize &size,
	const PSize *maxsize,
	const PSize *chunks)
{
	H5::DataType fileType = data_type_to_h5_filetype(dtype);
	H5::DataSpace space;

	if (size.size() > 0) {
		int rank = static_cast<int>(size.size());
		const hsize_t *maxdims = maxsize != nullptr ? &(*maxsize)[0] : nullptr;
		space = H5::DataSpace(rank, &size[0], maxdims);
	}

	H5::DSetCreatPropList plcreate = H5::DSetCreatPropList::DEFAULT;

	if (chunks != nullptr) {
		int rank = static_cast<int>(chunks->size());
		plcreate.setChunk(rank, &(*chunks)[0]);
	}

	H5::DataSet dset = parent.createDataSet(name, fileType, space);
	return DataSet(dset);
}

} //namespace pandora