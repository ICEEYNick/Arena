#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <memory>
#include <cmath>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

// Define necessary structures and classes
typedef struct _Entity {
    int type;
    uint64_t actor;
    uint64_t mesh;
    uint64_t bonearray;
    uint32_t object_id;
    uint64_t playerstate;
    std::string name;
} _Entity;

std::vector<_Entity> EntityList;
std::mutex entityMutex;

uint64_t camera_manager;

template<class T>
class TArrayDrink {
public:
    TArrayDrink() : Data(nullptr), Count(0), Max(0) {}

    T operator[](uint64_t i) const {
        return driver::read<uintptr_t>(((uintptr_t)Data) + i * sizeof(T));
    }

    T* Data;
    unsigned int Count;
    unsigned int Max;
};

static bool isVisible(DWORD_PTR mesh) {
    float fLastSubmitTime = driver::read<float>(mesh + 0x3d8 + 0x4);
    float fLastRenderTimeOnScreen = driver::read<float>(mesh + 0x3d8 + 0xC);
    const float fVisionTick = 0.06f;
    bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;
    return bVisible;
}

static auto RotatePoint(Vector3 pointToRotate, Vector3 centerPoint, int posX, int posY, int sizeX, int sizeY, float angle, bool* viewCheck, bool angleInRadians = false) -> Vector3 {
    float r_1, r_2;
    float x_1, y_1;

    r_1 = -(pointToRotate.y - centerPoint.y);
    r_2 = pointToRotate.x - centerPoint.x;

    float Yaw = angle - 90.0f;

    float yawToRadian = Yaw * (float)(M_PI / 180.0F);
    x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
    y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;

    *viewCheck = y_1 < 0;

    int sizX = sizeX / 2;
    int sizY = sizeY / 2;

    x_1 += sizX;
    y_1 += sizY;

    if (x_1 < 5)
        x_1 = 5;

    if (x_1 > sizeX - 5)
        x_1 = sizeX - 5;

    if (y_1 < 5)
        y_1 = 5;

    if (y_1 > sizeY - 5)
        y_1 = sizeY - 5;

    x_1 += posX;
    y_1 += posY;

    return Vector3(x_1, y_1, 0);
}

static FTransform GetBoneIndex(ULONG64 dwbonemesh, int indexnum) {
    uint64_t bonearray = driver::read<uint64_t>(dwbonemesh + 0x648);
    if (!bonearray)
        bonearray = driver::read<uint64_t>(dwbonemesh + 0x648 + 0x10);
    return driver::read<FTransform>(bonearray + (indexnum * 0x30));
}

