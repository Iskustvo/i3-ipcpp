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
 * \file i3_ipc-requests.cpp
 *
 * Defines member functions of "i3_ipc" class used for synchronous requests.
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
#include <utility>
#include <string_view>

// C headers.
#include <cstdint>

namespace
{
    /**
     * \brief                       Sends the specified request to i3 and returns its response.
     *
     * \param [in] a_socket         Socket throught which message exchange will happen.
     *
     * \param [in] a_type           Type of the request.
     *
     * \param [in] a_payload        Optional content of the message.
     *
     * \return                      Type of reply and its content.
     *
     * \throws std::system_error    When system error occurs while reading/writing from/to "a_socket".
     *
     * \throws i3_ipc_bad_message   When i3's response message is invalid.
     */
    std::string send_request(int a_socket,
                             i3_message_type a_type,
                             const std::optional<std::string_view>& a_payload = std::nullopt)
    {
        i3_message::send(a_socket, a_type, a_payload);
        const std::pair<i3_message_type, std::string> response = i3_message::receive(a_socket);

        if (response.first != a_type)
        {
            throw i3_ipc_bad_message("Wrong message type!\n"
                                     "Expected: " + std::to_string(static_cast<std::uint32_t>(a_type)) + "\n"
                                     "Received: " + std::to_string(static_cast<std::uint32_t>(response.first)));
        }

        return response.second;
    }
} // Unnamed namespace

void i3_ipc::execute_commands(std::string_view a_commands) const
{
    if (a_commands.empty())
    {
        return;
    }

    const std::string response = send_request(m_request_socket, i3_message_type::command, a_commands);
    i3_json_parser::parse_command_response(response.c_str());
}

std::vector<i3_containers::workspace> i3_ipc::get_workspaces() const
{
    const std::string response = send_request(m_request_socket, i3_message_type::workspaces);
    return i3_json_parser::parse_workspaces(response.c_str());
}

std::vector<i3_containers::output> i3_ipc::get_outputs() const
{
    const std::string response = send_request(m_request_socket, i3_message_type::outputs);
    return i3_json_parser::parse_outputs(response.c_str());
}

i3_containers::node i3_ipc::get_tree() const
{
    const std::string response = send_request(m_request_socket, i3_message_type::tree);
    return i3_json_parser::parse_tree(response.c_str());
}

std::vector<std::string> i3_ipc::get_marks() const
{
    const std::string response = send_request(m_request_socket, i3_message_type::marks);
    return i3_json_parser::parse_marks(response.c_str());
}

std::vector<std::string> i3_ipc::get_bar_IDs() const
{
    const std::string response = send_request(m_request_socket, i3_message_type::bar_config);
    return i3_json_parser::parse_bar_names(response.c_str());
}

i3_containers::bar_config i3_ipc::get_bar_config(std::string_view a_bar_ID) const
{
    const std::string response = send_request(m_request_socket, i3_message_type::bar_config, a_bar_ID);

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
    const std::string response = send_request(m_request_socket, i3_message_type::version);
    return i3_json_parser::parse_version(response.c_str());
}

std::vector<std::string> i3_ipc::get_binding_modes() const
{
    const std::string response = send_request(m_request_socket, i3_message_type::binding_modes);
    return i3_json_parser::parse_binding_modes(response.c_str());
}

std::string i3_ipc::get_config() const
{
    const std::string response = send_request(m_request_socket, i3_message_type::config);
    return i3_json_parser::parse_config(response.c_str());
}

void i3_ipc::send_tick(const std::optional<std::string_view>& a_payload) const
{
    const std::string response = send_request(m_request_socket, i3_message_type::tick, a_payload);
    i3_json_parser::parse_tick_response(response.c_str());
}

void i3_ipc::sync(std::uint32_t a_window, std::uint32_t a_random) const
{
    const std::string payload = "{\"window\":" + std::to_string(a_window) + ","
                                 "\"random\":" + std::to_string(a_random) + "}";
    const std::string response = send_request(m_request_socket, i3_message_type::sync, payload);
    i3_json_parser::parse_sync_response(response.c_str());
}
