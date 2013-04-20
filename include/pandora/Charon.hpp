
#ifndef PANDORA_CHARON_H
#define PANDORA_CHARON_H

#include <H5Cpp.h>
#include <boost/multi_array.hpp>

namespace pandora {
namespace hades {

/* ********** */
template<typename T>
struct TypeSpec {
  
  static const bool is_valid = false;
  const H5::DataType fileType;
  const H5::DataType memType;
};

//

template<>
struct TypeSpec<char> {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_I8LE;
  const H5::DataType memType = H5::PredType::NATIVE_CHAR;
};

template<>
struct TypeSpec<int16_t> {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_I16LE;
  const H5::DataType memType = H5::PredType::NATIVE_INT16;
};

template<>
struct TypeSpec<uint16_t> {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_U16LE;
  const H5::DataType memType = H5::PredType::NATIVE_UINT16;
};

template<>
struct TypeSpec<int32_t> {
  
  static  const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_I32LE;
  const H5::DataType memType = H5::PredType::NATIVE_INT32;
};

template<>
struct TypeSpec<uint32_t> {
  
  static  const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_U32LE;
  const H5::DataType memType = H5::PredType::NATIVE_UINT32;
};

template<>
struct TypeSpec<int64_t> {
  
  static  const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_I64LE;
  const H5::DataType memType = H5::PredType::NATIVE_INT64;
};

template<>
struct TypeSpec<uint64_t> {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_U64LE;
  const H5::DataType memType = H5::PredType::NATIVE_UINT64;
};

template<>
struct TypeSpec<float> {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::IEEE_F32LE;
  const H5::DataType memType = H5::PredType::NATIVE_FLOAT;
};

template<>
struct TypeSpec<double> {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::IEEE_F64LE;
  const H5::DataType memType = H5::PredType::NATIVE_DOUBLE;
};

template<>
struct TypeSpec<std::string> {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::StrType(H5::PredType::C_S1, H5T_VARIABLE);
  const H5::DataType memType = H5::StrType(H5::PredType::C_S1, H5T_VARIABLE);
};

/* ********** */

class InvalidRankException : public std::runtime_error {
public:
  InvalidRankException(const std::string &message)
    : std::runtime_error(message) { }
};

/* ** */
template<typename T>
class TypeInfo {
public:
  typedef T element_type;
  
  static hsize_t* shape(const T &value, size_t &rank) {
    rank = 0;
    return nullptr;
  }
  
  static size_t num_elements(const T &value) {
    return 1;
  }
  
  static const element_type* getData(const T &value) {
    return &value;
  }
  
  static element_type* getData(T &value) {
    return &value;
  }
  
  static void resize(T &value, size_t rank, hsize_t *dims) {
    if (rank != 0) {
      throw InvalidRankException("Cannot resize scalar");
    }
  }
};
  


template<typename T>
class TypeInfo<std::vector<T>> {
public:
  typedef T element_type;
  typedef std::vector<T> vector_type;
  
  static hsize_t *shape(const vector_type &value, size_t &rank) {
    rank = 1;
    hsize_t *dims = new hsize_t[1];
    dims[0] = value.size();
    return dims;
  }
  
  static size_t num_elements(const vector_type &value) {
    return value.size();
  }
  
  static const element_type* getData(const vector_type &value) {
    return &value[0];
  }
  
  static element_type* getData(vector_type &value) {
    return &value[0];
  }
  
  static void resize(vector_type &value, size_t rank, hsize_t *dims) {
    if (rank != 1) {
      throw InvalidRankException("Cannot change rank of vector"); //FIXME
    }
    
    if (dims[0] == value.size())
      return;
    
    value.resize(dims[0]);
  }
};

template<typename T, size_t N>
class TypeInfo<boost::multi_array<T, N>> {
public:
  typedef boost::multi_array<T, N>     array_type;
  typedef typename array_type::element element_type;
  
  static hsize_t *shape(const array_type &value, size_t &rank) {
    rank = N;
    hsize_t *dims = new hsize_t[N];
    const size_t *shape = value.shape();
    std::copy(shape, shape + N, dims);
    return dims;
  }
  
