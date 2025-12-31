#include "save_load.h"

void create_file_submenu()
{
    const int size = 4;
    string options[size] = {"1) load.",
                            "2) save in a new file.",
                            "3) overwrite.",
                            "0) return."};

    clear_terminal();
    create_sub_menu(options, size);
}

string ask_user_for_name()
{
    string file_name;

    cout << "What do you want to save this file as: ";
    while (true)
    {
        bool change = false;

        getline(cin, file_name);

        if (file_name.empty()) 
        {
            continue;
        }

        // search the input for these invalid chars
        // string::npos means "non - position" or in this context not found
        const string invalid_chars = "/\\:*?\"<>|";
        if (file_name.find_first_of(invalid_chars) != string::npos) {
            write_incolor("Filename contains invalid characters ( / \\ : * ? \" < > | ).\n", ERROR);
            cout << "Enter valid name: ";
            continue;
        }
        string file_path = SAVE_DIR + '/' + file_name + ".dat";

        ifstream file;
        file.open(file_path);
        // check if the file exists
        if (file)
        {
            file.close();
            write_incolor("file already exists.\n", INFO);
            cout << "Enter the name: ";
            continue;
        }

        char answer;
        cout << "Are you sure you want to name the file " << file_name << "(y,n): ";
        cin >> answer;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (tolower(answer) == 'n')
        {
            cout << "Operation cancelled.\n Please enter a new name: ";
            continue;
        }

        break;
    }

    clear_terminal();
    return file_name;
}

void write_ship_data(ostream& out_stream, const Ship ships[], const int count)
{
    for (int i = 0; i < count; i++)
    {
        out_stream.write((char*)&ships[i].number,  sizeof(ships[i].number));

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

void save_new_file(const Ship ships[], int count, string& current_file)
{
    if (count <= 0)
    {
        char answer;
        if (!current_file.empty())
        {
            write_incolor("there are no ships to save\n", INFO);
            cout << "Do you want to save anyway[y,n]: ";
            cin >> answer;
        }
        else
            answer = 'y';
        
        if (tolower(answer) == 'n')
        {
            return;
        }
    }
 
    if (!filesystem::exists(SAVE_DIR))
        filesystem::create_directory(SAVE_DIR);

    if (is_directory_full(SAVE_DIR, MAX_FILES))
        { write_incolor("you can't save in a new file! max save files reached", ERROR); return; }

    string filename = ask_user_for_name();
    string file_path = SAVE_DIR + '/' +filename + ".dat";

    ofstream out_stream;
    out_stream.open(file_path, ios::binary);
    if(!out_stream)
    {
        write_incolor("couldn't save the file.\n", ERROR);
        return;
    }

    current_file = file_path;

    out_stream.write((char *)&count, sizeof(count)); // first four bytes
    write_ship_data(out_stream, ships, count);

    out_stream.close();
    string half_message = "succsefully write " + count; 
    write_incolor(half_message + " ships to " + file_path + '\n', SUCCESS);
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

void load_file(Ship ships[], int &count, string& current_file)
{
    if (!filesystem::exists(SAVE_DIR))
    {
        filesystem::create_directory(SAVE_DIR);
    }

    if (filesystem::is_empty(SAVE_DIR))
    {
        write_incolor("No save files found. You must create one to start.\n", INFO);
        save_new_file(ships, count, current_file);
        return;
    }

    // list all the files available for the user
    cout << "All available save files: \n";
    string filenames[MAX_FILES];
    int file_count = 0;
    display_save_files(filenames, file_count);

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
    {
        save_new_file(ships, count, current_file);
        return;
    }

    string file_path = SAVE_DIR + '/' + filenames[choice - 1] + ".dat";
    
    ifstream in_stream;
    in_stream.open(file_path, ios::binary);
    if (!in_stream)
    {
        write_incolor("Couldn't open the file.\n", ERROR);
        return;
    }

    current_file = file_path;

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
            int stored_count = ships[i].container_count;

            if (ships[i].container_count > MAX_CONTAINERS)
            {
                write_incolor("the save file containes more containers than max allowed\nwill only load the max amount of conatiners allowed[10]\n", ERROR);
                ships[i].container_count = MAX_CONTAINERS;
            }

            in_stream.read((char *)&ships[i].container, sizeof(Container) * ships[i].container_count);

            // SKIP the extra data
            if (stored_count > MAX_CONTAINERS)
            {
                int extra_containers = stored_count - MAX_CONTAINERS;
                // Move the file cursor forward by the size of the skipped containers
                in_stream.seekg(sizeof(Container) * extra_containers, ios::cur);
            }
        }
    }

    in_stream.close();
    clear_terminal();
    write_incolor("Successfully loaded the file\n", SUCCESS);   
}

void append_ships(const string file_path, Ship new_ships[], const int new_count)
{
    fstream stream;
    stream.open(file_path, ios::in | ios::out | ios::binary);

    if(!stream)
    {
        clear_terminal();
        write_incolor("Could not open file for appending.\n" ,ERROR);
        return;
    }

    int old_count;
    stream.read((char *)&old_count, sizeof(old_count));

    int total_count = old_count + new_count;

    stream.seekg(0, ios::beg);
    stream.write((char*)&total_count, sizeof(total_count));

    stream.seekg(0, ios::end);

    write_ship_data(stream, new_ships, new_count);

    stream.close();
}

void overwrite_file(const Ship ships[], int count, const string current_filename)
{
    if(current_filename.empty())
    {
        write_incolor("No file is currently open to overwrite.\n", ERROR);
        return;
    }

    ofstream out_stream;
    out_stream.open(current_filename, ios::binary);

    if(!out_stream)
    {
        write_incolor("Can't open the file.\n", ERROR);
        return;
    }

    out_stream.write((char *)&count, sizeof(count));

    write_ship_data(out_stream, ships, count);
    out_stream.close();
}
