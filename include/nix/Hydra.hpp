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
#include <valarray>

#include <nix/Exception.hpp>
#include <nix/NDSize.hpp>
#include <nix/DataType.hpp>

#ifndef NIX_HYDRA_H
#define NIX_HYDRA_H

namespace nix {

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
            throw InvalidRank("Cannot resize scalar");
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
            throw InvalidRank("Cannot resize native arrays");
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
            throw InvalidRank("Cannot resize native arrays");
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
            throw InvalidRank("Cannot change rank of vector"); //FIXME
        }

        if (dims[0] == value.size())
            return;

        value.resize(dims[0]);
    }
};

template<typename T>
class data_traits<std::valarray<T>> {
public:

    typedef std::valarray<T>     value_type;
    typedef value_type&          reference;
    typedef const value_type&    const_reference;

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
            throw InvalidRank("Cannot change rank of valarray");
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
                       [](NDSize::const_reference val) { return static_cast<T>(val);});
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
            throw InvalidRank("Cannot change rank of multiarray");
        }

        value.resize(dims);
    }
};

/* *** */

template<typename T>
class Hydra {
public:
    typedef T                                    value_type;
    typedef T&                                   reference;
    typedef typename std::remove_const<T>::type  vanilla_type;
    typedef data_traits<vanilla_type>            data_traits_t;
    typedef typename data_traits_t::element_type element_t;

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

    void resize(const NDSize &new_shape) {
        data_traits<vanilla_type>::resize(value, new_shape);
    }

    element_t *data() {
        return data_traits<vanilla_type>::get_data(value);
    }

    const element_t *data() const {
        return data_traits<vanilla_type>::get_data(value);
    }

private:
    reference value;
};


} //::nix


#endif

