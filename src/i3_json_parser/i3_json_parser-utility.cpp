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
 * \file    i3_json_parser-utility.cpp
 *
 * \brief   Defines private member functions of "i3_json_parser" class.
 */

// Library headers.
#include "i3_json_parser.hpp"
#include "i3_containers.hpp"
#include "i3_ipc_unsupported.hpp"

// Library templates.
#include "i3_json_parser-utility.inl"

// External library headers.
#include <rapidjson/document.h>

// C++ headers.
#include <string>
#include <optional>
#include <string_view>

namespace
{
    /**
     * \brief                       Extracts info about i3 node type from JSON object.
     *
     * \param [in] a_json_object    JSON object.
     *
     * \return                      Extracted node type from the "a_json_object".
     *
     * \throws i3_ipc_unsupported   When value of extracted node type is unknown to "i3-ipc++" library.
     */
    i3_containers::node_type extract_node_type(const rapidjson::Value& a_json_object)
    {
        const std::string_view type = get_attribute_value<const char*>(a_json_object, "type");
        if (type == "root")
        {
            return i3_containers::node_type::root;
        }
        else if (type == "output")
        {
            return i3_containers::node_type::output;
        }
        else if (type == "con")
        {
            return i3_containers::node_type::con;
        }
        else if (type == "floating_con")
        {
            return i3_containers::node_type::floating_con;
        }
        else if (type == "workspace")
        {
            return i3_containers::node_type::workspace;
        }
        else if (type == "dockarea")
        {
            return i3_containers::node_type::dockarea;
        }
        else
        {
            throw i3_ipc_unsupported("Unknown node type: \"" + std::string(type) + "\"");
        }
    }

    /**
     * \brief                       Extracts info about border style from JSON object.
     *
     * \param [in] a_json_object    JSON object.
     *
     * \return                      Extracted border style from the "a_json_object".
     *
     * \throws i3_ipc_unsupported   When value of extracted border style is unknown to "i3-ipc++" library.
     */
    i3_containers::border_style extract_border_style(const rapidjson::Value& a_json_object)
    {
        const std::string_view border_style = get_attribute_value<const char*>(a_json_object, "border");
        if (border_style == "normal")
        {
            return i3_containers::border_style::normal;
        }
        else if (border_style == "none")
        {
            return i3_containers::border_style::none;
        }
        else if (border_style == "pixel")
        {
            return i3_containers::border_style::pixel;
        }
        else
        {
            throw i3_ipc_unsupported("Unknown border style: \"" + std::string(border_style) + "\"");
        }
    }

    /**
     * \brief                       Extracts info about node layout from JSON object.
     *
     * \param [in] a_json_object    JSON object.
     *
     * \return                      Extracted node layout from the "a_json_object".
     *
     * \throws i3_ipc_unsupported   When value of extracted node layout is unknown to "i3-ipc++" library.
     */
    i3_containers::node_layout extract_node_layout(const rapidjson::Value& a_json_object)
    {
        const std::string_view layout = get_attribute_value<const char*>(a_json_object, "layout");
        if (layout == "splith")
        {
            return i3_containers::node_layout::splith;
        }
        else if (layout == "splitv")
        {
            return i3_containers::node_layout::splitv;
        }
        else if (layout == "stacked")
        {
            return i3_containers::node_layout::stacked;
        }
        else if (layout == "tabbed")
        {
            return i3_containers::node_layout::tabbed;
        }
        else if (layout == "dockarea")
        {
            return i3_containers::node_layout::dockarea;
        }
        else if (layout == "output")
        {
            return i3_containers::node_layout::output;
        }
        else
        {
            throw i3_ipc_unsupported("Unknown node layout: \"" + std::string(layout) + "\"");
        }
    }

    /**
     * \brief                      Extracts info about window properties from JSON object.
     *
     * \param [in] a_json_object   JSON object.
     *
     * \return                     Extracted window properties from the "a_json_object".
     */
    std::optional<i3_containers::x11_window> extract_window_properties(const rapidjson::Value& a_json_object)
    {
        assert(a_json_object.IsObject());
        if (!a_json_object.HasMember("window_properties") || a_json_object["window_properties"].IsNull())
        {
            return std::nullopt;
        }

        const rapidjson::Value& window_object = a_json_object["window_properties"];

        i3_containers::x11_window window;
        window.window_class = get_attribute_value<std::optional<const char*>>(window_object, "class");
        window.window_instance = get_attribute_value<std::optional<const char*>>(window_object, "instance");
        window.window_role = get_attribute_value<std::optional<const char*>>(window_object, "window_role");
        window.window_title = get_attribute_value<std::optional<const char*>>(window_object, "title");
        window.transient_for = get_attribute_value<std::optional<std::uint64_t>>(window_object, "transient_for");

        return window;
    }

