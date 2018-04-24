#include <imconfig.h>
#include <imgui.h>
#include <imgui_internal.h>

static float scaling_factor = 1.0f;

using namespace ImGui;

void MyImGui::SetScaling(float scaling_factor) {
	scaling_factor = scaling_factor;
}

void MyImGui::SameLine(float pos_x, float spacing_w) {
	ImGui::SameLine(pos_x * scaling_factor, spacing_w * scaling_factor);
}

bool MyImGui::Button(const char* label, const ImVec2& size_arg) {
	return ImGui::Button(label, ImVec2(
		size_arg.x*scaling_factor,
		size_arg.y*scaling_factor)
	);

}
