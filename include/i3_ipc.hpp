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
 * \file i3_ipc.hpp
 *
 * Defines "i3_ipc" class used for over-socket communication with running i3 window manager.
 */

#ifndef I3_IPC_HPP
#define I3_IPC_HPP

// Library headers.
#include "i3_containers.hpp"

// C++ headers.
#include <vector>
#include <string>
#include <bitset>
#include <variant>
#include <functional>
#include <string_view>

// C headers.
#include <cstdint>

/**
 * This class is used for over-socket communication with running i3 window manager.<br>
 * Implementation of this class is spread over several files in "src/i3_ipc/" directory.(see README.md)
 */
class i3_ipc
{

public:

    /**
     * \brief                      Constructor which finds the path to i3's socket and establishes connections with it.
     *
     * \throws std::bad_alloc      When bad allocation occurs while searching for the path to i3's socket.
     *
     * \throws std::system_error   When system error occurs while searching for i3's socket or connecting to it.
     *
     * \throws i3_ipc_exception    When unable to find valid path to i3's socket. (Usually because i3 is not running.)
     */
    i3_ipc();

    /**
     * \brief                         Constructor which establishes connection with i3 over given socket path.
     *
     * \param [in] a_i3_socket_path   Path to i3's socket.
     *
     * \throws std::system_error      When system error occurs while connecting to i3's socket.
     */
    i3_ipc(std::string_view a_i3_socket_path);

    /// \cond DO_NOT_DOCUMENT
    i3_ipc(const i3_ipc& a_ipc) = delete;
    /// \endcond

    /**
     * \brief              Move constructor which forms the new object with data from existing one.
     *
     * \param [in] a_ipc   Existing object from which data will be taken.
     */
    i3_ipc(i3_ipc&& a_ipc);

    /// \cond DO_NOT_DOCUMENT
    i3_ipc& operator=(const i3_ipc& a_ipc) = delete;
    /// \endcond

    /**
     * \brief              Move assignment operator which takes data from right object and gives it to the left object.
     *
     * \param [in] a_ipc   Existing object from which data will be taken.
     *
     * \return             Reference of assigned variable.
     */
    i3_ipc& operator=(i3_ipc&& a_ipc);

    /**
     * \brief   Destructor which closes opened sockets upon object destruction.
     */
    ~i3_ipc();

    /**
     * \brief    Returns the copy of internally stored path to i3's socket.
     *
     * \return   Copy of internally stored path to i3's socket.
     */
    std::string get_i3_socket_path() const;

    /**
     * \brief                         Reinitializes the whole object with new path to i3's socket.
     *
     * \param [in] a_i3_socket_path   Path to i3's socket.
     *
     * \throws std::system_error      When system error occurs while connecting to i3's socket.
     */
    void set_i3_socket_path(std::string_view a_i3_socket_path);

    /**
     * \brief                      Executes "i3 --get-socketpath" and returns the path to i3's socket.
     *
     * \return                     The path to i3's socket or std::nullopt if unable to find it.
     *
     * \throws std::bad_alloc      When bad allocation occurs while searching for the path to i3's socket.
     *
     * \throws std::system_error   When system error occurs while searching for the path to i3's socket.
     */
    static std::optional<std::string> find_i3_socket_path();

    /**
     * \brief                            Tells i3 to execute given command(s).<br>
     *                                   NOTE: "restart" and "exit" are not implemented properly and will always throw.
     *
     * \param [in] a_commands            Semicolon-separated list of commands which i3 should execute.
     *
     * \throws std::system_error         When system error occurs while communicating through the socket.
     *
     * \throws i3_ipc_bad_message        When i3's response message is invalid.
     *
     * \throws i3_ipc_invalid_argument   When i3 declines to execute sent command(s).
     */
    void execute_commands(std::string_view a_commands) const;

    /**
     * \brief                       Gets info for existing workspaces in i3.
     *
     * \return                      Info about existing workspaces in i3.
     *
     * \throws std::system_error    When system error occurs while communicating through the socket.
     *
     * \throws i3_ipc_bad_message   When i3's response message is invalid.
     */
    std::vector<i3_containers::workspace> get_workspaces() const;

    /**
     * \brief                       Gets info for existing outputs in i3.
     *
     * \return                      Info about existing outputs in i3.
     *
     * \throws std::system_error    When system error occurs while communicating through the socket.
     *
     * \throws i3_ipc_bad_message   When i3's response message is invalid.
     */
    std::vector<i3_containers::output> get_outputs() const;

