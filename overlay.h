#pragma once
#include <d3d9.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <windows.h>
#include <random>

bool render_menu = 0;
IDirect3D9Ex* p_Object = nullptr;
IDirect3DDevice9Ex* p_Device = nullptr;
D3DPRESENT_PARAMETERS p_Params = { 0 };

HWND GameWnd = 0, MyWnd = 0;
RECT GameRect = {};
HWND fortnite_window = { };
HWND window_handle;

ImFont* SkeetFont;

bool menu_key = true;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
static auto wnd_proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
        return true;

    switch (Message)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, Message, wParam, lParam);
    }
    return 0;
}

static auto setup_window() -> void {
    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        0,
        wnd_proc,
        0,
        0,
        nullptr,
        LoadIcon(nullptr, IDI_APPLICATION),
        LoadCursor(nullptr, IDC_ARROW),
        nullptr,
        nullptr,
        TEXT("Notepad"),
        LoadIcon(nullptr, IDI_APPLICATION)
    };

    int xOff = rand() % 15;
    int yOff = rand() % 15;

    GameWnd = FindWindowA(NULL, "Arena Breakout Infinite  ");

    RECT Rect;
    RegisterClassEx(&wc);
    MyWnd = CreateWindowEx(NULL, TEXT("Notepad"), TEXT("Untitled - Notepad"), NULL, 0, 0, globals::width + xOff, globals::height + yOff, NULL, NULL, 0, NULL);
    MARGINS margin = { -4, -2, globals::width + xOff, globals::height + yOff };
    DwmExtendFrameIntoClientArea(MyWnd, &margin);
    SetWindowLongPtr(MyWnd, GWLP_WNDPROC, (LONG_PTR)&wnd_proc);
    SetWindowLongPtr(MyWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
    SetWindowLongPtr(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
}

static auto setup_directx(HWND hWnd) -> void
{
    if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
        exit(3);

    ZeroMemory(&p_Params, sizeof(p_Params));
    p_Params.Windowed = TRUE;
    p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    p_Params.hDeviceWindow = hWnd;
    p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
    p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
    p_Params.BackBufferWidth = globals::width;
    p_Params.BackBufferHeight = globals::height;
    p_Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    p_Params.EnableAutoDepthStencil = TRUE;
    p_Params.AutoDepthStencilFormat = D3DFMT_D16;
    p_Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
    {
        p_Object->Release();
        exit(4);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsClassic();
    ImGuiStyle* style = &ImGui::GetStyle();

    // Set ImGui style colors
    style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    //style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border] = ImColor(40, 40, 40, 255);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImColor(150, 0, 0, 255);
    style->Colors[ImGuiCol_SliderGrab] = ImColor(40, 40, 40, 255);
    style->Colors[ImGuiCol_SliderGrabActive] = ImColor(60, 60, 60, 255);
    style->Colors[ImGuiCol_Button] = ImColor(40, 40, 40, 255);
    style->Colors[ImGuiCol_ButtonHovered] = ImColor(50, 50, 50, 255);
    style->Colors[ImGuiCol_ButtonActive] = ImColor(50, 50, 50, 255);
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    //style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    //style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    //style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

    ImFontConfig font_config;
    font_config.OversampleH = 1;
    font_config.OversampleV = 1;
    font_config.PixelSnapH = true;

    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF,
        0x0400, 0x044F,
        0,
    };

    Verdana = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 15.0f);
    SkeetFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(icon_compressed_data, icon_compressed_size, 15.f, &font_config, ranges);
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX9_Init(p_Device);

    p_Object->Release();
}

