#pragma once
namespace SetUp {

    inline HWND gWnd{}, DrawWnd{};

#define MAX_CLASSNAME 255
#define MAX_WNDNAME 255
#define TRANSPARENCY_COLOR RGB(0, 254, 0)

    struct WindowsFinderParams {
        DWORD pidOwner = NULL;
        string wndClassName = "";
        string wndName = "";
        RECT pos = { 0, 0, 0, 0 };
        POINT res = { 0, 0 };
        float percentAllScreens = 0.0f;
        float percentMainScreen = 0.0f;
        DWORD style = NULL;
        DWORD styleEx = NULL;
        bool satisfyAllCriteria = false;
        vector<HWND> hwnds;
    };

    std::vector<HWND> WindowsFinder(WindowsFinderParams params);
    BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam);
    HWND HiJackNotepadWindow();
    std::vector<DWORD> GetPIDs(wstring targetProcessName);

    HWND HiJackNotepadWindow()
    {
        HWND hwnd = NULL;
        vector<DWORD> notepads;
        if (notepads.empty())
        {
            //system(EX("taskkill /IM notepad.exe >nul 2>nul"));
            system(EX("start notepad"));
            notepads = GetPIDs(L"notepad.exe");
        }
        else {
            //system(EX("taskkill /IM Notepad.exe >nul 2>nul"));
            system(EX("start Notepad"));
            notepads = GetPIDs(L"Notepad.exe");
        }

        WindowsFinderParams params;
        params.pidOwner = notepads[0];
        params.style = WS_VISIBLE;
        params.satisfyAllCriteria = true;
        vector<HWND> hwnds;
        int attempt = 0;
        while (hwnd == NULL || attempt > 50) {
            Sleep(100);
            hwnds = WindowsFinder(params);
            if (hwnds.size() > 1)
                return hwnd;
            hwnd = hwnds[0];
            ++attempt;
        }
        if (!hwnd)
            return hwnd;
        SetMenu(hwnd, NULL);
        SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE);
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT); // WS_EX_NOACTIVATE  and WS_EX_TOOLWINDOW removes it from taskbar

        SetWindowPos(hwnd, NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
        globals::width = GetSystemMetrics(SM_CXSCREEN);
        globals::height = GetSystemMetrics(SM_CYSCREEN);
        return hwnd;
    }
    std::vector<DWORD> GetPIDs(wstring targetProcessName) {
        vector<DWORD> pids;
        if (targetProcessName == L"")
            return pids;
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32W entry;
        entry.dwSize = sizeof entry;
        if (!Process32FirstW(snap, &entry)) {
            CloseHandle(snap);
            return pids;
        }
        do {
            if (wstring(entry.szExeFile) == targetProcessName) {
                pids.emplace_back(entry.th32ProcessID);
            }
        } while (Process32NextW(snap, &entry));
        CloseHandle(snap);
        return pids;
    }
    BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam) {
        WindowsFinderParams& params = *(WindowsFinderParams*)lParam;

        unsigned char satisfiedCriteria = 0, unSatisfiedCriteria = 0;

        // If looking for windows of a specific PDI
        DWORD pid = 0;
        GetWindowThreadProcessId(hwnd, &pid);
        if (params.pidOwner != NULL)
            if (params.pidOwner == pid)
                ++satisfiedCriteria; // Doesn't belong to the process targeted
            else
                ++unSatisfiedCriteria;

        // If looking for windows of a specific class
        char className[MAX_CLASSNAME] = "";
        GetClassName(hwnd, (LPSTR)className, MAX_CLASSNAME);
        string classNameWstr = className;
        if (params.wndClassName != "")
            if (params.wndClassName == classNameWstr)
                ++satisfiedCriteria; // Not the class targeted
            else
                ++unSatisfiedCriteria;

        // If looking for windows with a specific name
        char windowName[MAX_WNDNAME] = "";
        GetWindowText(hwnd, (LPSTR)windowName, MAX_CLASSNAME);
        string windowNameWstr = windowName;
        if (params.wndName != "")
            if (params.wndName == windowNameWstr)
                ++satisfiedCriteria; // Not the class targeted
            else
                ++unSatisfiedCriteria;

        // If looking for window at a specific position
        RECT pos;
        GetWindowRect(hwnd, &pos);
        if (params.pos.left || params.pos.top || params.pos.right || params.pos.bottom)
            if (params.pos.left == pos.left && params.pos.top == pos.top && params.pos.right == pos.right && params.pos.bottom == pos.bottom)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // If looking for window of a specific size
        POINT res = { pos.right - pos.left, pos.bottom - pos.top };
        if (params.res.x || params.res.y)
            if (res.x == params.res.x && res.y == params.res.y)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // If looking for windows taking more than a specific percentage of all the screens
        float ratioAllScreensX = res.x / GetSystemMetrics(SM_CXSCREEN);
        float ratioAllScreensY = res.y / GetSystemMetrics(SM_CYSCREEN);
        float percentAllScreens = ratioAllScreensX * ratioAllScreensY * 100;
        if (params.percentAllScreens != 0.0f)
            if (percentAllScreens >= params.percentAllScreens)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // If looking for windows taking more than a specific percentage or the main screen
        RECT desktopRect;
        GetWindowRect(GetDesktopWindow(), &desktopRect);
        POINT desktopRes = { desktopRect.right - desktopRect.left, desktopRect.bottom - desktopRect.top };
        float ratioMainScreenX = res.x / desktopRes.x;
        float ratioMainScreenY = res.y / desktopRes.y;
        float percentMainScreen = ratioMainScreenX * ratioMainScreenY * 100;
        if (params.percentMainScreen != 0.0f)
            if (percentAllScreens >= params.percentMainScreen)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // Looking for windows with specific styles
        LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
        if (params.style)
            if (params.style & style)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // Looking for windows with specific extended styles
        LONG_PTR styleEx = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
        if (params.styleEx)
            if (params.styleEx & styleEx)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        if (!satisfiedCriteria)
            return TRUE;

        if (params.satisfyAllCriteria && unSatisfiedCriteria)
            return TRUE;

        // If looking for multiple windows
        params.hwnds.push_back(hwnd);
        return TRUE;
    }
    std::vector<HWND> WindowsFinder(WindowsFinderParams params) {
        EnumWindows(EnumWindowsCallback, (LPARAM)&params);
        return params.hwnds;
    }
}