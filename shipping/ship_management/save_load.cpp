#include "save_load.h"

string ask_user_for_name()
{
    cout << "Asking user for a name.....\n";

    string file_name;

    while (true)
    {
        cout << "What do you want to save this file as: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, file_name);

        if (file_name.empty()) 
        {
            write_incolor("Filename cannot be empty.\n", ERROR);
            continue;
        }

        // search the input for these invalid chars we use the escape character '\' before \ and "
        // string::npos means "non - position" or in this context not found
        const string invalid_chars = "/\\:*?\"<>|";
        if (file_name.find_first_of(invalid_chars) != string::npos) {
            write_incolor("Filename contains invalid characters ( / \\ : * ? \" < > | ).\n", ERROR);
            continue;
        }

        string file_path = SAVE_DIR + '/' + file_name + ".dat";

        fstream file;
        file.open(file_path);

        // check if the file exists
        if (file)
        {
            file.close();
            write_incolor("file already exists.\n", INFO);
            continue;
        }

        break;
    }

    cout << "got the name.....\n";

    cout << file_name << endl;

    return file_name;
}

bool is_directory_full(string directory, const int limit)
{
    bool exceed = false;

    int existing_files = 0;
    for (const filesystem::directory_entry& entry : filesystem::directory_iterator(directory)) 
    {
        if (entry.path().extension() == ".dat") 
            existing_files++;

        if (existing_files >= limit) 
        {
            write_incolor("You have hit the maximum amount of save files.\n", ERROR);
            exceed = true;
        }
    }

    return exceed;
}

// save the ships in a new file
void save_file(const Ship ships[], int count)
{
    if (count <= 0)
    {
        write_incolor("there are no ships to save\n", INFO);
        return;
    }
 
    if (!filesystem::exists(SAVE_DIR))
        filesystem::create_directory(SAVE_DIR);

    if (is_directory_full(SAVE_DIR, MAX_FILES))
        return;

    string filename = ask_user_for_name();
    string file_path = SAVE_DIR + '/' +filename + ".dat";

    ofstream out_stream;
    out_stream.open(file_path, ios::binary);
    if(!out_stream)
    {
        // replace with write_in_color()
        cout << "A problem happend.\n";
        return;
    }

    out_stream.write((char *)&count, sizeof(count));

    for (int i = 0; i < count; i++)
    {
        out_stream.write((char*)&ships[i].number,  sizeof(ships[i].number));

        // WRITE THE STRING LENGTH, THEN THE STRING AS CSTRING
        int ship_name_len = ships[i].name.length();
        out_stream.write((char *)&ship_name_len, sizeof(ship_name_len));
        out_stream.write(ships[i].name.c_str(), ship_name_len);

        out_stream.write((char *)&ships[i].capacity, sizeof(ships[i].capacity));
        out_stream.write((char *)&ships[i].used_capacity, sizeof(ships[i].used_capacity));
        out_stream.write((char *)&ships[i].departure_day, sizeof(ships[i].departure_day));

        out_stream.write((char *)&ships[i].container_count, sizeof(ships[i].container_count));

        if (ships[i].container_count > 0)
        {
            // we can do so because all the containers are saved in an array which make them one after the other in memory
            out_stream.write((char *)&ships[i].container, sizeof(Container) * ships[i].container_count);
        }
    }

    out_stream.close();
    cout << "succsefully wrote " << count <<" ships to" << file_path << ".\n";
}

void display_save_files(string filenames[], int& file_counter)
{
    cout << "0) new file\n";

    for (const filesystem::directory_entry& entry : filesystem::directory_iterator(SAVE_DIR))
    {
        if (file_counter >= MAX_FILES)
            break;

        if (entry.path().extension() == ".dat")
        {
            string file_name = entry.path().stem().string();

            filenames[file_counter] = file_name;

            cout << file_counter + 1 << ") " << file_name << endl;

            file_counter++;
        }
    }
}

void load_file(Ship ships[], int &count)
{
    // TODO: ask the user which save file he wants to read from
    if (!filesystem::exists(SAVE_DIR) || filesystem::is_empty(SAVE_DIR))
    {
        write_incolor("No save directory or files found!\n", INFO);
        return;
    }

    // list all the files available for the user
    cout << "All available save files: \n";
    string filenames[MAX_FILES];
    int file_count = 0;
    display_save_files(filenames, file_count);

    // ask the user to choose which file
    int choice;

    while (true)
    {
        cout << "Enter the file that u want to use[ 0 - " << file_count << "] : ";
        while (!(cin >> choice) || choice < 0 || choice > file_count)
        {
            clear_faulty_input("Enter a number in the specified range\n");
            cout << "Enter the file that u want to use[ 0 - " << file_count << "] : ";
        }

        if (choice == 0 && file_count >= MAX_FILES)
        {
            write_incolor("You have hit the maximum amount of save files.\n", ERROR);
            continue;
        }

        break;
    }

    clear_terminal();

    if (choice == 0)
        return;

    string file_path = SAVE_DIR + '/' + filenames[choice - 1] + ".dat";
    cout << file_path << endl;
    
    ifstream in_stream;
    in_stream.open(file_path, ios::binary);
    if (!in_stream)
    {
        write_incolor("Couldn't open the file.\n", ERROR);
        return;
    }

    in_stream.read((char *)&count, sizeof(count));

    if (count > MAX_SHIPS)
    {
        write_incolor("The save file contains more ships than the maximum allowed [20]\nWill only load the first 20.\n", INFO);
        count = MAX_SHIPS;
    }

    for (int i = 0 ; i < count; i++)
    {
        in_stream.read((char *)&ships[i].number, sizeof(ships[i].number));

        int name_len;
        in_stream.read((char *)&name_len, sizeof(name_len));

        ships[i].name.resize(name_len);
        in_stream.read(&ships[i].name[0], name_len);

        in_stream.read((char *)&ships[i].capacity, sizeof(ships[i].capacity));
        in_stream.read((char *)&ships[i].used_capacity, sizeof(ships[i].used_capacity));
        in_stream.read((char *)&ships[i].departure_day, sizeof(ships[i].departure_day));
        in_stream.read((char *)&ships[i].container_count, sizeof(ships[i].container_count));

        if (ships[i].container_count > 0)
        {
            if (ships[i].container_count > MAX_CONTAINERS)
            {
                write_incolor("the save file containes more containers than max allowed\nwill only load the max amount of conatiners allowed[10]\n", ERROR);
                ships[i].container_count = MAX_CONTAINERS;
            }

            in_stream.read((char *)&ships[i].container, sizeof(Container) * ships[i].container_count);
        }
    }

    in_stream.close();
    write_incolor("Successfully loaded the file\n", SUCCESS);   
}
