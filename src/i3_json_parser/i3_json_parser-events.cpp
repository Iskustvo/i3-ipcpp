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
 * \file i3_json_parser-events.cpp
 *
 * Defines member functions of "i3_json_parser" class that are used for parsing the data from events.
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
     * \brief                       Extracts info about workspace change from JSON object.
     *
     * \param [in] a_json_object    JSON object.
     *
     * \return                      Extracted workspace change from the "a_json_object".
     *
     * \throws i3_ipc_unsupported   When value of extracted workspace change is unknown to "i3-ipc++" library.
     */
    i3_containers::workspace_change extract_workspace_change(const rapidjson::Value& a_json_object)
    {
        const std::string_view workspace_change = get_attribute_value<const char*>(a_json_object, "change");
        if (workspace_change == "focus")
        {
            return i3_containers::workspace_change::focus;
        }
        else if (workspace_change == "init")
        {
            return i3_containers::workspace_change::init;
        }
        else if (workspace_change == "empty")
        {
            return i3_containers::workspace_change::empty;
        }
        else if (workspace_change == "urgent")
        {
            return i3_containers::workspace_change::urgent;
        }
        else if (workspace_change == "reload")
        {
            return i3_containers::workspace_change::reload;
        }
        else if (workspace_change == "rename")
        {
            return i3_containers::workspace_change::rename;
        }
        else if (workspace_change == "restored")
        {
            return i3_containers::workspace_change::restored;
        }
        else if (workspace_change == "move")
        {
            return i3_containers::workspace_change::move;
        }
        else
        {
            throw i3_ipc_unsupported("Unknown workspace change: \"" + std::string(workspace_change) + "\"");
        }
    }

    /**
     * \brief                       Extracts info about output change from JSON object.
     *
     * \param [in] a_json_object    JSON object.
     *
     * \return                      Extracted output change from the "a_json_object".
     *
     * \throws i3_ipc_unsupported   When value of extracted output change is unknown to "i3-ipc++" library.
     */
    i3_containers::output_change extract_output_change(const rapidjson::Value& a_json_object)
    {
        const std::string_view output_change = get_attribute_value<const char*>(a_json_object, "change");
        if (output_change == "unspecified")
        {
            return i3_containers::output_change::unspecified;
        }
        else
        {
            throw i3_ipc_unsupported("Unknown output change: \"" + std::string(output_change) + "\"");
        }
    }

    /**
     * \brief                       Extracts info about window change from JSON object.
     *
     * \param [in] a_json_object    JSON object.
     *
     * \return                      Extracted window change from the "a_json_object".
     *
     * \throws i3_ipc_unsupported   When value of extracted window change is unknown to "i3-ipc++" library.
     */
    i3_containers::window_change extract_window_change(const rapidjson::Value& a_json_object)
    {
        const std::string_view window_change = get_attribute_value<const char*>(a_json_object, "change");
        if (window_change == "new")
        {
            return i3_containers::window_change::create;
        }
        else if (window_change == "close")
        {
            return i3_containers::window_change::close;
        }
        else if (window_change == "focus")
        {
            return i3_containers::window_change::focus;
        }
        else if (window_change == "title")
        {
            return i3_containers::window_change::title;
        }
        else if (window_change == "fullscreen_mode")
        {
            return i3_containers::window_change::fullscreen_mode;
        }
        else if (window_change == "move")
        {
            return i3_containers::window_change::move;
        }
        else if (window_change == "floating")
        {
            return i3_containers::window_change::floating;
        }
        else if (window_change == "urgent")
        {
            return i3_containers::window_change::urgent;
        }
        else if (window_change == "mark")
        {
            return i3_containers::window_change::mark;
        }
        else
        {
            throw i3_ipc_unsupported("Unknown window change: \"" + std::string(window_change) + "\"");
        }
    }

    /**
     * \brief                       Extracts info about binding change from JSON object.
     *
     * \param [in] a_json_object    JSON object.
     *
     * \return                      Extracted binding change from the "a_json_object".
     *
     * \throws i3_ipc_unsupported   When value of extracted binding change is unknown to "i3-ipc++" library.
     */
    i3_containers::binding_change extract_binding_change(const rapidjson::Value& a_json_object)
    {
        const std::string_view binding_change = get_attribute_value<const char*>(a_json_object, "change");
        if (binding_change == "run")
        {
            return i3_containers::binding_change::run;
        }
        else
        {
            throw i3_ipc_unsupported("Unknown binding change: \"" + std::string(binding_change) + "\"");
        }
    }

    /**
     * \brief                       Extracts info about binding input type from JSON object.
     *
     * \param [in] a_json_object    JSON object.
     *
     * \return                      Extracted binding input type from the "a_json_object".
     *
     * \throws i3_ipc_unsupported   When value of extracted binding input type is unknown to "i3-ipc++" library.
     */
    i3_containers::binding_input_type extract_binding_input_type(const rapidjson::Value& a_json_object)
    {
        const std::string_view binding_input_type = get_attribute_value<const char*>(a_json_object, "input_type");
        if (binding_input_type == "keyboard")
        {
            return i3_containers::binding_input_type::keyboard;
        }
        else if (binding_input_type == "mouse")
        {
            return i3_containers::binding_input_type::mouse;
        }
        else
        {
            throw i3_ipc_unsupported("Unknown binding input type: \"" + std::string(binding_input_type) + "\"");
        }
    }

    /**
     * \brief                       Extracts binding info from JSON object.
     *
     * \param [in] a_json_object    JSON object.
     *
     * \return                      Extracted binding info from the "a_json_object".
     *
     * \throws i3_ipc_unsupported   When value of extracted binding input type is unknown to "i3-ipc++" library.
     */
    i3_containers::binding_info extract_binding_info(const rapidjson::Value& a_json_object)
    {
        assert(a_json_object.IsObject());
        assert(a_json_object.HasMember("binding"));

        const rapidjson::Value& binding_object = a_json_object["binding"];

        i3_containers::binding_info binding_info;
        binding_info.command = get_attribute_value<const char*>(binding_object, "command");

        assert(binding_object.HasMember("event_state_mask"));
        assert(binding_object["event_state_mask"].IsArray());
        for (const auto& event_state_mask : binding_object["event_state_mask"].GetArray())
        {
            assert(event_state_mask.IsString());
            binding_info.event_state_mask.push_back(event_state_mask.GetString());
        }

        binding_info.input_code = get_attribute_value<std::optional<std::uint8_t>>(binding_object, "input_code");
        const auto& tmp = get_attribute_value<std::optional<const char*>>(binding_object, "symbol");
        binding_info.symbol = tmp ? std::make_optional(**tmp) : std::nullopt;
        binding_info.input_type = extract_binding_input_type(binding_object);

        return binding_info;
    }

    /**
     * \brief                       Extracts info about shutdown type from JSON object.
     *
     * \param [in] a_json_object    JSON object.
     *
     * \return                      Extracted shutdown type from the "a_json_object".
     *
     * \throws i3_ipc_unsupported   When value of extracted shutdown type is unknown to "i3-ipc++" library.
     */
    i3_containers::shutdown_type extract_shutdown_type(const rapidjson::Value& a_json_object)
    {
        const std::string_view shutdown_type = get_attribute_value<const char*>(a_json_object, "change");
        if (shutdown_type == "restart")
        {
            return i3_containers::shutdown_type::restart;
        }
        else if (shutdown_type == "exit")
        {
            return i3_containers::shutdown_type::exit;
        }
        else
        {
            throw i3_ipc_unsupported("Unknown shutdown type: \"" + std::string(shutdown_type) + "\"");
        }
    }
} // Unnamed namespace.

