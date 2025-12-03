#include "ship_search.h"
#include "ship_operation.h"

void create_search_submenu()
{
    const int size = 3;
    const string options[size] = {"1) search by name (full or prefix).",
                            "2) Search by loaded cargo amount within limits - enter min and max (tons)",
                            "0) return."};

    clear_terminal();
    create_sub_menu(options, size);
}

// ONLY CHECKS IF THE STRING SATRTS WITH THE USER INPUT
// case-insensitive
bool is_valid_name(const string& prefix, const string& name)
{
    const int prefix_length = prefix.size();

    // if the checking name is smaller
    if (prefix_length > name.size())
        return false;

    for (int i = 0; i < prefix_length; i++)
    {
        if(!(tolower(prefix[i]) == tolower(name[i])))
            return false;
    }

    return true;
}

// search with ship name (full or partial)
void search_with_name(Ship ships[], const int& ship_count)
{
    // the ships that we found 
    Ship valid_ships[MAX_SHIPS];
    int found_count = 0;

    string prefix;
    cout << "Search: ";
    cin >> ws; // clear the leftover input.
    getline(cin, prefix);

    for (int i = 0; i < ship_count; i++)
    {
        string ship_name = ships[i].name;
        if (is_valid_name(prefix, ship_name))
        {
            if (found_count < MAX_SHIPS)
            {
                valid_ships[found_count++] = ships[i];
            }
        }
    }

    // display the found ships
    if (found_count > 0)
        display_ships(valid_ships, found_count);
    else
        write_incolor("No ships founded!", ERROR);
}

void get_min_max(int &min, int &max)
{
    cout << "Enter the min weight in tons: ";
    while (!(cin >> min) || min < 0)
    {
        clear_faulty_input("Please, enter a positive number.\n");
    }

    cout << "Enter the max weight in tons: ";
    while (!(cin >> max) || max < 0)
    {
        clear_faulty_input("Please, enter a positive number.\n");
    }
}

// search the ship that has a loaded cap within a certain range
void search_by_cargo(Ship ships[], const int& ship_count)
{
    Ship valid_ships[MAX_SHIPS];
    int ship_counter(0), min, max;

    get_min_max(min, max);

    if (min > max)
    {
        swap(min, max);
        write_incolor("the min number is larger than the max number\nSwapping the numbers.\n", INFO);
    }

    for (int i = 0; i < ship_count; i++)
    {
        // to avoid creating a new ship I used a refrence
        const Ship &current_ship = ships[i];

        if (current_ship.used_capacity <= max && current_ship.used_capacity >= min)
            {
                valid_ships[ship_counter] = current_ship;
                ship_counter++;
            }
    }

    if (ship_counter == 0)
    {
        write_incolor("No ships in that range were found", INFO);
    }
    else
    {
        display_ships(valid_ships, ship_counter);
    }
}

// Gets the ship with the most free capacity
void search_with_freecap(Ship ships[], const int& ship_count)
{
    if (ship_count == 0)
    {
        clear_terminal();
        write_incolor("No ships founded!\n", ERROR);
        return;
    }

    if (ship_count == 1)
    {
        clear_terminal();
        display_ships(ships, ship_count);
        return;
    }

    Ship largest_freecap_ship = ships[0];
    for (int i = 1; i < ship_count; i++)
    {
        Ship current_ship = ships[i];
        int current_ship_free_cap = get_remaining_capacity(current_ship);
        if (current_ship_free_cap > get_remaining_capacity(largest_freecap_ship))
            largest_freecap_ship = current_ship;
    }

    clear_terminal();
    display_ships(&largest_freecap_ship, 1);
}
