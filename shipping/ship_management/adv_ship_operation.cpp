#include "adv_ship_operation.h"

void create_adv_menu()
{
    const int size = 4;
    string options[size] = {"1) Search ships for a container.",
                            "2) Cargo distribution.",
                            "3) auto-load/save (toggle on/off).",
                            "0) return."};

    clear_terminal();
    create_sub_menu(options, size);
}

void create_container_menu()
{
    const int size = 3;
    string options[size] = {"1) Find ships that can carry a container.",
                            "2) Find ships with cargo larger than 10 tons.",
                            "0) return."};

    clear_terminal();
    create_sub_menu(options, size);
}

void submenu_adv_1(Ship ships[], const int ship_count)
{
    int choice;
    bool running = true;
    create_container_menu();
    cout << "Enter your option: ";

    while(running)
    {
        while(!(cin >> choice))
            { clear_faulty_input("Please enter a valid input.\n"); }

        switch (choice)
        {
            case 0:
                running = false;
                break;

            case 1:
                search_by_cap_left(ships, ship_count);
                running = false;
                break;

            case 2:
                search_ship_loaded_10t(ships, ship_count);
                running = false;
                break;

            default:
                break;
        }
    }
}

// display the ships that can carry a container with a user-given weight
void search_by_cap_left(Ship ships[], const int ship_count)
{
    // early exit
    if (ship_count == 0)
    {
        clear_terminal();
        write_incolor("No ships found!", ERROR);
        return;
    }

    int container_weight;

    cout << "Enter the container weight(in tons): ";
    while(!(cin >> container_weight) || container_weight < 0)
    {
        clear_faulty_input("Please enter a poitive number.\n");
        cout << "Enter the container weight: ";
    }

    clear_terminal();
    
    // Header for the results
    cout << "Ships that can carry " << container_weight << " tons:\n";
    cout << "----------------------------------------\n";

    Ship valid_ships[MAX_SHIPS];
    int new_arr_idx(0);

    // loop through all ships to find the right ships
    for (int i = 0; i < ship_count; i++)
    {
        // a refrence to avoid copying
        const Ship &current_ship = ships[i];
        int remaining_cap = get_remaining_capacity(current_ship);

        if (remaining_cap >= container_weight)
        {
            valid_ships[new_arr_idx] = current_ship;
            new_arr_idx++;
        }
    }

    if (new_arr_idx == 0)
    {
        write_incolor("No ships can carry this amount of cargo", INFO);
        return;
    }

    display_ships(valid_ships, new_arr_idx);
}

// display ships that have more than 10 tons loaded
void search_ship_loaded_10t(Ship ships[], const int ship_count)
{
    // early exit
    if (ship_count == 0)
    {
        clear_terminal();
        write_incolor("No ships found!", ERROR);
        return;
    }

    clear_terminal();
    cout << "Ships with loaded cargo larger than 10 tons\n";
    cout << "----------------------------------------\n";

    Ship valid_ships[MAX_SHIPS];
    int new_arr_idx(0);

    for (int i = 0; i < ship_count; i++)
    {
        // a refrence to avoid copying
        const Ship &current_ship = ships[i];
        if (current_ship.used_capacity > 10)
        {
            valid_ships[new_arr_idx] = current_ship;
            new_arr_idx++;
        }
    }

    if (new_arr_idx == 0)
    {
        write_incolor("No ships have loaded cargo larger than 10 tons", INFO);
        return;
    }
 
    display_ships(valid_ships, new_arr_idx);
}

bool toggle_auto_save(bool& auto_save)
{
    if (auto_save) 
    {
        // Turn OFF
        auto_save = false;
        clear_terminal();
        write_incolor("Auto-save disabled.\n", INFO);
    } 
    else 
    {
        // Turn ON
        auto_save = true;
        clear_terminal();
        write_incolor("Auto-save ENABLED.\n", SUCCESS);
        write_incolor("everytime you make a Change it will be saved automatically.\n", TIP);
        write_incolor("Syncing file now...\n", INFO);
    }

    return auto_save;
}
