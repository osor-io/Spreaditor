#include "GUIManager.h"
#include "../time/TimeManager.h"
#include <Debug.h>
#include <../config/Config.h>
#include "../debug/cafe.inl"
#include "../utils/File.h"
#include "../render/RenderManager.h"
#include "gsl\gsl"
#include "../os/OSManager.h"
#include "GUIUtils.h"
#include "../sprites/SpriteManager.h"
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

void GUIManager::set_debug_open(bool open) {
    m_debug_open = open;
}

bool GUIManager::is_debug_open() const {
    return m_debug_open;
}

void GUIManager::do_gui() {

    if (m_debug_open) {

        ImGui::PushFont(m_font);

        ImGui::BeginMainMenuBar();

        if (ImGui::BeginMenu("File")) {

            BEGIN_MENU_POPUP_MODAL("New Project From Spritesheet");
            {

                static auto filename = std::string{};

                static char loading_filename[MAX_OS_FILENAME_SIZE];

                ImGui::Text("Spritesheet file: ");
                ImGui::SameLine(150);
                ImGui::InputText("##Filename", loading_filename, MAX_OS_FILENAME_SIZE);
                ImGui::SameLine();
                if (ImGui::Button("Explore", ImVec2(120, 0))) {
                    filename = OSManager::get().user_open_file(\
                        "(*.png) Portable Network Graphics\0*.png\0"
                        "(*.bmp) Windows bitmap\0*.bmp\0"
                        "(*.jpg) Joint Photographic Experts Group\0*.jpg\0"

                    );
                    assert(filename.size() < MAX_OS_FILENAME_SIZE);
                    filename.copy(loading_filename, filename.size());
                    loading_filename[filename.size()] = '\0';
                }

                ImGui::TextWrapped("This will close the current project without keeping unsaved changes.");


                IF_BUTTON_ALIGNED_RIGHT_FIRST("Cancel", ImVec2(120, 0))
                {
                    ImGui::CloseCurrentPopup();
                }
                END_BUTTON_ALIGNED_RIGHT_FIRST;

                IF_BUTTON_ALIGNED_RIGHT_NEXT("Accept", ImVec2(120, 0), accept) {

                    filename.clear();
                    filename.append(loading_filename);

                    CLOG("Loading spritesheet from " << filename);

                    auto cfilename = filename.c_str();

                    if (!file_exists(cfilename)) {
                        CLOG_ERROR("Incorrect File Path");
                        ImGui::OpenPopup("Incorrect File Path");
                    }
                    else {

                        auto loaded = SpriteManager::get().load_spritesheet(cfilename);

                        auto spritesheet = SpriteManager::get().get_spritesheet();

                        if (!loaded || !spritesheet) {
                            CLOG_ERROR("Incorrect Spritesheet");
                            ImGui::OpenPopup("Incorrect Spritesheet");
                        }
                        else {

                            /*
                            @@TODO

                            Check why these are getting logged into the file but yes into the console
                            */
                            CLOG("\tSpritesheet rows: " << spritesheet->get_rows());
                            CLOG("\tSpritesheet columns: " << spritesheet->get_cols());
                            CLOG("\tSprite width: " << spritesheet->get_sprite_width());
                            CLOG("\tSprite height: " << spritesheet->get_sprite_height());
                            CLOG("\tAmount of sprites read: " << spritesheet->get_sprites().size());

                            CLOG("Correctly loaded spritesheet from " << filename);
                            ImGui::CloseCurrentPopup();
                        }
                    }
                }
                END_BUTTON_ALIGNED_RIGHT_NEXT(accept);

                if (ImGui::BeginPopupModal("Incorrect File Path"))
                {
                    ImGui::Text("The spritesheet file you are trying to open doesn't exist:\nPath: \"%s\"", filename.c_str());

                    IF_BUTTON_ALIGNED_RIGHT_FIRST("Close", ImVec2(120, 0))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                    END_BUTTON_ALIGNED_RIGHT_FIRST;

                    ImGui::EndPopup();
                }

                if (ImGui::BeginPopupModal("Incorrect Spritesheet"))
                {
                    ImGui::Text("The spritesheet file could not be parsed correctly:\nPath: \"%s\"", filename.c_str());

                    IF_BUTTON_ALIGNED_RIGHT_FIRST("Close", ImVec2(120, 0))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                    END_BUTTON_ALIGNED_RIGHT_FIRST;

                    ImGui::EndPopup();
                }

            }
            END_MENU_POPUP_MODAL;




            // ====== END OF FILE MENU ======
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            ImGui::Checkbox("Timeline", &m_show_timeline);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug")) {
            ImGui::Checkbox("Debug Overlay", &m_show_debug_overlay);
            ImGui::Checkbox("ImGui Demo", &m_show_imgui_demo);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();

        if (m_show_debug_overlay) draw_corner_overlay_debug_info();
        if (m_show_imgui_demo) ImGui::ShowDemoWindow();
        if (m_show_timeline) draw_timeline();

        ImGui::PopFont();
    }
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
    }
    ImGui::End();
    ImGui::PopStyleColor();
}

void GUIManager::draw_timeline() {
    auto screen_size = RenderManager::get().get_main_render_target()->getSize();
    ImGui::SetNextWindowSize(ImVec2(screen_size.x, 0));
    if (ImGui::Begin("Timeline", nullptr,
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoTitleBar
    )) {


        ImGui::Text("Testing the timeline a;sdlkfja;lsdkfja;sldfjkal;sdkfja;sldkfja;lsdkfj;asldjkfal;sdkjf;alsdkfja;lsdjkfa;lsdkjf\n"
            "a; lsdkjf; asldkfja; lsdkfjal; sdkdfjkl;geriou;dfkls;alsdfkuiasdfgk;aklsdvj;adsklfasdf;klds;fjkllaksdhjhfsdfja; ");

        /*
        @@DOING
        
        Trying to render horizontal columns for the sprites
        
        Uncomment the next region.
        */
        /*
        ImGui::BeginChild("##ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        auto sprites = SpriteManager::get().get_sprites();
        auto sprite_count = sprites.size();
        ImGui::Columns(7);
        ImGuiListClipper clipper(sprite_count);
        while (clipper.Step()) {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
                ImGui::Text("Sprite Number: %d", i);
                ImGui::Text("Sprite Number: %d", i);
                ImGui::Text("Sprite Number: %d", i);
                ImGui::Text("Sprite Number: %d", i);
                ImGui::NextColumn();
            }
        }
        ImGui::Columns(1);
        auto child_height = clipper.ItemsHeight;
        ImGui::EndChild();
        */

        auto window_height = ImGui::GetCursorPos().y /*+ 40 /*?*/;
        ImGui::SetWindowPos(ImVec2(0, screen_size.y - window_height), true);
    }
    ImGui::End();

}