static Vector3 GetBoneWithRotation(ULONG64 mesh, int index) {
    FTransform bone = GetBoneIndex(mesh, index);
    auto c2wptr = driver::read<uintptr_t>(mesh + 0x210);
    FTransform ComponentToWorld = driver::read<FTransform>(c2wptr);
    D3DMATRIX Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
    return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

void DrawBone(int64_t pMeshAddress, RGBA color, float thickness, FMinimalViewInfo X) {
    Vector3 vHeadBoneOut, vNeckOut, vPelvisOut;
    Vector3 vUpperArmLeftOut, vLeftHandOut, vlowerarmLeftOut;
    Vector3 vUpperArmRightOut, vLowerarmRightOut, vRightHandOut;
    Vector3 vLeftThighOut, vLeftCalfOut, vLeftLballOut, vLeftFootOut;
    Vector3 vRightThighOut, vRightCalfOut, vRightLballut, vRightFootOut;

    Vector3 vHeadBone = GetBoneWithRotation(pMeshAddress, 16);
    vHeadBoneOut = WorldToScreen(vHeadBone, X);
    Vector3 vNeck = GetBoneWithRotation(pMeshAddress, 15);
    vNeckOut = WorldToScreen(vNeck, X);
    Vector3 vPelvis = GetBoneWithRotation(pMeshAddress, 1);
    vPelvisOut = WorldToScreen(vPelvis, X);

    Vector3 vLeftUpperArm = GetBoneWithRotation(pMeshAddress, 51);
    vUpperArmLeftOut = WorldToScreen(vLeftUpperArm, X);
    Vector3 vLeftlowerarm = GetBoneWithRotation(pMeshAddress, 52);
    vlowerarmLeftOut = WorldToScreen(vLeftlowerarm, X);
    Vector3 vLeftHand = GetBoneWithRotation(pMeshAddress, 53);
    vLeftHandOut = WorldToScreen(vLeftHand, X);

    Vector3 vRightUpperArm = GetBoneWithRotation(pMeshAddress, 21);
    vUpperArmRightOut = WorldToScreen(vRightUpperArm, X);
    Vector3 vRightLowerarm = GetBoneWithRotation(pMeshAddress, 22);
    vLowerarmRightOut = WorldToScreen(vRightLowerarm, X);
    Vector3 vRightHand = GetBoneWithRotation(pMeshAddress, 23);
    vRightHandOut = WorldToScreen(vRightHand, X);

    Vector3 vLeftThigh = GetBoneWithRotation(pMeshAddress, 2);
    vLeftThighOut = WorldToScreen(vLeftThigh, X);
    Vector3 vLeftCalf = GetBoneWithRotation(pMeshAddress, 3);
    vLeftCalfOut = WorldToScreen(vLeftCalf, X);
    Vector3 vLeftFoot = GetBoneWithRotation(pMeshAddress, 4);
    vLeftFootOut = WorldToScreen(vLeftFoot, X);
    Vector3 vLeftLball = GetBoneWithRotation(pMeshAddress, 5);
    vLeftLballOut = WorldToScreen(vLeftLball, X);

    Vector3 vRightThigh = GetBoneWithRotation(pMeshAddress, 7);
    vRightThighOut = WorldToScreen(vRightThigh, X);
    Vector3 vRightCalf = GetBoneWithRotation(pMeshAddress, 8);
    vRightCalfOut = WorldToScreen(vRightCalf, X);
    Vector3 vRightFoot = GetBoneWithRotation(pMeshAddress, 9);
    vRightFootOut = WorldToScreen(vRightFoot, X);
    Vector3 vRightLball = GetBoneWithRotation(pMeshAddress, 10);
    vRightLballut = WorldToScreen(vRightLball, X);

    DrawLine(vNeckOut.x, vNeckOut.y, vHeadBoneOut.x, vHeadBoneOut.y, &color, thickness);
    DrawLine(vPelvisOut.x, vPelvisOut.y, vNeckOut.x, vNeckOut.y, &color, thickness);

    DrawLine(vUpperArmLeftOut.x, vUpperArmLeftOut.y, vNeckOut.x, vNeckOut.y, &color, thickness);
    DrawLine(vUpperArmRightOut.x, vUpperArmRightOut.y, vNeckOut.x, vNeckOut.y, &color, thickness);

    DrawLine(vlowerarmLeftOut.x, vlowerarmLeftOut.y, vUpperArmLeftOut.x, vUpperArmLeftOut.y, &color, thickness);
    DrawLine(vLowerarmRightOut.x, vLowerarmRightOut.y, vUpperArmRightOut.x, vUpperArmRightOut.y, &color, thickness);

    DrawLine(vLeftHandOut.x, vLeftHandOut.y, vlowerarmLeftOut.x, vlowerarmLeftOut.y, &color, thickness);
    DrawLine(vRightHandOut.x, vRightHandOut.y, vLowerarmRightOut.x, vLowerarmRightOut.y, &color, thickness);

    DrawLine(vLeftThighOut.x, vLeftThighOut.y, vPelvisOut.x, vPelvisOut.y, &color, thickness);
    DrawLine(vRightThighOut.x, vRightThighOut.y, vPelvisOut.x, vPelvisOut.y, &color, thickness);

    DrawLine(vLeftCalfOut.x, vLeftCalfOut.y, vLeftThighOut.x, vLeftThighOut.y, &color, thickness);
    DrawLine(vRightCalfOut.x, vRightCalfOut.y, vRightThighOut.x, vRightThighOut.y, &color, thickness);

    DrawLine(vLeftFootOut.x, vLeftFootOut.y, vLeftCalfOut.x, vLeftCalfOut.y, &color, thickness);
    DrawLine(vRightFootOut.x, vRightFootOut.y, vRightCalfOut.x, vRightCalfOut.y, &color, thickness);

    DrawLine(vLeftFootOut.x, vLeftFootOut.y, vLeftLballOut.x, vLeftLballOut.y, &color, thickness);
    DrawLine(vRightFootOut.x, vRightFootOut.y, vRightLballut.x, vRightLballut.y, &color, thickness);
}

int localplayerID;

uint64_t localpawn;
Vector3 localrelativelocation;
int local_team_id;
int local_team_id2;

inline const BYTE XOR_KEY = 0xACACACADDEDFFFA;

inline void EncryptFunctionBytes(BYTE* bytes, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        bytes[i] ^= XOR_KEY;
    }
}

