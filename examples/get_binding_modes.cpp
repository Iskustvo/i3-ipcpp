// i3-ipc++ headers.
#include "i3_ipc.hpp"

// C++ headers.
#include <vector>
#include <string>
#include <iostream>

int main()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Get avalible binding modes from i3 and write them to stdout.
    const std::vector<std::string> binding_modes = i3.get_binding_modes();
    std::cout << "Binding modes:";
    for (const auto& binding_mode : binding_modes)
    {
        std::cout << ' ' << binding_mode;
    }
    std::cout << std::endl;

    return 0;
}
