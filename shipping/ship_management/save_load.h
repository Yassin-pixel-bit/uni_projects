#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "../core/utils.h"
#include "../core/ships.h"
#include "ship_operation.h"

const string SAVE_DIR = "../save_files";
using namespace std;

// prototypes
void save_file(const Ship ships[], int count);