inline void DecryptFunctionBytes(BYTE* bytes, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        bytes[i] ^= XOR_KEY;
    }
}

enum InjectedInputMouseOptions {
    Absolute = 32768,
    HWheel = 4096,
    LeftDown = 2,
    LeftUp = 4,
    MiddleDown = 32,
    MiddleUp = 64,
    Move = 1,
    MoveNoCoalesce = 8192,
    None = 0,
    RightDown = 8,
    RightUp = 16,
    VirtualDesk = 16384,
    Wheel = 2048,
    XDown = 128,
    XUp = 256
};

typedef struct _InjectedInputMouseInfo {
    int DeltaX;
    int DeltaY;
    unsigned int MouseData;
    InjectedInputMouseOptions MouseOptions;
    unsigned int TimeOffsetInMilliseconds;
    void* ExtraInfo;
} InjectedInputMouseInfo;

typedef bool (WINAPI* InjectMouseInput_t)(InjectedInputMouseInfo* inputs, int count);
inline InjectMouseInput_t InjectMouseInput;

namespace MouseController {
    static BYTE NtUserInjectMouseInput_Bytes[30];

    static BOOLEAN WINAPI Init() {
        LPVOID NtUserInjectMouseInput_Addr = GetProcAddress(GetModuleHandle("win32u"), "NtUserInjectMouseInput");
        if (!NtUserInjectMouseInput_Addr) {
            NtUserInjectMouseInput_Addr = GetProcAddress(GetModuleHandle("user32"), "NtUserInjectMouseInput");
            if (!NtUserInjectMouseInput_Addr) {
                NtUserInjectMouseInput_Addr = GetProcAddress(GetModuleHandle("user32"), "InjectMouseInput");
                if (!NtUserInjectMouseInput_Addr)
                    return FALSE;
            }
        }
        memcpy(NtUserInjectMouseInput_Bytes, NtUserInjectMouseInput_Addr, 30);
        EncryptFunctionBytes(NtUserInjectMouseInput_Bytes, 30);
        return TRUE;
    }

    static BOOLEAN WINAPI NtInjectMouse(InjectedInputMouseInfo* input, int count) {
        LPVOID NtUserInjectMouseInput_Spoof = VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (!NtUserInjectMouseInput_Spoof)
            return FALSE;

        DecryptFunctionBytes(NtUserInjectMouseInput_Bytes, 30);
        memcpy(NtUserInjectMouseInput_Spoof, NtUserInjectMouseInput_Bytes, 30);
        EncryptFunctionBytes(NtUserInjectMouseInput_Bytes, 30);

        NTSTATUS Result = reinterpret_cast<NTSTATUS(NTAPI*)(InjectedInputMouseInfo*, int)>(NtUserInjectMouseInput_Spoof)(input, count);
        ZeroMemory(NtUserInjectMouseInput_Spoof, 0x1000);
        VirtualFree(NtUserInjectMouseInput_Spoof, 0, MEM_RELEASE);
        return (Result > 0);
    }

    static BOOLEAN WINAPI NtInjectMouse(int X, int Y) {
        InjectedInputMouseInfo info = { 0 };
        info.DeltaX = X;
        info.DeltaY = Y;
        return NtInjectMouse(&info, 1);
    }
}

static double GetCrossDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

void SetMouseAbsPosition(DWORD x, DWORD y) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = x;
    input.mi.dy = y;
    SendInput(1, &input, sizeof(input));
}

