#include "Config.h"

std::array<ImColor, 9> config::get_rusty_palette() {

    return std::array<ImColor, 9>{ // Bright to Dark
            ImColor(255, 233, 221),
            ImColor(255, 218, 197),
            ImColor(255, 127, 74),
            ImColor(254, 108, 64),
            ImColor(254, 91, 62),
            ImColor(200, 39, 63),
            ImColor(126, 0, 49),
            ImColor(84, 1, 50),
            ImColor(48, 18, 46),
    };
}
