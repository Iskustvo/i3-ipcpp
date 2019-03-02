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
 * \file i3_ipc_exception.hpp
 *
 * Defines "i3_ipc_exception" class used for throwing errors from "i3-ipc++" library.
 */

#ifndef I3_IPC_EXCEPTION_HPP
#define I3_IPC_EXCEPTION_HPP

// C++ headers.
#include <string>
#include <exception>
#include <string_view>

/**
 * \brief   Exception class used only for internal problems of "i3-ipc++" library.
 */
class i3_ipc_exception : public std::exception
{

public:

    /**
     * \brief                        Constructs exception class with error message.
     *
     * \param [in] a_error_message   Error message used to explain what went wrong.
     */
    i3_ipc_exception(std::string_view a_error_message);

    /**
     * \brief    Function explaining what went wrong.
     *
     * \return   Error message.
     */
    virtual const char* what() const noexcept;

protected:

    std::string m_error_message; /**< Error message explaining what went wrong. */

};

#endif // I3_IPC_EXCEPTION_HPP
