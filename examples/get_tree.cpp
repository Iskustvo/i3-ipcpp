// i3-ipc++ headers.
#include "i3_ipc.hpp"
#include "i3_containers.hpp"

// C++ headers.
#include <string>
#include <iostream>

void print_i3_tree(const i3_containers::node& a_node, const std::string& a_indent = "")
{
    // Print ID and name.
    std::cout << a_indent << "ID: " << a_node.id << "\n"
              << a_indent << "Name: \"" << a_node.name.value_or("None") << "\"\n";

    // Print node type.
    std::string type;
    switch (a_node.type)
    {
        case i3_containers::node_type::root:
            type = "Root";
            break;
        case i3_containers::node_type::output:
            type = "Output";
            break;
        case i3_containers::node_type::con:
            type = "Container";
            break;
        case i3_containers::node_type::floating_con:
            type = "Floating container";
            break;
        case i3_containers::node_type::workspace:
            type = "Workspace";
            break;
        case i3_containers::node_type::dockarea:
            type = "Dockarea";
            break;
    }
    std::cout << a_indent << "Type: \"" << type << "\"\n";

    // Print border style.
    std::string border;
    switch (a_node.border)
    {
        case i3_containers::border_style::normal:
            border = "Normal";
            break;
        case i3_containers::border_style::none:
            border = "None";
            break;
        case i3_containers::border_style::pixel:
            border = "Pixel";
            break;
    }
    std::cout << a_indent << "Border style: \"" << border << "\"\n";

    // Print border width.
    if (a_node.current_border_width)
    {
        std::cout << a_indent << "Border width: " << static_cast<unsigned>(*a_node.current_border_width) << "\n";
    }
    else
    {
        std::cout << a_indent << "Border width: None\n";
    }

    // Print layout.
    std::string layout;
    switch (a_node.layout)
    {
        case i3_containers::node_layout::splith:
            layout = "Horizontal split";
            break;
        case i3_containers::node_layout::splitv:
            layout = "Vertical split";
            break;
        case i3_containers::node_layout::stacked:
            layout = "Stacked";
            break;
        case i3_containers::node_layout::tabbed:
            layout = "Tabbed";
            break;
        case i3_containers::node_layout::dockarea:
            layout = "Dockarea";
            break;
        case i3_containers::node_layout::output:
            layout = "Output";
            break;
    }
    std::cout << a_indent << "Layout: " << layout << "\n";

    // Print percentage taken from parent container.
    if (a_node.percent)
    {
        std::cout << a_indent << "Percentage of parent area: " << *a_node.percent << "\n";
    }
    else
    {
        std::cout << a_indent << "Percentage of parent area: None\n";
    }

    // Print rectangles.
    std::cout << a_indent << "Absolute display coordinates: {" << a_node.rect.x << ", "
                                                               << a_node.rect.y << ", "
                                                               << a_node.rect.width << ", "
                                                               << a_node.rect.height << "}\n"
              << a_indent << "Actual client window coordinates: {" << a_node.window_rect.x << ", "
                                                                   << a_node.window_rect.y << ", "
                                                                   << a_node.window_rect.width << ", "
                                                                   << a_node.window_rect.height << "}\n"
              << a_indent << "Window decorations coordinates: {" << a_node.deco_rect.x << ", "
                                                                 << a_node.deco_rect.y << ", "
                                                                 << a_node.deco_rect.width << ", "
                                                                 << a_node.deco_rect.height << "}\n"
              << a_indent << "Original i3 geometry: {" << a_node.geometry.x << ", "
                                                       << a_node.geometry.y << ", "
                                                       << a_node.geometry.width << ", "
                                                       << a_node.geometry.height << "}\n";

    // Print X11 window ID that is displayed in container.
    std::cout << a_indent << "X11 window ID: " << (a_node.window ? std::to_string(*a_node.window) : "None") << "\n";

    // Print X11 properties of the container.
    if (a_node.window_properties)
    {

        const i3_containers::x11_window& window = *a_node.window_properties;
        const std::string& window_class = window.window_class;
        const std::string& window_instance = window.window_instance;
        const std::string& window_role = window.window_role ? *window.window_role : "None";
        const std::string& transient_for = window.transient_for ? std::to_string(*window.transient_for) : "None";

        std::cout << a_indent << "X11 window properties: " << "\n"
                  << a_indent + "\t" << "Class = \"" << window_class << "\"\n"
                  << a_indent + "\t" << "Instance = \"" << window_instance << "\"\n"
                  << a_indent + "\t" << "Role = \"" << window_role << "\"\n"
                  << a_indent + "\t" << "Transient for = \"" << transient_for << "\"\n";
    }
    else
    {
        std::cout << a_indent << "Window properties: None\n";
    }

    // Print info about node being urgent or focused.
    std::cout << std::boolalpha
              << a_indent << "Is urgent: " << a_node.is_urgent << "\n"
              << a_indent << "Is focused: " << a_node.is_focused << "\n";

    // Print ID's of focused children to be able to reach the most-inner child which is actually focused.
    std::cout << a_indent << "Child IDs in focus order:" << (a_node.focus.empty() ? " None" : "");
    for (const auto& node_id : a_node.focus)
    {
        std::cout << " " << node_id;
    }
    std::cout << "\n";

    // Recursively print info about all tilling nodes that are children of this node.
    std::cout << a_indent << "Tilling children nodes:" << (a_node.nodes.empty() ? " None" : "") << std::endl;
    for (const auto& node : a_node.nodes)
    {
        print_i3_tree(node, a_indent + "\t");
    }

    // Recursively print info about all floating nodes that are children of this node.
    std::cout << a_indent << "Floating children nodes:" << (a_node.floating_nodes.empty() ? " None" : "") << std::endl;
    for (const auto& floating_node : a_node.floating_nodes)
    {
        print_i3_tree(floating_node, a_indent + "\t");
    }
}

int main()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Get info about internal node tree in i3 and print it.
    print_i3_tree(i3.get_tree());

    return 0;
}