static auto move_to(float x, float y) -> void {
    float center_x = (ImGui::GetIO().DisplaySize.x / 2);
    float center_y = (ImGui::GetIO().DisplaySize.y / 2);

    float smooth = aimbot::aimspeed;

    float target_x = 0.f;
    float target_y = 0.f;

    if (x != 0.f) {
        if (x > center_x) {
            target_x = -(center_x - x);
            target_x /= smooth;
            if (target_x + center_x > center_x * 2.f) target_x = 0.f;
        }

        if (x < center_x) {
            target_x = x - center_x;
            target_x /= smooth;
            if (target_x + center_x < 0.f) target_x = 0.f;
        }
    }

    if (y != 0.f) {
        if (y > center_y) {
            target_y = -(center_y - y);
            target_y /= smooth;
            if (target_y + center_y > center_y * 2.f) target_y = 0.f;
        }

        if (y < center_y) {
            target_y = y - center_y;
            target_y /= smooth;
            if (target_y + center_y < 0.f) target_y = 0.f;
        }
    }

    MouseController::NtInjectMouse((int)target_x, (int)(target_y));
}

static void fname_decrypt(char* inputBuf, int namelength) {
    const char xor_key = driver::read<BYTE>(globals::imagebase + 0xA1AAE08);
    __int64 v3;
    char v4;
    char result;

    if (namelength) {
        v3 = namelength;
        do {
            ++inputBuf;
            v4 = xor_key ^ ((unsigned __int8)xor_key >> 1) & 8 ^ (2
                * ((xor_key ^ ((unsigned __int8)xor_key >> 1) & 8) & 8));
            result = v4 ^ *(BYTE*)(inputBuf - 1) ^ ((unsigned __int8)(v4 & 0x10 ^ 0xE) >> 1);
            *(BYTE*)(inputBuf - 1) = result;
            --v3;
        } while (v3);
    }
}

std::unordered_map<uint32_t, std::string>* cached_fnames = nullptr;

std::string to_string2(uint32_t ComparisonIndex) {
    if (!cached_fnames) {
        return {};
    }

    const auto cached_name = cached_fnames->find(ComparisonIndex);
    if (cached_name != cached_fnames->end()) {
        return cached_name->second;
    }

    const auto index = static_cast<uint32_t>(static_cast<int32_t>(ComparisonIndex) >> 16);
    const auto name_offset = static_cast<uint16_t>(ComparisonIndex);
    const auto pool_chunk = driver::read<uint64_t>(globals::imagebase + 0xA1AB1C0 + (8 * index + 0x10));

    const auto entry_offset = pool_chunk + static_cast<uint32_t>(2 * name_offset);
    const auto name_entry = driver::read<int16_t>(entry_offset);

    const auto len = name_entry >> 6;
    char buff[1028]{ 0 };

    if (len > 0 && len < sizeof(buff)) {
        driver::read_memory(reinterpret_cast<void*>(entry_offset + 2), buff, len);
        buff[len] = '\0';

        fname_decrypt(buff, len);

        const std::string ret(buff);

        cached_fnames->emplace(ComparisonIndex, ret);

        return ret;
    } else {
        return {};
    }
}

void initialize_cache() {
    cached_fnames = new std::unordered_map<uint32_t, std::string>();
}

bool is_player(const std::string& name) {
    return (name == "BP_UamCharacter_C" || name == "BP_UamAICharacter_C");
}

int MyUniqueID;

