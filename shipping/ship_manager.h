#pragma once
#include <iostream>
#include "utils.h"
#include "ships.h"

using namespace std;

void display_ships(Ship ships[], const int &count);
void add_ship(Ship ship[], int &ship_count);
void add_multiple_ships(Ship ship[], int &ship_count);
void create_addShip_submenu();
void search_with_name(Ship ships[], const int& ship_count);
void create_search_submenu();
void search_by_cargo(Ship ships[], const int& ship_count);