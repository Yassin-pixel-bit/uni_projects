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
void add_ship(all_ships &ship_list);

int main()
{
    int choice;
    all_ships ship_list;

    print_menu();

    add_ship(ship_list);

    return 0;
}

void print_menu()
{
    // TODO: print the title in the middle example: =====title=====
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

void add_ship(all_ships &ship_list)
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

    // if ship_list.count doesn't have a value assign a value to it and put the new ship.
    if(!ship_list.count)
    {
        ship_list.count = 1;
        ship_list.ship[0] = &new_ship;
    }
    else 
    {
        ship_list.count++;
        ship_list.ship[ship_list.count - 1] = &new_ship;
    }
}