    /**
     * \brief                       Gets info for internal i3 node tree.
     *
     * \return                      Info about the root node in i3 node tree.
     *
     * \throws std::system_error    When system error occurs while communicating through the socket.
     *
     * \throws i3_ipc_bad_message   When i3's response message is invalid.
     *
     * \throws i3_ipc_unsupported   When value of some node property is unknown to "i3-ipc++" library.
     */
    i3_containers::node get_tree() const;

    /**
     * \brief                       Gets marks used by i3.
     *
     * \return                      Marks used by i3.
     *
     * \throws std::system_error    When system error occurs while communicating through the socket.
     *
     * \throws i3_ipc_bad_message   When i3's response message is invalid.
     */
    std::vector<std::string> get_marks() const;

    /**
     * \brief                       Gets IDs of status bars in i3.
     *
     * \return                      IDs of status bars in i3.
     *
     * \throws std::system_error    When system error occurs while communicating through the socket.
     *
     * \throws i3_ipc_bad_message   When i3's response message is invalid.
     */
    std::vector<std::string> get_bar_IDs() const;

    /**
     * \brief                            Gets info about specified bar from i3.
     *
     * \param [in] a_bar_ID              ID of the bar for which i3 should return info.
     *
     * \return                           Info about requested bar.
     *
     * \throws std::system_error         When system error occurs while communicating through the socket.
     *
     * \throws i3_ipc_bad_message        When i3's response message is invalid.
     *
     * \throws i3_ipc_unsupported        When value of some property is unknown to "i3-ipc++" library.
     *
     * \throws i3_ipc_invalid_argument   When i3 didn't recognize requested bar ID.
     */
    i3_containers::bar_config get_bar_config(std::string_view a_bar_ID) const;

    /**
     * \brief                       Gets the version of i3.
     *
     * \return                      Version of i3.
     *
     * \throws std::system_error    When system error occurs while communicating through the socket.
     *
     * \throws i3_ipc_bad_message   When i3's response message is invalid.
     */
    i3_containers::version get_version() const;

    /**
     * \brief                       Gets names of binding modes in i3.
     *
     * \return                      Names of binding modes in i3.
     *
     * \throws std::system_error    When system error occurs while communicating through the socket.
     *
     * \throws i3_ipc_bad_message   When i3's response message is invalid.
     */
    std::vector<std::string> get_binding_modes() const;

    /**
     * \brief                       Gets the last loaded configuration file from i3.
     *
     * \return                      Last loaded configuration file for i3.
     *
     * \throws std::system_error    When system error occurs while communicating through the socket.
     *
     * \throws i3_ipc_bad_message   When i3's response message is invalid.
     */
    std::string get_config() const;

    /**
     * \brief                            Sends tick to i3 to broadcast it over other IPCs.
     *
     * \param [in] a_payload             Message payload to be broadcasted to other IPCs.
     *
     * \throws std::system_error         When system error occurs while communicating through the socket.
     *
     * \throws i3_ipc_bad_message        When i3's response message is invalid.
     *
     * \throws i3_ipc_invalid_argument   When i3 declines to boradcast sent tick event.
     */
    void send_tick(const std::optional<std::string_view>& a_payload = std::nullopt) const;

    /**
     * \brief                            Tells i3 to send a message to X11 window after all events are handled.
     *
     * \param [in] a_window              ID of X11 window which should receive message after all events are handled.
     *
     * \param [in] a_random              Random number used for correlation.
     *
     * \throws std::system_error         When system error occurs while communicating through the socket.
     *
     * \throws i3_ipc_bad_message        When i3's response message is invalid.
     *
     * \throws i3_ipc_invalid_argument   When i3 declines to send sync event.
     */
    void sync(std::uint32_t a_window, std::uint32_t a_random) const;

    /**
     * \brief                      Subscribes to "workspace" event and stores callback function for it.
     *
     * \param [in] a_callback      Callback function which will be executed once "workspace" event is triggered.
     *
     * \throws std::system_error   When system error occurs while sending the subscription to i3.
     */
    void on_workspace_event(const std::function<void(const i3_containers::workspace_event&)>& a_callback);

    /**
     * \brief                      Subscribes to "output" event and stores callback function for it.
     *
     * \param [in] a_callback      Callback function which will be executed once "output" event is triggered.
     *
     * \throws std::system_error   When system error occurs while sending the subscription to i3.
     */
    void on_output_event(const std::function<void(const i3_containers::output_event&)>& a_callback);

    /**
     * \brief                      Subscribes to "mode" event and stores callback function for it.
     *
     * \param [in] a_callback      Callback function which will be executed once "mode" event is triggered.
     *
     * \throws std::system_error   When system error occurs while sending the subscription to i3.
     */
    void on_mode_event(const std::function<void(const i3_containers::mode_event&)>& a_callback);

