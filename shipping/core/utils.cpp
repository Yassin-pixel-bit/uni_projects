#include "utils.h"

void clear_faulty_input(const string &message)
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    write_incolor(message, INFO);
}

void clear_terminal()
{
    // ANSI code
    cout << "\033[2J\033[1;1H";
}

void write_incolor(const string &message, const int &type)
{
    const int red = 31;
    const int green = 32;
    const int yellow = 33;
    const int reset = 0;

    switch (type)
    {
        case ERROR:
            cout << "\033[" << red << "m" << message << "\033[" << reset << "m";
            break;

        case INFO:
            cout << "\033[" << yellow << "m" << message << "\033[" << reset << "m";
            break;

        case SUCCESS:
            cout << "\033[" << green << "m" << message << "\033[" << reset << "m";
            break;

        default:
            // print the message without any colors 
            cout << message;
            break;
    }
}

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
                cout << "Please enter a number between ["
                     << min << " and " << max << "].\n";
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
        cout << options[i] << endl;
    }
}

// TODO: use in the future
void pause()
{
    cout << "\nPress Enter to return...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
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