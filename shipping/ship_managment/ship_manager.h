#pragma once
#include <iostream>
#include <algorithm>
#include "../utils.h"
#include "../ships.h"

using namespace std;

void display_ships(Ship ships[], const int &count);
void add_ship(Ship ship[], int &ship_count);
void add_multiple_ships(Ship ship[], int &ship_count);
void create_addShip_submenu();
void search_with_name(Ship ships[], const int& ship_count);
void create_search_submenu();
void search_by_cargo(Ship ships[], const int& ship_count);
void create_sort_submenu();
void sort_by_ship_cap(Ship ships[], const int& ship_count);
void sort_by_loaded_cargo(Ship ships[], const int& ship_count);