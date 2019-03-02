// i3-ipc++ headers.
#include "i3_ipc.hpp"
#include "i3_containers.hpp"

// C++ headers.
#include <iostream>

int main()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Get version from i3 and write it on stdout.
    const i3_containers::version version = i3.get_version();
    std::cout << "Version: " << static_cast<unsigned>(version.major)
                             << '.' << static_cast<unsigned>(version.minor)
                             << '.' << static_cast<unsigned>(version.patch) << '\n'
              << "Human readable: " << version.human_readable << '\n'
              << "Path to loaded config: " << version.loaded_config_file_name << std::endl;

    return 0;
}
