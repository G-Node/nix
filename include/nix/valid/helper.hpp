// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_HELPER_H
#define NIX_HELPER_H

#include <string>
#include <vector>
#include <nix/base/IDimensions.hpp>
#include <nix/Platform.hpp>

namespace nix {
namespace valid {

    /**
     * @brief Message struct with entity id and msg string
     * 
     * Struct with message text and id string to save a message and the
     * entity id together, used by {@link Results}.
     */
    struct NIXAPI Message {
        std::string id;
        std::string msg;

        Message(std::string new_id, std::string new_msg)
        : id(new_id), msg(new_msg)
        {
        }
    };

    /**
     * @brief Helper to check classes for an "id" method via SFINAE
     * 
     * Helper class that checks whether a given class or struct has
     * a method called "id"
     * USAGE: hasID<TOBJ>::value
     */
#ifndef _WIN32
    template<typename T>
    class NIXAPI hasID
    {
        typedef char one;
        typedef long two;

        template<typename C> static one test( decltype(&C::id) ) ;
        template<typename C> static two test(...);

    public:
        enum { value = sizeof(test<T>(0)) == sizeof(char) };
    };
#else
	template<typename T>
	class hasID
	{
	public:
		__if_exists(T::id) {
			static const bool value = true;
		}
		__if_not_exists(T::id) {
			static const bool value = false;
		}
	};
#endif

    /**
     * @brief Helper to check classes for an "empty" method via SFINAE
     * 
     * Helper class that checks whether a given class or struct has
     * a method called "empty"
     * USAGE: hasEmpty<TOBJ>::value
     */
#ifndef _WIN32
    template<typename T>
    class NIXAPI hasEmpty
    {
        typedef char one;
        typedef long two;

        template<typename C> static one test( decltype(&C::empty) ) ;
        template<typename C> static two test(...);

    public:
        enum { value = sizeof(test<T>(0)) == sizeof(char) };
    };
#else
	template<typename T>
	class hasEmpty
	{
	public:
		__if_exists(T::empty) {
			static const bool value = true;
		}
		__if_not_exists(T::empty) {
			static const bool value = false;
		}
	};
#endif

    /**
     * @brief Helper calling "id" method in class if and only if it exists (SFINAE)
     * 
     * Helper class with "get" method that will execute a given
     * objects "id" method & return result if "HAS_ID" template param
     * is true and return string "unknown" otherwise.
     * USAGE: ID< hasID<TOBJ>::value >().get(obj)
     */
    template<bool HAS_ID>
    class NIXAPI ID {
    public:
        template<typename TOBJ>
        auto get(TOBJ parent) -> decltype(parent.id()) {
            return parent.id();
        }
    };
    template<>
    class NIXAPI ID<false> {
    public:
        template<typename TOBJ>
        std::string get(TOBJ parent) {
            return std::string("unknown");
        }
    };

    /**
     * @brief Helper getting the units from all dimensions of DataArray
     * 
     * Helper function that gets all the units from all the dimensions
     * of the given DataArray and returns them as vector of strings.
     * For all units not set (since boost::optional) and for all dims
     * that have no unit (since SetDimension) it inserts an empty string
     * so that the number of returned units matches the number of
     * dimensions and indices correspond.
     */
    template<typename T>
    std::vector<std::string> getDimensionsUnits(T darray) {
        std::vector<std::string> units;
        
        for(auto &dim : darray.dimensions()) {
            if(dim.dimensionType() != DimensionType::Range) {
                auto d = dim.asRangeDimension();
                units.push_back(d.unit() ? *d.unit() : std::string());
            }
            if(dim.dimensionType() == DimensionType::Sample) {
                auto d = dim.asRangeDimension();
                units.push_back(d.unit() ? *d.unit() : std::string());
            }
            if(dim.dimensionType() == DimensionType::Set) {
                units.push_back(std::string());
            }
        }
        
        return units;
    }

} // namespace valid
} // namespace nix

#endif // NIX_HELPER_H
