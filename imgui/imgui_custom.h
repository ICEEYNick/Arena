#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include <map>
#include "imgui_tricks.hpp"

namespace ImGui {
    bool tab(const char* label, bool selected);
    bool subtab(const char* label, bool selected);
    bool toggle(const char* label, bool *v);
    void beginchild(const char* label, ImVec2 size);
    void endchild();
}
