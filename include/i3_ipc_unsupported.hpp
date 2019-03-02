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
 * \file i3_ipc_unsupported.hpp
 *
 * Defines "i3_ipc_unsupported" class used for throwing errors when some property has the value unknown to the library.
 */

#ifndef I3_IPC_UNSUPPORTED_HPP
#define I3_IPC_UNSUPPORTED_HPP

// Library headers.
#include "i3_ipc_exception.hpp"

// C++ headers.
#include <string_view>

/**
 * \brief   Exception class used only when some property has the value unknown to the library.
 */
class i3_ipc_unsupported : public i3_ipc_exception
{

public:

    /**
     * \brief                        Constructs exception class with error message.
     *
     * \param [in] a_error_message   Error message used to explain what went wrong.
     */
    i3_ipc_unsupported(const std::string& a_error_message);

    /**
     * \brief    Function explaining what went wrong.
     *
     * \return   Error message.
     */
    virtual const char* what() const noexcept;

};

#endif // I3_IPC_UNSUPPORTED_HPP
