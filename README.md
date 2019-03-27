# i3-ipc++ #

C++ library that implements the [i3 IPC protocol](https://i3wm.org/docs/ipc.html).

## Index ##

* [Introduction](#introduction)
* [Goals](#goals)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
* [Usage](#usage)
  * [Write example code](#write-example-code)
    * [Maximize the currently focused window](#example-1)
    * [Get title of the focused window](#example-2)
    * [When new window is created, write its title](#example-3)
    * [More examples](#more-examples)
  * [Compilation](#compilation)
  * [Linking](#linking)
    * [Static linking](#static-linking)
    * [Dynamic linking](#dynamic-linking)
  * [Running](#running)
* [Public API](#public-api)
  * [Headers](#headers)
  * [Classes](#classes)
  * [Namespaces](#namespaces)
* [Internals](#internals)
  * [Overview](#overview)
  * [Code structure](#code-structure)
* [Compatibility](#compatibility)
* [Known issues](#known-issues)
* [TODO](#todo)
* [License](#license)

## Introduction ##

[i3 window manager](https://i3wm.org) defines the [IPC](https://en.wikipedia.org/wiki/Inter-process_communication)
protocol which can be used by other processes to directly talk to **i3**.  
This is very useful because it allows external projects to interact closely with **i3** quite easily.  
Currently, **i3** allows other processes to:
  1) Send predefined commands which **i3** can execute. (Example: [Maximize the currently focused window](#example-1).)
  2) Request some data that **i3** has. (Example: [Get title of the focused window](#example-2).)
  3) Get notifications about events that are happening in **i3**. (Example: [When new window is created, write its title](#example-3).)

**i3-ipc++** allows C++ projects to easily use what **i3** offers without the need to go into details of the implementation.

## Goals ##

Here is a list of goals in decreasing order of importance:

  1) **Completeness** - Support every feature that **i3** offers.
  2) **Documentation** - Thoroughly document everything implemented.
  3) **Stability** - Try to handle/propagate most errors to avoid the crash.
  4) **Convenience** - Strive to give the simplest API to the user.
  5) **Code readibility** - Avoid using efficient *hacks* at the cost of code readability.
  6) **Performance** - Try to be as performant as possible without hurting other goals.

## Prerequisites ##

* [i3](https://github.com/i3/i3)
* [RapidJSON](https://github.com/Tencent/rapidjson/)
* Modern C++17 compiler.

## Installation ##

```sh
./configure --help    # See how you can configure build system.
./configure [options] # Configure build for your needs.
make                  # Build library in configured way.
# make install        # Optionally install the library in configured way.
```

## Usage ##

### Write example code ###

----------

##### Example 1 #####

* Maximize the currently focused window.


*example.cpp*

```cpp
// Include the i3_ipc class which does all IPC communication with i3 for you.
#include "i3_ipc.hpp"

int main()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Tell i3 to maximize currently focused window.
    i3.execute_commands("fullscreen");

    return 0;
}
```
----------

##### Example 2 #####

* Get title of the focused window.

*example.cpp*

```cpp
// Include the i3_ipc class which does all IPC communication with i3 for you.
#include "i3_ipc.hpp"

// Include containers to store data from i3.
#include "i3_containers.hpp"

#include <cstdint>
#include <iostream>
#include <optional>
#include <algorithm>

// Find the focused node in i3's tree of containers.
std::optional<i3_containers::node> find_focused_node(const i3_containers::node& a_root)
{
    // If the current node is focused, we finished the search.
    if (a_root.is_focused)
    {
        return a_root;
    }

    // i3 provides a list of child nodes that we need to traverse in order to find the focused window.
    const std::vector<std::uint64_t>& traverse_IDs = a_root.focus;
    
    // Search all tilling child nodes.
    for (const auto& node : a_root.nodes)
    {
        // If current child node should be traversed, traverse it recursively.
        if (std::find(traverse_IDs.cbegin(), traverse_IDs.cend(), node.id) != traverse_IDs.cend())
        {
            const std::optional<i3_containers::node>& found_node = find_focused_node(node);
            if (found_node)
            {
                return found_node;
            }
        }
    }

    // Search all floating child nodes.
    for (const auto& node : a_root.floating_nodes)
    {
        // If current floating child node should be traversed, traverse it recursively.
        if (std::find(traverse_IDs.cbegin(), traverse_IDs.cend(), node.id) != traverse_IDs.cend())
        {
            const std::optional<i3_containers::node>& found_node = find_focused_node(node);
            if (found_node)
            {
                return found_node;
            }
        }
    }

    // We couldn't find focused window.
    return std::nullopt;
}

int main()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Get internal node tree from i3.
    const i3_containers::node& tree = i3.get_tree();

    // Try to find the focused node in i3's tree of containers.
    const std::optional<i3_containers::node>& focused_node = find_focused_node(tree);

    // Check if focused node is found.
    if(!focused_node)
    {
        std::cout << "Couldn't find focused node!" << std::endl;
        return 0;
    }

    std::cout << "Title of the focused window: " << focused_node->name.value_or("Unknown to i3") << std::endl;

    return 0;
}
```
----------

##### Example 3 #####

* When new window is created, write its title.

*example.cpp*

```cpp
// Include the i3_ipc class which does all IPC communication with i3 for you.
#include "i3_ipc.hpp"

// Include containers to store data from i3.
#include "i3_containers.hpp"

#include <string>
#include <optional>
#include <iostream>

// Callback function which will be called on i3's window event.
void window_callback(const i3_containers::window_event& a_window_event)
{
    // Only when new window is created, write it's title.
    if(a_window_event.change == i3_containers::window_change::create)
    {
        // i3 gave us container which will contain this new window.
        // Its name should be same as the title of the window.
        const std::optional<std::string>& title = a_window_event.container.name;

        // Write the title of the newly created window.
        std::cout << "New window's title is: " << title.value_or("Unknown to i3.") << std::endl;
    }
}

int main()
{
    // Create IPC object and connect it to running i3 process.
    i3_ipc i3;

    // Subscribe to i3's window event and store callback function to react on it.
    i3.on_window_event(window_callback);

    // Wait and handle all subscribed events.
    // In our case, we only subscribed to window event.
    while(true)
    {
        i3.handle_next_event();
    }

    return 0;
}
```

#### More examples ####

This project contains several examples in the [examples](https://github.com/Iskustvo/i3-ipcpp/blob/master/examples) directory.  
Most examples are simple and just show you how to use the library to get something done.  
There are few of them that are focusing on proper exception handling with this library, be sure not to miss them.

### Compilation ###

```sh
g++ -c -std=c++17 -I[path_to_include_dir] example.cpp
```

If you [installed](#installation) headers from **i3-ipc++** in place that compiler will search, you can omit `-I[path_to_include_dir]`.  
Otherwise, `[path_to_include_dir]` should point to where they are installed (or *include* directory in this project).

### Linking ###

Depending on your [installation](#installation), you should have either static, dynamic or both versions of the library built.

#### Static linking ####

For static linkage, you should use:
```sh
g++ -static -L[path_to_static_lib] example.o -o example -li3-ipc++
```

If you [installed](#installation) **libi3-ipc++.a** in place that compiler will search, you can omit `-L[path_to_static_lib]`.  
Otherwise, `[path_to_static_lib]` should point to where it is installed (or *build/lib/static* directory in this project).

#### Dynamic linking ####

For dynamic linkage, you should use:
```sh
g++ -L[path_to_dynamic_lib] -Wl,-rpath,[path_to_dynamic_lib] example.o -o example -li3-ipc++
```

If you [installed](#installation) **libi3-ipc++.so** in place that compiler will search, you can omit `-L[path_to_dynamic_lib]`  
and `-Wl,-rpath,[path_to_dynamic_lib]`.  
Otherwise, `[path_to_dynamic_lib]` should point to where it is installed (or *build/lib/dynamic* directory in this project).  
**Note** that when you run dynamically linked *example*, **libi3-ipc++.so** must exist in `[path_to_dynamic_lib]`.

### Running ###

If you did everything right, you should be able to run `example`.
```sh
./example
```

## Public API ##

### Headers ###

* [i3_ipc.hpp](https://github.com/Iskustvo/i3-ipcpp/blob/master/include/i3_ipc.hpp): Defines [i3_ipc](#i3_ipc) class.
* [i3_containers.hpp](https://github.com/Iskustvo/i3-ipcpp/blob/master/include/i3_containers.hpp): Defines [i3_containers](#i3_containers) namespace.
* [i3_ipc_exception.hpp](https://github.com/Iskustvo/i3-ipcpp/blob/master/include/i3_ipc_exception.hpp): Defines [i3_ipc_exception](#i3_ipc_exception) class.
* [i3_ipc_unsupported.hpp](https://github.com/Iskustvo/i3-ipcpp/blob/master/include/i3_ipc_unsupported.hpp): Defines [i3_ipc_unsupported](#i3_ipc_unsupported) class.
* [i3_ipc_bad_message.hpp](https://github.com/Iskustvo/i3-ipcpp/blob/master/include/i3_ipc_bad_message.hpp): Defines [i3_ipc_bad_message](#i3_ipc_bad_message) class.
* [i3_ipc_invalid_argument.hpp](https://github.com/Iskustvo/i3-ipcpp/blob/master/include/i3_ipc_invalid_argument.hpp): Defines [i3_ipc_invalid_argument](#i3_ipc_invalid_argument) class.

### Classes ###

* #### i3_ipc ####
  This class is used for over-socket communication with running **i3** window manager.

* #### i3_ipc_exception ####
  Exception class, derived from [std::exception](https://en.cppreference.com/w/cpp/error/exception), used only for internal problems of **i3-ipc++** library.

* #### i3_ipc_unsupported ####
  Exception class, derived from [i3_ipc_exception](#i3_ipc_exception), used when some property has the value unknown to **i3-ipc++**.

* #### i3_ipc_bad_message ####
  Exception class derived from [i3_ipc_exception](#i3_ipc_exception), used when **i3-ipc++** receives invalid message from **i3**.

* #### i3_ipc_invalid_argument ####
  Exception class, derived from [i3_ipc_exception](#i3_ipc_exception), used when user provides bad arguments to the **i3-ipc++**.

### Namespaces ###

* #### i3_containers ####
  Defines containers for all info that **i3** can provide.

## Internals ##

### Overview ###

This picture represents the simplified overview of how library is working internally.

![i3-ipc++ oveview](docs/overview/overview.png)

----------

* **i3_ipc** contains two sockets connected to **i3**.
  * One for synchronous requests - *m_request_socket*
  * One for asynchronous events - *m_event_socket*

----------

* For synchronous requests(left half of the [picture](#overview)), the case is pretty simple:
  * User sends a request to **i3_ipc** with one of the member functions implemented in [i3_ipc-requests.cpp](https://github.com/Iskustvo/i3-ipcpp/blob/master/src/i3_ipc/i3_ipc-requests.cpp).
  * **i3_ipc** writes the requst in correct format to **i3**'s socket and waits to read **i3**'s response.
  * Once **i3_ipc** reads the response, it uses **i3_json_parser** class to extract all data from JSON format.
  * The extracted structure containing the response is returned to the user.

----------

* For asynchronous events(right half of the [picture](#overview)), the case is a bit more complicated:
  * User sends a callback function to **i3_ipc** in order for it to be executed on specific **i3** event.  
    User does so by using one of the public member functions implemented in [i3_ipc-events.cpp](https://github.com/Iskustvo/i3-ipcpp/blob/master/src/i3_ipc/i3_ipc-events.cpp).

  * **i3_ipc** sends subscription request to **i3** and parses events from *m_event_socket* until it reaches response.  
    All events parsed(using the **i3_json_parser**) this way are pushed to *m_event_queue* for later handling.  
    Afterwards, callback sent by the user is also pushed to *m_event_queue* to be used at appropriate time.

  * From this point, **i3** will write info about subscribed event to *m_event_socket*, whenever event happens.

  * User will handle events with **i3_ipc::handle_next_event()** member function.  
    Events will be handled from *m_event_queue* first, and when empty, from *m_event_socket* directly.  
    If you were careful, you should've noticed that *m_event_queue* has 2 different types of events:
    * Callback functions that should be internally stored for execution on events.
    * Parsed data from real **i3** events that will be passed to stored callbacks.  

    **i3_ipc::handle_next_event()** will internally store callbacks and end once the **real** event is handled.

### Code structure ###

    i3-ipc++
    ├── examples                            // Source code for examples.
    ├── include                             // Include headers representing the public API.
    └── src
        ├── exceptions                      // Source code of exception classes.
        ├── i3_ipc
        │   ├── i3_ipc-general.cpp          // Implements general member functions of i3_ipc class.
        │   ├── i3_ipc-requests.cpp         // Implements member functions of i3_ipc class used for requests.
        │   └── i3_ipc-events.cpp           // Implements member functions of i3_ipc class used for events.
        ├── i3_json_parser
        │   ├── i3_json_parser.hpp          // Defines the abstract i3_json_parser class.
        │   ├── i3_json_parser-utility.inl  // Implements utility templates used in i3_json_parser class.
        │   ├── i3_json_parser-utility.cpp  // Implements utility member functions used in i3_json_parser class.
        │   ├── i3_json_parser-requests.cpp // Implements member functions of i3_json_parser class used for requests.
        │   └── i3_json_parser-events.cpp   // Implements member functions of i3_json_parser class used for events.
        ├── i3_message.hpp                  // Defines i3_message namespace used for socket communication with i3.
        └── i3_message.cpp                  // Implements functions from i3_message namespace.

## Compatibility ##

At the moment, **i3-ipc++** supports **i3** version `4.16`.

If you have older version of **i3**, there is a chance that **i3-ipc++** won't compile.  
This will most likely happen because **i3-ipc++** is missing some defines from **i3**'s [ipc.h](https://github.com/i3/i3/blob/next/include/i3/ipc.h).  
There are few ways to solve this:
* Get later version of **i3** which has those defines.
* Replace missing defines in [i3_message.hpp](https://github.com/Iskustvo/i3-ipcpp/blob/master/src/i3_message.hpp) with `-1` and of course, don't use requests/events for them.


## Known issues ##

* Sending *restart* or *exit* through **i3_ipc::execute_commands** will always throw([#3624](https://github.com/i3/i3/pull/3624), [#3565](https://github.com/i3/i3/issues/3565)).

## TODO ##

* Make [i3_ipc](#i3_ipc) class thread-safe.

## License ##

![GPL v3 logo](https://www.gnu.org/graphics/gplv3-with-text-136x68.png)

This library is available under the [GNU GPL](https://www.gnu.org/licenses/gpl.html) version 3 or later. See LICENSE.GPL for details.

Copyright (C) 2019, Ivan Balević. All rights reserved.

---

This software uses the following external libraries:

* [RapidJSON](https://github.com/Tencent/rapidjson/)
  * Copyright (C) 2015 THL A29 Limited, a Tencent company, and Milo Yip. All rights reserved.
  * Licensed under the MIT license. See LICENSE.MIT for details.
