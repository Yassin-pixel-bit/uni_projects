#include "ship_manager.h"

void display_ships(Ship ships[], const int &count)
{
    clear_terminal();

    if (count == 0)
        write_incolor("There are no ships to display\n", ERROR);

    for (int i = 0; i < count; i++)
    {
        Ship current_ship = ships[i];
        int id = i + 1;
        cout << id << " | shipNo: " << current_ship.number << " Ship name: " << current_ship.name
        << "  capacity(t): " << current_ship.capacity << " Departure day: " << current_ship.departure_day << endl;
    }
}

int get_remaning_capacity(const Ship &ship)
{
    return ship.capacity - ship.used_capcity;
}

void add_containers(Ship &ship)
{
    // TODO: make sure that the containers weight don't exceed the ship's load capacity

    int container_count;

    cout << "How many containers do you want to add: ";
    while(!(cin >> container_count) || container_count > MAX_CONTAINERS || container_count < 0)
    {
        clear_faulty_input("Please enter a number between [0 - 10]\n"); 
    }

    // checks if the ship can carry more containers
    int space_left = MAX_CONTAINERS - ship.container_count;
    if (container_count > space_left)
    {
        cout << "Only " << space_left << " container slots available. Adding maximum allowed.\n";
        container_count = space_left;
    }

    if (container_count == 0)
        return;

    for (int i = 0; i < container_count; i++)
    {
        int remaning_weight = get_remaning_capacity(ship);

        if (remaning_weight <= 0)
        {
            write_incolor("The ship can't carry any more weight.\n", ERROR);
            return;
        }

        int weight;

        cout << "how much should container " << i + 1 << " weight(in tons): ";

        while(!(cin >> weight) || weight < 0 || weight > remaning_weight)
        { 
            clear_faulty_input("Enter a positive number not exceeding remaining capacity.\n");
            cout << "Remaining capacity: " << remaning_weight << " tons\n";
        }

        ship.container[ship.container_count].weight = weight;
        ship.used_capcity += weight;
        ship.container_count++;
    }
}

void add_ship(Ship ship[], int &ship_count)
{
    if (ship_count >= MAX_SHIPS)
    {
        write_incolor("Maximum ship limit reached.\n", INFO);
        return;
    }

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

    char cont_agree;
    do {
        cout << "Do you want to add containers to this ship [y/n]: ";
        cin >> cont_agree;
        cont_agree = tolower(cont_agree);
    } while (cont_agree != 'y' && cont_agree != 'n');

    if (cont_agree == 'y')
    {
        clear_terminal();
        add_containers(new_ship);
    }

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

        if (current_ship.used_capcity <= max && current_ship.used_capcity >= min)
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