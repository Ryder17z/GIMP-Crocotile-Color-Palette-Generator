#include <iostream>
#include <stdio.h>

// Pause methods
#ifdef _WIN32
// For windows
void waitForAnyKey() {
	system("pause");
}
#elif __linux__
// For linux
void waitForAnyKey() {
	std::cout << "Press any key to continue...";
	system("read -s -N 1"); // Continues when pressed a key like windows
}
#endif

void APP_EXIT_IO_FAIL()
{
	printf("Error Writing to output.file\nPlease verify file/folder R/W permissions\nThe application will now exit.\n\n");
	waitForAnyKey();
	exit(0); // RW failure
}