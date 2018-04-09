#pragma once
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <array>

namespace config {

    const auto project_name = "Spreaditor";

    std::array<ImColor, 9> get_rusty_palette();    

}