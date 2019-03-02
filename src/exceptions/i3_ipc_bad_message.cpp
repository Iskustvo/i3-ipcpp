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
 * \file i3_ipc_bad_message.cpp
 *
 * Implements member functions of "i3_ipc_bad_message" class.
 */

// Library headers.
#include "i3_ipc_bad_message.hpp"

// C++ headers.
#include <string>

static const std::string message = "\n"
                                   "This error represents the inconsistency between i3 and i3-ipc++.\n"
                                   "Please consider opening an issue to help us to resolve it for everyone.\n"
                                   "GitHub issue tracker: https://github.com/Iskustvo/i3-ipcpp/issues\n";

i3_ipc_bad_message::i3_ipc_bad_message(const std::string& a_error_message) : i3_ipc_exception(a_error_message + message)
{
}

const char* i3_ipc_bad_message::what() const noexcept
{
    return m_error_message.c_str();
}
