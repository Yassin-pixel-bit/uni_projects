#include "../ship_management/save_load.h"
#include "../ship_management/ship_operation.h"
#include "../core/ships.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <sstream>
#include <cstring>

using namespace std;

// --- Helper: Clean/Reset Save Directory ---
void reset_env() {
    if (filesystem::exists(SAVE_DIR)) {
        for (const auto& entry : filesystem::directory_iterator(SAVE_DIR))
            filesystem::remove_all(entry.path());
    } else {
        filesystem::create_directory(SAVE_DIR);
    }
}

// Helper to manually write ship fields
// We set a specific departure_day (100) to check for corruption later
void write_manual_ship_header(ostream& out, int number, string name) {
    out.write((char*)&number, sizeof(number));
    
    int name_len = name.length();
    out.write((char*)&name_len, sizeof(name_len));
    out.write(name.c_str(), name_len);
    
    int dummy_cap = 1000;
    out.write((char*)&dummy_cap, sizeof(dummy_cap)); // capacity
    out.write((char*)&dummy_cap, sizeof(dummy_cap)); // used_capacity
    
    int dep_day = 100; 
    out.write((char*)&dep_day, sizeof(dep_day)); // departure_day
}

void test_ship_limit_overflow() {
    cout << "\n[TEST] Loading file with 25 ships (Max allowed: " << MAX_SHIPS << ")..." << endl;
    reset_env();

    string file_path = SAVE_DIR + "/overflow_ships.dat";
    ofstream out(file_path, ios::binary);
    
    int excess_count = MAX_SHIPS + 5; 
    out.write((char*)&excess_count, sizeof(excess_count));

    for (int i = 0; i < excess_count; i++) {
        write_manual_ship_header(out, i, "Ship_" + to_string(i));
        int zero_containers = 0;
        out.write((char*)&zero_containers, sizeof(zero_containers));
    }
    out.close();

    istringstream mock_input("1\n");
    streambuf* old_cin = cin.rdbuf(mock_input.rdbuf());

    Ship ships[MAX_SHIPS];
    int count = 0;
    load_file(ships, count);

    cin.rdbuf(old_cin);

    assert(count == MAX_SHIPS);
    cout << "[PASS] Successfully truncated extra ships." << endl;
}

void test_container_skip_logic() {
    cout << "\n[TEST] Loading ship with 15 containers + verifying next ship..." << endl;
    reset_env();

    string file_path = SAVE_DIR + "/skip_logic.dat";
    ofstream out(file_path, ios::binary);

    int ship_count = 2; // We write 2 ships
    out.write((char*)&ship_count, sizeof(ship_count));

    // --- SHIP 1: The "Corrupt" One (15 containers) ---
    write_manual_ship_header(out, 1, "OverflowShip");
    
    int excess_containers = MAX_CONTAINERS + 5; // 15
    out.write((char*)&excess_containers, sizeof(excess_containers));

    for(int i=0; i < excess_containers; i++) {
        Container c;
        // First 10 = weight 1, Extra 5 = weight 99 (If 99 appears in memory, we failed)
        c.weight = (i < MAX_CONTAINERS) ? 1 : 99; 
        out.write((char*)&c, sizeof(Container));
    }

    // --- SHIP 2: The "Control" One (Normal) ---
    // If the reader didn't skip the extra 5 containers of Ship 1, 
    // it will be reading garbage when it tries to read Ship 2's ID.
    write_manual_ship_header(out, 2, "RescueShip");
    int zero_containers = 0;
    out.write((char*)&zero_containers, sizeof(zero_containers));

    out.close();

    // Mock Input
    istringstream mock_input("1\n");
    streambuf* old_cin = cin.rdbuf(mock_input.rdbuf());

    Ship ships[MAX_SHIPS];
    int count = 0;
    load_file(ships, count);

    cin.rdbuf(old_cin);

    // --- VERIFICATION ---
    
    // 1. Check Ship 1 Container Count
    assert(ships[0].container_count == MAX_CONTAINERS); 
    cout << "Checked: Container count capped at 10." << endl;

    // 2. Check Memory Integrity (Departure Day)
    // If we overwrote the array bounds, departure_day (value 100) 
    // might have been overwritten by the extra container weight (99).
    if (ships[0].departure_day != 100) {
        cerr << "[FAIL] Memory Corruption Detected! departure_day became " << ships[0].departure_day << endl;
        exit(1);
    }
    cout << "Checked: Memory not corrupted (departure_day is intact)." << endl;

    // 3. Check Ship 2 (The crucial skip check)
    // If skipping failed, Ship 2 wouldn't load or would have a garbage name.
    assert(count == 2);
    if (ships[1].name != "RescueShip") {
        cerr << "[FAIL] File Sync Lost! Could not read 2nd ship correctly." << endl;
        cerr << "Read name: " << ships[1].name << endl;
        exit(1);
    }
    cout << "Checked: File stream synchronized (RescueShip loaded correctly)." << endl;

    cout << "[PASS] Successfully skipped extra containers." << endl;
}

int main() {
    if(filesystem::exists(SAVE_DIR)) filesystem::remove_all(SAVE_DIR);
    
    try {
        test_ship_limit_overflow();
        test_container_skip_logic();
    } catch (const exception& e) {
        cerr << "Test Exception: " << e.what() << endl;
        return 1;
    }

    if(filesystem::exists(SAVE_DIR)) filesystem::remove_all(SAVE_DIR);

    cout << "\nAll Strict Edge Case Tests Passed!" << endl;
    return 0;
}