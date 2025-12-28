#pragma once
#include <iostream>
#include <algorithm>
#include "../core/utils.h"
#include "../core/ships.h"

using namespace std;

void create_sort_submenu();
bool sort_by_ship_cap(Ship ships[], const int& ship_count);
bool sort_by_loaded_cargo(Ship ships[], const int& ship_count);
