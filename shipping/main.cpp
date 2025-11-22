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
void display_ships(Ship ships[], const int &count);
void create_first_submenu();
void submenu_1(Ship ship[], int &ship_count);

int main()
{
    // TODO: if possible put the ships array in a .ini file or similar
    /* 
    const int MAX_SHIPS = 20;
    const int MAX_CONTAINER = 10;
    */
    int choice, ships_count(0);
    Ship ships[configs::MAX_SHIPS];

    while(true)
    {
        print_menu();

        cout << "Enter your option: ";

        if(!(cin >> choice))
        {
            clear_terminal();
            clear_faulty_input("Please enetr a valid input.\n");
            continue;
        }

        switch (choice)
        {
            case 0:
            if(exit_program())
                return 0;
            clear_terminal();
            break;

            case 1:
                submenu_1(ships, ships_count);
                break;

            case 2:
                display_ships(ships, ships_count);
                break;
            
            default:
                clear_terminal();
                write_incolor("there is no such command. Please enter a number between [1-7].", ERROR);
                break;
        }
    }

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
    cout << "Enter 0 to exit and save.\n";
}

void display_ships(Ship ships[], const int &count)
{
    clear_terminal();

    for (int i = 0; i < count; i++)
    {
        Ship current_ship = ships[i];
        int id = i + 1;
        cout << id << " | shipNo: " << current_ship.number << " Ship name: " << current_ship.name
        << "  capacity(t): " << current_ship.capacity << " Departure day: " << current_ship.departure_day << endl;
    }
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
    while (!(cin >> new_ship.capacity) || new_ship.capacity <= 0)
        { clear_faulty_input("Please enter a positive number.\n"); }

    cout << "departure day: ";
    new_ship.departure_day = read_int_in_range(1, 30);

    // add the newly created ship to the ship array
    ship[ship_count] = new_ship;
    ship_count++;

    clear_terminal();
}

void add_multiple_ships(Ship ship[], int &ship_count)
{
    int amount;

    cout << "How many ships do you want to enter: ";
    
    while(!(cin >> amount) || amount < 0 || amount > configs::MAX_SHIPS)
    { clear_faulty_input("Please enter a valid input."); }

    for (int i = 0; i < amount; i++)
    {
        add_ship(ship, ship_count);
    }
}

void create_first_submenu()
{
    const int size = 3;
    const string options[size] = {"1) add only one ship.\n",
                              "2) add a list of ships.\n",
                              "0) return.\n"};
    
    clear_terminal();
    create_sub_menu(options, size);
}

void submenu_1(Ship ship[], int &ship_count)
{
    int choice;
    bool running = true;
    create_first_submenu();
    cout << "Enter Your option: ";

    while(running)
    {
        
        while(!(cin >> choice))
            { clear_faulty_input("Please enter a valid input."); }

        switch (choice)
        {
            case 0:
                running = false;
                break;

            case 1:
                add_ship(ship, ship_count);
                write_incolor("Ship added!\n\n", SUCCESS);
                running = false;
                break;

            case 2:
                add_multiple_ships(ship, ship_count);
                write_incolor("Ship added!\n\n", SUCCESS);
                running = false;
                break;
                
            default:
                write_incolor("No such command", ERROR);
                break;
        }
    }
}