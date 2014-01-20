// Copyright © 2013,2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <type_traits>

#include <boost/multi_array.hpp>
#include <nix/NDSize.hpp>

#ifndef NIX_HYDRA_H
#define NIX_HYDRA_H

namespace nix {

enum class DataType {
    Bool,
    Char,
    Float,
    Double,
    Int8,
    Int16,
    Int32,
    Int64,
    UInt8,
    UInt16,
    UInt32,
    UInt64,
    String,
    Date,
    DateTime
};

template<typename T>
struct to_data_type {
    static const bool is_valid = false;
};


template<>
struct to_data_type<bool> {
    static const bool is_valid = true;
    static const DataType value = DataType::Bool;
};

template<>
struct to_data_type<char> {
    static const bool is_valid = true;
    static const DataType value = DataType::Char;
};

template<>
struct to_data_type<float> {
    static const bool is_valid = true;
    static const DataType value = DataType::Float;
};

template<>
struct to_data_type<double> {
    static const bool is_valid = true;
    static const DataType value = DataType::Double;
};

template<>
struct to_data_type<int16_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int16;
};

template<>
struct to_data_type<uint16_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt16;
};

template<>
struct to_data_type<int32_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int32;
};

template<>
struct to_data_type<uint32_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt32;
};

template<>
struct to_data_type<int64_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int64;
};

template<>
struct to_data_type<uint64_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt64;
};

template<>
struct to_data_type<std::string> {
    static const bool is_valid = true;
    static const DataType value = DataType::String;
};



class InvalidRankException : public std::runtime_error {
public:
    InvalidRankException(const std::string &message)
        : std::runtime_error(message) { }
};

template<typename T>
struct data_traits {

    typedef T                 value_type;
    typedef value_type&       reference;
    typedef const value_type& const_reference;

    typedef T        element_type;
    typedef T*       element_pointer;
    typedef const T* const_element_pointer;

    static DataType data_type(const_reference value) {
        return to_data_type<element_type>::value;
    }

    static NDSize shape(const_reference value) {
        return NDSize();
    }

    static size_t num_elements(const_reference value) {
        return 1;
    }

    static const_element_pointer get_data(const_reference value) {
        return &value;
    }

    static element_pointer get_data(reference value) {
        return &value;
    }

    static void resize(reference value, const NDSize &dims) {
        if (dims.size() != 0) {
            throw InvalidRankException("Cannot resize scalar");
        }
    }

};


template<typename T, size_t N>
class data_traits<T[N]> {
public:

    typedef T                  value_type[N];
    typedef value_type&        reference;
    typedef const value_type&  const_reference;

    typedef T        element_type;
    typedef T*       element_pointer;
    typedef const T* const_element_pointer;

    static DataType data_type(const_reference val) {
        return to_data_type<element_type>::value;
    }

    static NDSize shape(const_reference value) {
        return NDSize{N};
    }

    static size_t num_elements(const_reference value) {
        return N;
    }

    static const_element_pointer get_data(const value_type& value) {
        return value;
    }

    static element_pointer get_data(value_type &value) {
        return value;
    }

    static void resize(reference value, const NDSize &dims) {
        if (dims.size() != 1 || dims[0] != N) {
            throw InvalidRankException("Cannot resize native arrays");
        }
        //NOOP
    }
};

template<typename T, size_t M, size_t N>
class data_traits<T[M][N]> {
public:

    typedef T                 value_type[M][N];
    typedef value_type&       reference;
    typedef const value_type& const_reference;

    typedef T        element_type;
    typedef T*       element_pointer;
    typedef const T* const_element_pointer;

    static DataType data_type(const_reference val) {
        return to_data_type<element_type>::value;
    }

    static NDSize shape(const_reference value) {
        return NDSize{M, N};
    }

    static size_t num_elements(const_reference value) {
        return M*N;
    }

    static const_element_pointer get_data(const value_type& value) {
        return value[0];
    }

    static element_pointer get_data(value_type &value) {
        return value[0];
    }

    static void resize(reference value, const NDSize &dims) {
        if (dims.size() != 2 || dims[0] != M || dims[1] != N) {
            throw InvalidRankException("Cannot resize native arrays");
        }
        //NOOP
    }
};


template<typename T>
class data_traits<std::vector<T>> {
public:

    typedef std::vector<T>     value_type;
    typedef value_type&        reference;
    typedef const value_type&  const_reference;

    typedef T        element_type;
    typedef T*       element_pointer;
    typedef const T* const_element_pointer;

    static DataType data_type(const_reference val) {
        return to_data_type<element_type>::value;
    }

    static NDSize shape(const_reference value) {
        return NDSize{value.size()};
    }

    static size_t num_elements(const_reference value) {
        return value.size();
    }

