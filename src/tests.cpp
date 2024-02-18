
#include "tests.hpp"
#include "system.hpp"

#include <unistd.h>
#include <iostream>

using namespace Sim;

void Tests::run()
{
	std::cout << "Load savefile.json:\n";
	System::load("savefile.json");
	std::cout << "Save savefile2.json:\n";
	System::save("savefile2.json");
	std::cout << "Load savefile2.json:\n";
	System::load("savefile2.json");
	std::cout << "Save savefile3.json:\n";
	System::save("savefile3.json");
}