void menu() {
    static int MenuTab = 0;
    static int VisualTab = 0;

    static auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;

    ImGuiStyle* style = &ImGui::GetStyle();

    float
        TextSpaceLine = 90.f,
        SpaceLineOne = 120.f,
        SpaceLineTwo = 280.f,
        SpaceLineThr = 420.f;

    ImGui::Begin("Arena Breakout Infinite                                                     V1", NULL, flags);
    {
        ImGui::PushFont(Verdana);

        ImGui::SetWindowSize(ImVec2(400.f, 670.f), ImGuiCond_Once);
        {
            ImGui::BeginChild("##Tabs", ImVec2(-1, 30.0f));
            {
                style->ItemSpacing = ImVec2(5, 2);

                // Calculate the spacing between buttons
                float buttonWidth = 85.0f;
                float totalButtonsWidth = buttonWidth * 0;
                float spacing = (ImGui::GetContentRegionAvail().x - totalButtonsWidth) / 50; // 4 because there are 3 buttons and 4 gaps

                ImGui::SameLine(spacing); // Start with initial spacing

                if (ImGui::Button("Aimbot", ImVec2(buttonWidth, 20)))
                {
                    MenuTab = 0;
                }
                ImGui::SameLine(0, spacing); // Add spacing between buttons

                if (ImGui::Button("Visuals", ImVec2(buttonWidth, 20)))
                {
                    MenuTab = 1;
                }
                ImGui::SameLine(0, spacing); // Add spacing between buttons

                if (ImGui::Button("Misc", ImVec2(buttonWidth, 20)))
                {
                    MenuTab = 2;
                }
                ImGui::SameLine(0, spacing); // Add spacing between buttons

                if (ImGui::Button("Colors", ImVec2(buttonWidth, 20)))
                {
                    MenuTab = 3;
                }

                ImGui::EndChild();
            }

            // Add spacing between the tabs and the features
            ImGui::Spacing();

            if (MenuTab == 0)
            {
                ImGui::PushItemWidth(100);
                if (ImGui::BeginChild("Aimbot", ImVec2(385, 600), true))
                {
                    ImGui::Checkbox("Aimbot", &aimbot::aimbot);
                    ImGui::Spacing();
                    ImGui::Checkbox("Ignore Team", &aimbot::ignore_team);
                    ImGui::Spacing();
                    ImGui::Checkbox("Ignore AIScav", &aimbot::ignore_aiscavs);
                    ImGui::Spacing();
                    ImGui::Checkbox("Prediction", &aimbot::prediction);
                    ImGui::Spacing();
                    ImGui::Checkbox("Smooth", &aimbot::smooth);
                    ImGui::Spacing();
                    ImGui::Checkbox("FOV", &aimbot::fovcircle);
                    ImGui::Checkbox("Vis Check", &aimbot::vischeck); // Added for visual check toggle
                    ImGui::Spacing();
                    ImGui::Checkbox("Disable On Kill", &aimbot::disableondeath); // Added for disable on kill toggle
                    ImGui::Spacing();
                    ImGui::Checkbox("Show Crosshair", &aimbot::crosshair);
                    ImGui::Spacing();

                    ImGui::Text("FOV: ");
                    ImGui::PushItemWidth(200); // Set the width of the next item to 200 pixels
                    ImGui::SliderInt("##fovkrai", reinterpret_cast<int*>(&aimbot::aimfov), 0, 700, "%d");
                    ImGui::PopItemWidth(); // Restore the default item width
                    ImGui::Spacing();

                    ImGui::Text("Smooth: ");
                    ImGui::PushItemWidth(200); // Set the width of the next item to 200 pixels
                    ImGui::SliderInt("##smoothkrai", reinterpret_cast<int*>(&aimbot::aimspeed), 0, 100, "%d");
                    ImGui::PopItemWidth(); // Restore the default item width
                    ImGui::Spacing();

                    ImGui::Text("Aimbot Distance: ");
                    ImGui::PushItemWidth(200); // Set the width of the next item to 200 pixels
                    ImGui::SliderInt("##aimbotdistance", reinterpret_cast<int*>(&aimbot::aimdistance), 0, 1000, "%d");
                    ImGui::PopItemWidth(); // Restore the default item width
                    ImGui::Spacing();

                    ImGui::Text("Aim Key: ");
                    HotkeyButton(hotkeys::aimkey, ChangeKey, keystatus);

                    ImGui::PushItemWidth(100.f);
                    ImGui::Text("Hitbox: ");
                    ImGui::Combo("##BONES", &aimbot::hitbox, Hitbox, IM_ARRAYSIZE(Hitbox));
                    ImGui::Spacing();
                }
                ImGui::EndChild();
            }

            else if (MenuTab == 1)
            {
                if (ImGui::BeginChild("ESP", ImVec2(385, 600), true))
                {
                    ImGui::PushItemWidth(50.f);
                    ImGui::Text("Options:");
                    ImGui::Spacing();
                    ImGui::Checkbox("Show Box", &visuals::box);
                    ImGui::Spacing();
                    ImGui::Checkbox("Radar", &visuals::radar);
                    ImGui::Spacing();
                    ImGui::Checkbox("Loot Creates", &visuals::show_crates);
                    ImGui::Spacing();
                    ImGui::Checkbox("Show Toolbox", &visuals::itemtoolbox);
                    ImGui::Spacing();
                    ImGui::Checkbox("Show Skeleton", &visuals::skel);
                    ImGui::Spacing();
                    ImGui::Checkbox("Show Team", &visuals::team);
                    ImGui::Spacing();
                    ImGui::Checkbox("Show Lines", &visuals::lines);
                    ImGui::Spacing();
                    ImGui::Checkbox("Show Dist&Name", &visuals::name);
                    ImGui::Spacing();
                    // Character Type Checkboxes
                    ImGui::Text("Character Types:");
                    ImGui::Checkbox("None", &visuals::characterTypes[0]);
                    ImGui::Checkbox("PMC", &visuals::characterTypes[1]);
                    ImGui::Checkbox("SCAV", &visuals::characterTypes[2]);
                    ImGui::Checkbox("AI_SCAV", &visuals::characterTypes[3]);
                    ImGui::Checkbox("AI_SCAV_BOSS", &visuals::characterTypes[4]);
                    ImGui::Checkbox("AI_PMC", &visuals::characterTypes[5]);
                    ImGui::Checkbox("AI_ELIT", &visuals::characterTypes[6]);
                    ImGui::Checkbox("BOSS", &visuals::characterTypes[7]);
                    ImGui::Checkbox("AI_SCAV_Follower", &visuals::characterTypes[8]);
                    ImGui::Checkbox("MAX", &visuals::characterTypes[9]);

                    ImGui::Text("Esp Distance: ");
                    ImGui::PushItemWidth(200); // Set the width of the next item to 200 pixels
                    ImGui::SliderInt("##espdistance", reinterpret_cast<int*>(&visuals::MaxDistance), 1, 1000, "%d");
                    ImGui::PopItemWidth(); // Restore the default item width
                    ImGui::Spacing();

                    ImGui::Text("Skeleton Distance: ");
                    ImGui::PushItemWidth(200); // Set the width of the next item to 200 pixels
                    ImGui::SliderInt("##skeletondistance", reinterpret_cast<int*>(&visuals::MaxSkeletonDrawDistance), 1, 1000, "%d");
                    ImGui::PopItemWidth(); // Restore the default item width
                    ImGui::Spacing();

                    ImGui::PushItemWidth(100.f);
                    ImGui::Text("Box Style");
                    ImGui::Combo("##BOXSTYLES", &visuals::boxMode, boxStyle, IM_ARRAYSIZE(boxStyle));
                    ImGui::Spacing();

                    ImGui::PushItemWidth(100.f);
                    ImGui::Text("Lines Style");
                    ImGui::Combo("##LINESSTYLE", &visuals::lineMode, linesMode, IM_ARRAYSIZE(linesMode));
                    ImGui::Spacing();
                }
                ImGui::EndChild();
            }
            else if (MenuTab == 2)
            {
                if (ImGui::BeginChild("Misc", ImVec2(385, 600), true))
                {
                    ImGui::PushItemWidth(100.f);

                    ImGui::Checkbox("No Recoil", &misc::norecoil);
                    ImGui::Spacing();
                    ImGui::Checkbox("No spread", &misc::nospread);
                    ImGui::Spacing();
                    ImGui::Checkbox("No Sway", &misc::nosway);
                    ImGui::Spacing();

                    ImGui::Text("Radar Style:");
                    ImGui::Combo("##radar", &visuals::radarstyle, RadarStyle, IM_ARRAYSIZE(RadarStyle));
                    ImGui::Spacing();

                    ImGui::Text("Config: ");
                    ImGui::Combo("##settsas", &setting::settsMode, settsName, IM_ARRAYSIZE(settsName));
                    ImGui::Spacing();

                    if (ImGui::Button("Save", ImVec2(100, 20)))
                    {
                        if (setting::settsMode == 0)
                            Save_Settings("C:\\Default.ini");
                        if (setting::settsMode == 1)
                            Save_Settings("C:\\Default1.ini");
                        if (setting::settsMode == 2)
                            Save_Settings("C:\\Default2.ini");
                    }

                    if (ImGui::Button("Load", ImVec2(100, 20)))
                    {
                        if (setting::settsMode == 0)
                            Load_Settings("C:\\Default.ini");
                        if (setting::settsMode == 1)
                            Load_Settings("C:\\Default1.ini");
                        if (setting::settsMode == 2)
                            Load_Settings("C:\\Default2.ini");
                    }
                }
                ImGui::EndChild();
            }
            else if (MenuTab == 3)
            {
                if (ImGui::BeginChild("Colors", ImVec2(385, 600), true))
                {
                    ImGui::PushItemWidth(100.f);

                    ImGui::Text("FOV Color:");
                    ImGui::ColorEdit3("##fovcolor", colors::fov, ImGuiColorEditFlags_NoInputs);
                    ImGui::Spacing();

                    ImGui::Text("Crosshair Color:");
                    ImGui::ColorEdit3("##crosshaircolor", colors::crosshair, ImGuiColorEditFlags_NoInputs);
                    ImGui::Spacing();

                    ImGui::Text("Skeleton Color:");
                    ImGui::ColorEdit3("##skels", colors::skeleton, ImGuiColorEditFlags_NoInputs);
                    ImGui::Spacing();

                    ImGui::Text("Lines Color:");
                    ImGui::ColorEdit3("##lines", colors::lines, ImGuiColorEditFlags_NoInputs);
                    ImGui::Spacing();
                }
                ImGui::EndChild();
            }
        }
        ImGui::PopFont();
        ImGui::End();
    }
}

