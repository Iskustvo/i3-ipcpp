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
    command = I3_IPC_MESSAGE_TYPE_RUN_COMMAND,               /**< Tell i3 to execute command(s).                      */
    workspaces = I3_IPC_MESSAGE_TYPE_GET_WORKSPACES,         /**< Get info about workspaces in i3.                    */
    subscribe = I3_IPC_MESSAGE_TYPE_SUBSCRIBE,               /**< Subscribe to event from i3.                         */
    outputs = I3_IPC_MESSAGE_TYPE_GET_OUTPUTS,               /**< Get info about avalible outputs in i3.              */
    tree = I3_IPC_MESSAGE_TYPE_GET_TREE,                     /**< Get info about internal node tree in i3.            */
    marks = I3_IPC_MESSAGE_TYPE_GET_MARKS,                   /**< Get marks that i3 uses.                             */
    bar_config = I3_IPC_MESSAGE_TYPE_GET_BAR_CONFIG,         /**< Get bar configuration that i3 uses.                 */
    version = I3_IPC_MESSAGE_TYPE_GET_VERSION,               /**< Get version of i3.                                  */
    binding_modes = I3_IPC_MESSAGE_TYPE_GET_BINDING_MODES,   /**< Get info about avalible binding modes in i3.        */
    config = I3_IPC_MESSAGE_TYPE_GET_CONFIG,                 /**< Get last loaded configuration from i3.              */
    tick = I3_IPC_MESSAGE_TYPE_SEND_TICK,                    /**< Tell i3 to broadcast given message to other IPCs.   */
    sync = I3_IPC_MESSAGE_TYPE_SYNC,                         /**< TODO */

    // Event message types.
    workspace_event = I3_IPC_EVENT_WORKSPACE,                /**< Get notified when there are changes in workspaces.  */
    output_event = I3_IPC_EVENT_OUTPUT,                      /**< Get notified when there are changes in outputs.     */
    mode_event = I3_IPC_EVENT_MODE,                          /**< Get notified when binding mode changes.             */
    window_event = I3_IPC_EVENT_WINDOW,                      /**< Get notified when there are changes in windows.     */
    bar_config_update_event = I3_IPC_EVENT_BARCONFIG_UPDATE, /**< Get notified when bar configuration is updated.     */
    binding_event = I3_IPC_EVENT_BINDING,                    /**< Get notified when binding event was triggered.      */
    shutdown_event = I3_IPC_EVENT_SHUTDOWN,                  /**< Get notified when i3 is about to restart/shutdown.  */
    tick_event = I3_IPC_EVENT_TICK                           /**< Get notified when IPCs broadcast message through i3.*/
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
