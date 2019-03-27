// i3-ipc++ headers.
#include "i3_ipc.hpp"
#include "i3_ipc_bad_message.hpp"
#include "i3_ipc_unsupported.hpp"

// C++ headers.
#include <iostream>

// Callback function which will be called on i3's window events.
void window_callback(const i3_containers::window_event& event)
{
    switch(event.change)
    {
        case i3_containers::window_change::create:          std::cout << "Creating window!"        << std::endl; return;
        case i3_containers::window_change::close:           std::cout << "Closing window!"         << std::endl; return;
        case i3_containers::window_change::focus:           std::cout << "Focusing window!"        << std::endl; return;
        case i3_containers::window_change::title:           std::cout << "Changing window title!"  << std::endl; return;
        case i3_containers::window_change::fullscreen_mode: std::cout << "Toggle fullscreen mode!" << std::endl; return;
        case i3_containers::window_change::move:            std::cout << "Moving window!"          << std::endl; return;
        case i3_containers::window_change::floating:        std::cout << "Toggle floating window!" << std::endl; return;
        case i3_containers::window_change::urgent:          std::cout << "Toggle urgent flag!"     << std::endl; return;
        case i3_containers::window_change::mark:            std::cout << "Window got/lost mark!"   << std::endl; return;
    }
}

// Callback function which will be called when i3 changes binding modes.
void mode_callback(const i3_containers::mode_event& event)
{
    std::cout << "Current mode: " << event.change << std::endl;
}

int main()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Subscribe to i3 and store functions which will be executed when events happen.
    try
    {
        i3.on_window_event(window_callback);
        i3.on_mode_event(mode_callback);
    }
    catch(...)
    {
        std::cout << "Failed to subscribe on events!" << std::endl;
        throw;
    }

    std::cout << "Play with windows and binding modes to create events" << std::endl;

    // Handle window and mode events.
    while(true)
    {
        try
        {
            const i3_ipc::event_type handled_event_type = i3.handle_next_event();
            switch(handled_event_type)
            {
                case i3_ipc::event_type::mode:
                    std::cout << "Handled \"mode\" event!\n" << std::endl;
                    break;

                case i3_ipc::event_type::window:
                    std::cout << "Handled \"window\" event!\n" << std::endl;
                    break;

                default:
                    std::cout << "Handled INVALID event! Ignoring...\n" << std::endl;
            }
        }
        catch (const std::system_error& exception)
        {
            std::cout << "Caught system error: " << exception.what() << std::endl;
        }
        catch (const i3_ipc_bad_message& exception)
        {
            std::cout << "Caught bad message from i3: " << exception.what() << std::endl;
        }
        catch (const i3_ipc_unsupported& exception)
        {
            std::cout << "Couldn't parse i3's response: " << exception.what() << std::endl;
        }
    }
}
