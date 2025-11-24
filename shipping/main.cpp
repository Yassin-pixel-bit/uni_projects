/*
=============================================================================
main.cpp
=============================================================================
*/
#include "ship_manager.h"

using namespace std;

// prototypes
void print_menu();
void submenu_addShip(Ship ship[], int &ship_count);
void submenu_search(Ship ship[], int &ship_count);

int main()
{
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
                submenu_addShip(ships, ships_count);
                break;

            case 2:
                display_ships(ships, ships_count);
                break;

            case 3:
                submenu_search(ships, ships_count);
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

void submenu_addShip(Ship ship[], int &ship_count)
{
    int choice;
    bool running = true;
    create_addShip_submenu();
    cout << "Enter Your option: ";

    while(running)
    {
        
        while(!(cin >> choice))
            { clear_faulty_input("Please enter a valid input."); }

        switch (choice)
        {
            case 0:
                clear_terminal();
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

void submenu_search(Ship ship[], int &ship_count)
{
    int choice;
    bool running = true;
    create_search_submenu();
    cout << "Enter your option: ";

    while (running)
    {
        while(!(cin >> choice))
            { clear_faulty_input("Please enter a valid input."); }

        switch (choice)
        {
        case 0:
            clear_terminal();
            running = false;
            break;

        case 1:
            search_with_name(ship, ship_count);
            running = false;
            break;
        
        default:
            break;
        }
    }
}