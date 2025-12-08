#pragma once
#include <string>

enum configs {
    MAX_SHIPS = 20,
    MAX_CONTAINERS = 10
};

struct Container {
    int weight;
};

struct Ship {
    int number;
    std::string name;
    int capacity;
    int used_capacity = 0;
    Container container[MAX_CONTAINERS];
    int container_count = 0;
    int departure_day;
};

// OPTINAL: make a vector-like struct.
// very complex, but fun and informative.
/*
struct ShipArray {
    Ship* data;
    int size; //  number of elements used
    int capacity; //  number of elements allocated
    const int MAX_SIZE = MAX_SHIPS;
}; 
*/

