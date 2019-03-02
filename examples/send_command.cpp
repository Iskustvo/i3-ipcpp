// i3-ipc++ headers.
#include "i3_ipc.hpp"
#include "i3_ipc_exception.hpp"
#include "i3_ipc_invalid_argument.hpp"

// C++ headers.
#include <iostream>
#include <exception>

int main()
{
    try
    {
        // Create IPC object and connect it to running i3 process.
        i3_ipc i3;

        // Tell i3 to execute command(s).
        i3.execute_commands("move left; move down");
    }
    catch(const i3_ipc_invalid_argument& exception)
    {
        // i3 declined the execution of the command.
        std::cout << "i3 declined to execute your command(s)!\n"
                  << "Here is the reported error:\n\n"
                  << exception.what() << std::endl;
        return 1;
    }
    catch(const i3_ipc_exception& exception)
    {
        // Creation of object failed because it couldn't find socket of the running i3 process.
        std::cout << exception.what() << "\n"
                  << "Is your i3 instance even running?" << std::endl;
        return 1;
    }
    catch(const std::exception& exception)
    {
        // Some nasty error happened...
        std::cout << "May the force be with you!\n\n"
                  << exception.what() << std::endl;
        return 1;
    }

    std::cout << "Your command(s) finished successfully!" << std::endl;
    return 0;
}
