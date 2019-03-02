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
 * \file i3_ipc-events.cpp
 *
 * Defines member functions of "i3_ipc" class used for asynchronous events.
 */

// Library headers.
#include "i3_ipc.hpp"
#include "i3_message.hpp"
#include "i3_containers.hpp"
#include "i3_json_parser.hpp"
#include "i3_ipc_bad_message.hpp"

// C++ headers.
#include <string>
#include <utility>

// C headers.
#include <cstdint>
#include <cassert>

void i3_ipc::subscribe(event_type a_event_type)
{
    // No need to do anything in case of repeated subscription.
    const std::uint8_t subscription_index = static_cast<std::uint8_t>(a_event_type);
    if (m_event_subscriptions[subscription_index])
    {
        return;
    }

    m_event_subscriptions[subscription_index] = true;

    const char* event_json;
    switch(a_event_type)
    {
        case event_type::workspace:
            event_json = "[\"workspace\"]";
            break;

        case event_type::output:
            event_json = "[\"output\"]";
            break;

        case event_type::mode:
            event_json = "[\"mode\"]";
            break;

        case event_type::window:
            event_json = "[\"window\"]";
            break;

        case event_type::bar_config_update:
            event_json = "[\"barconfig_update\"]";
            break;

        case event_type::binding:
            event_json = "[\"binding\"]";
            break;

        case event_type::shutdown:
            event_json = "[\"shutdown\"]";
            break;

        case event_type::tick:
            event_json = "[\"tick\"]";
            break;
    }

    i3_message::send(m_event_socket, i3_message_type::subscribe, event_json);
}

void i3_ipc::on_workspace_event(const std::function<void(const i3_containers::workspace_event&)>& a_callback)
{
    subscribe(event_type::workspace);
    m_callbacks[static_cast<std::uint8_t>(event_type::workspace)] = a_callback;
}

void i3_ipc::on_output_event(const std::function<void(const i3_containers::output_event&)>& a_callback)
{
    subscribe(event_type::output);
    m_callbacks[static_cast<std::uint8_t>(event_type::output)] = a_callback;
}

void i3_ipc::on_mode_event(const std::function<void(const i3_containers::mode_event&)>& a_callback)
{
    subscribe(event_type::mode);
    m_callbacks[static_cast<std::uint8_t>(event_type::mode)] = a_callback;
}

void i3_ipc::on_window_event(const std::function<void(const i3_containers::window_event&)>& a_callback)
{
    subscribe(event_type::window);
    m_callbacks[static_cast<std::uint8_t>(event_type::window)] = a_callback;
}

void i3_ipc::on_bar_config_event(const std::function<void(const i3_containers::bar_config&)>& a_callback)
{
    subscribe(event_type::bar_config_update);
    m_callbacks[static_cast<std::uint8_t>(event_type::bar_config_update)] = a_callback;
}

void i3_ipc::on_binding_event(const std::function<void(const i3_containers::binding_event&)>& a_callback)
{
    subscribe(event_type::binding);
    m_callbacks[static_cast<std::uint8_t>(event_type::binding)] = a_callback;
}

void i3_ipc::on_shutdown_event(const std::function<void(const i3_containers::shutdown_event&)>& a_callback)
{
    subscribe(event_type::shutdown);
    m_callbacks[static_cast<std::uint8_t>(event_type::shutdown)] = a_callback;
}

void i3_ipc::on_tick_event(const std::function<void(const i3_containers::tick_event&)>& a_callback)
{
    subscribe(event_type::tick);
    m_callbacks[static_cast<std::uint8_t>(event_type::tick)] = a_callback;
}

