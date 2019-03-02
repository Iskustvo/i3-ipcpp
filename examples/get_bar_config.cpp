// i3-ipc++ headers.
#include "i3_ipc.hpp"
#include "i3_containers.hpp"

// C++ headers.
#include <vector>
#include <string>
#include <iostream>

std::string bar_mode_to_string(const i3_containers::bar_mode& a_mode)
{
    switch (a_mode)
    {
        case i3_containers::bar_mode::dock:
            return "Dock";
        case i3_containers::bar_mode::hide:
            return "Hide";
    }

    return "Unknown";
}

std::string bar_position_to_string(const i3_containers::bar_position& a_position)
{
    switch (a_position)
    {
        case i3_containers::bar_position::bottom:
            return "Bottom";
        case i3_containers::bar_position::top:
            return "Top";
    }

    return "Unknown";
}

void print_bar_config(const i3_containers::bar_config& a_config)
{
    std::cout << std::boolalpha
              << "ID: \"" << a_config.id << "\"\n"
              << "Mode: " << bar_mode_to_string(a_config.mode) << "\n"
              << "Position: " << bar_position_to_string(a_config.position) << "\n"
              << "Status command: \"" << a_config.status_command << "\"\n"
              << "Font: \"" << a_config.font << "\"\n"
              << "Workspace buttons: " << a_config.workspace_buttons << "\n"
              << "Binding mode indicator: " << a_config.binding_mode_indicator << "\n"
              << "Verbose: " << a_config.verbose << "\n"
              << "Colors:\n"
              << "\tBackground: " << a_config.colors.background.value_or("null") << "\n"
              << "\tFocused background: " << a_config.colors.focused_background.value_or("null") << "\n"
              << "\tStatusline: " << a_config.colors.statusline.value_or("null") << "\n"
              << "\tFocused statusline: " << a_config.colors.focused_statusline.value_or("null") << "\n"
              << "\tSeparator: " << a_config.colors.separator.value_or("null") << "\n"
              << "\tFocused separator: " << a_config.colors.focused_separator.value_or("null") << "\n"
              << "\tFocused workspace text: " << a_config.colors.focused_workspace_text.value_or("null") << "\n"
              << "\tFocused workspace bg: " << a_config.colors.focused_workspace_bg.value_or("null") << "\n"
              << "\tFocused workspace border: " << a_config.colors.focused_workspace_border.value_or("null") << "\n"
              << "\tActive workspace text: " << a_config.colors.active_workspace_text.value_or("null") << "\n"
              << "\tActive workspace bg: " << a_config.colors.active_workspace_bg.value_or("null") << "\n"
              << "\tActive workspace border: " << a_config.colors.active_workspace_border.value_or("null") << "\n"
              << "\tInactive workspace text: " << a_config.colors.inactive_workspace_text.value_or("null") << "\n"
              << "\tInactive workspace bg: " << a_config.colors.inactive_workspace_bg.value_or("null") << "\n"
              << "\tInactive workspace border: " << a_config.colors.inactive_workspace_border.value_or("null") << "\n"
              << "\tUrgent workspace text: " << a_config.colors.urgent_workspace_text.value_or("null") << "\n"
              << "\tUrgent workspace bg: " << a_config.colors.urgent_workspace_bg.value_or("null") << "\n"
              << "\tUrgent workspace border: " << a_config.colors.urgent_workspace_border.value_or("null") << "\n"
              << "\tBinding mode text: " << a_config.colors.binding_mode_text.value_or("null") << "\n"
              << "\tBinding mode bg: " << a_config.colors.binding_mode_bg.value_or("null") << "\n"
              << "\tBinding mode border: " << a_config.colors.binding_mode_border.value_or("null") << std::endl;
}

int main()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Request bar IDs from i3.
    const std::vector<std::string> bar_IDs = i3.get_bar_IDs();

    // Ask i3 for info on each given bar ID and print it.
    for (const auto& bar_ID : bar_IDs)
    {
        print_bar_config(i3.get_bar_config(bar_ID));
    }

    return 0;
}