void i3_json_parser::parse_subscribe_response(const char* a_json_string)
{
    if (!get_attribute_value<bool>(parse_json(a_json_string), "success"))
    {
        throw i3_ipc_invalid_argument("i3 declined subscription!");
    }
}

i3_containers::workspace_event i3_json_parser::parse_workspace_event(const char* a_json_string)
{
    const rapidjson::Document& json_object = parse_json(a_json_string);

    i3_containers::workspace_event workspace_event;
    workspace_event.change = extract_workspace_change(json_object);

    if (!json_object.HasMember("old") || json_object["old"].IsNull())
    {
        workspace_event.old = std::nullopt;
    }
    else
    {
        workspace_event.old = std::make_optional(extract_tree(json_object["old"]));
    }

    if (!json_object.HasMember("current") || json_object["current"].IsNull())
    {
        workspace_event.current = std::nullopt;
    }
    else
    {
        workspace_event.current = std::make_optional(extract_tree(json_object["current"]));
    }

    return workspace_event;
}

i3_containers::output_event i3_json_parser::parse_output_event(const char* a_json_string)
{
    return { extract_output_change(parse_json(a_json_string)) };
}

i3_containers::mode_event i3_json_parser::parse_mode_event(const char* a_json_string)
{
    const rapidjson::Document& json_object = parse_json(a_json_string);

    i3_containers::mode_event mode_event;
    mode_event.change = get_attribute_value<const char*>(json_object, "change");
    mode_event.pango_markup = get_attribute_value<bool>(json_object, "pango_markup");

    return mode_event;
}

i3_containers::window_event i3_json_parser::parse_window_event(const char* a_json_string)
{
    const rapidjson::Document& json_object = parse_json(a_json_string);

    i3_containers::window_event window_event;
    window_event.change = extract_window_change(json_object);
    window_event.container = extract_tree(json_object["container"]);

    return window_event;
}

i3_containers::binding_event i3_json_parser::parse_binding_event(const char* a_json_string)
{
    const rapidjson::Document& json_object = parse_json(a_json_string);

    i3_containers::binding_event binding_event;
    binding_event.change = extract_binding_change(json_object);
    binding_event.binding = extract_binding_info(json_object);

    return binding_event;
}

i3_containers::shutdown_event i3_json_parser::parse_shutdown_event(const char* a_json_string)
{
    return { extract_shutdown_type(parse_json(a_json_string)) };
}

i3_containers::tick_event i3_json_parser::parse_tick_event(const char* a_json_string)
{
    const rapidjson::Document& json_object = parse_json(a_json_string);

    i3_containers::tick_event tick_event;
    tick_event.is_first = get_attribute_value<bool>(json_object, "first");
    tick_event.payload = get_attribute_value<std::optional<const char*>>(json_object, "payload");

    return tick_event;
}
