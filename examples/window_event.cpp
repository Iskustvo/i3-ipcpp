// i3-ipc++ headers.
#include "i3_ipc.hpp"
#include "i3_containers.hpp"

// C++ headers.
#include <thread>
#include <chrono>
#include <iostream>

int main()
{
    // Create ipc object and connect it to running i3 process.
    i3_ipc i3;

    // Subscribe to i3 and store lambda function which will be executed when i3 detects change in window.
    i3.on_window_event([&](const i3_containers::window_event& event) -> void
                       {
                           if (event.change == i3_containers::window_change::fullscreen_mode)
                           {
                               std::cout << "Detected fullscreen mode change!\n"
                                            "Sleeping for 2 seconds." << std::endl;
                               std::this_thread::sleep_for(std::chrono::seconds(2));

                               std::cout << "Reverting fullscreen mode back!" << std::endl;
                               i3.execute_commands("fullscreen toggle");
                           }
                       }
                      );

    // Toggle window fullscreen. i3 will notice this and send window event.
    std::cout << "Toggling fullscreen mode!" << std::endl;
    i3.execute_commands("fullscreen toggle");

    // Handle window event - Lambda callback will notice that fullscreen mode changed and revert it.
    i3.handle_next_event();

    return 0;
}
