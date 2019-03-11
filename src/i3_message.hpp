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
 * \file i3_message.hpp
 *
 * Defines enumerator "i3_message_type" which specifies all supported message types by the i3.<br>
 * Also defines "i3_message" namespace with functions specialized for communication with i3.
 */

#ifndef I3_MESSAGE_HPP
#define I3_MESSAGE_HPP

// External library headers.
#include <i3/ipc.h>

// C++ headers.
#include <string>
#include <utility>
#include <optional>
#include <string_view>

// C headers.
#include <cstdint>

/**
 * \brief Enumerator specifying all message types supported by i3.
 */
enum class i3_message_type : std::uint32_t
{
    // Request message types.
    command = static_cast<std::uint32_t>(I3_IPC_MESSAGE_TYPE_RUN_COMMAND),             /**< Execute command(s).       */
    workspaces = static_cast<std::uint32_t>(I3_IPC_MESSAGE_TYPE_GET_WORKSPACES),       /**< Get workspaces in i3.     */
    subscribe = static_cast<std::uint32_t>(I3_IPC_MESSAGE_TYPE_SUBSCRIBE),             /**< Subscribe to event.       */
    outputs = static_cast<std::uint32_t>(I3_IPC_MESSAGE_TYPE_GET_OUTPUTS),             /**< Get outputs in i3.        */
    tree = static_cast<std::uint32_t>(I3_IPC_MESSAGE_TYPE_GET_TREE),                   /**< Get node tree in i3.      */
    marks = static_cast<std::uint32_t>(I3_IPC_MESSAGE_TYPE_GET_MARKS),                 /**< Get marks that i3 uses.   */
    bar_config = static_cast<std::uint32_t>(I3_IPC_MESSAGE_TYPE_GET_BAR_CONFIG),       /**< Get bar configuration.    */
    version = static_cast<std::uint32_t>(I3_IPC_MESSAGE_TYPE_GET_VERSION),             /**< Get version of i3.        */
    binding_modes = static_cast<std::uint32_t>(I3_IPC_MESSAGE_TYPE_GET_BINDING_MODES), /**< Get binding modes.        */
    config = static_cast<std::uint32_t>(I3_IPC_MESSAGE_TYPE_GET_CONFIG),               /**< Get configuration.        */
    tick = static_cast<std::uint32_t>(I3_IPC_MESSAGE_TYPE_SEND_TICK),                  /**< Broadcast message.        */
    sync = static_cast<std::uint32_t>(I3_IPC_MESSAGE_TYPE_SYNC),                       /**< Sync i3 and X11 events.   */

    // Event message types.
    workspace_event = static_cast<std::uint32_t>(I3_IPC_EVENT_WORKSPACE),              /**< Listen workspaces events. */
    output_event = static_cast<std::uint32_t>(I3_IPC_EVENT_OUTPUT),                    /**< Listen outputs events.    */
    mode_event = static_cast<std::uint32_t>(I3_IPC_EVENT_MODE),                        /**< Listen to mode changes.   */
    window_event = static_cast<std::uint32_t>(I3_IPC_EVENT_WINDOW),                    /**< Listen window events.     */
    bar_config_event = static_cast<std::uint32_t>(I3_IPC_EVENT_BARCONFIG_UPDATE),      /**< Listen bar config events. */
    binding_event = static_cast<std::uint32_t>(I3_IPC_EVENT_BINDING),                  /**< Listen binding events.    */
    shutdown_event = static_cast<std::uint32_t>(I3_IPC_EVENT_SHUTDOWN),                /**< Listen shutdown events.   */
    tick_event = static_cast<std::uint32_t>(I3_IPC_EVENT_TICK)                         /**< Listen broadcasted events.*/
};

/**
 * Contains functions specialized for communication with i3.
 */
namespace i3_message
{
    /**
     * \brief                      Constructs message from given arguments and sends it to i3.
     *
     * \param [in] a_socket        Socket through which message will be sent to i3.
     *
     * \param [in] a_type          Type of the message.
     *
     * \param [in] a_payload       Content of the message.
     *
     * \throws std::system_error   When system error occurs while writing to "a_socket".
     */
    void send(int a_socket, i3_message_type a_type, const std::optional<std::string_view>& a_payload = std::nullopt);

    /**
     * \brief                       Receives message from i3 and extracts its type and content.
     *
     * \param [in] a_socket         Socket through which message will be received from i3.
     *
     * \return                      Message type and message content.
     *
     * \throws std::system_error    When system error occurs while reading from "a_socket".
     *
     * \throws i3_ipc_bad_message   When magic string sent by i3 is not appropriate.
     */
    std::pair<i3_message_type, std::string> receive(int a_socket);
} // namespace i3_message.

#endif // I3_MESSAGE_HPP