  static size_t num_elements(const array_type &value) {
    return value.num_elements();
  }
  
  static const element_type* getData(const array_type &value) {
    return value.data();
  }
  
  static element_type* getData(array_type &value) {
    return value.data();
  }
  
  static void resize(array_type &value, size_t rank, hsize_t *dims) {
    if (rank != N) {
      throw InvalidRankException("Cannot change rank of multiarray");
    }
    
    std::vector<std::size_t> extend;
    extend.resize(N);
    std::copy(dims, dims+rank, extend.begin());
    value.resize(extend);
  }
};

  
template<typename T, size_t N>
class TypeInfo<T[N]> {
public:
  typedef T element_type;
  typedef T array_type[N];
  
  static hsize_t *shape(const array_type &value, size_t &rank) {
    rank = 1;
    hsize_t *dims = new hsize_t[1];
    dims[0] = N;
    return dims;
  }
  
  static size_t num_elements(const array_type&value) {
    return N;
  }
  
  static const element_type* getData(const array_type &value) {
    return value;
  }
  
  static element_type* getData(array_type &value) {
    return value;
  }
  
  static void resize(array_type &value, size_t rank, hsize_t *dims) {
    if (rank != 1 && dims[0] != N) {
      throw InvalidRankException("Cannot resize native arrays");
    }
    //NOOP
  }
};
  
  
template<typename T, size_t N, size_t M>
class TypeInfo<T[M][N]> {
public:
  typedef T element_type;
  typedef T array_type[M][N];
  
  static hsize_t *shape(const array_type &value, size_t &rank) {
    rank = 2;
    hsize_t *dims = new hsize_t[2];
    dims[0] = M;
    dims[1] = N;
    return dims;
  }
  
  static size_t num_elements(const array_type&value) {
    return M*N;
  }
  
  static const element_type* getData(const array_type &value) {
    return value[0];
  }
  
  static element_type* getData(array_type &value) {
    return value[0];
  }
  
  static void resize(array_type &value, size_t rank, hsize_t *dims) {
    if (rank != 2 && dims[0] != M && dims[1] != N) {
      throw InvalidRankException("Cannot resize native arrays");
    }
    //NOOP
  }
};

  
template<typename T>
class ValueBox : private TypeInfo<T>,
    public TypeSpec<typename TypeInfo<T>::element_type> {
  
public:
  typedef TypeInfo<T> info_type;
  typedef T           value_type;
  typedef T          &value_ref;
  typedef typename info_type::element_type  element;
  typedef element *element_ptr;
  
  ValueBox(value_ref val) : value(val) {}
  
  element_ptr get_data() { return info_type::getData(value); }
  value_ref   get() { return value; }
  hsize_t *   shape(size_t &rank) const { return info_type::shape(value, rank); }
  size_t      size() { return this->num_elements(value); }
  
  void        resize(size_t rank, hsize_t *dims) {info_type::resize (value, rank, dims);}
  
private:
  value_ref value;
};


template<typename T>
class ValueBox<const T> : private TypeInfo<T>,
    public TypeSpec<typename TypeInfo<T>::element_type> {
  
public:
  typedef TypeInfo<T>  info_type;
  typedef const T      value_type;
  typedef const T     &value_ref;
  typedef typename info_type::element_type element;
  typedef typename info_type::element_type element_type;
  typedef const element *element_ptr;
  
  ValueBox(value_ref val) : value(val) {}
  
  element_ptr get_data() const { return info_type::getData(value); }
  value_ref   get() const { return value; }
  hsize_t *   shape(size_t &rank) const { return info_type::shape(value, rank); }
  size_t      size() { return this->num_elements(value); }
  
private:
  value_ref value;
};


/* *** */
template<
  typename T,
  template <typename> class ValueBox,
  typename ElementType
>
class DataBox {
public:
  typedef ValueBox<T>  &vbox_ref;
  typedef ElementType  data_type;
  typedef data_type   *data_ptr;
  
  DataBox(vbox_ref val) : value(val) {}
  
