// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_CHECKS_H
#define NIX_CHECKS_H

#include <boost/optional.hpp>
#include <boost/any.hpp>
#include <nix/util/util.hpp>
#include <nix/valid/helper.hpp>

#include <nix/base/IDimensions.hpp>

#include <nix/types.hpp>

namespace nix {
namespace valid {

    /**
     * @brief Check if later given not greater than initally defined value.
     * 
     * One Check struct that checks whether the given value is not greater
     * than the initially given other value, both of which have to be
     * convertible to double.
     */
    struct notGreater {
        const double value;
        
        template<typename T>
        notGreater(T value) : value(static_cast<double>(value)) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return !(static_cast<double>(val) > value);
        }
    };

    /**
     * @brief Check if later given greater than initally defined value.
     * 
     * One Check struct that checks whether the given value is greater than
     * the initially given other value, both of which have to be
     * convertible to double.
     */
    struct isGreater {
        const double value;
        
        template<typename T>
        isGreater(T value) : value(static_cast<double>(value)) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return static_cast<double>(val) > value;
        }
    };

    /**
     * @brief Check if later given not smaller than initally defined value.
     * 
     * One Check struct that checks whether the given value is not smaller
     * than the initially given other value, both of which have to be
     * convertible to double.
     */
    struct notSmaller {
        const double value;
        
        template<typename T>
        notSmaller(T value) : value(static_cast<double>(value)) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return !(static_cast<double>(val) < value);
        }
    };

    /**
     * @brief Check if later given smaller than initally defined value.
     * 
     * One Check struct that checks whether the given value is smaller than
     * the initially given other value, both of which have to be
     * convertible to double.
     */
    struct isSmaller {
        const double value;
        
        template<typename T>
        isSmaller(T value) : value(static_cast<double>(value)) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return static_cast<double>(val) < value;
        }
    };

    /**
     * @brief Check for un-equality of initally defined and later given value.
     * 
     * One Check struct that checks whether the given value is not equal
     * to the initially given other value.
     */
    template<typename T>
    struct notEqual {
        const T value;
        
        notEqual(T value) : value(value) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return value != val;
        }
    };

    /**
     * @brief Check for equality of initally defined and later given value.
     * 
     * One Check struct that checks whether the given value is equal to
     * the initially given other value.
     */
    template<typename T>
    struct isEqual {
        const T value;
        
        isEqual(T value) : value(value) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return value == val;
        }
    };
    // needed because: for bizarre reasons bool converts to int when compared to boost::optional
    template<>
    struct isEqual<bool> {
        const bool value;
        
        isEqual(bool value) : value(value) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return value ? !!val : !val;
        }
    };

    /**
     * @brief Check if given value casts to boolean true
     * 
     * One Check struct that checks whether the given value casts to true
     * or to false.
     * T can be: boost::optional, boost::none, nix-entity
     * and any basic type.
     */
    struct notFalse {
        // WARNING: enum will convert via int, which means 0 = false !
        template<typename T>
        bool operator()(const T &val) const {
            return !!val;
        }
    };

    /**
     * @brief Check if given value casts to boolean false
     * 
     * One Check struct that checks whether the given value casts to false
     * or to true.
     * T can be: boost::optional, boost::none, nix-entity
     * and any basic type.
     */
    struct isFalse {
        // WARNING: enum will convert via int, which means 0 = false !
        template<typename T>
        bool operator()(const T &val) const {
            return !val;
        }
    };

    /**
     * @brief Check if given class/struct returns "empty() == false"
     * 
     * One Check struct that checks whether the given value is not empty
     * or is empty.
     * T can be: any STL container.
     */
    struct notEmpty {
        template<typename T>
        bool operator()(const T &val) const {
            return !(val.empty());
        }
    };

    /**
     * @brief Check if given class/struct returns "empty() == true"
     * 
     * One Check struct that checks whether the given value is empty or
     * not.
     * T can be: any STL container.
     */
    struct isEmpty {
        template<typename T>
        bool operator()(const T &val) const {
            return val.empty();
        }
    };
    
    /**
     * @brief Check if given class represents valid SI unit string(s)
     * 
     * Base struct to be inherited by the {@link isValidUnit}, {@link
     * isAtomicUnit}, {@link isCompoundUnit}. Not viable on its own!
     */
    struct isUnit {
        typedef std::function<bool(std::string)> TFUNC;
        
        virtual bool operator()(const std::string &u) const = 0;
        
        bool operator()(const boost::optional<std::string> &u) const {
            // note: relying on short-curcuiting here
            return u && (*this)(*u);
        }
                
        bool operator()(const std::vector<std::string> &u, TFUNC obj) const {
            // if test fails we have invalid unit & find_if_not will return it != end
            return std::find_if_not(u.begin(), u.end(), obj) != u.end();
        }
    };

    /**
     * @brief Check if given class represents valid SI unit string(s)
     * 
     * One Check struct that checks whether the given string(s) represent(s)
     * a valid atomic or compound SI unit.
     * Parameter can be of type boost optional (containing nothing or
     * string) or of type string or a vector of strings.
     */
    struct isValidUnit : public isUnit {
        bool operator()(const std::string &u) const {
            return (util::isSIUnit(u) || util::isCompoundSIUnit(u));
        }
        bool operator()(const boost::optional<std::string> &u) const {
            return isUnit::operator()(u);
        }
        bool operator()(const std::vector<std::string> &u) const {
            return isUnit::operator()(u, *this);
        }
    };

    /**
     * @brief Check if given class represents valid atomic SI unit string(s)
     * 
     * One Check struct that checks whether the given string(s) represent(s)
     * a valid atomic SI unit.
     * Parameter can be of type boost optional (containing nothing or
     * string) or of type string or a vector of strings.
     */
    struct isAtomicUnit : public isUnit {
        bool operator()(const std::string &u) const {
            return util::isSIUnit(u);
        }
        bool operator()(const boost::optional<std::string> &u) const {
            return isUnit::operator()(u);
        }
        bool operator()(const std::vector<std::string> &u) const {
            return isUnit::operator()(u, *this);
        }
    };

    /**
     * @brief Check if given class represents valid compound SI unit string(s)
     * 
     * One Check struct that checks whether the given string(s) represent(s)
     * a valid compound SI unit.
     * Parameter can be of type boost optional (containing nothing or
     * string) or of type string or a vector of strings.
     */
    struct isCompoundUnit : public isUnit {
        bool operator()(const std::string &u) const {
            return util::isCompoundSIUnit(u);
        }
        bool operator()(const boost::optional<std::string> &u) const {
            return isUnit::operator()(u);
        }
        bool operator()(const std::vector<std::string> &u) const {
            return isUnit::operator()(u, *this);
        }
    };

    /**
     * @brief Check if given value can be regarded as being set
     * 
     * One Check struct that checks whether the given value can be
     * considered set, by applying {@link notFalse} and {@link notEmpty}
     * checks. Value thus is set if: STL cotnainer not empty OR
     * bool is true OR boost optional is set OR number is not 0.
     * Parameter can be of above types or even boost none_t.
     * NOTE: use this if you don't know wheter a type has and "empty"
     * method.
     */
    struct isSet {
        template<typename T>
        bool operator()(const T &val) const {
            typedef typename std::conditional<hasEmpty<T>::value, notEmpty, notFalse>::type subCheck;
            return subCheck(val);
        }
    };

    /**
     * @brief Check if given container is sorted using std::is_sorted
     * 
     * One Check struct that checks whether the given container is sorted
     * according to std::is_sorted. Thus supports types that are
     * supported by std::is_sorted.
     */
    struct isSorted {
        template<typename T>
        bool operator()(const T &container) const {
            return std::is_sorted(container.begin(), container.end());
        }
    };

    /**
     * @brief Check if given DataArray has given dimensionality
     * 
     * One Check struct that checks whether the given DataArray entity
     * has a dimensionality of the given uint value by getting its'
     * NDSize class via the "dataExtent" method and checking its' size
     * via "size" the method.
     */
    struct dimEquals {
        const size_t &value;
        
        dimEquals(const size_t &value) : value(value) {}
        
        bool operator()(const DataArray &array) const;
    };

    /**
     * @brief Check if given DataArrays' dimensions all have units where given units vector has
     * 
     * One Check struct that checks whether the given referenced
     * DataArrays' dimensions all have units defined where the tag has.
     * (where the tag has means at the same index in the tag's units
     * vector as the dimension index in the referenced DataArray)
     * Therefore it takes all dimensions of all given references
     * and checks whether the dimension has a unit set if the tag has.
     * The test counts as passed if all dimensions have units set where
     * the tag has and have no units set where the tag has not. It
     * counts as failed immediately if number of dimensions differs from
     * number of units in given unit vector.
     */
    struct tagRefsHaveUnits {
        const std::vector<std::string> &units;
        
        tagRefsHaveUnits(const std::vector<std::string> &units) : units(units) {}
        
        bool operator()(const std::vector<DataArray> &references) const;
    };

    /**
     * @brief Check if given units match given referenced DataArrays' units
     * 
     * One Check struct that checks whether the given units (vector of
     * strings) match the given referenced DataArrays' (vector of
     * DataArray references) units. Therefore it takes all dimensions of
     * all given references and checks whether the dimension has a unit
     * convertible to the unit with the same index in the given units
     * vector. The test counts as passed if all dimensions have units
     * set that are convertible where the units vector has a unit set
     * and all dims have no unit set where the units vector has not.
     * The test counts as failed immediately if the number of dimensions
     * in a DataArray differs the number of units in the units vector.
     */
    struct tagUnitsMatchRefsUnits {
        const std::vector<std::string> &units;
        
        tagUnitsMatchRefsUnits(const std::vector<std::string> &units) : units(units) {}
        
        bool operator()(const std::vector<DataArray> &references) const;
    };

    /**
     * @brief Check if given number of positions and extents matches
     * 
     * One Check struct that checks whether the given number of
     * positions matches the given number of extents. It is irrelevant
     * which gets passed at construction time and which via operator().
     */
    struct extentsMatchPositions {
        const boost::any extents;
        
        extentsMatchPositions(const DataArray &extents) : extents(extents) {}
        
        extentsMatchPositions(const std::vector<double> &extents) : extents(extents) {}
        
        bool operator()(const DataArray &positions) const;
        bool operator()(const std::vector<double> &positions) const;
    };

    /**
     * @brief Check if number of extents (along 2nd dim) match number of references' data dims
     * 
     * One Check struct that checks whether the given number of
     * extents (if DataArray: size along 2nd dimensions of extents
     * DataArray; if vector: size of vector) matches the given number of
     * dimensions in each of the given referenced DataArrays.
     */
    struct extentsMatchRefs {
        const std::vector<DataArray> &refs;
        
        extentsMatchRefs(const std::vector<DataArray> &refs) : refs(refs) {}

        bool operator()(const DataArray &extents) const;
        bool operator()(const std::vector<double> &extents) const;
    };

    /**
     * @brief Check if number of positions (along 2nd dim) match number of references' data dims
     * 
     * One Check struct that checks whether the given number of
     * positions (if DataArray: size along 2nd dimensions of positions
     * DataArray; if vector: size of vector) matches the given number of
     * dimensions in each of the given referenced DataArrays.
     * Note: this is just an alias for extentsMatchRefs wich does the
     * same thing.
     */
    struct positionsMatchRefs {
        const std::vector<DataArray> &refs;

        positionsMatchRefs(const std::vector<DataArray> &refs) : refs(refs) {}
    
        bool operator()(const DataArray &positions) const;
        bool operator()(const std::vector<double> &positions) const;
    };

    /**
     * @brief Check if range dimension specifics ticks match data
     * 
     * One Check struct that checks whether the dimensions of type
     * "Range" in the given dimensions vector have ticks that match
     * the given DataArray's data: number of ticks == number of entries
     * along the corresponding dimension in the data.
     */
    struct dimTicksMatchData {
        const DataArray &data;

        dimTicksMatchData(const DataArray &data) : data(data) {}
    
        bool operator()(const std::vector<Dimension> &dims) const;
    };

    /**
     * @brief Check if set dimension specifics labels match data
     * 
     * One Check struct that checks whether the dimensions of type
     * "Set" in the given dimensions vector have labels that match
     * the given DataArray's data: number of labels == number of entries
     * along the corresponding dimension in the data.
     */
    struct dimLabelsMatchData {
        const DataArray &data;

        dimLabelsMatchData(const DataArray &data) : data(data) {}
    
        bool operator()(const std::vector<Dimension> &dims) const;
    };

} // namespace valid
} // namespace nix

#endif // NIX_CHECKS_H
