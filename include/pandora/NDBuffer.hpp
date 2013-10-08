// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
// Copyright (c) 2013, Christian Kellner <kellner@bio.lmu.de>
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef PANDORA_NDBUFFER_H
#define PANDORA_NDBUFFER_H

#include <pandora/DataType.hpp>
#include <pandora/PSize.hpp>
#include <pandora/Charon.hpp>

#include <vector>
#include <iostream>

namespace pandora {


class NDBuffer {
public:
  typedef uint8_t byte_type;

  NDBuffer(DataType dtype, PSize dims);

  size_t rank() const { return extends.size(); }
  size_t num_elements() const { return extends.nelms(); }
  PSize  shape() const { return extends; }
  PSize  size()const { return extends; }
  DataType dtype() const { return dataType ;}

  template<typename T> const T get(size_t index) const;
  template<typename T> const T get(const PSize &index) const;
  template<typename T> void set(size_t index, T value);
  template<typename T> void set(const PSize &index, T value);
  
  byte_type *data() { return &dstore[0]; }
  const byte_type *data() const { return &dstore[0]; }

  void resize(const PSize &new_size);
  
  size_t sub2index(const PSize &sub) const;

private:
  DataType  dataType;
  void allocate_space();
  void calc_strides();

  PSize                  extends;
  PSize                  strides;
  std::vector<byte_type> dstore;

};

/* ******************************************* */
template<typename T>
const T NDBuffer::get(size_t index) const
{
  const T *dx = reinterpret_cast<const T *>(&dstore[0]);
  return dx[index];
}
  
template<typename T>
const T NDBuffer::get(const PSize &index) const
{
  size_t pos = sub2index(index);
  const T *dx = reinterpret_cast<const T *>(&dstore[0]);
  return dx[pos];
}

  
template<typename T>
void NDBuffer::set(size_t index, T value)
{
  T* dx = reinterpret_cast<T *>(&dstore[0]);
  dx[index] = value;
}

template<typename T>
void NDBuffer::set(const PSize &index, T value)
{
  size_t pos = sub2index(index);
  T* dx = reinterpret_cast<T *>(&dstore[0]);
  dx[pos] = value;
}
  
/* ****************************************** */
namespace hades {

template<>
class ValueBox<NDBuffer> : public TypeSpec<DataType> {

public:
  typedef uint8_t  element_type;
  typedef NDBuffer  value_type;
  typedef NDBuffer &value_ref;
  typedef uint8_t  element;
  typedef element *element_ptr;

  ValueBox(value_ref val) : TypeSpec(val.dtype()), value(val) {}

  element_ptr get_data() { return value.data(); }
  value_ref   get() { return value; }
  PSize       shape() const { return value.shape(); }
  size_t      size() { return value.num_elements(); }
  void        resize(const PSize &size) {value.resize(size);}

private:
  value_ref value;
};

template<>
class ValueBox<const NDBuffer> : public TypeSpec<DataType> {

public:
  typedef uint8_t       element_type;
  typedef const NDBuffer  value_type;
  typedef const NDBuffer &value_ref;
  typedef const uint8_t  element;
  typedef element *element_ptr;

  ValueBox(value_ref val) : TypeSpec(val.dtype()), value(val) {}

  element_ptr get_data() const { return value.data(); }
  value_ref   get() const { return value; }
  PSize       shape() const { return value.shape(); }
  size_t      size() const { return value.num_elements(); }

private:
  value_ref value;
};


} //namespace hades
} //namespace pandora

#endif //PANDORA_NDBUFFER_H