static auto shortcurts() -> void
{
    if (GetAsyncKeyState(VK_HOME) || GetAsyncKeyState(VK_DELETE) || GetAsyncKeyState(VK_INSERT))
    {
        if (menu_key == false)
        {
            menu_key = true;
        }
        else if (menu_key == true)
        {
            menu_key = false;
        }
        Sleep(200);
    }
}

void DrawCrosshair(float x, float y, float size, ImVec4 color) {
    ImGui::GetOverlayDrawList()->AddLine(ImVec2(x - size, y), ImVec2(x + size, y), ImGui::ColorConvertFloat4ToU32(color), 1.0f);
    ImGui::GetOverlayDrawList()->AddLine(ImVec2(x, y - size), ImVec2(x, y + size), ImGui::ColorConvertFloat4ToU32(color), 1.0f);
}

void DrawCrosshairDot(float x, float y, float size, ImVec4 color) {
    ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(x, y), size, ImGui::ColorConvertFloat4ToU32(color));
}

void DrawCrosshairCircle(float x, float y, float radius, ImVec4 color) {
    ImGui::GetOverlayDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(color));
}

void DrawCrosshairWithCircle(float x, float y, float size, float radius, ImVec4 color) {
    DrawCrosshair(x, y, size, color);
    DrawCrosshairCircle(x, y, radius, color);
}

