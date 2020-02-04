/*
    Copyright (C) 2019, Ivan Balević

    This file is part of "i3-ipc++" library.

    "i3-ipc++" is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    "i3-ipc++" is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "i3-ipc++". If not, see <https://www.gnu.org/licenses/>.
*/

/**
 * \file    i3_containers.hpp
 *
 * \brief   Defines "i3_containers" namespace which provides containers needed to store any data received from i3.
 */

#ifndef I3_CONTAINERS_HPP
#define I3_CONTAINERS_HPP

// C++ headers.
#include <vector>
#include <string>
#include <variant>
#include <optional>

// C headers.
#include <cstdint>

/**
 * \brief   Defines containers for all info that i3 can provide.
 */
namespace i3_containers
{
    /**
     * \brief   Structutre used to represent windows.
     */
    struct rectangle
    {
        std::uint16_t x;      /**< X coordinate of starting point. */
        std::uint16_t y;      /**< Y coordinate of starting point. */
        std::uint16_t width;  /**< Width of the window.            */
        std::uint16_t height; /**< Height of the window.           */
    };

    /**
     * \brief   Structutre used to represent one workspace.
     */
    struct workspace
    {
        std::optional<std::uint8_t> num; /**< Logical number of the workspace.                   */
        std::string name;                /**< Name of the workspace.                             */
        bool is_visible;                 /**< Whether the workspace is currently visible or not. */
        bool is_focused;                 /**< Whether the workspace is currently focused or not. */
        bool is_urgent;                  /**< Whether the workspace is currently urgent or not.  */
        rectangle rect;                  /**< Window in which this workspace is drown.           */
        std::string output;              /**< Video output(Monitor) that this workspace is on.   */
    };

    /**
     * \brief   Structure used to represent one video output(Monitor).
     */
    struct output
    {
        std::string name;                             /**< Name of the output.                                 */
        bool is_active;                               /**< Whether this output is currently active or not.     */
        bool is_primary;                              /**< Whether this output is the primary output or not.   */
        std::optional<std::string> current_workspace; /**< Workspace that is currently visible on this output. */
        rectangle rect;                               /**< Window of this output.                              */
    };

    /**
     * \brief   Enumerator used to describe the type of i3 node/container.
     */
    enum class node_type : std::uint8_t
    {
        root,         /**< Root container.      */
        output,       /**< Output container.    */
        con,          /**< Tilling container.   */
        floating_con, /**< Floating container.  */
        workspace,    /**< Workspace container. */
        dockarea      /**< Dockarea container.  */
    };

    /**
     * \brief   Enumerator used to describe the type of border between windows.
     */
    enum class border_style : std::uint8_t
    {
        normal, /**< Normal border style. */
        none,   /**< No borders.          */
        pixel   /**< Pixel border style.  */
    };

    /**
     * \brief   Enumerator used to describe the layout of i3 node/container.
     */
    enum class node_layout : std::uint8_t
    {
        splith,   /**< Horizontal split container. */
        splitv,   /**< Vertical split container.   */
        stacked,  /**< Stacked container.          */
        tabbed,   /**< Tabbed container.           */
        dockarea, /**< Dockarea container.         */
        output    /**< Output container.           */
    };

    /**
     * \brief   Structure used to represent X11 window.
     */
    struct x11_window
    {
        std::optional<std::string> window_class;    /**< X11 window class (WM_CLASS class).                     */
        std::optional<std::string> window_instance; /**< X11 window class instance (WM_CLASS instance).         */
        std::optional<std::string> window_role;     /**< X11 window role (WM_WINDOW_ROLE).                      */
        std::optional<std::string> window_title;    /**< X11 window title in UTF-8 (_NET_WM_NAME).              */
        std::optional<std::uint64_t> transient_for; /**< X11 window ID of the parent window (WM_TRANSIENT_FOR). */
    };

    /**
     * \brief   Enumerator used to describe the fullscreen mode of a window.
     */
    enum class fullscreen_mode_type : std::uint8_t
    {
        no_fullscreen,    /**< No fullscreen.                                           */
        local_fullscreen, /**< Local fullscreen (window is fullscreened on its output). */
        global_fullscreen /**< Global fullscreen (window is globally fullscreened).     */
    };

