
#ifndef PANDORA_CHARON_H
#define PANDORA_CHARON_H


#if defined(_WIN32)
#include <cpp/H5Cpp.h>
#else
#include <H5Cpp.h>
#endif
#include <boost/multi_array.hpp>

#include <pandora/PSize.hpp>
#include <pandora/DataType.hpp>

#include <type_traits>

namespace pandora {
namespace hades {

/* ********** */

template<typename T>
struct TypeSpecBase {
  typedef T inner_type;
  static T get_inner(T val) {
    return val;
  }

};

template<typename T>
struct TypeSpec {
  
  static const bool is_valid = false;
  const H5::DataType fileType;
  const H5::DataType memType;
};

//

template<>
struct TypeSpec<char> : public TypeSpecBase<char> {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_I8LE;
  const H5::DataType memType = H5::PredType::NATIVE_CHAR;
};

template<>
struct TypeSpec<int16_t> : public TypeSpecBase<int16_t>  {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_I16LE;
  const H5::DataType memType = H5::PredType::NATIVE_INT16;
};

template<>
struct TypeSpec<uint16_t> : public TypeSpecBase<uint16_t> {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_U16LE;
  const H5::DataType memType = H5::PredType::NATIVE_UINT16;
};

template<>
struct TypeSpec<int32_t> : public TypeSpecBase<int32_t> {
  
  static  const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_I32LE;
  const H5::DataType memType = H5::PredType::NATIVE_INT32;
};

template<>
struct TypeSpec<uint32_t> : public TypeSpecBase<uint32_t> {
  
  static  const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_U32LE;
  const H5::DataType memType = H5::PredType::NATIVE_UINT32;
};

template<>
struct TypeSpec<int64_t> : public TypeSpecBase<int64_t> {
  
  static  const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_I64LE;
  const H5::DataType memType = H5::PredType::NATIVE_INT64;
};

template<>
struct TypeSpec<uint64_t> : public TypeSpecBase<uint64_t> {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_U64LE;
  const H5::DataType memType = H5::PredType::NATIVE_UINT64;
};

template<>
struct TypeSpec<float> : public TypeSpecBase<float> {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::IEEE_F32LE;
  const H5::DataType memType = H5::PredType::NATIVE_FLOAT;
};

template<>
struct TypeSpec<double> : public TypeSpecBase<double> {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::IEEE_F64LE;
  const H5::DataType memType = H5::PredType::NATIVE_DOUBLE;
};

template<>
struct TypeSpec<bool> : public TypeSpecBase<int8_t> {

  static const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_I8LE;
  const H5::DataType memType = H5::PredType::NATIVE_HBOOL;

  static int8_t get_inner(bool b) {
    return b;
  }

};

template<>
struct TypeSpec<std::string> : public TypeSpecBase<char *> {
  
  static const bool is_valid = true;
  const H5::DataType fileType = H5::StrType(H5::PredType::C_S1, H5T_VARIABLE);
  const H5::DataType memType = H5::StrType(H5::PredType::C_S1, H5T_VARIABLE);

  static char* get_inner(const std::string &val) {
    return (char*) val.c_str();
  }

};


template<>
struct TypeSpec<DataType> {

  static const bool is_valid = true;

  TypeSpec(DataType type)
    : fileType(data_type_to_h5_filetype(type)),
      memType(data_type_to_h5_memtype(type))
     { }

  const H5::DataType fileType;
  const H5::DataType memType;
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
  typedef TypeSpec<element_type> spec_type;

  static spec_type type_spec(const T &value) { return spec_type(); };

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
class TypeInfo<std::vector<T> > {
public:
  typedef T element_type;
  typedef std::vector<T> vector_type;
  typedef TypeSpec<element_type> spec_type;

  static spec_type type_spec(const vector_type &value) { return spec_type(); };
  
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
class TypeInfo<boost::multi_array<T, N> > {
public:
  typedef boost::multi_array<T, N>     array_type;
  typedef typename array_type::element element_type;
  typedef TypeSpec<element_type> spec_type;
  
  static spec_type type_spec(const array_type &value) { return spec_type(); };
  
