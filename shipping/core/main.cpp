#include "../ship_management/ship_search.h"
#include "../ship_management/ship_sort.h"
#include "../ship_management/ship_operation.h"
#include "../ship_management/adv_ship_operation.h"
#include "../ship_management/save_load.h"
#include "ships.h"

using namespace std;

void print_menu();
void submenu_addShip(Ship ship[], int &ship_count, bool auto_save, const string& current_file);
void submenu_search(Ship ship[], int &ship_count);
void submenu_sort(Ship ship[], int &ship_count, const bool auto_save, const string& current_file);
void submenu_adv(Ship ship[], int &ship_count, bool& auto_save, const string& current_file);
void submenu_file(Ship ship[], int &ship_count, string& current_filename);

int main()
{
    clear_terminal();

    int choice, ships_count(0);
    Ship ships[configs::MAX_SHIPS];
    string current_file = "";
    bool auto_save = false;

    load_file(ships, ships_count, current_file);

    while(true)
    {
        if (!auto_save)
        {
            write_incolor("Auto-save is OFF. Toggle it On in Advanced features to save automatically.\n", TIP);
        }

        print_menu();

        cout << "Enter your option: ";

        if(!(cin >> choice))
        {
            clear_terminal();
            clear_faulty_input("Please enter a valid input.\n");
            continue;
        }

        switch (choice)
        {
            case 0:
            if(exit_program(ships, auto_save,ships_count, current_file))
                return 0;
            clear_terminal();
            break;

            case 1:
                submenu_addShip(ships, ships_count, auto_save, current_file);
                break;
            
            case 2:
                if (user_add_container(ships, ships_count) )
                {
                    if(auto_save)
                    {
                        overwrite_file(ships, ships_count, current_file);
                        clear_terminal();
                        write_incolor("[Auto-save] changes saved", SUCCESS);
                    }
                }
                break;

            case 3:
                clear_terminal();
                display_ships(ships, ships_count);
                break;

            case 4:
                submenu_search(ships, ships_count);
                break;

            case 5:
                submenu_sort(ships, ships_count, auto_save, current_file);
                break;

            case 6:
                submenu_file(ships, ships_count, current_file);
                break;

            case 7:
                submenu_adv(ships, ships_count, auto_save, current_file);
                break;

            case 8:
                search_with_freecap(ships, ships_count);
                break;
            
            default:
                clear_terminal();
                write_incolor("there is no such command. Please enter a number between [1-7].\n", ERROR);
                break;
        }
    }

    return 0;
}

void print_menu()
{
    cout << "\n======SHIPPING======\n\n";
    
    cout << "1) add ships (single or list).\n";
    cout << "2) add a container.\n";
    cout << "3) display all shipments.\n";
    cout << "4) Search & filter.\n";
    cout << "5) Sort.\n";
    cout << "6) File managment.\n";
    cout << "7) advanced features.\n";
    cout << "8) Ship with the largest free capacity.\n";
    cout << "0) exit.\n";
}

void submenu_addShip(Ship ship[], int &ship_count, bool auto_save, const string& current_file)
{
    int choice;
    bool running = true;
    create_addShip_submenu();
    cout << "Enter Your option: ";

    int old_count = ship_count;

    while(running)
    {
        
        while(!(cin >> choice))
            { clear_faulty_input("Please enter a valid input.\n"); }

        switch (choice)
        {
            case 0:
                clear_terminal();
                running = false;
                break;

            case 1:
                add_ship(ship, ship_count);
                write_incolor("Ship added!\n", SUCCESS);

                if (auto_save && !current_file.empty()) 
                {
                    append_ships(current_file, &ship[old_count], 1);
                    write_incolor("[Auto-Save] changes saved.\n", SUCCESS);
                }

                running = false;
                break;

            case 2:
                add_multiple_ships(ship, ship_count);
                write_incolor("Ship added!\n", SUCCESS);

                if (auto_save && !current_file.empty()) 
                {
                    int added_amount = ship_count - old_count;
                    if(added_amount > 0) 
                    {
                        append_ships(current_file, &ship[old_count], added_amount);
                        write_incolor("[Auto-Save] changes saved.\n", SUCCESS);
                    }
                }

                running = false;
                break;
                
            default:
                write_incolor("No such command\n", ERROR);
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
            { clear_faulty_input("Please enter a valid input.\n"); }

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
        
        case 2:
            search_by_cargo(ship, ship_count);
            running = false;
            break;
        
        default:
            break;
        }
    }
}

