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
 * \file i3_json_parser-requests.cpp
 *
 * Defines member functions of "i3_json_parser" class that are used for parsing the data from requests.
 */

// Library headers.
#include "i3_json_parser.hpp"
#include "i3_containers.hpp"
#include "i3_ipc_unsupported.hpp"
#include "i3_ipc_invalid_argument.hpp"

// Library templates.
#include "i3_json_parser-utility.inl"

// External library headers.
#include <rapidjson/document.h>

// C++ headers.
#include <string>
#include <optional>
#include <string_view>

// C headers.
#include <cstdint>

namespace
{
    /**
     * \brief                       Extracts info about bar mode from JSON object.
     *
     * \param [in] a_json_object    JSON object.
     *
     * \return                      Extracted bar mode from the "a_json_object".
     *
     * \throws i3_ipc_unsupported   When value of extracted bar mode is unknown to "i3-ipc++" library.
     */
    i3_containers::bar_mode extract_bar_mode(const rapidjson::Value& a_json_object)
    {
        const std::string_view bar_mode = get_attribute_value<const char*>(a_json_object, "mode");
        if (bar_mode == "dock")
        {
            return i3_containers::bar_mode::dock;
        }
        else if (bar_mode == "hide")
        {
            return i3_containers::bar_mode::hide;
        }
        else
        {
            throw i3_ipc_unsupported("Unknown bar mode: \"" + std::string(bar_mode) + "\"");
        }
    }

    /**
     * \brief                       Extracts info about bar position from JSON object.
     *
     * \param [in] a_json_object    JSON object.
     *
     * \return                      Extracted bar position from the "a_json_object".
     *
     * \throws i3_ipc_unsupported   When value of extracted bar position is unknown to "i3-ipc++" library.
     */
    i3_containers::bar_position extract_bar_position(const rapidjson::Value& a_json_object)
    {
        const std::string_view bar_position = get_attribute_value<const char*>(a_json_object, "position");
        if (bar_position == "bottom")
        {
            return i3_containers::bar_position::bottom;
        }
        else if (bar_position == "top")
        {
            return i3_containers::bar_position::top;
        }
        else
        {
            throw i3_ipc_unsupported("Unknown bar position: \"" + std::string(bar_position) + "\"");
        }
    }

    /**
     * \brief                      Extracts info about bar colors from JSON object.
     *
     * \param [in] a_json_object   JSON object.
     *
     * \return                     Extracted bar colors from the "a_json_object".
     */
    i3_containers::bar_colors extract_bar_colors(const rapidjson::Value& a_json_object)
    {
        return
        {
            get_attribute_value<std::optional<const char*>>(a_json_object, "background"),
            get_attribute_value<std::optional<const char*>>(a_json_object, "focused_background"),

            get_attribute_value<std::optional<const char*>>(a_json_object, "statusline"),
            get_attribute_value<std::optional<const char*>>(a_json_object, "focused_statusline"),

            get_attribute_value<std::optional<const char*>>(a_json_object, "separator"),
            get_attribute_value<std::optional<const char*>>(a_json_object, "focused_separator"),

            get_attribute_value<std::optional<const char*>>(a_json_object, "focused_workspace_text"),
            get_attribute_value<std::optional<const char*>>(a_json_object, "focused_workspace_bg"),
            get_attribute_value<std::optional<const char*>>(a_json_object, "focused_workspace_border"),

            get_attribute_value<std::optional<const char*>>(a_json_object, "active_workspace_text"),
            get_attribute_value<std::optional<const char*>>(a_json_object, "active_workspace_bg"),
            get_attribute_value<std::optional<const char*>>(a_json_object, "active_workspace_border"),

            get_attribute_value<std::optional<const char*>>(a_json_object, "inactive_workspace_text"),
            get_attribute_value<std::optional<const char*>>(a_json_object, "inactive_workspace_bg"),
            get_attribute_value<std::optional<const char*>>(a_json_object, "inactive_workspace_border"),

            get_attribute_value<std::optional<const char*>>(a_json_object, "urgent_workspace_text"),
            get_attribute_value<std::optional<const char*>>(a_json_object, "urgent_workspace_bg"),
            get_attribute_value<std::optional<const char*>>(a_json_object, "urgent_workspace_border"),

            get_attribute_value<std::optional<const char*>>(a_json_object, "binding_mode_text"),
            get_attribute_value<std::optional<const char*>>(a_json_object, "binding_mode_bg"),
            get_attribute_value<std::optional<const char*>>(a_json_object, "binding_mode_border")
        };
    }
} // Unnamed namespace.

void i3_json_parser::parse_command_response(const char* a_json_string)
{
    const rapidjson::Document& json_array = parse_json(a_json_string);
    assert(json_array.IsArray());

    // Analyze command status for each command separately.
    for (const auto& command_status : json_array.GetArray())
    {
        // If i3 declined current command, extract error message and throw exception with it.
        if (!get_attribute_value<bool>(command_status, "success"))
        {
            const bool is_parse_error = get_attribute_value<bool>(command_status, "parse_error");
            const char* message = get_attribute_value<const char*>(command_status, "error");
            const char* command = get_attribute_value<const char*>(command_status, "input");
            const char* position = get_attribute_value<const char*>(command_status, "errorposition");
            const std::string error_message = std::string(is_parse_error ? "Parsing error!" : "") + '\n'
                                              + command + '\n'
                                              + position + "\n\n"
                                              + message + '\n';

            throw i3_ipc_invalid_argument(error_message);
        }
    }
}

