// Simple unit test for save_file
#include "../ship_management/save_load.h"
#include <filesystem>
#include <cassert>
#include <iostream>

int main()
{
    using namespace std;

    // Prepare a minimal ships array
    Ship ships[2];
    Ship empty_ships[2];
    int count = 0;
    ships[0].number = 100;
    ships[0].name = "TestShip";
    ships[0].capacity = 25;
    ships[0].departure_day = 10;

    ships[1].number = 101;
    ships[1].name = "TestShip2";
    ships[1].capacity = 20;
    ships[1].departure_day = 13;
    ships[1].container_count = 2;

    Container container1;
    container1.weight = 5;
    Container container2;
    container1.weight = 6;

    ships[1].container[0] = container1;
    ships[1].container[1] = container2;

    // Call the function under test
    save_file(ships,2);

    // Verify the save directory was created
    assert(filesystem::exists(SAVE_DIR) && "SAVE_DIR should exist after save_file call");

    // see if it displayes the files correctly
    load_file(empty_ships, count);

    display_ships(empty_ships, count);

    std::cout << "save_file test passed\n";
    return 0;
}
