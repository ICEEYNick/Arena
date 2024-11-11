#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <algorithm>
#include <xmmintrin.h>
#include <emmintrin.h>

#include <dwmapi.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mutex>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <random>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <algorithm>
#include <tchar.h>
#include <tuple>
#include <string>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3d11.h>
#include <dxgi.h>
#include <Dwmapi.h>

using namespace std;
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_custom.h"

#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9.lib")
namespace globals
{
    static int32_t ProcID;
    int64_t imagebase;
    int32_t UsermodepID;
    int64_t GameAssembly;
    int64_t UnityPlayer;

    int width = GetSystemMetrics(SM_CXSCREEN), height = GetSystemMetrics(SM_CYSCREEN);
    uint64_t BaseGameModeList;

    int Tab;
    bool menuVisible;
    HWND MyWnd;
}

INT32 find_process(LPCTSTR process_name) {
    PROCESSENTRY32 pt;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hsnap, &pt)) {
        do {
            if (!lstrcmpi(pt.szExeFile, process_name)) {
                CloseHandle(hsnap);
                globals::ProcID = pt.th32ProcessID;
                return pt.th32ProcessID;
            }
        } while (Process32Next(hsnap, &pt));
    }
    CloseHandle(hsnap);

    return { NULL };
}

using namespace std;

#include "xor.h"
#include "print.h"
#include "vector.h"
#include "vars.h"
#include "driver.hpp"
#include "renderer.h"
#include "configs.h"
#include "defs.h"
#include "function.h"
#include "font.h"
#include "overlay.h"
