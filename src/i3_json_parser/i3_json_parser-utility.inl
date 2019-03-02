/*
    Copyright (C) 2019, Ivan Balević

    This file is part of "i3-ipc++" library.

    "i3-ipc++" is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    "i3-ipc++" is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "i3-ipc++". If not, see <https://www.gnu.org/licenses/>.
*/

/**
 * \file i3_json_parser-utility.inl
 *
 * Defines template function used for convenient extraction of data from "RapidJSON" objects.
 */

#ifndef I3_JSON_PARSER_UTILITY_INL
#define I3_JSON_PARSER_UTILITY_INL

// External library headers.
#include <rapidjson/document.h>

// C++ headers.
#include <optional>
#include <type_traits>

// C headers.
#include <cstdint>

/**
 * \brief      Helper template used to determine if provided type is optional or not.
 *
 * \tparam T   Type to be checked.
 *
 * \return     True of false.
 */
template<typename T> struct is_optional : std::false_type {};
template<typename T> struct is_optional<std::optional<T>> : std::true_type {};
template<typename T> constexpr bool is_optional_v = is_optional<T>::value;

/**
 * \brief      Helper template used to extract core type from given type.
 *
 * \tparam T   Type from which core type will be extracted.
 *
 * \return     If T is std::optional, return the underneath type. Otherwise, return the same type T.
 */
template<typename T> struct core_type { using type = T; };
template<typename T> struct core_type<std::optional<T>> { using type = T; };
template<typename T> using core_type_t = typename core_type<T>::type;

/**
 * \brief      Helper template which checks if type T is supported by "RapidJSON" library.
 *
 * \tparam T   Type to be checked.
 *
 * \return     True of false.
 */
template<typename T> using is_supported_by_rapidjson = std::disjunction<
                                                                        std::is_same<T, bool>,
                                                                        std::is_same<T, int>,
                                                                        std::is_same<T, unsigned>,
                                                                        std::is_same<T, std::int64_t>,
                                                                        std::is_same<T, std::uint64_t>,
                                                                        std::is_same<T, double>,
                                                                        std::is_same<T, float>,
                                                                        std::is_same<T, const char*>
                                                                       >;
template<typename T> constexpr bool is_supported_by_rapidjson_v = is_supported_by_rapidjson<T>::value;

/**
 * \brief      Helper template which checks if type T is supported only by "i3-ipc++" library.
 *
 * \tparam T   Type to be checked.
 *
 * \return     True of false.
 */
template<typename T> using is_supported_by_i3_ipc = std::disjunction<
                                                                     std::is_same<T, std::int8_t>,
                                                                     std::is_same<T, std::int16_t>,
                                                                     std::is_same<T, std::int32_t>,
                                                                     std::is_same<T, std::uint8_t>,
                                                                     std::is_same<T, std::uint16_t>,
                                                                     std::is_same<T, std::uint32_t>
                                                                    >;
template<typename T> constexpr bool is_supported_by_i3_ipc_v = is_supported_by_i3_ipc<T>::value;

/**
 * \brief      Helper template which checks if type T is supported either by "RapidJSON" or "i3-ipc++" library.
 *
 * \tparam T   Type to be checked.
 *
 * \return     True of false.
 */
template<typename T> using is_supported = std::disjunction<is_supported_by_rapidjson<T>, is_supported_by_i3_ipc<T>>;
template<typename T> constexpr bool is_supported_v = is_supported<T>::value;

/**
 * \brief                      Extracts value of the requested attribute from given JSON object.
 *
 * \tparam T                   Return type of the extracted value.
 *
 * \param [in] a_json_object   JSON object.
 *
 * \param [in] a_attribute     Name of the attribute for which value will be extracted.
 *
 * \return                     Extracted value of the "a_attribute" from "a_json_object".
 */
template<typename T, typename = std::enable_if_t<is_supported_v<core_type_t<T>>>>
T get_attribute_value(const rapidjson::Value& a_json_object, const char* a_attribute)
{
    assert(a_json_object.IsObject());

    // Handle non-existing attributes.
    if constexpr (is_optional_v<T>)
    {
        if (!a_json_object.HasMember(a_attribute) || a_json_object[a_attribute].IsNull())
        {
            return std::nullopt;
        }
    }
    else
    {
        assert(a_json_object.HasMember(a_attribute));
    }

    // Extract attribute value from JSON object.
    using return_type = core_type_t<T>;
    if constexpr (is_supported_by_rapidjson_v<return_type>)
    {
        assert(a_json_object[a_attribute].template Is<return_type>());
        return a_json_object[a_attribute].template Get<return_type>();
    }
    else if constexpr (std::is_signed_v<return_type>)
    {
        if constexpr (sizeof(return_type) <= sizeof(int))
        {
            assert(a_json_object[a_attribute].template Is<int>());
            return static_cast<return_type>(a_json_object[a_attribute].template Get<int>());
        }
        else
        {
            assert(a_json_object[a_attribute].template Is<std::int64_t>());
            return static_cast<return_type>(a_json_object[a_attribute].template Get<std::int64_t>());
        }
    }
    else if constexpr (std::is_unsigned_v<return_type>)
    {
        if constexpr (sizeof(return_type) <= sizeof(unsigned))
        {
            assert(a_json_object[a_attribute].template Is<unsigned>());
            return static_cast<return_type>(a_json_object[a_attribute].template Get<unsigned>());
        }
        else
        {
            assert(a_json_object[a_attribute].template Is<std::uint64_t>());
            return static_cast<return_type>(a_json_object[a_attribute].template Get<std::uint64_t>());
        }
    }
}

#endif // I3_JSON_PARSER_UTILITY_INL
