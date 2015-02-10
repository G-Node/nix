// Copyright © 2013,2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <nix/Exception.hpp>
#include <nix/NDSize.hpp>
#include <nix/DataType.hpp>

#include <type_traits>
#include <limits>
#include <valarray>

#ifndef NIX_HYDRA_H
#define NIX_HYDRA_H

namespace nix {

namespace check {

template<typename T>
inline typename std::enable_if<! std::is_same<T, size_t>::value, size_t>::type
fits_in_size_t(T size, const std::string &msg_if_fail) {
    if (size > std::numeric_limits<size_t>::max()) {
        throw OutOfBounds(msg_if_fail);
    }
    return static_cast<size_t>(size);
}

template<typename T>
inline typename std::enable_if<std::is_same<T, size_t>::value, size_t>::type
fits_in_size_t(T size, const std::string &msg_if_fail) {
    return size;
}

} // nix::check::

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
        if (!(dims.size() == 0 || dims.nelms() == 1)) {
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
        return value.data();
    }

    static element_pointer get_data(value_type &value) {
        return value.data();
    }

    static void resize(reference value, const NDSize &dims) {
        size_t non_singletons = 0;
        size_t non_singleton_index = 0;
        for (size_t i = 0; i < dims.size(); ++i) {
            if (dims[i] > 1) {
                non_singletons++;
                non_singleton_index = i;
            }
        }
        if (non_singletons > 1) {
            throw InvalidRank("Cannot change rank of vector");
        }

        ndsize_t to_resize = dims[non_singleton_index];
        size_t size = check::fits_in_size_t(to_resize, "Can't resize: data to big for memory");
        value.resize(size);
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

        size_t size = check::fits_in_size_t(dims[0], "can't resize: data to big for memory");
        value.resize(size);
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

