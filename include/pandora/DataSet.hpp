#ifndef PANDORA_DATASET_H
#define PANDORA_DATASET_H

#include <pandora.hpp>
#include <H5Cpp.h>

namespace pandora {

class DataSet {
public:
	explicit DataSet(H5::DataSet dset);

	template<typename T> void read(T &data, bool resize = true);
	template<typename T> void write(const T &data) const;

	static DataSet create(const H5::CommonFG &parent, const std::string &name, DataType dtype,
		const PSize &size, const PSize *maxsize = nullptr, const PSize *chunks = nullptr);

	void extend(const PSize &size);

private:
	H5::DataSet h5dset;
};


template<typename T> void DataSet::read(T &value, bool resize)
{
	typedef Charon<T> charon_type;
	typedef typename charon_type::dbox_type dbox_type;

	charon_type charon(value);

	H5::DataSpace space = h5dset.getSpace();
	if (resize) {
		size_t rank = (size_t) space.getSimpleExtentNdims();
		PSize dims(rank);
		space.getSimpleExtentDims (&dims[0], nullptr);
		charon.resize(dims);
	}

	dbox_type data = charon.get_data();
	h5dset.read(*data, charon.getMemType());
	data.finish(&space);
}

template<typename T> void DataSet::write(const T &value) const
{
	typedef Charon<const T> charon_type;
	typedef typename charon_type::dbox_type dbox_type;

	charon_type charon(value);

	dbox_type data = charon.get_data();
	h5dset.write(*data, charon.getMemType());
	data.finish();
}

}

#endif // PANDORA_DATASET_H