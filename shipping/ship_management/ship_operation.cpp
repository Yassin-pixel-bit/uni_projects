#include "ship_operation.h"

void display_ships(Ship ships[], const int &count)
{
    if (count == 0)
        write_incolor("There are no ships to display\n", ERROR);

    // Find the longest ship name
    const string name_header = "Ship Name";
    int max_name_length = name_header.length();

    for (int i = 0; i < count; i++)
    {
        if (ships[i].name.length() > max_name_length)
            max_name_length = ships[i].name.length();
    }

    // add padding
    max_name_length += 2;

    // Column widths
    const int id_width = 4;
    const int ship_no_width = 10;
    const int capacity_width = 13;
    const int used_width = 11;
    const int departure_width = 15;
    const int name_width = max_name_length;

    // total table width
    int total_width = id_width + ship_no_width + name_width + capacity_width + used_width + departure_width + 7; // +7 for separators

    // Print top border
    cout << "+";
    for (int i = 0; i < total_width - 2; i++)
        cout << "=";
    cout << "+\n";

    // Print header
    cout << "| " 
         << left << setw(id_width - 1) << "ID" << "| "
         << setw(ship_no_width - 1) << "Ship No." << "| "
         << setw(name_width - 1) << "Ship Name" << "| "
         << setw(capacity_width - 1) << "Capacity(t)" << "| "
         << setw(used_width - 1) << "Used(t)" << "| "
         << setw(departure_width - 1) << "Departure Day" << " |\n";

    // Print header separator
    cout << "+";
    cout << string(id_width, '=') << "|";
    cout << string(ship_no_width, '=') << "|";
    cout << string(name_width, '=') << "|";
    cout << string(capacity_width, '=') << "|";
    cout << string(used_width, '=') << "|";
    cout << string(departure_width, '=');
    cout << "+\n";

    // Print ship data
    for (int i = 0; i < count; i++)
    {
        Ship current_ship = ships[i];
        int id = i + 1;

        cout << "| " 
             << left << setw(id_width - 1) << id << "| "
             << setw(ship_no_width - 1) << current_ship.number << "| "
             << setw(name_width - 1) << current_ship.name << "| "
            << setw(capacity_width - 1) << current_ship.capacity << "| "
            << setw(used_width - 1) << current_ship.used_capacity << "| "
             << setw(departure_width - 1) << current_ship.departure_day << " |\n";
    }

    // Print bottom border
    cout << "+";
    for (int i = 0; i < total_width - 2; i++)
        cout << "=";
    cout << "+\n";
}

int get_remaining_capacity(const Ship &ship)
{
    return ship.capacity - ship.used_capacity;
}

// Make sure that the user-given ship number exists
int check_ship_number(const Ship ships[], const int ship_count, const int input)
{
    // loop through each ship and check it's number with the input
    for (int i = 0; i < ship_count; i++)
    {
        if (ships[i].number == input)
        {
            return i; // return the ship's place in the array
        }
    }

    // if not found return -1
    return -1;
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
        int remaining_weight = get_remaining_capacity(ship);

        if (remaining_weight <= 0)
        {
            write_incolor("The ship can't carry any more weight.\n", ERROR);
            return;
        }

        int weight;

        cout << "how much should container " << i + 1 << " weight(in tons): ";

        while(!(cin >> weight) || weight < 0 || weight > remaining_weight)
        { 
            clear_faulty_input("Enter a positive number not exceeding remaining capacity.\n");
            cout << "Remaining capacity: " << remaining_weight << " tons\n";
            cout << "weight(in tons): ";
        }

        ship.container[ship.container_count].weight = weight;
        ship.used_capacity += weight;
        ship.container_count++;
    }
}

// add container(s) to a user-chosen ship
void user_add_containers(Ship ships[], const int ship_count)
{
    // early exit
    if (ship_count == 0)
    {
        clear_terminal();
        write_incolor("No ships founded!", ERROR);
        return;
    }

    cout << "These are all the available ships: \n";
    cout << "--------------------------------------\n";

    // only list ships that have free capacity
    Ship free_ships[MAX_SHIPS];
    int arr_idx(0);

    for (int i = 0; i < ship_count; i++)
    {
        if (get_remaining_capacity(ships[i]) != 0 && ships[i].container_count < MAX_CONTAINERS)
        {
            free_ships[arr_idx] = ships[i];
            arr_idx++;
        }
    }

    if (arr_idx == 0)
    {
        write_incolor("there are no ships that have free capacity.", INFO);
        return;
    }

    display_ships(free_ships, arr_idx);
    cout << "--------------------------------------\n";

    // ask the user to enter the ship number and then add the container(s)
    bool invalid_shipNo = true;
    int ship_num;
    int ship_idx;
    do
    {
        cout << "Enter the ship number to add a container to it: ";
        while (!(cin >> ship_num))
        {
            clear_faulty_input("Please enter an existing ship number");
            cout << "ship number: ";
        }

        ship_idx = check_ship_number(ships, ship_count, ship_num); 
        if (ship_idx != -1) // did find
        {
            if (get_remaining_capacity(ships[ship_idx]) > 0)
            {
                invalid_shipNo = false; // Valid ship and has space
            }
            else
            {
                cout << "That ship is full. Please choose one from the list above.\n";
            }
        }
        else
            write_incolor("Ship not found!\n", ERROR);
        
    } while (invalid_shipNo);

    add_containers(ships[ship_idx]);
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
    while (!(cin >> new_ship.number) || new_ship.number < 0) 
        { clear_faulty_input("Please enter a positive number.\n"); }

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
