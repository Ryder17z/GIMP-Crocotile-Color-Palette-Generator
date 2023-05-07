#include <iostream>
#include <stdio.h>

/**
 * @brief Pause the program and wait for any key to be pressed.
 *
 * On Windows, this function will display the "Press any key to continue..."
 * message and wait for a key to be pressed. On Linux, it will display the
 * same message and wait for a key to be pressed, similar to how Windows works.
 */
void waitForAnyKey() {
#ifdef _WIN32
    system("pause");
#elif __linux__
    std::cout << "Press any key to continue...";
    system("read -s -N 1"); // Continues when pressed a key like windows
#endif
}

/**
 * @brief Display an error message and exit the application.
 *
 * This function will print an error message to stdout indicating that an
 * output file could not be written to, and that the program will now exit.
 * It will also wait for any key to be pressed before exiting. The filename
 * causing the error should be passed as a parameter.
 *
 * @param fname The name of the output file causing the error.
 */
void APP_EXIT_IO_FAIL(std::string& fname)
{
    printf("Error Writing to output.file: %s\nPlease verify file/folder R/W permissions\nThe application will now exit.\n\n", fname.c_str());
    waitForAnyKey();
    exit(0); // RW failure
}
