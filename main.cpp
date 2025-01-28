#include "utilities\memory.h"
#include <string>
#include <iostream>
#include <vector>
#include <thread>

//Menu Colors
const std::string red = "\033[31m";
const std::string green = "\033[32m";
const std::string yellow = "\033[33m";
const std::string blue = "\033[34m";
const std::string magenta = "\033[35m";
const std::string cyan = "\033[36m";
const std::string white = "\033[37m";
const std::string reset = "\033[0m";

//logo
std::vector<std::string> logo{
	" _____      _           _   _       ",
	"/  __ \\    | |         | | (_)      ",
	"| /  \\/ ___| | ___  ___| |_ _  __ _ ",
	"| |    / _ \\ |/ _ \\/ __| __| |/ _` |",
	"| \\__/\\  __/ |  __/\\__ \\ |_| | (_| |",
	" \\____/\\___|_|\___||___/\\__|_|\\__,_|"
};

class NamePadding
{
public:
	char preNamePadding[549];
	char maxCharactersInName[16];
	char postNamePadding[3609];
};


//Offsets for 1.2.0.2 version.

constexpr auto localPlayer = 0x10F4F4;
constexpr auto health = 0xF8;
constexpr auto armor = 0xFC;
constexpr auto RifleAmmo = 0x150;
constexpr auto nameOffset = 0x0225;

int main()
{
	Memory memory{ "ac_client.exe" };
	const auto ModuleBaseAddress = memory.GetModuleAddress("ac_client.exe");

	//Derefrencing the pointer.
	const auto localPlayerAddress = memory.Read<std::uintptr_t>(ModuleBaseAddress + localPlayer);
	const auto HealthAddress = localPlayerAddress + health;
	const auto ArmorAddress = localPlayerAddress + armor;
	const auto RifleAddress = localPlayerAddress + RifleAmmo;
	const auto nameAddress = localPlayerAddress + nameOffset;

	std::string name = memory.Read<NamePadding>(nameAddress).preNamePadding;

	std::cout << std::endl;
	for (const auto line:logo)
	{
		std::cout << cyan << line << std::endl;
	}

	std::cout << std::endl << magenta << "=====================================" << reset << std::endl;


	std::cout << std::endl;
	std::cout << "Welcome to CELESTIA, " << blue << name << std::endl;
	std::cout << std::endl;
	std::cout << green << "F1: Health, " << std::endl;
	std::cout << green << "F2: Armor" << std::endl;
	std::cout << green << "F3: Ammo, "  << std::endl;


	while (true)
	{
		if (GetAsyncKeyState(VK_F1) & 0x8000) {
			memory.Write<int>(HealthAddress, 1000);
			std::cout << red <<"Health Hacked!" << std::endl;
		}
			
		else if (GetAsyncKeyState(VK_F2) & 0x8000)
		{
			memory.Write<int>(ArmorAddress, 1000);
			std::cout << red << "Armor Hacked!" << std::endl;
		}
			
		else if (GetAsyncKeyState(VK_F3) & 0x8000)
		{
			memory.Write<int>(RifleAddress, 1000);
			std::cout << red << "Rifle Ammo Hacked!" << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
			
	}




}