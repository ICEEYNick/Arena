#include "main.h"
#include "KeyAuth/auth.hpp"
#include "lazy.h"
#include "antidebug.hpp"

using namespace KeyAuth;

std::string name = XorStr("Arena Breakout");
std::string ownerid = XorStr("HiPYDPbwsa");
std::string secret = XorStr("dceb75c7693396209e11488265e73ec1ac9aab024b61fe865910bdd7307bd33e");
std::string version = XorStr("1.0");
std::string url = XorStr("https://keyauth.win/api/1.2/"); // change if you're self-hosting
std::string path = XorStr(""); //optional, set a path if you're using the token validation setting

api KeyAuthApp(name, ownerid, secret, version, url, path);

void SetConsoleSize()
{
	HANDLE hOut;
	SMALL_RECT DisplayArea = { 0, 0, 0, 0 };
	int x = 84;
	int y = 26;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DisplayArea.Right = x;
	DisplayArea.Bottom = y;

	SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);
}

void rndmTitle()
{
	constexpr int length = 15;
	const auto characters = TEXT("abcdefghi9182345jklmnopqrstuv211935960473wxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	TCHAR title[length + 1]{};

	for (int j = 0; j != length; j++)
	{
		title[j] += characters[rand() % 55 + 1];
	}

	SetConsoleTitle(title);
}

#include "Mapper/kdmapper.hpp"
#include "bytes.h"

HANDLE iqvw64e_device_handle;

LONG WINAPI SimplestCrashHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
	std::cout << EX("[+] Driver Error at 0x") << ExceptionInfo->ExceptionRecord->ExceptionAddress << EX(" by 0x") << std::hex << ExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;

	if (iqvw64e_device_handle)
		intel_driver::Unload(iqvw64e_device_handle);

	return EXCEPTION_EXECUTE_HANDLER;
}

bool callbackExample(ULONG64* param1, ULONG64* param2, ULONG64 allocationPtr, ULONG64 allocationSize, ULONG64 mdlptr) {
	UNREFERENCED_PARAMETER(param1);
	UNREFERENCED_PARAMETER(param2);
	UNREFERENCED_PARAMETER(allocationPtr);
	UNREFERENCED_PARAMETER(allocationSize);
	UNREFERENCED_PARAMETER(mdlptr);
	//Log("[+] Callback example called" << std::endl);

	/*
	This callback occurs before call driver entry and
	can be usefull to pass more customized params in
	the last step of the mapping procedure since you
	know now the mapping address and other things
	*/
	return true;
}

int load_driver()
{
	static HANDLE iqvw64e_device_handle = intel_driver::Load();
	if (!iqvw64e_device_handle || iqvw64e_device_handle == INVALID_HANDLE_VALUE)
	{
		cout << EX(" Couldn't create driver, make sure this program runs as administrator, all antivirus systems are turned off and anticheats such as vanguard are not running in the background.\nIf this error continues, restart your device.") << endl;
		Sleep(3000);
		exit(0);
	}

	kdmapper::AllocationMode mode = kdmapper::AllocationMode::AllocatePool;

	bool free = false;
	bool mdlMode = true;
	bool passAllocationPtr = false;

	std::vector<uint8_t> raw_image;
	for (int i = 0; i <= 30480; i++)
	{
		raw_image.push_back(rawData[i]);
	}

	NTSTATUS exitCode = 0;
	if (!kdmapper::MapDriver(iqvw64e_device_handle, raw_image.data(), 0, 0, free, true, mode, passAllocationPtr, callbackExample, &exitCode))
	{
		intel_driver::Unload(iqvw64e_device_handle);
		cout << EX(" Couldn't load Kernel mode driver, please only use supported windows versions.") << endl;
		Sleep(3000);
		exit(0);
	}

	intel_driver::Unload(iqvw64e_device_handle);
	system(EX("cls"));
}

void ADBG()
{
	while (true)
	{
		driverdetect();
	}
}

