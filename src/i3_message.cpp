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
 * \file    i3_message.cpp
 *
 * \brief   Contains the implementation of the functions from i3_message namespace.
 */

// Library headers.
#include "i3_message.hpp"
#include "i3_ipc_bad_message.hpp"

// External library headers.
#include <i3/ipc.h>

// C++ headers.
#include <vector>
#include <string>
#include <optional>
#include <string_view>
#include <type_traits>
#include <system_error>

// C headers.
#include <cstdint>
#include <cstring>
#include <unistd.h>

namespace
{
    constexpr std::string_view MAGIC_STRING = I3_IPC_MAGIC; /**< Magic string used for communication with i3. */
} // Unnamed namespace.

void i3_message::send(int a_socket, i3_message::type a_type, const std::optional<std::string_view>& a_payload)
{
    const auto message_type = static_cast<std::underlying_type_t<decltype(a_type)>>(a_type);
    const std::uint32_t payload_size = a_payload ? static_cast<std::uint32_t>(a_payload->size()) : 0;

    // Allocate vector big enough for whole message.
    std::vector<std::uint8_t> message(MAGIC_STRING.size() + sizeof(message_type) + sizeof(payload_size) + payload_size);

    // Store magic string, payload size, message type and payload in allocated vector.
    memcpy(message.data(), MAGIC_STRING.data(), MAGIC_STRING.size());
    memcpy(message.data() + MAGIC_STRING.size(), &payload_size, sizeof(payload_size));
    memcpy(message.data() + MAGIC_STRING.size() + sizeof(payload_size), &message_type, sizeof(message_type));
    if (a_payload)
    {
        constexpr std::uint32_t offset = MAGIC_STRING.size() + sizeof(payload_size) + sizeof(message_type);
        memcpy(message.data() + offset, a_payload->data(), payload_size);
    }

    // Send message through a socket.
    if (write(a_socket, message.data(), message.size()) == -1)
    {
        throw std::system_error(errno, std::generic_category());
    }
}

i3_message::response i3_message::receive(int a_socket)
{
    // Read message header.
    i3_ipc_header_t header;
    if (read(a_socket, &header, sizeof(header)) == -1)
    {
        throw std::system_error(errno, std::generic_category());
    }

    // Check if magic string is valid.
    if (const std::string_view magic_string(header.magic, MAGIC_STRING.size()); magic_string != MAGIC_STRING)
    {
        throw i3_ipc_bad_message("Bad magic string!\n"
                                 "Expected: " + std::string(MAGIC_STRING) + "\n"
                                 "Received: " + std::string(magic_string));
    }

    // Read message payload if it exists.
    std::vector<char> buffer(header.size);
    if (buffer.size() && read(a_socket, buffer.data(), buffer.size()) == -1)
    {
        throw std::system_error(errno, std::generic_category());
    }

    return { static_cast<i3_message::type>(header.type), std::string(buffer.data(), buffer.size()) };
}
