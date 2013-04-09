

#include <H5Cpp.h>
#include <boost/multi_array.hpp>

namespace pandora {


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


//
template<typename T, typename U = T>
class Nyx {
  
public:
  const TypeSpec<U> m;
  T &value;
  
  typedef U base_type;
  
public:
  
  Nyx(T &val) : m(TypeSpec<U>()), value(val) {
    static_assert(TypeSpec<U>::is_valid, "No specialisation fore base type found");
  }
  
  const H5::DataType& getFileType() const { return m.fileType; }
  const H5::DataType& getMemType() const { return m.memType; }
  
  virtual H5::DataSpace getDataSpace() const {
    return H5::DataSpace();
  }
  
  virtual void write(H5::Attribute &attr) {
    attr.write(this->m.memType, getData());
  }
  
  virtual void read(H5::Attribute &attr) {
    attr.read(this->m.memType, getData());
  }
  
  virtual base_type* getData() = 0;
  
  virtual ~Nyx() { }
};



template<typename T>
class Charon : public Nyx<T>
{
  using typename Nyx<T>::base_type;
  
public:
  Charon(T &val) : Nyx<T>(val) {}
  base_type* getData() { return &this->value; }
};


template<>
class Charon<std::string> : public Nyx<std::string>
{
public:
  Charon(std::string &val) : Nyx<std::string>(val) {}
  
  virtual void write(H5::Attribute &attr) {
    attr.write(this->m.memType, this->value);
  }
  
  virtual void read(H5::Attribute &attr) {
    attr.read(this->m.memType, this->value);
  }
  
  base_type* getData() { return &this->value; }
};

  
template<typename T, int N>
class CharonMultiArray :
public Nyx<boost::multi_array<T, N>, T> {
  
protected:
  hsize_t dims[N];
  H5::DataSpace memspace;
  
public:
  typedef boost::multi_array<T, N> array_type;
  using typename Nyx<array_type, T>::base_type;
  
  CharonMultiArray(array_type &value) : Nyx<array_type, T>(value) {
    auto *shape = this->value.shape();
    std::copy(shape, shape + N, dims);
    memspace = H5::DataSpace(N, dims, NULL);
  }
  
  virtual H5::DataSpace getDataSpace() const {
    return memspace;
  }
  
  base_type* getData() { return this->value.data(); }
};

  
template<typename T, int N>
class Charon<boost::multi_array<T, N>> :
public CharonMultiArray<T, N> {
public:
  typedef boost::multi_array<T, N> array_type;
  Charon(array_type &val) : CharonMultiArray<T, N>(val) {}
};

  
template<int N>
class Charon<boost::multi_array<std::string, N>> :
public CharonMultiArray<std::string, N>{
public:
  typedef boost::multi_array<std::string, N> array_type;
  
  Charon(array_type &value) : CharonMultiArray<std::string, N>(value) {}
  
  virtual void write(H5::Attribute &attr) {
    std::string *vptr = this->value.data();
    auto nelms = this->value.num_elements();
    char const* *data = new char const* [nelms];
    
    for (auto i = 0; i < nelms; i++) {
      data[i] = vptr[i].c_str();
      std::cout << i << " " << data[i] << std::endl;
    }
    
    attr.write(this->m.memType, data);
    delete[] data;
  }
  
  virtual void read(H5::Attribute &attr) {
    std::string *vptr = this->value.data();
    auto nelms = this->value.num_elements();
    char **data = new char *[nelms];
    
    attr.read(this->m.memType, data);
    for (auto i = 0; i < nelms; i++) {
      vptr[i] = data[i];
    }
    
    H5::DataSet::vlenReclaim(data, this->m.memType, attr.getSpace());
    delete[] data;
  }
};
  
} //namespace pandora
