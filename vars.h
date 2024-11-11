#pragma once



namespace aimbot
{
	bool aimbot = false;
	bool ignore_team = false;
	bool ignore_aiscavs = false;
	//bool rcs = true;
	bool forceauto = false;
	bool smooth = false;
	bool fovcircle = false;
	bool prediction = false;
	bool alonercs = false;
	bool vischeck = false;
	bool crosshair = false;
	bool disableondeath = false;

	int aimdistance = 300;
	int smoothscale = 25;
	int aimspeed = 0;
	int aimfov = 100;

	//int selected_weapon = 0;

	int hitbox = 0;

	uint64_t resultant_entity;
}
namespace hotkeys
{
	int aimkey;
	int airstuckey;
}

namespace player {
	bool isADS = false;
}

namespace visuals
{
	bool characterTypes[10] = { false }; // 10 types in ECharacterType
	bool factionTypes[12] = { false };   // 12 types in EFactionType
	bool corpse = false;
	bool box = false;
	bool player_view_angle = false;
	bool skel = false;
	bool car = false;
	bool health = false;
	bool checkdedson = false;
	//bool zombies = false;
	//bool animals = false;
	bool wepnhands = false;
	bool name = false;
	bool dbgname = false;
	bool outline = false;
	bool lines = false;
	bool knocked = false;
	bool team = false;
	bool team_name = false;
	bool show_head = false;
	bool health_based = false;
	bool sleeping_player = false;
	bool visable_player = false;
	bool nonvisable_player = false;

	bool team_check = false;

	//bool show_ore = false;
	///bool ore_stone = false;
	//bool ore_metal = false;
	//bool ore_sulfur = false;

	//bool show_collectable = false;
	//bool coll_vegetables = false;
	//bool coll_wood = false;
	//bool coll_stone = false;
	//bool coll_metal = false;
	//bool coll_sulfur = false;

	bool show_crates = false;
	bool crate_tools = false;
	bool crate_military = false;
	bool crate_normal = false;
	bool crate_barrel = false;
	bool crate_elite = false;

	bool show_droppeditems = false;
	bool show_scientists = false;

	static bool radar = false;
	static int radarstyle = 0;

	int boxMode = 0; // 0: Normal, 1: Filled, 2: Corner, 3: Filled Corner
	int nameMode = 0;
	int skelMode = 0;
	int animMode = 0;
	int itemMode = 0;
	int carMode = 0;
	int lineMode = 0; // 0: Bottom, 1: Top, 2: Center
	int healthMode = 0;

	int NumberofEntity = 0;
	int SpectatorCount = 0;

	int MaxDistance = 300;
	int MaxCrateDistance = 100;
	int MaxCollectableDistance = 50;
	int MaxOreDistance = 100;
	int MaxScientistDistance = 200;
	int MaxSkeletonDrawDistance = 300;
	int MaxZombDistance = 300;
	int MaxAnimalDistance = 300;

	Vector3 CurrentPosition = { 0, 0, 0 };
	Vector3 SavePosition1 = { 0, 0, 0 };

	bool itemtoolbox = false;
	bool itemweaponbox = false;
	bool itemgrenadebox = false;
	bool itemsportbox = false;
	bool itemmunitionbox = false;
}
namespace misc
{
	//bool speedhack = false;
	//bool superjump = false;
	//bool removegrass = false;
	//bool alwaysday = false;
	//bool overlayfps = false;

	//bool peter_parker = false;
	//bool admin_flag = false;
	bool norecoil = false;
	bool nospread = false;
	bool nosway = false;
	//bool notimeeoka = false;
	//bool airstuck = false;

	//float speedval = 2.0f;
	//float settime = 0.f;
	//float superheight = 1;
	//int superscale = 2;
}
namespace colors {

	static float box[3] = { 1.0f , 0.0f , 0.0f };
	float fov[4] = { 1.0f, 0.0f, 0.0f, 1.0f }; // Default red color with full opacity
	float crosshair[4] = { 1.0f, 0.0f, 0.0f, 1.0f }; // Default red color with full opacity
	static float skeleton[3] = { 1.0f , 0.0f , 0.0f };
	static float lines[3] = { 1.0f , 0.0f , 0.0f };

	static float invisible[3] = { 1.0f , 0.0f , 0.0f };
	static float visible[3] = { 1.0f , 0.0f , 0.0f };
}
namespace setting {

	static int settsMode = 0;
}

static const char* settsName[] =
{
	"Default",
	"Arena-Default1",
	"Arena-Default2"
};

static const char* Hitbox[] =
{
	"Head",
	"Neck",
	"Pelvis"
};
static const char* boxStyle[] =
{
	"2D",
	"2D Filled",
	"2D Corner",
	"2D Corner Filled",
};
static const char* healthMode[] =
{
	"Left Bar",
	"Right Bar",
	"Numbers",
	"Lbar + Numbers",
	"Rbar + Numbers",
	"Bottom Bar"
};
static const char* linesMode[] =
{
	"Bottom",
	"Top",
	"Center",
};
static const char* weaponsMode[] =
{
	"All",
	"Rifles",
	"Snipers",
	"Pistols",
	"Shotguns",
	"SMGs",
	"LMGs",
	"Bows"
};

static const char* RadarStyle[] =
{
	"Box",
	"Box Filled",
	"Circle",
	"Circle Filled"
};

static const char* keyNames[] =
{
	"",
	"Left Mouse",
	"Right Mouse",
	"Cancel",
	"Middle Mouse",
	"Mouse 5",
	"Mouse 4",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
};
static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}
static int keystatus = 0;
static int realkey = 0;
static bool GetKey(int key)
{
	realkey = key;
	return true;
}
static void ChangeKey(void* blank)
{
	keystatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				hotkeys::aimkey = i;
				keystatus = 0;
				return;
			}
		}
	}
}
void ChangeKey2(void* blank)
{
	keystatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				hotkeys::airstuckey = i;
				keystatus = 0;
				return;
			}
		}
	}
}
static void HotkeyButton(int aimkey, void* changekey, int status)
{
	const char* preview_value = NULL;
	if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, aimkey, &preview_value);

	std::string aimkeys;
	if (preview_value == NULL)
		aimkeys = EX("Select Key");
	else
		aimkeys = preview_value;

	if (status == 1)
	{
		aimkeys = EX("Press key to bind");
	}
	if (ImGui::Button(aimkeys.c_str(), ImVec2(125, 20)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}
static DWORD select_hitbox()
{
	int hitbox = 0;

	if (aimbot::hitbox == 0)
		hitbox = 16; //head
	else if (aimbot::hitbox == 1)
		hitbox = 15; //neck
	else if (aimbot::hitbox == 2)
		hitbox = 1; //chest
	else if (aimbot::hitbox == 3)
		hitbox = 1;
	else if (aimbot::hitbox == 4)
		hitbox = 1;

	return hitbox;
}