    /**
     * \brief                      Subscribes to "window" event and stores callback function for it.
     *
     * \param [in] a_callback      Callback function which will be executed once "window" event is triggered.
     *
     * \throws std::system_error   When system error occurs while sending the subscription to i3.
     */
    void on_window_event(const std::function<void(const i3_containers::window_event&)>& a_callback);

    /**
     * \brief                      Subscribes to "bar_config" event and stores callback function for it.
     *
     * \param [in] a_callback      Callback function which will be executed once "bar_config" event is triggered.
     *
     * \throws std::system_error   When system error occurs while sending the subscription to i3.
     */
    void on_bar_config_event(const std::function<void(const i3_containers::bar_config&)>& a_callback);

    /**
     * \brief                      Subscribes to "binding" event and stores callback function for it.
     *
     * \param [in] a_callback      Callback function which will be executed once "binding" event is triggered.
     *
     * \throws std::system_error   When system error occurs while sending the subscription to i3.
     */
    void on_binding_event(const std::function<void(const i3_containers::binding_event&)>& a_callback);

    /**
     * \brief                      Subscribes to "shutdown" event and stores callback function for it.
     *
     * \param [in] a_callback      Callback function which will be executed once "shutdown" event is triggered.
     *
     * \throws std::system_error   When system error occurs while sending the subscription to i3.
     */
    void on_shutdown_event(const std::function<void(const i3_containers::shutdown_event&)>& a_callback);

    /**
     * \brief                      Subscribes to "tick" event and stores callback function for it.
     *
     * \param [in] a_callback      Callback function which will be executed once "tick" event is triggered.
     *
     * \throws std::system_error   When system error occurs while sending the subscription to i3.
     */
    void on_tick_event(const std::function<void(const i3_containers::tick_event&)>& a_callback);

    /**
     * \brief                       Parses the first event from event socket and executes callback stored for it.
     *
     * \throws std::system_error    When system error occurs while reading from event socket.
     *
     * \throws i3_ipc_bad_message   When read message is invalid.
     *
     * \throws i3_ipc_unsupported   When value of some node property is unknown to "i3-ipc++" library.
     */
    void handle_next_event() const;

private:

    /**
     * Enumerator used to describe the type of i3 events.
     */
    enum class event_type : std::uint8_t
    {
        workspace,         /**< Get notified when there are changes in workspaces.   */
        output,            /**< Get notified when there are changes in outputs.      */
        mode,              /**< Get notified when binding mode changes.              */
        window,            /**< Get notified when there are changes in windows.      */
        bar_config_update, /**< Get notified when bar configuration is updated.      */
        binding,           /**< Get notified when binding event was triggered.       */
        shutdown,          /**< Get notified when i3 is about to restart/shutdown.   */
        tick               /**< Get notified when IPCs broadcast message through i3. */
    };
    static constexpr std::uint8_t number_of_event_types = 8; /**< Number of supported event types. */

    /**
     * Type used to store the variant of any supported event callback.
     */
    using i3_callback = std::variant<
                                     std::function<void(const i3_containers::workspace_event&)>,
                                     std::function<void(const i3_containers::output_event&)>,
                                     std::function<void(const i3_containers::mode_event&)>,
                                     std::function<void(const i3_containers::window_event&)>,
                                     std::function<void(const i3_containers::bar_config_event&)>,
                                     std::function<void(const i3_containers::binding_event&)>,
                                     std::function<void(const i3_containers::shutdown_event&)>,
                                     std::function<void(const i3_containers::tick_event&)>
                                    >;
    static_assert(std::variant_size_v<i3_callback> == number_of_event_types, "Each event type must have one variant!");

    /**
     * \brief                      Sends event subscription to i3.
     *
     * \param [in] a_event_type    Event for which subscription request will be sent.
     *
     * \throws std::system_error   When system error occurs while sending the subscription to i3.
     */
    void subscribe(event_type a_event_type);

    std::string m_i3_socket_path;                               /**< Path to i3's socket.                          */
    int m_request_socket;                                       /**< Socket used for requests to i3.               */
    int m_event_socket;                                         /**< Socket used for event notifications from i3.  */
    std::bitset<number_of_event_types> m_event_subscriptions;   /**< List of flags indicating event subscriptions. */
    std::array<i3_callback, number_of_event_types> m_callbacks; /**< List of callback functions for i3 events.     */

}; // class i3_ipc.

#endif // I3_IPC_HPP
