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
        cout << "----------------------------------------\n";
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

void show_user_cargo_split(Ship ships[], int ship_count, int container_weight)
{
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
        write_incolor("Even combining all available ships, capacity is insufficient.\n", ERROR);
        cout << "Total available: " << total_available << "t / Needed: " << container_weight << "t\n";
        return;
    }

    // 2. Calculation: Generate the Plan
    SplitAction plan[MAX_SHIPS]; // We can't use more ships than exist
    int plan_count = 0;
    
    int remaining_needed = container_weight;

    for (int i = 0; i < ship_count && remaining_needed > 0; i++)
    {
        int space = get_remaining_capacity(ships[i]);
        bool has_slot = ships[i].container_count < MAX_CONTAINERS;

        if (space > 0 && has_slot)
        {
            int take = (remaining_needed < space) ? remaining_needed : space;
            
            // Record the action
            plan[plan_count].ship_index = i;
            plan[plan_count].weight = take;
            plan_count++;

            remaining_needed -= take;
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
        cout << "Ship " << ships[idx].number << " (" << ships[idx].name << "): Load " << weight << " tons";
        
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
    }
    else
    {
        write_incolor("Operation cancelled.\n", INFO);
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
    
    // 1. Try to find single ships first (keep existing logic)
    if (!find_single_ships(ships, ship_count, container_weight))
    {
        // 2. If no single ship works, generate a split plan
        show_user_cargo_split(ships, ship_count, container_weight);
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
