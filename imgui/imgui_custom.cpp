#include "imgui_custom.h"

static std::map<ImGuiID, float> tab_alpha;
static std::map<ImGuiID, float> subtab_alpha;
static std::map<ImGuiID, ImVec4> toggle;

bool ImGui::tab(const char* label, bool selected) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    
    ImVec2 size = CalcItemSize(label_size, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, { pos.x + size.x, pos.y + size.y });
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    float* alpha = &tab_alpha[id];

    if (*alpha >= 1.f) {
        *alpha = 1.f;
    }

    if (*alpha <= 0.3f) {
        *alpha = 0.3f;
    }

    if (selected) {
        if (*alpha < 1.f)
            *alpha += ImGui::GetIO().DeltaTime * 6;
    }
    else {
        if (*alpha > 0.3f)
            *alpha -= ImGui::GetIO().DeltaTime * 6;
    }

    PushStyleColor(ImGuiCol_Text, ImVec4(195 / 255.f, 196 / 255.f, 200 / 255.f, *alpha));
    RenderText(bb.Min, label);
    PopStyleColor();

    return pressed;
}

bool ImGui::subtab(const char* label, bool selected) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;

    ImVec2 size = CalcItemSize(label_size, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, { pos.x + size.x, pos.y + size.y });
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    float* alpha = &tab_alpha[id];
    float* lineAlpha = &subtab_alpha[id];

    if (*alpha >= 1.f) {
        *alpha = 1.f;
    }

    if (*alpha <= 0.3f) {
        *alpha = 0.3f;
    }

    if (selected) {
        if (*alpha < 1.f)
            *alpha += ImGui::GetIO().DeltaTime * 6;
    }
    else {
        if (*alpha > 0.3f)
            *alpha -= ImGui::GetIO().DeltaTime * 6;
    }

    if (*lineAlpha >= 1.f) {
        *lineAlpha = 1.f;
    }

    if (*lineAlpha <= 0.0f) {
        *lineAlpha = 0.0f;
    }

    if (selected) {
        if (*lineAlpha < 1.f)
            *lineAlpha += ImGui::GetIO().DeltaTime * 6;
    }
    else {
        if (*lineAlpha > 0.0f)
            *lineAlpha -= ImGui::GetIO().DeltaTime * 6;
    }

    window->DrawList->AddLine({ bb.Min.x - 16, bb.Max.y + 7 }, { bb.Max.x + 16, bb.Max.y + 7 }, ImColor(40 / 255.f, 41 / 255.f, 45 / 255.f, *lineAlpha), 2);

    PushStyleColor(ImGuiCol_Text, ImVec4(195 / 255.f, 196 / 255.f, 200 / 255.f, *alpha));
    RenderText(bb.Min, label);
    PopStyleColor();

    return pressed;
}

bool ImGui::toggle(const char* label, bool* v) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, { pos.x + window->Size.x - g.Style.WindowPadding.x * 2, pos.y + label_size.y });
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
    {
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    float active = ImTricks::Animations::FastFloatLerp("toggleActive", *v, 0.f, 1.f, GetIO().DeltaTime*6);
    float grabAnim = ImTricks::Animations::FastFloatLerp("grabAnim", *v, 0.f, 1.f, GetIO().DeltaTime * 6);

    window->DrawList->AddRectFilled({ total_bb.Max.x - 26, total_bb.Min.y }, { total_bb.Max.x, total_bb.Max.y }, ImTricks::Animations::FastColorLerp(ImColor(52, 53, 57), ImColor(77, 78, 82), active), 7);
    window->DrawList->AddCircleFilled({ total_bb.Max.x - 26 + 4 + 6.5f / 2 + grabAnim * 11, total_bb.Min.y + 15 / 2 }, 3.5f, ImColor(1.f, 1.f, 1.f, 0.2f));

    ImVec4 col = ImTricks::Animations::FastColorLerp(GetColorU32(ImGuiCol_Text, 0.3f), GetColorU32(ImGuiCol_Text, 1.f), active);

    PushStyleColor(ImGuiCol_Text, col);
    RenderText(total_bb.Min, label);
    PopStyleColor();

    return pressed;
}

void ImGui::beginchild(const char* label, ImVec2 size)
{
    BeginChild(label, size, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    SetCursorPos(GImGui->Style.WindowPadding);
    BeginChild(std::string(label).append("##1").c_str(), { size.x - GetCurrentWindow()->WindowPadding.x, size.y - GetCurrentWindow()->WindowPadding.y }, false, ImGuiWindowFlags_NoBackground);
}

void ImGui::endchild()
{
    EndChild();
    EndChild();
}
