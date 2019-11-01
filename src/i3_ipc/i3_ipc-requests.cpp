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
 * \file    i3_ipc-requests.cpp
 *
 * \brief   Defines member functions of "i3_ipc" class used for synchronous requests.
 */

// Library headers.
#include "i3_ipc.hpp"
#include "i3_message.hpp"
#include "i3_containers.hpp"
#include "i3_json_parser.hpp"
#include "i3_ipc_bad_message.hpp"
#include "i3_ipc_invalid_argument.hpp"

// C++ headers.
#include <string>
#include <vector>
#include <optional>
#include <exception>
#include <string_view>

// C headers.
#include <cstdint>

std::string i3_ipc::send_request(i3_message::type a_type, const std::optional<std::string_view>& a_payload) const
{
    i3_message::send(m_socket, a_type, a_payload);

    // Read messages from socket until response is read.
    // Every event read in the meantime should be parsed and pushed to event queue for later handling.
    i3_message::response response = i3_message::receive(m_socket);
    while (response.message_type != a_type)
    {
        i3_event event;
        try
        {
            event = i3_json_parser::parse_event(response.message_type, response.payload.c_str());
        }
        catch(const i3_ipc_bad_message&)
        {
            throw;
        }
        catch(...)
        {
            event = std::current_exception();
        }
        m_event_queue.push(event);

        response = i3_message::receive(m_socket);
    }

    return response.payload;
}

void i3_ipc::execute_commands(std::string_view a_commands) const
{
    if (a_commands.empty())
    {
        return;
    }

    const std::string response = send_request(i3_message::type::command, a_commands);
    i3_json_parser::parse_command_response(response.c_str());
}

std::vector<i3_containers::workspace> i3_ipc::get_workspaces() const
{
    const std::string response = send_request(i3_message::type::workspaces);
    return i3_json_parser::parse_workspaces(response.c_str());
}

std::vector<i3_containers::output> i3_ipc::get_outputs() const
{
    const std::string response = send_request(i3_message::type::outputs);
    return i3_json_parser::parse_outputs(response.c_str());
}

i3_containers::node i3_ipc::get_tree() const
{
    const std::string response = send_request(i3_message::type::tree);
    return i3_json_parser::parse_tree(response.c_str());
}

std::vector<std::string> i3_ipc::get_marks() const
{
    const std::string response = send_request(i3_message::type::marks);
    return i3_json_parser::parse_marks(response.c_str());
}

std::vector<std::string> i3_ipc::get_bar_IDs() const
{
    const std::string response = send_request(i3_message::type::bar_config);
    return i3_json_parser::parse_bar_names(response.c_str());
}

i3_containers::bar_config i3_ipc::get_bar_config(std::string_view a_bar_ID) const
{
    const std::string response = send_request(i3_message::type::bar_config, a_bar_ID);

    try
    {
        return i3_json_parser::parse_bar_config(response.c_str());
    }
    catch(i3_ipc_invalid_argument& exception)
    {
        exception.set_error_message("i3 does not have a bar named \"" + std::string(a_bar_ID) + "\"!");
        throw;
    }
}

i3_containers::version i3_ipc::get_version() const
{
    const std::string response = send_request(i3_message::type::version);
    return i3_json_parser::parse_version(response.c_str());
}

std::vector<std::string> i3_ipc::get_binding_modes() const
{
    const std::string response = send_request(i3_message::type::binding_modes);
    return i3_json_parser::parse_binding_modes(response.c_str());
}

std::string i3_ipc::get_config() const
{
    const std::string response = send_request(i3_message::type::config);
    return i3_json_parser::parse_config(response.c_str());
}

void i3_ipc::send_tick(const std::optional<std::string_view>& a_payload) const
{
    const std::string response = send_request(i3_message::type::tick, a_payload);
    i3_json_parser::parse_tick_response(response.c_str());
}

void i3_ipc::sync(std::uint32_t a_window, std::uint32_t a_random) const
{
    const std::string payload = "{\"window\":" + std::to_string(a_window) + ","
                                 "\"random\":" + std::to_string(a_random) + "}";
    const std::string response = send_request(i3_message::type::sync, payload);
    i3_json_parser::parse_sync_response(response.c_str());
}