    /**
     * \brief   Structure used to represent one i3 node/container.
     */
    struct node
    {
        std::uint64_t id;                                 /**< ID of the container.                                   */
        std::optional<std::string> name;                  /**< Name of the container.                                 */
        node_type type;                                   /**< Type of the container.                                 */
        border_style border;                              /**< Border style of the container.                         */
        std::optional<std::uint8_t> current_border_width; /**< Border width of the container.                         */
        node_layout layout;                               /**< Layout of the current container.                       */
        std::optional<float> percent;                     /**< Percentage of surface taken from parent container.     */
        rectangle rect;                                   /**< Absolute display coordinates for the container.        */
        rectangle window_rect;                            /**< Coordinates of container relative to parent container. */
        rectangle deco_rect;                              /**< Coordinates of window decorations inside container.    */
        rectangle geometry;                               /**< Geometry the window specified when i3 mapped it.       */
        std::optional<std::uint64_t> window;              /**< X11 window ID of the client inside the container.      */
        std::optional<x11_window> window_properties;      /**< X11 properties of the window.                          */
        bool is_urgent;                                   /**< Whether the container is (in)directly urgent or not.   */
        bool is_focused;                                  /**< Whether the container is currently focused or not.     */
        std::vector<std::uint64_t> focus;                 /**< List of child node IDs in focus order.                 */
        fullscreen_mode_type fullscreen_mode;             /**< The fullscreen mode of the container.                  */
        std::vector<node> nodes;                          /**< The tiling child containers of this node.              */
        std::vector<node> floating_nodes;                 /**< The floating child containers of this node.            */
    };

    /**
     * \brief   Enumerator used to describe the mode of i3 bar.
     */
    enum class bar_mode : std::uint8_t
    {
        dock, /**< Docked bar. */
        hide  /**< Hidden bar. */
    };

    /**
     * \brief   Enumerator used to describe the position of i3 bar.
     */
    enum class bar_position : std::uint8_t
    {
        bottom, /**< Bar is positioned at the bottom of the screen. */
        top     /**< Bar is positioned at the top of the screen.    */
    };

    /**
     * \brief   Structure used to represent color settings on i3 bar.
     */
    struct bar_colors
    {
        std::optional<std::string> background;                /**< Bar background color.                              */
        std::optional<std::string> focused_background;        /**< Bar background color on focused monitor.           */

        std::optional<std::string> statusline;                /**< Statusline text color.                             */
        std::optional<std::string> focused_statusline;        /**< Statusline text color on focused monitor.          */

        std::optional<std::string> separator;                 /**< Separator text color.                              */
        std::optional<std::string> focused_separator;         /**< Separator text color on focused monitor.           */

        std::optional<std::string> focused_workspace_text;    /**< Workspace text color when focused.                 */
        std::optional<std::string> focused_workspace_bg;      /**< Workspace background color when focused.           */
        std::optional<std::string> focused_workspace_border;  /**< Workspace border color when focused.               */

        std::optional<std::string> active_workspace_text;     /**< Workspace text color when active but unfocused.    */
        std::optional<std::string> active_workspace_bg;       /**< Workspace bg color when active but unfocused.      */
        std::optional<std::string> active_workspace_border;   /**< Workspace border color when active but unfocused.  */

        std::optional<std::string> inactive_workspace_text;   /**< Workspace text color when unfocused and inactive.  */
        std::optional<std::string> inactive_workspace_bg;     /**< Workspace bg color when unfocused and inactive.    */
        std::optional<std::string> inactive_workspace_border; /**< Workspace border color when unfocused and inactive.*/

        std::optional<std::string> urgent_workspace_text;     /**< Text color for workspaces with urgent window.      */
        std::optional<std::string> urgent_workspace_bg;       /**< Bg color for workspaces with urgent window.        */
        std::optional<std::string> urgent_workspace_border;   /**< Border color for workspaces with urgent window.    */

        std::optional<std::string> binding_mode_text;         /**< Text color for the binding mode indicator.         */
        std::optional<std::string> binding_mode_bg;           /**< Bg color for the binding mode indicator.           */
        std::optional<std::string> binding_mode_border;       /**< Border color for the binding mode indicator.       */
    };

    /**
     * \brief   Structure used to represent i3 bar.
     */
    struct bar_config
    {
        std::string id;              /**< Id for the i3 bar.                                */
        bar_mode mode;               /**< Mode of the i3 bar.                               */
        bar_position position;       /**< Position of the i3 bar.                           */
        std::string status_command;  /**< Command which will generate statusline.           */
        std::string font;            /**< Font used on the i3 bar.                          */
        bool workspace_buttons;      /**< Wheter worskapce buttons are displayed or not.    */
        bool binding_mode_indicator; /**< Wheter binding mode indicator is displaed or not. */
        bool verbose;                /**< Wheter the i3 bar has verbose output or not.      */
        bar_colors colors;           /**< Colors used on the i3 bar.                        */
    };

    /**
     * \brief   Structure used to represent version of i3.
     */
    struct version
    {
        std::uint8_t major;                  /**< Major version of i3.                 */
        std::uint8_t minor;                  /**< Minor version of i3.                 */
        std::uint8_t patch;                  /**< Patch version of i3.                 */
        std::string human_readable;          /**< Human readable version of i3.        */
        std::string loaded_config_file_name; /**< Name of the last loaded config file. */
    };

