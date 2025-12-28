#pragma once
#include <iostream>
#include "../core/utils.h"
#include "../core/ships.h"
#include "ship_operation.h"

using namespace std;

void create_adv_menu();
void create_container_menu();
void search_by_cap_left(Ship ships[], const int ship_count);
void submenu_adv_1(Ship ships[], const int ship_count);
void search_ship_loaded_10t(Ship ships[], const int ship_count);
bool toggle_auto_save(bool& auto_save);

struct SplitAction
{
    int ship_index;
    int weight;
};
