// i3-ipc++ headers.
#include "i3_ipc.hpp"

// C++ headers.
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <exception>

// Used to avoid having to put all i3_ipc related code in try/catch block.
std::unique_ptr<i3_ipc> safely_create_i3_ipc()
{
    try
    {
        return std::make_unique<i3_ipc>();
    }
    catch(const std::exception& exception) // You can catch and handle other exceptions as well.
    {
        return nullptr;
    }
}

int main()
{
    // Create IPC object and connect it to running i3 process.
    std::unique_ptr<i3_ipc> i3 = safely_create_i3_ipc();
    if (!i3)
    {
        std::cout << "Failed with creation of i3_ipc object!" << std::endl;
        return 1;
    }

    // Get list of marks used by i3.
    const std::vector<std::string> marks = i3->get_marks();

    // Write marks.
    std::cout << "Marks:" << (marks.empty() ? " None" : "");
    for (const auto& mark : marks)
    {
        std::cout << ' ' << mark;
    }
    std::cout << std::endl;

    return 0;
}
