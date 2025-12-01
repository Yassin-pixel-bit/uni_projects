#pragma once
#include <iostream>
#include <algorithm>
#include "../utils.h"
#include "../ships.h"

using namespace std;

void search_with_name(Ship ships[], const int& ship_count);
void create_search_submenu();
void search_by_cargo(Ship ships[], const int& ship_count);
