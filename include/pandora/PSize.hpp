#ifndef PANDORA_PSIZE_H
#define PANDORA_PSIZE_H

#include <stdexcept>

namespace pandora {

template<typename T>
class PSizeBase {
public:
	typedef T        value_type;
	typedef T       *iterator;
	typedef const T *const_iterator;
	typedef T       &reference;
	typedef const T  const_reference;
	typedef T       *pointer;
	typedef size_t   difference_type;
	typedef size_t   size_type;

	PSizeBase() : rank(0), dims(nullptr) { }

	explicit PSizeBase(size_t _rank) : rank(_rank), dims(nullptr) {
		allocate();
	}

	explicit PSizeBase(size_t _rank, T fillValue) : rank(_rank), dims(nullptr) {
		allocate();
		fill(fillValue);
	}

	//copy
	PSizeBase(const PSizeBase &other) : rank(other.rank), dims(nullptr) {
		allocate();
		std::copy(other.dims, other.dims + rank, dims);
	}

	//move (not tested due to: http://llvm.org/bugs/show_bug.cgi?id=12208)
	PSizeBase(PSizeBase &&other) : rank(other.rank), dims(other.dims) {
		other.dims = nullptr;
		other.rank = 0;
	}

	//copy and move assignment operator (not tested, see above)
	PSizeBase& operator=(PSizeBase other) {
		swap(other);
		return *this;
	}

	T& operator[] (const size_t index) {
		const PSizeBase *this_const = const_cast<const PSizeBase*>(this);
		return const_cast<T&>(this_const->operator[](index));
	}

	const T& operator[] (const size_t index) const {
		if (index > rank) {
			throw std::out_of_range ("Index out of bounds");
		}
		return dims[index];
	}

	void swap(PSizeBase &other) {
		using std::swap;
		swap(dims, other.dims);
		rank = other.rank;
	}

	size_t size() const { return rank; }

	size_t nelms() const {
		size_t product = 1;
		std::for_each(begin(), end(), [&](hsize_t val) {
			product *= val;
		});
		return product;
	}

	void fill(T value) {
		std::fill_n(dims, rank, value);
	}

	~PSizeBase(){
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
			dims = new T[rank];
		}
	}

	size_t   rank;
	T *dims;
};

template<typename T>
PSizeBase<T> operator-(const PSizeBase<T> &lhs, const PSizeBase<T> &rhs)
{
	if(rhs.size() != lhs.size()) {
	  throw std::out_of_range (""); //fixme: use different exception
	}

  PSizeBase<T> result(rhs.size());
  for (size_t i = 0; i < rhs.size(); i++) {
  	result[i] = lhs[i] - rhs[i];
  }

  return result;
}

template<typename T>
PSizeBase<T> operator+(const PSizeBase<T> &lhs, const PSizeBase<T> &rhs)
{
	if(rhs.size() != lhs.size()) {
	  throw std::out_of_range (""); //fixme: use different exception
	}

  PSizeBase<T> result(rhs.size());
  for (size_t i = 0; i < rhs.size(); i++) {
  	result[i] = lhs[i] + rhs[i];
  }

  return result;
}

template<typename T>
PSizeBase<T> operator+(const PSizeBase<T> &lhs, int rhs)
{
  PSizeBase<T> result(lhs.size());
  for (size_t i = 0; i < lhs.size(); i++) {
  	result[i] = lhs[i] + static_cast<T>(rhs);
  }

  return result;
}


template<typename T>
PSizeBase<T> operator+(int lhs, const PSizeBase<T> &rhs)
{
  return operator+(rhs, lhs);
}

typedef PSizeBase<hsize_t>  PSize;
typedef PSizeBase<hssize_t> PSSize;

} //namespace pandroa
#endif // PANDORA_PSIZE_H
