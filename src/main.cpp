
#include "fuel_rod.hpp"

#include <iostream>

#include <unistd.h>

int main()
{
	sim::fuel_rod fr(100, 200);

	fr.set_reactivity(0);

	for(;;)
	{
		for(int i = 0; i < 1e4; i++)
		{
			fr.update(1e-4);
		}

		std::cout << "\nFuel Rod:\n\n" << fr << "\n\n";
	}

	return 0;
}

