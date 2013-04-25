#ifndef PANDORA_DATASET_H
#define PANDORA_DATASET_H

#include <pandora.hpp>
#include <H5Cpp.h>
#include <pandora/Selection.hpp>

namespace pandora {

class DataSet {
public:
	explicit DataSet(H5::DataSet dset);

	template<typename T> void read(T &value, bool resize = false);
	template<typename T> void read(T &value, const Selection &fileSel, bool resize = false);
	template<typename T> void read(T &value, const Selection &fileSel, const Selection &memSel);

	template<typename T> void write(const T &value) const;
	template<typename T> void write(const T &value, const Selection &fileSel) const;
	template<typename T> void write(const T &value, const Selection &fileSel, const Selection &memSel) const;

	static DataSet create(const H5::CommonFG &parent, const std::string &name, DataType dtype,
		const PSize &size, const PSize *maxsize = nullptr, const PSize *chunks = nullptr);
	static PSize guessChunking(PSize dims, DataType dtype);

	void extend(const PSize &size);
	Selection createSelection() const;

	template<typename T>
	Selection createSelection(const T& value) const;

private:
	H5::DataSet h5dset;
};


/* ************************************************************************* */
/**
 * Read *all* the data from a DataSet into memory
 *
 * @param value    Reference to a variable to store the data in
 * @param resize   Resize variable to fit the size of the DataSet
 */
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

/**
 * Read *selected* data from a DataSet into memory
 *
 * NB: Since this function assumes that whole variable is being read into,
 * the number of selected elements in fileSel and the size of the variable value
 * must be the same
 *
 * @param value    Reference to a variable to store the data in
 * @param fileSel  Selection to indicate which subspace of the DataSet to read
 * @param resize   Resize variable to fit the size of the Selection
 */
template<typename T> void DataSet::read(T &value, const Selection &fileSel, bool resize)
{
	Charon<T> charon(value);

	if (resize) {
		PSize fsize = fileSel.size();
		charon.resize(fsize);
	}

	read(value, fileSel, createSelection(value));
}

/**
 * Read *selected* data from a DataSet into *selected part* of memory
 *
 * @param value    Reference to a variable to store the data in
 * @param fileSel  Selection to indicate which subspace of the DataSet to read
 * @param memSel   Selection to indicate which subspace of the memory to read into
 */
template<typename T> void DataSet::read(T &value, const Selection &fileSel, const Selection &memSel)
{
	typedef Charon<T> charon_type;
	typedef typename charon_type::dbox_type dbox_type;

	charon_type charon(value);
	dbox_type data = charon.get_data();
	h5dset.read(*data, charon.getMemType(), memSel.h5space(), fileSel.h5space());
	const H5::DataSpace space = memSel.h5space();
	data.finish(&space);
}

/* ************************************************************************* */

/**
 * Write all memory stored in the variable value into all of the DataSet
 *
 * NB: Size of the DataSet and the variable must be the same
 * @param value    Reference to a variable to read the data from
 */
template<typename T> void DataSet::write(const T &value) const
{
	typedef Charon<const T> charon_type;
	typedef typename charon_type::dbox_type dbox_type;

	charon_type charon(value);

	dbox_type data = charon.get_data();
	h5dset.write(*data, charon.getMemType());

	data.finish();
}

/**
 * Write all memory stored in the variable value into a *selected* subspace of the DataSet
 *
 * NB: Size of the DataSet and the variable must be the same
 * @param value    Reference to a variable to read the data from
 * @param fileSel    Selection to indicate into which subspace of value to read data from
 */
template<typename T> void DataSet::write(const T &value, const Selection &fileSel) const
{
	write(value, fileSel, createSelection(value));
}

/**
 * Write a *selected part* of memory stored in the variable value into a *selected* subspace of the DataSet
 *
 * NB: Size of the DataSet and the variable must be the same
 * @param value    Reference to a variable to read the data from
 * @param fileSel  Selection to indicate into which subspace of value to read data from
 * @param memSel   Selection to indicate into which subspace of the DataSpace to write to
 */
template<typename T> void DataSet::write(const T &value, const Selection &fileSel, const Selection &memSel) const
{
	typedef Charon<const T> charon_type;
	typedef typename charon_type::dbox_type dbox_type;

	charon_type charon(value);

	dbox_type data = charon.get_data();
	h5dset.write(*data, charon.getMemType(), memSel.h5space(), fileSel.h5space());
	data.finish();
}

template<typename T>
Selection DataSet::createSelection(const T &value) const
{
	typedef Charon<const T> charon_type;
	charon_type charon(value);
	return Selection(charon.createDataSpace(false));
}

}

#endif // PANDORA_DATASET_H