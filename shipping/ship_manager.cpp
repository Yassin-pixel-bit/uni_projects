#include "ship_manager.h"

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

void create_addShip_submenu()
{
    const int size = 3;
    const string options[size] = {"1) add only one ship.",
                              "2) add a list of ships.",
                              "0) return."};
    
    clear_terminal();
    create_sub_menu(options, size);
}

void create_search_submenu()
{
    const int size = 3;
    const string options[size] = {"1) search by name (full or prefix).",
                            "2) Search by cargo amount within limits — enter min and max (tons)",
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