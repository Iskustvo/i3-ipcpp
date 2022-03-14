set(EXAMPLES
    get_bar_config get_config get_outputs get_version multiple_events
    send_command window_event get_binding_modes get_marks get_tree
    get_workspaces restart_event tick_event)
foreach(EXAMPLE IN LISTS EXAMPLES)
    add_executable("${EXAMPLE}")
    target_sources("${EXAMPLE}" PRIVATE "${EXAMPLE}.cpp")
    target_compile_features("${EXAMPLE}" PUBLIC cxx_std_17)
    target_compile_options("${EXAMPLE}" PRIVATE -Wall -Wextra -Wpedantic)
    target_link_options("${EXAMPLE}" PRIVATE -pthread)
    target_link_libraries("${EXAMPLE}" PRIVATE i3-ipc++)
    set_target_properties("${EXAMPLE}"
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/")
endforeach()
