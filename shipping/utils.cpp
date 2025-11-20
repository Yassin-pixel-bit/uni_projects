#include "utils.h"

void clear_faulty_input(const string &message)
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << message;
}

void clear_terminal()
{
    cout << "\033[2J\033[1;1H";
}

// TODO: add a function for notification like messages

int read_int_in_range(const int &min, const int &max)
{
    int value;

    while (true)
    {
        if (cin >> value)
        {
            if (value >= min && value <= max)
                return value;    // valid → return
            else
                cout << "Please enter a number between "
                     << min << " and " << max << ".\n";
        }
        else
        {
            // clear bad input
            clear_faulty_input("Please Enter a number.\n");
        }
    }

    return 0;
}

void create_sub_menu(const std::string options[], const int &count)
{
    for (int i = 0; i < count; i++)
    {
        cout << options[i];
    }
}

bool exit_program()
{
    char close;

    while(true)
    {
        cout << "Are you sure you want to exit (y,n) ";
        while(!(cin >> close))
        { clear_faulty_input("Enter y or n.\n"); }

        if(tolower(close) == 'y') 
        { 
            cout << "GoodBye!"; 
            return true; 
        }
        else if(tolower(close) == 'n')
            return false;
        else
            cout << "Enter Y or N.\n";
    }

    cout << "unrecognized answer. Not exiting.";
    return false;
}