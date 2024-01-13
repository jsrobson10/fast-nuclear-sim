
#include "reactor.hpp"

#include <iostream>

#include <unistd.h>

int main()
{
	sim::reactor reactor(5, {100, 200});

//	react.set_reactivity(0);

	for(;;)
	{
		for(int i = 0; i < 1e3; i++)
		{
			reactor.update(1e-3);
		}

		std::cout << "\n\nShow:\n\n" << reactor << "\n";
	}

	return 0;
}

