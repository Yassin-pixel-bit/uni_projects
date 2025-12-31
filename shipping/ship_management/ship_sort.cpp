#include "ship_sort.h"
#include "ship_operation.h"

void create_sort_submenu()
{
    const int size = 3;
    const string options[size] = {"1) Sort by ship capacity (ascending)",
                            "2) Sort by loaded amount (ascending/descending)",
                            "0) return."};

    clear_terminal();
    create_sub_menu(options, size);
}

void ask_user_tosave(Ship ships[], const int ship_count, char &answer)
{
    clear_terminal();
    display_ships(ships, ship_count);

    do {
        cout << "Do you want to save or use the old arrangment ['y','n']: ";
        cin >> answer;
        answer = tolower(answer); 

    } while (answer != 'y' && answer != 'n');

}

bool sort_by_ship_cap(Ship ships[], const int& ship_count)
{
    Ship sorted_ships[MAX_SHIPS];

    if (ship_count == 0)
    {
        clear_terminal();
        write_incolor("No ships found!\n", ERROR);
        return false;
    }

    if (ship_count == 1)
    {
        clear_terminal();
        write_incolor("There is only one ship, nothing changed.\n", INFO);
        return false;
    }

    copy(&ships[0], ships + ship_count, sorted_ships);

    for (int i = ship_count; i > 1; i--)
    {
        bool sorted = true;

        for (int j = 0; j < i - 1; j++)
        {
            Ship &current_ship = sorted_ships[j];
            Ship &next_ship = sorted_ships[j + 1];

            if (current_ship.capacity > next_ship.capacity)
            {
                swap(current_ship, next_ship);
                sorted = false;
            }
        }

        if (sorted)
            break;
    }

    char answer;
    ask_user_tosave(sorted_ships, ship_count, answer);

    if (answer == 'y')
    {
        clear_terminal();
        copy(&sorted_ships[0], sorted_ships + ship_count, ships);
        return true;
    }
    else
        return false;

}

bool sort_by_loaded_cargo(Ship ships[], const int& ship_count)
{
    clear_terminal();
    string options[2] = {"1) ascending order.", "2) descending order."};
    create_sub_menu(options, 2);

    if (ship_count == 0)
    {
        clear_terminal();
        write_incolor("No ships found!\n", ERROR);
        return false;
    }

    if (ship_count == 1)
    {
        clear_terminal();
        write_incolor("There is only one ship, nothing changed.\n", INFO);
        return false;
    }

    int choice;
    cout << "Enter your choice: ";
    while (!(cin >> choice) || choice > 2 || choice < 1)
    {
        clear_faulty_input("Invalid input. Please Enter either 1 or 2");
    }

    Ship asc_ships[MAX_SHIPS];

    copy(&ships[0], ships + ship_count, asc_ships);

    for (int i = ship_count; i > 1; i--)
    {
        bool sorted = true;

        for (int j = 0; j < i - 1; j++)
        {
            Ship &current_ship = asc_ships[j];
            Ship &next_ship = asc_ships[j + 1];

            if (current_ship.used_capacity > next_ship.used_capacity)
            {
                swap(current_ship, next_ship);
                sorted = false;
            }
        }

        if (sorted)
            break;
    }

    char answer;
    if (choice == 1)
    {
        ask_user_tosave(asc_ships, ship_count, answer);

        if (answer == 'y')
        {
            clear_terminal();
            copy(&asc_ships[0], asc_ships + ship_count, ships);
            return true;
        }
        else
            return false;
    }
    else
    {
        Ship desc_ships[MAX_SHIPS];
        reverse_copy(&asc_ships[0], asc_ships + ship_count, desc_ships);

        ask_user_tosave(desc_ships, ship_count, answer);
        
        if (answer == 'y')
        {
            clear_terminal();
            copy(&desc_ships[0], desc_ships + ship_count, ships);
            return true;
        }
        else
            return false;
    }
}
