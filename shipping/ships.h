#include <string>

struct Container {
    int weight;
};

struct Ship {
    int number;
    std::string name;
    int capacity;
    Container container[10]; // MAX_CONTAINERS = 10;
    int container_count;
    std::string departure_date;
};

struct all_ships {
    // MAX_SHIPS = 20;
    Ship* ship[20];
    int count;
};