void ADBG2()
{
	while (true)
	{
		if (LI_FN(IsDebuggerPresent).safe_cached()())
			DeadScreen();
	}
}

void ADBG3()
{
	while (true)
	{
		if (HasRemoteDebugger())
			DeadScreen();
	}
}

HWND Entryhwnd;

auto main() -> int
{
	rndmTitle();
	SetConsoleSize();

	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &screenBufferInfo);
	COORD new_screen_buffer_size;
	new_screen_buffer_size.X = screenBufferInfo.srWindow.Right -
		screenBufferInfo.srWindow.Left + 1;
	new_screen_buffer_size.Y = screenBufferInfo.srWindow.Bottom -
		screenBufferInfo.srWindow.Top + 1;
	SetConsoleScreenBufferSize(hConsole, new_screen_buffer_size);

	KeyAuthApp.init();
	if (!KeyAuthApp.response.success)
	{
		Sleep(1000);
		exit(0);
	}

	rndmTitle();
	system(EX("cls"));

	KeyAuthApp.check();

	if (std::filesystem::exists(("C:\\ProgramData\\") + name))
	{
		Sleep(500);
		std::cout << EX("\n");
		Log2(EX("Verifying license key! Please wait...\n"));
		Sleep(1000);
		goto Load;
	}

	KeyAuthApp.check();
	Sleep(100);

Load:

	std::string key;

	if (std::filesystem::exists(("C:\\ProgramData\\") + name))
	{
		std::ifstream InFile(("C:\\ProgramData\\") + name, std::ios::in);
		std::getline(InFile, key);

		KeyAuthApp.license(key);

		if (!KeyAuthApp.response.success)
		{
			std::string del = ("C:\\ProgramData\\") + name;
			remove(del.c_str());
			goto A;
		}

	}
	else
	{
	A:
		std::cout << EX("\n");
		Log2(EX("Please enter your license key...\n"));

		bool authed = false;
		while (authed == false)
		{
			std::cin >> key;
			std::cout << EX("\n");
			Log4(EX("Verifying license key! Please wait...\n"));
			KeyAuthApp.license(key);

			if (KeyAuthApp.response.success)
			{
				system(EX("cls")); // Clear the console
				std::cout << EX("License key verified! Please start Game...\n"); // Print the new message
				std::ofstream OutFile(("C:\\ProgramData\\") + name, std::ios::out);
				OutFile << key;
				OutFile.close();
				authed = true;
			}
			else {
				std::cout << EX("\n");
				Sleep(100);
				Log4(EX("The license key provided is invalid!\n"));
				std::cout << EX("\n");
				goto A;
			}
		}
	}

	if (!KeyAuthApp.response.success)
	{
		Sleep(1500);
		exit(0);
	}

	if (GlobalFindAtomA(EX("DriverAlreadyLoadedx")) == 0)
	{
		load_driver();
		GlobalAddAtomA(EX("DriverAlreadyLoadedx"));
	}

	std::cout << EX("\n");
	Log3(EX("Press [ F5 ] once you are in Arena Breakout Infinite Lobby...\n"));
	std::cout << EX("\n");
	system(EX("pause >nul 2>&1"));
	Beep(325, 300);

	Sleep(500);

	while (Entryhwnd == NULL)
	{
		Entryhwnd = FindWindowA(0, EX("Arena Breakout Infinite  "));
		Sleep(1);
	}

	while (true)
	{
		if (GetAsyncKeyState(VK_F5))
		{
			break;
			Beep(300, 300);
		}
	}

	driver::initialize();
	MouseController::Init();
	globals::ProcID = find_process(EX("UAGame.exe"));
	driver::set_pid(globals::ProcID);
	globals::imagebase = driver::get_modulebase(L"UAGame.exe");

	setup_window();
	setup_directx(MyWnd);

	_beginthread(cache_entities, 0, nullptr);

	main_loop();
}