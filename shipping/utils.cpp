#include "utils.h"

void clear_faulty_input(const std::string &message)
{
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');
    std::cout << message;
}