cmake_minimum_required(VERSION 3.16.2)

project(i3-ipc++ VERSION 0.9.0 LANGUAGES CXX)

include(CMakePackageConfigHelpers)

find_package(RapidJSON REQUIRED)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                               i3-ipc++                               #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
option(build_dynamic "Build using shared libraries" TRUE)
option(build_static "Build using static libraries" TRUE)
option(build_examples "Build example programs which use this library." TRUE)

set(headers
    include/i3_ipc.hpp
    include/i3_containers.hpp
    include/i3_ipc_exception.hpp
    include/i3_ipc_bad_message.hpp
    include/i3_ipc_unsupported.hpp
    include/i3_ipc_invalid_argument.hpp
)
add_library(i3-ipc++-obj OBJECT)
target_sources(i3-ipc++-obj
    PRIVATE
        src/i3_ipc/i3_ipc-general.cpp
        src/i3_ipc/i3_ipc-events.cpp
        src/i3_ipc/i3_ipc-requests.cpp
        src/exceptions/i3_ipc_invalid_argument.cpp
        src/exceptions/i3_ipc_exception.cpp
        src/exceptions/i3_ipc_bad_message.cpp
        src/exceptions/i3_ipc_unsupported.cpp
        src/i3_json_parser/i3_json_parser-events.cpp
        src/i3_json_parser/i3_json_parser-requests.cpp
        src/i3_json_parser/i3_json_parser-utility.cpp
        src/i3_message.cpp
)
target_compile_features(i3-ipc++-obj PUBLIC cxx_std_17)
target_compile_options(i3-ipc++-obj
    PRIVATE
        -O2 -Wall -Wextra -Wshadow -Wold-style-cast -Wcast-align -Wunused -Wpedantic -Wconversion
        -Wsign-conversion -Wdouble-promotion -Wformat=2 -Werror -Wfatal-errors
)
target_link_libraries(i3-ipc++-obj PRIVATE rapidjson)
target_include_directories(i3-ipc++-obj
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/>
        $<INSTALL_INTERFACE:include/>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/i3_json_parser/>
        $<INSTALL_INTERFACE:src/>
        $<INSTALL_INTERFACE:src/i3_json_parser/>
        ${RAPIDJSON_INCLUDE_DIRS}
)
set_target_properties(i3-ipc++-obj PROPERTIES POSITION_INDEPENDENT_CODE 1)


if (build_dynamic)
    add_library(i3-ipc++-shared SHARED $<TARGET_OBJECTS:i3-ipc++-obj>)
    set_target_properties(i3-ipc++-shared
        PROPERTIES
            POSITION_INDEPENDENT_CODE 1
            OUTPUT_NAME i3-ipc++
    )
    target_include_directories(i3-ipc++-shared
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/>
            $<INSTALL_INTERFACE:include/>
    )
    list(APPEND build_targets_list i3-ipc++-shared)
    if (NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
        add_library(i3-ipc++::i3-ipc++-shared ALIAS i3-ipc++-shared)
    endif()
endif()

if (build_static)
    add_library(i3-ipc++-static STATIC $<TARGET_OBJECTS:i3-ipc++-obj> ${headers})
    set_target_properties(i3-ipc++-static PROPERTIES OUTPUT_NAME i3-ipc++)
    target_include_directories(i3-ipc++-static
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/>
            $<INSTALL_INTERFACE:include/>
    )
    list(APPEND build_targets_list i3-ipc++-static)
    if (NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
        add_library(i3-ipc++::i3-ipc++-static ALIAS i3-ipc++-static)
    endif()
endif()

add_library(i3-ipc++ INTERFACE)
target_link_libraries(i3-ipc++ INTERFACE ${build_targets_list})
if (NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_library(i3-ipc++::i3-ipc++ ALIAS i3-ipc++)
endif()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                            build examples                            #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
if (build_examples)
    add_subdirectory(examples)
endif()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                              packaging                               #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
include(GNUInstallDirs)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    PATH_VARS CMAKE_INSTALL_LIBDIR)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    COMPATIBILITY SameMajorVersion
)

install(TARGETS i3-ipc++ ${build_targets_list} EXPORT ${PROJECT_NAME}-targets)

install(EXPORT ${PROJECT_NAME}-targets
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    NAMESPACE i3-ipc++::
    FILE "${PROJECT_NAME}-targets.cmake"
)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/i3-ipc++/
)

install(FILES
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