    static const_element_pointer get_data(const_reference value) {
        return &value[0];
    }

    static element_pointer get_data(value_type &value) {
        return &value[0];
    }

    static void resize(reference value, const NDSize &dims) {

        if (dims.size() != 1) {
            throw InvalidRankException("Cannot change rank of vector"); //FIXME
        }

        if (dims[0] == value.size())
            return;

        value.resize(dims[0]);
    }
};


template<typename T, size_t N>
class data_traits<boost::multi_array<T, N> > {
public:

    typedef boost::multi_array<T, N> value_type;
    typedef value_type&              reference;
    typedef const value_type&        const_reference;

    typedef T        element_type;
    typedef T*       element_pointer;
    typedef const T* const_element_pointer;

    static DataType data_type(const_reference val) {
        return to_data_type<element_type>::value;
    }

    static NDSize shape(const value_type &value) {
        NDSize ndsize(N);
        const size_t *ma_shape = value.shape();
        std::transform(ma_shape, ma_shape + N, ndsize.data(),
                       [](NDSize::const_reference val){ return static_cast<T>(val);});
        return ndsize;
    }

    static size_t num_elements(const_reference value) {
        return  value.num_elements();;
    }

    static const_element_pointer get_data(const value_type& value) {
        return value.data();
    }

    static element_pointer get_data(value_type &value) {
        return value.data();
    }

    static void resize(reference value, const NDSize &dims) {
        if (dims.size() != N) {
            throw InvalidRankException("Cannot change rank of multiarray");
        }

        value.resize(dims);
    }
};

/* *** */
/* new interface */
// DataWriter for wirting to, i.e. reading from disk: non-const types */
template<typename T, typename ElementType>
class DataWriter {
public:
    typedef data_traits<T>  data_traits_t;
    typedef ElementType     data_type;
    typedef data_type      *data_ptr;

    DataWriter(T &val) : value(val) {}

    data_ptr begin() { return data_traits_t::get_data(value); }
    void finish() { }

private:
    T &value;
};

template<typename T>
class DataWriter<T, std::string> {
public:
    typedef data_traits<T>  data_traits_t;
    typedef char           *data_type;
    typedef data_type      *data_ptr;

    DataWriter(T &val) : value(val) {
        size_t nelms = data_traits_t::num_elements(value);
        data = new data_type[nelms];
    }

    data_ptr begin() {
        return data;
    }

    void finish() {
        size_t nelms = data_traits_t::num_elements(value);
        auto vptr = data_traits_t::get_data(value);

        for (size_t i = 0; i < nelms; i++) {
            vptr[i] = data[i];
        }
    }

    ~DataWriter() {
        delete[] data;
    }

private:
    T        &value;
    data_ptr  data;
};

template <typename T, typename ElementType>
class DataReader {
public:
    typedef data_traits<T>        data_traits_t;
    typedef const ElementType     data_type;
    typedef data_type            *data_ptr;

    DataReader(const T &val) : value(val) {}

    data_ptr begin() const { return data_traits_t::get_data(value); }
    void finish() const { }

private:
    const T &value;
};

template<typename T>
class DataReader<T, std::string> {
public:
    typedef data_traits<T>   data_traits_t;
    typedef char const      *data_type;
    typedef data_type       *data_ptr;

    DataReader(const T &val) : value(val) {
        size_t nelms = data_traits_t::num_elements(value);
        data = new data_type[nelms];
        auto vptr = data_traits_t::get_data(value);

        for (size_t i = 0; i < nelms; i++) {
            data[i] = vptr[i].c_str();
        }
    }

    data_ptr begin() const { return data; }

    void finish() const { }

    ~DataReader() {
        delete[] data;
    }

private:
    const T  &value;
    data_ptr  data;
};



template<typename T>
class Hydra {
public:
    typedef T                                    value_type;
    typedef T&                                   reference;
    typedef typename std::remove_const<T>::type  vanilla_type;
    typedef data_traits<vanilla_type>            data_traits_t;
    typedef typename data_traits_t::element_type element_t;

    static const bool vtype_is_const = std::is_const<T>::value;

    typedef       DataWriter<vanilla_type, element_t> writer_t;
    typedef const DataReader<vanilla_type, element_t> reader_t;

    Hydra(reference value_ref) : value(value_ref) {
        static_assert(to_data_type<element_t>::is_valid,
                      "Hydra cannot handle this type");
    }

    DataType element_data_type() const {
        return data_traits<vanilla_type>::data_type(value);
    }

    NDSize shape() const {
        return data_traits<vanilla_type>::shape(value);
    }

    writer_t writer() {
        return writer_t(value);
    }

    reader_t reader() const {
        return reader_t(value);
    }

private:
    reference value;
};


} //::nix


#endif

