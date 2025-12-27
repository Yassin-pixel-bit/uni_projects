#include "../ship_management/save_load.h"
#include "../ship_management/ship_operation.h" // Required for display_ships
#include "../core/ships.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <vector>

using namespace std;

// --- Helper to write data manually (simulating a save) ---
void write_test_ship_to_stream(ostream& out, const Ship& ship) {
    out.write((char*)&ship.number, sizeof(ship.number));
    
    int name_len = ship.name.length();
    out.write((char*)&name_len, sizeof(name_len));
    out.write(ship.name.c_str(), name_len);
    
    out.write((char*)&ship.capacity, sizeof(ship.capacity));
    out.write((char*)&ship.used_capacity, sizeof(ship.used_capacity));
    out.write((char*)&ship.departure_day, sizeof(ship.departure_day));
    out.write((char*)&ship.container_count, sizeof(ship.container_count));
    
    if (ship.container_count > 0) {
        out.write((char*)ship.container, sizeof(Container) * ship.container_count);
    }
}

// --- Helper to read the file content into a Ship array ---
void read_file_into_array(const string& path, Ship loaded_ships[], int& loaded_count) {
    ifstream in(path, ios::binary);
    assert(in.is_open() && "Could not open file for verification");

    in.read((char*)&loaded_count, sizeof(loaded_count));

    for (int i = 0; i < loaded_count; i++) {
        in.read((char*)&loaded_ships[i].number, sizeof(loaded_ships[i].number));

        int name_len;
        in.read((char*)&name_len, sizeof(name_len));
        
        loaded_ships[i].name.resize(name_len);
        in.read(&loaded_ships[i].name[0], name_len);

        in.read((char*)&loaded_ships[i].capacity, sizeof(loaded_ships[i].capacity));
        in.read((char*)&loaded_ships[i].used_capacity, sizeof(loaded_ships[i].used_capacity));
        in.read((char*)&loaded_ships[i].departure_day, sizeof(loaded_ships[i].departure_day));
        in.read((char*)&loaded_ships[i].container_count, sizeof(loaded_ships[i].container_count));

        if (loaded_ships[i].container_count > 0) {
            in.read((char*)loaded_ships[i].container, sizeof(Container) * loaded_ships[i].container_count);
        }
    }
    in.close();
}

int main() {
    string test_file = "unit_test_display.dat";

    // 1. SETUP: Create an initial file with 1 ship
    Ship old_ship;
    old_ship.number = 101;
    old_ship.name = "Black Pearl";
    old_ship.capacity = 500;
    old_ship.used_capacity = 100;
    old_ship.departure_day = 5;
    old_ship.container_count = 1;
    old_ship.container[0].weight = 100;

    {
        ofstream out(test_file, ios::binary);
        int count = 1;
        out.write((char*)&count, sizeof(count));
        write_test_ship_to_stream(out, old_ship);
        out.close();
    }

    // 2. ACTION: Append 1 new ship
    Ship new_ship;
    new_ship.number = 202;
    new_ship.name = "Flying Dutchman";
    new_ship.capacity = 800;
    new_ship.used_capacity = 0;
    new_ship.departure_day = 25;
    new_ship.container_count = 0;

    Ship ships_to_append[1] = { new_ship };
    
    cout << "Appending new ship to file..." << endl;
    append_ships(test_file, ships_to_append, 1);

    // 3. READ BACK: Load data to verify
    Ship loaded_ships[MAX_SHIPS];
    int loaded_count = 0;
    read_file_into_array(test_file, loaded_ships, loaded_count);

    // 4. DISPLAY: Use your existing function to show the result
    cout << "\n================ VISUAL VERIFICATION ================" << endl;
    cout << "File content after append (Expected: 2 ships):" << endl;
    display_ships(loaded_ships, loaded_count);
    cout << "=====================================================\n" << endl;

    // 5. AUTOMATED CHECKS
    assert(loaded_count == 2);
    assert(loaded_ships[0].name == "Black Pearl");
    assert(loaded_ships[1].name == "Flying Dutchman");

    // Cleanup
    if (filesystem::exists(test_file)) {
        filesystem::remove(test_file);
    }

    cout << "SUCCESS: Test passed." << endl;
    return 0;
}