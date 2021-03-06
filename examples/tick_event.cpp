// i3-ipc++ headers.
#include "i3_ipc.hpp"

// C++ headers.
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <iostream>

// Callback function which will be used when tick event happens in i3.
void tick_callback(const i3_containers::tick_event& event)
{
    // Since i3 will send tick event just to confirm subscription, that one should be ignored.
    if (!event.is_first)
    {
        std::cout << "\nReceived tick event!" << std::endl;
        if (event.payload)
        {
            std::cout << "Its payload is: \"" << *event.payload << "\"" << std::endl;
        }
        else
        {
            std::cout << "Unfortunately, there was no payload in it" << std::endl;
        }
    }
}

// This function will be called from a thread to send tick events to i3.
void tick_sender_thread()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Send tick events to i3, one on every 2 seconds.
    const unsigned number_of_ticks_to_send = 5;
    for (unsigned counter = 0; counter < number_of_ticks_to_send; counter++)
    {
        const std::string message = "Hello " + std::to_string(counter);
        i3.send_tick(message);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    // Send last tick event.
    i3.send_tick("That's all folks!");
}

int main()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Subscribe to i3 and store function which will be executed when tick event happens.
    i3.on_tick_event(tick_callback);

    // Start a thread which will send tick events to i3.
    std::future thread = std::async(std::launch::async, tick_sender_thread);

    // Wait and handle subscribed events(only tick in this case) while thread is alive.
    while(thread.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
    {
        i3.handle_next_event();
    }

    return 0;
}