    /**
     * \brief   Enumerator used to describe the workspace change.
     */
    enum class workspace_change : std::uint8_t
    {
        focus,    /**< Workspace is being focused.     */
        init,     /**< Workspace is being initialized. */
        empty,    /**< Workspace became empty.         */
        urgent,   /**< Workspace got urgent flag.      */
        reload,   /**< Workspace was reloaded.         */
        rename,   /**< Workspace is renamed.           */
        restored, /**< Workspace is restored.          */
        move      /**< Workspace is moved.             */
    };

    /**
     * \brief   Structure used to represent workspace event.
     */
    struct workspace_event
    {
        workspace_change change;     /**< Describes the type of workspace change.              */
        std::optional<node> old;     /**< When appropriate, the tree of the old workspace.     */
        std::optional<node> current; /**< When appropriate, the tree of the current workspace. */
    };

    /**
     * \brief   Enumerator used to describe the output change.
     */
    enum class output_change : std::uint8_t
    {
        unspecified /** Unspecified output change. */
    };

    /**
     * \brief   Structure used to represent output event.
     */
    struct output_event
    {
        output_change change; /**< Describes the type of output change. */
    };

    /**
     * \brief   Structure used to represent mode change event.
     */
    struct mode_event
    {
        std::string change; /**< Describes the type of mode change.                        */
        bool pango_markup;  /**< Informs if pango markup is used for displaying this mode. */
    };

    /**
     * \brief   Enumerator used to describe the window change.
     */
    enum class window_change : std::uint8_t
    {
        create,          /**< New window is created.                        */
        close,           /**< Window is closed.                             */
        focus,           /**< Window is being focused.                      */
        title,           /**< Window title has changed.                     */
        fullscreen_mode, /**< Window toggeled fullscreen mode.              */
        move,            /**< Window was moved.                             */
        floating,        /**< Window transitioned to/from floating windows. */
        urgent,          /**< Window got urgent flag.                       */
        mark             /**< Window got/lost mark.                         */
    };

    /**
     * \brief   Structure used to represent window event.
     */
    struct window_event
    {
        window_change change; /**< Describes the type of window change.  */
        node container;       /**< Represents window's parent container. */
    };

    /**
     * \brief   Structure used to represent update event for bar config.
     */
    using bar_config_event = bar_config;

    /**
     * \brief   Enumerator used to describe the binding change.
     */
    enum class binding_change : std::uint8_t
    {
        run /**< Binding was triggered. */
    };

    /**
     * \brief   Enumerator used to describe the binding input type.
     */
    enum class binding_input_type : std::uint8_t
    {
        keyboard, /**< Keyboard binding type. */
        mouse     /**< Mouse binding type.    */
    };

    /**
     * \brief   Structure used to represent binding info.
     */
    struct binding_info
    {
        std::string command;                       /**< Command that is executed on the triggered binding.     */
        std::vector<std::string> event_state_mask; /**< Modifier keys used with triggered binding.             */
        std::optional<std::uint8_t> input_code;    /**< If appropriate, the key code of the triggered binding. */
        std::optional<char> symbol;                /**< If appropriate, the symbol of the triggered binding.   */
        binding_input_type input_type;             /**< Type of the input binding.                             */
    };

    /**
     * \brief   Structure used to represent binding event.
     */
    struct binding_event
    {
        binding_change change; /**< Describes the type of binding change. */
        binding_info binding;  /**< Describes the triggered binding.      */
    };

    /**
     * \brief   Enumerator used to describe the shutdown type.
     */
    enum class shutdown_type : std::uint8_t
    {
        restart, /**< i3 is about to restart. */
        exit     /**< i3 is about to exit.    */
    };

    /**
     * \brief   Structure used to represent shutdown event.
     */
    struct shutdown_event
    {
        shutdown_type change; /**< Describes the type of shutdown. */
    };

    /**
     * \brief   Structure used to represent tick event.
     */
    struct tick_event
    {
        bool is_first;                      /**< Tells if this is confirmation to the subscription. */
        std::optional<std::string> payload; /**< Payload sent by the broadcaster of this event.     */
    };

    /**
     * \brief   Type used to store info of any supported i3 event.
     */
    // NOTE: Keep the size and order of elements synced with "i3_ipc::event_type" and "i3_ipc::i3_callback".
    using event = std::variant<
                               i3_containers::workspace_event,
                               i3_containers::output_event,
                               i3_containers::mode_event,
                               i3_containers::window_event,
                               i3_containers::bar_config_event,
                               i3_containers::binding_event,
                               i3_containers::shutdown_event,
                               i3_containers::tick_event
                              >;

} // i3_containers namespace

#endif // I3_CONTAINERS_HPP