std::vector<i3_containers::workspace> i3_json_parser::parse_workspaces(const char* a_json_string)
{
    const rapidjson::Document& json_array = parse_json(a_json_string);
    assert(json_array.IsArray());

    std::vector<i3_containers::workspace> workspaces;
    for (const auto& workspace_info : json_array.GetArray())
    {
        i3_containers::workspace workspace;
        const std::int8_t tmp = get_attribute_value<std::int8_t>(workspace_info, "num");
        workspace.num = tmp < 0 ? std::nullopt : std::make_optional(static_cast<std::uint8_t>(tmp));
        workspace.name = get_attribute_value<const char*>(workspace_info, "name");
        workspace.is_visible = get_attribute_value<bool>(workspace_info, "visible");
        workspace.is_focused = get_attribute_value<bool>(workspace_info, "focused");
        workspace.is_urgent = get_attribute_value<bool>(workspace_info, "urgent");
        workspace.rect = extract_rectangle(workspace_info, "rect");
        workspace.output = get_attribute_value<const char*>(workspace_info, "output");

        workspaces.push_back(workspace);
    }

    return workspaces;
}

std::vector<i3_containers::output> i3_json_parser::parse_outputs(const char* a_json_string)
{
    const rapidjson::Document& json_array = parse_json(a_json_string);
    assert(json_array.IsArray());

    std::vector<i3_containers::output> outputs;
    for (const auto& output_info : json_array.GetArray())
    {
        i3_containers::output output;
        output.name = get_attribute_value<const char*>(output_info, "name");
        output.is_active = get_attribute_value<bool>(output_info, "active");
        output.is_primary = get_attribute_value<bool>(output_info, "primary");
        output.current_workspace = get_attribute_value<std::optional<const char*>>(output_info, "current_workspace");
        output.rect = extract_rectangle(output_info, "rect");

        outputs.push_back(output);
    }

    return outputs;
}

i3_containers::node i3_json_parser::parse_tree(const char* a_json_string)
{
    return extract_tree(parse_json(a_json_string));
}

std::vector<std::string> i3_json_parser::parse_marks(const char* a_json_string)
{
    const rapidjson::Document& json_array = parse_json(a_json_string);
    assert(json_array.IsArray());

    std::vector<std::string> marks;
    for (const auto& mark : json_array.GetArray())
    {
        assert(mark.IsString());
        marks.push_back(mark.GetString());
    }

    return marks;
}

std::vector<std::string> i3_json_parser::parse_bar_names(const char* a_json_string)
{
    const rapidjson::Document& json_array = parse_json(a_json_string);
    assert(json_array.IsArray());

    std::vector<std::string> bar_names;
    for (const auto& bar_name : json_array.GetArray())
    {
        assert(bar_name.IsString());
        bar_names.push_back(bar_name.GetString());
    }

    return bar_names;
}

i3_containers::bar_config i3_json_parser::parse_bar_config(const char* a_json_string)
{
    const rapidjson::Document& json_object = parse_json(a_json_string);

    const std::optional<std::string_view> id = get_attribute_value<std::optional<const char*>>(json_object, "id");
    if (!id)
    {
        throw i3_ipc_invalid_argument("i3 didn't recognize requested bar ID!");
    }

    i3_containers::bar_config bar;
    bar.id = *id;
    bar.mode = extract_bar_mode(json_object);
    bar.position = extract_bar_position(json_object);
    bar.status_command = get_attribute_value<const char*>(json_object, "status_command");
    bar.font = get_attribute_value<const char*>(json_object, "font");
    bar.workspace_buttons = get_attribute_value<bool>(json_object, "workspace_buttons");
    bar.binding_mode_indicator = get_attribute_value<bool>(json_object, "binding_mode_indicator");
    bar.verbose = get_attribute_value<bool>(json_object, "verbose");
    bar.colors = extract_bar_colors(json_object);

    return bar;
}

i3_containers::version i3_json_parser::parse_version(const char* a_json_string)
{
    const rapidjson::Document& json_object = parse_json(a_json_string);

    i3_containers::version version;
    version.major = get_attribute_value<std::uint8_t>(json_object, "major");
    version.minor = get_attribute_value<std::uint8_t>(json_object, "minor");
    version.patch = get_attribute_value<std::uint8_t>(json_object, "patch");
    version.human_readable = get_attribute_value<const char*>(json_object, "human_readable");
    version.loaded_config_file_name = get_attribute_value<const char*>(json_object, "loaded_config_file_name");

    return version;
}

std::vector<std::string> i3_json_parser::parse_binding_modes(const char* a_json_string)
{
    const rapidjson::Document& json_array = parse_json(a_json_string);
    assert(json_array.IsArray());

    std::vector<std::string> binding_modes;
    for (const auto& binding_mode : json_array.GetArray())
    {
        assert(binding_mode.IsString());
        binding_modes.push_back(binding_mode.GetString());
    }

    return binding_modes;
}

std::string i3_json_parser::parse_config(const char* a_json_string)
{
    return get_attribute_value<const char*>(parse_json(a_json_string), "config");
}

void i3_json_parser::parse_tick_response(const char* a_json_string)
{
    if (!get_attribute_value<bool>(parse_json(a_json_string), "success"))
    {
        throw i3_ipc_invalid_argument("Sending TICK failed");
    }
}

void i3_json_parser::parse_sync_response(const char* a_json_string)
{
    if (!get_attribute_value<bool>(parse_json(a_json_string), "success"))
    {
        throw i3_ipc_invalid_argument("Sending SYNC failed");
    }
}