  static PSize shape(const array_type &value) {
    PSize hsize(N);
    const size_t *shape = value.shape();
    std::copy(shape, shape + N, hsize.data());
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
  typedef TypeSpec<element_type> spec_type;

  static spec_type type_spec(const array_type &value) { return spec_type(); };
  
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
  typedef TypeSpec<element_type> spec_type;

  static spec_type type_spec(const array_type &value) { return spec_type(); };
  
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
  
  
/* *** */
template<typename T, typename ElementType, typename isConst = void> class DataBox;
  
/* generic version for non-const types */
template<
  typename T,
  typename ElementType >
class DataBox<T, ElementType, typename std::enable_if<! std::is_const<T>::value>::type> {
public:
  typedef hades::TypeInfo<T>  type_info_t;
  typedef ElementType         data_type;
  typedef data_type          *data_ptr;

  DataBox(T &val) : value(val) {}

  data_ptr operator *() { return get(); }
  data_ptr get() { return type_info_t::getData(value); }
  void finish(const H5::DataSpace *space = nullptr) { }

private:
  T &value;
};

/* generic version for const types */
template<
  typename T,
  typename ElementType
>
class DataBox<T, ElementType, typename std::enable_if<std::is_const<T>::value>::type> {
public:
  typedef typename std::remove_const<T>::type  vanilla;
  typedef hades::TypeInfo<vanilla>             type_info_t;
  typedef const ElementType                    data_type;
  typedef       data_type                     *data_ptr;

  DataBox(T &val) : value(val) {}

  const data_ptr operator *() const { return get(); }
  const data_ptr get() const { return type_info_t::getData(value); }
  void finish(const H5::DataSpace *space = nullptr) const { }

private:
  T &value;
};


/* string version for non-const types */
template<
  typename T
>
class DataBox<T, std::string, typename std::enable_if<! std::is_const<T>::value>::type> {
public:
  typedef hades::TypeInfo<T>  type_info_t;
  typedef char               *data_type;
  typedef data_type          *data_ptr;

  DataBox(T &val) : value(val) {
    size_t nelms = type_info_t::num_elements(value);
    data = new data_type[nelms];
  }

  data_ptr operator *() { return get(); }
  data_ptr get() { return data; }

  void finish(const H5::DataSpace *space = nullptr) {
    size_t nelms = type_info_t::num_elements(value);
    auto vptr = type_info_t::getData(value);

    for (size_t i = 0; i < nelms; i++) {
      vptr[i] = data[i];
    }

    if (space) {
      TypeSpec<std::string> spec;
			H5::DataSet::vlenReclaim(data, spec.memType, *space);
		}
  }

  ~DataBox() {
     delete[] data;
  }

private:
  data_ptr  data;
  T        &value;
};

/* string version for const types */
template<
  typename T
>
  class DataBox<T, std::string, typename std::enable_if<std::is_const<T>::value>::type> {
public:
  typedef typename std::remove_const<T>::type  vanilla;
  typedef hades::TypeInfo<vanilla>             type_info_t;
  typedef char const                          *data_type;
  typedef data_type                           *data_ptr;

  DataBox(T &val) : value(val) {
     size_t nelms = type_info_t::num_elements(value);
     data = new data_type[nelms];
     auto vptr = type_info_t::getData(value);

     for (size_t i = 0; i < nelms; i++) {
      data[i] = vptr[i].c_str();
    }
  }

  data_ptr operator *() const { return get(); }
  data_ptr get() const { return data; }
  void finish(const H5::DataSpace *space = nullptr) const { }

  ~DataBox() {
     delete[] data;
  }

private:
  data_ptr  data;
  T        &value;
};

} //namespace hades
/**/
template<typename T>
class Charon {
  
public:
  
  typedef T &value_ref;
  
  typedef typename std::remove_const<T>::type             vanilla;
  typedef hades::TypeInfo<vanilla>                        type_info_t;
  typedef typename hades::TypeInfo<vanilla>::element_type element_type;
  typedef typename hades::TypeInfo<vanilla>::spec_type    type_spec_t;

  typedef hades::DataBox<T, element_type> dbox_type;
  typedef typename dbox_type::data_ptr    data_ptr;
  

  Charon(value_ref val) : value(val), type_spec(type_info_t::type_spec(val)) {
    static_assert(type_spec_t::is_valid, "No valid type spec");
  }
  
  const H5::DataType& getFileType() const { return type_spec.fileType; }
  const H5::DataType& getMemType() const { return type_spec.memType; }
  
  H5::DataSpace createDataSpace(bool maxdimsUnlimited) const {

    if (maxdimsUnlimited) {
      PSize dims = type_info_t::shape(value);
      PSize maxdims(dims.size(), H5S_UNLIMITED);
      return createDataSpace(&maxdims);
    } else {
      return createDataSpace();
    }
  
  }

  H5::DataSpace createDataSpace(const PSize *maxdims = nullptr) const {
    PSize dims = type_info_t::shape(value);
    H5::DataSpace space;
    
    if (dims.size() == 0) {
      space = H5::DataSpace();
      return space; //no need to delete shape
    }
    
    int rank = (int) dims.size();
    if (maxdims != nullptr) {
      space = H5::DataSpace(rank, dims.data(), maxdims->data());
    } else {
      space = H5::DataSpace(rank, dims.data());
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
  
  dbox_type get_data() const {
    dbox_type data(value);
    return data;
  }

  void resize(const PSize &size) {
    type_info_t::resize(value, size);
  }

private:
  value_ref   value;
  type_spec_t type_spec;
};


}  //namespace pandora

#endif //PANDORA_CHARON_H
