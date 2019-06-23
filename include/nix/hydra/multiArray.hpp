// Copyright © 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>
#ifndef HYDRA_MULTI_ARRAY_H
#define HYDRA_MULTI_ARRAY_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

#include <boost/multi_array.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <nix/Hydra.hpp>

#include <type_traits>


namespace nix {

template<typename T, size_t N>
class data_traits<boost::multi_array<T, N> > {
public:

    typedef boost::multi_array<T, N> value_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;

    typedef T element_type;
    typedef T *element_pointer;
    typedef const T *const_element_pointer;

    static DataType data_type(const_reference val) {
        return to_data_type<element_type>::value;
    }

    static NDSize shape(const value_type &value) {
        NDSize ndsize(N);
        const size_t *ma_shape = value.shape();
        std::transform(ma_shape, ma_shape + N, ndsize.data(),
                [](NDSize::const_reference val) {
                    return static_cast<T>(val);
                });
        return ndsize;
    }

    static size_t num_elements(const_reference value) {
        return value.num_elements();;
    }

    static const_element_pointer get_data(const value_type &value) {
        return value.data();
    }

    static element_pointer get_data(value_type &value) {
        return value.data();
    }

    static void resize(reference value, const NDSize &dims) {
        check_rank(dims.size());
        //FIXME: not needed for ndsize_t == size_t case, optimize
        std::vector<size_t> size(N);

        for (size_t i = 0; i < N; i++) {
            size[i] = check::fits_in_size_t(dims[i], "Cannot resize multiarray: too big for memroy");
        }

        value.resize(size);
    }

    static void check_rank(size_t rank) {
        if (rank != N) {
            throw InvalidRank("Cannot change rank of multiarray");
        }
    }

};

} //nix::

#endif // HYDRA_MULTI_ARRAY_H
