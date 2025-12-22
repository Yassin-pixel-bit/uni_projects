// Simple unit test for save_file
#include "../ship_management/save_load.h"
#include <filesystem>
#include <cassert>
#include <iostream>

int main()
{
    using namespace std;

    // Ensure a clean state
    if (filesystem::exists(SAVE_DIR))
        filesystem::remove_all(SAVE_DIR);

    // Prepare a minimal ships array
    Ship ships[1];
    ships[0].number = 1;
    ships[0].name = "TestShip";
    ships[0].capacity = 100;

    // Call the function under test
    save_file(ships,1);

    // Verify the save directory was created
    assert(filesystem::exists(SAVE_DIR) && "SAVE_DIR should exist after save_file call");

    std::cout << "save_file test passed\n";
    return 0;
}
