
#include "random.hpp"

using namespace sim;

std::mt19937 random::gen;

void random::init()
{
	std::random_device rd;
	gen = std::mt19937(rd());
}