  data_type* get() { return value.get_data(); }
  void finish(data_ptr data, const H5::DataSpace *space) { }
  
private:
  vbox_ref value;
};

template<
  typename T,
  template <typename> class ValueBox,
  typename ElementType
>
class DataBox<const T, ValueBox, ElementType> {
public:
  typedef ValueBox<const T> &vbox_ref;
  typedef const ElementType  data_type;
  typedef       data_type   *data_ptr;
  
  DataBox(vbox_ref val) : value(val) {}
  
  data_type* get() const { return value.get_data(); }
  void finish(data_ptr data, const H5::DataSpace *space) { }
  
private:
  vbox_ref value;
};


template<
  typename T,
  template <typename> class ValueBox
>
class DataBox<T, ValueBox, std::string> {
public:
  typedef ValueBox<T>  &vbox_ref;
  typedef char        *data_type;
  typedef data_type   *data_ptr;
  
  DataBox(vbox_ref val) : value(val) {}
  
  data_type* get() {
    size_t nelms = value.size();
    data_ptr data = new data_type[nelms];
    return data;
  }
  
  void finish(data_ptr data, const H5::DataSpace *space) {
    size_t nelms = value.size();
    auto vptr = value.get_data();
    
    for (size_t i = 0; i < nelms; i++) {
      vptr[i] = data[i];
    }
    
    if (space) {
			H5::DataSet::vlenReclaim(data, value.memType, *space);
		}
    
    delete[] data;
  }
  
private:
  vbox_ref value;
};

template<
  typename T,
  template <typename> class ValueBox
>
class DataBox<const T, ValueBox, std::string> {
public:
  typedef ValueBox<const T> &vbox_ref;
  typedef char const       *data_type;
  typedef data_type        *data_ptr;
  
  DataBox(vbox_ref val) : value(val) {}
  
  data_type* get() const {
    size_t nelms = value.size();
    data_ptr data = new data_type[nelms];
    auto vptr = value.get_data();
    
    for (auto i = 0; i < nelms; i++) {
      data[i] = vptr[i].c_str();
    }
    
    return data;
  }
  
  void finish(data_ptr data, const H5::DataSpace *space) {
    delete[] data;
  }
  
private:
  vbox_ref value;
};

} //namespace hades
/**/
template<typename T>
class Charon {
  
public:
  typedef hades::ValueBox<T> vbox_type;
  typedef typename vbox_type::element       element_type;
  typedef typename vbox_type::value_ref     value_ref;
  
  typedef hades::DataBox<T, hades::ValueBox, element_type> dbox_type;
  typedef typename dbox_type::data_ptr       data_ptr;
  

  Charon(value_ref val) : value(val), data(value) {
    static_assert(vbox_type::is_valid, "No valid type spec");
  }
  
  const H5::DataType& getFileType() const { return value.fileType; }
  const H5::DataType& getMemType() const { return value.memType; }
  
  H5::DataSpace createDataSpace(bool maxdimsUnlimited) const {
    hsize_t *shape;
    size_t   rank;
    shape = value.shape(rank);
    H5::DataSpace space;
    
    if (rank == 0) {
      space = H5::DataSpace();
      return space; //no need to delete shape
    }
    
    if (maxdimsUnlimited) {
      hsize_t *maxdims = new hsize_t[rank];
      std::fill_n(maxdims, rank, H5S_UNLIMITED);
      space = H5::DataSpace((int) rank, shape, maxdims);
      delete[] maxdims;
    } else {
      space = H5::DataSpace((int) rank, shape);
    }
    
    delete[] shape;
    return space;
  }
  
  hsize_t* shape(size_t &rank) const {
    return value.shape(rank);
  }
  
  data_ptr get() {
    return data.get();
  }
  
  void finish(data_ptr val, const H5::DataSpace *space = nullptr) {
    data.finish(val, space);
  }
  
  void resize(size_t rank, hsize_t *dims) {
    value.resize(rank, dims);
  }
  
private:
  vbox_type  value;
  dbox_type  data;
};

#endif //PANDORA_CHARON_H

} //namespace pandora
