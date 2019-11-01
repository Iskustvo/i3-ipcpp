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
 * \file    i3_ipc_bad_message.hpp
 *
 * \brief   Defines "i3_ipc_bad_message" class used for throwing errors about invalid messages received from i3.
 */

#ifndef I3_IPC_BAD_MESSAGE_HPP
#define I3_IPC_BAD_MESSAGE_HPP

// Library headers.
#include "i3_ipc_exception.hpp"

// C++ headers.
#include <string>

/**
 * \brief   Exception class used only when "i3-ipc++" library receives unexpected/invalid message from i3.
 */
class i3_ipc_bad_message : public i3_ipc_exception
{

public:

    /**
     * \brief                        Constructs exception class with error message.
     *
     * \param [in] a_error_message   Error message used to explain what went wrong.
     */
    i3_ipc_bad_message(const std::string& a_error_message);

    /**
     * \brief    Function explaining what went wrong.
     *
     * \return   Error message.
     */
    virtual const char* what() const noexcept;

};

#endif // I3_IPC_BAD_MESSAGE_HPP
