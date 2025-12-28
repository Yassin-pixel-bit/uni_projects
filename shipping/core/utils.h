#pragma once
#include <iostream>
#include <limits>
#include "ships.h"
#include "../ship_management/save_load.h"

using namespace std;

enum message_type {
    ERROR = 0,
    INFO = 1,
    SUCCESS = 2,
    TIP = 3
};

void clear_faulty_input(const std::string &message);
int read_int_in_range(const int &min, const int &max);
void create_sub_menu(const std::string options[], const int &count);
bool exit_program(Ship ships[], bool auto_save, const int ship_count, const string& file_name);
void clear_terminal();
void write_incolor(const string &message, const int &type);