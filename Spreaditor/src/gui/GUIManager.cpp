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

    // We are not using this font right now
    /*
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    m_font = io.Fonts->AddFontFromMemoryCompressedTTF(cafe_compressed_data, cafe_compressed_size, 18.0f);
    ImGui::SFML::UpdateFontTexture();
    */

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
            ImGui::Separator();
            ImGui::Checkbox("Style Editor", &m_show_style_editor);
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
        if (m_show_style_editor) draw_style_editor();


        ImGui::PopFont();
    }
}

void GUIManager::draw_corner_overlay_debug_info() {
    bool open = true;
    const float DISTANCE = 10.0f;
    static int corner = 1;
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


        ImGui::Text("Timeline");
        //ImGui::Separator();

        ImGui::BeginChild("##TimelineScrollingRegion", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * TIMELINE_SIZE), false, ImGuiWindowFlags_HorizontalScrollbar);
        {
            auto sprites = SpriteManager::get().get_sprites();
            auto sprite_count = sprites.size();
            auto timeline_sprite_height = ImGui::GetTextLineHeightWithSpacing() * (TIMELINE_SIZE - TIMELINE_HEADER_SIZE) - ImGui::GetStyle().ScrollbarSize;
            if (sprite_count > 0) {

                draw_timeline_sprite(sprites[0], 0, timeline_sprite_height);

                for (int i = 1; i < sprite_count; i++)
                {
                    ImGui::SameLine();
                    draw_timeline_sprite(sprites[i], i, timeline_sprite_height);

                }
            }
        }
        ImGui::EndChild();

        auto window_height = ImGui::GetCursorPos().y;
        ImGui::SetWindowPos(ImVec2(0, screen_size.y - window_height), true);
    }
    ImGui::End();

}

void GUIManager::draw_timeline_sprite(const sf::Sprite& sprite, int sprite_index, float height) {

    auto temp_sprite = sprite;
    temp_sprite.setScale(
        (height) / static_cast<float>(temp_sprite.getTextureRect().height),
        (height) / static_cast<float>(temp_sprite.getTextureRect().height));
    temp_sprite.setPosition(sf::Vector2f(0.f, 0.f));
    auto sprite_id = std::string("sprite_");
    sprite_id += sprite_index;
    ImGui::PushID(sprite_id.c_str());
    if (ImGui::ImageButton(temp_sprite)) {

        SpriteManager::get().set_current_main_sprite_index(sprite_index);

    }
    ImGui::PopID();

}

