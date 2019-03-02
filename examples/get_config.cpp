// i3-ipc++ headers.
#include "i3_ipc.hpp"

// C++ headers.
#include <string>
#include <iostream>

int main()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Get config used by i3 and write it to stdout.
    const std::string config = i3.get_config();
    std::cout << "Last loaded config:\n" << config << std::endl;

    return 0;
}
