#pragma once
// Icon Solution for Includes
#include <Windows.h>
#include <iostream>
#include <fcntl.h>
#include <thread>
#include <random>
#include <vector>
#include <string>
#include <io.h>

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
int num = 5, num2 = 3;

const char* Title()
{
	/*_setmode(_fileno(stdout), _O_U8TEXT);
	std::wcout << LR"(

				██████╗ ███████╗    ███████╗███████╗██╗     ██╗     
				██╔══██╗██╔════╝    ██╔════╝██╔════╝██║     ██║     
				██████╔╝█████╗█████╗███████╗█████╗  ██║     ██║     
				██╔══██╗██╔══╝╚════╝╚════██║██╔══╝  ██║     ██║     
				██║  ██║███████╗    ███████║███████╗███████╗███████╗
				╚═╝  ╚═╝╚══════╝    ╚══════╝╚══════╝╚══════╝╚══════╝
                                                    

                                                      
                                                      
    )";*/
	_setmode(_fileno(stdout), _O_TEXT);
	std::cout << EX("                             <DBD Ready>") << '\n' << '\n';
	return "";
}

const char* SetWhite()
{
	SetConsoleTextAttribute(h, 7);
	return "";
}

const char* SetGreen()
{
	SetConsoleTextAttribute(h, 10);
	return "";
}

const char* SetBlue()
{
	SetConsoleTextAttribute(h, 3);
	return "";
}

const char* SetGold()
{
	SetConsoleTextAttribute(h, 6);
	return "";
}

const char* SetRed()
{
	SetConsoleTextAttribute(h, 4);
	return "";
}

void Log(const char* text)
{
	SetGreen();
	std::cout << EX(">> ");
	SetWhite();
	std::cout << text;
}

void Log2(const char* text)
{
	SetGold();
	std::cout << EX(">> ");
	SetWhite();
	std::cout << text;
}

void Log3(const char* text)
{
	SetBlue();
	std::cout << EX(">> ");
	SetWhite();
	std::cout << text;
}

void Log4(const char* text)
{
	SetRed();
	std::cout << EX(">> ");
	SetWhite();
	std::cout << text;
}

void Log5(const char* text)
{
	SetRed();
	std::cout << EX(">> ");
	SetWhite();
	std::cout << text;
}

void Opti(const char* text)
{
	SetBlue();
	std::cout << EX(" 1} ");
	SetWhite();
	std::cout << text << std::endl;
}

void Opt2(const char* text)
{
	SetBlue();
	std::cout << EX(" 2} ");
	SetWhite();
	std::cout << text << std::endl;
}

void CountDown()
{
	std::cout << EX("\n");
	while (num > -1)
	{
		SetBlue();
		std::cout << EX("\r>> ");
		SetWhite();
		std::cout << EX("Closing in ");

		SetBlue();
		std::cout << num;
		SetWhite();

		std::cout << EX(" seconds! Please start your game...");
		Sleep(1000);
		num--;
	}
}

void Green(const char* text)
{
	SetGreen();
	std::cout << text << std::endl;
	SetWhite();
}

void Blue(const char* text)
{
	SetBlue();
	std::cout << text << std::endl;
	SetWhite();
}

void Red(const char* text)
{
	SetRed();
	std::cout << text << std::endl;
	SetWhite();
}

std::string tm_to_readable_time(tm ctx)
{
	char buffer[80];

	strftime(buffer, sizeof(buffer), EX("%m/%d/%y"), &ctx);

	return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp)
{
	auto cv = strtol(timestamp.c_str(), NULL, 10);

	return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp)
{
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}