void GUIManager::draw_style_editor(ImGuiStyle* ref) {


    // You can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it compares to an internally stored reference)
    ImGuiStyle& style = ImGui::GetStyle();
    static ImGuiStyle ref_saved_style;

    // Default to using internal storage as reference
    static bool init = true;
    if (init && ref == NULL)
        ref_saved_style = style;
    init = false;
    if (ref == NULL)
        ref = &ref_saved_style;

    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);

    if (ImGui::ShowStyleSelector("Colors##Selector"))
        ref_saved_style = style;
    ImGui::ShowFontSelector("Fonts##Selector");

    // Simplified Settings
    if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
        style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
    { bool window_border = (style.WindowBorderSize > 0.0f); if (ImGui::Checkbox("WindowBorder", &window_border)) style.WindowBorderSize = window_border ? 1.0f : 0.0f; }
    ImGui::SameLine();
    { bool frame_border = (style.FrameBorderSize > 0.0f); if (ImGui::Checkbox("FrameBorder", &frame_border)) style.FrameBorderSize = frame_border ? 1.0f : 0.0f; }
    ImGui::SameLine();
    { bool popup_border = (style.PopupBorderSize > 0.0f); if (ImGui::Checkbox("PopupBorder", &popup_border)) style.PopupBorderSize = popup_border ? 1.0f : 0.0f; }

    // Save/Revert button
    if (ImGui::Button("Save Local"))
        *ref = ref_saved_style = style;
    ImGui::SameLine();
    if (ImGui::Button("Revert Local"))
        style = *ref;
    ImGui::SameLine();
    show_help_marker("This saves the local style for the current execution of the program.\n\nThe style saved with this won't be maintained between executions "
        "so if you close the program and open it again the style saved with this options will be lost.\n\n"
        "This is meant to save temporarily a style that you like and keep changing it while giving you the option to revert to the saved one quickly if you so desire");

    // Save To File
    if (ImGui::Button("Save To File")) {
    
        auto json_style = style_to_json();

        LOG(std::setw(4) << json_style);

    }
    ImGui::SameLine();
    show_help_marker("This saves the local style to a file that will be used when opening the program so the style can be maintained when opening"
    " and closing the program again.");

    if (ImGui::TreeNode("Settings"))
    {
        ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
        ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 16.0f, "%.0f");
        ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
        ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
        ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
        ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
        ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
        ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
        ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
        ImGui::Text("BorderSize");
        ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
        ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
        ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
        ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
        ImGui::Text("Rounding");
        ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 14.0f, "%.0f");
        ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 16.0f, "%.0f");
        ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
        ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
        ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
        ImGui::Text("Alignment");
        ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f"); ImGui::SameLine(); show_help_marker("Alignment applies when a button is larger than its text content.");
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Colors"))
    {
        static int output_dest = 0;
        static bool output_only_modified = true;
        if (ImGui::Button("Export Unsaved"))
        {
            if (output_dest == 0)
                ImGui::LogToClipboard();
            else
                ImGui::LogToTTY();
            ImGui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;" IM_NEWLINE);
            for (int i = 0; i < ImGuiCol_COUNT; i++)
            {
                const ImVec4& col = style.Colors[i];
                const char* name = ImGui::GetStyleColorName(i);
                if (!output_only_modified || memcmp(&col, &ref->Colors[i], sizeof(ImVec4)) != 0)
                    ImGui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);" IM_NEWLINE, name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
            }
            ImGui::LogFinish();
        }
        ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0"); ImGui::PopItemWidth();
        ImGui::SameLine(); ImGui::Checkbox("Only Modified Colors", &output_only_modified);

        ImGui::Text("Tip: Left-click on colored square to open color picker,\nRight-click to open edit options menu.");

        static ImGuiTextFilter filter;
        filter.Draw("Filter colors", 200);

        static ImGuiColorEditFlags alpha_flags = 0;
        ImGui::RadioButton("Opaque", &alpha_flags, 0); ImGui::SameLine();
        ImGui::RadioButton("Alpha", &alpha_flags, ImGuiColorEditFlags_AlphaPreview); ImGui::SameLine();
        ImGui::RadioButton("Both", &alpha_flags, ImGuiColorEditFlags_AlphaPreviewHalf);

        ImGui::BeginChild("#colors", ImVec2(0, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);
        ImGui::PushItemWidth(-160);
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            const char* name = ImGui::GetStyleColorName(i);
            if (!filter.PassFilter(name))
                continue;
            ImGui::PushID(i);
            ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
            if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
            {
                // Tips: in a real user application, you may want to merge and use an icon font into the main font, so instead of "Save"/"Revert" you'd use icons.
                // Read the FAQ and extra_fonts/README.txt about using icon fonts. It's really easy and super convenient!
                ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Save")) ref->Colors[i] = style.Colors[i];
                ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Revert")) style.Colors[i] = ref->Colors[i];
            }
            ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
            ImGui::TextUnformatted(name);
            ImGui::PopID();
        }
        ImGui::PopItemWidth();
        ImGui::EndChild();

        ImGui::TreePop();
    }

    ImGui::PopItemWidth();
}

// All variables to save
/*

WindowPadding
WindowRounding
FramePadding
FrameRounding
ItemSpacing
ItemInnerSpacing
IndentSpacing
ScrollbarSize
ScrollbarRounding
GrabMinSize
GrabMinSize
GrabRounding
Colors[ImGuiCol_Text]
Colors[ImGuiCol_TextDisabled]
Colors[ImGuiCol_WindowBg]
Colors[ImGuiCol_ChildWindowBg]
Colors[ImGuiCol_PopupBg]
Colors[ImGuiCol_Border]
Colors[ImGuiCol_BorderShadow]
Colors[ImGuiCol_FrameBg]
Colors[ImGuiCol_FrameBgHovered]
Colors[ImGuiCol_FrameBgActive]
Colors[ImGuiCol_TitleBg]
Colors[ImGuiCol_TitleBgCollapsed]
Colors[ImGuiCol_TitleBgActive]
Colors[ImGuiCol_MenuBarBg]
Colors[ImGuiCol_ScrollbarBg]
Colors[ImGuiCol_ScrollbarGrab]
Colors[ImGuiCol_ScrollbarGrabHovered]
Colors[ImGuiCol_ScrollbarGrabActive]
Colors[ImGuiCol_CheckMark]
Colors[ImGuiCol_SliderGrab]
Colors[ImGuiCol_SliderGrabActive]
Colors[ImGuiCol_Button]
Colors[ImGuiCol_ButtonHovered]
Colors[ImGuiCol_ButtonActive]
Colors[ImGuiCol_Header]
Colors[ImGuiCol_HeaderHovered]
Colors[ImGuiCol_HeaderActive]
Colors[ImGuiCol_Column]
Colors[ImGuiCol_ColumnHovered]
Colors[ImGuiCol_ColumnActive]
Colors[ImGuiCol_ResizeGrip]
Colors[ImGuiCol_ResizeGripHovered]
Colors[ImGuiCol_ResizeGripActive]
Colors[ImGuiCol_CloseButton]
Colors[ImGuiCol_CloseButtonHovered]
Colors[ImGuiCol_CloseButtonActive]
Colors[ImGuiCol_PlotLines]
Colors[ImGuiCol_PlotLinesHovered]
Colors[ImGuiCol_PlotHistogram]
Colors[ImGuiCol_PlotHistogramHovered]
Colors[ImGuiCol_TextSelectedBg]
Colors[ImGuiCol_ModalWindowDarkening]

*/

