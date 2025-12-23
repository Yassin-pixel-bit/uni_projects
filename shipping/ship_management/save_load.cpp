#include "save_load.h"

string ask_user_for_name()
{
    cout << "Asking user for a name.....\n";

    string file_name;
    cout << "What do you want to save this file as: ";
    getline(cin, file_name);

    cout << "got the name.....\n";

    file_name.insert(file_name.begin(), '/');

    cout << file_name << endl;

    return file_name;
}


// Make a save/load function
void save_file(const Ship ships[], int count)
{
    if (!filesystem::exists(SAVE_DIR))
        filesystem::create_directory(SAVE_DIR);

    string filename = ask_user_for_name();
    string file_path = SAVE_DIR + filename + ".dat";

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

void read(Ship ships[], int &count)
{
    // TODO: ask the user which save file he wants to read from
    
}
