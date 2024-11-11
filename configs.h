#pragma once

static BOOL WritePrivateProfileInt(LPCSTR lpAppName, LPCSTR lpKeyName, int nInteger, LPCSTR lpFileName) {
    char lpString[1024];
    sprintf(lpString, EX("%d"), nInteger);
    return WritePrivateProfileStringA(lpAppName, lpKeyName, lpString, lpFileName);
}
static BOOL WritePrivateProfileFloat(LPCSTR lpAppName, LPCSTR lpKeyName, float nInteger, LPCSTR lpFileName) {
    char lpString[1024];
    sprintf(lpString, EX("%f"), nInteger);
    return WritePrivateProfileStringA(lpAppName, lpKeyName, lpString, lpFileName);
}
static float GetPrivateProfileFloat(LPCSTR lpAppName, LPCSTR lpKeyName, FLOAT flDefault, LPCSTR lpFileName)
{
    char szData[32];

    GetPrivateProfileStringA(lpAppName, lpKeyName, std::to_string(flDefault).c_str(), szData, 32, lpFileName);

    return (float)atof(szData);
}

static void Save_Settings(LPCSTR path)
{
    WritePrivateProfileInt((EX("Visuals")), (EX("m_playeresp")), visuals::box, path);
    WritePrivateProfileInt((EX("Visuals")), (EX("m_boxstyle")), visuals::boxMode, path);
    WritePrivateProfileInt((EX("Visuals")), (EX("m_skeleton")), visuals::skel, path);
    WritePrivateProfileInt((EX("Visuals")), (EX("m_playername")), visuals::name, path);
    WritePrivateProfileInt((EX("Visuals")), (EX("m_snaplines")), visuals::lines, path);
    WritePrivateProfileInt((EX("Visuals")), (EX("m_linemode")), visuals::lineMode, path);
    WritePrivateProfileInt((EX("Visuals")), (EX("m_teamid")), visuals::team_check, path);
    WritePrivateProfileInt((EX("Visuals")), (EX("m_health")), visuals::health, path);
    WritePrivateProfileInt((EX("Visuals")), (EX("m_radar")), visuals::radar, path);
    WritePrivateProfileInt((EX("Visuals")), (EX("m_radarstyle")), visuals::radarstyle, path);

    WritePrivateProfileFloat((EX("Colors")), (EX("m_invir")), colors::invisible[0], path);
    WritePrivateProfileFloat((EX("Colors")), (EX("m_invig")), colors::invisible[1], path);
    WritePrivateProfileFloat((EX("Colors")), (EX("m_invib")), colors::invisible[2], path);
    WritePrivateProfileFloat((EX("Colors")), (EX("m_visibr")), colors::visible[0], path);
    WritePrivateProfileFloat((EX("Colors")), (EX("m_visibg")), colors::visible[1], path);
    WritePrivateProfileFloat((EX("Colors")), (EX("m_visibb")), colors::visible[2], path);

    WritePrivateProfileInt((EX("Aimbot")), (EX("m_aimbot")), aimbot::aimbot, path);
    WritePrivateProfileInt((EX("Aimbot")), (EX("m_smooth")), aimbot::smooth, path);
    WritePrivateProfileFloat((EX("Aimbot")), (EX("m_aimbotfov")), aimbot::aimfov, path);
    WritePrivateProfileFloat((EX("Aimbot")), (EX("m_aimbotsmooth")), aimbot::aimspeed, path);
    WritePrivateProfileInt((EX("Aimbot")), (EX("m_fovcircle")), aimbot::fovcircle, path);
    WritePrivateProfileInt((EX("Aimbot")), (EX("m_aimkey")), hotkeys::aimkey, path);
    WritePrivateProfileInt((EX("Aimbot")), (EX("m_hitbox")), aimbot::hitbox, path);
    WritePrivateProfileInt((EX("Distance")), (EX("m_aimbotdistance")), aimbot::aimdistance, path);
}
static void Load_Settings(LPCSTR path)
{
    visuals::box = GetPrivateProfileIntA((EX("Visuals")), (EX("m_playeresp")), visuals::box, path);
    visuals::boxMode = GetPrivateProfileIntA((EX("Visuals")), (EX("m_boxstyle")), visuals::boxMode, path);
    visuals::skel = GetPrivateProfileIntA((EX("Visuals")), (EX("m_skeleton")), visuals::skel, path);
    visuals::name = GetPrivateProfileIntA((EX("Visuals")), (EX("m_playername")), visuals::name, path);
    visuals::lines = GetPrivateProfileIntA((EX("Visuals")), (EX("m_snaplines")), visuals::lines, path);
    visuals::lineMode = GetPrivateProfileIntA((EX("Visuals")), (EX("m_linemode")), visuals::lineMode, path);
    visuals::team_check = GetPrivateProfileIntA((EX("Visuals")), (EX("m_teamid")), visuals::team_check, path);
    visuals::health = GetPrivateProfileIntA((EX("Visuals")), (EX("m_health")), visuals::health, path);
    visuals::radar = GetPrivateProfileIntA((EX("Visuals")), (EX("m_radar")), visuals::radar, path);
    visuals::radarstyle = GetPrivateProfileIntA((EX("Visuals")), (EX("m_radarstyle")), visuals::radarstyle, path);

    colors::invisible[0] = GetPrivateProfileFloat((EX("Colors")), (EX("m_invir")), colors::invisible[0], path);
    colors::invisible[1] = GetPrivateProfileFloat((EX("Colors")), (EX("m_invig")), colors::invisible[1], path);
    colors::invisible[2] = GetPrivateProfileFloat((EX("Colors")), (EX("m_invib")), colors::invisible[2], path);
    colors::visible[0] = GetPrivateProfileFloat((EX("Colors")), (EX("m_visibr")), colors::visible[0], path);
    colors::visible[1] = GetPrivateProfileFloat((EX("Colors")), (EX("m_visibg")), colors::visible[1], path);
    colors::visible[2] = GetPrivateProfileFloat((EX("Colors")), (EX("m_visibb")), colors::visible[2], path);

    aimbot::aimbot = GetPrivateProfileIntA((EX("Aimbot")), (EX("m_aimbot")), aimbot::aimbot, path);
    aimbot::smooth = GetPrivateProfileIntA((EX("Aimbot")), (EX("m_smooth")), aimbot::smooth, path);
    aimbot::aimfov = GetPrivateProfileFloat((EX("Aimbot")), (EX("m_aimbotfov")), aimbot::aimfov, path);
    aimbot::aimspeed = GetPrivateProfileFloat((EX("Aimbot")), (EX("m_aimbotsmooth")), aimbot::aimspeed, path);
    aimbot::fovcircle = GetPrivateProfileIntA((EX("Aimbot")), (EX("m_fovcircle")), aimbot::fovcircle, path);
    hotkeys::aimkey = GetPrivateProfileIntA((EX("Aimbot")), (EX("m_aimkey")), hotkeys::aimkey, path);
    aimbot::hitbox = GetPrivateProfileIntA((EX("Aimbot")), (EX("m_hitbox")), aimbot::hitbox, path);
    aimbot::aimdistance = GetPrivateProfileIntA((EX("Distance")), (EX("m_aimbotdistance")), aimbot::aimdistance, path);
};