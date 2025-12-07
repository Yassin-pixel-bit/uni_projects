#pragma once
#include <iostream>
#include "../utils.h"
#include "../ships.h"
#include "ship_operation.h"

using namespace std;

void create_adv_menu();
void create_container_menu();
void search_by_cap_left(Ship ships[], const int ship_count);
void submenu_adv_1(Ship ships[], const int ship_count);