#include <imconfig.h>
#include <imgui.h>
#include <imgui_internal.h>

static float scaling_factor = 1.0f;

using namespace ImGui;

static ImVec2 CalcSizeAfterConstraint(ImGuiWindow* window, ImVec2 new_size)
{
    ImGuiContext& g = *GImGui;
    if (g.SetNextWindowSizeConstraint)
    {
        // Using -1,-1 on either X/Y axis to preserve the current size.
        ImRect cr = g.SetNextWindowSizeConstraintRect;
        new_size.x = (cr.Min.x >= 0 && cr.Max.x >= 0) ? ImClamp(new_size.x, cr.Min.x, cr.Max.x) : window->SizeFull.x;
        new_size.y = (cr.Min.y >= 0 && cr.Max.y >= 0) ? ImClamp(new_size.y, cr.Min.y, cr.Max.y) : window->SizeFull.y;
        if (g.SetNextWindowSizeConstraintCallback)
        {
            ImGuiSizeConstraintCallbackData data;
            data.UserData = g.SetNextWindowSizeConstraintCallbackUserData;
            data.Pos = window->Pos;
            data.CurrentSize = window->SizeFull;
            data.DesiredSize = new_size;
            g.SetNextWindowSizeConstraintCallback(&data);
            new_size = data.DesiredSize;
        }
    }

    // Minimum size
    if (!(window->Flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_AlwaysAutoResize)))
    {
        new_size = ImMax(new_size, g.Style.WindowMinSize);
        new_size.y = ImMax(new_size.y, window->TitleBarHeight() + window->MenuBarHeight() + ImMax(0.0f, g.Style.WindowRounding - 1.0f)); // Reduce artifacts with very small windows
    }
    return new_size;
}

static ImRect GetBorderRect(ImGuiWindow* window, int border_n, float perp_padding, float thickness)
{
    ImRect rect = window->Rect();
    if (thickness == 0.0f) {
        rect.Max.x -= 1;
        rect.Max.y -= 1;
    };
    if (border_n == 0) return ImRect(rect.Min.x + perp_padding, rect.Min.y, rect.Max.x - perp_padding, rect.Min.y + thickness);
    if (border_n == 1) return ImRect(rect.Max.x - thickness, rect.Min.y + perp_padding, rect.Max.x, rect.Max.y - perp_padding);
    if (border_n == 2) return ImRect(rect.Min.x + perp_padding, rect.Max.y - thickness, rect.Max.x - perp_padding, rect.Max.y);
    if (border_n == 3) return ImRect(rect.Min.x, rect.Min.y + perp_padding, rect.Min.x + thickness, rect.Max.y - perp_padding);
    IM_ASSERT(0);
    return ImRect();
}

static void CalcResizePosSizeFromAnyCorner(ImGuiWindow* window, const ImVec2& corner_target, const ImVec2& corner_norm, ImVec2* out_pos, ImVec2* out_size)
{
    ImVec2 pos_min = ImLerp(corner_target, window->Pos, corner_norm);                // Expected window upper-left
    ImVec2 pos_max = ImLerp(window->Pos + window->Size, corner_target, corner_norm); // Expected window lower-right
    ImVec2 size_expected = ImVec2(pos_max.x - pos_min.x, pos_max.y - pos_min.y);
    ImVec2 size_constrained = CalcSizeAfterConstraint(window, size_expected);
    *out_pos = pos_min;
    if (corner_norm.x == 0.0f)
        out_pos->x -= (size_constrained.x - size_expected.x);
    if (corner_norm.y == 0.0f)
        out_pos->y -= (size_constrained.y - size_expected.y);
    *out_size = size_constrained;
}

void ImGui::SetSamelineScaling(float scaling_factor) {
    scaling_factor = scaling_factor;
}

void ImGui::SameLineS(float pos_x, float spacing_w) {
    ImGui::SameLine(pos_x * scaling_factor, spacing_w * scaling_factor);
}

/*

@@UNTESTED

*/
void ImGui::ResizeableBorder(int border_n) {

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float grip_draw_size = (float)(int)ImMax(g.FontSize * 1.35f, style.WindowRounding + 1.0f + g.FontSize * 0.2f);
    const float grip_hover_size = (float)(int)(grip_draw_size * 0.75f);

    auto border_held = -1;

    ImVec2 pos_target(FLT_MAX, FLT_MAX);
    ImVec2 size_target(FLT_MAX, FLT_MAX);

    // Manual resize grips
    PushID("#RESIZE");

    auto window = ImGui::GetCurrentWindow();

    {
        const float BORDER_SIZE = 5.0f;          // FIXME: Only works _inside_ window because of HoveredWindow check.
        const float BORDER_APPEAR_TIMER = 0.05f; // Reduce visual noise
        bool hovered, held;
        ImRect border_rect = GetBorderRect(window, border_n, grip_hover_size, BORDER_SIZE);
        ButtonBehavior(border_rect, window->GetID((void*)(intptr_t)(border_n + 4)), &hovered, &held, ImGuiButtonFlags_FlattenChildren);
        if ((hovered && g.HoveredIdTimer > BORDER_APPEAR_TIMER) || held)
        {
            g.MouseCursor = (border_n & 1) ? ImGuiMouseCursor_ResizeEW : ImGuiMouseCursor_ResizeNS;
            if (held) border_held = border_n;
        }
        if (held)
        {
            ImVec2 border_target = window->Pos;
            ImVec2 border_posn;
            if (border_n == 0) { border_posn = ImVec2(0, 0); border_target.y = (g.IO.MousePos.y - g.ActiveIdClickOffset.y); }
            if (border_n == 1) { border_posn = ImVec2(1, 0); border_target.x = (g.IO.MousePos.x - g.ActiveIdClickOffset.x + BORDER_SIZE); }
            if (border_n == 2) { border_posn = ImVec2(0, 1); border_target.y = (g.IO.MousePos.y - g.ActiveIdClickOffset.y + BORDER_SIZE); }
            if (border_n == 3) { border_posn = ImVec2(0, 0); border_target.x = (g.IO.MousePos.x - g.ActiveIdClickOffset.x); }
            CalcResizePosSizeFromAnyCorner(window, border_target, border_posn, &pos_target, &size_target);
        }
    }
    PopID();

    // Apply back modified position/size to window
    if (size_target.x != FLT_MAX)
    {
        window->SizeFull = size_target;
    }
    if (pos_target.x != FLT_MAX)
    {
        window->Pos = window->PosFloat = ImVec2((float)(int)pos_target.x, (float)(int)pos_target.y);
    }

    window->Size = window->SizeFull;

    //title_bar_rect = window->TitleBarRect();

    if (border_held != -1){
        ImRect border = GetBorderRect(window, border_held, grip_draw_size, 0.0f);
        window->DrawList->AddLine(border.Min, border.Max, GetColorU32(ImGuiCol_SeparatorActive), ImMax(1.0f, window->WindowBorderSize));
    }

}
