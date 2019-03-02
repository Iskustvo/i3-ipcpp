// i3-ipc++ headers.
#include "i3_ipc.hpp"
#include "i3_containers.hpp"

// C++ headers.
#include <vector>
#include <string>
#include <iostream>

int main()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Request info about outpus that i3 sees.
    std::vector<i3_containers::output> outputs = i3.get_outputs();

    // Write received output info.
    for (const auto& output : outputs)
    {
        std::cout << std::boolalpha
                  << "Name: \"" << output.name << "\"\n"
                  << "Is active: " << output.is_active << "\n"
                  << "Is primary: " << output.is_primary << "\n"
                  << "Output coordinates: {" << output.rect.x << ", "
                                             << output.rect.y << ", "
                                             << output.rect.width << ", "
                                             << output.rect.height << "}\n"
                  << "Current workspace: \"" << output.current_workspace.value_or("None") << "\"\n" << std::endl;
    }

    return 0;
}
