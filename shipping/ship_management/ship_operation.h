#pragma once
#include <iostream>
#include <algorithm>
#include <iomanip>
#include "../core/utils.h"
#include "../core/ships.h"

using namespace std;

void display_ships(Ship ships[], const int &count);
void add_ship(Ship ship[], int &ship_count);
void add_containers(Ship &ship);
void add_multiple_ships(Ship ship[], int &ship_count);
void create_addShip_submenu();
int get_remaining_capacity(const Ship &ship);
