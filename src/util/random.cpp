
#include "random.hpp"

using namespace Sim::Util;

std::mt19937 Random::gen;

void Random::init()
{
	std::random_device rd;
	gen = std::mt19937(rd());
}

