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

bool find_single_ships(Ship ships[], int ship_count, int container_weight)
{
    Ship valid_ships[MAX_SHIPS];
    int count = 0;

    for (int i = 0; i < ship_count; i++)
    {
        if (get_remaining_capacity(ships[i]) >= container_weight && ships[i].container_count < MAX_CONTAINERS)
        {
            valid_ships[count++] = ships[i];
        }
    }

    if (count > 0)
    {
        cout << "Ships that can carry " << container_weight << " tons:\n";
        display_ships(valid_ships, count);
        return true;
    }
    return false;
}

// Helper: Applies the pre-calculated split plan
// Now it just blindly follows orders - no math needed!
void apply_cargo_split(Ship ships[], const SplitAction actions[], int action_count)
{
    for (int i = 0; i < action_count; i++)
    {
        int idx = actions[i].ship_index;
        int weight = actions[i].weight;

        // Apply changes
        ships[idx].container[ships[idx].container_count].weight = weight;
        ships[idx].container_count++;
        ships[idx].used_capacity += weight;
    }
    write_incolor("Cargo successfully distributed across ships.\n", SUCCESS);
}

bool show_user_cargo_split(Ship ships[], int ship_count, int container_weight)
{
    clear_terminal();
    write_incolor("No single ship can carry this amount of cargo.\n", INFO);

    // 1. Validation: Check global capacity first
    int total_available = 0;
    for (int i = 0; i < ship_count; i++)
    {
        if (ships[i].container_count < MAX_CONTAINERS)
            total_available += get_remaining_capacity(ships[i]);
    }

    if (total_available < container_weight)
    {
        write_incolor("Insufficient total capacity across all ships.\n", ERROR);
        cout << "Total available: " << total_available << "t / Needed: " << container_weight << "t\n";
        return false;
    }

    // 2. Calculation: Generate the Plan
    SplitAction plan[MAX_SHIPS]; // We can't use more ships than exist
    int plan_count = 0;
    
    int remaining = container_weight;

    for (int i = 0; i < ship_count && remaining > 0; i++)
    {
        int space = get_remaining_capacity(ships[i]);
        bool has_slot = ships[i].container_count < MAX_CONTAINERS;

        if (space > 0 && has_slot)
        {
            int take = (remaining < space) ? remaining : space;
            
            // Record the action
            plan[plan_count].ship_index = i;
            plan[plan_count].weight = take;
            plan_count++;

            remaining -= take;
        }
    }

    // 3. Visualization: Show the plan to the user
    write_incolor("You can split the cargo across these ships:\n", TIP);
    cout << "-----------------------------------------------------\n";
    
    for (int i = 0; i < plan_count; i++)
    {
        int idx = plan[i].ship_index;
        int weight = plan[i].weight;
        
        // We can access the ship details using the index stored in the plan
        cout << "Ship " << ships[idx].number << " ( " << ships[idx].name << " ) " <<": will load " << plan[i].weight << " tons";
        
        if (get_remaining_capacity(ships[idx]) == weight)   
            cout << " (FULL)";
            
        cout << endl;
    }
    cout << "-----------------------------------------------------\n";

    // 4. Execution: Ask user to commit
    char choice;
    do {
        cout << "Do you want to apply this distribution? [y/n]: ";
        cin >> choice;
        choice = tolower(choice);
    } while (choice != 'y' && choice != 'n');

    if (choice == 'y')
    {
        // Pass the PLAN, not the weight
        apply_cargo_split(ships, plan, plan_count);
        return true;
    }
    else
    {
        write_incolor("Operation cancelled.\n", INFO);
        return false;
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
    
    if (!find_single_ships(ships, ship_count, container_weight))
    {
        cout << "No ship can carry this container\n";
    }
}

bool user_add_container(Ship ships[], const int ship_count)
{
    clear_terminal();

    if (ship_count == 0) {
        write_incolor("No ships available!\n", ERROR);
        return false;
    }

    int weight;
    cout << "Enter container weight (tons): ";
    while(!(cin >> weight) || weight <= 0) {
        clear_faulty_input("Enter a positive number: ");
        cout << "Enter container weight (tons): ";
    }

    Ship viable_ships[MAX_SHIPS];
    int viable_count = 0;

    for (int i = 0; i < ship_count; i++)
    {
        if (get_remaining_capacity(ships[i]) >= weight && ships[i].container_count < MAX_CONTAINERS) 
        {
            viable_ships[viable_count++] = ships[i];
        }
    }

    if (viable_count > 0)
    {
        cout << "\nAvailable ships for " << weight << "t container:\n";
        display_ships(viable_ships, viable_count);

        int chosen_ship_num;
        bool invalid_ship_num = false;
        int main_arr_idx;
        do
        {
            cout << "Enter the ship number to add a container to it: ";
            while (!(cin >> chosen_ship_num))
            {
                clear_faulty_input("Please enter a number.\n");
                cout << "Enter the ship number to add a container to it: ";
            }

            if(check_ship_number(viable_ships, viable_count, chosen_ship_num) != -1)
            {
                for(int i = 0; i < ship_count; i++)
                {
                    if(ships[i].number == chosen_ship_num)
                    {
                        main_arr_idx = i;
                        invalid_ship_num = false;
                        break;
                    }
                }
            }
            else
            {
                write_incolor("Please, Choose a ship from the table above.\n", INFO);
                invalid_ship_num = true;
            }
        } while (invalid_ship_num);

        // 3. APPLY THE CHANGE (Actually add the container)
        ships[main_arr_idx].container[ships[main_arr_idx].container_count].weight = weight;
        ships[main_arr_idx].container_count++;
        ships[main_arr_idx].used_capacity += weight;

        write_incolor("Container added successfully!\n", SUCCESS);
        return true;
    }
    else 
    {
        bool splited = show_user_cargo_split(ships, ship_count, weight);
        return splited;
    }
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
    } 
    else 
    {
        // Turn ON
        auto_save = true;
        clear_terminal();
    }

    return auto_save;
}
