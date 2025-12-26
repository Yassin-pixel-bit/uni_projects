#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "../core/utils.h"
#include "../core/ships.h"
#include "ship_operation.h"

const string SAVE_DIR = "save_files";
const int MAX_FILES = 10;
using namespace std;

// prototypes
void save_file(const Ship ships[], int count);
void load_file(Ship ships[], int &count);
void create_file_submenu();