void DrawCrosshairWithDot(float x, float y, float size, float dotSize, ImVec4 color) {
    DrawCrosshair(x, y, size, color);
    DrawCrosshairDot(x, y, dotSize, color);
}

auto render() -> void {
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    RGBA Fov = { colors::fov[0] * 255, colors::fov[1] * 255, colors::fov[2] * 255, 255 };
    if (aimbot::fovcircle)
    {
        DrawCircle(ImGui::GetIO().DisplaySize.x / 2.f, ImGui::GetIO().DisplaySize.y / 2.f, aimbot::aimfov, &Fov, 100);
    }

    if (aimbot::crosshair) {
        ImVec4 crosshairColor = ImVec4(colors::crosshair[0], colors::crosshair[1], colors::crosshair[2], colors::crosshair[3]);
        float crosshairSize = 10.0f;
        float crosshairDotSize = 2.0f;
        float crosshairRadius = 50.0f;

        float x = ImGui::GetIO().DisplaySize.x / 2.0f;
        float y = ImGui::GetIO().DisplaySize.y / 2.0f;

        DrawCrosshairWithDot(x, y, crosshairSize, crosshairDotSize, crosshairColor);
    }

    shortcurts();
    if (menu_key)
    {
        menu();
    }
    esp();

    ImGui::EndFrame();
    p_Device->SetRenderState(D3DRS_ZENABLE, false);
    p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

    p_Device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
    if (p_Device->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        p_Device->EndScene();
    }

    if (p_Device->Present(0, 0, 0, 0) == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
    {
        ImGui_ImplDX9_InvalidateDeviceObjects();
        p_Device->Reset(&p_Params);
        ImGui_ImplDX9_CreateDeviceObjects();
    }
}

auto main_loop() -> WPARAM
{
    static RECT old_rc;
    static MSG Message;

    ZeroMemory(&Message, sizeof(MSG));

    while (Message.message != WM_QUIT)
    {
        if (PeekMessageA(&Message, MyWnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessageA(&Message);
        }

        HWND hwnd_active = GetForegroundWindow();

        if (hwnd_active == GameWnd) {
            HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
            SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }

        ImGuiIO& io = ImGui::GetIO();
        POINT p;
        GetCursorPos(&p);
        io.MousePos.x = p.x;
        io.MousePos.y = p.y;
        if (GetAsyncKeyState(VK_LBUTTON)) {
            io.MouseDown[0] = true;
            io.MouseClicked[0] = true;
            io.MouseClickedPos[0].x = io.MousePos.x;
            io.MouseClickedPos[0].y = io.MousePos.y;
        }
        else {
            io.MouseDown[0] = false;
        }

        render();
        Sleep(1);
    }

    return Message.wParam;
}

auto get_screen_status() -> bool
{
    if (fortnite_window == GetForegroundWindow()) {
        return true;
    }

    if (fortnite_window == GetActiveWindow()) {
        return true;
    }

    if (GetActiveWindow() == GetForegroundWindow()) {
        return true;
    }

    return false;
}

auto Draw() -> void {
    // Initialize ImGui
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Toggle menu
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        render_menu = !render_menu;
    }

    // Check if game window is active
    if (get_screen_status()) {
        // Draw ESP
        esp();

        // Draw menu if enabled
        if (render_menu) {
            menu();
        }

        // Draw FOV circle if enabled
        if (aimbot::fovcircle) {
            RGBA Fov = { colors::fov[0] * 255, colors::fov[1] * 255, colors::fov[2] * 255, 255 };
            DrawCircle(ImGui::GetIO().DisplaySize.x / 2.f, ImGui::GetIO().DisplaySize.y / 2.f, aimbot::aimfov, &Fov, 100);
        }
    }

    // Render ImGui
    ImGui::Render();

    // Clear screen
    p_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

    // Begin scene
    if (p_Device->BeginScene() >= 0) {
        // Render ImGui draw data
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        // End scene
        p_Device->EndScene();
    }

    // Present scene
    p_Device->Present(NULL, NULL, NULL, NULL);
}