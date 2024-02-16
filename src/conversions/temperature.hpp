
#pragma once

namespace Sim::Conversions::Temperature
{

constexpr double k_to_c(double k) { return k - 273.15; }
constexpr double c_to_k(double c) { return c + 273.15; }

};

