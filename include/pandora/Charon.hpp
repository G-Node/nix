
#ifndef PANDORA_CHARON_H
#define PANDORA_CHARON_H

#include <H5Cpp.h>
#include <boost/multi_array.hpp>

#include <pandora/PSize.hpp>

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

  static PSize shape(const T &value) { return PSize(); }

  static size_t num_elements(const T &value) {
    return 1;
  }
  
  static const element_type* getData(const T &value) {
    return &value;
  }
  
  static element_type* getData(T &value) {
    return &value;
  }
  
  static void resize(T &value, const PSize &dims) {
    if (dims.size() != 0) {
      throw InvalidRankException("Cannot resize scalar");
    }
  }
};
  


template<typename T>
class TypeInfo<std::vector<T>> {
public:
  typedef T element_type;
  typedef std::vector<T> vector_type;

  static PSize shape(const vector_type &value) {
    PSize hsize(1);
    hsize[0] = value.size();
    return hsize;
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

  static void resize(vector_type &value, const PSize &dims) {
    if (dims.size() != 1) {
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

  static PSize shape(const array_type &value) {
    PSize hsize(N);
    const size_t *shape = value.shape();
    std::copy(shape, shape + N, &hsize[0]);
    return hsize;
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
  
  static void resize(array_type &value, const PSize &dims) {
    if (dims.size() != N) {
      throw InvalidRankException("Cannot change rank of multiarray");
    }

    value.resize(dims);
  }
};

  
template<typename T, size_t N>
class TypeInfo<T[N]> {
public:
  typedef T element_type;
  typedef T array_type[N];

  static PSize shape(const array_type &value) {
    PSize hsize(1);
    hsize[0] = N;
    return hsize;
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

  static void resize(array_type &value, const PSize &dims) {
    if (dims.size() != 1 && dims[0] != N) {
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

  static PSize shape(const array_type &value) {
    PSize hsize(2);
    hsize[0] = M;
    hsize[1] = N;
    return hsize;
  }

  static size_t num_elements(const array_type &value) {
    return M*N;
  }
  
  static const element_type* getData(const array_type &value) {
    return value[0];
  }
  
  static element_type* getData(array_type &value) {
    return value[0];
  }

  static void resize(array_type &value, const PSize &dims) {
    if (dims.size() != 2 && dims[0] != M && dims[1] != N) {
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
  PSize       shape() const { return info_type::shape(value); }
  size_t      size() { return this->num_elements(value); }
  
  void        resize(const PSize &size) {info_type::resize (value, size);}
  
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
  PSize       shape() const { return info_type::shape(value); }
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

  data_ptr operator *() { return get(); }
  data_ptr get() { return value.get_data(); }
  void finish(const H5::DataSpace *space = nullptr) { }

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

  data_ptr operator *() { return get(); }
  data_ptr get() const { return value.get_data(); }
  void finish(const H5::DataSpace *space = nullptr) { }

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

  DataBox(vbox_ref val) : value(val) {
    size_t nelms = value.size();
    data = new data_type[nelms];
  }

  data_ptr operator *() { return get(); }
  data_ptr get() { return data; }

  void finish(const H5::DataSpace *space = nullptr) {
    size_t nelms = value.size();
    auto vptr = value.get_data();

    for (size_t i = 0; i < nelms; i++) {
      vptr[i] = data[i];
    }

    if (space) {
			H5::DataSet::vlenReclaim(data, value.memType, *space);
		}
  }

  ~DataBox() {
     delete[] data;
  }

private:
  data_ptr data;
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

  DataBox(vbox_ref val) : value(val) {
     size_t nelms = value.size();
     data = new data_type[nelms];
     auto vptr = value.get_data();

     for (auto i = 0; i < nelms; i++) {
      data[i] = vptr[i].c_str();
    }
  }

  data_ptr operator *() { return get(); }
  data_ptr get() const { return data; }
  void finish(const H5::DataSpace *space = nullptr) { }

  ~DataBox() {
     delete[] data;
  }

private:
  data_ptr data;
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
  

  Charon(value_ref val) : value(val) {
    static_assert(vbox_type::is_valid, "No valid type spec");
  }
  
  const H5::DataType& getFileType() const { return value.fileType; }
  const H5::DataType& getMemType() const { return value.memType; }
  
  H5::DataSpace createDataSpace(bool maxdimsUnlimited) const {
    PSize shape = value.shape();
    H5::DataSpace space;

    if (shape.size() == 0) {
      space = H5::DataSpace();
      return space; //no need to delete shape
    }

    int rank = (int) shape.size();
    if (maxdimsUnlimited) {
      PSize maxdims(shape.size());
      std::fill_n(&maxdims[0], rank, H5S_UNLIMITED);
      space = H5::DataSpace(rank, &shape[0], &maxdims[0]);
    } else {
      space = H5::DataSpace(rank, &shape[0]);
    }

    return space;
  }

  PSize shape() const {
    return value.shape();
  }

  dbox_type get_data() {
    dbox_type data(value);
    return data;
  }

  void resize(const PSize &size) {
    value.resize(size);
  }

private:
  vbox_type  value;
};

#endif //PANDORA_CHARON_H

} //namespace pandora
