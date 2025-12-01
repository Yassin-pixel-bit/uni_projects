#pragma once
#include <iostream>
#include <algorithm>
#include "../utils.h"
#include "../ships.h"

using namespace std;

void create_sort_submenu();
void sort_by_ship_cap(Ship ships[], const int& ship_count);
void sort_by_loaded_cargo(Ship ships[], const int& ship_count);