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