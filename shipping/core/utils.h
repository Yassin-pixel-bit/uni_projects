#pragma once
#include <iostream>
#include <limits>

using namespace std;

enum message_type {
    ERROR = 0,
    INFO = 1,
    SUCCESS = 2
};

void clear_faulty_input(const std::string &message);
int read_int_in_range(const int &min, const int &max);
void create_sub_menu(const std::string options[], const int &count);
bool exit_program();
void clear_terminal();
void write_incolor(const string &message, const int &type);