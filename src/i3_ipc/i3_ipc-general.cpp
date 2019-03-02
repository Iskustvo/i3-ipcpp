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
 * \file i3_ipc-general.cpp
 *
 * Defines member functions of "i3_ipc" class that have general purpose.
 */

// Library headers.
#include "i3_ipc.hpp"
#include "i3_ipc_exception.hpp"

// C++ headers.
#include <string>
#include <string_view>
#include <system_error>

// C headers.
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

namespace
{
    /**
     * \brief                      Executes "i3 --get-socketpath" and returns the path to i3's socket.
     *
     * \return                     The path to i3's socket.
     *
     * \throws std::bad_alloc      When bad allocation occurs while searching for the path to i3's socket.
     *
     * \throws std::system_error   When system error occurs while searching for the path to i3's socket.
     *
     * \throws i3_ipc_exception    When unable to find valid path to i3's socket. (Usually because i3 is not running.)
     */
    std::string find_i3_socket_path()
    {
        // Ask i3 for socket path by executing "i3 --get-socketpath".
        FILE* pipe_stream = popen("i3 --get-socketpath", "r");
        if (!pipe_stream)
        {
            if (errno)
            {
                throw std::system_error(errno, std::generic_category());
            }

            throw std::bad_alloc();
        }

        // Read socket path provided by i3.
        std::string socket_path;
        char buffer[512];
        while (fgets(buffer, sizeof(buffer), pipe_stream))
        {
            socket_path += buffer;
        }
        if (errno)
        {
            throw std::system_error(errno, std::generic_category());
        }

        // Close pipe stream after reading the socket path.
        if (pclose(pipe_stream))
        {
            if (errno)
            {
                throw std::system_error(errno, std::generic_category());
            }

            // Command "i3 --get_socketpath" exited unsuccessfully.
            throw i3_ipc_exception("Unable to find i3 socket path!");
        }

        if (socket_path.empty())
        {
            throw i3_ipc_exception("Path to i3's socket is empty!");
        }

        if (socket_path.back() == '\n')
        {
            socket_path.pop_back();
        }

        return socket_path;
    }

    /**
     * \brief                         Creates a new socket and connects it to the running i3 process.
     *
     * \param [in] a_i3_socket_path   Path to i3's socket.
     *
     * \return                        File descriptor for created socket that is now connected to running i3 process.
     *
     * \throws std::system_error      When system error occurs while creating new socket or connecting to i3.
     */
    int connect_to_i3(std::string_view a_i3_socket_path)
    {
        // Create a new socket.
        const int new_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
        if (new_socket == -1)
        {
            throw std::system_error(errno, std::generic_category());
        }

        // Connect new socket to socket from i3.
        struct sockaddr_un i3_address;
        memset(&i3_address, 0, sizeof(struct sockaddr_un));
        i3_address.sun_family = AF_LOCAL;
        strncpy(i3_address.sun_path, a_i3_socket_path.data(), sizeof(i3_address.sun_path) - 1);
        if (connect(new_socket, reinterpret_cast<struct sockaddr*>(&i3_address), sizeof(struct sockaddr_un)))
        {
            throw std::system_error(errno, std::generic_category());
        }

        return new_socket;
    }
} // Unnamed namespace.

i3_ipc::i3_ipc() : m_i3_socket_path(::find_i3_socket_path()),
                   m_request_socket(connect_to_i3(m_i3_socket_path)),
                   m_event_socket(connect_to_i3(m_i3_socket_path)),
                   m_event_subscriptions(),
                   m_callbacks()
{
}

i3_ipc::i3_ipc(std::string_view a_i3_socket_path) : m_i3_socket_path(a_i3_socket_path),
                                                    m_request_socket(connect_to_i3(m_i3_socket_path)),
                                                    m_event_socket(connect_to_i3(m_i3_socket_path)),
                                                    m_event_subscriptions(),
                                                    m_callbacks()
{
}

i3_ipc::i3_ipc(i3_ipc&& a_ipc) : m_i3_socket_path(std::move(a_ipc.m_i3_socket_path)),
                                 m_request_socket(std::move(a_ipc.m_request_socket)),
                                 m_event_socket(std::move(a_ipc.m_event_socket)),
                                 m_event_subscriptions(std::move(a_ipc.m_event_subscriptions)),
                                 m_callbacks(std::move(a_ipc.m_callbacks))
{
    // Make sure that a_ipc doesn't close sockets upon destruction.
    a_ipc.m_request_socket = 0;
    a_ipc.m_event_socket = 0;
}

i3_ipc& i3_ipc::operator=(i3_ipc&& a_ipc)
{
    // Cover self-asignment.
    if (this == &a_ipc)
    {
        return *this;
    }

    // Close opened sockets.
    this->~i3_ipc();

    // Use move constructor to take data from a_ipc.
    new(this) i3_ipc(std::move(a_ipc));
    return *this;
}

i3_ipc::~i3_ipc()
{
    if (m_request_socket)
    {
        close(m_request_socket);
    }
    if (m_event_socket)
    {
        close(m_event_socket);
    }
}

std::string i3_ipc::get_i3_socket_path() const
{
    return m_i3_socket_path;
}

void i3_ipc::set_i3_socket_path(std::string_view a_i3_socket_path)
{
    this->~i3_ipc();
    new(this) i3_ipc(a_i3_socket_path);
}

std::optional<std::string> i3_ipc::find_i3_socket_path()
{
    try
    {
        return std::make_optional(::find_i3_socket_path());
    }
    catch (const i3_ipc_exception&)
    {
        return std::nullopt;
    }
}