void yawa() {
    std::vector<_Entity> EntityList_tmp;
    static uint64_t Gworld = 0, gameinstance = 0, persistlevel = 0, localplayer = 0, playercontroller = 0;

    Gworld = driver::read<uint64_t>(globals::imagebase + 0x9D830C8);
    gameinstance = driver::read<uint64_t>(Gworld + 0x180);
    persistlevel = driver::read<uint64_t>(Gworld + 0x30);
    localplayer = driver::read<uint64_t>(driver::read<uint64_t>(gameinstance + 0x38));
    playercontroller = driver::read<uint64_t>(localplayer + 0x30);

    localpawn = driver::read<uint64_t>(playercontroller + 0x390);
    camera_manager = driver::read<uint64_t>(playercontroller + 0x3A8);

    auto LocalPlayerStatte = driver::read<uintptr_t>(localpawn + 0x340);
    local_team_id = driver::read<int>(LocalPlayerStatte + 0x4D0);

    auto actor_array = driver::read<TArrayDrink<uint64_t>>(persistlevel + 0x98);

    auto MyHUD = driver::read<uint64_t>(playercontroller + 0x3a0);

    auto localrootcomponent = driver::read<uint64_t>(localpawn + 0x168);
    localrelativelocation = driver::read<Vector3>(localrootcomponent + 0x16C);

    for (uint32_t i = 0; i < actor_array.Count; ++i) {
        auto actor = actor_array[i];

        auto object_id = driver::read<int>(actor + 0x18);
        auto names = to_string2(object_id);

        if (is_player(names)) {
            auto mesh = driver::read<uint64_t>(actor + 0x380);

            if (!actor || !mesh || actor == localpawn)
                continue;

            _Entity currEntity{};
            currEntity.type = 0;
            currEntity.actor = actor;
            currEntity.mesh = mesh;
            currEntity.object_id = object_id;

            EntityList_tmp.push_back(currEntity);
        }
    }

    std::lock_guard<std::mutex> lock(entityMutex);
    EntityList = std::move(EntityList_tmp);
}