void i3_ipc::handle_next_event() const
{
    // Read first event from i3's socket.
    std::pair <i3_message_type, std::string> response = i3_message::receive(m_event_socket);

    // Ignore responses from subscription requests.
    while(response.first == i3_message_type::subscribe)
    {
        /* FIXME: Since declined subscrption by i3 can't be handled properly with current implementation, there is no
           need to check the result. Once event queue is implemented and every subscription checks its response, then
           "parse_subscribe_response" should be used(but not here...). */
        //i3_json_parser::parse_subscribe_response(response.second.c_str());
        response = i3_message::receive(m_event_socket);
    }

    // Actually handle the next event.
    switch(response.first)
    {
        case i3_message_type::subscribe:
            assert("Subscription response not handled properly!" && 0);
            break;

        case i3_message_type::workspace_event:
        {
            constexpr std::uint8_t index = static_cast<std::uint8_t>(event_type::workspace);

            if (!m_event_subscriptions[index])
            {
                throw i3_ipc_bad_message("Received \"workspace\" event, but not subscribed to it!");
            }

            const i3_containers::workspace_event event = i3_json_parser::parse_workspace_event(response.second.c_str());
            assert(m_callbacks[index].index() == index);
            std::get<index>(m_callbacks[index])(event);
            break;
        }

        case i3_message_type::output_event:
        {
            constexpr std::uint8_t index = static_cast<std::uint8_t>(event_type::output);

            if (!m_event_subscriptions[index])
            {
                throw i3_ipc_bad_message("Received \"output\" event, but not subscribed to it!");
            }

            const i3_containers::output_event event = i3_json_parser::parse_output_event(response.second.c_str());
            assert(m_callbacks[index].index() == index);
            std::get<index>(m_callbacks[index])(event);
            break;
        }

        case i3_message_type::mode_event:
        {
            constexpr std::uint8_t index = static_cast<std::uint8_t>(event_type::mode);

            if (!m_event_subscriptions[index])
            {
                throw i3_ipc_bad_message("Received \"mode\" event, but not subscribed to it!");
            }

            const i3_containers::mode_event event = i3_json_parser::parse_mode_event(response.second.c_str());
            assert(m_callbacks[index].index() == index);
            std::get<index>(m_callbacks[index])(event);
            break;
        }

        case i3_message_type::window_event:
        {
            constexpr std::uint8_t index = static_cast<std::uint8_t>(event_type::window);

            if (!m_event_subscriptions[index])
            {
                throw i3_ipc_bad_message("Received \"window\" event, but not subscribed to it!");
            }

            const i3_containers::window_event event = i3_json_parser::parse_window_event(response.second.c_str());
            assert(m_callbacks[index].index() == index);
            std::get<index>(m_callbacks[index])(event);
            break;
        }

        case i3_message_type::bar_config_update_event:
        {
            constexpr std::uint8_t index = static_cast<std::uint8_t>(event_type::bar_config_update);

            if (!m_event_subscriptions[index])
            {
                throw i3_ipc_bad_message("Received \"bar_config\" event, but not subscribed to it!");
            }

            const i3_containers::bar_config event = i3_json_parser::parse_bar_config(response.second.c_str());
            assert(m_callbacks[index].index() == index);
            std::get<index>(m_callbacks[index])(event);
            break;
        }

        case i3_message_type::binding_event:
        {
            constexpr std::uint8_t index = static_cast<std::uint8_t>(event_type::binding);

            if (!m_event_subscriptions[index])
            {
                throw i3_ipc_bad_message("Received \"binding\" event, but not subscribed to it!");
            }

            const i3_containers::binding_event event = i3_json_parser::parse_binding_event(response.second.c_str());
            assert(m_callbacks[index].index() == index);
            std::get<index>(m_callbacks[index])(event);
            break;
        }

        case i3_message_type::shutdown_event:
        {
            constexpr std::uint8_t index = static_cast<std::uint8_t>(event_type::shutdown);

            if (!m_event_subscriptions[index])
            {
                throw i3_ipc_bad_message("Received \"shutdown\" event, but not subscribed to it!");
            }

            const i3_containers::shutdown_event event = i3_json_parser::parse_shutdown_event(response.second.c_str());
            assert(m_callbacks[index].index() == index);
            std::get<index>(m_callbacks[index])(event);
            break;
        }

        case i3_message_type::tick_event:
        {
            constexpr std::uint8_t index = static_cast<std::uint8_t>(event_type::tick);

            if (!m_event_subscriptions[index])
            {
                throw i3_ipc_bad_message("Received \"tick\" event, but not subscribed to it!");
            }

            const i3_containers::tick_event event = i3_json_parser::parse_tick_event(response.second.c_str());
            assert(m_callbacks[index].index() == index);
            std::get<index>(m_callbacks[index])(event);
            break;
        }

        default:
            const std::string message_type = std::to_string(static_cast<std::uint32_t>(response.first));
            throw i3_ipc_bad_message("Received unexpected message of type: " + message_type);
    }
}