#define SAVE_STYLE_VAR(x) data[#x] = style.##x
#define LOAD_STYLE_VAR(x) style.##x = data[#x]

GUIManager::json GUIManager::style_to_json() {
    auto data = json{};
    auto style = ImGui::GetStyle();

    {
        SAVE_STYLE_VAR(WindowPadding);
        SAVE_STYLE_VAR(WindowRounding);
        SAVE_STYLE_VAR(FramePadding);
        SAVE_STYLE_VAR(FrameRounding);
        SAVE_STYLE_VAR(ItemSpacing);
        SAVE_STYLE_VAR(ItemInnerSpacing);
        SAVE_STYLE_VAR(IndentSpacing);
        SAVE_STYLE_VAR(ScrollbarSize);
        SAVE_STYLE_VAR(ScrollbarRounding);
        SAVE_STYLE_VAR(GrabMinSize);
        SAVE_STYLE_VAR(GrabMinSize);
        SAVE_STYLE_VAR(GrabRounding);
        SAVE_STYLE_VAR(Colors[ImGuiCol_Text]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_TextDisabled]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_WindowBg]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_ChildWindowBg]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_PopupBg]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_Border]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_BorderShadow]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_FrameBg]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_FrameBgHovered]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_FrameBgActive]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_TitleBg]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_TitleBgCollapsed]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_TitleBgActive]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_MenuBarBg]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_ScrollbarBg]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_ScrollbarGrab]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_ScrollbarGrabHovered]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_ScrollbarGrabActive]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_CheckMark]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_SliderGrab]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_SliderGrabActive]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_Button]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_ButtonHovered]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_ButtonActive]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_Header]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_HeaderHovered]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_HeaderActive]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_Column]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_ColumnHovered]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_ColumnActive]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_ResizeGrip]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_ResizeGripHovered]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_ResizeGripActive]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_CloseButton]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_CloseButtonHovered]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_CloseButtonActive]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_PlotLines]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_PlotLinesHovered]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_PlotHistogram]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_PlotHistogramHovered]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_TextSelectedBg]);
        SAVE_STYLE_VAR(Colors[ImGuiCol_ModalWindowDarkening]);
    }

    return data;
}

void GUIManager::style_from_json(json data) {

    auto style = ImGui::GetStyle();

    {
        LOAD_STYLE_VAR(WindowPadding);
        LOAD_STYLE_VAR(WindowRounding);
        LOAD_STYLE_VAR(FramePadding);
        LOAD_STYLE_VAR(FrameRounding);
        LOAD_STYLE_VAR(ItemSpacing);
        LOAD_STYLE_VAR(ItemInnerSpacing);
        LOAD_STYLE_VAR(IndentSpacing);
        LOAD_STYLE_VAR(ScrollbarSize);
        LOAD_STYLE_VAR(ScrollbarRounding);
        LOAD_STYLE_VAR(GrabMinSize);
        LOAD_STYLE_VAR(GrabMinSize);
        LOAD_STYLE_VAR(GrabRounding);
        LOAD_STYLE_VAR(Colors[ImGuiCol_Text]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_TextDisabled]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_WindowBg]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_ChildWindowBg]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_PopupBg]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_Border]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_BorderShadow]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_FrameBg]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_FrameBgHovered]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_FrameBgActive]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_TitleBg]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_TitleBgCollapsed]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_TitleBgActive]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_MenuBarBg]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_ScrollbarBg]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_ScrollbarGrab]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_ScrollbarGrabHovered]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_ScrollbarGrabActive]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_CheckMark]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_SliderGrab]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_SliderGrabActive]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_Button]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_ButtonHovered]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_ButtonActive]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_Header]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_HeaderHovered]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_HeaderActive]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_Column]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_ColumnHovered]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_ColumnActive]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_ResizeGrip]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_ResizeGripHovered]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_ResizeGripActive]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_CloseButton]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_CloseButtonHovered]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_CloseButtonActive]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_PlotLines]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_PlotLinesHovered]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_PlotHistogram]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_PlotHistogramHovered]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_TextSelectedBg]);
        LOAD_STYLE_VAR(Colors[ImGuiCol_ModalWindowDarkening]);
    }

}
