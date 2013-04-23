#ifndef PANDORA_PSIZE_H
#define PANDORA_PSIZE_H

#include <stdexcept>

namespace pandora {

class PSize {
public:
	typedef hsize_t        value_type;
	typedef hsize_t       *iterator;
	typedef const hsize_t *const_iterator;
	typedef hsize_t       &reference;
	typedef const hsize_t  const_reference;
	typedef hsize_t       *pointer;
	typedef size_t         difference_type;
	typedef size_t         size_type;

	PSize() : rank(0), dims(nullptr) { }

	explicit PSize(size_t _rank) : rank(_rank), dims(nullptr) {
		allocate();
	}

	//copy
	PSize(const PSize &other) : rank(other.rank), dims(nullptr) {
		allocate();
		std::copy(other.dims, other.dims + rank, dims);
	}

	//move (not tested due to: http://llvm.org/bugs/show_bug.cgi?id=12208)
	PSize(PSize &&other) : rank(other.rank), dims(other.dims) {
		other.dims = nullptr;
		other.rank = 0;
	}

	//copy and move assignment operator (not tested, see above)
	PSize& operator=(PSize other) {
		swap(other);
		return *this;
	}

	hsize_t& operator[] (const size_t index) {
		const PSize *this_const = const_cast<const PSize*>(this);
		return const_cast<hsize_t&>(this_const->operator[](index));
	}

	const hsize_t& operator[] (const size_t index) const {
		if (index > rank) {
			throw std::out_of_range ("Index out of bounds");
		}
		return dims[index];
	}

	void swap(PSize &other) {
		using std::swap;
		swap(dims, other.dims);
		rank = other.rank;
	}

	size_t size() const { return rank; }

	~PSize(){
		delete[] dims;
	}

	//we are modelling a boost::Collection
	iterator begin() { return dims; }
	iterator end() { return dims + rank; }
	const_iterator begin() const { return dims; }
	const_iterator end() const { return dims + rank; }
	bool empty() const { return rank == 0; }

private:
	void allocate() {
		if (rank > 0) {
			dims = new hsize_t[rank];
		}
	}

	size_t   rank;
	hsize_t *dims;
};

} //namespace pandroa
#endif // PANDORA_PSIZE_H
