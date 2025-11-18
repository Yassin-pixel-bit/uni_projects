/*
=============================================================================
main.cpp
=============================================================================
*/
#include "ships.h"
#include "utils.h"

using namespace std;

// prototypes
void print_menu();
void add_ship(Ship ship[], int&);

int main()
{
    /* 
    const int MAX_SHIPS = 20;
    const int MAX_CONTAINER = 10;
    */
    int choice, ships_count(0);
    Ship ships[configs::MAX_SHIPS];

    print_menu();

    add_ship(ships, ships_count);

    return 0;
}

void print_menu()
{
    // print the title
    cout << "\n======SHIPPING======\n\n";
    
    // print the main menu
    cout << "1) add ships (single or list).\n";
    cout << "2) display all shipments.\n";
    cout << "3) Serach & filter.\n";
    cout << "4) Sort.\n";
    cout << "5) File managment.\n";
    cout << "6) advanced features.\n";
    cout << "7) Ship with the largest free capacity.\n";
    cout << "Press any Character to exit and save.\n";
}

void add_ship(Ship ship[], int &ship_count)
{
    // TODO: read ship parameters from input and pass them to Ship constructor
    Ship new_ship;

    cout << "Ship name: ";
    cin >> ws; // clears cin.
    getline(cin, new_ship.name);

    cout << "Ship number: ";
    while (!(cin >> new_ship.number)) 
        { clear_faulty_input("Please enter a number.\n"); }

    cout << "Ship max capacity: ";
    while (!(cin >> new_ship.capacity))
        { clear_faulty_input("Please enter a number.\n"); }

    cout << "departure day: ";
    new_ship.departure_day = read_int_in_range(1, 30);

    // add the newly created ship to the ship array
    ship[ship_count] = new_ship;
    ship_count++;
}