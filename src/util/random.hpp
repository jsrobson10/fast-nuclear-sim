
#pragma once

#include <random>

namespace Sim::Util::Random
{

extern std::mt19937 gen;

void init();

template <typename T>
T randint(T min, T max)
{
	return std::uniform_int_distribution<T>(min, max)(gen);
}

template <typename T>
T randint(T max)
{
	return std::uniform_int_distribution<T>(0, max)(gen);
}

template <typename T>
T rand(T min, T max)
{
	return std::uniform_real_distribution<T>(min, max)(gen);
}

template <typename T>
T rand(T max)
{
	return std::uniform_real_distribution<T>(0, max)(gen);
}

};

