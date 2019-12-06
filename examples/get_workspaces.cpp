// i3-ipc++ headers.
#include "i3_ipc.hpp"
#include "i3_containers.hpp"
#include "i3_ipc_bad_message.hpp"

// C++ headers.
#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include <exception>

int main()
{
    // Find path to socket of the running i3 process.
    const std::optional<std::string> i3_socket_path = i3_ipc::find_i3_socket_path();

    // If unable to get path to i3's socket, exit.
    if (!i3_socket_path)
    {
        std::cout << "Cannot find path to i3's socket!\n"
                  << "Is your i3 instance even running?" << std::endl;
        return 1;
    }

    std::vector<i3_containers::workspace> workspaces;
    try
    {
        // Create IPC object and connect it to running i3 process.
        // By using the constructor with provided path, there are fewer exception that it might throw.
        i3_ipc i3(*i3_socket_path);

        // Request info about existing workspaces from i3.
        workspaces = i3.get_workspaces();
    }
    catch(const i3_ipc_bad_message& exception)
    {
        // If this happens, the problem is most likely in bad implementation of i3-ipc++ library.
        std::cout << "\n" << exception.what() << std::endl;
        return 1;
    }
    catch(const std::exception& exception)
    {
        // Some nasty error happened...
        std::cout << "May the force be with you!\n\n"
                  << exception.what() << std::endl;
        return 1;
    }

    // List info about workspaces in i3.
    for (const auto& workspace : workspaces)
    {
        const std::string number = workspace.num ? std::to_string(*workspace.num) : "None";
        std::cout << std::boolalpha
                  << "Number: " << number << "\n"
                  << "Name: \"" << workspace.name << "\"\n"
                  << "Is visible: " << workspace.is_visible << "\n"
                  << "Is focused: " << workspace.is_focused << "\n"
                  << "Workspace coordinates: {" << workspace.rect.x << ", "
                                                << workspace.rect.y << ", "
                                                << workspace.rect.width << ", "
                                                << workspace.rect.height << "}\n"
                  << "Output: \"" << workspace.output << "\"\n"
                  << "Is urgent: " << workspace.is_urgent << "\n" << std::endl;
    }

    return 0;
}
