#include "adv_ship_operation.h"

void create_adv_menu()
{
    const int size = 4;
    string options[size] = {"1) Search ships for a container.",
                            "2) Cargo distribution.",
                            "3) auto-load/save at start/exit (toggle on/off).",
                            "0) return."};

    clear_terminal();
    create_sub_menu(options, size);
}

// TODO: everything.