void submenu_sort(Ship ship[], int &ship_count, const bool auto_save, const string& current_file)
{
    int choice;
    bool running = true;
    create_sort_submenu();
    cout << "Enter your option: ";

    while (running)
    {
        while(!(cin >> choice))
            { clear_faulty_input("Please enter a valid input.\n"); }

        switch (choice)
        {
        case 0:
            clear_terminal();
            running = false;
            break;

        case 1:
            if (sort_by_ship_cap(ship, ship_count) && auto_save)
            {
                overwrite_file(ship, ship_count, current_file);
                write_incolor("[Auto-save] changes saved.\n", SUCCESS);
            }
            running = false;
            break;
        
        case 2:
            if (sort_by_loaded_cargo(ship, ship_count) && auto_save)
            {
                overwrite_file(ship, ship_count, current_file);
                write_incolor("[Auto-save] changes saved.\n", SUCCESS);
            }
            running = false;
            break;
        
        default:
            clear_terminal();
            write_incolor("No such command\n", ERROR);
            break;
        }
    }
}

void submenu_adv(Ship ship[], int &ship_count, bool& auto_save, const string& current_file)
{
    int choice;
    bool running = true;

    cout << "Auto-Save : " << (auto_save ? "ON\n" : "OFF\n");
    if (auto_save && !current_file.empty()) 
        cout << "Current File: " << current_file << "\n\n";

    create_adv_menu();
    cout << "Enter your option: ";

    while (running)
    {
        while(!(cin >> choice))
            { clear_faulty_input("Please enter a valid input.\n"); }

        switch (choice)
        {
            case 0:
                clear_terminal();
                running = false;
                break;
                
            case 1:
                submenu_adv_1(ship, ship_count);
                running = false;
                break;

            case 2:
                if (distribute_ship_cargo(ship, ship_count) && auto_save)
                {
                    overwrite_file(ship, ship_count, current_file);
                    write_incolor("[Auto-save] Changes saved.\n", SUCCESS);
                }
                
                running = false;
                break;

            case 3:
                if(toggle_auto_save(auto_save))
                {
                    overwrite_file(ship, ship_count, current_file);
                    write_incolor("Auto-save ENABLED.\n", SUCCESS);
                    write_incolor("everytime you make a Change it will be saved automatically.\n", TIP);
                }
                else
                    write_incolor("Auto-save DISABLED", SUCCESS);

                running = false;
                break;
            
            default:
                clear_terminal();
                write_incolor("No such command\n", ERROR);
                break;
        }
    } 
}

void submenu_file(Ship ship[], int &ship_count, string& current_filename)
{
    int choice;
    bool running = true;
    create_file_submenu();
    cout << "Enter your option: ";

    while (running)
    {
        while(!(cin >> choice))
            { clear_faulty_input("Please enter a valid input.\n"); }

        switch (choice)
        {
            case 0:
                clear_terminal();
                running = false;
                break;

            case 1:
                clear_terminal();
                load_file(ship, ship_count, current_filename);
                running = false;
                break;

            case 2:
                clear_terminal();
                save_new_file(ship, ship_count, current_filename);
                running = false;
                break;

            case 3:
                overwrite_file(ship, ship_count, current_filename);
                running = false;
                break;
            
            default:
                clear_terminal();
                write_incolor("No such command\n", ERROR);
                break;
        }
    }
    
}