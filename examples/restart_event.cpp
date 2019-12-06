// i3-ipc++ headers.
#include "i3_ipc.hpp"
#include "i3_containers.hpp"

// C++ headers.
#include <thread>
#include <chrono>
#include <iostream>

// Callback function which will be used when i3 is about to restart.
void restart_callback(const i3_containers::shutdown_event& event)
{
    if (event.change == i3_containers::shutdown_type::restart)
    {
        std::cout << "Caught restart event!\n" << std::endl;
    }
}

// This function will be called from a thread to restart i3.
void restart_thread()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Sleep 2 seconds and then restart i3.
    std::cout << "Restarting i3 in 2 seconds!" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    i3.execute_commands("restart");
}

int main()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Subscribe to i3 and store function which will be executed when i3 is about to restart.
    i3.on_shutdown_event(restart_callback);

    // Start a thread which will restart i3 in 2 seconds.
    std::thread thread(restart_thread);

    // Wait and handle restart event.
    i3.handle_next_event();

    thread.join();

    return 0;
}
