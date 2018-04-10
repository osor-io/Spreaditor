#include "GUIManager.h"
#include "../time/TimeManager.h"
#include <Debug.h>
#include <../config/Config.h>
#include "../debug/cafe.inl"
#include "../utils/File.h"
#include "../render/RenderManager.h"
#include "gsl\gsl"
#include "../os/OSManager.h"

//@@TODO @@REMOVE
#include "../sprites/Spritesheet.h"

GUIManager::GUIManager() {}

GUIManager::~GUIManager() {}

void GUIManager::start_up() {

    ImGuiStyle * style = &ImGui::GetStyle();

    auto palette = config::get_rusty_palette();


    style->WindowPadding = ImVec2(15, 15);
    style->WindowRounding = 5.0f;
    style->FramePadding = ImVec2(10, 5);
    style->FrameRounding = 4.0f;
    style->ItemSpacing = ImVec2(12, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;

    style->Colors[ImGuiCol_Text] = palette[0];
    style->Colors[ImGuiCol_TextDisabled] = palette[1];
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 0.80f);
    style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border] = palette[1];
    style->Colors[ImGuiCol_BorderShadow] = palette[0];
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = palette[7];  //ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = palette[8]; //ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive] = palette[3]; //ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = palette[0];
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = palette[1];
    style->Colors[ImGuiCol_ScrollbarGrabActive] = palette[2];
    style->Colors[ImGuiCol_CheckMark] = palette[0];
    style->Colors[ImGuiCol_SliderGrab] = palette[0];
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = palette[2];
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = palette[0];
    style->Colors[ImGuiCol_ResizeGripHovered] = palette[1];
    style->Colors[ImGuiCol_ResizeGripActive] = palette[2];
    style->Colors[ImGuiCol_CloseButton] = palette[0];//ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
    style->Colors[ImGuiCol_CloseButtonHovered] = palette[5];//ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
    style->Colors[ImGuiCol_CloseButtonActive] = palette[7];//ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);


    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    m_font = io.Fonts->AddFontFromMemoryCompressedTTF(cafe_compressed_data, cafe_compressed_size, 18.0f);
    ImGui::SFML::UpdateFontTexture();



}

void GUIManager::shut_down() {


}

void GUIManager::draw_corner_overlay_debug_info() {
    bool open = true;
    const float DISTANCE = 10.0f;
    static int corner = 2;
    auto window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
    auto window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f)); // Transparent background
    if (ImGui::Begin("Debug Overlay", &open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
    {
        auto mouse_pos = ImGui::GetIO().MousePos;

        auto real_mouse_pos = RenderManager::get().get_main_render_target()->mapPixelToCoords(sf::Vector2i(
            gsl::narrow_cast<int>(mouse_pos.x),
            gsl::narrow_cast<int>(mouse_pos.y)));

        ImGui::Text("Mouse Position: (%6.1f,%6.1f)", real_mouse_pos.x, real_mouse_pos.y);
        ImGui::Text("Frames Per Second: (%.1f)", (1.f / TimeManager::get().get_delta_time().asSeconds()));
        ImGui::Text("Frame Time: (%d) ms", (TimeManager::get().get_delta_time().asMilliseconds()));
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            ImGui::EndPopup();
        }
        ImGui::End();
    }
    ImGui::PopStyleColor();
}

void GUIManager::set_debug_open(bool open) {
    m_debug_open = open;
}

bool GUIManager::is_debug_open() const {
    return m_debug_open;
}

void GUIManager::draw_gui() {

    if (m_debug_open) {

        ImGui::PushFont(m_font);

        ImGui::BeginMainMenuBar();

        if (ImGui::BeginMenu("File")) {

            if (ImGui::Button("New Project From Spritesheet")) {

                auto filename = OSManager::get().user_open_file();

                if (filename.size() > 0)
                    CLOG(filename);

                //@@TODO: Check here for the file type.

                auto spritesheet = Spritesheet(filename);

                CLOG_NAMED(spritesheet.get_cols());
                CLOG_NAMED(spritesheet.get_rows());
                CLOG_NAMED(spritesheet.get_sprite_height());
                CLOG_NAMED(spritesheet.get_sprite_width());
                CLOG_NAMED(spritesheet.get_sprites().size());

                //@@DOING: Loading sprites read into somewhere?

            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            ImGui::Checkbox("Debug Overlay", &m_show_debug_overlay);
            ImGui::Checkbox("ImGui Demo", &m_show_imgui_demo);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();

        if (m_show_debug_overlay) draw_corner_overlay_debug_info();
        if (m_show_imgui_demo) ImGui::ShowDemoWindow();

        ImGui::PopFont();
    }
}

