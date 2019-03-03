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
#include <type_traits>

// C headers.
#include <cstdint>
#include <cassert>

namespace
{
    /**
     * \brief                     Creates string in JSON format for given event type.
     *
     * \param [in] a_event_type   Event type for subscription.
     *
     * \return                    String in JSON format representing the subscription request for i3.
     */
    const char* create_json_subscription_request(i3_ipc::event_type a_event_type)
    {
        switch(a_event_type)
        {
            case i3_ipc::event_type::workspace:         return "[\"workspace\"]";
            case i3_ipc::event_type::output:            return "[\"output\"]";
            case i3_ipc::event_type::mode:              return "[\"mode\"]";
            case i3_ipc::event_type::window:            return "[\"window\"]";
            case i3_ipc::event_type::bar_config_update: return "[\"barconfig_update\"]";
            case i3_ipc::event_type::binding:           return "[\"binding\"]";
            case i3_ipc::event_type::shutdown:          return "[\"shutdown\"]";
            case i3_ipc::event_type::tick:              return "[\"tick\"]";

            default:
                assert("Unhandled type of i3_event!" && 0);
                return nullptr;
        }
    }
} // Unnamed namespace.

void i3_ipc::subscribe(event_type a_event_type, const i3_callback& a_callback)
{
    // Subscription request should be sent to i3 even when we are already subscribed!
    // This is done to ensure that ONLY the events that happend before subscription
    // will end up in event queue before we set new callback function.
    const char* subscription_request = create_json_subscription_request(a_event_type);
    i3_message::send(m_event_socket, i3_message_type::subscribe, subscription_request);

    // Read messages from event socket until subscription response is read.
    // Every event read in the meantime should be parsed and pushed to event queue for later handling.
    std::pair<i3_message_type, std::string> response = i3_message::receive(m_event_socket);
    while (response.first != i3_message_type::subscribe)
    {
        i3_event event;
        try
        {
            event = i3_json_parser::parse_event(response.first, response.second.c_str());
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

        response = i3_message::receive(m_event_socket);
    }

    // Check and throw i3_ipc_invalid_argument if i3 declined subscription.
    i3_json_parser::parse_subscribe_response(response.second.c_str());

    // Once the subscription is successful, push the callback to event queue to be used at appropriate time.
    m_event_queue.push(a_callback);
}

void i3_ipc::on_workspace_event(const std::function<void(const i3_containers::workspace_event&)>& a_callback)
{
    subscribe(event_type::workspace, a_callback);
}

void i3_ipc::on_output_event(const std::function<void(const i3_containers::output_event&)>& a_callback)
{
    subscribe(event_type::output, a_callback);
}

void i3_ipc::on_mode_event(const std::function<void(const i3_containers::mode_event&)>& a_callback)
{
    subscribe(event_type::mode, a_callback);
}

void i3_ipc::on_window_event(const std::function<void(const i3_containers::window_event&)>& a_callback)
{
    subscribe(event_type::window, a_callback);
}

void i3_ipc::on_bar_config_event(const std::function<void(const i3_containers::bar_config&)>& a_callback)
{
    subscribe(event_type::bar_config_update, a_callback);
}

void i3_ipc::on_binding_event(const std::function<void(const i3_containers::binding_event&)>& a_callback)
{
    subscribe(event_type::binding, a_callback);
}

void i3_ipc::on_shutdown_event(const std::function<void(const i3_containers::shutdown_event&)>& a_callback)
{
    subscribe(event_type::shutdown, a_callback);
}

void i3_ipc::on_tick_event(const std::function<void(const i3_containers::tick_event&)>& a_callback)
{
    subscribe(event_type::tick, a_callback);
}

/// \cond DO_NOT_DOCUMENT

/**
 * \brief      Checks if type T is std::variant.
 *
 * \tparam T   Type to be checked if it is std::variant.
 *
 * \return     True if T is std::variant, false otherwise.
 */
template<typename T> struct is_variant : std::false_type { };
template<typename... Ts> struct is_variant<std::variant<Ts...>> : std::true_type { };
// Commented out because clang complains about it being unused. Use is_variant<V>::value instead.
//template<typename T> constexpr bool is_variant_v = is_variant<T>::value;

/**
 * \brief          Checks if type T is in the Ts... list of types.
 *
 * \tparam T       Type to be search in the Ts... list.
 *
 * \tparam Ts...   List of types where T will be searched for.
 *
 * \return         True if T is in Ts... list, false otherwise.
 */
template<typename T, typename... Ts> struct is_one_of : std::disjunction<std::is_same<T, Ts>...> { };
template<typename T, typename... Ts> constexpr bool is_one_of_v = is_one_of<T, Ts...>::value;

/**
 * \brief      Returns the index of the specified alternative in variant.
 *
 * \tparam T   Type of one alternative in variant.
 *
 * \tparam V   Variant type.
 *
 * \return     Index of type T in variant V.
 */
template<typename T> struct literal { };
template<typename T, typename V> struct get_index
{
    static_assert(is_variant<V>::value, "Second argument must be std::variant!");
};
template<typename T, typename... Ts> struct get_index<T, std::variant<Ts...>>
{
    static_assert(is_one_of_v<T, Ts...>, "First argument type must be a valid variant alternative!");
    static constexpr std::uint8_t value = std::integral_constant<
                                                                 std::uint8_t,
                                                                 std::variant<literal<Ts>...>(literal<T>()).index()
                                                                >::value;
};
template<typename T, typename V> constexpr std::uint8_t get_index_v = get_index<T, V>::value;

/// \endcond

void i3_ipc::handle_subscription_event(const i3_callback& a_callback)
{
    std::visit([&](const auto& callback) -> void
               {
                   using callback_type = std::decay_t<decltype(callback)>;
                   constexpr std::uint8_t index = get_index_v<callback_type, i3_callback>;
                   m_callbacks[index] = callback;
               },
               a_callback);
}

i3_ipc::event_type i3_ipc::handle_i3_event(const i3_event& a_i3_event) const
{
    return std::visit([&](const auto& event) -> event_type
                      {
                          using i3_event_or_excpetion = std::decay_t<decltype(event)>;
                          if constexpr (std::is_same_v<i3_event_or_excpetion, i3_containers::event>)
                          {
                              return std::visit([&](const auto& i3_event_info) -> event_type
                                                {
                                                    using i3_event_type = std::decay_t<decltype(i3_event_info)>;
                                                    constexpr std::uint8_t index = get_index_v<i3_event_type,
                                                                                               i3_containers::event>;
                                                    std::get<index>(m_callbacks[index])(i3_event_info);
                                                    return static_cast<event_type>(index);
                                                },
                                                event);
                          }
                          else if constexpr (std::is_same_v<i3_event_or_excpetion, std::exception_ptr>)
                          {
                              std::rethrow_exception(event);
                          }
                          else
                          {
                              assert("Unhandled type of i3_event!" && 0);
                          }
                      },
                      a_i3_event);
}

std::optional<i3_ipc::event_type> i3_ipc::handle_i3_ipc_event(const i3_ipc_event& a_i3_ipc_event)
{
    return std::visit([&](const auto& event) -> std::optional<event_type>
                      {
                          using i3_ipc_event_type = std::decay_t<decltype(event)>;
                          if constexpr (std::is_same_v<i3_ipc_event_type, i3_callback>)
                          {
                               handle_subscription_event(event);
                               return std::nullopt;
                          }
                          else if constexpr (std::is_same_v<i3_ipc_event_type, i3_event>)
                          {
                               return handle_i3_event(event);
                          }
                          else
                          {
                              assert("Unhandled type of i3_ipc_event!" && 0);
                          }
                      },
                      a_i3_ipc_event);
}

std::optional<i3_ipc::event_type> i3_ipc::handle_next_i3_ipc_event()
{
    if (m_event_queue.empty())
    {
        const std::pair<i3_message_type, std::string> response = i3_message::receive(m_event_socket);
        const i3_event event = i3_json_parser::parse_event(response.first, response.second.c_str());
        return handle_i3_event(event);
    }

    const i3_ipc_event event = m_event_queue.front();
    m_event_queue.pop();

    return handle_i3_ipc_event(event);
}

i3_ipc::event_type i3_ipc::handle_next_event()
{
    std::optional<event_type> handled_event_type = handle_next_i3_ipc_event();
    while (!handled_event_type)
    {
        handled_event_type = handle_next_i3_ipc_event();
    }

    return *handled_event_type;
}