    /**
     * \brief                      Extracts the fullscreen_mode of the window from the JSON object.
     *
     * \param [in] a_json_object   JSON object.
     *
     * \return                     Extracted fullscreen_mode.
     */
    i3_containers::fullscreen_mode_type extract_fullscreen_mode(const rapidjson::Value& a_json_object)
    {
        assert(a_json_object.IsObject());
        assert(a_json_object.HasMember("fullscreen_mode") && a_json_object["fullscreen_mode"].IsUint());

        const auto val = get_attribute_value<std::uint8_t>(a_json_object, "fullscreen_mode");
        return static_cast<i3_containers::fullscreen_mode_type>(val);
    }


    /**
     * \brief                      Extracts the marks of the node from the JSON object.
     *
     * \param [in] a_json_object   JSON object.
     *
     * \return                     Extracted vector of marks
     */
    std::vector<std::string> extract_marks(const rapidjson::Value& a_json_object)
    {
        assert(a_json_object.IsObject());
        if (!a_json_object.HasMember("marks") || !a_json_object["marks"].IsArray())
        {
            return { };
        }

        auto& json_marks = a_json_object["marks"];
        std::vector<std::string> marks{};
        marks.reserve(json_marks.GetArray().Size());
        for (const auto& mark : json_marks.GetArray())
        {
            assert(mark.IsString());
            marks.push_back(mark.GetString());
        }

        return marks;
    }
} // Unnamed namespace.

rapidjson::Document i3_json_parser::parse_json(const char* a_json_string)
{
    rapidjson::Document document;
    document.Parse(a_json_string);
    assert(!document.HasParseError());

    return document;
}

i3_containers::rectangle i3_json_parser::extract_rectangle(const rapidjson::Value& a_json_object,
                                                           const char* a_attribute)
{
    assert(a_json_object.IsObject());
    assert(a_json_object.HasMember(a_attribute));
    const rapidjson::Value& rectangle_object = a_json_object[a_attribute];

    i3_containers::rectangle rectangle;
    rectangle.x      = get_attribute_value<std::uint16_t>(rectangle_object, "x");
    rectangle.y      = get_attribute_value<std::uint16_t>(rectangle_object, "y");
    rectangle.width  = get_attribute_value<std::uint16_t>(rectangle_object, "width");
    rectangle.height = get_attribute_value<std::uint16_t>(rectangle_object, "height");

    return rectangle;
}

i3_containers::node i3_json_parser::extract_tree(const rapidjson::Value& a_json_object)
{
    // Extract trivial node info.
    i3_containers::node node;
    node.id = get_attribute_value<std::uint64_t>(a_json_object, "id");
    node.name = get_attribute_value<std::optional<const char*>>(a_json_object, "name");
    node.type = extract_node_type(a_json_object);
    node.border = extract_border_style(a_json_object);
    const std::int8_t tmp = get_attribute_value<std::int8_t>(a_json_object, "current_border_width");
    node.current_border_width = tmp <= 0 ? std::nullopt : std::make_optional(static_cast<std::uint8_t>(tmp));
    node.layout = extract_node_layout(a_json_object);
    node.percent = get_attribute_value<std::optional<float>>(a_json_object, "percent");
    node.rect = extract_rectangle(a_json_object, "rect");
    node.window_rect = extract_rectangle(a_json_object, "window_rect");
    node.deco_rect = extract_rectangle(a_json_object, "deco_rect");
    node.geometry = extract_rectangle(a_json_object, "geometry");
    node.window = get_attribute_value<std::optional<std::uint64_t>>(a_json_object, "window");
    node.window_properties = extract_window_properties(a_json_object);
    node.is_urgent = get_attribute_value<bool>(a_json_object, "urgent");
    node.is_focused = get_attribute_value<bool>(a_json_object, "focused");
    node.fullscreen_mode = extract_fullscreen_mode(a_json_object);
    node.marks = extract_marks(a_json_object);

    // Extract list of node IDs that have focus.
    assert(a_json_object.HasMember("focus"));
    assert(a_json_object["focus"].IsArray());
    for (const auto& node_ID : a_json_object["focus"].GetArray())
    {
        assert(node_ID.IsUint64());
        node.focus.push_back(node_ID.GetUint64());
    }

    // Recursively call this function to extract info about tilling child nodes.
    assert(a_json_object.HasMember("nodes"));
    assert(a_json_object["nodes"].IsArray());
    for (const auto& node_object : a_json_object["nodes"].GetArray())
    {
        node.nodes.push_back(extract_tree(node_object));
    }

    // Recursively call this function to extract info about floating child nodes.
    assert(a_json_object.HasMember("floating_nodes"));
    assert(a_json_object["floating_nodes"].IsArray());
    for (const auto& node_object : a_json_object["floating_nodes"].GetArray())
    {
        node.floating_nodes.push_back(extract_tree(node_object));
    }

    return node;
}
