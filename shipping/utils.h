#pragma once
#include <iostream>
#include <limits>

using namespace std;

void clear_faulty_input(const std::string &message);
int read_int_in_range(const int &min, const int &max);
void create_sub_menu(const std::string options[], const int &count);
bool exit_program();
void clear_terminal();