void __cdecl cache_entities(void*) {
    while (true) {
        yawa();
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

char* drawBuff = (char*)malloc(1024);
D3DXVECTOR4 Rect;

enum class ECharacterType : int8_t {
    ECharacterType_None = 0,
    ECharacterType_PMC = 1,
    ECharacterType_SCAV = 2,
    ECharacterType_AI_SCAV = 3,
    ECharacterType_AI_SCAV_BOSS = 4,
    ECharacterType_AI_PMC = 5,
    ECharacterType_AI_ELIT = 6,
    ECharacterType_BOSS = 7,
    ECharacterType_AI_SCAV_Follower = 8,
    ECharacterType_MAX = 9,
};

enum class EFactionType : int8_t {
    None = 0,
    NormalPMC = 1,
    NormalScav = 2,
    PlayerScav = 3,
    RebelFaction = 4,
    LakeFaction = 5,
    GangsterFaction = 6,
    KurtTeam = 7,
    NavyFaction = 8,
    Blackgold = 9,
    Gnesk = 10,
    EFactionType_MAX = 11,
};

std::string playernamestype(ECharacterType idboss) {
    if (idboss == ECharacterType::ECharacterType_None) return EX("[None]");
    if (idboss == ECharacterType::ECharacterType_PMC) return EX("[PMC]");
    if (idboss == ECharacterType::ECharacterType_SCAV) return EX("[SCAV]");
    if (idboss == ECharacterType::ECharacterType_AI_SCAV) return EX("[AI_SCAV]");
    if (idboss == ECharacterType::ECharacterType_AI_SCAV_BOSS) return EX("[AI_SCAV_BOSS]");
    if (idboss == ECharacterType::ECharacterType_AI_PMC) return EX("[AI_PMC]");
    if (idboss == ECharacterType::ECharacterType_AI_ELIT) return EX("[AI_ELIT]");
    if (idboss == ECharacterType::ECharacterType_BOSS) return EX("[BOSS]");
    if (idboss == ECharacterType::ECharacterType_AI_SCAV_Follower) return EX("[AI_SCAV_Followe]");
    if (idboss == ECharacterType::ECharacterType_MAX) return EX("[MAX]");
    return EX("[None]");
}

std::string boosnamesfaction(EFactionType idboss) {
    if (idboss == EFactionType::None) return EX("[None]");
    if (idboss == EFactionType::NormalPMC) return EX("[NormalPMC]");
    if (idboss == EFactionType::NormalScav) return EX("[NormalScav]");
    if (idboss == EFactionType::PlayerScav) return EX("[PlayerScav]");
    if (idboss == EFactionType::RebelFaction) return EX("[RebelFaction]");
    if (idboss == EFactionType::LakeFaction) return EX("[LakeFaction]");
    if (idboss == EFactionType::GangsterFaction) return EX("[GangsterFaction]");
    if (idboss == EFactionType::KurtTeam) return EX("[KurtTeam]");
    if (idboss == EFactionType::NavyFaction) return EX("[NavyFaction]");
    if (idboss == EFactionType::Blackgold) return EX("[Blackgold]");
    if (idboss == EFactionType::Gnesk) return EX("[Gnesk]");
    if (idboss == EFactionType::EFactionType_MAX) return EX("[MAX]");
    return EX("[None]");
}

int center_x = GetSystemMetrics(0) / 2 - 3;
int center_y = GetSystemMetrics(1) / 2 - 3;
static Vector3 pRadar;

void DrawRadar(Vector3 EntityPos, RGBA* color) {
    auto radar_posX = pRadar.x + 135;
    auto radar_posY = pRadar.y + 135;
    FCameraCacheEntry LastFrameCameraCachePrivate = driver::read<FCameraCacheEntry>(camera_manager + 0x1E90);
    auto Radar2D = WorldRadar(localrelativelocation, EntityPos, LastFrameCameraCachePrivate.POV.Rotation.y, radar_posX, radar_posY, 135.f);
    DrawCircleFilled2(Radar2D.x, Radar2D.y, 4, color);
}

void updateADSState() {
    // Example: Check the camera FOV to determine if the player is ADS
    float currentFOV = driver::read<float>(camera_manager + 0x1E90 + 0x10);
    player::isADS = (currentFOV < 60.0f); // Adjust the threshold as needed
}

void esp() {
    static float best_dist = FLT_MAX;
    static uint64_t closestPawn = NULL;
    static bool targetlocked = false;
    static bool isaimbotting = false;

    initialize_cache();

    updateADSState(); // Update the ADS state

    FCameraCacheEntry LastFrameCameraCachePrivate = driver::read<FCameraCacheEntry>(camera_manager + 0x1E90);

    uint64_t PawnMesh = driver::read<uint64_t>(localpawn + 0x380);
    Vector3 pLocalPosition = GetBoneWithRotation(PawnMesh, 16);
    if (_isnanf(pLocalPosition.x) || _isnanf(pLocalPosition.y) || _isnanf(pLocalPosition.z)) {
        pLocalPosition = { 0.f, 0.f, 0.f };
    }

    RGBA Skeleton = { colors::skeleton[0] * 255, colors::skeleton[1] * 255, colors::skeleton[2] * 255, 255 };
    RGBA Lines = { colors::lines[0] * 255, colors::lines[1] * 255, colors::lines[2] * 255, 255 };
    RGBA Box = { colors::box[0] * 255, colors::box[1] * 255, colors::box[2] * 255, 255 };

    bool Visible;

    std::lock_guard<std::mutex> lock(entityMutex);

    for (unsigned long i = 0; i < EntityList.size(); i++) {
        auto entity = EntityList[i];

        if (!entity.actor || !entity.mesh) continue;

        auto PlayerStatte = driver::read<uintptr_t>(entity.actor + 0x340);

        auto RootComponent = driver::read<uintptr_t>(entity.actor + 0x168);
        Vector3 RelativeLocation = driver::read<Vector3>(RootComponent + 0x16C);

        Vector3 head_position = GetBoneWithRotation(entity.mesh, 16);
        Vector3 root_position = GetBoneWithRotation(entity.mesh, 0);

        Vector3 yawa2222 = WorldToScreen(RelativeLocation, LastFrameCameraCachePrivate.POV);

        int team_id = driver::read<int>(PlayerStatte + 0x4D0);

        if (visuals::team_check) {
            if (team_id == local_team_id) {
                continue;
            }
        }

        if (head_position.z <= root_position.z) {
            continue;  // Skip entities with invalid bone positions
        }

        Vector3 player_screen = WorldToScreen(root_position, LastFrameCameraCachePrivate.POV);
        Vector3 head_screen = WorldToScreen(Vector3(head_position.x, head_position.y + 0.3, head_position.z), LastFrameCameraCachePrivate.POV);
        Vector3 normal_head = WorldToScreen(head_position, LastFrameCameraCachePrivate.POV);

        float distance = localrelativelocation.Distance(RelativeLocation) / 100.f;

        float BoxHeight = player_screen.y - normal_head.y;
        float BoxWidth = BoxHeight / 1.8f;

        float height = abs(normal_head.y - player_screen.y);
        float width = height * 0.65;

        auto dx = normal_head.x - (globals::width / 2);
        auto dy = normal_head.y - (globals::height / 2);
        float crosshair_dist = sqrtf((dx * dx) + (dy * dy));

        const auto DeadthComponent = driver::read<uint64_t>(entity.actor + 0x15B0);
        bool isdead = driver::read<bool>(DeadthComponent + 0x0240);
        if (entity.type == 0) {
            if (aimbot::aimbot) {
                Visible = isVisible(entity.mesh); // Check if the target is visible

                if (crosshair_dist < aimbot::aimfov && crosshair_dist < best_dist && targetlocked == false && (aimbot::vischeck ? Visible : true)) {

                    best_dist = crosshair_dist;
                    closestPawn = entity.mesh;
                }
            }

            if (distance < visuals::MaxSkeletonDrawDistance) {
                if (visuals::skel) {
                    DrawBone(entity.mesh, Skeleton, 2.f, LastFrameCameraCachePrivate.POV);
                }
            }

            if (distance < visuals::MaxDistance) {
                if (visuals::name) {
                    wchar_t pPlayerName[24] = { 0 };  // Initialize the array with zeros

                    // Read the player name from memory
                    uint64_t playerNamePtr = driver::read<uint64_t>(PlayerStatte + 0x3f0);
                    driver::read_memory((uint64_t*)playerNamePtr, (uint8_t*)pPlayerName, sizeof(pPlayerName));

                    char distance_text[256];

                    ECharacterType CachedCharacterType = driver::read<ECharacterType>(entity.actor + 0x13ED);
                    std::string nametypeplayers = playernamestype(CachedCharacterType);

                    const auto AIStaticsComponent = driver::read<uint64_t>(entity.actor + 0x1a40);
                    EFactionType AIFactionType = driver::read<EFactionType>(AIStaticsComponent + 0x109);
                    std::string namesbossfaction = boosnamesfaction(AIFactionType);
                    std::string playershit = pPlayerName ? nametypeplayers.c_str() : namesbossfaction.c_str();

                    if (pPlayerName[0] == L'\0')  // Check if the first character is NULL, indicating an empty string
                    {
                        sprintf_s(distance_text, EX("[%.fm] - %s"), distance, playershit.c_str());
                    }
                    else {
                        sprintf_s(distance_text, EX("[%.fm] - %S"), distance, pPlayerName);
                    }

                    // Calculate the size of the text to center it on the screen
                    ImVec2 text_size = ImGui::CalcTextSize(distance_text);

                    // Draw the text on the screen
                    DrawPlayerBar(normal_head.x - (text_size.x / 2) + 10, normal_head.y - 4, &Col.darkgray_, &Col.white_, distance_text);
                }

                if (visuals::box) {
                    if (visuals::boxMode == 0 || visuals::boxMode == 1) {
                        if (visuals::outline) {
                            DrawNormalBox(player_screen.x - BoxWidth / 2 + 1, head_screen.y, BoxWidth, BoxHeight, 2, &Col.black);
                            DrawNormalBox(player_screen.x - BoxWidth / 2 - 1, head_screen.y, BoxWidth, BoxHeight, 2, &Col.black);
                            DrawNormalBox(player_screen.x - BoxWidth / 2, head_screen.y + 1, BoxWidth, BoxHeight, 2, &Col.black);
                            DrawNormalBox(player_screen.x - BoxWidth / 2, head_screen.y - 1, BoxWidth, BoxHeight, 2, &Col.black);
                        }
                        DrawNormalBox(player_screen.x - (BoxWidth / 2), head_screen.y, BoxWidth, BoxHeight, 2, &Box);
                    }
                    if (visuals::boxMode == 2 || visuals::boxMode == 3) {
                        if (visuals::outline) {
                            DrawCornerBox(player_screen.x - BoxWidth / 2 + 1, head_screen.y, BoxWidth, BoxHeight, 2, &Col.black);
                            DrawCornerBox(player_screen.x - BoxWidth / 2 - 1, head_screen.y, BoxWidth, BoxHeight, 2, &Col.black);
                            DrawCornerBox(player_screen.x - BoxWidth / 2, head_screen.y + 1, BoxWidth, BoxHeight, 2, &Col.black);
                            DrawCornerBox(player_screen.x - BoxWidth / 2, head_screen.y - 1, BoxWidth, BoxHeight, 2, &Col.black);
                        }
                        DrawCornerBox(player_screen.x - (BoxWidth / 2), head_screen.y, BoxWidth, BoxHeight, 2, &Box);
                    }

                    if (visuals::boxMode == 1 || visuals::boxMode == 3) {
                        DrawFilledRect(player_screen.x - (BoxWidth / 2), head_screen.y, BoxWidth, BoxHeight, &Col.filled);
                    }
                }

                if (visuals::lines) {
                    if (visuals::lineMode == 0)
                        DrawLine(globals::width / 2, globals::height, player_screen.x, player_screen.y, &Lines, 1.5f);
                    if (visuals::lineMode == 1)
                        DrawLine(globals::width / 2, 0.f, player_screen.x, player_screen.y, &Lines, 1.5f);
                    if (visuals::lineMode == 2)
                        DrawLine(globals::width / 2, globals::height / 2, player_screen.x, player_screen.y, &Lines, 1.5f);
                }
            }
        }
    }

    if (closestPawn != 0) {
        if (aimbot::aimbot && closestPawn && GetAsyncKeyState(hotkeys::aimkey)) {
            targetlocked = true;

            auto root = GetBoneWithRotation(closestPawn, select_hitbox());
            Vector3 rootHeadOut = WorldToScreen(root, LastFrameCameraCachePrivate.POV);

            auto RootComponent = driver::read<uintptr_t>(closestPawn + 0x168);
            Vector3 RelativeLocation = driver::read<Vector3>(RootComponent + 0x16C);
            float Distance = localrelativelocation.Distance(RelativeLocation) / 100.f;

            // Check if the target is dead
            const auto DeadthComponent = driver::read<uint64_t>(closestPawn + 0x15B0);
            bool isdead = driver::read<bool>(DeadthComponent + 0x0240);

            if (isdead && aimbot::disableondeath) {
                best_dist = FLT_MAX;
                closestPawn = NULL;
                targetlocked = false;
                return;
            }

            if (rootHeadOut.x != 0 || rootHeadOut.y != 0) {
                if ((GetCrossDistance(rootHeadOut.x, rootHeadOut.y, globals::width / 2, globals::height / 2) <= aimbot::aimfov)) {
                    //if (Distance <= aimbot::aimdistance)
                    {
                        DrawLine(globals::width / 2, globals::height / 2, rootHeadOut.x, rootHeadOut.y, &Col.white_, 1.f);
                        move_to(rootHeadOut.x, rootHeadOut.y);
                    }
                }
            }
        }
        else {
            isaimbotting = false;
            targetlocked = false;

            auto root = GetBoneWithRotation(closestPawn, select_hitbox());
            Vector3 rootHeadOut = WorldToScreen(root, LastFrameCameraCachePrivate.POV);

            auto RootComponent = driver::read<uintptr_t>(closestPawn + 0x168);
            Vector3 RelativeLocation = driver::read<Vector3>(RootComponent + 0x16C);
            float Distance = localrelativelocation.Distance(RelativeLocation) / 100.f;

            if (rootHeadOut.x != 0 || rootHeadOut.y != 0) {
                if ((GetCrossDistance(rootHeadOut.x, rootHeadOut.y, globals::width / 2, globals::height / 2) <= aimbot::aimfov)) {
                    //if (Distance <= aimbot::aimdistance)
                    {
                        DrawLine(globals::width / 2, globals::height / 2, rootHeadOut.x, rootHeadOut.y, &Col.white_, 1.f);
                    }
                }
            }
            best_dist = FLT_MAX;
            closestPawn = NULL;
        }
    }
}

void __cdecl draw_esp(void*) {
    while (true) {
        esp();
    }
}

int main2() {
    // Initialize cache
    initialize_cache();

    // Create threads
    std::thread player_info_thread(cache_entities, nullptr);
    std::thread esp_thread(draw_esp, nullptr);

    // Wait for threads to finish
    player_info_thread.join();
    esp_thread.join();

    return 0;
}